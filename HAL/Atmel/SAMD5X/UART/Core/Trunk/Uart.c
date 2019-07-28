/******************************************************************************
 * @file Uart.c
 *
 * @brief UART implementation
 *
 * This file provides the implemntation for the UART peripherals
 *
 * @copyright Copyright (c) 2012 CyberIntegration
 * This document contains proprietary data and information of CyberIntegration
 * LLC. It is the exclusive property of CyberIntegration, LLC and will not be
 * disclosed in any form to any party without prior written permission of
 * CyberIntegration, LLC. This document may not be reproduced or further used
 * without the prior written permission of CyberIntegration, LLC.
 *
 * Version History
 * ======
 * $Log: $
 * 
 *
 * \addtogroup UART
 * @{
 *****************************************************************************/

// local includes -------------------------------------------------------------
#include "UART/Uart.h"

// library includes -----------------------------------------------------------
#include "Clock/Clock.h"
#include "Interrupt/Interrupt.h"

// structures -----------------------------------------------------------------
/// define the buffer control structure
typedef struct _BUFCTL
{
  U16 wWrIdx;     ///< write index
  U16 wRdIdx;     ///< read index
  U16 wCount;     ///< buffer count
} BUFCTL, *PBUFCTL;
#define BUFCTL_SIZE   sizeof( BUFCTL )

/// define the local control structure
typedef struct _LCLCTL
{
  BUFCTL            tRxBuf;       ///< receive buffer control
  BUFCTL            tTxBuf;       ///< transmit buffer control
} LCLCTL, *PLCLCTL;
#define LCLCTL_SIZE   sizeof( LCLCTL )

// local parameter declarations -----------------------------------------------
static  UARTDEVENUM aePhyToDefMap[ UART_CHAN_MAX ];
static  LCLCTL      atLclCtls[ UART_DEV_ENUM_MAX ];

// local function prototypes --------------------------------------------------
static  SercomUsart*  GetSercomChannel( UARTCHAN eChan );
static  U16           ComputeBaudrate( PUARTDEF ptDef );
static  void          Irq0CommonHandler( UARTDEVENUM eDev );
static  void          Irq1CommonHandler( UARTDEVENUM eDev );
static  void          Irq2CommonHandler( UARTDEVENUM eDev );
static  void          Irq3CommonHandler( UARTDEVENUM eDev );

/******************************************************************************
 * @function Uart_Initialize
 *
 * @brief UART initialization
 *
 * This function will initialize a given device 
 *
 *****************************************************************************/
