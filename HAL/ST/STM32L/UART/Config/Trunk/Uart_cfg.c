/******************************************************************************
 * @file Uart_cfg.c
 *
 * @brief UART configuraiton implementation
 *
 * This file pvoides the UART configuration implementation
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
 * $Rev: $
 * 
 *
 * \addtogroup UART
 * @{
 *****************************************************************************/

// system includes ------------------------------------------------------------

// library includes -----------------------------------------------------------

// local includes -------------------------------------------------------------
#include "UART/Uart_cfg.h"

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// static function prototypes -------------------------------------------------
static  void  UartChan1IrqCallback( UARTIRQEVENTS eEvent, U8 nOption );
static  void  UartChan2IrqCallback( UARTIRQEVENTS eEvent, U8 nOption );
static  void  UartChan3IrqCallback( UARTIRQEVENTS eEvent, U8 nOption );
static  void  UartChan4IrqCallback( UARTIRQEVENTS eEvent, U8 nOption );
static  void  UartChan5IrqCallback( UARTIRQEVENTS eEvent, U8 nOption );

// global parameter declarations ----------------------------------------------
/// declare the buffers
// UART_BUFFER( name, rxsize, txsize );

/// device configuration table
const UARTDEF atUartDefs[ UART_DEV_ENUM_MAX ] = 
{
  // UART_ASYNC_DEF( chan, baudrate, parity, wrdlen, stopbits, name, rxsize, txsize, callback, rcvchr, rcverr, txemp, txcmp, brkdet )
};  


/******************************************************************************
 * @function UartChan0IrqCallback
 *
 * @brief UART channel 0 IRQ callback
 *
 * This function will handle the IRQ events for UART channel 00
 *
 * @param[in]     eEvent    IRQ event
 * @param[in]     nOption   optioanal data
 *
 *****************************************************************************/
static void UartChan1IrqCallback( UARTIRQEVENTS eEvent, U8 nOption )
{
  // process the event
  switch( eEvent )
  {
    // receive character event
    case UART_IRQ_EVENT_RXCHAR :
      // post it to the event queue
      break;

    // receive error event
    case UART_IRQ_EVENT_RXERR :
      break;

    // transmit empty event
    case UART_IRQ_EVENT_TXEMP :
      break;

    // transmit complete event
    case UART_IRQ_EVENT_TXCMP :
      break;

    // break detect event
    case UART_IRQ_EVENT_BRKDET :
      break;

    default :
      break;
  }
}

/******************************************************************************
 * @function UartChan2IrqCallback
 *
 * @brief UART channel 0 IRQ callback
 *
 * This function will handle the IRQ events for UART channel 00
 *
 * @param[in]     eEvent    IRQ event
 * @param[in]     nOption   optioanal data
 *
 *****************************************************************************/
static void UartChan2IrqCallback( UARTIRQEVENTS eEvent, U8 nOption )
{
  // process the event
  switch( eEvent )
  {
    // receive character event
    case UART_IRQ_EVENT_RXCHAR :
      // post it to the event queue
      break;

    // receive error event
    case UART_IRQ_EVENT_RXERR :
      break;

    // transmit empty event
    case UART_IRQ_EVENT_TXEMP :
      break;

    // transmit complete event
    case UART_IRQ_EVENT_TXCMP :
      break;

    // break detect event
    case UART_IRQ_EVENT_BRKDET :
      break;

    default :
      break;
  }
}
 
/******************************************************************************
 * @function UartChan3IrqCallback
 *
 * @brief UART channel 0 IRQ callback
 *
 * This function will handle the IRQ events for UART channel 00
 *
 * @param[in]     eEvent    IRQ event
 * @param[in]     nOption   optioanal data
 *
 *****************************************************************************/
static void UartChan3IrqCallback( UARTIRQEVENTS eEvent, U8 nOption )
{
  // process the event
  switch( eEvent )
  {
    // receive character event
    case UART_IRQ_EVENT_RXCHAR :
      break;

    // receive error event
    case UART_IRQ_EVENT_RXERR :
      break;

    // transmit empty event
    case UART_IRQ_EVENT_TXEMP :
      break;

    // transmit complete event
    case UART_IRQ_EVENT_TXCMP :
      break;

    // break detect event
    case UART_IRQ_EVENT_BRKDET :
      break;

    default :
      break;
  }
}

/******************************************************************************
 * @function UartChan4IrqCallback
 *
 * @brief UART channel 5 IRQ callback
 *
 * This function will handle the IRQ events for UART channel 00
 *
 * @param[in]     eEvent    IRQ event
 * @param[in]     nOption   optioanal data
 *
 *****************************************************************************/
static void UartChan4IrqCallback( UARTIRQEVENTS eEvent, U8 nOption )
{
  // process the event
  switch( eEvent )
  {
    // receive character event
    case UART_IRQ_EVENT_RXCHAR :
      break;

    // receive error event
    case UART_IRQ_EVENT_RXERR :
      break;

    // transmit empty event
    case UART_IRQ_EVENT_TXEMP :
      break;

    // transmit complete event
    case UART_IRQ_EVENT_TXCMP :
      break;

    // break detect event
    case UART_IRQ_EVENT_BRKDET :
      break;

    default :
      break;
  }
}

/******************************************************************************
 * @function UartChan5IrqCallback
 *
 * @brief UART channel 5 IRQ callback
 *
 * This function will handle the IRQ events for UART channel 00
 *
 * @param[in]     eEvent    IRQ event
 * @param[in]     nOption   optioanal data
 *
 *****************************************************************************/
static void UartChan5IrqCallback( UARTIRQEVENTS eEvent, U8 nOption )
{
  // process the event
  switch( eEvent )
  {
    // receive character event
    case UART_IRQ_EVENT_RXCHAR :
      break;

    // receive error event
    case UART_IRQ_EVENT_RXERR :
      break;

    // transmit empty event
    case UART_IRQ_EVENT_TXEMP :
      break;

    // transmit complete event
    case UART_IRQ_EVENT_TXCMP :
      break;

    // break detect event
    case UART_IRQ_EVENT_BRKDET :
      break;

    default :
      break;
  }
}

/**@} EOF Uart_cfg.c */
