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

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations ----------------------------------------------

// local parameter declarations -----------------------------------------------

// local function prototypes --------------------------------------------------

// constant parameter initializations -----------------------------------------

/******************************************************************************
 * @function Event_Initialize
 *
 * @brief initialize the event generators
 *
 * This function will process all defined event generators
 *
 *****************************************************************************/
void Event_Initialize( void )
{
  EVENTENUM           eEvent;
  PEVENTDEF           ptDef; 
  EVSYS_CHANNEL_Type  tChan;
  EVSYS_USER_Type     tUser;
  U8                  nTemp;

  // reset the system
  EVSYS->CTRL.bit.SWRST = ON;

  // set the clock to always on
  EVSYS->CTRL.bit.GCLKREQ = ON;

  // for each defined event generator
  for ( eEvent = 0; eEvent < EVENT_ENUM_MAX; eEvent++ )
  {
    // get a pointer to the definition
    ptDef = ( PEVENTDEF )&atEventDefs[ eEvent ];

    // set up the channel
    tChan.reg = EVSYS_CHANNEL_CHANNEL( ptDef->eChannel );
    tChan.reg |= EVSYS_CHANNEL_EDGSEL( ptDef->eEdge );
    tChan.reg |= EVSYS_CHANNEL_EVGEN( ptDef->eGenerator );
    
    // determine the ASYNC/RESYNC based on usermux
    switch( ptDef->eUserMux )
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
    tUser.reg = EVSYS_USER_USER( ptDef->eUserMux );
    tUser.reg |= EVSYS_USER_CHANNEL( ptDef->eChannel + 1 );
    EVSYS->USER.reg = tUser.reg;
  }
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
