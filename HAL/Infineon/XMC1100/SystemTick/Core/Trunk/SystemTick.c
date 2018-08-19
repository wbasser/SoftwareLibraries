/******************************************************************************
 * @file SystemTick.c
 *
 * @brief system tick 
 *
 * This file contains the functions to initialize the system tick controller
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
 * \addtogroup SystemTick
 * @{
 *****************************************************************************/

// library includes
#include "Micro/Types.h"

// local includes -------------------------------------------------------------
#include "SystemTick/SystemTick.h"
#include "SystemTick/SystemTick_cfg.h"

// local parameter declarations -----------------------------------------------
static  U64   hSystemTime;
static  U32   uTickRateUsec;

// local function prototypes
void SysTick_Handler( void );

/******************************************************************************
 * @function SystemTick_Initialize
 *
 * @brief this initialized the system tick
 *
 * This function initializes the system tick 
 * 
 *****************************************************************************/
void SystemTick_Initialize( void )
{
  U32 uStatus;
  
  // clear the system time
  hSystemTime = 0;
  
  // initialize the system tick counter
  SystemCoreClockUpdate( );
  uStatus = SysTick_Config( SystemCoreClock / SYSTEMTICK_RATE_HZ );
  NVIC_SetPriority( SysTick_IRQn, 3 );

  // for now if status is not equal to 0
  if ( uStatus )
  {
    // stay here forever
    FOREVER
    {
      // do nothinhg
    }
  }

  // compute the tick rate in microseconds
  uTickRateUsec = 1000000ul / SYSTEMTICK_RATE_HZ;
}

/******************************************************************************
 * @function SystemTick_GetTickRateUsec
 *
 * @brief gets the tick rate in usecs
 *
 * This function returns the execution rate in USECS
 * 
 * @return  the current tick rate in usecs
 *
 *****************************************************************************/
U32 SystemTick_GetTickRateUsec( void )
{
  // return the system rate in USEC
  return( uTickRateUsec );
}

/******************************************************************************
 * @function SystemTick_GetTimeUsec
 *
 * @brief gets the elapsed system time in USECS
 *
 * This function returns the elapsed time in USECS
 * 
 * @return  the current elapsed time
 *
 *****************************************************************************/
U64 SystemTick_GetTimeUsec( void )
{
  U32 uMicroSeconds;
  
  // compute the fraction
  uMicroSeconds = (( SysTick->VAL * uTickRateUsec ) / SysTick->LOAD );
  
  // return the elpased time
  return( hSystemTime +  uMicroSeconds );
}

/******************************************************************************
 * @function SystemTick_GetTimeMsec
 *
 * @brief gets the elapsed system time in MSECS
 *
 * This function returns the elapsed time in MSECS
 * 
 * @return  the current elapsed time
 *
 *****************************************************************************/
U32 SystemTick_GetTimeMsec( void )
{
  // return the elapsed time in milliseconds
  return(( U32 )( hSystemTime / 1000 ));
}
/******************************************************************************
 * @function SysTick_Handler
 *
 * @brief system tick irq handler
 *
 * This function implements the IRQ handler for the system tick.  It will call
 * any functions listed in the configuration table
 * 
 *****************************************************************************/
void SysTick_Handler( void )
{
  U8  nIdx;
  PVSYSTEMTICKFUNC pvTickFunc;
 
  // reset the index
  nIdx = 0;
  
  // adjust the system time
  hSystemTime += uTickRateUsec;
  
  // process the loop
  while (( pvTickFunc = ( PVSYSTEMTICKFUNC )PGM_RDWORD( apvSystemTickFunctions[ nIdx++ ])) != NULL )
  {
    // call the init function
    pvTickFunc( );
  }
}
 
/**@} EOF SystemTick.c */
