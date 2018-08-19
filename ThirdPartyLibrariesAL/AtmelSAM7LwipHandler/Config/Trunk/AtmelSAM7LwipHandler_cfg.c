/******************************************************************************
 * @file AtmelSAM7LwipHandler_cfg.c
 *
 * @brief Stellaris LWIP handler configuration implementation
 *
 * This file provides the ethernet configuration implementation
 *
 * @copyright Copyright (c) 2012 Cyber Intergration
 * This document contains proprietary data and information of Endurance Products 
 * LLC. It is the exclusive property of Endurance Products, LLC and will not be 
 * disclosed in any form to any party without prior written permission of 
 * Endurance Products, LLC. This document may not be reproduced or further used 
 * without the prior written permission of Endurance Products, LLC.
 *
 * Version History
 * ======
 * $Log: $
 * 
 *
 * \addtogroup AtmelSAM7LwipHandler
 * @{
 *****************************************************************************/

// system includes ------------------------------------------------------------

// library includes -----------------------------------------------------------

// local includes -------------------------------------------------------------
#include "AtmelSAM7LwipHandler/AtmelSAM7LwipHandler_cfg.h"

// global parameter declarations ----------------------------------------------
const LWIPPROTDEF atLwipProtDefs[ LWIP_PROT_MAX ] =
{
  // LWIPPROT( taskinit, taskprocess )
};

/**@} EOF AtmelSAM7LwipHandler_cfg.c */
