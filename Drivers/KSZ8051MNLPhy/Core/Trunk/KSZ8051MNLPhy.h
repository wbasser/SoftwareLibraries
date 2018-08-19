/******************************************************************************
 * @file KSZ8051MNLPhy.h
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
 * \addtogroup KSZ8051MNLPhy
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _KSZ8051MNLPHY_H
#define _KSZ8051MNLPHY_H

// system includes ------------------------------------------------------------
#include "Types/Types.h"
#include "GMAC/Gmac.h"

// local includes -------------------------------------------------------------
#include "KSZ8051MNLPhy/KSZ8051MNLPhy_cfg.h"

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations -----------------------------------------------

// global function prototypes --------------------------------------------------
extern  GMACSTATUS KSZ8051MNLPhy_Initialize( PU32 puMacAddr, U8 nPhyAddr );
extern  GMACSTATUS KSZ8051MNLPhy_SetLink( PU32 puMacAddr, U8 nPhyAddr, U8 nFlags );
extern  GMACSTATUS KSZ8051MNLPhy_AutoNegotiate( PU32 puMacAddr, U8 nPhyAddr );
extern  GMACSTATUS KSZ8051MNLPhy_Reset( PU32 puMacAddr, U8 nPhyAddr );

/**@} EOF KSZ8051MNLPhy.h */

#endif  // _KSZ8051MNLPHY_H