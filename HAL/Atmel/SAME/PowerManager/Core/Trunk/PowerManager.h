/******************************************************************************
 * @file PowerManager.h
 *
 * @brief power manager declarations
 *
 * This file provides the declarations for the power manager
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
 * \addtogroup PowerManager
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _POWERMANAGER_H
#define _POWERMANAGER_H

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "PowerManager/PowerManager_cfg.h"

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations -----------------------------------------------

// global function prototypes --------------------------------------------------
extern  void  PowerManager_Initialize( void );

/**@} EOF PowerManager.h */

#endif  // _POWERMANAGER_H