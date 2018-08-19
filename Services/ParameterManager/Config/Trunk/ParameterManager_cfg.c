/******************************************************************************
 * @file ParameterManager_cfg.c
 *
 * @brief parameter manager configuration implementation
 *
 * This file provides the implementation for the configuraitn of the paramete
 * manager
 *
 * @copyright Copyright (c) 2012 CyberIntegration
 * This document contains proprietary data and information of CyberIntegration
 * LLC. It is the exclusive property of CyberIntegration, LLC and will not be
 * disclosed in any form to any party without prior written permission of 
 * CyberIntegration, LLC. This document may not be reproduced or further used
 * without the prior written permission of CyberIntegration, LLC.
 *
 * Version History
 * ======
 * $Rev: $
 * 
 *
 * \addtogroup 
 * @{
 *****************************************************************************/

// local includes -------------------------------------------------------------
#include "ParameterManager\ParameterManager_cfg.h"
#include "ParameterManager\ParameterManager.h"

// Macros and Defines ---------------------------------------------------------

/// define the memory block write enable code

// constant parameter initializations -----------------------------------------
const CODE  PARAMDFLTS  atParamDefaults[ PARAMSEL_MAX_NUM ] =
{
  // populate the table with one of the below macros
  // PARAM_ENTRY( defval, minval, maxval, lock )
  // PARAM_ENTRYNAM( defval, minval, maxval, lock, name )

};

/**@} EOF ParamaterManager_cfg.c */
