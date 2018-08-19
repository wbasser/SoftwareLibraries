/******************************************************************************
 * @file InputOutputManager_def.h
 *
 * @brief   Input/Output Manager definition declarations
 *
 * This file provides the declarations for the input/output manager definitions
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
 * \addtogroup InputOutputManager
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _INPUTOUTPUTMANAGER_DEF_H
#define _INPUTOUTPUTMANAGER_DEF_H

// local includes -------------------------------------------------------------

// library includes -----------------------------------------------------------
#include "GPIO/Gpio.h"

// Macros and Defines ---------------------------------------------------------
/// define the pin index for an input pin
#define IOMNGRDEF_INP_PIN_IDX         ( 0 )
#define IOMNGRDEF_OUT_RLYCTL_PIN_IDX  ( 0 )
#define IOMNGRDEF_OUT_RLYSET_PIN_IDX  ( 0 )
#define IOMNGRDEF_OUT_RLYRST_PIN_IDX  ( 1 )
#define IOMNGRDEF_OUT_MTXSET_PIN_IDX  ( 0 )
#define IOMNGRDEF_OUT_MTXRST_PIN_IDX  ( 1 )

/// define the number of io pins
#define MAX_IO_NUM_PINS               ( 2 )

/// define the helper macro for defining an input
#define IOMNGRDEFINP( pinenum, debounce, callback ) \
  { \
    .eType = IOMNGR_TYPE_INP, \
    .aePins[ IOMNGRDEF_INP_PIN_IDX ] = pinenum, \
    .wTime = debounce, \
    .pvCallback = callback, \
  }

/// define the helper macro for defining a relay
#define IOMNGRDEFRLY( pinenum ) \
  { \
    .eType = IOMNGR_TYPE_RLY, \
    .aePins[ IOMNGRDEF_OUT_RLYCTL_PIN_IDX ] = pinenum, \
}

/// define the helper macro for defining a dual coil latched relay
#define IOMNGRDEFDCRLYLATCH( setpin, rstpin, drivetime ) \
  { \
    .eType = IOMNGR_TYPE_RLYDCLATCH, \
    .aePins[ IOMNGRDEF_OUT_RLYSET_PIN_IDX ] = setpin, \
    .aePins[ IOMNGRDEF_OUT_RLYRST_PIN_IDX ] = rstpin, \
    .wTime = drivetime, \
  }

/// define the helper macro for defining a single coil latched relay
#define IOMNGRDEFSCRLYLATCH( setpin, rstpin, drivetime ) \
  { \
    .eType = IOMNGR_TYPE_RLYSCLATCH, \
    .aePins[ IOMNGRDEF_OUT_RLYSET_PIN_IDX ] = setpin, \
    .aePins[ IOMNGRDEF_OUT_RLYRST_PIN_IDX ] = rstpin, \
    .wTime = drivetime, \
  }

/// define the helper macro for defining a matrix relay
#define IOMNGTRDEFMTXRELAY( mtxenum, setidx, rstidx, drivetime ) \
  { \
    .eType = IOMNGR_TYPE_RLYMATRIX, \
    .nMtxEnum = mtxenum, \
    .aePins[ IOMNGRDEF_OUT_MTXSET_PIN_IDX ] = ( GPIOPINENUM )setidx, \
    .aePins[ IOMNGRDEF_OUT_MTXRST_PIN_IDX ] = ( GPIOPINENUM )rstidx, \
    .wTime = drivetime, \
  }
  
/// define the helper macro for defining a relay drive matrix
#define IOMNGRDEFRLYMATRIX( mtxpin0, mtxpin1, mtxpin2, mtxpin3, mtxpin4, mtxpin5, mtxpin6, mtxpin7 ) \
  { \
    .aePins[ 0 ] = mtxpin0, \
    .aePins[ 1 ] = mtxpin1, \
    .aePins[ 2 ] = mtxpin2, \
    .aePins[ 3 ] = mtxpin3, \
    .aePins[ 4 ] = mtxpin4, \
    .aePins[ 5 ] = mtxpin5, \
    .aePins[ 6 ] = mtxpin6, \
    .aePins[ 7 ] = mtxpin7, \
  }
  
// enumerations ---------------------------------------------------------------
/// enumerate the types
typedef enum _IOMNGRTYPE
{
  IOMNGR_TYPE_INP = 0,          ///< input 
  IOMNGR_TYPE_RLY,              ///< simple relay
  IOMNGR_TYPE_RLYDCLATCH,       ///< latched dual coil relay
  IOMNGR_TYPE_RLYSCLATCH,       ///< latched single coil relay
  IOMNGR_TYPE_RLYSAFETY,        ///< relay safety
  IOMNGR_TYPE_RLYMATRIX,        ///< relay matrix
  IOMNGR_TYPE_MAX
} IOMNGRTYPE;

/// enumerate the callbacks
typedef enum _IOMNGRCBEVENT
{
  IOMNGRCB_EVENT_NONE = 0,      ///< no event
  IOMNGRCB_EVENT_INPON,         ///< input on
  IOMNGRCB_EVENT_INPOFF,        ///< input off
} IOMNGRCBEVENT;

/// enumerate the relay matrix entries
typedef enum _IOMNGRRLYMTXIDX
{
  IOMNGR_RLYMTXIDX_0 = 0,
  IOMNGR_RLYMTXIDX_1,
  IOMNGR_RLYMTXIDX_2,
  IOMNGR_RLYMTXIDX_3,
  IOMNGR_RLYMTXIDX_4,
  IOMNGR_RLYMTXIDX_5,
  IOMNGR_RLYMTXIDX_6,
  IOMNGR_RLYMTXIDX_7,
  IOMNGR_RLYMTXIDX_MAX
} IOMNGRRLYMTXIDX;

// structures -----------------------------------------------------------------
/// define the callback for inputs
typedef void ( *PVIOMNGRCALLBACK )( IOMNGRCBEVENT );

/// define the IO definition structure
typedef struct _IOMNGRCHNDEF
{
  IOMNGRTYPE        eType;                  ///< I/O type
  U8                nMtxEnum;               ///< matrix enum
  U16               wTime;                  ///< debounce/drive time in milliseconds
  GPIOPINENUM       aePins[ MAX_IO_NUM_PINS ]; ///< I/O pins array
  PVIOMNGRCALLBACK  pvCallback;             ///< callback
} IOMNGRCHNDEF, *PIOMNGRCHNDEF;
#define IOMNGRCHNDEF_SIZE        ( IOMNGRCHNDEF )

/// define the relay matrix definition structure
typedef struct _IOMNGRMTXDEF
{
  GPIOPINENUM       aePins[ IOMNGR_RLYMTXIDX_MAX ];    ///< I/O pins array
} IOMNGRMTXDEF, *PIOMNGRMTXDEF;
#define IOMNGRMTXDEF_SIZE       ( IOMNGRMTXDEF )

/**@} EOF InputOutputManager_def.h */

#endif  // _INPUTOUTPUTMANAGER_DEF_H_ */