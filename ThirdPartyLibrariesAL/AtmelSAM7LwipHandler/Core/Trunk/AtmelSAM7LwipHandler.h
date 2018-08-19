/******************************************************************************
 * @file AtmelSAM7LwipHandler.h
 *
 * @brief Stellaris LWIP handler declarations
 *
 * This file provides the declarations for the ethernet handler
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
 
// ensure only one instantiation
#ifndef _ATMELSAM7LWIPHANDLER_H
#define _ATMELSAM7LWIPHANDLER_H

// system includes ------------------------------------------------------------
#include "TaskManager/TaskManager.h"

// library includes -----------------------------------------------------------
#include "src/include/lwip/api.h"
#include "src/include/lwip/opt.h"
#include "src/include/lwip/mem.h"
#include "src/include/lwip/tcp.h"
#include "src/include/lwip/udp.h"
#include "TaskManager/TaskManager.h"

// local includes -------------------------------------------------------------
#include "AtmelSAM7LwipHandler/AtmelSAM7LwipHandler_cfg.h"

// Macros and Defines ---------------------------------------------------------
/// define the execution rate and number of events
#define ATMELSAM7LWIP_EXEC_RATE     TASK_TIME_MSECS( 10 )
#define ATMELSAM7LWIP_NUM_EVENTS    ( 2 )

/// define the MAC address length
#define LWIP_MAC_LENGTH             ( 6 )

// enumerations ---------------------------------------------------------------
/// enumerate the configuration modes
typedef enum _LWIPCONFIGMODE
{
  LWIP_CONFIG_MODE_STATIC = 0,      ///< static IP mode
  LWIP_CONFIG_MODE_DHCP,            ///< DHCP mode
  LWIP_CONFIG_MODE_AUTOIP,          ///< auto IP mode
} LWIPCONFIGMODE;

/// enuemrate the errors
typedef enum _LWIPERRORS
{
  LWIP_ERROR_NONE = 0,            ///< no error
  LWIP_ERROR_ILLMODE,             ///< illegal mode
} LWIPERRORS;

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

// global parameter declarations -----------------------------------------------

// global function prototypes --------------------------------------------------
extern  void        AtmelSAM7LwipHandler_Initialize( void );
extern  PLWIPCFGDEF AtmelSAM7LwipHandler_GetConfigDflt( void );
extern  PLWIPCFGDEF AtmelSAM7LwipHandler_GetConfigActl( void );
extern  void        AtmelSAM7LwipHandler_Configure( void );
extern  LWIPERRORS  AtmelSAM7LwipHandler_SetConfig( PLWIPCFGDEF ptLwipCfgDef );
extern  void        AtmelSAM7LwipHandler_GetMacAddr( PU8 pnMacAddress );
extern  BOOL        AtmelSAM7LwipHandler_ProcessTimer( TASKARG arg );
extern  void        AtmelSAM7LwipHandler_GetCurrentNetConfig( PLWIPCFGDEF ptLwipCfgDef );

/**@} EOF AtmelSAM7LwipHandler.h */

#endif  // _ATMELSAM7LWIPHANDLER_H