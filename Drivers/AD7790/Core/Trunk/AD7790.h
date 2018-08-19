/******************************************************************************
 * @file AD7790.h
 *
 * @brief AD7790 ADC declarations 
 *
 * This file provides the declarations for the AD7790 A2D converter
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
 * \addtogroup AD7790
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _AD7790_H
#define _AD7790_H

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------

// library includes -----------------------------------------------------------
#include "Types/Types.h"

// Macros and Defines ---------------------------------------------------------
/// define the reference voltage
#define AD7790_VREF_VOLTS             ( 1.2 )

// enumerations ---------------------------------------------------------------
/// enumerate the operationg modes
typedef enum _AD7790MODE
{
  AD7790_MODE_CONTINUOUS = 0,   ///< continuous mode
  AD7790_MODE_RESERVED,         ///< reserved
  AD7790_MODE_SINGLECONV,       ///< single conversion
  AD7790_MODE_POWERDOWN,        ///< power down
  AD7790_MODE_MAX
} AD7790MODE;

/// enumerate the analog input ranges
typedef enum _AD7790RANGE
{
  AD7790_RANGE_VREF = 0,        ///< VREF
  AD7790_RANGE_VREF2,           ///< VREF / 2
  AD7790_RANGE_VREF4,           ///< VREF / 4
  AD7790_RANGE_VREF8,           ///< VREF / 8
  AD7790_RANGE_MAX
} AD7790RANGE;

/// enumerate the filter values
typedef enum _AD7790FILT
{
  AD7790_FILT_120HZ = 0,        ///< 120HZ - 25db@60HZ
  AD7790_FILT_100HZ,            ///< 100HZ - 25db@50HZ
  AD7790_FILT_33HZ3,            ///< 33.3HZ - 
  AD7790_FILT_20HZ,             ///< 20HZ - 80db@60HZ
  AD7790_FILT_16HZ6,            ///< 16.6HZ - 65db@50/60HZ
  AD7790_FILT_16HZ7,            ///< 16.7HZ - 80db@50HZ
  AD7790_FILT_13HZ3,            ///< 13.3HZ
  AD7790_FILT_9HZ5,             ///< 9.5HZ - 62DB@50/60HZ
  AD7790_FILT_MAX
} AD7790FILT;

// structures -----------------------------------------------------------------

// global parameter declarations -----------------------------------------------

// global function prototypes --------------------------------------------------
extern  void  AD7790_Initialize( void );
extern  void  AD7790_Setup( AD7790MODE eMode, AD7790RANGE eRange, AD7790FILT eFilter );
extern  void  AD7790_StartConversion( void );
extern  U16   AD7790_GetConversionCounts( void );
extern  FLOAT AD7790_GetConversionVolts( void );

/**@} EOF AD7790.h */

#endif  // _AD7790_H