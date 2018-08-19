/******************************************************************************
 * @file NeoPixelHandler.h
 *
 * @brief Neo Pixel LED controller handler declarations 
 *
 * This file provides the declarations for the Neo Pixel LED controller
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
 * \addtogroup NeoPixelHandler
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _NEOPIXELHANDLER_H
#define _NEOPIXELHANDLER_H

// system includes ------------------------------------------------------------
#include "Types/Types.h"

// local includes -------------------------------------------------------------

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------
/// define the global inclusive device index
#define NEOPIXEL_ALL_DEVICES          ( 0xFF )

// enumerations ---------------------------------------------------------------
/// enumerate the Neo Pixel types
typedef enum _NEOPIXELTYPE
{
  NEOPIXEL_TYPE_RGB = 0,        ///<  0 - Red, Grn, Blu
  NEOPIXEL_TYPE_RBG,            ///<  1 - Red, Blu, Grn
  NEOPIXEL_TYPE_BRG,            ///<  2 - Blu, Red, Grn
  NEOPIXEL_TYPE_BGR,            ///<  3 - Blu, Grn, Red
  NEOPIXEL_TYPE_GBR,            ///<  4 - Grn, Blu, Red
  NEOPIXEL_TYPE_GRB,            ///<  5 - Grn, Red, Blu
  NEOPIXEL_TYPE_RGBW,           ///<  6 - Red, Grn, Blu, Wht
  NEOPIXEL_TYPE_RBGW,           ///<  7 - Red, Blu, Grn, Wht
  NEOPIXEL_TYPE_BRGW,           ///<  8 - Blu, Red, Grn, Wht
  NEOPIXEL_TYPE_BGRW,           ///<  9 - Blu, Grn, Red, Wht
  NEOPIXEL_TYPE_GBRW,           ///< 10 - Grn, Blu, Red, Wht
  NEOPIXEL_TYPE_GRBW,           ///< 11 - Grn, Red, Blu, Wht
  NEOPIXEL_TYPE_RGWB,           ///< 12 - Red, Grn, Wht, Blu
  NEOPIXEL_TYPE_RBWG,           ///< 13 - Red, Blu, Wht, Grn
  NEOPIXEL_TYPE_BRWG,           ///< 14 - Blu, Red, Wht, Grn
  NEOPIXEL_TYPE_BGWR,           ///< 15 - Blu, Grn, Wht, Red
  NEOPIXEL_TYPE_GBWR,           ///< 16 - Grn, Blu, Wht, Red
  NEOPIXEL_TYPE_GRWB,           ///< 17 - Grn, Red, Wht, Blu
  NEOPIXEL_TYPE_RWGB,           ///< 18 - Red, Wht, Grn, Blu
  NEOPIXEL_TYPE_RWBG,           ///< 19 - Red, Wht, Blu, Grn
  NEOPIXEL_TYPE_BWRG,           ///< 20 - Blu, Wht, Red, Grn
  NEOPIXEL_TYPE_BWGR,           ///< 21 - Blu, Wht, Grn, Red
  NEOPIXEL_TYPE_GWBR,           ///< 22 - Grn, Wht, Blu, Red
  NEOPIXEL_TYPE_GWRB,           ///< 23 - Grn, Wht, Red, Blu
  NEOPIXEL_TYPE_WRGB,           ///< 24 - Wht, Red, Grn, Blu
  NEOPIXEL_TYPE_WRBG,           ///< 25 - Wht, Red, Blu, Grn
  NEOPIXEL_TYPE_WBRG,           ///< 26 - Wht, Blu, Red, Grn
  NEOPIXEL_TYPE_WBGR,           ///< 27 - Wht, Blu, Grn, Red
  NEOPIXEL_TYPE_WGBR,           ///< 28 - Wht, Grn, Blu, Red
  NEOPIXEL_TYPE_WGRB,           ///< 29 - Wht, Grn, Red, Blu
  NEOPIXEL_TYPE_MAX
} NEOPIXELTYPE, *PNEOPIXELTYPE;

/// enumerate the operational speed
typedef enum _NEOPIXELSPEED
{
  NEOPIXEL_SPEED_400HZ = 0,
  NEOPIXEL_SPEED_800HZ,
  NEOPIXEL_SPEED_MAX
} NEOPIXELSPEED, *PNEOPIXELSPEED;

// structures -----------------------------------------------------------------

// global parameter declarations -----------------------------------------------

// global function prototypes --------------------------------------------------
extern  void  NeoPixelHandler_Initialize( void );
extern  void  NeoPixelHandler_SetConfiguration( NEOPIXELSPEED eSpeed, NEOPIXELTYPE eType, U8 nNumDevices );
extern  void  NeoPixelHandler_GetConfiguration( PNEOPIXELSPEED peSpeed, PNEOPIXELTYPE peType, PU8 pnNumDevices );
extern  void  NeoPixelHandler_SetPixelColor( U8 nDeviceIndex, U8 nRed, U8 nGrn, U8 nBlu, U8 nWht );
extern  void  NeoPixelHandler_Refresh( void );

/**@} EOF NeoPixelHandler.h */

#endif  // _NEOPIXELHANDLER_H