/******************************************************************************
 * @file ConfigManager_def.h
 *
 * @brief configuration manager definition declarations
 *
 * This file defines the structures and enumerations for the configuration
 * manager
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
#ifndef _CONFIGMANAGER_DEF_H
#define _CONFIGMANAGER_DEF_H

// system includes ------------------------------------------------------------
#include "Types/Types.h"

// library includes -----------------------------------------------------------

// local includes -------------------------------------------------------------
#include "ConfigManager/ConfigManager_prm.h"

// Macros and Defines ---------------------------------------------------------
/// define the macro for creating config block entries - fixed size
#define CONFIGMGRFIXBLKDEF( size, getdefault, getactual ) \
  { \
    .bFixedSize = TRUE, \
    .wSize = size, \
    .pvGetDefault= ( PVGETPOINTER )getdefault, \
    .pvGetActual = ( PVGETPOINTER )getactual, \
  }
  
/// define the macro for creating config block entries - dynamic size
#define CONFIGMGRVARBLKDEF( getsize, getdefault, getactual ) \
  { \
    .bFixedSize = FALSE, \
    .pvGetSize = ( PVGETSIZE )getsize, \
    .pvGetDefault= ( PVGETPOINTER )getdefault, \
    .pvGetActual = ( PVGETPOINTER )getactual, \
  }
  
// enumerations ---------------------------------------------------------------
/// enumerate the configuration source
typedef enum _CONFIGSOURCE
{
  CONFIG_SOURCE_EEPROM = 0,
  CONFIG_SOURCE_RAM,
  CONFIG_SOURCE_DFLTS,
} CONFIGSOURCE;

// structures -----------------------------------------------------------------
/// define the function pointer to get a pointer to default/data tables
typedef PU8 ( *PVGETPOINTER )( void );

/// define the function pointer to get the config size
typedef U16 ( *PVGETSIZE )( void );

/// define the structure for defining a config block control structure
typedef struct _CONFIGMGRDBLKEF
{
  BOOL          bFixedSize;         ///< fixed config size
  U16           wSize;              ///< size of the config block
  PVGETSIZE     pvGetSize;          ///< get a size
  PVGETPOINTER  pvGetDefault;       ///< get a pointer to the defaults
  PVGETPOINTER  pvGetActual;        ///< get a pointer to the actual table
} CONFIGMGRBLKDEF, *PCONFIGMGRBLKDEF;
#define CONFIGMGRBLKDEF_SIZE     sizeof( CONFIGMGRBLKDEF )

/**@} EOF ConfigManager_def.h */

#endif  // _CONFIGMANAGER_DEF_H