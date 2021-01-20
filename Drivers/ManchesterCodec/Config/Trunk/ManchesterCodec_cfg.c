/******************************************************************************
 * @file ManchesterCodec_cfg.c
 *
 * @brief Manchester Codec configuration implementation
 *
 * This file provides the implementation for the configuration of the
 * Manchester Codec
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
 * @function ManchesterCodec_LocalInitialize
 *
 * @brief initialize the manchester codec
 *
 * This function will perform all initialization for the manchester encoder/
 * decoder
 *
 * @param[in]   uHalfBitTime      half time bit
 *
 *****************************************************************************/
void  ManchesterCodec_LocalInitialize( U32 uHalfBitTime )
{
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

/******************************************************************************
 * @function  ManchesterCodec_TransmitCallback
 *
 * @brief  transmit timer callback function
 *
 * This function will process the transmit timer callback 
 *
 * @param[in]   nEvent    event that is generated
 * @param[in]   nChan     compare/capture channel
 * @param[in]   wValue    optional value
 *
 *****************************************************************************/
void ManchesterCodec_TransmitCallback( U8 nEvent, U8 nChan, U16 wValue )
{
  // process the bit timer
  ManchesterCodec_ProcessXmtTimer( );
}

/******************************************************************************
 * @function  ManchesterCodec_ReceiveCallback
 *
 * @brief  receive capture callback function
 *
 * This function will process the receive capture callback 
 *
 * @param[in]   eEvent    event that is generated
 * @param[in]   wValue    optional value
 *
 *****************************************************************************/
void ManchesterCodec_ReceiveCallback( U8 nEvent, U8 nChan, U16 wValue )
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
 * @function  ManchesterCodec_FlushRecvEents
 *
 * @brief  flush the receive events if any
 *
 * This function will flush the receive events
 *
 *****************************************************************************/
void ManchesterCodec_FlushRecvEvents( void )
{
}

/******************************************************************************
 * @function  ManchesterCodec_OutputControl
 *
 * @brief  control the output bit
 *
 * This function will set/clear the output bit
 *
 * @param[in]   bCurBit     current bit state
 *
 *****************************************************************************/
void ManchesterCodec_OutputControl( BOOL bCurBit )
{
}

/******************************************************************************
 * @function  ManchesterCodec_PostXmitEvent
 *
 * @brief  post an event to transmit task
 *
 * This function will post an event to the calling task
 *
 * @param[in]   uEvent      event to send
 *
 *****************************************************************************/
void ManchesterCodec_PostXmitEvent( U32 uEvent )
{
}

/******************************************************************************
 * @function  ManchesterCodec_PosRecvtEvent
 *
 * @brief  post an event to receive task
 *
 * This function will post an event to the calling task
 *
 * @param[in]   uEvent      event to send
 *
 *****************************************************************************/
void ManchesterCodec_PostRecvEvent( U32 uEvent )
{
}

/**@} EOF ManchesterCodec_cfg.c */
