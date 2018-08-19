/******************************************************************************
 * @file Micro_def.h
 *
 * @brief declarations
 *
 * This file declares the initialization structures for main
 *
 * @copyright Copyright (c) 2012 CyberIntegration
 * This document contains proprietary data and information of CyberIntegration 
 * LLC. It is the exclusive property of CyberIntegration, LLC and will not be 
 * disclosed in any form to any party without prior written permission of 
 * CyberIntegration, LLC. This document may not be reproduced or further used 
 * without the prior written permission of CyberIntegration, LLC.
 *
 * Version History
 * ======
 * $Rev: $
 * 
 *
 * \addtogroup Micro
 * @{
 *****************************************************************************/

// ensure only one instatiation
#ifndef _MICRO_DEF_H
#define _MICRO_DEF_H

// local includes -------------------------------------------------------------

// library includes -----------------------------------------------------------
#include "Types/Types.h"

// Macros and Defines ---------------------------------------------------------
/// define the initialization functions
typedef void  ( *PVMICROINITFUNC )( void );

/// define the dile functions
typedef void  ( *PVMICROIDLEFUNC )( void );

/// define the shutdown functions
typedef void  ( *PVMICROSHUTDNFUNC )( void );

/**@} EOF Micro_def.c */

#endif // _MICRO_DEF_H
