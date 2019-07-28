/******************************************************************************
 * @file FlashFileManager_cfg.c
 *
 * @brief	flash file manager configuration oimplementation 
 *
 * This file provides the implementation for the flash file manager configuration
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

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "FlashFileManager/FlashFileManager_cfg.h"

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations ----------------------------------------------

// local parameter declarations -----------------------------------------------

// local function prototypes --------------------------------------------------

// constant parameter initializations -----------------------------------------

/******************************************************************************
 * @function FlashFileManager_ReadBlock
 *
 * @brief flash file manager read a block
 *
 * This function will read a block of data
 *
 * @param[in]   uAddress		address of data
 * @param[in]   pnBuffer		pointer to the data block
 * @param[in]  	wBufLength	length to read
 *
 *****************************************************************************/
void FlashFileManager_ReadBlock( U32 uAddress, PU8 pnBuffer, U16 wBufLength )
{
}

/**@} EOF FlashFileManager_cfg.c */
