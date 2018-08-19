/******************************************************************************
 * @file ConfigManager.h
 *
 * @brief configuration manager declarations
 *
 * This file declares the interface to the configuration manager
 *
 * @copyright Copyright (c) 2012 Nan Technologies
 * This document contains proprietary data and information of Nan Technologies 
 * LLC. It is the exclusive property of Nan Technologies, LLC and will not be 
 * disclosed in any form to any party without prior written permission of 
 * Nan Technologies, LLC. This document may not be reproduced or further used 
 * without the prior written permission of Nan Technologies, LLC.
 *
 * $Date: $
 * 
 *
 * \addtogroup ConfigManager
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _CONFIGMANAGER_H
#define _CONFIGMANAGER_H

// system includes ------------------------------------------------------------

// library includes -----------------------------------------------------------

// local includes -------------------------------------------------------------
#include "ConfigManager/ConfigManager_cfg.h"

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations -----------------------------------------------

// global function prototypes --------------------------------------------------
extern  void  ConfigManager_Initialize( void );
extern  U16   ConfigManager_GetSignature( void );
extern  BOOL  ConfigManager_ResetDefaults( void );
extern  U16   ConfigManager_UpdateCRC( CONFIGSOURCE eSource );
extern  U16   ConfigManager_ComputeBlockCrc( CONFIGSOURCE eSource );
extern  void  ConfigManager_UpdateConfig( CONFIGTYPE eConfigType );

/**@} EOF ConfigManager.h */

#endif  // _CONFIGMANAGER_H