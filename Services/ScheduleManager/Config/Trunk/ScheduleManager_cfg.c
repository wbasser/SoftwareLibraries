/******************************************************************************
 * @file ScheduleManager_cfg.c
 *
 * @brief Schedule manager configuration implementation 
 *
 * This file will provide the implementation of the configuration
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
 * \addtogroup ScheduleManager_cfg
 * @{
 *****************************************************************************/

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "ScheduleManager/ScheduleManager_cfg.h"

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations ----------------------------------------------

// local parameter declarations -----------------------------------------------

// local function prototypes --------------------------------------------------

// constant parameter initializations -----------------------------------------
/// fill in the schedule definition
const CODE  SCHDMNGRDEFTBL    tSchdDefTable =
{
  {
    // instantiate schedule entries here using the below macro
    // SCHDMNGRDEFM( sday, shr, smin, eday, ehr,  ) 
    SCHDMNGRDEFM(                    0,   0,    0,    6,  23,   59 ),   // SCHDMNGRDEF_ENUM_DFLT
    SCHDMNGRDEFM( SCHDMNGR_DOW_ILLEGAL,   0,    0,    0,   0,    0 ),   // SCHDMNGDEF_ENUM_USER01
    SCHDMNGRDEFM( SCHDMNGR_DOW_ILLEGAL,   0,    0,    0,   0,    0 ),   // SCHDMNGDEF_ENUM_USER02
    SCHDMNGRDEFM( SCHDMNGR_DOW_ILLEGAL,   0,    0,    0,   0,    0 ),   // SCHDMNGDEF_ENUM_USER03
    SCHDMNGRDEFM( SCHDMNGR_DOW_ILLEGAL,   0,    0,    0,   0,    0 ),   // SCHDMNGDEF_ENUM_USER04
    SCHDMNGRDEFM( SCHDMNGR_DOW_ILLEGAL,   0,    0,    0,   0,    0 ),   // SCHDMNGDEF_ENUM_USER05
    SCHDMNGRDEFM( SCHDMNGR_DOW_ILLEGAL,   0,    0,    0,   0,    0 ),   // SCHDMNGDEF_ENUM_USER06
    SCHDMNGRDEFM( SCHDMNGR_DOW_ILLEGAL,   0,    0,    0,   0,    0 ),   // SCHDMNGDEF_ENUM_USER07
    SCHDMNGRDEFM( SCHDMNGR_DOW_ILLEGAL,   0,    0,    0,   0,    0 ),   // SCHDMNGDEF_ENUM_USER08
    SCHDMNGRDEFM( SCHDMNGR_DOW_ILLEGAL,   0,    0,    0,   0,    0 ),   // SCHDMNGDEF_ENUM_USER09
    SCHDMNGRDEFM( SCHDMNGR_DOW_ILLEGAL,   0,    0,    0,   0,    0 ),   // SCHDMNGDEF_ENUM_USER10
    SCHDMNGRDEFM( SCHDMNGR_DOW_ILLEGAL,   0,    0,    0,   0,    0 ),   // SCHDMNGDEF_ENUM_USER11
    SCHDMNGRDEFM( SCHDMNGR_DOW_ILLEGAL,   0,    0,    0,   0,    0 ),   // SCHDMNGDEF_ENUM_USER12
    SCHDMNGRDEFM( SCHDMNGR_DOW_ILLEGAL,   0,    0,    0,   0,    0 ),   // SCHDMNGDEF_ENUM_USER13
    SCHDMNGRDEFM( SCHDMNGR_DOW_ILLEGAL,   0,    0,    0,   0,    0 ),   // SCHDMNGDEF_ENUM_USER14
    SCHDMNGRDEFM( SCHDMNGR_DOW_ILLEGAL,   0,    0,    0,   0,    0 ),   // SCHDMNGDEF_ENUM_USER15
    SCHDMNGRDEFM( SCHDMNGR_DOW_ILLEGAL,   0,    0,    0,   0,    0 ),   // SCHDMNGDEF_ENUM_USER16
    SCHDMNGRDEFM( SCHDMNGR_DOW_ILLEGAL,   0,    0,    0,   0,    0 ),   // SCHDMNGDEF_ENUM_USER17
    SCHDMNGRDEFM( SCHDMNGR_DOW_ILLEGAL,   0,    0,    0,   0,    0 ),   // SCHDMNGDEF_ENUM_USER18
    SCHDMNGRDEFM( SCHDMNGR_DOW_ILLEGAL,   0,    0,    0,   0,    0 ),   // SCHDMNGDEF_ENUM_USER19
    SCHDMNGRDEFM( SCHDMNGR_DOW_ILLEGAL,   0,    0,    0,   0,    0 ),   // SCHDMNGDEF_ENUM_USER20
    SCHDMNGRDEFM( SCHDMNGR_DOW_ILLEGAL,   0,    0,    0,   0,    0 ),   // SCHDMNGDEF_ENUM_USER21
    SCHDMNGRDEFM( SCHDMNGR_DOW_ILLEGAL,   0,    0,    0,   0,    0 ),   // SCHDMNGDEF_ENUM_USER22
    SCHDMNGRDEFM( SCHDMNGR_DOW_ILLEGAL,   0,    0,    0,   0,    0 ),   // SCHDMNGDEF_ENUM_USER23
    SCHDMNGRDEFM( SCHDMNGR_DOW_ILLEGAL,   0,    0,    0,   0,    0 ),   // SCHDMNGDEF_ENUM_USER24
    SCHDMNGRDEFM( SCHDMNGR_DOW_ILLEGAL,   0,    0,    0,   0,    0 ),   // SCHDMNGDEF_ENUM_USER25
    SCHDMNGRDEFM( SCHDMNGR_DOW_ILLEGAL,   0,    0,    0,   0,    0 ),   // SCHDMNGDEF_ENUM_USER26
    SCHDMNGRDEFM( SCHDMNGR_DOW_ILLEGAL,   0,    0,    0,   0,    0 ),   // SCHDMNGDEF_ENUM_USER27
    SCHDMNGRDEFM( SCHDMNGR_DOW_ILLEGAL,   0,    0,    0,   0,    0 ),   // SCHDMNGDEF_ENUM_USER28
    SCHDMNGRDEFM( SCHDMNGR_DOW_ILLEGAL,   0,    0,    0,   0,    0 ),   // SCHDMNGDEF_ENUM_USER29
    SCHDMNGRDEFM( SCHDMNGR_DOW_ILLEGAL,   0,    0,    0,   0,    0 ),   // SCHDMNGDEF_ENUM_USER30
    SCHDMNGRDEFM( SCHDMNGR_DOW_ILLEGAL,   0,    0,    0,   0,    0 ),   // SCHDMNGDEF_ENUM_USER31
  },

  // instantiate the rules for each entry here
  {
  },
};


/// fillin the external rules functions
const CODE  SCHDMNGREXTTBL  tSchdExtTable = 
{
  {
    // instantiate external rules here using the below macro
    LightLevelControl_SetRuleIndex,
    DALILevelControl_SetRuleIndex,
  }
};

/******************************************************************************
 * @function   ScheduleManager_CheckValidDateTime
 *
 * @brief check for a valid date/time
 *
 * This function will check for a valid date time
 *
 * @return      TRUE if valid time, FALSE otherwise
 * 
 *****************************************************************************/
BOOL ScheduleManager_CheckValidDateTime( void )
{
  BOOL  bValid = FALSE;

  // return the valid status
  return( bValid );
}

/******************************************************************************
 * @function   ScheduleManager_RGetDateTime
 *
 * @brief get an RTC value
 *
 * This function will fetch the current date/time from the appropriate system
 *
 * @param[in]   ptDateTime    pointer to a Date/Time struture
 *
 *****************************************************************************/
void ScheduleManager_GetDateTime( PDATETIME ptDateTime )
{
  // add the user function here
}

/**@} EOF ScheduleManager_cfg.c */
