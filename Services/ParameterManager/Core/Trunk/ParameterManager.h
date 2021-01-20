/******************************************************************************
 * @file ParameterManager.h
 *
 * @brief parameter manager declarations
 *
 * This file provides the parameter manager declarations
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
 * \addtogroup ParameterManager
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _PARAMETERMANAGER_H
#define _PARAMETERMANAGER_H

// local includes -------------------------------------------------------------
#include "ParameterManager/ParameterManager_cfg.h"

// library includes -----------------------------------------------------------
#if ( PARAM_ENABLE_DEBUG_COMMANDS == 1 )
  #include "AsciiCommandHandler/AsciiCommandHandler.h"
#endif  // PARAM_ENABLE_DEBUG_COMMANDS

// global parameter declarations -----------------------------------------------
#if ( PARAM_ENABLE_DEBUG_COMMANDS == 1 )
  extern  const CODE ASCCMDENTRY g_atParamManagerCmdHandlerTable[ ];
#endif  // PARAM_ENABLE_DEBUG_COMMANDS

// global function prototypes --------------------------------------------------
extern	void	    ParameterManager_Initialize( BOOL bForceReset );
extern	void 	    ParameterManager_SetDefaults( void );
extern  void      ParameterManager_SetDefaultsRange( PARAMSELENUM eBegParam, PARAMSELENUM eEndParam );
extern	PARAMERRS	ParameterManager_GetValue( PARAMSELENUM eParam, PPARAMARG pxValue );
extern	PARAMERRS	ParameterManager_PutValue( PARAMSELENUM eParam, PARAMARG xValue, BOOL bBypassLock );
extern	PARAMERRS	ParameterManager_GetLimits( PARAMSELENUM eParam, PPARAMARG pxMinValue, PPARAMARG pxMaxValue );
#if ( PARAM_USE_NAMES == 1 )
  extern	PARAMERRS	ParameterMangaer_GetName( U8 nParameterMangaer_Num, PFC8 pszName );
#endif
extern  PARAMERRS ParameterManager_GetDefaultValue( PARAMSELENUM eParm, PPARAMARG pxDfltValue );
extern  BOOL      ParameterManager_GetDefaultStatus( void );

/**@} EOF ParameterManager.h */

#endif  // _PARAMETERMANAGER_H