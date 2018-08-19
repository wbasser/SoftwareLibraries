/******************************************************************************
 * @file LwipLocatorHandler_cfg.h
 *
 * @brief LWIP locator handler configuration declarations 
 *
 * This file 
 *
 * @copyright Copyright (c) 2012 Nan Technologies
 * This document contains proprietary data and information of Nan Technologies 
 * LLC. It is the exclusive property of Nan Technologies, LLC and will not be 
 * disclosed in any form to any party without prior written permission of 
 * Nan Technologies, LLC. This document may not be reproduced or further used 
 * without the prior written permission of Nan Technologies, LLC.
 *
 * $Date: $
 *
 * Version History
 * ======
 * $Rev: $
 * 
 *
 * \addtogroup LwipLocatorHandler
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _LWIPLOCATORHANDLER_CFG_H
#define _LWIPLOCATORHANDLER_CFG_H

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "LwipLocatorHandler/LwipLocatorHandler_def.h"

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations -----------------------------------------------
extern  const CODE LWIPLOCATORDEF  tLwipLocDefDflts;

// global function prototypes --------------------------------------------------
extern  void  LwipLocatorHandler_GetMacAddr( PU8 pnMacAddress );

/**@} EOF LwipLocatorHandler_cfg.h */

#endif  // _LWIPLOCATORHANDLER_CFG_H