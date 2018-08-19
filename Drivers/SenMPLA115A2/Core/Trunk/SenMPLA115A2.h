/******************************************************************************
 * @file SenMPLA1152.h
 *
 * @brief MPLA1152 barometric sensor declarations 
 *
 * This file provides the declarations for the MPLA1152 barometric sensor
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
 * \addtogroup SenMPLA1152
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _SENMPLA1152_H
#define _SENMPLA1152_H

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------
/// enumerate the measurement types
typedef enum _SENMPLA115ATYPE
{
  SENMPLA1152_TYPE_PRESS = 0,
  SENMPLA1152_TYPE_TEMP
} SENMPLA115ATYPE;

// structures -----------------------------------------------------------------

// global parameter declarations -----------------------------------------------

// global function prototypes --------------------------------------------------
extern  void  SenMPLA1152_Initialize( void );
extern  U16   SenMPLA1152_GetValue( SENMPLA115ATYPE eType );
extern  FLOAT SenMPLA1152_Linearize( U16 wValue, U8 nOption );

/**@} EOF SenMPLA1152.h */

#endif  // _SENMPLA1152_H