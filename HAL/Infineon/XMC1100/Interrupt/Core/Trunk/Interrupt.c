/******************************************************************************
 * @file Interrupt.c
 *
 * @brief interrupt manager
 *
 * This file provides the interrupt control and management
 *
 * @copyright Copyright (c) 2012 Nan Technologies
 * This document contains proprietary data and information of Nan Technologies 
 * LLC. It is the exclusive property of Nan Technologies, LLC and will not be 
 * disclosed in any form to any party without prior written permission of 
 * Nan Technologies, LLC. This document may not be reproduced or further used 
 * without the prior written permission of Nan Technologies, LLC.
 *
 * Version History
 * ======
 * $Rev: $
 * 
 *
 * \addtogroup  Interrupt 
 * @{
 *****************************************************************************/

// library includes -----------------------------------------------------------
#include "Micro/Types.h"

// local includes -------------------------------------------------------------
#include "Interrupt/Interrupt.h"
#include "Interrupt/Interrupt_prm.h"

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations ----------------------------------------------

// local parameter declarations -----------------------------------------------
static  U32   auPrvIrq[ IRQ_STACK_LEVEL ];
static  U8    nIrqIndex;

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
  // if room, store previous
  if ( nIrqIndex < IRQ_STACK_LEVEL )
  {
    // save current value, clear interrupt
    auPrvIrq[ nIrqIndex++ ] = __get_PRIMASK( );
  }

  // clear interrupt enable
  __set_PRIMASK( 1 );
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
  
  // if items on stack
  if ( nIrqIndex > 0 )
  {
    // restore previous state
    __set_PRIMASK( auPrvIrq[ --nIrqIndex ] );
  }
  else
  {
    // just enable interrupts
    __enable_irq( );
  }
  
  // set the state
  bIrqStatus = ( __get_PRIMASK( ) == 0 ) ? TRUE : FALSE;
  
  // return the state of the interrupts
  return( bIrqStatus );
}

/**@} EOF Interrupt.c */




