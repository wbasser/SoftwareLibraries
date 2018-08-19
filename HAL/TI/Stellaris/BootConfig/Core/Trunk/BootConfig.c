/******************************************************************************
 * @file BootConfig.c
 *
 * @brief Boot Configuration implementation
 *
 * This file provides the implementation for the Boot Configuration
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
 * \addtogroup BootConfig
 * @{
 *****************************************************************************/

// system includes ------------------------------------------------------------

// library includes -----------------------------------------------------------
#include "Types/Types.h"
#include "SystemTick/SystemTick.h"

// local includes -------------------------------------------------------------
#include "BootConfig/BootConfig.h"
#include "BootConfig/BootConfig_prm.h"

// Macros and Defines ---------------------------------------------------------
#define BOOT_CONFIG_COMMIT_ADDR 0x75100000

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations ----------------------------------------------

// local parameter declarations -----------------------------------------------

// local function prototypes --------------------------------------------------

// constant parameter initializations -----------------------------------------

/******************************************************************************
 * @function BootConfig_Initialize
 *
 * @brief initialize the boot config register
 *
 * This function will test the boot configuration register for the correct
 * value and will adjust it as such
 *
 *****************************************************************************/
void BootConfig_Initialize( void )
{
  U32   uCurrentBootConfig;
  
  // get the current state of the boot config register
  uCurrentBootConfig = HWREG( FLASH_BOOTCFG );
  
  // can it be modified
  if(( uCurrentBootConfig & FLASH_BOOTCFG_NW ) == FLASH_BOOTCFG_NW )
  {
    // now set the appropriate values
    uCurrentBootConfig &= ~( FLASH_BOOTCFG_PORT_M | FLASH_BOOTCFG_PIN_M | FLASH_BOOTCFG_POL | FLASH_BOOTCFG_EN );
    uCurrentBootConfig |= BOOT_CONFIG_PORT | BOOT_CONFIG_PIN | BOOT_CONFIG_POL | BOOT_CONFIG_ENAB;

    // set the clock rate
    HWREG(FLASH_USECRL) = SysCtlClockGet( ) / 1000000l;
     
    // now write it
    HWREG( FLASH_BOOTCFG ) = uCurrentBootConfig;
    HWREG( FLASH_FMD ) = uCurrentBootConfig;
    HWREG( FLASH_FMA ) = BOOT_CONFIG_COMMIT_ADDR;
    
    // now set the write and commit bit in the control
    HWREG( FLASH_FMC ) = FLASH_FMC_WRKEY | FLASH_FMC_COMT;

    // Wait until the write has completed.
    while( HWREG( FLASH_FMC ) & FLASH_FMC_COMT );
  }
}

/**@} EOF BootConfig.c */
