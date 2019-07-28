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

// structures -----------------------------------------------------------------

// global parameter declarations -----------------------------------------------

// global function prototypes --------------------------------------------------
extern  void  SenBMP388_Initialize( void );
extern  void  SenBMP388_ProcessDataReady( void );
extern  FLOAT SenBMP388_GetPressure( void );
extern  FLOAT SenBMP388_GetAltitude( void );
extern  FLOAT SenBMP388_GetTemperature( void );

/**@} EOF SenBMP388.h */

#endif  // _SENBMP388_H