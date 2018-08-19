/******************************************************************************
 * @file Adc_def.h
 *
 * @brief ADC configuration definitions
 *
 * This file provides the ADC definition for the configuration
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
 * \addtogroup Adc
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _ADC_DEF_H
#define _ADC_DEF_H

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------

// library includes -----------------------------------------------------------
#include "Types/Types.h"

// Macros and Defines ---------------------------------------------------------
/// define the helper macro for normal mode, signle ended, blocking
#define ADCDEF_NORMSING_BLKING( inpchan, refsel, prescale, gain ) \
  { \
    .eInpMode = ADC_INPMODE_SING, \
    .ePosChan = inpchan, \
    .eAdcRef = refsel, \
    .ePrescale = prescale, \
    .eGain = gain, \
    .pvCallback = NULL, \
  }

/// define the helper macro for normal mode, signle ended, non-blocking
#define ADCDEF_NORMSING_NONBLK( inpchan, refsel, prescale, gain, callback ) \
  { \
    .eInpMode = ADC_INPMODE_SING, \
    .ePosChan = inpchan, \
    .eAdcRef = refsel, \
    .ePrescale = prescale, \
    .eGain = gain, \
    .pvCallback = callback, \
  }

// enumerations ---------------------------------------------------------------
/// enumerate the mode
typedef enum _ADCINPMODE
{
  ADC_INPMODE_SING = 0,     ///< single ended mode
  ADC_INPMODE_DIFF,         ///< differential mode
  ADC_INPMODE_MAX           ///< 
} ADCINPMODE;

/// enumerate the  positive channels
typedef enum _ADCPOSCHAN
{
  ADC_POSCHAN_AIN00 = 0,      ///< ADC AIN0 pin
  ADC_POSCHAN_AIN01,          ///< ADC AIN1 pin
  ADC_POSCHAN_AIN02,          ///< ADC AIN2 pin
  ADC_POSCHAN_AIN03,          ///< ADC AIN3 pin
  ADC_POSCHAN_AIN04,          ///< ADC AIN4 pin
  ADC_POSCHAN_AIN05,          ///< ADC AIN5 pin
  ADC_POSCHAN_AIN06,          ///< ADC AIN6 pin
  ADC_POSCHAN_AIN07,          ///< ADC AIN7 pin
  ADC_POSCHAN_AIN08,          ///< ADC AIN8 pin
  ADC_POSCHAN_AIN09,          ///< ADC AIN9 pin
  ADC_POSCHAN_AIN10,          ///< ADC AIN10 pin
  ADC_POSCHAN_AIN11,          ///< ADC AIN11 pin
  ADC_POSCHAN_AIN12,          ///< ADC AIN12 pin
  ADC_POSCHAN_AIN13,          ///< ADC AIN13 pin
  ADC_POSCHAN_AIN14,          ///< ADC AIN14 pin
  ADC_POSCHAN_AIN15,          ///< ADC AIN15 pin
  ADC_POSCHAN_AIN16,          ///< ADC AIN16 pin
  ADC_POSCHAN_INPMAX,         ///< maximum number of input pins
  ADC_POSCHAN_BANDGAP = 0x18, ///< Bandgap
  ADC_POSCHAN_GND,            ///< ground
  ADC_POSCHAN_TEMP,           ///< Temperate reference
  ADC_POSCHAN_MAX
} ADCPOSCHAN;

/// enumerate the negative channels
typedef enum _ADCNEGCHAN
{
  ADC_NEGCHAN_AIN00 = 0,      ///< ADC AIN0 pin
  ADC_NEGCHAN_AIN01,          ///< ADC AIN1 pin
  ADC_NEGCHAN_AIN02,          ///< ADC AIN2 pin
  ADC_NEGCHAN_AIN03,          ///< ADC AIN3 pin
  ADC_NEGCHAN_AIN04,          ///< ADC AIN4 pin
  ADC_NEGCHAN_AIN05,          ///< ADC AIN5 pin
  ADC_NEGCHAN_AIN06,          ///< ADC AIN6 pin
  ADC_NEGCHAN_AIN07,          ///< ADC AIN7 pin
  ADC_NEGCHAN_INPMAX,         ///< maximum number of input pins
  ADC_NEGCHAN_INTGND = 0x18,  ///< internal ground
  ADC_NEGCHAN_MAX
} ADCNEGCHAN;

/// enumerate the references
typedef enum _ADCREF
{
  ADC_REF_EXTVREF = 0,        ///< external VREFA
  ADC_REF_AVCC,               ///< analog supply pin
  ADC_REF_INT1V1,             ///< internal 1.1V 
  ADC_REF_INT2V56,            ///< internal 2.56V
  ADC_REF_MAX
} ADCREF;

/// enumerate the prescale 
typedef enum _ADCPRESCALE
{
  ADC_PRESCALE_2 = 1,     ///< divide by 2
  ADC_PRESCALE_4,         ///< divide by 4
  ADC_PRESCALE_8,         ///< divide by 8
  ADC_PRESCALE_16,        ///< divide by 16
  ADC_PRESCALE_32,        ///< divide by 32
  ADC_PRESCALE_64,        ///< divide by 64
  ADC_PRESCALE_128,       ///< divide by 128
  ADC_PRESCALE_MAX
} ADCPRESCALE;

/// enumerate the gain
typedef enum _ADCGAIN
{
  ADC_GAIN_1 = 0,         ///< gain of 1
  ADC_GAIN_10,            ///< gain of 10
  ADC_GAIN_40,            ///< gain of 40
  ADC_GAIN_200,           ///< gain of 200
  ADC_GAIN_MAX
} ADCGAIN;

// structures -----------------------------------------------------------------
/// define the callback 
typedef void ( *PVADCCALLBACK )( U16 );

/// define the ADC channel definition structure
typedef struct _ADCDEF
{
  ADCINPMODE    eInpMode;     ///< input mode
  ADCPOSCHAN    ePosChan;     ///< positive channel
  ADCNEGCHAN    eNegChan;     ///< negative channel
  ADCREF        eAdcRef;      ///< reference selection
  ADCGAIN       eGain;        ///< channel gain selection
  ADCPRESCALE   ePrescale;    ///< clock prescale
  PVADCCALLBACK pvCallback;   ///< callback
} ADCDEF, *PADCDEF;
#define ADCDEF_SIZE     sizeof( ADCDEF )

/**@} EOF Adc_def.h */

#endif  // _ADC_DEF_H