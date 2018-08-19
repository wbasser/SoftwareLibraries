/******************************************************************************
 * @file Uart.c
 *
 * @brief UART implementation
 *
 * This file provides the implemntation for the UART peripherals
 *
 * @copyright Copyright (c) 2012 Cyber Integration
 * This document contains proprietary data and information of Cyber Integration
 * LLC. It is the exclusive property of Cyber Integration, LLC and will not be
 * disclosed in any form to any party without prior written permission of
 * Cyber Integration, LLC. This document may not be reproduced or further used
 * without the prior written permission of Cyber Integration, LLC.
 *
 * Version History
 * ======
 * $Log: $
 * 
 *
 * \addtogroup UART
 * @{
 *****************************************************************************/

// library includes -----------------------------------------------------------
#include "Clock/Clock.h"
#include "Interrupt/Interrupt.h"

// local includes -------------------------------------------------------------
#include "UART/Uart.h"

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
static  USART_TypeDef*  GetUsartChannel( UARTCHAN eChan );
static  U16             ComputeBaudrate( PUARTDEF ptDef );
static  void            IrqCommonHandler( UARTDEVENUM eDev );

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
  PUARTDEF        ptDef;
  PLCLCTL         ptCtl;
  USART_TypeDef*  ptUsart;
  U16             wBaudRate;
  UARTDEVENUM     eDev;
  U32             uTemp;
  
  // for each defined UART
  for ( eDev = 0; eDev < UART_DEV_ENUM_MAX; eDev++ )
  {
    // get pointer to the definition/control structures
    ptDef = ( PUARTDEF )&atUartDefs[ eDev ];
    ptCtl = &atLclCtls[ eDev ];

    // clear the receive/transmit buffers
    ptCtl->tRxBuf.wWrIdx = ptCtl->tRxBuf.wRdIdx = ptCtl->tRxBuf.wCount = 0;
    ptCtl->tTxBuf.wWrIdx = ptCtl->tTxBuf.wRdIdx = ptCtl->tTxBuf.wCount = 0;
    
    // get the pointer to the USART channel
    ptUsart = GetUsartChannel( ptDef->eChan );
    
    // now set the registers/CR3/CR2
    ptUsart->BRR = ComputeBaudrate( ptDef );
    ptUsart->CR3 = 0;
    uTemp = BITS( ptDef->eStopBit, USART_CR2_STOP_Pos );
    uTemp |= BITS( ptDef->eDataOrder, USART_CR2_MSBFIRST_Pos );
    ptUsart->CR2 = uTemp;
    
    // register the interrupt
    switch( ptDef->eChan )
    {
      case UART_CHAN_1 :
        uTemp = USART1_IRQn;
        break;
        
      case UART_CHAN_2 :
        uTemp = USART2_IRQn;
        break;

      case UART_CHAN_3 :
        uTemp = USART3_IRQn;
        break;

      case UART_CHAN_4 :
        uTemp = UART4_IRQn;
        break;

      case UART_CHAN_5 :
        uTemp = UART5_IRQn;
        break;
        
      default :
        uTemp = 0;
        break;
    }
    NVIC_EnableIRQ( uTemp );
    
    // now setup CR1
    uTemp = USART_CR1_RE | USART_CR1_TE | USART_CR1_RXNEIE;
    uTemp |= BITS( ptDef->eParity, USART_CR1_PS_Pos );
    switch( ptDef->eWrdLen )
    {
      case UART_WRDLEN_7 :
        uTemp |= USART_CR1_M1;
        break;

      case UART_WRDLEN_9 :
        uTemp |= USART_CR1_M0;
        break;

      default :
        break;
    }
    uTemp |= ( ptDef->eSamp == UART_SAMP_8X ) ? USART_CR1_OVER8 : 0;
    uTemp |= USART_CR1_UE;
    ptUsart->CR1 = uTemp;
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
  UARTERR         eError = UART_ERR_NONE;
  PUARTDEF        ptDef;
  PLCLCTL         ptCtl;
  U16             wBytesRemaining, wIdx;
  USART_TypeDef*  ptUsart;
 
  // check for a valid UART
  if ( eDev < UART_DEV_ENUM_MAX )
  {
    // set the bytes written to zero
    *pwBytesWritten = 0;

    // get pointers to the control/def structure
    ptDef = ( PUARTDEF )&atUartDefs[ eDev ];
    ptCtl = &atLclCtls[ eDev ];
    
    // get the pointer to the USART channel
    ptUsart = GetUsartChannel( ptDef->eChan );

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

    // enable the data register enmpty interrupt
    ptUsart->CR1 |= ( USART_CR1_TXEIE | USART_CR1_TCIE );

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
    ptDef = ( PUARTDEF )&atUartDefs[ eDev ];
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
  UARTERR eError = UART_ERR_NONE;
  
  // return the error
  return( eError );
}

/******************************************************************************
 * @function USART1_Handler
 *
 * @brief interrupt USART 1 handler
 *
 * This function handles the interrupts from USART 1
 *
 *****************************************************************************/
void USART1_IRQHandler( void )
{
  // call the common handler
  IrqCommonHandler( aePhyToDefMap[ UART_CHAN_1 ] );
}

/******************************************************************************
 * @function USART2_Handler
 *
 * @brief interrupt USART 2 handler
 *
 * This function handles the interrupts from USART 2
 *
 *****************************************************************************/
void USART2_IRQHandler( void )
{
  // call the common handler
  IrqCommonHandler( aePhyToDefMap[ UART_CHAN_2 ] );
}

/******************************************************************************
 * @function USART3_Handler
 *
 * @brief interrupt USART 3 handler
 *
 * This function handles the interrupts from USART 3
 *
 *****************************************************************************/
void USART3_IRQHandler( void )
{
  // call the common handler
  IrqCommonHandler( aePhyToDefMap[ UART_CHAN_3 ] );
}

/******************************************************************************
 * @function UART4_Handler
 *
 * @brief interrupt UART 4 handler
 *
 * This function handles the interrupts from UART 4
 *
 *****************************************************************************/
void UART4_IRQHandler( void )
{
  // call the common handler
  IrqCommonHandler( aePhyToDefMap[ UART_CHAN_4 ] );
}

/******************************************************************************
 * @function UART5_Handler
 *
 * @brief interrupt UART 5 handler
 *
 * This function handles the interrupts from UART 5
 *
 *****************************************************************************/
void UART5_IRQHandler( void )
{
  // call the common handler
  IrqCommonHandler( aePhyToDefMap[ UART_CHAN_5 ] );
}

/******************************************************************************
 * @function IrqCommandHandler
 *
 * @brief common interrupt handler
 *
 * This function checks for the type of interrupts and processes them
 * appropriately
 *
 * @param[in]   eDev        Device enumeration
 *
 *****************************************************************************/
static void IrqCommonHandler( UARTDEVENUM eDev )
{
  PUARTDEF            ptDef;
  PLCLCTL             ptCtl;
  USART_TypeDef*      ptUsart;
  U8                  nOption;
  PVUARTINTCALLBACK   pvCallBack;
  U32                 uIntFlags;
  UARTIRQEVENTS       eEvent = UART_IRQ_EVENT_NONE;
  
  // get pointer to the definition/control structures
  ptDef = ( PUARTDEF )&atUartDefs[ eDev ];
  ptCtl = &atLclCtls[ eDev ];
  
  // get a pointer to the UART registers
  ptUsart = GetUsartChannel( ptDef->eChan );

  // get the callback
  pvCallBack = ptDef->pvCallback;
  
  // get the interrupt cause
  uIntFlags = ptUsart->ISR;

  // was this a RX interrupt
  if ( uIntFlags & USART_ISR_RXNE )
  {
    // get the data
    nOption = ( U8 )ptUsart->RDR;

    // check for an error
    if ( uIntFlags & USART_ISR_ORE )
    {
      // clear the error flag
      ptUsart->ICR = USART_ICR_ORECF;
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
  if ( uIntFlags & USART_ISR_TXE )
  {
    // check for characters available
    if ( ptCtl->tTxBuf.wCount != 0 )
    {
      // output the character
      nOption = *( ptDef->pnTxBuffer + ptCtl->tTxBuf.wRdIdx++ );
      ptUsart->TDR = nOption;
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
      ptUsart->CR1 &= ~USART_CR1_TXEIE;
    }
  }  
    
  // check for a transmit complete interrupt
  if ( uIntFlags & USART_ISR_TC )
  {
    // clear the interrupt
    ptUsart->ICR = USART_ICR_TCCF;
    
    // if data is done, perform the callback
    if ( ptCtl->tTxBuf.wCount == 0 )
    {
      if (( pvCallBack  != NULL ) && ( ptDef->tEventFlags.bTxCmp ))
      {
        // call back with the appropriate TX event
        eEvent = UART_IRQ_EVENT_TXCMP;
        nOption = 0;
        pvCallBack( eEvent, nOption );

        // turn off the transmit complete interrupt
        ptUsart->CR1 &= ~USART_CR1_TCIE;
      }
    }
  }
}

/******************************************************************************
 * @function GetUsartChannel
 *
 * @brief gets a pointer to the control structure
 *
 * This function will return a pointer to the correct control structure
 * for a given channel
 *
 * @param[in]   eChan			USART channel
 *
 * @return      pointer to a USART or NULL if illegal channel
 *
 *****************************************************************************/
static USART_TypeDef* GetUsartChannel( UARTCHAN eChan )
{
  USART_TypeDef* ptUsart = NULL;
  
  switch( eChan )
  {
    case UART_CHAN_1 :
      ptUsart = USART1;
      break;
    
    case UART_CHAN_2:
      ptUsart = USART2;
      break;
    
    case UART_CHAN_3 :
      ptUsart = USART3;
      break;
    
    case UART_CHAN_4 :
      ptUsart = UART4;
      break;
    
    case UART_CHAN_5 :
      ptUsart = UART5;
      break;
    
    default :
      break;
  }
  
  // return the pointer to the channlel
  return( ptUsart );
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
  if ( ptDef->eChan == UART_CHAN_1 )
  {
    uPeripheralClock = Clock_GetAPB2Frequency( );
  }
  else
  {
    uPeripheralClock = Clock_GetAPB1Frequency( );
  }
  
  // check for baud rate outside of normal range
  wBaudRate = uPeripheralClock / ptDef->uBaudRate;
  
  // return the baud rate
  return( wBaudRate );
}

/**@} EOF Uart.c */
