/******************************************************************************
 * @file Uart_cfg.c
 *
 * @brief UART configuraiton implementation
 *
 * This file pvoides the UART configuration implementation
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
static  void  UartChan0IrqCallback( UARTIRQEVENTS eEvent, U8 nOption1, U8 nOption2 );
static  void  UartChan1IrqCallback( UARTIRQEVENTS eEvent, U8 nOption1, U8 nOption2 );
static  void  UartChan2IrqCallback( UARTIRQEVENTS eEvent, U8 nOption1, U8 nOption2 );

// global parameter declarations ----------------------------------------------
/// declare the buffers
// UART_BUFFER( name, rxsize, txsize );

/// device configuration table
const UARTDEF atUartDefs[ UART_DEV_ENUM_MAX ] = 
{
  // UART_DEF( chan, baudrate, parity, stopbit, name, rxsize, txsize, callback, rcvchr, rcverr, txemp, txcmp, xmtctl )
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
static void UartChan0IrqCallback( UARTIRQEVENTS eEvent, U8 nOption1, U8 nOption2 )
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
  
    // enable transmit
    case UART_IRQ_EVENT_TXENB :
      break;

    default :
      break;
  }
}

/******************************************************************************
 * @function UartChan1IrqCallback
 *
 * @brief UART channel 1 IRQ callback
 *
 * This function will handle the IRQ events for UART channel 1
 *
 * @param[in]     eEvent    IRQ event
 * @param[in]     nOption   optioanal data
 *
 *****************************************************************************/
static void UartChan1IrqCallback( UARTIRQEVENTS eEvent, U8 nOption1, U8 nOption2 )
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

    // enable transmit
    case UART_IRQ_EVENT_TXENB :
      break;

    default :
      break;
  }
}

/******************************************************************************
 * @function UartChan2IrqCallback
 *
 * @brief UART channel 2 IRQ callback
 *
 * This function will h ndle the IRQ events for UART channel 2
 *
 * @param[in]     eEvent    IRQ event
 * @param[in]     nOption   optioanal data
 *
 *****************************************************************************/
static void UartChan2IrqCallback( UARTIRQEVENTS eEvent, U8 nOption1, U8 nOption2 )
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

    // enable transmit
    case UART_IRQ_EVENT_TXENB :
      break;

    default :
      break;
  }
}

/**@} EOF Uart_cfg.c */
