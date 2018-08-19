/******************************************************************************
 * @file LwipLocatorHandler_Cfg.c
 *
 * @brief LWIP locator handler configuration implementation
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

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "LwipLocatorHandler/LwipLocatorHandler_cfg.h"

// library includes -----------------------------------------------------------
#include "Types/Types.h"

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations ----------------------------------------------

// local parameter declarations -----------------------------------------------

// local function prototypes --------------------------------------------------

// constant parameter initializations -----------------------------------------
const CODE LWIPLOCATORDEF  tLwipLocDefDflts =
{
  .acNameLoc  = "GH-DeviceControl",
  .wPort      = 23,
  .acKey      = "DEADBEEF_GARBAGE$"
};

/******************************************************************************
 * @function LwipLocatorpHandler_GetMacAddr
 *
 * @brief gets the current mac address
 *
 * This function gets the current MAC address
 *
 * @param[io]   pnMacAddress   pointer to the address
 *
 *****************************************************************************/
void LwipLocatorHandler_GetMacAddr( PU8 pnMacAddress )
{
  // now call the user defined function to actually do it
  LwipControlHandler_GetMacAddr( pnMacAddress  );
}

/**@} EOF LwipLocatorHandler_Cfg.c */
