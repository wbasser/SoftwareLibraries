/******************************************************************************
 * @file KSZ8081RNACAPhy_cfg.h
 *
 * @brief KSZ8081RNACA PHY configuration delcartations 
 *
 * This file provides the declarations for the configuration of the KSZ8051 PHY
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
#ifndef _KSZ8081RNACAPHY_CFG_H
#define _KSZ8081RNACAPHY_CFG_H

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------

// library includes -----------------------------------------------------------
#include "GMAC/Gmac.h"

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations -----------------------------------------------

// global function prototypes --------------------------------------------------
extern  void        KSZ8081RNACAPhy_SetMdcClock( PU32 puGmacAddr, U32 uClockRate );
extern  void        KSZ8081RNACAPhy_EnableManagement( PU32 puGmacAddr, BOOL bState );
extern  GMACSTATUS  KSZ8081RNACAPhy_PhyWrite( PU32 puGmacAddr, U8 nPhyAddress, U8 naddress, U32 uValue );
extern  GMACSTATUS  KSZ8081RNACAPhy_PhyRead( PU32 puGmacAddr, U8 nPhyAddress, U8 naddress, PU32 puValue );
extern  void        KSZ8081RNACAPhy_SetSpeed( PU32 puGmacAddr, BOOL bSpeed );
extern  void        KSZ8081RNACAPhy_EnableFullDuplex( PU32 puGmacAddr, BOOL bFullDuplex );
extern  void        KSZ8081RNACAPhy_SelectMiiMode( PU32 puGmacAddr, GMACMIIMODE eMode );
extern  void        KSZ8081RNACAPhy_EnableTransmit( PU32 puGmacAddr, BOOL bState );
extern  void        KSZ8081RNACAPhy_EnableReceive( PU32 puGmacAddr, BOOL bState );

/**@} EOF KSZ8081RNACAPhy_cfg.h */

#endif  // _KSZ8081RNACAPHY_CFG_H