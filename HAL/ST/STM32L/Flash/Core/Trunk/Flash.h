/******************************************************************************
 * @file Flash.h
 *
 * @brief Flash HAL module implemtation declarations 
 *
 * This file provides the declarations for the flash module
 *
 * @copyright Copyright (c) 2012 Cyber Integration
 * This document contains proprietary data and information of Cyber Integration 
 * LLC. It is the exclusive property of Cyber Integration, LLC and will not be 
 * disclosed in any form to any party without prior written permission of 
 * Cyber Integration, LLC. This document may not be reproduced or further used 
 * without the prior written permission of Cyber Integration, LLC.
 *
 * $Date: $
 *
 * Version History
 * ======
 * $Rev: $
 * 
 *
 * \addtogroup Flash
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _FLASH_H
#define _FLASH_H

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "Flash/Flash_cfg.h"

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations -----------------------------------------------

// global function prototypes --------------------------------------------------
extern  void  Flash_Initialize( void );

/**@} EOF Flash.h */

#endif  // _FLASH_H