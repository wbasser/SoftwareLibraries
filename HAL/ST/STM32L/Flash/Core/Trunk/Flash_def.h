/******************************************************************************
 * @file Flash_def.h
 *
 * @brief flash module definitions 
 *
 * This file provides the definitions for the flash module
 *
 * @copyright Copyright (c) 2012 Cyber Integration
 * This document contains proprietary data and information of Cyber Integration 
 * LLC. It is the exclusive property of Cyber Integration, LLC and will not be 
 * disclosed in any form to any party without prior written permission of 
 * Cyber Integration, LLC. This document may not be reproduced or further used 
 * without the prior written permission of Cyber Integration, LLC.
 *
 * $Date: $
 *
 * Version History
 * ======
 * $Rev: $
 * 
 *
 * \addtogroup Flash
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _FLASH_DEF_H
#define _Flash_def_H

// system includes ------------------------------------------------------------
#include "Types/Types.h"

// local includes -------------------------------------------------------------

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------
/// define the helper macro for defining the flash definition
#define FLASHDEFM( preenb, icenb, dcenb, wait ) \
    .bPrefetchEnb  = preenb, \
    .bInstCacheEnb = icenb, \
    .bDataCacheEnb = dcenb, \
    .eWaitStates   = wait \
  
// enumerations ---------------------------------------------------------------
/// enumerate the number of wait states
typedef enum _FLASHWAIT
{
  FLASH_WAIT_0 = 0,       ///< zero wait states
  FLASH_WAIT_1,           ///< one wait state
  FLASH_WAIT_2,           ///< two wait states
  FLASH_WAIT_3,           ///< three wait states
  FLASH_WAIT_4,           ///< four wait states
  FLASH_WAIT_MAX          
} FLASHWAIT;

// structures -----------------------------------------------------------------
typedef struct _FLASHDEF
{
  BOOL      bPrefetchEnb;
  BOOL      bInstCacheEnb;
  BOOL      bDataCacheEnb;
  FLASHWAIT eWaitStates;
} FLASHDEF, *PFLASHDEF;
#define FLASHDEF_SIZE             ( FLASHDEF )
/**@} EOF Flash_def.h */

#endif  // _FLASH_DEF_H