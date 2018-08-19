/******************************************************************************
 * @file PowerManager.c
 *
 * @brief power manager implementation
 *
 * This file provides the implementation for the power manager
 *
 * @copyright Copyright (c) 2012 CyberIntegration
 * This document contains proprietary data and information of CyberIntegration 
 * LLC. It is the exclusive property of CyberIntegration, LLC and will not be 
 * disclosed in any form to any party without prior written permission of 
 * CyberIntegration, LLC. This document may not be reproduced or further used 
 * without the prior written permission of CyberIntegration, LLC.
 *
 * $Date: $
 *
 * Version History
 * ======
 * $Rev: $
 * 
 *
 * \addtogroup PowerManager
 * @{
 *****************************************************************************/

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "PowerManager/PowerManager.h"

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations ----------------------------------------------

// local parameter declarations -----------------------------------------------

// local function prototypes --------------------------------------------------

// constant parameter initializations -----------------------------------------

/******************************************************************************
 * @function  PowerManager_DisableEnablePeriphB
 *
 * @brief peripheral enable/disable a peripheral
 *
 * This function will enable/disable the selected lperipheral
 *
 *****************************************************************************/
void PowerManager_DisableEnablePeriphB( U32 uPeriphMask, BOOL bState )
{
  // disable/enable the pheriphiral
  if ( bState )
  {
    // enable it
    PM->APBBMASK.reg |= uPeriphMask;
  }
  else
  {
    // disable it
    PM->APBBMASK.reg &= ~( uPeriphMask );
  }
}
 
/******************************************************************************
 * @function  PowerManager_DisableEnablePeriphC
 *
 * @brief peripheral enable/disable a peripheral
 *
 * This function will enable/disable the selected lperipheral
 *
 *****************************************************************************/
void PowerManager_DisableEnablePeriphC( U32 uPeriphMask, BOOL bState )
{
  // disable/enable the pheriphiral
  if ( bState )
  {
    // enable it
    PM->APBCMASK.reg |= uPeriphMask;
  }
  else
  {
    // disable it
    PM->APBCMASK.reg &= ~( uPeriphMask );
  }
}
 
/**@} EOF PowerManager.c */
