/******************************************************************************
 * @file LwipControlHandler.h
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
 * \addtogroup LwipControlHandler
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _LWIPCONTROLHANDLER_H
#define _LWIPCONTROLHANDLER_H

// system includes ------------------------------------------------------------
#if (SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_TASKMANAGER )
#include "TaskManager/TaskManager.h"
#endif

// local includes -------------------------------------------------------------
#include "LwipControlHandler/LwipControlHandler_cfg.h"

// library includes -----------------------------------------------------------
#include "lwip/api.h"
#include "lwip/dhcp.h"
#include "lwip/mem.h"
#include "lwip/opt.h"
#include "lwip/tcp.h"
#include "lwip/udp.h"
#if ( LWIPCONTROLHANDLER_ENABLE_DEBUGCMDS == 1 )
#include "AsciiCommandHandler/AsciiCommandHandler.h"
#endif // LWIPCONTROLHANDLER_ENABLE_DEBUGCMDS

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------
/// enuemrate the errors
typedef enum _LWIPERRORS
{
  LWIP_ERROR_NONE = 0,            ///< no error
  LWIP_ERROR_ILLMODE,             ///< illegal mode
} LWIPERRORS;

// structures -----------------------------------------------------------------

// global parameter declarations -----------------------------------------------
#if ( LWIPCONTROLHANDLER_ENABLE_DEBUGCMDS == 1 )
/// initialize the command table
extern const CODE ASCCMDENTRY atLwipHandlerCmdTable[ ];
#endif // LWIPCONTROLHANDLER_ENABLE_DEBUGCMDS

// global function prototypes --------------------------------------------------
extern  void        LwipControlHandler_Initialize( void );
extern  PLWIPCFGDEF LwipControlHandler_GetConfigDflt( void );
extern  PLWIPCFGDEF LwipControlHandler_GetConfigActl( void );
extern  void        LwipControlHandler_Configure( void );
extern  LWIPERRORS  LwipControlHandler_SetConfig( PLWIPCFGDEF ptLwipCfgDef );
extern  void        LwipControlHandler_GetMacAddr( PU8 pnMacAddress );
extern  void        LwipControlHandler_SetMacAddr( PU8 pnMacAddress );
extern  void        LwipControlHandler_GetCurrentNetConfig( PLWIPCFGDEF ptLwipCfgDef );
extern  void        LwipControlHandler_ProcessTimer( void );

/**@} EOF LwipControlHandler.h */

#endif  // _LWIPCONTROLHANDLER_H