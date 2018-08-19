/******************************************************************************
 * @file AlarmHandler_prm.h
 *
 * @brief Alarm Handler parameter declarations 
 *
 * This file provides the alarm handler declarations
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
 * \addtogroup AlarmHandler
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _ALARMHANDLER_PRM_H
#define _ALARMHANDLER_PRM_H

// system includes ------------------------------------------------------------
#include "Types/Types.h"

// Macros and Defines ---------------------------------------------------------
/// define the argument types
#define ALARMHANDLER_TYPE_SIGNED16             ( 0 )
#define ALARMHANDLER_TYPE_SIGNED32             ( 1 )
#define ALARMHANDLER_TYPE_FLOAT                ( 2 )

/// define the sensor manager argument type using on eof the three above defines
#define ALARMHANDLER_ARGUMENT_TYPE             ( ALARMHANDLER_TYPE_SIGNED16 )

/**@} EOF AlarmHandler_prm.h */

#endif  // _ALARMHANDLER_PRM_H