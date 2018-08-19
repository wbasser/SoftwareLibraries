/******************************************************************************
 * @file LedManager_def.h
 *
 * @brief Led manager definitions
 *
 * This file provides the definitions for the LED manager
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
 * $Log: $
 * 
 *
 * \addtogroup LedManager
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _LEDMANAGER_DEF_H
#define _LEDMANAGER_DEF_H

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "LedManager/LedManager_prm.h"

// library includes -----------------------------------------------------------
#include "GPIO/Gpio.h"

// Macros and Defines ---------------------------------------------------------
/// define the helper macro for creating a direct drive led entry
#define LEDDEF_DIRECT( pin ) \
  { \
    .eDriveType   = LED_DRIVETYPE_DIRECT, \
    .tRowDrive    = \
    { \
      .eDrivePin  = pin, \
    }, \
  }
  
/// define the helper macro for creating a matrix drive led entry
#define LEDDEF_MATRIX( row, col ) \
  { \
    .eDriveType   = LED_DRIVETYPE_MATRIX, \
    .tRowDrive    = \
    { \
      .nRowIndex  = row, \
    }, \
    .nColIndex    = col \
  }

/// define the helper macro for creating a special drive led entry
#define LEDDEF_SPECIAL( func, col ) \
  { \
    .eDriveType   = LED_DRIVETYPE_SPECIAL, \
    .tRowDrive    = \
    { \
      .pvSpclFunc = func, \
    }, \
    .nColIndex    = col \
  }   
  
/// define the helper macro for creating an animation step
#define LEDMNGRDEFSTEP( action, led, duration, nextevent, option ) \
  { \
    .eAction = action, \
    .nLedEnum = led, \
    .wDurationMsecs = duration, \
    .eEvent = nextevent, \
    .nOption = option \
  }
  
/// define the helper macro for starting an animation sequence
#define LEDMNGRDEFSTART( name ) \
  static  const CODE  LEDSEQENTRY at ## name ## Defs[ ] = { \

/// define the helper macro for ending an animation sequence
#define LEDMNGRDEFSTOP \
  };
  
/// define the helper macro for defining an animation sequence
#define LEDMNGRANIMATIONDEF( name ) \
    ( const CODE PLEDSEQENTRY )&at ## name ## Defs \
  
/// define the helper macro for creating an RGB led
#define LEDMNGRRGBDEF( red, grn, blu ) \
  { \
    .nRedEnum = red, \
    .nGrnEnum = grn, \
    .nBluEnum = blu, \
  }

// enumerations ---------------------------------------------------------------
/// enumerate the LED drive types
typedef enum _LEDDRIVETYPE
{
  LED_DRIVETYPE_DIRECT = 0, ///< direct drive
  LED_DRIVETYPE_MATRIX,     ///< matrix drive
  LED_DRIVETYPE_SPECIAL,    ///< special type
  LED_DRIVETYPE_MAX
} LEDDRIVETYPE;

/// enumerate the LED actions
typedef enum _LEDACTION
{
  LED_ACTION_OFF = 0,           ///< turn off the led
  LED_ACTION_ON,                ///< turn on the led
  LED_ACTION_BLINKSLOW,         ///< blink the led slow
  LED_ACTION_BLINKFAST,         ///< blink the led fast
  LED_ACTION_PULSE,             ///< pulse the led on
  LED_ACTION_BLINKSLOW_LOCK,    ///< blink the led slow - disable all off/on changes
  LED_ACTION_BLINKFAST_LOCK,    ///< blink the led fast - disable all off/on changes
  LED_ACTION_PULSE_LOCK,        ///< pulse the led on - disable all off/on changes 
  LED_ACTION_MAX,               ///< individual max
  LED_ACTION_ALLOFF,            ///< all off
  LED_ACTION_ALLON,             ///< all on
  LED_ACTION_ALLBLINKSLOW,      ///< all blink slow
  LED_ACTION_ALLBLINKFAST,      ///< all blink fast
  LED_ACTION_ALLMAX,            ///< all max
} LEDACTION;

/// enumerate the RGB colors
typedef enum _LEDRGBCOLOR
{
  LED_RGBCOLOR_BLK = 0,         ///< black
  LED_RGBCOLOR_BLU,             ///< blue
  LED_RGBCOLOR_GRN,             ///< green
  LED_RGBCOLOR_CYN,             ///< cyan
  LED_RGBCOLOR_RED,             ///< red
  LED_RGBCOLOR_VIO,             ///< violet
  LED_RGBCOLOR_ORN,             ///< orange
  LED_RGBCOLOR_WHT,             ///< white
  LED_RGBCOLOR_MAX
} LEDRGBCOLOR;

/// enumerate the LED sequence events
typedef enum _LEDSEQEVENT
{
  LED_SEQEVENT_DONE = 0,    ///< sequence done
  LED_SEQEVENT_NEXT,        ///< next sequence
  LED_SEQEVENT_JMPBEG,      ///< jump to the beginning
  LED_SEQEVENT_JMPSEQ,      ///< jump to a step within this sequence
  LED_SEQEVENT_WAIT,        ///< wait till count is zero
  #if ( LEDMANAGER_ANIMATION_CALLSTACK_DEPTH != 0 )
  LED_SEQEVENT_CALL,        ///< call another sequence
  #endif
  LED_SEQEVENT_MAX    
} LEDSEQEVENT, *PLEDSEQEVENT;

// structures -----------------------------------------------------------------
/// define the function type for special LED drives
typedef void ( *PVLEDSPCLFUNC )( U8, BOOL );

/// define the 

/// define the definition structure for LEDS
typedef struct _LEDDEF
{
  LEDDRIVETYPE      eDriveType;   ///< drive type
  union
  {
    GPIOPINENUM     eDrivePin;    ///< direct type drive pin
    U8              nRowIndex;    ///< row index for matrix
    PVLEDSPCLFUNC   pvSpclFunc;   ///< special drive funciton
  } tRowDrive;                    ///< row drive
  U8                nColIndex;    ///< column index
} LEDDEF, *PLEDDEF;
#define LEDDEF_SIZE   sizeof( LEDDEF )

/// define the structure for a sequence entry
typedef struct _LEDSEQENTRY
{
  LEDACTION     eAction;        ///< LED action
  U8            nLedEnum;       ///< led enumeration
  U16           wDurationMsecs; ///< duration in msecs
  LEDSEQEVENT   eEvent;         ///< action to execute when this step is done
  U8            nOption;        ///< option used with jump or repeat events or extended events
} LEDSEQENTRY, *PLEDSEQENTRY;
#define LEDSEQENTRY_SIZE  sizeof( LEDSEQENTRY )

#if ( LEDMANAGER_RGB_LEDS_ENABLED == 1 )
/// define the structure for defining a RGB led
typedef struct _LEDRGBDEF
{
  U8            nRedEnum;       ///< red LED enumeration
  U8            nGrnEnum;       ///< green led enumeration
  U8            nBluEnum;       ///< blue led enumeration
} LEDRGBDEF, *PLEDRGBDEF;
#define LEDRGBDEF_SIZE        sizeof( LEDRGBDEF )
#endif  // LEDMANAGER_RGB_LEDS_ENABLED

/**@} EOF LedManager_def.h */

#endif  // _LEDMANAGER_DEF_H