/******************************************************************************
 * @file LwipHttpHandler_cfg.h
 *
 * @brief LWIP HTTP handler configuration declarations
 *
 * This file provides the declarations for the HTTP handler configuration
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
 * \addtogroup LwipHttpHandler
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _LWIPHTTPHANDLER_CFG_H
#define _LWIPHTTPHANDLER_CFG_H

// local includes -------------------------------------------------------------
#include "LwipHttpHandler/LwipHttpHandler_def.h"

// enumerations ---------------------------------------------------------------

// global parameter declarations -----------------------------------------------
extern  const char*            apszSsiTags[ ];
extern  const LWIPHTTPSSITAG  atLwipHttpSsiTags[ ];
extern  const LWIPHTTPCGIFUNC atLwipHttpCgiFuncs[ ];
//extern  const tCGI            atLwipHttpCgiFuncs[ ];
extern  const LWIPHTTPDYNPAGE atLwipHttpDynPages[ ];

// global function prototypes --------------------------------------------------
extern  U32 LwipHttpHandler_GetNumSsiTags( void );
extern  U32 LwipHttpHandler_GetCgiFuncSize( void );
extern  U32 LwipHttpHandler_GetDynPageSize( void );

/**@} EOF LwipHttpHandler_cfg.h */

#endif  // _LWIPHTTPHANDLER_CFG_H