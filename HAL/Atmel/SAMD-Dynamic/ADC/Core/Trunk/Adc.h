/******************************************************************************
 * @file Adc.h
 *
 * @brief ADC declarations
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
 * \addtogroup ADC
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _ADC_H
#define _ADC_H

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------

// library includes -----------------------------------------------------------
#include "GPIO/Gpio.h"

// Macros and Defines ---------------------------------------------------------
/// define the maximum counts
#define ADC_MAX_COUNTS                          ( 4095 )

// enumerations ---------------------------------------------------------------
/// enumerate the errors
typedef enum _ADCERRS
{
  ADC_ERR_NONE = 0,       ///< no error
  ADC_ERR_BLOCKING,       ///< blocking operation in progress
  ADC_ERR_ILLENUM = 0x80, ///< illegal enumeration
  ADC_ERR_ILLPRM,         ///< illegal parameter
  ADC_ERR_ILLMODE,        ///< illegal mode
} ADCERRS;

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
  ADC_POSCHAN_AIN00 = 0,    ///< ADC AIN0 pin
  ADC_POSCHAN_AIN01,        ///< ADC AIN1 pin
  ADC_POSCHAN_AIN02,        ///< ADC AIN2 pin
  ADC_POSCHAN_AIN03,        ///< ADC AIN3 pin
  ADC_POSCHAN_AIN04,        ///< ADC AIN4 pin
  ADC_POSCHAN_AIN05,        ///< ADC AIN5 pin
  ADC_POSCHAN_AIN06,        ///< ADC AIN6 pin
  ADC_POSCHAN_AIN07,        ///< ADC AIN7 pin
  ADC_POSCHAN_AIN08,        ///< ADC AIN8 pin
  ADC_POSCHAN_AIN09,        ///< ADC AIN9 pin
  ADC_POSCHAN_AIN10,        ///< ADC AIN10 pin
  ADC_POSCHAN_AIN11,        ///< ADC AIN11 pin
  ADC_POSCHAN_AIN12,        ///< ADC AIN12 pin
  ADC_POSCHAN_AIN13,        ///< ADC AIN13 pin
  ADC_POSCHAN_AIN14,        ///< ADC AIN14 pin
  ADC_POSCHAN_AIN15,        ///< ADC AIN15 pin
  ADC_POSCHAN_AIN16,        ///< ADC AIN16 pin
  ADC_POSCHAN_AIN17,        ///< ADC AIN17 pin
  ADC_POSCHAN_AIN18,        ///< ADC AIN18 pin
  ADC_POSCHAN_AIN19,        ///< ADC AIN19 pin
  ADC_POSCHAN_INPMAX,       ///< maximum number of input pins
  ADC_POSCHAN_TEMP = 0x18,  ///< Temperate reference
  ADC_POSCHAN_BANDGAP,      ///< Bandgap
  ADC_POSCHAN_COREVCC,      ///< 1/4 scaled core VCC
  ADC_POSCHAN_IOVCC,        ///< 1/4 scaled IO VCC
  ADC_POSCHAN_DAC,          ///< DAC 
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
  ADC_NEGCHAN_IOGND,          ///< I/O ground
  ADC_NEGCHAN_MAX
} ADCNEGCHAN;

/// enumerate the references
typedef enum _ADCREF
{
  ADC_REF_INT1V = 0,      ///< internal 1V 
  ADC_REF_INTVCC0,        ///< internal 1/1.48 VDDANA
  ADC_REF_INTVCC1,        ///< internal 1/2 VDDANA
  ADC_REF_EXTVREFA,       ///< external VREFA
  ADC_REF_EXTVREFB,       ///< external VREFB
  ADC_REF_MAX
} ADCREF;

/// enumerate the prescale 
typedef enum _ADCPRESCALE
{
  ADC_PRESCALE_4 = 0,     ///< divide by 4
  ADC_PRESCALE_8,         ///< divide by 8
  ADC_PRESCALE_16,        ///< divide by 16
  ADC_PRESCALE_32,        ///< divide by 32
  ADC_PRESCALE_64,        ///< divide by 64
  ADC_PRESCALE_128,       ///< divide by 128
  ADC_PRESCALE_256,       ///< divide by 256
  ADC_PRESCALE_512,       ///< divide by 512
  ADC_PRESCALE_MAX
} ADCPRESCALE;

/// enumerate the number of samples
typedef enum _ADCNUMSAMP
{
  ADC_NUMSAMP_1 = 0,      ///< 1 sample
  ADC_NUMSAMP_2,          ///< 2 samples
  ADC_NUMSAMP_4,          ///< 4 samples
  ADC_NUMSAMP_8,          ///< 8 samples
  ADC_NUMSAMP_16,         ///< 16 samples
  ADC_NUMSAMP_32,         ///< 32 samples
  ADC_NUMSAMP_64,         ///< 64 samples
  ADC_NUMSAMP_128,        ///< 128 samples
  ADC_NUMSAMP_256,        ///< 256 samples
  ADC_NUMSAMP_512,        ///< 512 samples
  ADC_NUMSAMP_1024,       ///< 1024 samples
  ADC_NUMSAMP_MAX
} ADCNUMSAMP;

/// enumerate the gain
typedef enum _ADCGAIN
{
  ADC_GAIN_1 = 0,         ///< gain of 1
  ADC_GAIN_2,             ///< gain of 2
  ADC_GAIN_4,             ///< gain of 4
  ADC_GAIN_8,             ///< gain of 8
  ADC_GAIN_16,            ///< gain of 16
  ADC_GAIN_HALF = 0x0F,   ///< gain of 1/2
  ADC_GAIN_MAX
} ADCGAIN;

/// enumerate the conversion modes
typedef enum _ADCMODE
{
  ADC_MODE_NORMAL = 0,    ///< normal conversion
  ADC_MODE_ACCUM,         ///< accumulation mode
  ADC_MODE_AVERAGE,       ///< averaging mode
  ADC_MODE_DECIMATE,      ///< decimate mode
  ADC_MODE_MAX
} ADCMODE;

/// enumerate the decimation result resolution
typedef enum _ADCDECRES
{
  ADC_DECRES_13 = 0,      ///< resolution 13 bits
  ADC_DECRES_14,          ///< resolution 14 bits
  ADC_DECRES_15,          ///< resolution 15 bits
  ADC_DECRES_16,          ///< resolution 16 bits
  ADC_DECRES_MAX
} ADCDECRES;

// structures -----------------------------------------------------------------
/// define the handle declaration
typedef PVOID   PADCHANDLE;

/// define the callback function
typedef void    ( *PVADCCALLBACK )( U16 );

/// define the channel definition structure
typedef struct _ADCCHANDEF
{
  GPIOPORT      eDevPort;     ///< device port
  U8            nAinPin;      ///< ain pin
  ADCINPMODE    eInpMode;
  ADCPOSCHAN    ePosChan;
  ADCNEGCHAN    eNegChan; 
  ADCREF        eRef;
  ADCPRESCALE   ePreScale;
  ADCNUMSAMP    eNumSamps; 
  ADCGAIN       eGain; 
  ADCMODE       eAdcMode;
  ADCDECRES     eDecRes;
  PVADCCALLBACK pvCallback;
} ADCCHANDEF, *PADCCHANDEF;
#define ADCCHANDEF_SIZE                     sizeof( ADCCHANDEF )

// global parameter declarations -----------------------------------------------

// global function prototypes --------------------------------------------------
extern  void        Adc_Initialize( void );
extern  PADCHANDLE  Adc_Create( PADCCHANDEF ptChanDef );
extern  ADCERRS     Adc_ConvertChannel( PADCHANDLE pvHandle, PU16 pwResult );

/**@} EOF Adc.h */

#endif  // _ADC_H