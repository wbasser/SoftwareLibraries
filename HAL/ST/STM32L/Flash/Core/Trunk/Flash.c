/******************************************************************************
 * @file Flash.c
 *
 * @brief Flash module implemntation 
 *
 * This file provides the implementation for the HAL flash module
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

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "Flash/Flash.h"

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations ----------------------------------------------

// local parameter declarations -----------------------------------------------

// local function prototypes --------------------------------------------------

// constant parameter initializations -----------------------------------------

/******************************************************************************
 * @function Flash_Initialize
 *
 * @brief flash module initialization
 *
 * This function will initialize the flash module
 *
 *****************************************************************************/
void Flash_Initialize( void )
{
  // initialize the flash 
  U32 uTemp;
  
  // build the value
  uTemp =  ( tFlashDef.bPrefetchEnb )  ? FLASH_ACR_PRFTEN : 0;
  uTemp |= ( tFlashDef.bInstCacheEnb ) ? FLASH_ACR_ICEN   : 0;
  uTemp |= ( tFlashDef.bDataCacheEnb ) ? FLASH_ACR_DCEN   : 0;
  uTemp |= BITS( tFlashDef.eWaitStates, FLASH_ACR_LATENCY_Pos );
  FLASH->ACR = uTemp;
}

/**@} EOF Flash.c */
