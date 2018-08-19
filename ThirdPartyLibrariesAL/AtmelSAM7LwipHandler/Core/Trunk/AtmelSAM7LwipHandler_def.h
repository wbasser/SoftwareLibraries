/******************************************************************************
 * @file AtmelSAM7LwipHandler_def.h
 *
 * @brief Stellaris LWIP handler definitionsdeclarations
 *
 * This file provides the definitionsdeclarations for the ethernet handler
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
 * \addtogroup AtmelSAM7LwipHandler
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _ATMELSAM7LWIPHANDLER_DEF_H
#define _ATMELSAM7LWIPHANDLER_DEF_H

// system includes ------------------------------------------------------------
#include "Types/Types.h"

// Macros and Defines ---------------------------------------------------------
/// define the protocol handler helper macro
#define	LWIPPROT( taskinit, taskprocess )	\
                { .pvInit = taskinit, \
                  .pvProc = taskprocess, \
                }

/// define the function pointer format for the different structures
typedef void  ( *PVINITFUNC )( void );
typedef void  ( *PVPROCFUNC )( void );

// structures -----------------------------------------------------------------
/// define the scheduled task definition structure
typedef struct 	_LWIPPROTDEF
{
	PVINITFUNC  pvInit;		    ///< pointer to the task initializer
	PVPROCFUNC  pvProc;	      ///< pointer to the task process
} LWIPPROTDEF, *PLWIPPROTDEF;
#define	LWIPPROTDEF_SIZE	sizeof( LWIPPROTDEF )

/**@} EOF AtmelSAM7LwipHandler_def.h */

#endif  // _ATMELSAM7LWIPHANDLER_DEF_H