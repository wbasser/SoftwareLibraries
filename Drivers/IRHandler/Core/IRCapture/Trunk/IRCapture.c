/******************************************************************************
 * @file IrCapture.c
 *
 * @brief IR handler implementation
 *
 * This file provides the IR handler implementation
 *
 * @copyright Copyright (c) 2012 Cyber Intergration
 * This document contains proprietary data and information of Cyber Integration 
 * LLC. It is the exclusive property of Cyber Integration, LLC and will not be 
 * disclosed in any form to any party without prior written permission of 
 * Cyber Integration, LLC. This document may not be reproduced or further used 
 * without the prior written permission of Cyber Integration, LLC.
 *
 * Version History
 * ======
 * $Log: $
 * 
 *
 * \addtogroup IrCapture
 * @{
 *****************************************************************************/

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "IRHandler/IRHandler_def.h"

// library includes -----------------------------------------------------------
#include "GPIO/Gpio.h"

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations ----------------------------------------------

// local parameter declarations -----------------------------------------------
static	U32UN	  tCapData;
static  BOOL    bCapInProg;

// local function prototypes --------------------------------------------------

// constant parameter initializations -----------------------------------------

/******************************************************************************
 * @function IrCapture_CaptureControl
 *
 * @brief IR handler capture control
 *
 * This function will turn off/on the capture
 *
 * @param[in]   bState    state of the capture run
 *
 *****************************************************************************/
void IRCapture_Control( BOOL bState )
{
  // reset the state
  bCapInProg = FALSE;

  // start/top the timer
  Timers_Ioctl( IRHANDLER_CAPTURE_TIMER, TIMER_IOCTL_STOPSTART, ( PVOID )&bState );
}

/******************************************************************************
 * @function IRCapture_Process
 *
 * @brief process the input capture
 *
 * This function processes the input capture callback
 *
 *****************************************************************************/
void IRCapture_Process( TIMERCBEVENT eEvent, U16 wCapture )
{
  // determine the event
  switch( eEvent )
  {
    case TIMER_CBEVENT_CAPFE :
      //  is this the first edge
      if ( !bCapInProg )
      {
        // set cap in progress
        bCapInProg = TRUE;
      }
      else
      {
        // store it
        tCapData.awValue[ IRCAP_INDEX_OFFTIME ] = wCapture;

        // put in queue
        QueueManager_PutTail( IRHANDLER_CAPTURE_QUEUE, ( PU8 )&tCapData.uValue );
      }
      break;
      
    case TIMER_CBEVENT_CAPRE :
      // store the data
      tCapData.awValue[ IRCAP_INDEX_ONTIME ] = wCapture;
      break;
      
    case TIMER_CBEVENT_TMO :
      // check for a valid timeout
      if ( bCapInProg )
      {
        // reset the state
        bCapInProg = FALSE;
      }
      break;
      
    default :
      break;
  }
}

/**@} EOF IrCapture.c */