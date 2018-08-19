/******************************************************************************
 * @file Interrupt.c
 *
 * @brief interrupt manager
 *
 * This file provides the interrupt control and management
 *
 * @copyright Copyright (c) 2012 Cyber Integration
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

// library includes -----------------------------------------------------------
#include "Types/Types.h"

// local includes -------------------------------------------------------------
#include "Interrupt/Interrupt.h"
#include "Interrupt/Interrupt_prm.h"

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations ----------------------------------------------

// local parameter declarations -----------------------------------------------
static  U32     auPrvIrq[ IRQ_STACK_LEVEL ];
static  U8      nIrqIndex;
static  U32UN   tCurIpAddr;
static  VU32    uR0;
static  VU32    uR1;
static  VU32    uR2;
static  VU32    uR3;
static  VU32    uR12;
static  VU32    uLR;
static  VU32    uPC;
static  VU32    uPSR;

// local function prototypes --------------------------------------------------
static  void  GetRegistersFromStack( PU32 puFaultStackAddress );

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
  __set_PRIMASK( 15 );
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
//    __enable_irq( );
//    __set_PRIMASK( 0 );
    __enable_irq( );
  }
  
  // set the state
  bIrqStatus = ( __get_PRIMASK( ) == 0 ) ? TRUE : FALSE;
  
  // return the state of the interrupts
  return( bIrqStatus );
}

void HardFault_Handler( void )
{
  __asm volatile
  (
    " tst lr, #4 \n"
    " ite eq \n"
    " mrseq r0,msp \n"
    " mrsne r0, psp \n"
    " ldr r1, [r0, #24] \n"
    " ldr r2, handler2_address_const \n"
    " bx r2 \n"
    " handler2_address_const: .word GetRegistersFromStack \n"
   );

   FOREVER
   {
   }
}

static void GetRegistersFromStack( PU32 puFaultStackAddress )
{
  uR0 = puFaultStackAddress[ 0 ];
  uR1 = puFaultStackAddress[ 1 ];
  uR2 = puFaultStackAddress[ 2 ];
  uR3 = puFaultStackAddress[ 3 ];
  uR12 = puFaultStackAddress[ 4 ];
  uLR = puFaultStackAddress[ 5 ];
  uPC = puFaultStackAddress[ 6 ];
  uPSR = puFaultStackAddress[ 7 ];
}


/**@} EOF Interrupt.c */