void Uart_Initialize( void )
{
  PUARTDEF                    ptDef;
  PLCLCTL                     ptCtl;
  SercomUsart*                ptUsart;
  U16                         wBaudRate;
  SERCOM_USART_CTRLA_Type     tCtrlA;
  SERCOM_USART_CTRLB_Type     tCtrlB;
  SERCOM_USART_INTENSET_Type  tIntSet;
  UARTDEVENUM                 eDev;
  
  // for each defined UART
  for ( eDev = 0; eDev < UART_DEV_ENUM_MAX; eDev++ )
  {
    // get pointer to the definition/control structures
    ptDef = ( PUARTDEF )&g_atUartDefs[ eDev ];
    ptCtl = &atLclCtls[ eDev ];

    // clear the receive/transmit buffers
    ptCtl->tRxBuf.wWrIdx = ptCtl->tRxBuf.wRdIdx = ptCtl->tRxBuf.wCount = 0;
    ptCtl->tTxBuf.wWrIdx = ptCtl->tTxBuf.wRdIdx = ptCtl->tTxBuf.wCount = 0;
    
    // get the pointer to the USART channel
    ptUsart = GetSercomChannel( ptDef->eChan );

    // clear the two registers
    tCtrlA.reg = 0;
    tCtrlB.reg = 0;
    
    // disable the control adn wait for done
    ptUsart->CTRLA.bit.SWRST = 1;
    while( ptUsart->SYNCBUSY.bit.SWRST );
    
    // now compute the baud rate
    if (( wBaudRate = ComputeBaudrate( ptDef )) != 0 )
    {
      // valid baud rate
      ptUsart->BAUD.reg = wBaudRate;
      
      // now set the modes
      switch( ptDef->eMode )
      {
        case UART_MODE_ASYNCEXT :
          tCtrlA.bit.MODE = 0;
          tCtrlA.bit.CMODE = 0;
          break;
          
        case UART_MODE_ASYNCINT :
          tCtrlA.bit.MODE = 1;
          tCtrlA.bit.CMODE = 0;
          break;
          
        case UART_MODE_SYNCEXT :
          tCtrlA.bit.MODE = 1;
          tCtrlA.bit.CMODE = 1;
          break;
          
        case UART_MODE_SYNCINT :
          tCtrlA.bit.MODE = 1;
          tCtrlA.bit.CMODE = 1;
          break;
          
        default :
          tCtrlA.bit.MODE = 0;
          tCtrlA.bit.CMODE = 0;
          break;
      }
      
      // set the run in standby/sample rate/TX pin/RX pin
      tCtrlA.bit.RUNSTDBY = ptDef->bRunStandby;
      tCtrlA.bit.SAMPR = ptDef->eSamp;
      tCtrlA.bit.TXPO = ptDef->eTxdPad;
      tCtrlA.bit.RXPO = ptDef->eRxdPad;
      tCtrlA.bit.DORD = UART_DATAORDER_LSB;
      
      // set the frame/parity
      tCtrlA.bit.FORM = ( ptDef->eParity != UART_PARITY_NONE ) ? 1 : 0;
      
      // set the word length/stop bits/parity/TX enable/RX enable
      switch( ptDef->eWrdLen )
      {
        case UART_WRDLEN_5 : 
          tCtrlB.reg |= SERCOM_USART_CTRLB_CHSIZE( 5 );
          break;
          
        case UART_WRDLEN_6 :
          tCtrlB.reg |= SERCOM_USART_CTRLB_CHSIZE( 6 );
          break;
          
        case UART_WRDLEN_7 :
          tCtrlB.reg |= SERCOM_USART_CTRLB_CHSIZE( 7 );
          break;
          
        case UART_WRDLEN_8 :
          tCtrlB.bit.CHSIZE = 0;
          break;
          
        case UART_WRDLEN_9 :
          tCtrlB.bit.CHSIZE = 1;
          break;
          
        default :
          tCtrlB.bit.CHSIZE = 0;
          break;
      }
      tCtrlB.bit.SBMODE = ptDef->eStopBit;
      tCtrlB.bit.PMODE = ( ptDef->eParity == UART_PARITY_ODD ) ? ON : 0;
      tCtrlB.bit.RXEN = ON;
      tCtrlB.bit.TXEN = ON;
      
      // now write CTRLA/CTRLB
      ptUsart->CTRLA = tCtrlA;
      ptUsart->CTRLB = tCtrlB;
      while( ptUsart->SYNCBUSY.bit.CTRLB & SERCOM_USART_SYNCBUSY_CTRLB );

      // set up the interrupts
      aePhyToDefMap[ ptDef->eChan ] = eDev;
      tIntSet.bit.TXC = ON;
      tIntSet.bit.RXC = ON;
      tIntSet.bit.RXBRK = ON;
      tIntSet.bit.ERROR = ON;
      
      // set the interrupts
      ptUsart->INTENSET = tIntSet;

      // enable the interrupt in the NVIC      
      NVIC_EnableIRQ( SERCOM0_0_IRQn + ( ptDef->eChan * 4 ));

      // now enable the device
      ptUsart->CTRLA.reg |= SERCOM_USART_CTRLA_ENABLE;
      while( ptUsart->SYNCBUSY.bit.CTRLB & SERCOM_USART_SYNCBUSY_ENABLE );
    }
  }
}

/******************************************************************************
 * @function Uart_CloseAll
 *
 * @brief UART close all devices
 *
 * This function will close all defined devices
 *
 *****************************************************************************/
void Uart_CloseAll( void )
{
  PUARTDEF                    ptDef;
  SercomUsart*                ptUsart;
  UARTDEVENUM                 eDev;
  
  // for each defined UART
  for ( eDev = 0; eDev < UART_DEV_ENUM_MAX; eDev++ )
  {
    // get pointer to the definition/control structures
    ptDef = ( PUARTDEF )&g_atUartDefs[ eDev ];

    // get the pointer to the USART channel
    ptUsart = GetSercomChannel( ptDef->eChan );

    // disable the control adn wait for done
    ptUsart->CTRLA.bit.SWRST = 1;
    while( ptUsart->SYNCBUSY.bit.SWRST );
    
    // enable the interrupt in the NVIC
    NVIC_DisableIRQ( SERCOM0_0_IRQn + ( ptDef->eChan * 4 ));
  }
}

