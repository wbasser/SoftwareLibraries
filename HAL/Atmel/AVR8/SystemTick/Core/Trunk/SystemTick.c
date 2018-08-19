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

// library includes -----------------------------------------------------------
#include "Timers/Timers.h"

// local parameter declarations -----------------------------------------------
static  U64   hSystemTime;
static  U32   uTickRateUsec;

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
  // return the elpased time
  return( hSystemTime );
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
 * @function SystemTick_Process
 *
 * @brief system tick irq handler
 *
 * This function implements the IRQ handler for the system tick.  It will call
 * any functions listed in the configuration table
 * 
 *****************************************************************************/
void SystemTick_ProcessTick( TIMERCBEVENT eCbEvent, U16 uValue )
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

/******************************************************************************
 * @function SystemTick_DelayMsecs
 *
 * @brief delay for a specified amount of milliseconds
 *
 * This function will delay for the given amount of milliseconds
 * 
 * @param[in]   nMsecs    number of milliseconds to delay for
 *
 *****************************************************************************/
void SystemTick_DelayMsec( U8 nMsecs )
{
  U32   uTargetTime;
  VU32  uCurrentTime;

  // get the current time
  uTargetTime = SystemTick_GetTimeMsec( ) + nMsecs;

  // now delay
  do 
  {
    uCurrentTime = SystemTick_GetTimeMsec( );
  } while( uCurrentTime < uTargetTime );
}
 
/**@} EOF SystemTick.c */
