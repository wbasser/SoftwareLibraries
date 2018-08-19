/******************************************************************************
 * @file SystemDefines_prm.h
 *
 * @brief system defines declarations
 *
 * This file provides the system definitions
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
 * \addtogroup SystemDefines
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _SYSTEMDEFINES_PRM_H
#define _SYSTEMDEFINES_PRM_H

// Macros and Defines ---------------------------------------------------------
/// define the operating system types
#define SYSTEMDEFINE_OS_TASKMANAGER               ( 0 )
#define SYSTEMDEFINE_OS_FREERTOS                  ( 1 )
#define SYSTEMDEFINE_OS_LINUX                     ( 2 )

/// define the selected operating system here using one of the above macros
#define SYSTEMDEFINE_OS_SELECTION                 ( SYSTEMDEFINE_OS_TASKMANAGER )

/// define the system password for configuration reset
#define SYSTEMDEFINE_CONFIG_RESET_DEFAULT         ( 0xBEEFDEAD )

/// define the system password for log reset
#define SYSTEMDEFINE_LOGENTRIES_RESET             ( 0xDEAFFEED )

/**@} EOF SystemDefines.h */

#endif  // _SYSTEMDEFINES_PRM_H