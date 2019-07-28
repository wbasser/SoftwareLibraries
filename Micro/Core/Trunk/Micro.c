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

// library includes -----------------------------------------------------------
#include "Interrupt/Interrupt.h"
#include "SystemTick/SystemTick.h"

// local parameter declarations -----------------------------------------------
static  U32 uMaxExecTimeMsec;

// local function prototypes --------------------------------------------------
#if ( MICRO_ENABLE_SYSTIMECMDS  == 1 )
/// command handlers
static  ASCCMDSTS CmdQrySys( U8 nCmdEnum );

// constant parameter initializations -----------------------------------------
/// declare the command strings
static  const CODE C8 szQrySys[ ]   = { "QRYSYS" };

/// initialize the command table
const CODE ASCCMDENTRY g_atMicroCmdHandlerTable[ ] =
{
  ASCCMD_ENTRY( szQrySys,  6, 0, ASCFLAG_COMPARE_NONE, 0, CmdQrySys ),

  // the entry below must be here
  ASCCMD_ENDTBL( )
};

/// define the response strings
static  const CODE C8 szRspSys[ ]   = { "RSYS, %8lu\n\r" };
#endif // MICRO_ENABLE_SYSTIMECMDS


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
#if ( SYSTEMDEFINE_OS_SELECTION != SYSTEMDEFINE_OS_MINIMAL )
  U8              nIdx;
  PVMICROINITFUNC pvInitFunc;
  PVMICROIDLEFUNC pvIdleFunc;
  U32             uStartTime, uStopTime, uDiffTime;
#endif // SYSTEMDEFINE_OS_SELECTION != SYSTEMDEFINE_OS_MINIMAL

  // disable interrupts
  Interrupt_Disable( );
  
  // call the local initialization
  #if ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_MINIMAL )
    Micro_LocalInitialize( );
  #endif // SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_MINIMAL

#if ( SYSTEMDEFINE_OS_SELECTION != SYSTEMDEFINE_OS_MINIMAL )
  // clear the execution time
  uMaxExecTimeMsec = 0;
  
  // initialize IRQ disabled functions
  nIdx = 0;
  while (( pvInitFunc = ( PVMICROINITFUNC )PGM_RDWORD( g_apvInitIrqDsbFunctions[ nIdx++ ])) != NULL )
  {
    // call the init function
    pvInitFunc( );
  }
#endif // SYSTEMDEFINE_OS_SELECTION != SYSTEMDEFINE_OS_MINIMAL
  
  // enable interrupts
  Interrupt_Enable( );
  
  // call the local initialization
  #if ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_MINIMAL )
    Micro_LocalIrqInitialize( );
  #endif // SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_MINIMAL
  
#if ( SYSTEMDEFINE_OS_SELECTION != SYSTEMDEFINE_OS_MINIMAL )
  // initialize IRQ enabled functions
  nIdx = 0;
  while (( pvInitFunc = ( PVMICROINITFUNC )PGM_RDWORD( g_apvInitIrqEnbFunctions[ nIdx++ ])) != NULL )
  {
    // call the init function
    pvInitFunc( );
  }
#endif // SYSTEMDEFINE_OS_SELECTION != SYSTEMDEFINE_OS_MINIMAL
  
  // idle loop
  FOREVER
  {
    // local the local idle
#if ( SYSTEMDEFINE_OS_SELECTION != SYSTEMDEFINE_OS_MINIMAL )
    // get the current time
    uStartTime = SystemTick_GetTimeMsec();
        
    // reset index
    nIdx = 0;
	
    // while not a null entry
    while (( pvIdleFunc = ( PVMICROIDLEFUNC )PGM_RDWORD( g_apvIdleFunctions[ nIdx++ ])) != NULL )
    {
      // call the init function
      pvIdleFunc( );
    }

    // get the stop time
    uStopTime = SystemTick_GetTimeMsec();
    
    // compute time
    if ( uStopTime < uStartTime )
    {
      uDiffTime = uStartTime - uStopTime;      
    }
    else
    {
      uDiffTime = uStopTime - uStartTime;
    }
    
    // is this the max
    if ( uDiffTime > uMaxExecTimeMsec )
    {
      uMaxExecTimeMsec = uDiffTime;
    }
    
    // check for pending tasks
    if ( !Micro_CheckTasksPending( ))
    {
      // go to sleep
      Micro_EnterSleepMode( );
    }
#else
    Micro_LocalIdle( );
#endif // SYSTEMDEFINE_OS_SELECTION != SYSTEMDEFINE_OS_MINIMAL
  }
  
  // return a -1
  return( -1 );
}

#if ( SYSTEMDEFINE_OS_SELECTION != SYSTEMDEFINE_OS_MINIMAL )
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
  while (( pvShutdownFunc = ( PVMICROSHUTDNFUNC ) PGM_RDWORD( g_apvShutdownFunctions[ nIdx++ ] )) != NULL )
  {
    // call the shutdown function
    pvShutdownFunc( );
  }
} 
#endif // SYSTEMDEFINE_OS_SELECTION != SYSTEMDEFINE_OS_MINIMAL

#if ( MICRO_ENABLE_SYSTIMECMDS  == 1 )
/******************************************************************************
 * @function CmdQrySys
 *
 * @brief query system execution time command handler
 *
 * This function outputs the current board title and version
 *
 * @return  Aappropriate statue
 *****************************************************************************/
static ASCCMDSTS CmdQrySys( U8 nCmdEnum )
{
  PC8   pcBuffer;
 
  // get a pointer to the buffer
  AsciiCommandHandler_GetBuffer( nCmdEnum, &pcBuffer );

  // output the version=
  SPRINTF_P( ( pcBuffer ), ( char const * )szRspSys, uMaxExecTimeMsec );

  // return the error
  return( ASCCMD_STS_OUTPUTBUFFER);
}
#endif // MICRO_ENABLE_SYSTIMECMDS

/**@} EOF Micro.c */
