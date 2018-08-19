/******************************************************************************
 * @file StellarisLwipHandler.h
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
 * \addtogroup StellarisLwipHandler
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _STELLARISLWIPHANDLER_H
#define _STELLARISLWIPHANDLER_H

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
#include "StellarisLwipHandler/StellarisLwipHandler_cfg.h"
#include "StellarisLwipHandler/StellarisLwipHandler_prm.h"

// Macros and Defines ---------------------------------------------------------
#define STELLARISLWIP_EXEC_RATE     TASK_TIME_MSECS( 10 )
#define STELLARISLWIP_NUM_EVENTS    ( 2 )

// enumerations ---------------------------------------------------------------
/// enumerate the configuration modes
typedef enum _LWIPCONFIGMODE
{
  LWIP_CONFIG_MODE_STATIC = 0,      ///< static IP mode
  LWIP_CONFIG_MODE_DHCP,            ///< DHCP mode
  LWIP_CONFIG_MODE_AUTOIP,          ///< auto IP mode
  LWIP_CONFIG_MODE_MAX              ///< config mode max
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
  LWIPCONFIGMODE  eMode;        ///< config mode
  U32UN           tAddr;        ///< address
  U32UN           tMask;        ///< mask
  U32UN           tGate;        ///< gateway
} LWIPCFGDEF, *PLWIPCFGDEF;
#define LWIPCFGDEF_SIZE   sizeof( LWIPCFGDEF )

// global parameter declarations -----------------------------------------------
#if ( STELLARISLWIP_ENABLE_DEBUGCMDS == 1 )
/// initialize the command table
extern const CODE ASCCMDENTRY atLwipHandlerCmdTable[ ];
#endif // STELLARISLWIP_ENABLE_DEBUGCMDS

// global function prototypes --------------------------------------------------
extern  void        StellarisLwipHandler_Initialize( void );
extern  PLWIPCFGDEF StellarisLwipHandler_GetConfigDflt( void );
extern  PLWIPCFGDEF StellarisLwipHandler_GetConfigActl( void );
extern  void        StellarisLwipHandler_Configure( void );
extern  LWIPERRORS  StellarisLwipHandler_SetConfig( PLWIPCFGDEF ptLwipCfgDef );
extern  void        StellarisLwipHandler_GetMacAddr( PU8 pnMacAddress );
extern  BOOL        StellarisLwipHandler_ProcessTimer( TASKARG arg );
extern  void        StellarisLwipHandler_GetCurrentNetConfig( PLWIPCFGDEF ptLwipCfgDef );

/**@} EOF StellarisLwipHandler.h */

#endif  // _STELLARISLWIPHANDLER_H