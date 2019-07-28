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
#include "GPIO/Gpio.h"
#include "Interrupt/Interrupt.h"
#include "PowerManager/PowerManager.h"

// macros and defines ---------------------------------------------------------
/// define the macro to map the control pointer
#define MAP_HANDLE_TO_POINTER( handle )   (( PLCLCTL )handle )

// structures -----------------------------------------------------------------
/// define the buffer control structure
typedef struct _BUFCTL
{
  U16 wWrIdx;     ///< write index
  U16 wRdIdx;     ///< read index
  U16 wCount;     ///< buffer count
  U16 wSize;      ///< size
  PU8 pnBuffer;   ///< pointer to the buffer
} BUFCTL, *PBUFCTL;
#define BUFCTL_SIZE   sizeof( BUFCTL )

/// define the local control structure
typedef struct _LCLCTL
{
  struct _LCLCTL*   ptSignature;  ///< signature
  #if (SYSTEMDEFINE_OS_SELECTION != SYSTEMDDEFINE_OS_TASKMINIMAL )
  BUFCTL            tRxBuf;       ///< receive buffer control
  #endif // SYSTEMDEFINE_OS_SELECTON != SYSTEMDEFINE_OS_TASKMINIMAL
  BUFCTL            tTxBuf;       ///< transmit buffer control
  SercomUsart*      ptUsart;      ///< USART
  PVUARTINTCALLBACK pvCallback;   ///< pointer to the callback
  USARTEVENTFLAGS   tEventFlags;  ///< event flags
} LCLCTL, *PLCLCTL;
#define LCLCTL_SIZE   sizeof( LCLCTL )

// local parameter declarations -----------------------------------------------
static  PLCLCTL     aptLclCtls[ UART_CHAN_MAX ];

// local function prototypes --------------------------------------------------
static  SercomUsart*  GetSercomChannel( UARTCHAN eChan );
static  U16           ComputeBaudrate( PUARTDEF ptDef );
static  void          IrqCommonHandler( PLCLCTL ptCtl );

/******************************************************************************
 * @function Uart_Configure
 *
 * @brief UART configure
 *
 * This function will initialize a given device 
 *
 * @param[in]   ptDef     pointer to the definition structure
 *
 *****************************************************************************/
