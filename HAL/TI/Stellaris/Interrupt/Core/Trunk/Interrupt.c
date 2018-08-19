/******************************************************************************
 * @file Interrupt.c
 *
 * @brief interrupt manager
 *
 * This file provides the interrupt control and management
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
 * \addtogroup  Interrupt 
 * @{
 *****************************************************************************/

// library includes -----------------------------------------------------------
#include "Types/Types.h"

// StellarisWare Library includes
#include <driverlib/interrupt.h>
#include <driverlib/rom.h>
#include <driverlib/rom_map.h>

// local includes -------------------------------------------------------------
#include "Interrupt/Interrupt.h"
#include "Interrupt/Interrupt_prm.h"

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations ----------------------------------------------

// local parameter declarations -----------------------------------------------
static  U32 auPrvIrq[ IRQ_STACK_LEVEL ];
static  U8  nIrqIndex;

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
  // reset the index
  nIrqIndex = 0;
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
  // get the previous value/disable
  if( nIrqIndex < IRQ_STACK_LEVEL )
  {
    auPrvIrq[ nIrqIndex++ ] = MAP_IntMasterDisable( );
  }
  else
  {
    // just disable
    MAP_IntMasterDisable( );
  }
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
  BOOL  bIrqStatus = FALSE;
  
  // are we in to the irq stack
  if ( nIrqIndex == 0 )
  {
    // just enable the interrupts
    bIrqStatus = TRUE;
    MAP_IntMasterEnable( );
  }
  else
  {
    // retrieve the previous value and enable if true
    if( auPrvIrq[ --nIrqIndex ] == FALSE )
    {
      // enable interrupts
      bIrqStatus = TRUE;
      MAP_IntMasterEnable( );
    }
  }
  
  // return the state of the interrupts
  return( bIrqStatus );
}

/**@} EOF Interrupt.c */




