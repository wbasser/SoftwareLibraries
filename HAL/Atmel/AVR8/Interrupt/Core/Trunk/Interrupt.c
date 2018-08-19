/******************************************************************************
 * @file Interrupt.c
 *
 * @brief interrupt manager
 *
 * This file provides the interrupt control and management
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
 * \addtogroup  Interrupt 
 * @{
 *****************************************************************************/

// local includes -------------------------------------------------------------
#include "Interrupt/Interrupt.h"
#include "Interrupt/Interrupt_prm.h"

// library includes -----------------------------------------------------------
#include "Types/Types.h"

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations ----------------------------------------------

// local parameter declarations -----------------------------------------------
static  U8  anPrvIrq[ IRQ_STACK_LEVEL ];
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
  // if room, store previous
  if ( nIrqIndex < IRQ_STACK_LEVEL )
  {
    // save current value, clear interrupt
    anPrvIrq[ nIrqIndex++ ] = ( SREG & SREG_I );
  }

  // clear interrupt enable
  SREG &= ( U8 )~SREG_I;
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
    SREG |= anPrvIrq[ --nIrqIndex ];
  }
  else
  {
    // just enable interrupts
    sei( );
  }
  
  // set the state
  bIrqStatus = ( SREG & SREG_I ) ? TRUE : FALSE;
  
  // return the state of the interrupts
  return( bIrqStatus );
}

/******************************************************************************
 * @function Bad interrupt
 *
 * @brief bad interrupt catchall
 *
 * This function will restore the previous state if one is in the stack
 * otherwise it will just enable interrupts.
 *
 *****************************************************************************/
ISR( BADISR_vect )
{
  // kill the watchdog
  wdt_disable();

  // stay here forever
  FOREVER
  {

  }
}
/**@} EOF Interrupt.c */




