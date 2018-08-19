/******************************************************************************
 * @file ParameterManager_def.h
 *
 * @brief Parameter Manager definitions 
 *
 * This file provides the definitions for the parameter manager
 *
 * @copyright Copyright (c) 2012 Cyber Intergration
 * This document contains proprietary data and information of Cyber Integration 
 * LLC. It is the exclusive property of Cyber Integration, LLC and will not be 
 * disclosed in any form to any party without prior written permission of 
 * Cyber Integration, LLC. This document may not be reproduced or further used 
 * without the prior written permission of Cyber Integration, LLC.
 *
 * Version History
 * ======
 * $Log: $
 * 
 *
 * \addtogroup ParameterManager
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _PARAMETERMANAGER_DEF_H
#define _PARAMETERMANAGER_DEF_H

// system includes ------------------------------------------------------------
#include "Types/Types.h"

// local includes -------------------------------------------------------------
#include "ParameterManager/ParameterManager_prm.h"

// Macros and Defines ---------------------------------------------------------
/// define the parameter length
#if ( PARAM_SIZE_BYTES == 4 )
typedef	U32		  PARAMARG;
typedef PU32	  PPARAMARG;
#elif ( PARAM_SIZE_BYTES == 2 )
typedef	U16		  PARAMARG;
typedef PU16	  PPARAMARG;
#elif ( PARAM_SIZE_BYTES == 1 )
typedef	U8		  PARAMARG;
typedef	PU8		  PPARAMARG;
#else
typedef	U8		  PARAMARG;
typedef	PU8		  PPARAMARG;
#endif

/// define the helper macro for creating an entry into the parameter table
#define PARAM_ENTRY( defval, minval, maxval, locked ) \
  { \
    .xDefVal = defval, \
    .xMinVal = minval, \
    .xMaxVal = maxval, \
    .bLocked = locked \
  }

/// define the helper macro for creating an entry into the parameter table
#if ( PARAM_USE_NAMES == 1 )
#define PARAM_ENTRYNAM( defval, minval, maxval, locked, name ) \
  { \
    .xDefVal = defval, \
    .xMinVal = minval, \
    .xMaxVal = maxval, \
    .bLocked = locked, \
    .pszName = ( PC8 )&name \
  }
#endif
  
// enumerations ---------------------------------------------------------------
/// enumerate the errors
typedef enum _PARAMERRS
{
  PARAM_ERR_NONE  = 0,
	PARAM_ERR_PNUM  = 0xEE,
	PARAM_ERR_PVAL,
  PARAM_ERR_LOCKED,
  PARAM_ERR_MAX
} PARAMERRS;

// structures -----------------------------------------------------------------
// global type definitions
typedef struct _PARAMDEFAULTS
{
	#if ( PARAM_USE_NAMES == 1 )
	PC8	      pszName;        ///< pointer to the name
	#endif
	PARAMARG	xDefVal;        ///< default value
	PARAMARG	xMinVal;        ///< minimum value
	PARAMARG	xMaxVal;        ///< maximum value
  BOOL      bLocked;        ///< locked
} PARAMDFLTS, *PPARAMDFLTS;
#define PARAMDFLTS_SIZE       sizeof( PARAMDFLTS )

/**@} EOF ParameterManager_def.h */

#endif  // PARAMETERMANAGER_DEF_H