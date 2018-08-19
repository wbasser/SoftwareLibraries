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

// local function prototypes -------------------------------------------------
static void   Uart_Chan0_Callback( UARTIRQEVENTS eEvent, U8 nOption );
static void   Uart_Chan1_Callback( UARTIRQEVENTS eEvent, U8 nOption );
static void   Uart_Chan2_Callback( UARTIRQEVENTS eEvent, U8 nOption );
static void   Uart_Chan3_Callback( UARTIRQEVENTS eEvent, U8 nOption );
static void   Uart_Chan4_Callback( UARTIRQEVENTS eEvent, U8 nOption );

// global parameter declarations ----------------------------------------------
#if ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_TASKMANAGER )
/// declare the buffers
// UART_BUFFER( name, rxsize, txsize );
#endif // SYSTEMDEFINE_OS_SELECTION

/// device configuration table
const UARTDEF atUartDefs[ UART_DEV_ENUM_MAX ] = 
{
  // define the UART channels
  #if ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_TASKMANAGER )
  // UART_DEF( chan, baudrate, parity, name, rxsize, txsize, callback, rcvchr, rcverr, txemp, txcmp, txenb )
  #elif ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_FREERTOS )
  // UART_DEF( chan, baudrate, parity, rxsize, txsize, callback, rcvchr, rcverr, txemp, txcmp, txenb )
  #endif // SYSTEMDEFINE_OS_SELECTION
};  

/******************************************************************************
 * @function Uart_Chan0_Callback 
 *
 * @brief processes the callback from the UART
 *
 * This function will process the callback from the uart
 *
 * @param[in]   eEvent    UART callback event
 * @param[in]   nOption   optional data or status byte
 *
 *****************************************************************************/
static void Uart_Chan0_Callback( UARTIRQEVENTS eEvent, U8 nOption )
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

    default :
      break;
  }  
}

/******************************************************************************
 * @function Uart_Chan1_Callback 
 *
 * @brief processes the callback from the UART
 *
 * This function will process the callback from the uart
 *
 * @param[in]   eEvent    UART callback event
 * @param[in]   nOption   optional data or status byte
 *
 *****************************************************************************/
static void Uart_Chan1_Callback( UARTIRQEVENTS eEvent, U8 nOption )
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

    default :
      break;
  }  
}

/******************************************************************************
 * @function Uart_Chan2_Callback 
 *
 * @brief processes the callback from the UART
 *
 * This function will process the callback from the uart
 *
 * @param[in]   eEvent    UART callback event
 * @param[in]   nOption   optional data or status byte
 *
 *****************************************************************************/
static void Uart_Chan2_Callback( UARTIRQEVENTS eEvent, U8 nOption )
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

    default :
      break;
  }  
}

/******************************************************************************
 * @function Uart_Chan3_Callback 
 *
 * @brief processes the callback from the UART
 *
 * This function will process the callback from the uart
 *
 * @param[in]   eEvent    UART callback event
 * @param[in]   nOption   optional data or status byte
 *
 *****************************************************************************/
static void Uart_Chan3_Callback( UARTIRQEVENTS eEvent, U8 nOption )
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

    default :
      break;
  }  
}

/******************************************************************************
 * @function Uart_Chan4_Callback 
 *
 * @brief processes the callback from the UART
 *
 * This function will process the callback from the uart
 *
 * @param[in]   eEvent    UART callback event
 * @param[in]   nOption   optional data or status byte
 *
 *****************************************************************************/
static void Uart_Chan4_Callback( UARTIRQEVENTS eEvent, U8 nOption )
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

    default :
      break;
  }  
}

/**@} EOF Uart_cfg.c */
