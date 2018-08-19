/******************************************************************************
 * @file Timers.c
 *
 * @brief timers implementation 
 *
 * This file provides the implementation for the timers module
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
 * \addtogroup Timers
 * @{
 *****************************************************************************/

// system includes ------------------------------------------------------------

// library includes -----------------------------------------------------------

// local includes -------------------------------------------------------------
#include "Timers/Timers.h"
#include "GPIO/Gpio.h"

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations ----------------------------------------------

// local parameter declarations -----------------------------------------------

// local function prototypes --------------------------------------------------

// constant parameter initializations -----------------------------------------

/******************************************************************************
 * @function Timers_Initialize
 *
 * @brief initializes the timers
 *
 * This function initializes each timer in the configuration table
 *
 *****************************************************************************/
void Timers_Initialize( void )
{
  TIMERENUM   eIdx;
  PTIMERDEF   ptDef;
  U32         uTemp;
  
  // for each timer in the list
  for( eIdx = 0; eIdx < TIMER_ENUM_MAX; eIdx++ )
  {
    // get a pointer to the structure
    ptDef = ( PTIMERDEF )&atTimerDefs[ eIdx ];
    
    // enable the timer/GPIO peripherals
    MAP_SysCtlPeripheralEnable( ptDef->uTimePeriph );

    // if pin is used, enable it
    if ( ptDef->uGpioPin != 0 )
    {
      MAP_SysCtlPeripheralEnable( ptDef->uGpioPeriph );
    
      // is this a tempest part
      if (( CLASS_IS_TEMPEST ) && ( ptDef->uFuncPin != 0 ))
      {
        // set the mux
        MAP_GPIOPinConfigure( ptDef->uFuncPin );
      }

      // set the GPIO pin as a timer
      MAP_GPIOPinTypeTimer( ptDef->uGpioBase, ptDef->uGpioPin );
    }
    
    // configure the timer
    MAP_TimerConfigure( ptDef->uTimeBase, ptDef->eMode );
    
    // check for RTC
    if ( ptDef->eMode == TIMER_CFG_32_RTC )
    {
      // enable the RTC
      MAP_TimerRTCEnable( ptDef->uTimeBase );
    }
    
    // set the prescale, match, and count
    MAP_TimerPrescaleSet( ptDef->uTimeBase, ptDef->eSel, ptDef->uPrescale );
    MAP_TimerLoadSet( ptDef->uTimeBase, ptDef->eSel, ptDef->uCount );
    MAP_TimerMatchSet( ptDef->uTimeBase, ptDef->eSel, ptDef->uMatch );
    
    // is there a callback
    if ( ptDef->pvCallBack != NULL )
    {
      // register the callback
      TimerIntRegister( ptDef->uTimeBase, ptDef->eSel, ( PVOID )ptDef->pvCallBack );
      
      // set the interrupt masks
      uTemp = HWREG( ptDef->uTimeBase + TIMER_O_IMR );
      uTemp |= ptDef->eInts;
      MAP_TimerIntEnable( ptDef->uTimeBase, uTemp );
    }
    
    // check for active
    if ( ptDef->bEnabled )
    {
      // enable the timer
      MAP_TimerEnable( ptDef->uTimeBase, ptDef->eSel );
    }
  }
}

/******************************************************************************
 * @function Timers_GetCount
 *
 * @brief gets the current count
 *
 * This function returns the given count for the selected timer
 *
 * @param[in]   eTimerSel timer selection
 * @param[io]   puCount   pointer to store the current count
 *
 * @return      TIMERERR  appropriate timer error
 *
 *****************************************************************************/
TIMERERR Timers_GetCount( TIMERENUM  eTimerSel, PU32 puCount )
{
  TIMERERR  eError = TIMER_ERR_NONE;
  PTIMERDEF ptDef;
  
  // check for valid timer
  if ( eTimerSel < TIMER_ENUM_MAX )
  {
    // get a pointer to the structure
    ptDef = ( PTIMERDEF )&atTimerDefs[ eTimerSel ];
    
    // get the current count
    *( puCount ) = MAP_TimerValueGet( ptDef->uTimeBase, ptDef->eSel );
  }
  else
  {
    // set the error
    eError = TIMER_ERR_ILLTIM;
  }
  
  // return the error
  return( eError );
}

