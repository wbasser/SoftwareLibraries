/******************************************************************************
 * @file SystemTick.c
 *
 * @brief system tick 
 *
 * This file contains the functions to initialize the system tick controller
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
 * \addtogroup SystemTick
 * @{
 *****************************************************************************/

// local includes -------------------------------------------------------------
#include "SystemTick/SystemTick.h"
#include "SystemTick/SystemTick_cfg.h"

// library includes
#include "Clock/CLock.h"
#include "Interrupt/Interrupt.h"
#if (SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_FREERTOS )
#include "FreeRTOS/FreeRTOS.h"
#endif // SYSTEMDEFINE_OS_SELECTION

// local parameter declarations -----------------------------------------------
static  U64   hSystemTime;
static  U32   uTickRateUsec;
static  VU32  vuDelayTime;

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
  
  // only initialize if not freerots
  #if (SYSTEMDEFINE_OS_SELECTION != SYSTEMDEFINE_OS_FREERTOS )
  // initialize the system tick counter
	SysTick->CTRL = 0;
	SysTick->LOAD = Clock_GetMainClkFreq( ) / SYSTEMTICK_RATE_HZ;
	SysTick->VAL  = 0;
	SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_TICKINT_Msk | SysTick_CTRL_ENABLE_Msk;
  #endif // SYSTEMDEFINE_OS_SELECTION

  // compute the tick ra\\te in microseconds
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
  #if (SYSTEMDEFINE_OS_SELECTION != SYSTEMDEFINE_OS_FREERTOS )
  // clear the control register
  SysTick->CTRL = 0;
  #endif // SYSTEMDEFINE_OS_SELECTION
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
  vuDelayTime = ( wMilliSeconds * 1000 ) / uTickRateUsec;
  Interrupt_Enable( );
  
  // loop while not done
  while( !SystemTick_IsDelayExpired( ));
}

/******************************************************************************
 * @function SystemTick_DelayMsecNoWait
 *
 * @brief delays for the prescribed milliseconds
 *
 * This function will compute the delay time, and wait till the time expires
 * 
 * @param[in] wMilliSeconds   the time in milliseconds to delay
 *
 *****************************************************************************/
void SystemTick_DelayMsecNoWait( U16 wMilliSeconds )
{
  // calculate the delay time
  Interrupt_Disable( );
  vuDelayTime = ( wMilliSeconds * 1000 ) / uTickRateUsec;
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
  return(( vuDelayTime == 0 ) ? TRUE : FALSE );
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
#if (SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_FREERTOS )
void  SystemTick_ProcessTick( void )
#else
void SysTick_Handler( void )
#endif // SYSTEMDEFINE_OS_SELECTION
{
  U8  nIdx;
  PVSYSTEMTICKFUNC pvTickFunc;
 
  // reset the index
  nIdx = 0;
  
  // adjust the system time
  hSystemTime += uTickRateUsec;
  
  // decrement the delay time
  if ( vuDelayTime != 0 )
  {
    vuDelayTime--;
  }

  // process the loop
  while (( pvTickFunc = ( PVSYSTEMTICKFUNC )PGM_RDWORD( g_apvSystemTickFunctions[ nIdx++ ])) != NULL )
  {
    // call the init function
    pvTickFunc( );
  }
}
 
/**@} EOF SystemTick.c */
