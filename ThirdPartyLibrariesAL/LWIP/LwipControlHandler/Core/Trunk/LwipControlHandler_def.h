/******************************************************************************
 * @file LwipControlHandler_def.h
 *
 * @brief LWIP handler definitionsdeclarations
 *
 * This file provides the definitionsdeclarations for the ethernet handler
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
 * \addtogroup LwipControlHandler
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _LWIPCONTROLHANDLER_DEF_H
#define _LWIPCONTROLHANDLER_DEF_H

// system includes ------------------------------------------------------------
#include "LwipControlHandler\LwipControlHandler_prm.h"

// Macros and Defines ---------------------------------------------------------
/// define the protocol handler helper macro
#define	LWIPPROT( taskinit, taskprocess )	\
  { \
    .pvInit = taskinit, \
    .pvProc = taskprocess \
  }

/// define the MAC address length
#define LWIP_MAC_LENGTH                     ( 6 )

/// define the function pointer format for the different structures
typedef void  ( *PVINITFUNC )( void );
typedef void  ( *PVPROCFUNC )( void );

// enumerations -----------------------------------------------------------------
/// enumerate the configuration modes
typedef enum _LWIPCONFIGMODE
{
  LWIP_CONFIG_MODE_STATIC = 0,      ///< static IP mode
  LWIP_CONFIG_MODE_DHCP,            ///< DHCP mode
  LWIP_CONFIG_MODE_AUTOIP,          ///< auto IP mode
  LWIP_CONFIG_MODE_MAX              ///< max config mode 
} LWIPCONFIGMODE;

// structures -----------------------------------------------------------------
/// define the initialization structure
typedef struct PACKED _LWIPCFGDEF
{
  LWIPCONFIGMODE  eMode;                      ///< config mode
  U32UN           tAddr;                      ///< address
  U32UN           tMask;                      ///< mask
  U32UN           tGate;                      ///< gateway
  U8              anMac[ LWIP_MAC_LENGTH ];   ///< MAC address
} LWIPCFGDEF, *PLWIPCFGDEF;
#define LWIPCFGDEF_SIZE   sizeof( LWIPCFGDEF )

/// define the scheduled task definition structure
typedef struct 	_LWIPPROTDEF
{
	PVINITFUNC  pvInit;		    ///< pointer to the task initializer
	PVPROCFUNC  pvProc;	      ///< pointer to the task process
} LWIPPROTDEF, *PLWIPPROTDEF;
#define	LWIPPROTDEF_SIZE	sizeof( LWIPPROTDEF )

/**@} EOF LwipControlHandler_def.h */

#endif  // _LWIPCONTROLHANDLER_DEF_H