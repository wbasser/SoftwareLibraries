/******************************************************************************
 * @file SerialNumber.h
 *
 * @brief Serial number declrations 
 *
 * This file provides the declarations for the serial number
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
 * \addtogroup SerialNumber
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _SERIALNUMBER_H
#define _SERIALNUMBER_H

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------

// library includes -----------------------------------------------------------
#include "Types/Types.h"

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------
// define the serial number structure
typedef union _SERIALNUMBER
{
  struct 
  {
    U32UN   tWord0;
    U32UN   tWord1;
    U32UN   tWord2;
    U32UN   tWord3;
  } tWords;
  U8  anBytes[ 1 ];
} SERIALNUMBER, *PSERIALNUMBER;
#define SERIALNUMBER_SIZE         sizeof( SERIALNUMBER )

// global parameter declarations -----------------------------------------------

// global function prototypes --------------------------------------------------
extern  void  SerialNumber_Get( PSERIALNUMBER ptSerNum );

/**@} EOF SerialNumber.h */

#endif  // _SERIALNUMBER_H