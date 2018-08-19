/******************************************************************************
 * @file SystemTick.c
 *
 * @brief system tick 
 *
 * This file contains the functions to initialize the system tick controller
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
 * \addtogroup SystemTick
 * @{
 *****************************************************************************/

// library includes
#include "Types/Types.h"
#include "Clock/CLock.h"
#include "Interrupt/Interrupt.h"

// local includes -------------------------------------------------------------
#include "SystemTick/SystemTick.h"
#include "SystemTick/SystemTick_cfg.h"

// local parameter declarations -----------------------------------------------
static  U64   hSystemTime;
static  U32   uTickRateUsec;
static  U32   uDelayTime;

// local function prototypes
//void SysTick_Handler( void );

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
  // clear the system time
  hSystemTime = 0;
  
  // initialize the system tick counter
	SysTick->CTRL = 0;
	SysTick->LOAD = Clock_GetFreq( ) / SYSTEMTICK_RATE_HZ;
	SysTick->VAL  = 0;
	SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_TICKINT_Msk | SysTick_CTRL_ENABLE_Msk;

  // compute the tick rate in microseconds
  uTickRateUsec = 1000000ul / SYSTEMTICK_RATE_HZ;
}

/******************************************************************************
 * @function SystemTick_Close
 *
 * @brief closes the system tick 
 *
 * This function closes the system tick
 * 
 *****************************************************************************/
void SystemTick_Close( void )
{
  // clear the control register
  SysTick->CTRL = 0;
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
 * @function SystemTick_DelayMsec
 *
 * @brief delays for the prescribed milliseconds
 *
 * This function will compute the delay time, and wait till the time expires
 * 
 * @param[in] wMilliSeconds   the time in milliseconds to delay
 *
 *****************************************************************************/
void SystemTick_DelayMsec( U16 wMilliSeconds )
{
  // calculate the delay time
  Interrupt_Disable( );
  uDelayTime = ( wMilliSeconds * 1000 ) / uTickRateUsec;
  Interrupt_Enable( );
}

/******************************************************************************
 * @function SystemTick_IsDelayExpired
 *
 * @brief returns the state of the current delay
 *
 * This function returns TRUE if the current delay time is expired
 * 
 * @return  TRUE if time expired, FALSE if not
 *
 *****************************************************************************/
BOOL SystemTick_IsDelayExpired( void )
{
  // return the state of delay time
  return(( uDelayTime == 0 ) ? TRUE : FALSE );
}

/******************************************************************************
 * @function IrqHandler
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
