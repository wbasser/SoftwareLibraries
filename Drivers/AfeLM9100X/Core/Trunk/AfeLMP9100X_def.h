/******************************************************************************
 * @file AfeLMP9100X_def.h
 *
 * @brief Analog Front End LMP9100 definitions file  
 *
 * This file provides the definition declarations for the LMP91000 AFE chip
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
 * \addtogroup AfeLMP9100X
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _AFELMP9100X_DEF_H
#define _AFELMP9100X_DEF_H

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------

// library includes -----------------------------------------------------------
#include "GPIO\Gpio.h"
#include "I2C\I2c.H"

// Macros and Defines ---------------------------------------------------------
/// define the helper macro for defining the control
#define AFELMLP9100XCTLDEFM( i2c, gpio ) \
  { \
    .eI2cDev    = i2c, \
    .eGpioPin   = gpio, \
  }
  
// enumerations ---------------------------------------------------------------
/// enumerate the gain values
typedef enum _AFELMP9100XTIAGAIN
{
  AFELMP9100X_TIAGAIN_EXTERNAL = 0,        ///< external gain resistor
  AFELMP9100X_TIAGAIN_INT2K75,             ///< internal 2K75 
  AFELMP9100X_TIAGAIN_INT3K5,              ///< internal 3K
  AFELMP9100X_TIAGAIN_INT7K,               ///< internal 7K
  AFELMP9100X_TIAGAIN_INT14K,              ///< internal 14K
  AFELMP9100X_TIAGAIN_INT35K,              ///< internal 35K
  AFELMP9100X_TIAGAIN_INT120K,             ///< internal 120K
  AFELMP9100X_TIAGAIN_INT350K,             ///< internal 350K
  AFELMP9100X_TIAGAIN_MAX
} AFELMP9100XTIAGAIN;

/// enumerate the load
typedef enum _AFELMP9100XTIARLOAD
{
  AFELMP9100X_TIARLOAD_10 = 0,             ///< 10 ohm load
  AFELMP9100X_TIARLOAD_33,                 ///< 33 ohm load
  AFELMP9100X_TIARLOAD_50,                 ///< 50 ohm load
  AFELMP9100X_TIARLOAD_100,                ///< 100 ohm load
  AFELMP9100X_TIARLOAD_MAX
} AFELMP9100XTIARLOAD;

/// enumerate the ref souce
typedef enum _AFELMP9100XREFSOURCE
{
  AFELMP9100X_REFSOURCE_INT = 0,           ///< internal reference source
  AFELMP9100X_REFSOURCE_EXT,               ///< external reference source
  AFELMP9100X_REFSOURCE_MAX
} AFELMP9100XREFSOURCE;

/// enumerate the internal zero
typedef enum _AFELMP9100XREFINTZ
{
  AFELMP9100X_REFINTZ_20PCT = 0,          ///< 20% 
  AFELMP9100X_REFINTZ_50PCT,              ///< 50%
  AFELMP9100X_REFINTZ_67PCT,              ///< 67%
  AFELMP9100X_REFINTZ_BYPASSED,           ///< bypassed
  AFELMP9100X_REFINTZ_MAX
} AFELMP9100XREFINTZ;

/// enumerate the bias sign
typedef enum _AFELMP9100XREFBIAS
{
  AFELMP9100X_REFBIAS_NEG = 0,            ///< negative bias
  AFELMP9100X_REFBIAS_POS,                ///< positivie bias
  AFELMP9100X_REFBIAS_MAX
} AFELMP9100XREFBIAS;

/// enumerate the bias PCT
typedef enum _AFELMP9100XREFBPCT
{
  AFELMP9100X_REFBPCT_0PCT = 0,           ///< 0%
  AFELMP9100X_REFBPCT_1PCT,               ///< 1%
  AFELMP9100X_REFBPCT_2PCT,               ///< 2%
  AFELMP9100X_REFBPCT_4PCT,               ///< 4%
  AFELMP9100X_REFBPCT_6PCT,               ///< 6%
  AFELMP9100X_REFBPCT_8PCT,               ///< 8%
  AFELMP9100X_REFBPCT_10PCT,              ///< 10%
  AFELMP9100X_REFBPCT_12PCT,              ///< 12%
  AFELMP9100X_REFBPCT_14PCT,              ///< 14%
  AFELMP9100X_REFBPCT_16PCT,              ///< 16%
  AFELMP9100X_REFBPCT_18PCT,              ///< 18%
  AFELMP9100X_REFBPCT_20PCT,              ///< 20%
  AFELMP9100X_REFBPCT_22PCT,              ///< 22%
  AFELMP9100X_REFBPCT_24PCT,              ///< 24%
  AFELMP9100X_REFBPCT_MAX
} AFELMP9100XREFBPCT;

/// enumerate the operational modes
typedef enum _AFELMP9100XOPMODE
{
  AFELMP9100X_OPMODE_DEEPSLP = 0,         ///< deep sleep
  AFELMP9100X_OPMODE_2LEADGALV,           ///< 2 lead galvanic
  AFELMP9100X_OPMODE_STANDBY,             ///< standby
  AFELMP9100X_OPMODE_3LEADAMP,            ///< 3 lead amperometric
  AFELMP9100X_OPMODE_TEMPOFF,             ///< temperature measurement off
  AFELMP9100X_OPMODE_TEMPON,              ///< temperature measurement on
  AFELMP9100X_OPMODE_MAX
} AFELMP9100XOPMODE;

// structures -----------------------------------------------------------------
/// define the configuration structure
typedef struct _AFELMP9100XCFGDEF
{
  AFELMP9100XTIAGAIN    eTiaGain;         ///< TIA gain
  AFELMP9100XTIARLOAD   eTiaLoad;         ///< TIA load
  AFELMP9100XREFSOURCE  eRefSource;       ///< reference source
  AFELMP9100XREFINTZ    eRefIntZero;      ///< reference internal zero
  AFELMP9100XREFBIAS    eRefBiasSign;     ///< reference bias sign
  AFELMP9100XREFBPCT    eRefBiasPct;      ///< reference bias percent
} AFELMP9100XCFGDEF, *PAFELMP9100XCFGDEF;
#define AFELMP9100XCFGDEF_SIZE            sizeof( AFELMP9100XCFGDEF )

/// define the control definition structure
typedef struct _AFELMLP9100XCTLDEF
{
  I2CDEVENUM            eI2cDev;          ///< I2C device enuemration
  GPIOPINENUM           eGpioPin;         ///< GPIO pin enumeration
} AFELMLP9100XCTLDEF, *PAFELMLP9100XCTLDEF;
#define AFELMLP9100XCTLDEF_SIZE           sizeof( AFELMLP9100XCTLDEF )

// global parameter declarations -----------------------------------------------

// global function prototypes --------------------------------------------------

/**@} EOF AfeLMP9100X_def.h */

#endif  // _AFELMP9100X_DEF_H