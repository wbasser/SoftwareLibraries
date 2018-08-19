/******************************************************************************
 * @file DigitalPot.h
 *
 * @brief Digital potentiometer definition file
 *
 * This file provides the declarations for the digial pot definition
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
 * \addtogroup DigitalPot
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _DIGITALPOT_DEF_H
#define _DIGITALPOT_DEF_H

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------
/// define the helper macro for defining a up/down pot
typedef DIGPOTDEF_UPDN( numtaps, incpin, updnpin, cenpin ) \
  { \
    .eType                    = DIGPOT_TYPE_UPDN, \
    .wNumTaps                 = numtaps, \
    .tPins.tUpDn.nIncrement   = incpin, \
    .tPins.tUpDn.nUpDown      = updnpin, \
    .tPins.tUpDn.nChipEnable  = cenpin, \
  }
  
// enumerations ---------------------------------------------------------------
/// enumerate the digital pot types
typedef enum  _DIGPOTTYPE
{
  DIGPOT_TYPE_UPDN = 0,     ///< up/down
  DIGPOT_TYPE_PBUT,         ///< push button
  DIGPOT_TYPE_I2C,          ///< I2C
  DIGPOT_TYPE_SPI,          ///< SPI
} DIGPOTTYPE;

// structures -----------------------------------------------------------------
/// define the definition struction for a digital pot
typedef struct _DIFPOTDEF
{
  DIGPOTTYPE    eType;      ///< digital pot type
  U16           wNumTaps;   ///< number of taps
  union
  {
    struct
    {
      U8  nIncrement;       ///< increment pin
      U8  nUpDown;          ///< up/down select
      U8  nChipEnable;      ///< chip enable
    } tUpDn;
    struct
    {
    } tPbut;
    struct
    {
    } tSpi;
  } tPins
} DIFPOTDEF, *PDIFPOTDEF;
#define DIFPOTDEF_SIZE        sizeof( DIFPOTDEF )

/**@} EOF DigitalPot_def.h */

#endif  // _DIGITALPOT_DEF_H