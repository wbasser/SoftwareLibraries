/******************************************************************************
 * @file KSZ8081RNACAPhy.h
 *
 * @brief KSZ8051 physical interface declarations
 *
 * This file provides the declarations for the KSZ8051 PHY
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
 * \addtogroup KSZ8081RNACAPhy
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _KSZ8081RNACAPHY_H
#define _KSZ8081RNACAPHY_H

// system includes ------------------------------------------------------------
#include "Types/Types.h"
#include "GMAC/Gmac.h"

// local includes -------------------------------------------------------------
#include "KSZ8081RNACAPhy/KSZ8081RNACAPhy_cfg.h"

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations -----------------------------------------------

// global function prototypes --------------------------------------------------
extern  GMACSTATUS KSZ8081RNACAPhy_Initialize( PU32 puMacAddr, U8 nPhyAddr );
extern  GMACSTATUS KSZ8081RNACAPhy_SetLink( PU32 puMacAddr, U8 nPhyAddr, U8 nFlags );
extern  GMACSTATUS KSZ8081RNACAPhy_AutoNegotiate( PU32 puMacAddr, U8 nPhyAddr );
extern  GMACSTATUS KSZ8081RNACAPhy_Reset( PU32 puMacAddr, U8 nPhyAddr );

/**@} EOF KSZ8081RNACAPhy.h */

#endif  // _KSZ8081RNACAPHY_H