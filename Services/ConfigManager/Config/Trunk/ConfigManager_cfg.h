/******************************************************************************
 * @file ConfigManager_cfg.h
 *
 * @brief configuration manager user config declarations
 *
 * This file provides the user with the configuration to control the 
 * configuration manager
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
 *
 * \addtogroup ConfigManager
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _CONFIGMANAGER_CFG_H
#define _CONFIGMANAGER_CFG_H

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "ConfigManager/ConfigManager_def.h"

// library includes -----------------------------------------------------------
#if ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_TASKMANAGER )
  #include "TaskManager/TaskManager.h"
#elif ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_MINIMAL )
  #include "TaskMinimal/TaskMinimal.h"
#endif // SYSTEMDEFINE_OS_SELECTION

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------
/// enumerate the configuration types
typedef enum _CONFIGTYPE
{
  // enumerate user defined config blocks here

  // DO NOT REMOVE THE ENTRIES BELOW
  CONFIG_TYPE_MAX,
  CONFIG_TYPE_ILLEGAL = 0xFF
} CONFIGTYPE;

// global parameter declarations -----------------------------------------------
extern  const CODE CONFIGMGRBLKDEF atConfigDefs[ ];

// global function prototypes --------------------------------------------------
extern  void  ConfigManager_LocalInitialize( void );
extern  U8    ConfigManager_GetVerMajor( void );
extern  U8    ConfigManager_GetVerMinor( void );
extern  BOOL  ConfigManager_RdWord( U16 wAddress, PU16 wValue );
extern  BOOL  ConfigManager_RdBlock( U16 wAddress, U16 wLength, PU8 pnData );
extern  BOOL  ConfigManager_WrWord( U16 wAddress, U16 wValue );
extern  BOOL  ConfigManager_WrBlock( U16 wAddress, U16 wLength, PU8 pnData );

#if ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_TASKMANAGER )
  extern  BOOL  ConfigManager_ProcessUpdate( TASKARG xArg );
#endif // SYSTEMDEFINE_OS_SELECTION

/**@} EOF COnfigManager_cfg.h */

#endif  // CONFIGMANAGER_CFG_H