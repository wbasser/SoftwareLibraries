/******************************************************************************
 * @file RTCManager_prm.h
 *
 * @brief RTC Manager parameters files 
 *
 * This file provides the configuration parameters for the RTC manager
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
#ifndef _RTCMANAGER_PRM_H
#define _RTCMANAGER_PRM_H

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------
/// define the Network Time Protocol available
#define RTCMANAGER_NTP_AVAILABLE          ( 0 )

/// define the internal/external RTCs
#define RTCMANAGER_INTERNAL_RTC           ( 0 )
#define RTCMANAGER_EXTERNAL_RTC           ( 1 )

/// define the RTC selection
#define RTCMANAGER_INTEXT_SELECTION       ( RTCMANAGER_INTERNAL_RTC )

/// define the macro to enable debug commands
#define RTCMANAGER_ENABLE_DEBUG_COMMANDS  ( 1 )

/**@} EOF RTCManager_prm.h */

#endif  // _RTCMANAGER_PRM_H