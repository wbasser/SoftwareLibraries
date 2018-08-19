/******************************************************************************
 * @file ManchesterCodec_cfg.c
 *
 * @brief 
 *
 * This file 
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
 * \addtogroup ManchesterCodec
 * @{
 *****************************************************************************/

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "ManchesterCodec/ManchesterCodec.h"

// library includes -----------------------------------------------------------
#include "Clock/Clock.h"

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations ----------------------------------------------

// local parameter declarations -----------------------------------------------

// local function prototypes --------------------------------------------------

// constant parameter initializations -----------------------------------------

/******************************************************************************
 * @function  ManchesterCodec_TransmitCallback
 *
 * @brief  transmit timer callback function
 *
 * This function will process the transmit timer callback 
 *
 * @param[in]   eEvent    event that is generated
 * @param[in]   eChan     compare/capture channel
 * @param[in]   wValue    optional value
 *
 *****************************************************************************/
void ManchesterCodec_TransmitCallback( TIMERCBEVENT eEvent, TIMERCMPCAP eChan, U16 wValue )
{
  // process the bit timer
  ManchesterCodec_ProcessXmtTimer( );
}

/******************************************************************************
 * @function  ManchesterCodec_TReceiveCallback
 *
 * @brief  receive capture callback function
 *
 * This function will process the receive capture callback 
 *
 * @param[in]   eEvent    event that is generated
 * @param[in]   wValue    optional value
 *
 *****************************************************************************/
void ManchesterCodec_ReceiveCallback( TIMERCBEVENT eEvent, TIMERCMPCAP eChan, U16 wValue )
{
}

/******************************************************************************
 * @function  ManchesterCodec_RecvTimerControl
 *
 * @brief  set timer for receive
 *
 * This function will configure and start the timer for receive
 *
 * @param[in]   bState    desired state of the transmit timer
 *
 *****************************************************************************/
void ManchesterCodec_RecvTimerControl( BOOL bState )
{
  // turn off/on the receive timer
}

/******************************************************************************
 * @function  ManchesterCodec_GetClockFreq
 *
 * @brief  get the current clock frequency used by the manchester codec
 *
 * This function will return the base clock frequency for the manchester
 * codec
 *
 *****************************************************************************/
U32 ManchesterCodec_GetClockFreq( void )
{
  // return the clock frequency
  return( 0 );
}

/**@} EOF ManchesterCodec_cfg.c */
