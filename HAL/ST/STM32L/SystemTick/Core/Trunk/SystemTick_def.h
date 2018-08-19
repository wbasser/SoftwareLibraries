/******************************************************************************
 * @file SystemTick_def.h
 *
 * @brief configuration file declarations
 *
 * This file declares the initialization structures for system tick
 *
 * @copyright Copyright (c) 2012 Cyber Integration
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
 * \addtogroup SystemTick
 * @{
 *****************************************************************************/

// ensure only one instatiation
#ifndef _SYSTEMTICK_DEF_H
#define _SYSTEMTICK_DEF_H

// library includes -----------------------------------------------------------
#include "Types/Types.h"

// local includes -------------------------------------------------------------

// Macros and Defines ---------------------------------------------------------
/// define the system tick functions
typedef void ( *PVSYSTEMTICKFUNC )( void );

/**@} EOF Micro_def.c */

#endif // _MICRO_DEF_H
