/******************************************************************************
 * @file SleepManager.c
 *
 * @brief sleep manager implementation
 *
 * This file provides the implementation for the sleep manager
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
 * $Rev: $
 * 
 *
 * \addtogroup SleepManager
 * @{
 *****************************************************************************/

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "SleepManager/SleepManager.h"

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations ----------------------------------------------

// local parameter declarations -----------------------------------------------

// local function prototypes --------------------------------------------------

// constant parameter initializations -----------------------------------------

/******************************************************************************
 * @function  SleepManager_Initialize
 *
 * @brief initialization
 *
 * This function will perform any needed initialization
 *
 *****************************************************************************/
void SleepManager_Initialize( void )
{
}

/******************************************************************************
 * @function  SleepManager_EnterSleepState
 *
 * @brief enter sleep mode
 *
 * This function will cause the micro to enter sleep mode
 *
 * @param[in]   esleepMode      desired sleep mode
 *
 *****************************************************************************/
void SleepManager_EnterSleepState( SLEEPMNGRSLEEPMODE eSleepMode )
{
  // set the sleep mode
  switch ( eSleepMode )
  {
    case SLEEPMNGR_SLEEP_MODE_IDLE_0 :
    case SLEEPMNGR_SLEEP_MODE_IDLE_1 :
    case SLEEPMNGR_SLEEP_MODE_IDLE_2 :
      // set the sleep mode
      PM->SLEEP.reg = eSleepMode;

      // clear the deep sleep
      SCB->SCR &= ~SCB_SCR_SLEEPDEEP_Msk;
      break;

    case SLEEPMNGR_SLEEP_MODE_STDBY :
      // set the sleep mode
      PM->SLEEP.reg = eSleepMode;

      // clear the deep sleep
      SCB->SCR != SCB_SCR_SLEEPDEEP_Msk;
      break;
  }

  // execute a WFI instruction
  __ASM( "WFI" );
}

/**@} EOF SleepManager.c */