/******************************************************************************
 * @function Timers_GetCapture
 *
 * @brief gets the current capture value
 *
 * This function returns the current captured value
 *
 * @param[in]   eTimerSel timer selection
 * @param[io]   puCapture pointer to store the captured value
 *
 * @return      TIMERERR  appropriate timer error
 *
 *****************************************************************************/
TIMERERR Timers_GetCapture( TIMERENUM  eTimerSel, PU32 puCapture )
{
  TIMERERR  eError = TIMER_ERR_NONE;
  PTIMERDEF ptDef;
  
  // check for valid timer
  if ( eTimerSel < TIMER_ENUM_MAX )
  {
    // get a pointer to the structure
    ptDef = ( PTIMERDEF )&atTimerDefs[ eTimerSel ];
    
    // get the current count
    *( puCapture ) = MAP_TimerMatchGet( ptDef->uTimeBase, ptDef->eSel );
  }
  else
  {
    // set the error
    eError = TIMER_ERR_ILLTIM;
  }
  
  // return the error
  return( eError );
}

/******************************************************************************
 * @function Timers_Ioctl
 *
 * @brief IOCTL control 
 *
 * This function provides various control functions for the timer
 *
 * @param[in]   eTimerSel timer selection
 * @param[in]   eAction   IOCTL action
 * @param[io]   pvData    pointer to get/set a data value
 *
 * @return      TIMERERR  appropriate timer error
 *
 *****************************************************************************/
TIMERERR Timers_Ioctl( TIMERENUM  eTimerSel, TIMERACT eAction, PVOID pvData )
{
  TIMERERR  eError = TIMER_ERR_NONE;
  PTIMERDEF ptDef;
  PU32      puTemp;
  
  // check for valid timer
  if ( eTimerSel < TIMER_ENUM_MAX )
  {
    // get a pointer to the structure
    ptDef = ( PTIMERDEF )&atTimerDefs[ eTimerSel ];

    // deference the data
    puTemp = ( PU32 )pvData;

    // process the action
    switch( eAction )
    {
      case TIMER_ACT_IRQENB :
        // enable the interrupts
        MAP_TimerIntEnable( ptDef->uTimeBase, ptDef->eSel );
        break;
        
      case TIMER_ACT_IRQDSB :
        // disable all interrupts
        MAP_TimerIntDisable( ptDef->uTimeBase, ptDef->eSel );
        break;
        
      case TIMER_ACT_SETCNT :
        // set the counter value
        MAP_TimerLoadSet( ptDef->uTimeBase, ptDef->eSel, *( puTemp ));
        break;
        
      case TIMER_ACT_GETPRE :
        // get the prescale
        *( puTemp ) = ( U32 )MAP_TimerPrescaleGet( ptDef->uTimeBase, ptDef->eSel );
        break;
        
      case TIMER_ACT_SETPRE :
        // set the prescale
        MAP_TimerPrescaleSet( ptDef->uTimeBase, ptDef->eSel, *( puTemp ));
        break;
        
      case TIMER_ACT_ENBTIM :
        // enable the timer
        MAP_TimerEnable( ptDef->uTimeBase, ptDef->eSel );
        break;
        
      case TIMER_ACT_DSBTIM :
        // disable the timer
        MAP_TimerEnable( ptDef->uTimeBase, ptDef->eSel );
        break;
        
      default :
        // set the error
        eError = TIMER_ERR_ILLACT;
        break;
    }
  }
  else
  {
    // set the error
    eError = TIMER_ERR_ILLTIM;
  }
  
  // return the error
  return( eError );
}
 
/**@} EOF Timers.c */
