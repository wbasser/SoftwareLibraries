/******************************************************************************
 * @file Interrupt.c
 *
 * @brief interrupt manager
 *
 * This file provides the interrupt control and management
 *
 * @copyright Copyright (c) 2012 CyberIntegration
 * This document contains proprietary data and information of CyberIntegration 
 * LLC. It is the exclusive property of CyberIntegration, LLC and will not be 
 * disclosed in any form to any party without prior written permission of 
 * CyberIntegration, LLC. This document may not be reproduced or further used 
 * without the prior written permission of CyberIntegration, LLC.
 *
 * Version History
 * ======
 * $Log: $
 * 
 *
 * \addtogroup  Interrupt 
 * @{
 *****************************************************************************/

// local includes -------------------------------------------------------------
#include "Interrupt/Interrupt.h"

// library includes -----------------------------------------------------------
#include "Types/Types.h"

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations ----------------------------------------------

// local parameter declarations -----------------------------------------------

// local function prototypes --------------------------------------------------

/******************************************************************************
 * @function Interrupt_Initializle
 *
 * @brief intialize the interrupt handler
 *
 * This function resets the stack level index
 *
 *****************************************************************************/
void Interrupt_Initialize( void )
{
}


/******************************************************************************
 * @function Interrupt_Disable
 *
 * @brief disable the global interrupts
 *
 * This function will disable the interrupts and if there is room in the stack
 * store the previous function of the interrupt enable state
 *
 *****************************************************************************/
void Interrupt_Disable( void )
{
  U32 uFlags;
  
  // disable interrupt
//  local_irq_save( uFlags );
}

/******************************************************************************
 * @function Interrupt_Enable
 *
 * @brief restores the previous state of the interrupt enable
 *
 * This function will restore the previous state if one is in the stack
 * otherwise it will just enable interrupts.
 *
 * @return  the current interrupt enable state
 *
 *****************************************************************************/
BOOL Interrupt_Enable( void )
{
  U32   uFlags;
  BOOL  bIrqStatus = FALSE;
  
  // restore interrupts
//  local_irq_restore( uFlags );
  
  // return the state of the interrupts
  return( bIrqStatus );
}

/**@} EOF Interrupt.c */




