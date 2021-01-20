/******************************************************************************
 * @file Event.c
 *
 * @brief Event generator implementation
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
 * $Date: $
 *
 * Version History
 * ======
 * $Rev: $
 * 
 *
 * \addtogroup Event
 * @{
 *****************************************************************************/

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "Event/Event.h"

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
 * @function Event_Create
 *
 * @brief initialize the event generators
 *
 * This function will process all defined event generators
 *
 * @param[in]   eChannel			event channel
 * @param[in]   eGenerator		event generator
 * @param[in]   eEdge					event edge
 * @param[in]   eUserMux			event user mux
 *
 *****************************************************************************/
void Event_Create( EVENTCHAN eChannel, EVENTGEN eGenerator, EVENTEDGE eEdge, EVENTUSERMUX eUserMux )
{
  EVSYS_CHANNEL_Type  tChan;
  U8                  nTemp;

  // enable the clock


  // reset the system
  EVSYS->CTRL.bit.SWRST = ON;

  // set the clock to always on
  EVSYS->CTRL.bit.GCLKREQ = ON;

	// set up the channel
	tChan.reg = EVSYS_CHANNEL_CHANNEL( eChannel );
	tChan.reg |= EVSYS_CHANNEL_EDGSEL( eEdge );
	tChan.reg |= EVSYS_CHANNEL_EVGEN( eGenerator );
    
	// determine the ASYNC/RESYNC based on usermux
	switch( eUserMux )
	{
		case EVENT_USERMUX_DMAC_CH0 :
		case EVENT_USERMUX_DMAC_CH1 :
		case EVENT_USERMUX_DMAC_CH2 :
		case EVENT_USERMUX_DMAC_CH3 :
			nTemp = EVSYS_CHANNEL_PATH_RESYNCHRONIZED_Val;
			break;

		default :
			nTemp = EVSYS_CHANNEL_PATH_ASYNCHRONOUS_Val;
			break;
	}
	tChan.reg |= EVSYS_CHANNEL_PATH( nTemp );
	EVSYS->CHANNEL.reg = tChan.reg;   

	// set up the user mux
	EVSYS->USER.reg = EVSYS_USER_USER( eUserMux ) | EVSYS_USER_CHANNEL( eChannel + 1 );
}

/******************************************************************************
 * @function Event_Close
 *
 * @brief turn off all events
 *
 * This function will turn off all events
 *
 *****************************************************************************/
void Event_Close( void )
{
  // reset the system
  EVSYS->CTRL.bit.SWRST = ON;
}

/******************************************************************************
 * @function 
 *
 * @brief 
 *
 * This function 
 *
 * @param[in]   
 *
 * @return      
 *
 *****************************************************************************/
//extern  Event IOCTL( EVENTENUM eEnum, EVENTIOCACTION eAction, PVOID pvArg );

/**@} EOF .c */
