/******************************************************************************
 * @file LwipLocatorHandler.h
 *
 * @brief LWIP locator service declarations
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
#ifndef _LWIPLOCATORHANDLER_H
#define _LWIPLOCATORHANDLER_H

// system includes ------------------------------------------------------------

// library includes -----------------------------------------------------------
#include "Types/Types.h"

// local includes -------------------------------------------------------------
#include "LwipLocatorHandler/LwipLocatorHandler_cfg.h"

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations -----------------------------------------------

// global function prototypes --------------------------------------------------
extern  void            LwipLocatorHandler_Initialize( void );
extern  PLWIPLOCATORDEF LwipLocatorHandler_GetConfigDflt( void );
extern  PLWIPLOCATORDEF LwipLocatorHandler_GetConfigActl( void );
extern  void            LwipLocatorHandler_SetConfig( PLWIPLOCATORDEF ptLwipDef );
extern  void            LwipLocatorHandler_ClientIPSet( U32UN tIpAddr );
extern  void            LwipLocatorHandler_UpdateConfiguration( void );

/**@} EOF LwipLocatorHandler.h */

#endif  // _LWIPLOCATORHANDLER_H