PTUARTHANDLE Uart_Configure( PUARTDEF ptDef )
{
  U16           wBaudRate;
  U32           uTemp;
  PLCLCTL       ptCtl = NULL;
  
  // set the pointer
  if (( ptCtl = malloc( LCLCTL_SIZE )) != NULL )
  {
    // store it
    aptLclCtls[ ptDef->eChan ] = ptCtl;
    ptCtl->ptSignature = ptCtl;

    // allocate the buffers
    if (( ptCtl->tTxBuf.pnBuffer = malloc( ptCtl->tTxBuf.wSize )) != NULL )
    {
      #if (SYSTEMDEFINE_OS_SELECTION != SYSTEMDDEFINE_OS_TASKMINIMAL )
      if (( ptCtl->tRxBuf.pnBuffer = malloc( ptCtl->tRxBuf.wSize )) != NULL )
      #endif // SYSTEMDEFINE_OS_SELECTON != SYSTEMDEFINE_OS_TASKMINIMAL
      {
        // now compute the baud rate
        if (( wBaudRate = ComputeBaudrate( ptDef )) != 0 )
        {
          // clear the receive/transmit buffers
          ptCtl->tTxBuf.wWrIdx = ptCtl->tTxBuf.wCount = 0;
          ptCtl->tTxBuf.wSize = ptDef->wTxBufSize;
          #if (SYSTEMDEFINE_OS_SELECTION != SYSTEMDDEFINE_OS_TASKMINIMAL )
          ptCtl->tTxBuf.wRdIdx = 0;
          ptCtl->tRxBuf.wWrIdx = ptCtl->tRxBuf.wRdIdx = ptCtl->tRxBuf.wCount = 0;
          ptCtl->tRxBuf.wSize = ptDef->wRxBufSize;
          #endif // SYSTEMDEFINE_OS_SELECTON != SYSTEMDEFINE_OS_TASKMINIMAL

          // copy the callback and the event flags
          ptCtl->pvCallback = ptDef->pvCallback;
          ptCtl->tEventFlags = ptDef->tEventFlags;

          // configure the GPIO pins
          Gpio_Configure( ptDef->eDevPort, ptDef->nTxdPin, GPIO_MODE_OUTPUT_INPDSB, OFF, ptDef->eDevMux, OFF );
          Gpio_Configure( ptDef->eDevPort, ptDef->nRxdPin, GPIO_MODE_INPUT,         OFF, ptDef->eDevMux, OFF );

          // get the pointer to the USART channel
          ptCtl->ptUsart = GetSercomChannel( ptDef->eChan );

          // clear the two registers
          ptCtl->ptUsart->CTRLA.reg = 0;
          ptCtl->ptUsart->CTRLB.reg = 0;

          // disable the control adn wait for done
          ptCtl->ptUsart->CTRLA.bit.SWRST = 1;
          while( ptCtl->ptUsart->SYNCBUSY.bit.SWRST );
  
          // valid baud rate
          ptCtl->ptUsart->BAUD.reg = wBaudRate;
    
          // now set the modes
          switch( ptDef->eMode )
          {
            case UART_MODE_ASYNCINT :
              ptCtl->ptUsart->CTRLA.bit.MODE = 1;
              ptCtl->ptUsart->CTRLA.bit.CMODE = 0;
              break;
        
            case UART_MODE_SYNCEXT :
              ptCtl->ptUsart->CTRLA.bit.MODE = 1;
              ptCtl->ptUsart->CTRLA.bit.CMODE = 1;
              break;
        
            case UART_MODE_SYNCINT :
              ptCtl->ptUsart->CTRLA.bit.MODE = 1;
              ptCtl->ptUsart->CTRLA.bit.CMODE = 1;
              break;
        
            case UART_MODE_ASYNCEXT :
            default :
              ptCtl->ptUsart->CTRLA.bit.MODE = 0;
              ptCtl->ptUsart->CTRLA.bit.CMODE = 0;
              break;
          }
    
          // set the run in standby/sample rate/TX pin/RX pin
          ptCtl->ptUsart->CTRLA.bit.RUNSTDBY = ptDef->bRunStandby;
          ptCtl->ptUsart->CTRLA.bit.SAMPR = ptDef->eSamp;
          ptCtl->ptUsart->CTRLA.bit.TXPO = ptDef->eTxdPad;
          ptCtl->ptUsart->CTRLA.bit.RXPO = ptDef->eRxdPad;
          ptCtl->ptUsart->CTRLA.bit.DORD = UART_DATAORDER_LSB;
    
          // set the frame/parity
          ptCtl->ptUsart->CTRLA.bit.FORM = ( ptDef->eParity != UART_PARITY_NONE ) ? 1 : 0;
    
          // set the word length/stop bits/parity/TX enable/RX enable
          switch( ptDef->eWrdLen )
          {
            case UART_WRDLEN_5 : 
              ptCtl->ptUsart->CTRLB.reg |= SERCOM_USART_CTRLB_CHSIZE( 5 );
              break;
        
            case UART_WRDLEN_6 :
              ptCtl->ptUsart->CTRLB.reg |= SERCOM_USART_CTRLB_CHSIZE( 6 );
              break;
        
            case UART_WRDLEN_7 :
              ptCtl->ptUsart->CTRLB.reg |= SERCOM_USART_CTRLB_CHSIZE( 7 );
              break;
        
            case UART_WRDLEN_8 :
              ptCtl->ptUsart->CTRLB.bit.CHSIZE = 0;
              break;
        
            case UART_WRDLEN_9 :
              ptCtl->ptUsart->CTRLB.bit.CHSIZE = 1;
              break;
        
            default :
              ptCtl->ptUsart->CTRLB.bit.CHSIZE = 0;
              break;
          }
          ptCtl->ptUsart->CTRLB.bit.SBMODE = ptDef->eStopBit;
          ptCtl->ptUsart->CTRLB.bit.PMODE = ( ptDef->eParity == UART_PARITY_ODD ) ? ON : 0;
          ptCtl->ptUsart->CTRLB.bit.RXEN = ON;
          ptCtl->ptUsart->CTRLB.bit.TXEN = ON;
    
          // now write CTRLA/CTRLB
          while( ptCtl->ptUsart->SYNCBUSY.bit.CTRLB & SERCOM_USART_SYNCBUSY_CTRLB );

          // set up the interrupts
          ptCtl->ptUsart->INTENSET.bit.TXC = ON;
          ptCtl->ptUsart->INTENSET.bit.RXC = ON;
          ptCtl->ptUsart->INTENSET.bit.RXBRK = ON;
          ptCtl->ptUsart->INTENSET.bit.ERROR = ON;

          // enable the interrupt in the NVIC      
          NVIC_EnableIRQ( SERCOM0_IRQn + ptDef->eChan );
    
          // now enable the device
          ptCtl->ptUsart->CTRLA.reg |= SERCOM_USART_CTRLA_ENABLE;
          while( ptCtl->ptUsart->SYNCBUSY.bit.CTRLB & SERCOM_USART_SYNCBUSY_ENABLE );
        }
      }
      #if (SYSTEMDEFINE_OS_SELECTION != SYSTEMDDEFINE_OS_TASKMINIMAL )
      else
      {
        // free the transmit buffer
        free( ptCtl->tTxBuf.pnBuffer );
      }
      #endif // SYSTEMDEFINE_OS_SELECTON != SYSTEMDEFINE_OS_TASKMINIMAL
    }
    else
    {
      // free the control buffer
      free( ptCtl );
      ptCtl = NULL;
    }
  }
  else
  {
    // free all buffers/control
    #if (SYSTEMDEFINE_OS_SELECTION != SYSTEMDDEFINE_OS_TASKMINIMAL )
    free( ptCtl->tRxBuf.pnBuffer );
    #endif // SYSTEMDEFINE_OS_SELECTON != SYSTEMDEFINE_OS_TASKMINIMAL
    free( ptCtl->tTxBuf.pnBuffer );
    free( ptCtl );
    ptCtl = NULL;
  }

  // return the handle as a void
  return(( PTUARTHANDLE )ptCtl ); 
}