/******************************************************************************
 * @function Uart_Write
 *
 * @brief write some characters to the UART
 *
 * This function will write some characters to the UART buffer if room
 *
 * @param[in]   eDev        Device enumeration
 * @param[in]   pnData      pointer to the data buffer
 * @param[in]   wLength     size of the data buffer
 * @param[i0]   pwBytesWritten  the storage to return the number of bytes written
 *
 * @return      appropriate error value
 *
 *****************************************************************************/
UARTERR Uart_Write( UARTDEVENUM eDev, PU8 pnData, U16 wLength, PU16 pwBytesWritten )
{
  UARTERR             eError = UART_ERR_NONE;
  PUARTDEF            ptDef;
  PLCLCTL             ptCtl;
  U16                 wBytesRemaining, wIdx;
  SercomUsart*        ptUsart;
  PVUARTINTCALLBACK   pvCallback;
 
  // check for a valid UART
  if ( eDev < UART_DEV_ENUM_MAX )
  {
    // set the bytes written to zero
    *pwBytesWritten = 0;

    // get pointers to the control/def structure
    ptDef = ( PUARTDEF )&g_atUartDefs[ eDev ];
    ptCtl = &atLclCtls[ eDev ];
    
    // get the pointer to the USART channel
    ptUsart = GetSercomChannel( ptDef->eChan );

    // compute the bytes written
    wBytesRemaining = ptDef->wTxBufSize - ptCtl->tTxBuf.wCount;
    *pwBytesWritten = MIN( wBytesRemaining, wLength );
    
    // for each byte
    Interrupt_Disable( );
    for ( wIdx = 0; wIdx < *pwBytesWritten; wIdx++ )
    {
      // put in buffer/adjust pointer/increment count
      *( ptDef->pnTxBuffer + ptCtl->tTxBuf.wWrIdx++ ) = *( pnData + wIdx );
      ptCtl->tTxBuf.wWrIdx %= ptDef->wTxBufSize;
      ptCtl->tTxBuf.wCount++;
    }

    // check for function handler/
    if ((( pvCallback = ptDef->pvCallback ) != NULL ) && ( ptDef->tEventFlags.bTxEnb ))
    {
      // call the callback to enable the transmit 
      pvCallback( UART_IRQ_EVENT_TXENB, 0 ); 
    }

    // enable the data register enmpty interrupt
    ptUsart->INTENSET.reg = BIT( SERCOM_USART_INTENSET_DRE_Pos );

    // enable the interrupt
    Interrupt_Enable( );
  }
  else
  {
    // illegal device
    eError = UART_ERR_ILLDEV;
  }
  
  // return the error
  return( eError );
}

/******************************************************************************
 * @function Uart_Read
 *
 * @brief read some characters from the UART
 *
 * This function will read bytes from the buffer
 *
 * @param[in]   eDev        Device enumeration
 * @param[in]   pnData      pointer to the data buffer
 * @param[in]   wLength     size of the data buffer
 * @param[i0]   pwBytesRead the storage to return the number of bytes written
 *
 * @return      appropriate error value
 *
 *****************************************************************************/
UARTERR Uart_Read( UARTDEVENUM eDev, PU8 pnData, U16 wLength, PU16 pwBytesRead )
{
  UARTERR   eError = UART_ERR_NONE;
  PUARTDEF  ptDef;
  PLCLCTL   ptCtl;
  U16       wBytesAvailable, wIdx;
  
  // check for a valid UART
  if ( eDev < UART_DEV_ENUM_MAX )
  {
    // get pointers to the control/def structure
    ptDef = ( PUARTDEF )&g_atUartDefs[ eDev ];
    ptCtl = &atLclCtls[ eDev ];
    
    // compute the bytes written
    wBytesAvailable = ptDef->wRxBufSize - ptCtl->tRxBuf.wCount;
    *pwBytesRead = MIN( wBytesAvailable, wLength );
    
    // for each byte
    Interrupt_Disable( );
    for ( wIdx = 0; wIdx < *pwBytesRead; wIdx++ )
    {
      // put in buffer/adjust pointer/increment count
      *( pnData + wIdx ) = *( ptDef->pnRxBuffer + ptCtl->tRxBuf.wRdIdx++ );
      ptCtl->tRxBuf.wRdIdx %= ptDef->wRxBufSize;
      ptCtl->tRxBuf.wCount--;
    }
    Interrupt_Enable( );
  }
  else
  {
    // illegal device
    eError = UART_ERR_ILLDEV;
  }
  
  // return the error
  return( eError );
}

