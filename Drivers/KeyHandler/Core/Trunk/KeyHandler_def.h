/******************************************************************************
 * @file KeyHandler_def.h
 *
 * @brief Key handler configuration declarations 
 *
 * This file 
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
 * \addtogroup KeyHandler
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _KEYHANDLER_DEF_H
#define _KEYHANDLER_DEF_H

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------

// library includes -----------------------------------------------------------
#include "Types/Types.h"
#include "GPIO/Gpio.h"

// Macros and Defines ---------------------------------------------------------
/// define the helper macro for defining direct entries
#define KEYHAND_DIRECT( col ) \
  { \
    .eType = KEYHAND_TYPE_DIRECT, \
    .eColEnum = col, \
  }
  
/// define the helper macro for defining row col entries
#define KEYHAND_ROWCOL( row, col ) \
  { \
    .eType = KEYHAND_TYPE_ROWCOL, \
    .eRowEnum = row, \
    .eColEnum = col, \
  }

/// define the helper macro for defining the special
#define KEYHAND_SPECIAL( index, func ) \
  { \
    .eType = KEYHAND_TYPE_SPECIAL, \
    .eColEnum = index, \
    .pvGetFunc = func, \
  }
  
// enumerations ---------------------------------------------------------------
/// enumerate the key type
typedef enum  _KEYHANDTYPE
{
  KEYHAND_TYPE_DIRECT = 0,      ///< direct read
  KEYHAND_TYPE_ROWCOL,          ///< row col type
  KEYHAND_TYPE_SPECIAL,
  KEYHAND_TYPE_MAX
} KEYHANDTYPE;

// structures -----------------------------------------------------------------
/// define the special get function
typedef BOOL  ( *PVGETFUNC )( U8 );

/// define the structure to define a key entry
typedef struct _KEYHANDDEF
{
  KEYHANDTYPE   eType;          ///< key type
  GPIOPINENUM   eRowEnum;       ///< row GPIO enum
  GPIOPINENUM   eColEnum;       ///< col GPIO enum
  PVGETFUNC     pvGetFunc;      ///< get function for special
} KEYHANDDEF, *PKEYHANDDEF;
#define KEYHANDDEF_SIZE       sizeof( KEYHANDDEF)

/**@} EOF KeyHandler_def.h */

#endif  // _KEYHANDLER_DEF_H