/******************************************************************************
 * @function Uart_Write
 *
 * @brief write some characters to the UART
 *
 * This function will write some characters to the UART buffer if room
 *
 * @param[in]   eChan       channel
 * @param[in]   pnData      pointer to the data buffer
 * @param[in]   wLength     size of the data buffer
 * @param[i0]   pwBytesWritten  the storage to return the number of bytes written
 *
 * @return    appropriate error
 *
 *****************************************************************************/
UARTERR Uart_Write( PTUARTHANDLE ptUart, PU8 pnData, U16 wLength, PU16 pwBytesWritten )
{
  PLCLCTL             ptCtl;
  U16                 wBytesRemaining, wIdx;
  PVUARTINTCALLBACK   pvCallback;
  UARTERR             eError = UART_ERR_ILLHANDLE;

  // map handle to pointer
  ptCtl = MAP_HANDLE_TO_POINTER( ptUart );
 
  // check for a valid UART
  if ( ptCtl->ptSignature < ptCtl )
  {
    // set no error
    eError = UART_ERR_NONE;

    // set the bytes written to zero
    *pwBytesWritten = 0;

    // compute the bytes written
    wBytesRemaining = ptCtl->tTxBuf.wSize - ptCtl->tTxBuf.wCount;
    *pwBytesWritten = MIN( wBytesRemaining, wLength );
    
    // for each byte
    Interrupt_Disable( );
    for ( wIdx = 0; wIdx < *pwBytesWritten; wIdx++ )
    {
      // put in buffer/adjust pointer/increment count
      *( ptCtl->tTxBuf.pnBuffer + ptCtl->tTxBuf.wWrIdx++ ) = *( pnData + wIdx );
      ptCtl->tTxBuf.wWrIdx %= ptCtl->tTxBuf.wSize;
      ptCtl->tTxBuf.wCount++;
    }

    // check for function handler/
    if ((( pvCallback = ptCtl->pvCallback ) != NULL ) && ( ptCtl->tEventFlags.bTxEnb ))
    {
      // call the callback to enable the transmit 
      pvCallback( UART_IRQ_EVENT_TXENB, 0 ); 
    }

    // enable the data register enmpty interrupt
    ptCtl->ptUsart->INTENSET.reg = BIT( SERCOM_USART_INTENSET_DRE_Pos );

    // enable the interrupt
    Interrupt_Enable( );
  }

  // return the error
  return( eError );
}

