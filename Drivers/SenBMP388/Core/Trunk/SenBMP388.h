/******************************************************************************
 * @file SenBMP388.h
 *
 * @brief Bosch Sensortec BMP388 pressure sensor 
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
#ifndef _SENBMP388_H
#define _SENBMP388_H

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "SenBMP388/SenBMP388_cfg.h"

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------
/// enumerate the errors
typedef enum _SENBMP388ERR
{
  SENBMP388_ERR_NONE = 0,
  SENBMP388_ERR_NODEV = 0xF0,
  SENBMP388_ERR_CONF,
  SENBMP388_ERR_NODATA
} SENBMP388ERR;

/// enumerate the fields
typedef enum _SENBMP388FIELD
{
  SENBMP388_FIELD_PRESSURE = 0,
  SENBMP388_FIELD_ALTITUDE,
  SENBMP388_FIELD_TEMPERATURE,
  SEBBNO388_FIELD_MAX
} SENBMP388FIELD;

// structures -----------------------------------------------------------------

// global parameter declarations -----------------------------------------------

// global function prototypes --------------------------------------------------
extern  void          SenBMP388_Initialize( void );
extern  void          SenBMP388_ProcessDataReady( void );
extern  SENBMP388ERR  SenBMP388_GetPressure( PFLOAT pfValue );
extern  SENBMP388ERR  SenBMP388_GetAltitude( PFLOAT pfValue );
extern  SENBMP388ERR  SenBMP388_GetTemperature( PFLOAT pfValue );
extern  FLOAT         SenBMP388_GetFieldValue( SENBMP388FIELD eField );

/**@} EOF SenBMP388.h */

#endif  // _SENBMP388_H