/******************************************************************************
 * @function Uart_Ioctl
 *
 * @brief UART IOCTL functions
 *
 * This function provides functionality to modify the UARTs parameters
 *
 * @param[in]   eDev        Device enumeration
 * @param[in]   eAction     action to take
 * @param[io]   pvData      pointer to data storage/retrieval
 *
 * @return      appropriate error value
 *
 *****************************************************************************/
UARTERR Uart_Ioctl( UARTDEVENUM eDev, UARTACTION eAction, PVOID pvData )
{
  UARTERR eError = UART_ERR_NONE;
  
  // check for a valid UART
  if ( eDev < UART_DEV_ENUM_MAX )
  {
  }
  else
  {
    // illegal device
    eError = UART_ERR_ILLDEV;
  }
  
  // return the error
  return( eError );
}

/******************************************************************************
 * @function Uart_Close
 *
 * @brief Close the UART
 *
 * This function will disable any interrupt, unregister the interrupt handler
 *
 * @param[in]   eDev        Device enumeration
 *
 * @return      appropriate error value
 *
 *****************************************************************************/
UARTERR Uart_Close( UARTDEVENUM eDev )
{
  UARTERR       eError = UART_ERR_NONE;
  SercomUsart*  ptUsart;
  
  // check for a valid UART
  if ( eDev < UART_DEV_ENUM_MAX )
  {
    // get the pointer to the USART channel
    ptUsart = GetSercomChannel( g_atUartDefs[ eDev ].eChan );

    // disable the USART
    ptUsart->CTRLA.reg &= ~SERCOM_USART_CTRLA_ENABLE;
    while( ptUsart->SYNCBUSY.reg & SERCOM_USART_SYNCBUSY_ENABLE );
  }
  else
  {
    // illegal device
    eError = UART_ERR_ILLDEV;
  }
  
  // return the error
  return( eError );
}

#ifdef SERCOM0
#if ( UART_CHAN0_IN_USE == 1 )
/******************************************************************************
 * @function SERCOM0_Handler
 *
 * @brief interrupt SERCOM 0 handler
 *
 * This function handles the interrupts from SERCOM 0
 *
 *****************************************************************************/
void SERCOM0_Handler( void )
{
  // call the common handler
  IrqCommonHandler( aePhyToDefMap[ UART_CHAN_0 ] );
}
#endif // UART_CHAN0_IN_USE
#endif // SERCOM0

#ifdef SERCOM1
#if ( UART_CHAN1_IN_USE == 1 )
/******************************************************************************
 * @function SERCOM1_Handler
 *
 * @brief interrupt SERCOM 1 handler
 *
 * This function handles the interrupts from SERCOM 1
 *
 *****************************************************************************/
void SERCOM1_Handler( void )
{
  // call the common handler
  IrqCommonHandler( aePhyToDefMap[ UART_CHAN_1 ] );
}
#endif // UART_CHAN1_IN_USE
#endif // SERCOM1

#ifdef SERCOM2
#if ( UART_CHAN2_IN_USE == 1 )
/******************************************************************************
 * @function SERCOM2_Handler
 *
 * @brief interrupt SERCOM 2 handler
 *
 * This function handles the interrupts from SERCOM 2
 *
 *****************************************************************************/
void SERCOM2_Handler( void )
{
  // call the common handler
  IrqCommonHandler( aePhyToDefMap[ UART_CHAN_2 ] );
}
#endif // UART_CHAN2_IN_USE
#endif // SERCOM2

#ifdef SERCOM3
#if ( UART_CHAN3_IN_USE == 1 )
/******************************************************************************
 * @function SERCOM3_Handler
 *
 * @brief interrupt SERCOM 3 handler
 *
 * This function handles the interrupts from SERCOM 3
 *
 *****************************************************************************/
void SERCOM3_0_Handler( void )
{
  // call the common handler
  Irq0CommonHandler( aePhyToDefMap[ UART_CHAN_3 ] );
}
void SERCOM3_1_Handler( void )
{
  // call the common handler
  Irq1CommonHandler( aePhyToDefMap[ UART_CHAN_3 ] );
}
void SERCOM3_2_Handler( void )
{
  // call the common handler
  Irq2CommonHandler( aePhyToDefMap[ UART_CHAN_3 ] );
}
void SERCOM3_3_Handler( void )
{
  // call the common handler
  Irq3CommonHandler( aePhyToDefMap[ UART_CHAN_3 ] );
}
#endif // UART_CHAN3_IN_USE
#endif // SERCOM3

