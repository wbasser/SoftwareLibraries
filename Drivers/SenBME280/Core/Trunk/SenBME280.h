/******************************************************************************
 * @file SenBME280.h
 *
 * @brief Bosch Sensortec BME280 drive declarations
 *
 * This file provides the declarations for the Bosch Sensortec BME280
 * temperature/humitdity/pressure sensor.
 *
 * @copyright Copyright (c) 2017 Guardhat
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
 * \addtogroup SenBME280
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _SENBME280_H
#define _SENBME280_H

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "SenBME280/SenBME280_cfg.h"

// library includes -----------------------------------------------------------
#include "TaskManager/TaskManager.h"

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------
/// enumerate the measure type
typedef enum _SENBME280MEASTYPE
{
  SENBME280_MEASTYPE_TEMP = 0,
  SENBME280_MEASTYPE_HUMD,
  SENBME280_MEASTYPE_PRES,
  SENBME280_MEASTYPE_MAX
} SENBME280MEASTYPE;

// structures -----------------------------------------------------------------

// global parameter declarations -----------------------------------------------

// global function prototypes --------------------------------------------------
extern  void    SenBME280_Initialize( void );
extern  void    SenBME280_ProcessScan( void );
extern  S32     SenBME280_GetRawData( SENBME280MEASTYPE eMeasType );
extern  FLOAT   SenBME280_Compensate( SENBME280MEASTYPE eMeasType, S32 lValue );
extern  BOOL    SenBME280_GetMeasurement( SENBME280MEASTYPE eMeasType, PFLOAT pfValue );
extern  FLOAT   SenBME280_GetTemperature( void );
extern  FLOAT   SenBME280_GetHumidity( void );
extern  FLOAT   SenBME280_GetPressure( void );


/**@} EOF SenBME280.h */

#endif  // _SENBME280_H