#if (SYSTEMDEFINE_OS_SELECTION != SYSTEMDDEFINE_OS_TASKMINIMAL )
/******************************************************************************
 * @function Uart_Read
 *
 * @brief read some characters from the UART
 *
 * This function will read bytes from the buffer
 *
 * @param[in]   eChan       channel
 * @param[in]   pnData      pointer to the data buffer
 * @param[in]   wLength     size of the data buffer
 * @param[i0]   pwBytesRead the storage to return the number of bytes written
 *
 * @return    appropriate error
 *
 *****************************************************************************/
UARTERR Uart_Read( PTUARTHANDLE ptUart, PU8 pnData, U16 wLength, PU16 pwBytesRead )
{
  PLCLCTL   ptCtl;
  U16       wBytesAvailable, wIdx;
  UARTERR   eError = UART_ERR_ILLHANDLE;
  
  // map handle to pointer
  ptCtl = MAP_HANDLE_TO_POINTER( ptUart );
 
  // check for a valid UART
  if ( ptCtl->ptSignature < ptCtl )
  {
    // set no error
    eError = UART_ERR_NONE;

    // compute the bytes written
    wBytesAvailable = ptCtl->tRxBuf.wSize - ptCtl->tRxBuf.wCount;
    *pwBytesRead = MIN( wBytesAvailable, wLength );
    
    // for each byte
    Interrupt_Disable( );
    for ( wIdx = 0; wIdx < *pwBytesRead; wIdx++ )
    {
      // put in buffer/adjust pointer/increment count
      *( pnData + wIdx ) = *( ptCtl->tRxBuf.pnBuffer + ptCtl->tRxBuf.wRdIdx++ );
      ptCtl->tRxBuf.wRdIdx %= ptCtl->tRxBuf.wSize;
      ptCtl->tRxBuf.wCount--;
    }
    Interrupt_Enable( );
  }
  // return the error
  return( eError );
}
#endif // SYSTEMDEFINE_OS_SELECTON != SYSTEMDEFINE_OS_TASKMINIMAL

#ifdef SERCOM0
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
  IrqCommonHandler( aptLclCtls[ UART_CHAN_0 ] );
}
#endif // SERCOM0

#ifdef SERCOM1
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
  IrqCommonHandler( aptLclCtls[ UART_CHAN_1 ] );
}
#endif // SERCOM1

#ifdef SERCOM2
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
  IrqCommonHandler( aptLclCtls[ UART_CHAN_2 ] );
}
#endif // SERCOM2

#ifdef SERCOM3
/******************************************************************************
 * @function SERCOM3_Handler
 *
 * @brief interrupt SERCOM 3 handler
 *
 * This function handles the interrupts from SERCOM 3
 *
 *****************************************************************************/
void SERCOM3_Handler( void )
{
  // call the common handler
  IrqCommonHandler( aptLclCtls[ UART_CHAN_3 ] );
}
#endif // SERCOM3