#ifdef SERCOM4
#if ( UART_CHAN4_IN_USE == 1 )
/******************************************************************************
 * @function SERCOM4_Handler
 *
 * @brief interrupt SERCOM 4 handler
 *
 * This function handles the interrupts from SERCOM 4
 *
 *****************************************************************************/
void SERCOM4_0_Handler( void )
{
  // call the common handler
  Irq0CommonHandler( aePhyToDefMap[ UART_CHAN_4 ] );
}
void SERCOM4_1_Handler( void )
{
  // call the common handler
  Irq1CommonHandler( aePhyToDefMap[ UART_CHAN_4 ] );
}
void SERCOM4_2_Handler( void )
{
  // call the common handler
  Irq2CommonHandler( aePhyToDefMap[ UART_CHAN_4 ] );
}
void SERCOM4_3_Handler( void )
{
  // call the common handler
  Irq3CommonHandler( aePhyToDefMap[ UART_CHAN_4 ] );
}
#endif // UART_CHAN4_IN_USE
#endif // SERCOM4

#ifdef SERCOM5
#if ( UART_CHAN5_IN_USE == 1 )
/******************************************************************************
 * @function SERCOM5_Handler
 *
 * @brief interrupt SERCOM 5 handler
 *
 * This function handles the interrupts from SERCOM 5
 *
 *****************************************************************************/
void SERCOM5_Handler( void )
{
  // call the common handler
  IrqCommonHandler( aePhyToDefMap[ UART_CHAN_5 ] );
}
#endif // UART_CHAN5_IN_USE
#endif // SERCOM5

/******************************************************************************
 * @function Irq0CommandHandler
 *
 * @brief common interrupt handler
 *
 * This function checks for the type of interrupts and processes them
 * appropriately
 *
 * @param[in]   eDev        Device enumeration
 *
 *****************************************************************************/
static void Irq0CommonHandler( UARTDEVENUM eDev )
{
  PUARTDEF                  ptDef;
  PLCLCTL                   ptCtl;
  U8                        nOption;
  PVUARTINTCALLBACK         pvCallBack;
  SercomUsart*              ptUsart;
  U32                       uIntFlags;
  UARTIRQEVENTS             eEvent = UART_IRQ_EVENT_NONE;
  
  // get pointer to the definition/control structures
  ptDef = ( PUARTDEF )&g_atUartDefs[ eDev ];
  ptCtl = &atLclCtls[ eDev ];
  
  // get a pointer to the UART registers
  ptUsart = GetSercomChannel( ptDef->eChan );

  // get the callback
  pvCallBack = ptDef->pvCallback;
  
  // get the interrupt cause
  uIntFlags = ptUsart->INTFLAG.reg;

  // check for a data register empty interrupt
  if ( uIntFlags & SERCOM_USART_INTFLAG_DRE )
  {
    // clear the interrupt
    ptUsart->INTFLAG.reg |= SERCOM_USART_INTFLAG_DRE;
    
    // check for characters available
    if ( ptCtl->tTxBuf.wCount != 0 )
    {
      // output the character
      nOption = *( ptDef->pnTxBuffer + ptCtl->tTxBuf.wRdIdx++ );
      ptUsart->DATA.reg = nOption;
      ptCtl->tTxBuf.wRdIdx %= ptDef->wTxBufSize;
      ptCtl->tTxBuf.wCount--;
        
      // check for callback
      if (( pvCallBack != NULL ) && ( ptDef->tEventFlags.bTxEmp ))
      {
        // call back with the appropriate TX event
        eEvent = UART_IRQ_EVENT_TXEMP;
        nOption = 0;
        pvCallBack( eEvent, nOption );
      }
    }
    else
    {
      // turn off the data register empty interrupt
      ptUsart->INTENCLR.reg = BIT( SERCOM_USART_INTENSET_DRE_Pos );
    }
  }  
    
}

/******************************************************************************
 * @function Irq1CommandHandler
 *
 * @brief common interrupt handler
 *
 * This function checks for the type of interrupts and processes them
 * appropriately
 *
 * @param[in]   eDev        Device enumeration
 *
 *****************************************************************************/
