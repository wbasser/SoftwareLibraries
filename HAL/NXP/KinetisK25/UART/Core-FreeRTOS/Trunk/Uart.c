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
static  Uart* GetUartPointer( UARTCHAN eChan );
static  U16   ComputeBaudrate( U32 uBaudrate );
static  void  IrqCommonHandler( UARTDEVENUM eDev );

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
  U16                         wBaudRate;
  UARTDEVENUM                 eDev;
  Uart*                       ptUart;
  
  // for each defined UART
  for ( eDev = 0; eDev < UART_DEV_ENUM_MAX; eDev++ )
  {
    // get pointer to the definition/control structures
    ptDef = ( PUARTDEF )&atUartDefs[ eDev ];
    ptCtl = &atLclCtls[ eDev ];

    // clear the receive/transmit buffers
    ptCtl->tRxBuf.wWrIdx = ptCtl->tRxBuf.wRdIdx = ptCtl->tRxBuf.wCount = 0;
    ptCtl->tTxBuf.wWrIdx = ptCtl->tTxBuf.wRdIdx = ptCtl->tTxBuf.wCount = 0;
    
    // now compute the baud rate
    if (( wBaudRate = ComputeBaudrate( ptDef->uBaudRate )) != 0 )
    {
      // get the uart
      ptUart = GetUartPointer( ptDef->eChan );
      
      // valid baud rate
      ptUart->UART_BRGR = wBaudRate;
      
      // set the frame/parity
      ptUart->UART_MR = BITS( ptDef->eParity, UART_MR_PAR_Pos );
  
      // set the interrupts
      ptUart->UART_IER = ( UART_IER_RXRDY );

      // enable the interrupt in the NVIC      
      aePhyToDefMap[ ptDef->eChan ] = eDev;
      switch( ptDef->eChan )
      {
        case UART_CHAN_0 :
          NVIC_EnableIRQ( UART0_IRQn );
          break;

        case UART_CHAN_1 :
          NVIC_EnableIRQ( UART1_IRQn );
          break;

        default :
          break;
      }
      
      // now enable the device
      ptUart->UART_CR = UART_CR_RXEN | UART_CR_TXEN |  UART_CR_RSTSTA;
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
  UARTDEVENUM                 eDev;
  Uart*                       ptUart;
  
  // for each defined UART
  for ( eDev = 0; eDev < UART_DEV_ENUM_MAX; eDev++ )
  {
    // get pointer to the definition/control structures
    ptDef = ( PUARTDEF )&atUartDefs[ eDev ];

    // get the uart
    ptUart = GetUartPointer( ptDef->eChan );

    // enable the interrupt in the NVIC
    switch( ptDef->eChan )
      {
        case UART_CHAN_0 :
          NVIC_DisableIRQ( UART0_IRQn );
          break;

        case UART_CHAN_1 :
          NVIC_DisableIRQ( UART1_IRQn );
          break;

        default :
          break;
      }

    
    // turn of the UART
    ptUart->UART_CR = UART_CR_RXDIS | UART_CR_TXDIS;
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
  UARTERR       eError = UART_ERR_NONE;
  PUARTDEF      ptDef;
  PLCLCTL       ptCtl;
  U16           wBytesRemaining, wIdx;
  Uart*         ptUart;
 
  // check for a valid UART
  if ( eDev < UART_DEV_ENUM_MAX )
  {
    // set the bytes written to zero
    *pwBytesWritten = 0;

    // get pointers to the control/def structure
    ptDef = ( PUARTDEF )&atUartDefs[ eDev ];
    ptCtl = &atLclCtls[ eDev ];
    
    // get the uart
    ptUart = GetUartPointer( ptDef->eChan );

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
    ptUart->UART_IER = ( UART_IER_TXRDY | UART_IER_TXEMPTY );

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
  Uart*   ptUart;
  
  // check for a valid UART
  if ( eDev < UART_DEV_ENUM_MAX )
  {
    // get the uart
    ptUart = GetUartPointer( atUartDefs[ eDev ].eChan );

    // disable the USART
    ptUart->UART_CR &= ~( UART_CR_RXEN | UART_CR_TXEN );
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
 * @function UART0_Handler
 *
 * @brief interrupt UART0 handler
 *
 * This function handles the interrupts from UART0
 *
 *****************************************************************************/
void UART0_Handler( void )
{
  // call the common handler
  IrqCommonHandler( aePhyToDefMap[ UART_CHAN_0 ] );
}

/******************************************************************************
 * @function UART1_Handler
 *
 * @brief interrupt UART1 handler
 *
 * This function handles the interrupts from UART1
 *
 *****************************************************************************/
void UART1_Handler( void )
{
  // call the common handler
  IrqCommonHandler( aePhyToDefMap[ UART_CHAN_1 ] );
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
  U8                  nOption;
  PVUARTINTCALLBACK   pvCallBack;
  UARTIRQEVENTS       eEvent = UART_IRQ_EVENT_NONE;
  Uart*               ptUart;
  
  // get pointer to the definition/control structures
  ptDef = ( PUARTDEF )&atUartDefs[ eDev ];
  ptCtl = &atLclCtls[ eDev ];
  
  // get the uart
  ptUart = GetUartPointer( ptDef->eChan );

  // get the callback
  pvCallBack = ptDef->pvCallback;
  
  // was this a RX interrupt
  if ( ptUart->UART_SR & UART_SR_RXRDY )
  {
    // get the data
    nOption = ( U8 )ptUart->UART_RHR;

    // check for an error
    if (( ptUart->UART_SR & UART_SR_FRAME ) || ( ptUart->UART_SR & UART_SR_PARE ) || ( ptUart->UART_SR & UART_SR_OVRE ))
    {
      // clear the error flag
      ptUart->UART_CR = ( UART_CR_RSTSTA | UART_CR_RXEN | UART_CR_TXEN );
      
      // if callback process it
      if (( pvCallBack != NULL ) & ( ptDef->tEventFlags.bRxErr ))
      {
        // call the callback with a receive char event
        eEvent = UART_IRQ_EVENT_RXERR;
        pvCallBack( eEvent, nOption );
      }
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
  if ( ptUart->UART_SR & UART_SR_TXRDY )
  {
    // check for characters available
    if ( ptCtl->tTxBuf.wCount != 0 )
    {
      // output the character
      nOption = *( ptDef->pnTxBuffer + ptCtl->tTxBuf.wRdIdx++ );
      ptUart->UART_THR = nOption;
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
      ptUart->UART_IDR = UART_IDR_TXRDY;
    }
  }  
    
  // check for a transmit complete interrupt
  if ( ptUart->UART_SR & UART_SR_TXEMPTY )
  {
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
    
    // disable the interrupt
    ptUart->UART_IDR = UART_IDR_TXEMPTY;
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
static Uart* GetUartPointer( UARTCHAN eChan )
{
  Uart* ptUart = NULL;
  
  switch( eChan )
  {
    case UART_CHAN_0 :
      ptUart = UART0;
      break;
      
    case UART_CHAN_1 :
      ptUart = UART1;
      break;
    
    default :
      break;
  }
  
  // return the pointer to the channlel
  return( ptUart );
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
static U16 ComputeBaudrate( U32 uBaudRate )
{
  U32   uPeripheralClock;
  U16   wBaudRate = 0;
  
  // get the system clock value
  uPeripheralClock = Clock_GetFreq( );
  
  wBaudRate = uPeripheralClock / ( 16 * uBaudRate );
  
  // return the baud rate
  return( wBaudRate );
}

/**@} EOF Uart.c */
