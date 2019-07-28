/******************************************************************************
 * @file RTCManager_cfg.h
 *
 * @brief RTC manager configuration declarations
 *
 * This file provides the declarations for the RTC manager configuration file
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
 
// ensure only one instantiation
#ifndef _RTCMANAGER_CFG_H
#define _RTCMANAGER_CFG_H

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------

// library includes -----------------------------------------------------------
#include "TimeHandler/TimeHandler.h"

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations -----------------------------------------------

// global function prototypes --------------------------------------------------
extern  void  RTCManager_GetExternalRTC( PDATETIME ptDateTime );
extern  void  RTCManager_GetInternalRTC( PDATETIME ptDateTime );
extern  void  RTCManager_SetExternalRTC( PDATETIME ptDateTime );
extern  void  RTCManager_SetInternalRTC( PDATETIME ptDateTime );

/**@} EOF RTCManager_cfg.h */

#endif  // _RTCMANAGER_CFG_H