#ifdef SERCOM4
/******************************************************************************
 * @function SERCOM4_Handler
 *
 * @brief interrupt SERCOM 4 handler
 *
 * This function handles the interrupts from SERCOM 4
 *
 *****************************************************************************/
void SERCOM4_Handler( void )
{
  // call the common handler
  IrqCommonHandler( aptLclCtls[ UART_CHAN_4 ] );
}
#endif // SERCOM4

#ifdef SERCOM5
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
  IrqCommonHandler( aptLclCtls[ UART_CHAN_5 ] );
}
#endif // SERCOM5

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
static void IrqCommonHandler( PLCLCTL ptCtl )
{
  U8                        nOption;
  PVUARTINTCALLBACK         pvCallBack;
  U32                       uIntFlags;
  UARTIRQEVENTS             eEvent = UART_IRQ_EVENT_NONE;
  
  // get the callback
  pvCallBack = ptCtl->pvCallback;
  
  // get the interrupt cause
  uIntFlags = ptCtl->ptUsart->INTFLAG.reg;

  // was this a RX interrupt
  if ( uIntFlags & SERCOM_USART_INTFLAG_RXC )
  {
    // clear the flag
    ptCtl->ptUsart->INTFLAG.reg |= SERCOM_USART_INTFLAG_RXC;

    // get the data
    nOption = ( U8 )ptCtl->ptUsart->DATA.reg;

    // check for an error
    if ( uIntFlags & SERCOM_USART_INTFLAG_ERROR )
    {
      // clear the error flag
      ptCtl->ptUsart->INTFLAG.reg |= SERCOM_USART_INTFLAG_ERROR;
    }
    else
    {
      // check for callback
      if ( ptCtl->tEventFlags.bRxChar )
      {
        // if callback process it
        if (( pvCallBack != NULL ) & ( ptCtl->tEventFlags.bRxChar ))
        {
          // call the callback with a receive char event
          eEvent = UART_IRQ_EVENT_RXCHAR;
          pvCallBack( eEvent, nOption );
        }
        #if (SYSTEMDEFINE_OS_SELECTION != SYSTEMDDEFINE_OS_TASKMINIMAL )
        // check for room in local buffer
        else if ( ptCtl->tRxBuf.wCount < ptCtl->tRxBuf.wSize )
        {
          // stuff in buffer/adust index/increment count
          *( ptCtl->tRxBuf.pnBuffer + ptCtl->tRxBuf.wWrIdx++ ) = nOption;
          ptCtl->tRxBuf.wWrIdx %= ptCtl->tRxBuf.wSize;
          ptCtl->tRxBuf.wCount++;
        }
        #endif // SYSTEMDEFINE_OS_SELECTON != SYSTEMDEFINE_OS_TASKMINIMAL
      }
    }
  }
  
  // check for a data register empty interrupt
  if ( uIntFlags & SERCOM_USART_INTFLAG_DRE )
  {
    // clear the interrupt
    ptCtl->ptUsart->INTFLAG.reg |= SERCOM_USART_INTFLAG_DRE;
    
    // check for characters available
    if ( ptCtl->tTxBuf.wCount != 0 )
    {
      // output the character
      nOption = *( ptCtl->tTxBuf.pnBuffer + ptCtl->tTxBuf.wRdIdx++ );
      ptCtl->ptUsart->DATA.reg = nOption;
      ptCtl->tTxBuf.wRdIdx %= ptCtl->tTxBuf.wSize;
      ptCtl->tTxBuf.wCount--;
        
      // check for callback
      if (( pvCallBack != NULL ) && ( ptCtl->tEventFlags.bTxEmp ))
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
      ptCtl->ptUsart->INTENCLR.reg = BIT( SERCOM_USART_INTENSET_DRE_Pos );
    }
  }  
    
  // check for a transmit complete interrupt
  if ( uIntFlags & SERCOM_USART_INTFLAG_TXC )
  {
    // clear the interrupt
    ptCtl->ptUsart->INTFLAG.reg |= SERCOM_USART_INTFLAG_TXC;
    
    // if data is done, perform the callback
    if ( ptCtl->tTxBuf.wCount == 0 )
    {
      if (( pvCallBack  != NULL ) && ( ptCtl->tEventFlags.bTxCmp ))
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
    ptCtl->ptUsart->INTFLAG.reg |= SERCOM_USART_INTFLAG_RXBRK;
    
    // if callback and enabled, do it
    if (( pvCallBack != NULL ) && ( ptCtl->tEventFlags.bBrkDet ))
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
  Sercom*     ptSercom = NULL;
  CLOCKMUXID  eClockId;
  U32         uPeriphId;
  
  switch( eChan )
  {
    case UART_CHAN_0 :
      ptSercom = SERCOM0;
      eClockId = CLOCK_MUXID_SERCOM_0;
      uPeriphId = PM_APBCMASK_SERCOM0;
      break;
      
    case UART_CHAN_1 :
      ptSercom = SERCOM1;
      eClockId = CLOCK_MUXID_SERCOM_1;
      uPeriphId = PM_APBCMASK_SERCOM1;
      break;
    
    #ifdef SERCOM2
    case UART_CHAN_2:
      ptSercom = SERCOM2;
      eClockId = CLOCK_MUXID_SERCOM_2;
      uPeriphId = PM_APBCMASK_SERCOM2;
      break;
    #endif // SERCOM2
    
    #ifdef SERCOM3
    case UART_CHAN_3 :
      ptSercom = SERCOM3;
      eClockId = CLOCK_MUXID_SERCOM_3;
      uPeriphId = PM_APBCMASK_SERCOM3;
      break;
    #endif // SERCOM3
    
    #ifdef SERCOM4
    case UART_CHAN_4 :
      ptSercom = SERCOM4;
      eClockId = CLOCK_MUXID_SERCOM_4;
      uPeriphId = PM_APBCMASK_SERCOM4;
      break;
    #endif // SERCOM4
    
    #ifdef SERCOM5
    case UART_CHAN_5 :
      ptSercom = SERCOM5;
      eClockId = CLOCK_MUXID_SERCOM_5;
      uPeriphId = PM_APBCMASK_SERCOM5;
      break;
    #endif // SERCOM5
    
    default :
      break;
  }
  
  // now enable the clock and power mask
  Clock_PeriphEnable( eClockId, CLOCK_GENID_0 );
  PowerManager_DisableEnablePeriphC( uPeriphId, ON );

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
  U32   uPeripheralClock, uBaudIntermediate, uRatio;
  U8    nSampleRate, nFracIdx;
  BOOL  bFractional;
  U16   wBaudRate = 0;
  
  // get the system clock value
  uPeripheralClock = Clock_GetFreq( );
  
  // get the sample rate
  switch( ptDef->eSamp )
  {
    case UART_SAMP_16XARITH :
      nSampleRate = 16;
      bFractional = FALSE;
      break;
      
  #if (SYSTEMDEFINE_OS_SELECTION != SYSTEMDDEFINE_OS_TASKMINIMAL )
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
  #endif // SYSTEMDEFINE_OS_SELECTON != SYSTEMDEFINE_OS_TASKMINIMAL
      
    default :
      nSampleRate = 1;
      bFractional = FALSE;
      break;
  }
  
  // check for baud rate outside of normal range
  if (( ptDef->uBaudRate * nSampleRate ) <= uPeripheralClock )
  {
    #if (SYSTEMDEFINE_OS_SELECTION != SYSTEMDDEFINE_OS_TASKMINIMAL )
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
    #endif // SYSTEMDEFINE_OS_SELECTON != SYSTEMDEFINE_OS_TASKMINIMAL
  {
      // calculate the baud rate
      uRatio = (( nSampleRate * ( U64 )ptDef->uBaudRate ) << 32 ) / uPeripheralClock;
      wBaudRate = ( U16 )(( 65536 ) - uRatio );
    }
  }
  
  // return the baud rate
  return( wBaudRate );
}

/**@} EOF Uart.c */
