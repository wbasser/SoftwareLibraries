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

// library includes -----------------------------------------------------------

// local includes -------------------------------------------------------------
#include "ConfigManager/ConfigManager_prm.h"

// Macros and Defines ---------------------------------------------------------
/// define the macro for creating config block entries
#ifdef __ATMEL_AVR__
#define CONFIGMGRBLKDEF( size, getdefault, getactual, eepromaddr ) \
  { \
    .wSize = size, \
    .pvGetDefault= ( PVGETPOINTER )getdefault, \
    .pvGetActual = ( PVGETPOINTER )getactual, \
    .pwEepromAddr = eepromaddr, \
  }
#else
#define CONFIGMGRBLKDEF( size, getdefault, getactual ) \
  { \
    .wSize = size, \
    .pvGetDefault= ( PVGETPOINTER )getdefault, \
    .pvGetActual = ( PVGETPOINTER )getactual, \
  }
#endif //__ATMEL_AVR__
  
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

/// define the structure for defining a config block control structure
typedef struct _CONFIGMGRDBLKEF
{
  U16           wSize;              ///< size of the config block
  PVGETPOINTER  pvGetDefault;       ///< get a pointer to the defaults
  PVGETPOINTER  pvGetActual;        ///< get a pointer to the actual table
  #ifdef __ATMEL_AVR__
  PU16          pwEepromAddr;        ///< pointer to the EEPROM storage
  #endif //__ATMEL_AVR__
} CONFIGMGRBLKDEF, *PCONFIGMGRBLKDEF;
#define CONFIGMGRBLKDEF_SIZE     sizeof( CONFIGMGRBLKDEF )

/**@} EOF ConfigManager_def.h */

#endif  // _CONFIGMANAGER_DEF_H