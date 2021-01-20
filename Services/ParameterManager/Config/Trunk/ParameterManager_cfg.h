/******************************************************************************
 * @file ParameterManager_cfg.h
 *
 * @brief parameter manager configuration declarations
 *
 * This file provides the declarations for the parameter manager
 *
 * @copyright Copyright (c) 2012 CyberIntegration
 * This document contains proprietary data and information of CyberIntegration
 * LLC. It is the exclusive property of CyberIntegration, LLC and will not be
 * disclosed in any form to any party without prior written permission of 
 * CyberIntegration, LLC. This document may not be reproduced or further used
 * without the prior written permission of CyberIntegration, LLC.
 *
 * Version History
 * ======
 * $Rev: $
 * 
 *
 * \addtogroup ParameterManager_cfg
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _PARAMETERMANAGER_CFG_H
#define _PARAMETERMANAGER_CFG_H

// local includes -------------------------------------------------------------
#include "ParameterManager/ParameterManager_def.h"

// macros/defines-------------------------------------------------------------

// enumerations ---------------------------------------------------------------
/// enumerate each parameter
typedef enum _PARAMSELENUM
{
  /// enumerate parameters below

  // do not remove these lines below
  PARAMSEL_MAX_NUM,
  PARAMSEL_ILLEGAL
} PARAMSELENUM;

// global parameter declarations -----------------------------------------------
extern  const PARAMDFLTS  atParamDefaults[ ];

// global function prototypes --------------------------------------------------
extern  U8    ParameterManager_GetVerMajor( void );
extern  U8    ParameterManager_GetVerMinor( void );
extern  BOOL  ParameterManager_RdByte( U16 wAddress, PU8 pnData );
extern  BOOL  ParameterManager_RdWord( U16 wAddress, PU16 pwData );
extern  BOOL  ParameterManager_RdLong( U16 wAddress, PU32 puData );
extern  BOOL  ParameterManager_RdBlock( U16 wAddress, U16 wLength, PU8 pnData );
extern  BOOL  ParameterManager_WrByte( U16 wAddress, U8 nData );
extern  BOOL  ParameterManager_WrWord( U16 wAddress, U16 wData );
extern  BOOL  ParameterManager_WrLong( U16 wAddress, U32 uData );
extern  BOOL  ParameterManager_WrBlock( U16 wAddress, U16 wLength, PU8 pnData );

/**@} EOF ParameterManager_cfg.h */

#endif  // _PARAMETERMANAGER_CFG_H
