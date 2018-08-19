/******************************************************************************
 * @file RTCManager_cfg.c
 *
 * @brief RTC Manager configuration implemntation file 
 *
 * This file provides the implementation for the configuration of the RTC manager
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
 * \addtogroup RTCManager
 * @{
 *****************************************************************************/

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "RTCManager/RTCManager_cfg.h"

// library includes -----------------------------------------------------------
#include "RTC/Rtc.h"

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations ----------------------------------------------

// local parameter declarations -----------------------------------------------

// local function prototypes --------------------------------------------------

// constant parameter initializations -----------------------------------------

/******************************************************************************
 * @function RTCManager_GetExternalRTC
 *
 * @brief get external RTC time
 *
 * This function will call the appropriate external RTC handler to get
 * the external RTC time
 *
 * @param[i0]   ptDateTime    pointer to store the returned data time
 *
 *****************************************************************************/
void RTCManager_GetExternalRTC( PDATETIME ptDateTime )
{
}

/******************************************************************************
 * @function RTCManager_GetInternalRTC
 *
 * @brief get the internal RTC time
 *
 * This function will call the appropriate internal RTC handler to get
 * the internal RTC time
 *
 * @param[i0]   ptDateTime    pointer to store the returned data time
 *
 *****************************************************************************/
void RTCManager_GetInternalRTC( PDATETIME ptDateTime )
{
  // get the internal date time
  Rtc_GetDateTime( ptDateTime );
}
 
/******************************************************************************
 * @function RTCManager_SetExternalRTC
 *
 * @brief get external RTC time
 *
 * This function will call the appropriate external RTC handler to get
 * the external RTC time
 *
 * @param[i0]   ptDateTime    pointer to store the returned data time
 *
 *****************************************************************************/
void RTCManager_SetExternalRTC( PDATETIME ptDateTime )
{
}

/******************************************************************************
 * @function RTCManager_SetInternalRTC
 *
 * @brief set the internal RTC time
 *
 * This function will call the appropriate internal RTC handler to get
 * the internal RTC time
 *
 * @param[i0]   ptDateTime    pointer to store the returned data time
 *
 *****************************************************************************/
void RTCManager_SetInternalRTC( PDATETIME ptDateTime )
{
  // get the internal date time
  Rtc_SetDateTime( ptDateTime );
}
 
/**@} EOF RTCManager_cfg.c */
