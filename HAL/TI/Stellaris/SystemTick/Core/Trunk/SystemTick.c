/******************************************************************************
 * @file SystemTick.c
 *
 * @brief system tick 
 *
 * This file contains the functions to initialize the system tick controller
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
 * \addtogroup SystemTick
 * @{
 *****************************************************************************/

// local includes -------------------------------------------------------------
#include "SystemTick/SystemTick.h"
#include "SystemTick/SystemTick_cfg.h"
#include "SystemTick/SystemTick_prm.h"
#include "Interrupt/Interrupt.h"

// StellarisWare Library includes
#include "driverlib/sysctl.h"
#include "driverlib/systick.h"
#include "driverlib/interrupt.h"

// local parameter declarations -----------------------------------------------
static  U64   hSystemTime;
static  U32   uTickRateUsec;
static  U32   uDelayTime;

// local function prototypes --------------------------------------------------
static  void  IrqHandler( void );

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
  
  // initialize the system tick
  SysTickPeriodSet( SysCtlClockGet( ) / SYSTEMTICK_RATE_HZ );
  SysTickIntRegister( IrqHandler );
  SysTickIntEnable( );
  SysTickEnable( );
  
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
static  void  IrqHandler( void )
{
  U8  nIdx;
  PVSYSTEMTICKFUNC pvTickFunc;
  
  // adjust the system time
  hSystemTime += uTickRateUsec;

  // if time is not zero
  if ( uDelayTime != 0 )
  {
    // decrement the time
    uDelayTime--;
  }

  // process the tasks in the idle loop
  nIdx = 0;
  while (( pvTickFunc = apvSystemTickFunctions[ nIdx++ ]) != NULL )
  {
    // call the init function
    pvTickFunc( );
  }
}
 
/**@} EOF SystemTick.c */
