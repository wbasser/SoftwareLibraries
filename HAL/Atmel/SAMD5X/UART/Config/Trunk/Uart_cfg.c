/******************************************************************************
 * @file Uart_cfg.c
 *
 * @brief UART configuration implementation
 *
 * This file provides the UART configuration implementation
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
 * $Rev: $
 * 
 *
 * \addtogroup UART
 * @{
 *****************************************************************************/

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "UART/Uart_cfg.h"

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// static function prototypes -------------------------------------------------
#if ( UART_CHAN0_IN_USE == 1 )
static  void  UartChan0IrqCallback( UARTIRQEVENTS eEvent, U8 nOption );
#endif // UART_CHAN0_IN_USE
#if ( UART_CHAN1_IN_USE == 1 )
static  void  UartChan1IrqCallback( UARTIRQEVENTS eEvent, U8 nOption );
#endif // UART_CHAN1_IN_USE
#if ( UART_CHAN2_IN_USE == 1 )
static  void  UartChan2IrqCallback( UARTIRQEVENTS eEvent, U8 nOption );
#endif // UART_CHAN2_IN_USE
#if ( UART_CHAN3_IN_USE == 1 )
static  void  UartChan3IrqCallback( UARTIRQEVENTS eEvent, U8 nOption );
#endif // UART_CHAN3_IN_USE
#if ( UART_CHAN4_IN_USE == 1 )
static  void  UartChan4IrqCallback( UARTIRQEVENTS eEvent, U8 nOption );
#endif // UART_CHAN4_IN_USE
#if ( UART_CHAN5_IN_USE == 1 )
static  void  UartChan5IrqCallback( UARTIRQEVENTS eEvent, U8 nOption );
#endif // UART_CHAN5_IN_USE

// global parameter declarations ----------------------------------------------
/// declare the buffers
// UART_BUFFER( name, rxsize, txsize );

/// device configuration table
const UARTDEF g_atUartDefs[ UART_DEV_ENUM_MAX ] = 
{
  // UART_ASYNC_DEF( chan, txdpad, rxdpad, baudrate, parity, wrdlen, stopbits, standby, name, rxsize, txsize, callback, rcvchr, rcverr, txemp, txcmp, txenb, brkdet )
};  

#if ( UART_CHAN0_IN_USE == 1 )
/******************************************************************************
 * @function UartChan0IrqCallback
 *
 * @brief UART channel 0 IRQ callback
 *
 * This function will handle the IRQ events for UART channel 00
 *
 * @param[in]     eEvent    IRQ event
 * @param[in]     nOption   optional data
 *
 *****************************************************************************/
static void UartChan0IrqCallback( UARTIRQEVENTS eEvent, U8 nOption )
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

    // transmit enable event
    case UART_IRQ_EVENT_TXENB :
      break;

    // break detect event
    case UART_IRQ_EVENT_BRKDET :
      break;

    default :
      break;
  }
}
#endif

#if ( UART_CHAN1_IN_USE == 1 )
/******************************************************************************
 * @function UartChan0IrqCallback
 *
 * @brief UART channel 0 IRQ callback
 *
 * This function will handle the IRQ events for UART channel 00
 *
 * @param[in]     eEvent    IRQ event
 * @param[in]     nOption   optional data
 *
 *****************************************************************************/
static void UartChan1IrqCallback( UARTIRQEVENTS eEvent, U8 nOption )
{
  // process the event
  switch( eEvent )
  {
    // receive character event
    case UART_IRQ_EVENT_RXCHAR :
      // notify the task that a character has been received
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

    // transmit enable event
    case UART_IRQ_EVENT_TXENB :
      break;

    // break detect event
    case UART_IRQ_EVENT_BRKDET :
      break;

    default :
      break;
  }
}
#endif

#if ( UART_CHAN2_IN_USE == 1 )
/******************************************************************************
 * @function UartChan2IrqCallback
 *
 * @brief UART channel 0 IRQ callback
 *
 * This function will handle the IRQ events for UART channel 00
 *
 * @param[in]     eEvent    IRQ event
 * @param[in]     nOption   optional data
 *
 *****************************************************************************/
static void UartChan2IrqCallback( UARTIRQEVENTS eEvent, U8 nOption )
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

    // transmit enable event
    case UART_IRQ_EVENT_TXENB :
      break;

    // break detect event
    case UART_IRQ_EVENT_BRKDET :
      break;

    default :
      break;
  }
}
#endif

#if ( UART_CHAN3_IN_USE == 1 )
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

    // transmit enable event
    case UART_IRQ_EVENT_TXENB :
      break;

    // break detect event
    case UART_IRQ_EVENT_BRKDET :
      break;

    default :
      break;
  }
}
#endif

#if ( UART_CHAN4_IN_USE == 1 )
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

    // transmit enable event
    case UART_IRQ_EVENT_TXENB :
      break;

    // break detect event
    case UART_IRQ_EVENT_BRKDET :
      break;

    default :
      break;
  }
}
#endif

#if ( UART_CHAN5_IN_USE == 1 )
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

    // transmit enable event
    case UART_IRQ_EVENT_TXENB :
      break;

    // break detect event
    case UART_IRQ_EVENT_BRKDET :
      break;

    default :
      break;
  }
}
#endif


/**@} EOF Uart_cfg.c */
