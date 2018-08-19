/******************************************************************************
 * @file PowerManager_cfg.c
 *
 * @brief Power Manager configuration implementation
 *
 * This file provides the implementation of the power manager configuration
 *
 * @copyright Copyright (c) 2012 CyberIntegration
 * This document contains proprietary data and information of CyberIntegration 
 * LLC. It is the exclusive property of CyberIntegration, LLC and will not be 
 * disclosed in any form to any party without prior written permission of 
 * CyberIntegration, LLC. This document may not be reproduced or further used 
 * without the prior written permission of CyberIntegration, LLC.
 *
 * $Date: $
 *
 * Version History
 * ======
 * $Rev: $
 * 
 *
 * \addtogroup PowerManager.
 * @{
 *****************************************************************************/

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "PowerManager/PowerManager_cfg.h"

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations ----------------------------------------------
const CODE U8 anPmcPeripheralsDef[ ] =
{
  // add peripheral ID's here
  
  // do not remove the below entry
  0xFF          ///< marks end of table
};

/**@} EOF PowerManager_cfg..c */
