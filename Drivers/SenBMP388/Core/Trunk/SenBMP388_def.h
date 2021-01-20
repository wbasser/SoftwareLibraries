/******************************************************************************
 * @file SenBMP388_def.h
 *
 * @brief Bosch Sensortec BMP388 pressure sensor definitions
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
 * \addtogroup SenBMP388
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _SENBMP388_DEF_H
#define _SENBMP388_DEF_H

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------

// library includes -----------------------------------------------------------
#include "Types/Types.h"

// Macros and Defines ---------------------------------------------------------
/// define the helper macro for configurting the sensor
#define SENBMP388CONFIGM( mode, presssamprate, tempsamprate, datarate, filtcoeef, irqpin ) \
  .eMode = mode, \
  .ePressSampRate = presssamprate, \
  .eTempSampRate = tempsamprate, \
  .eDataRate = datarate, \
  .eFiltCoeef = filtcoeef, \
  .eIrqPin = irqpin

// enumerations ---------------------------------------------------------------
/// enumerate the mode
typedef enum _SENBMP388MODE
{
  SENBMP388_MODE_BLKFORCED = 0,
  SENBMP388_MODE_BLKNORMAL,
  SENBMP388_MODE_NONBLKFORCED,
  SENBMP388_MODE_NONBLKNORMAL,
  SENBMP388_MODE_MAX
} SENBMP388MODE;

/// enumerate the oversampling rate
typedef enum _SENBMP388OVERSAMPRATE
{
  SENBMP388_OVER_SAMPRATE_1 = 0,
  SENBMP388_OVER_SAMPRATE_2,
  SENBMP388_OVER_SAMPRATE_4,
  SENBMP388_OVER_SAMPRATE_8,
  SENBMP388_OVER_SAMPRATE_16,
  SENBMP388_OVER_SAMPRATE_32,
  SENBMP388_OVER_SAMPRATE_MAX
} SENBMP388OVERSAMPRATE;

/// enumerate the output data rate
typedef enum _SENBMP388OUTDATARATE
{
  SENBMP388_OUTPUT_DATRATE_5MSEC = 0,
  SENBMP388_OUTPUT_DATRATE_10MSEC,
  SENBMP388_OUTPUT_DATRATE_20MSEC,
  SENBMP388_OUTPUT_DATRATE_40MSEC,
  SENBMP388_OUTPUT_DATRATE_80MSEC,
  SENBMP388_OUTPUT_DATRATE_160MSEC,
  SENBMP388_OUTPUT_DATRATE_320MSEC,
  SENBMP388_OUTPUT_DATRATE_640MSEC,
  SENBMP388_OUTPUT_DATRATE_1SEC28,
  SENBMP388_OUTPUT_DATRATE_2SEC56,
  SENBMP388_OUTPUT_DATRATE_5SEC12,
  SENBMP388_OUTPUT_DATRATE_10SEC24,
  SENBMP388_OUTPUT_DATRATE_20SEC48,
  SENBMP388_OUTPUT_DATRATE_40SEC96,
  SENBMP388_OUTPUT_DATRATE_81SEC92,
  SENBMP388_OUTPUT_DATRATE_161SEC84,
  SENBMP388_OUTPUT_DATRATE_327SEC68,
  SENBMP388_OUTPUT_DATRATE_655SEC36,
  SENBMP388_OUTPUT_DATRATE_MAX
} SENBMP388OUTDATARATE;

/// enumerate the filter coeefieicnets
typedef enum _SENBMP388FILTCOEEF
{
  SENBMP388_FILTCOEEF_0 = 0,
  SENBMP388_FILTCOEEF_1,
  SENBMP388_FILTCOEEF_3,
  SENBMP388_FILTCOEEF_7,
  SENBMP388_FILTCOEEF_15,
  SENBMP388_FILTCOEEF_31,
  SENBMP388_FILTCOEEF_63,
  SENBMP388_FILTCOEEF_127,
  SENBMP388_FILTCOEEF_MAX
} SENBMP388FILTCOEEF;

/// enumerate the IRQ pin modes
typedef enum _SENBMP388IRQDEF
{
  SENBMP388_IRQDEF_PPLO = 0,
  SENBMP388_IRQDEF_ODLO,
  SENBMP388_IRQDEF_PPHI,
  SENBMP388_IRQDEF_ODHI,
  SENBMP388_IRQDEF_MAX,
  SENBMP388_IRQDEF_NONE
} SENBMP388IRQDEF;

// structures -----------------------------------------------------------------
typedef struct _SENBMP388CONFIG
{
  SENBMP388MODE         eMode;
  SENBMP388OVERSAMPRATE ePressSampRate;
  SENBMP388OVERSAMPRATE eTempSampRate;
  SENBMP388OUTDATARATE  eDataRate;
  SENBMP388FILTCOEEF    eFiltCoeef;
  SENBMP388IRQDEF       eIrqPin;
} SENBMP388CONFIG, *PSENBMP388CONFIG;
#define SENBMP388CONFIG_SIZE                              sizeof( SENBMP388CONFIG )

/**@} EOF SenBMP388_def.h */

#endif  // _SENBMP388_DEF_H