/******************************************************************************
 * @file ButtonManager_cfg.c
 *
 * @brief button manager configuration implementation
 *
 * This file provides the configuration for the button manager
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
 * $Log: $
 * 
 *
 * \addtogroup ButtonManager
 * @{
 *****************************************************************************/

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "ButtonManager/ButtonManager_cfg.h"

// library includes -----------------------------------------------------------
#include "TaskManager/TaskManager.h"

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations ----------------------------------------------

// local parameter declarations -----------------------------------------------

// local function prototypes --------------------------------------------------

// constant parameter initializations -----------------------------------------
/// fill out the defaults for the config
const CODE BTNMNGRCFG tBtnMgrCfg = 
{
  // BTNMNGR_CFG_ENTRY( debnc, repdly, reprate, sh_time, lh_time, stuck_time )
  BTNMNGR_CFG_ENTRY( 50, 500, 100, 1000, 3000, 7000 )
};

/// fill out the button defs
const CODE BTNMNGRDEF atBtnMgrDefs[ BTNMNGR_ENUM_MAX ]  =
{
  // BTNMNGR_DEFCB_ENTRY( keyenum, rel_enb, prs_enb, rep_enb, shh_enb, lng_enb, tgl_enb, callback )
  // BTNMNGR_DEFEVENT_ENTRY( keyenum, rel_enb, prs_enb, rep_enb, shh_enb, lng_enb, tgl_enb, task )
};


/******************************************************************************
 * @function ButtonManager_GetKeyStatus
 *
 * @brief button manager get key status
 *
 * This function calls the appropriate user defined function to return the
 * status of a given key enumeration
 *
 * @param[in]   nKeyEnum      key enumeration
 *  
 * @return      returns TRUE for key pressed, FALSE otherwise
 *
 *****************************************************************************/
BOOL ButtonManager_GetKeyStatus( U8 nKeyEnum )
{
  BOOL bButtonStatus = FALSE;
  
  // instantiate the call to the appropriate handler to test for key status
  
  // return the button status
  return( bButtonStatus );
}


/**@} EOF ButtonManager_cfg.c */
