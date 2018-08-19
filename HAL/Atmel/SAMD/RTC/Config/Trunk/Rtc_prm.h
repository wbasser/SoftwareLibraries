/******************************************************************************
 * @file Rtc_prm.h
 *
 * @brief Real Clock Time Paratemer declarations
 *
 * This file 
 *
 * @copyright Copyright (c) 2014 CyberIntegration
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
 * \addtogroup RTC
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _RTC_PRM_H
#define _RTC_PRM_H

// library includes -----------------------------------------------------------
#include "TYpes/Types.h"

// Macros and Defines ---------------------------------------------------------
/// define the operating system selection
#define RTC_OS_SELECT           ( TIME_OS_MICRODOS )

/// define the log RTC set enable macro
#define RTC_ENABLE_TIMESET_LOG  ( 0 )

/**@} EOF Rtc_prm.h */

#endif  // _RTC_PRM_H