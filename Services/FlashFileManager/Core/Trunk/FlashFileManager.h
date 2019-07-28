/******************************************************************************
 * @file FlashFileManager.h
 *
 * @brief flash file manager declarations
 *
 * This file provides the declarations for the Flash File Manager
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
 * $Rev: $
 * 
 *
 * \addtogroup FlashFileManager
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _FLASHFILEMANAGER_H
#define _FLASHFILEMANAGER_H

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "FlashFileManager/FlashFileManager_cfg.h"

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------
/// define the maximum file name
#define	FLASHFILE_MAX_FILE_NAME					( 256 )

// enumerations ---------------------------------------------------------------
/// enuemrate the errors
typedef enum _FLASHFILEERROR
{
  FLASHFILE_ERROR_NONE = 0,
  FLASHFILE_ERROR_NOTFOUND,
  FLASHFILE_ERROR_ILLHANDLE,
  FLASHFILE_ERROR_ENDOFFILE,
} FLASHFILEERROR;

// structures -----------------------------------------------------------------
typedef S16         FLASHFILEHANDLE;

// global parameter declarations -----------------------------------------------

// global function prototypes --------------------------------------------------
extern	void            FlashFileManager_Initialize( void );
extern	FLASHFILEHANDLE	FlashFileManager_Find( PC8 pszFileName );
extern	FLASHFILEERROR	FlashFileManager_Read( FLASHFILEHANDLE tHandle, PU8 pnBuffer, U16 wLength, PU16	puBytesRead );

/**@} EOF FlashFileManager.h */

#endif  // _FLASHFILEMANAGER_H