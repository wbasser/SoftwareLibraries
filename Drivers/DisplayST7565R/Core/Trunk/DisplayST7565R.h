/******************************************************************************
 * @file DisplayST7565R.h
 *
 * @brief 
 *
 * This file 
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
 * \addtogroup DisplayST7565R
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _DISPLAYST7565R_H
#define _DISPLAYST7565R_H

// system includes ------------------------------------------------------------

// library includes -----------------------------------------------------------

// local includes -------------------------------------------------------------
#include "DisplayST7565R/DisplayST7565R_prm.h"

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------
/// enumerate the actions
typedef enum _DISPLAYPIXACT
{
  DISPLAY_PIXACT_CLR = 0,
  DISPLAY_PIXACT_SET,
  DISPLAY_PIXACT_TGL
} DISPLAYPIXACT;

// structures -----------------------------------------------------------------

// global parameter declarations -----------------------------------------------

// global function prototypes --------------------------------------------------
extern  void  DisplayST7565R_Initialize( void );
extern  void  DisplayST7565R_SetBrightness( U8 nBrightness );
extern  void  DisplayST7565R_ClearScreen( void );
extern  void  DisplayST7565R_SetPixel( U8 nX, U8 nY, DISPLAYPIXACT eAction );
extern  void  DisplayST7565R_WriteBuffer( void );

/**@} EOF DisplayST7565R.h */

#endif  // _DISPLAYST7565R_H