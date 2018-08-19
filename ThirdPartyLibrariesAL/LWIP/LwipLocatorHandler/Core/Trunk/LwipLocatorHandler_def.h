/******************************************************************************
 * @file LwipLocatorHandler_def.h
 *
 * @brief LWIP locator service definitions declarations
 *
 * This file will implement a UDP socket on the designated port and listed for
 * a broadcast request for info and will respond with the program's information
 *
 * @copyright Copyright (c) 2012 Cyber Intergration
 * This document contains proprietary data and information of Endurance Products 
 * LLC. It is the eLWIPLOCATORclusive property of Endurance Products, LLC and will not be 
 * disclosed in any form to any party without prior written permission of 
 * Endurance Products, LLC. This document may not be reproduced or further used 
 * without the prior written permission of Endurance Products, LLC.
 *
 * Version History
 * ======
 * $Log: $
 * 
 *
 * \addtogroup LwipLocatorHanler
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _LWIPLOCATORHANDLER_DEF_H
#define _LWIPLOCATORHANDLER_DEF_H

// system includes ------------------------------------------------------------

// library includes -----------------------------------------------------------
#include "Types/Types.h"

// local includes -------------------------------------------------------------

// Macros and Defines ---------------------------------------------------------
#define LOC_NAM_LEN     ( 32 )
#define LOC_KEY_LEN     ( 32 )

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------
/// define the configuration structure
typedef struct PACKED _LWIPLOCATORDEF
{
  C8    acNameLoc[ LOC_NAM_LEN ];
  U16   wPort;
  C8    acKey[ LOC_KEY_LEN ];
} LWIPLOCATORDEF, *PLWIPLOCATORDEF;
#define LWIPLOCATORDEF_SIZE   sizeof( LWIPLOCATORDEF )

/**@} EOF LwipLocatorHandler_def.h */

#endif  // _LWIPLOCATORHANDLER_DEF_H