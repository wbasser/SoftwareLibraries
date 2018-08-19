/******************************************************************************
 * @file Micro.c
 *
 * @brief main system initialization and idle loop
 *
 * This file contains the system initializations system tick
 * initializations 
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
 * $Rev: $
 * 
 *
 * \addtogroup Micro
 * @{
 *****************************************************************************/

// local includes -------------------------------------------------------------
#include "Micro.h"
#include "MicroConfig/Micro_cfg.h"

// library includes -----------------------------------------------------------
#include "Interrupt/Interrupt.h"

/******************************************************************************
 * @function main
 *
 * @brief this is the main function
 *
 * This function initializes the system clock, initialization functions listed
 * in the config structure, system tick and the task manager.
 *
 * @return  -1 to indicate an error as we should never exit
 *
 *****************************************************************************/
int main( void )
{
  U8              nIdx;
  PVMICROINITFUNC pvInitFunc;
  PVMICROIDLEFUNC pvIdleFunc;
  
  // disable interrupts
  Interrupt_Disable( );
  
 // call the local initialization
 Micro_LocalInitialize( );

  // initialize IRQ disabled functions
  nIdx = 0;
  while (( pvInitFunc = ( PVMICROINITFUNC )PGM_RDWORD( apvInitIrqDsbFunctions[ nIdx++ ])) != NULL )
  {
    // call the init function
    pvInitFunc( );
  }
  
  // enable interrupts
  Interrupt_Enable( );
  
  // initialize IRQ enabled functions
  nIdx = 0;
  while (( pvInitFunc = ( PVMICROINITFUNC )PGM_RDWORD( apvInitIrqEnbFunctions[ nIdx++ ])) != NULL )
  {
    // call the init function
    pvInitFunc( );
  }
  
  // idle loop
  FOREVER
  {
	  // reset index
	  nIdx = 0;
	
	  // while not a null entry
    while (( pvIdleFunc = ( PVMICROIDLEFUNC )PGM_RDWORD( apvIdleFunctions[ nIdx++ ])) != NULL )
    {
      // call the init function
      pvIdleFunc( );
    }
    
    // check for pending tasks
    if ( !Micro_CheckTasksPending( ))
    {
      // go to sleep
      Micro_EnterSleepMode( );
    }
  }
  
  // return a -1
  return( -1 );
}

/******************************************************************************
 * @function Micro_Shutdown
 *
 * @brief shutdown all peripherals
 *
 * This function will shut down all peripherals
 *
 *****************************************************************************/
void Micro_Shutdown( void )
{
  U8                nIdx;
  PVMICROSHUTDNFUNC pvShutdownFunc;

  // disable interrupts
  Interrupt_Disable( );

  // initialize IRQ disabled functions
  nIdx = 0;
  while (( pvShutdownFunc = ( PVMICROSHUTDNFUNC ) PGM_RDWORD( apvShutdownFunctions[ nIdx++ ] )) != NULL )
  {
    // call the shutdown function
    pvShutdownFunc( );
  }
} 

/**@} EOF Micro.c */
