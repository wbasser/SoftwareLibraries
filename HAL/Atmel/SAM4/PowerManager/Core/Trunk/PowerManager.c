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
 * @function  PowerManager_Initialize
 *
 * @brief power manager inialization
 *
 * This function will initialize the power manager
 *
 *****************************************************************************/
void PowerManager_Initialize( void )
{ 
  U8  nTblIdx, nPerId, nOffset, nBank;
  
  // for each index in table
  nTblIdx = 0;
  while(( nPerId = anPmcPeripheralsDef[ nTblIdx++ ]) != 0xFF )
  {
    // calculate the bank/offset
    nBank = nPerId / 32;
    nOffset = nPerId % 32;
    
    // enable the peripheral clock
    switch( nBank )
    {
      case 0 :
        PMC->PMC_PCER0 = BIT( nOffset );
        break;
        
      case 1 :
        PMC->PMC_PCER1 = BIT( nOffset );
        break;
        
      default :
        break;
    }
  }
}
 
/**@} EOF PowerManager.c */
