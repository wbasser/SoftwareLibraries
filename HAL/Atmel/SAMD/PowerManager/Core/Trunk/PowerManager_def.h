/******************************************************************************
 * @file PowerManager
 *
 * @brief Power Manager Definition file
 *
 * This file provides the definitions for the Power Manager
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
#ifndef _POWERMANAGER_DEF_H
#define _POWERMANAGER_DEF_H

// system includes ------------------------------------------------------------

// library includes -----------------------------------------------------------
#include "Types/Types.h"

// structures -----------------------------------------------------------------
/// define the overall structure
typedef struct _PMDEF
{
  PM_AHBMASK_Type   tPmAhb;
  PM_APBAMASK_Type  tPmApbA;
  PM_APBBMASK_Type  tPmApbB;
  PM_APBCMASK_Type  tPmApbC;
} PMDEF, *PPMDEF;
#define PMDEF_SIZE      sizeof( PMDEF )

/**@} EOF PowerManager_def.h */

#endif  // _POWERMANAGER_DEF_H