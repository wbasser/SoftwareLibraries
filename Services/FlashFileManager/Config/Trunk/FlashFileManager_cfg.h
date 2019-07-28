/******************************************************************************
 * @file FlashFileManager_cfg.h
 *
 * @brief flash file manager configuration declarations
 *
 * This file provides the declarations for the flash file manager configuration
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
#ifndef _FLASHFILEMANAGER_CFG_H
#define _FLASHFILEMANAGER_CFG_H

// system includes ------------------------------------------------------------
#include "Types/Types.h"

// local includes -------------------------------------------------------------

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations -----------------------------------------------

// global function prototypes --------------------------------------------------
extern	void	FlashFileManager_ReadBlock( U32 uAddress, PU8 pnBuffer, U16 wBufLength );

/**@} EOF FlashFileManager_cfg.h */

#endif  // _FLASHFILEMANAGER_CFG_H