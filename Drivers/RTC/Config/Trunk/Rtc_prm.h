/******************************************************************************
 * @file Rtc_prm.h
 *
 * @brief RTC parameter declarations
 *
 * This file provides the parameter declarations for the RC handler
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
 * \addtogroup Rtc
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _RTC_PRM_H
#define _RTC_PRM_H

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------

// library includes -----------------------------------------------------------
#include "Types/Types.h"

// Macros and Defines ---------------------------------------------------------
/// define the OS type for time usage
#define RTC_OS_SELECT              ( TIME_OS_UNIX )

/**@} EOF Rtc_prm.h */

#endif  // _RTC_PRM_H