static void Irq1CommonHandler( UARTDEVENUM eDev )
{
  PUARTDEF                  ptDef;
  PLCLCTL                   ptCtl;
  U8                        nOption;
  PVUARTINTCALLBACK         pvCallBack;
  SercomUsart*              ptUsart;
  U32                       uIntFlags;
  UARTIRQEVENTS             eEvent = UART_IRQ_EVENT_NONE;
  
  // get pointer to the definition/control structures
  ptDef = ( PUARTDEF )&g_atUartDefs[ eDev ];
  ptCtl = &atLclCtls[ eDev ];
  
  // get a pointer to the UART registers
  ptUsart = GetSercomChannel( ptDef->eChan );

  // get the callback
  pvCallBack = ptDef->pvCallback;
  
  // get the interrupt cause
  uIntFlags = ptUsart->INTFLAG.reg;

  // check for a transmit complete interrupt
  if ( uIntFlags & SERCOM_USART_INTFLAG_TXC )
  {
    // clear the interrupt
    ptUsart->INTFLAG.reg |= SERCOM_USART_INTFLAG_TXC;
    
    // if data is done, perform the callback
    if ( ptCtl->tTxBuf.wCount == 0 )
    {
      if (( pvCallBack  != NULL ) && ( ptDef->tEventFlags.bTxCmp ))
      {
        // call back with the appropriate TX event
        eEvent = UART_IRQ_EVENT_TXCMP;
        nOption = 0;
        pvCallBack( eEvent, nOption );
      }
    }
  }
    
}

/******************************************************************************
 * @function Irq2CommandHandler
 *
 * @brief common interrupt handler
 *
 * This function checks for the type of interrupts and processes them
 * appropriately
 *
 * @param[in]   eDev        Device enumeration
 *
 *****************************************************************************/
static void Irq2CommonHandler( UARTDEVENUM eDev )
{
  PUARTDEF                  ptDef;
  PLCLCTL                   ptCtl;
  U8                        nOption;
  PVUARTINTCALLBACK         pvCallBack;
  SercomUsart*              ptUsart;
  U32                       uIntFlags;
  UARTIRQEVENTS             eEvent = UART_IRQ_EVENT_NONE;
  
  // get pointer to the definition/control structures
  ptDef = ( PUARTDEF )&g_atUartDefs[ eDev ];
  ptCtl = &atLclCtls[ eDev ];
  
  // get a pointer to the UART registers
  ptUsart = GetSercomChannel( ptDef->eChan );

  // get the callback
  pvCallBack = ptDef->pvCallback;
  
  // get the interrupt cause
  uIntFlags = ptUsart->INTFLAG.reg;

  // was this a RX interrupt
  if ( uIntFlags & SERCOM_USART_INTFLAG_RXC )
  {
    // clear the flag
    ptUsart->INTFLAG.reg |= SERCOM_USART_INTFLAG_RXC;

    // get the data
    nOption = ( U8 )ptUsart->DATA.reg;

    // check for an error
    if ( uIntFlags & SERCOM_USART_INTFLAG_ERROR )
    {
      // clear the error flag
      ptUsart->INTFLAG.reg |= SERCOM_USART_INTFLAG_ERROR;
    }
    else
    {
      // check for callback
      if ( ptDef->tEventFlags.bRxChar )
      {
        // if callback process it
        if (( pvCallBack != NULL ) & ( ptDef->tEventFlags.bRxChar ))
        {
          // call the callback with a receive char event
          eEvent = UART_IRQ_EVENT_RXCHAR;
          pvCallBack( eEvent, nOption );
        }
        // check for room in local buffer
        else if ( ptCtl->tRxBuf.wCount < ptDef->wRxBufSize )
        {
          // stuff in buffer/adust index/increment count
          *( ptDef->pnRxBuffer + ptCtl->tRxBuf.wWrIdx++ ) = nOption;
          ptCtl->tRxBuf.wWrIdx %= ptDef->wRxBufSize;
          ptCtl->tRxBuf.wCount++;
        }
      }
    }
  }
}

/******************************************************************************
 * @function Irq3CommandHandler
 *
 * @brief common interrupt handler
 *
 * This function checks for the type of interrupts and processes them
 * appropriately
 *
 * @param[in]   eDev        Device enumeration
 *
 *****************************************************************************/
