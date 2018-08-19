/******************************************************************************
 * @file SystemTick_def.h
 *
 * @brief configuration file declarations
 *
 * This file declares the initialization structures for system tick
 *
 * @copyright Copyright (c) 2012 Nan Technologies
 * This document contains proprietary data and information of Nan Technologies 
 * LLC. It is the exclusive property of Nan Technologies, LLC and will not be 
 * disclosed in any form to any party without prior written permission of 
 * Nan Technologies, LLC. This document may not be reproduced or further used 
 * without the prior written permission of Nan Technologies, LLC.
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

// local includes -------------------------------------------------------------

// library includes -----------------------------------------------------------
#include "Types/Types.h"

// Macros and Defines ---------------------------------------------------------
/// define the system tick functions
typedef void ( *PVSYSTEMTICKFUNC )( void );

/**@} EOF SystemTick_def.c */

#endif // _SYSTEMTICK_DEF_H
