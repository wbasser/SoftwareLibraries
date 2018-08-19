/******************************************************************************
 * @file Usart_cfg.c
 *
 * @brief USART configuraiton implementation
 *
 * This file pvoides the USART configuration implementation
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
 * \addtogroup USART
 * @{
 *****************************************************************************/

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "USART/Usart_cfg.h"

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------
#include "CommandHandlers/AscBinDirector/AscBinDirector.h"

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// local function prototypes -------------------------------------------------
static void   Usart_Chan0_Callback( USARTIRQEVENTS eEvent, U8 nOption );
static void   Usart_Chan1_Callback( USARTIRQEVENTS eEvent, U8 nOption );
static void   Usart_Chan2_Callback( USARTIRQEVENTS eEvent, U8 nOption );

// global parameter declarations ----------------------------------------------
#if ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_TASKMANAGER )
/// declare the buffers
// USART_BUFFER( name, rxsize, txsize );
#endif // SYSTEMDEFINE_OS_SELECTION

/// device configuration table
const USARTDEF atUsartDefs[ USART_DEV_ENUM_MAX ] = 
{
  // define the USART channels
  #if ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_TASKMANAGER )
  // USART_DEF( chan, baudrate, parity, name, rxsize, txsize, callback, rcvchr, rcverr, txemp, txcmp )
  #elif ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_FREERTOS )
  // USART_DEF( chan, baudrate, parity, rxsize, txsize, callback, rcvchr, rcverr, txemp, txcmp )
  #endif // SYSTEMDEFINE_OS_SELECTION
};  

/******************************************************************************
 * @function Usart_Chan0_Callback 
 *
 * @brief processes the callback from the USART
 *
 * This function will process the callback from the uart
 *
 * @param[in]   eEvent    USART callback event
 * @param[in]   nOption   optional data or status byte
 *
 *****************************************************************************/
static void Usart_Chan0_Callback( USARTIRQEVENTS eEvent, U8 nOption )
{
  // process the event
  switch( eEvent )
  {
    // receive character event
    case USART_IRQ_EVENT_RXCHAR :
      break;

    // receive error event
    case USART_IRQ_EVENT_RXERR :
      break;

    // transmit empty event
    case USART_IRQ_EVENT_TXEMP :
      break;

    // transmit complete event
    case USART_IRQ_EVENT_TXCMP :
      break;

    default :
      break;
  }  
}

/******************************************************************************
 * @function Usart_Chan1_Callback 
 *
 * @brief processes the callback from the USART
 *
 * This function will process the callback from the uart
 *
 * @param[in]   eEvent    USART callback event
 * @param[in]   nOption   optional data or status byte
 *
 *****************************************************************************/
static void Usart_Chan1_Callback( USARTIRQEVENTS eEvent, U8 nOption )
{
  // process the event
  switch( eEvent )
  {
    // receive character event
    case USART_IRQ_EVENT_RXCHAR :
      // post it to the event queue
      break;

    // receive error event
    case USART_IRQ_EVENT_RXERR :
      break;

    // transmit empty event
    case USART_IRQ_EVENT_TXEMP :
      break;

    // transmit complete event
    case USART_IRQ_EVENT_TXCMP :
      break;

    default :
      break;
  }  
}

/******************************************************************************
 * @function Usart_Chan2_Callback 
 *
 * @brief processes the callback from the USART
 *
 * This function will process the callback from the uart
 *
 * @param[in]   eEvent    USART callback event
 * @param[in]   nOption   optional data or status byte
 *
 *****************************************************************************/
static void Usart_Chan2_Callback( USARTIRQEVENTS eEvent, U8 nOption )
{
  // process the event
  switch( eEvent )
  {
    // receive character event
    case USART_IRQ_EVENT_RXCHAR :
      // post it to the event queue
      break;

    // receive error event
    case USART_IRQ_EVENT_RXERR :
      break;

    // transmit empty event
    case USART_IRQ_EVENT_TXEMP :
      break;

    // transmit complete event
    case USART_IRQ_EVENT_TXCMP :
      break;

    default :
      break;
  }  
}

/**@} EOF Usart_cfg.c */