static void Irq3CommonHandler( UARTDEVENUM eDev )
{
  PUARTDEF                  ptDef;
  PLCLCTL                   ptCtl;
  U8                        nOption;
  PVUARTINTCALLBACK         pvCallBack;
  SercomUsart*              ptUsart;
  U32                       uIntFlags;
  UARTIRQEVENTS             eEvent = UART_IRQ_EVENT_NONE;
  
  // get pointer to the definition/control structures
  ptDef = ( PUARTDEF )&g_atUartDefs[ eDev ];
  ptCtl = &atLclCtls[ eDev ];
  
  // get a pointer to the UART registers
  ptUsart = GetSercomChannel( ptDef->eChan );

  // get the callback
  pvCallBack = ptDef->pvCallback;
  
  // get the interrupt cause
  uIntFlags = ptUsart->INTFLAG.reg;

  // was this a RX interrupt
  if ( uIntFlags & SERCOM_USART_INTFLAG_RXC )
  {
    // clear the flag
    ptUsart->INTFLAG.reg |= SERCOM_USART_INTFLAG_RXC;

    // get the data
    nOption = ( U8 )ptUsart->DATA.reg;

    // check for an error
    if ( uIntFlags & SERCOM_USART_INTFLAG_ERROR )
    {
      // clear the error flag
      ptUsart->INTFLAG.reg |= SERCOM_USART_INTFLAG_ERROR;
    }
    else
    {
      // check for callback
      if ( ptDef->tEventFlags.bRxChar )
      {
        // if callback process it
        if (( pvCallBack != NULL ) & ( ptDef->tEventFlags.bRxChar ))
        {
          // call the callback with a receive char event
          eEvent = UART_IRQ_EVENT_RXCHAR;
          pvCallBack( eEvent, nOption );
        }
        // check for room in local buffer
        else if ( ptCtl->tRxBuf.wCount < ptDef->wRxBufSize )
        {
          // stuff in buffer/adust index/increment count
          *( ptDef->pnRxBuffer + ptCtl->tRxBuf.wWrIdx++ ) = nOption;
          ptCtl->tRxBuf.wWrIdx %= ptDef->wRxBufSize;
          ptCtl->tRxBuf.wCount++;
        }
      }
    }
  }
  
  // check for a data register empty interrupt
  if ( uIntFlags & SERCOM_USART_INTFLAG_DRE )
  {
    // clear the interrupt
    ptUsart->INTFLAG.reg |= SERCOM_USART_INTFLAG_DRE;
    
    // check for characters available
    if ( ptCtl->tTxBuf.wCount != 0 )
    {
      // output the character
      nOption = *( ptDef->pnTxBuffer + ptCtl->tTxBuf.wRdIdx++ );
      ptUsart->DATA.reg = nOption;
      ptCtl->tTxBuf.wRdIdx %= ptDef->wTxBufSize;
      ptCtl->tTxBuf.wCount--;
        
      // check for callback
      if (( pvCallBack != NULL ) && ( ptDef->tEventFlags.bTxEmp ))
      {
        // call back with the appropriate TX event
        eEvent = UART_IRQ_EVENT_TXEMP;
        nOption = 0;
        pvCallBack( eEvent, nOption );
      }
    }
    else
    {
      // turn off the data register empty interrupt
      ptUsart->INTENCLR.reg = BIT( SERCOM_USART_INTENSET_DRE_Pos );
    }
  }  
    
  // check for a transmit complete interrupt
  if ( uIntFlags & SERCOM_USART_INTFLAG_TXC )
  {
    // clear the interrupt
    ptUsart->INTFLAG.reg |= SERCOM_USART_INTFLAG_TXC;
    
    // if data is done, perform the callback
    if ( ptCtl->tTxBuf.wCount == 0 )
    {
      if (( pvCallBack  != NULL ) && ( ptDef->tEventFlags.bTxCmp ))
      {
        // call back with the appropriate TX event
        eEvent = UART_IRQ_EVENT_TXCMP;
        nOption = 0;
        pvCallBack( eEvent, nOption );
      }
    }
  }
    
  // check for break detect interrupt
  if ( uIntFlags & SERCOM_USART_INTFLAG_RXBRK )
  {
    // clear the interrupt
    ptUsart->INTFLAG.reg |= SERCOM_USART_INTFLAG_RXBRK;
    
    // if callback and enabled, do it
    if (( pvCallBack != NULL ) && ( ptDef->tEventFlags.bBrkDet ))
    {
      // set the event
      eEvent = UART_IRQ_EVENT_BRKDET;
      nOption = 0;
      pvCallBack( eEvent, nOption );
    }
  }
}
/******************************************************************************
 * @function GetSercomChannel
 *
 * @brief gets a pointer to the control structure
 *
 * This function will return a pointer to the correct control structure
 * for a given channel
 *
 * @param[in]   eChan			SERCOM channel
 *
 * @return      pointer to a SercomUsart or NULL if illegal channel
 *
 *****************************************************************************/
static SercomUsart* GetSercomChannel( UARTCHAN eChan )
{
  Sercom* ptSercom = NULL;
  
  switch( eChan )
  {
    case UART_CHAN_0 :
      ptSercom = SERCOM0;
      break;
      
    case UART_CHAN_1 :
      ptSercom = SERCOM1;
      break;
    
    #ifdef SERCOM2
    case UART_CHAN_2:
      ptSercom = SERCOM2;
      break;
    #endif // SERCOM2
    
    #ifdef SERCOM3
    case UART_CHAN_3 :
      ptSercom = SERCOM3;
      break;
    #endif // SERCOM3
    
    #ifdef SERCOM4
    case UART_CHAN_4 :
      ptSercom = SERCOM4;
      break;
    #endif // SERCOM4
    
    #ifdef SERCOM5
    case UART_CHAN_5 :
      ptSercom = SERCOM5;
      break;
    #endif // SERCOM5
    
    default :
      break;
  }
  
  // return the pointer to the channlel
  return( &ptSercom->USART );
}

/******************************************************************************
 * @function ComputeBaudRate
 *
 * @brief computes the step and divisor for a given baudrate
 *
 * This function will determine the best step and divisor for a given
 * baud rate
 *
 * @param[in]   uBaudrate   desired baudrate
 *
 * @return      baud rate value, 0 if not available
 *
 *****************************************************************************/
static U16 ComputeBaudrate( PUARTDEF ptDef )
{
  U32   uPeripheralClock, uBaudIntermediate;
  U8    nSampleRate, nFracIdx;
  BOOL  bFractional;
  U64   hScale, hRatio;
  U16   wBaudRate = 0;
  
  // get the system clock value
  uPeripheralClock = Clock_GetMultiplexerFreq( CLOCK_MUXID_SERCOM_0 + ptDef->eChan );
  
  // get the sample rate
  switch( ptDef->eSamp )
  {
    case UART_SAMP_16XARITH :
      nSampleRate = 16;
      bFractional = FALSE;
      break;
      
    case UART_SAMP_16XFRAC :
      nSampleRate = 16;
      bFractional = TRUE;
      break;
      
    case UART_SAMP_8XARITH :
      nSampleRate = 8;
      bFractional = FALSE;
      break;
      
    case UART_SAMP_8XFRAC :
      nSampleRate = 8;
      bFractional = TRUE;
      break;
      
    case UART_SAMP_3XARITH :
      nSampleRate = 3;
      bFractional = TRUE;
      break;
      
    default :
      nSampleRate = 1;
      bFractional = FALSE;
      break;
  }
  
  // check for baud rate outside of normal range
  if (( ptDef->uBaudRate * nSampleRate ) <= uPeripheralClock )
  {
    // check for fractional
    if ( bFractional )
    {
      // for each fractional value
      for( nFracIdx = 0; nFracIdx < 8; nFracIdx++ )
      {
        uBaudIntermediate = 8 * ( U64 )uPeripheralClock / (( U64 )ptDef->uBaudRate * nSampleRate ) - nFracIdx;
        uBaudIntermediate /= 8;
      
        // check for done
        if ( uBaudIntermediate < ( SERCOM_USART_BAUD_FRAC_BAUD_Msk + 1 ))
        {
          // exit for loop
          break;
        }
      }
      
      // check for error
      if ( nFracIdx < 8 )
      {
        // calculate the baud rate
        wBaudRate = uBaudIntermediate | ( nFracIdx << SERCOM_USART_BAUD_FRAC_FP_Pos );
      }
    }
    else
    {
      // calculate the baud rate
      hRatio = (( nSampleRate * ( U64 )ptDef->uBaudRate ) << 32 ) / uPeripheralClock;
      hScale = (( U64 )1 << 32 ) - hRatio;
      wBaudRate = ( 65536 * hScale ) >> 32;
    }
  }
  
  // return the baud rate
  return( wBaudRate );
}

/**@} EOF Uart.c */
