/******************************************************************************
 * @file Timers.c
 *
 * @brief timers implementation
 *
 * This file provides the implementation for the timers module
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
 * \addtogroup Timers
 * @{
 *****************************************************************************/

// local includes -------------------------------------------------------------
#include "Timers/Timers.h"

// libary includes -------------------------------------------------------------

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations ----------------------------------------------

// local parameter declarations -----------------------------------------------
static	PVTIMERCALLBACK apvCallbacks[ 2 ];

// local function prototypes --------------------------------------------------
static void Setup8BitTimer( TIMERMODE8 eMode, U8 nPrescale, U8 nReload );
static void CommonCmpIrq( U8 nIndex );
static void CommonCapIrq( U8 nIndex );
static void CommonOvfIrq( U8 nIndex );

// constant parameter initializations -----------------------------------------

/******************************************************************************
 * @function Timers_Create
 *
 * @brief timer creation
 *
 * This function initialization all configured timers
 *
 * @param[in]		eTimerSize			timer size
 * @param[in]		nTimerMode			timer mode
 * @param[in]		nPrescale				prescle value
 * @param[in]		pvCallback			pointer to the callback
 *
 * @return			the timer enum, or 0 if error
 *
 *****************************************************************************/
TIMERENUM Timers_Create( TIMERSIZE eTimerSize, U8 nTimerMode, U8 nPrescale, U8 nReload, PVTIMERCALLBACK pvCallback )
{
  TIMERENUM eTimer = 0;
	
	// determine the timer size
	switch( eTimerSize )
	{
		case TIMER_SIZE_8BIT :
			// set the callback
			apvCallbacks[ 0 ] = pvCallback;
			
			// call the setup
			Setup8BitTimer(( TIMERMODE8 )nTimerMode, nPrescale, nReload );
			break;
			
		case TIMER_SIZE_16BIT :
			// set the callback
			apvCallbacks[ 1 ] = pvCallback;
			break;
			
		default :
			break;
	}
  
  // return the enuemratin
  return( eTimer );
}


#if defined( TIMER0_COMPA_vect )
/******************************************************************************
 * @function TIMER0_COMPA_vect
 *
 * @brief timer 0 compare A IRQ vector
 *
 * This function provides the timer 0 compare A IRQ vector
 *
 *****************************************************************************/
ISR( TIMER0_COMPA_vect )
{
  // call the common interrupt
  CommonCmpIrq( 0 );
}
#endif

#if defined( TIMER0_COMPB_vect )
/******************************************************************************
 * @function TIMER0_COMPB_vect
 *
 * @brief timer 0 compare B IRQ vector
 *
 * This function provides the timer 0 compare B IRQ vector
 *
 *****************************************************************************/
ISR( TIMER0_COMPB_vect )
{
  // call the common interrupt
  CommonCmpIrq( 0 );
}
#endif

#if defined( TIMER0_OVF_vect )
/******************************************************************************
 * @function TIMER0_OVF_vect
 *
 * @brief timer 0 overflow vector
 *
 * This function provides the timer 0 overflow vector
 *
 *****************************************************************************/
ISR( TIMER0_OVF_vect )
{
  // call the common interrupt
  CommonOvfIrq( 0 );
}
#endif

#if defined( TIMER1_CAPT_vect )
/******************************************************************************
 * @function TIMER1_CAPT_vect
 *
 * @brief timer 0 input capture IRQ vector
 *
 * This function provides the timer 1 input capture  IRQ vector
 *
 *****************************************************************************/
ISR( TIMER1_CAPT_vect )
{
  // call the common interrupt
  CommonCapIrq( 1 );
}
#endif

#if defined( TIMER1_COMPA_vect )
/******************************************************************************
 * @function TIMER1_COMPA_vect
 *
 * @brief timer 1 compare A IRQ vector
 *
 * This function provides the timer 1 compare A IRQ vector
 *
 *****************************************************************************/
ISR( TIMER1_COMPA_vect )
{
  // call the common interrupt
  CommonCmpIrq( 1 );
}
#endif

#if defined( TIMER1_COMPB_vect )
/******************************************************************************
 * @function TIMER1_COMPB_vect
 *
 * @brief timer 1 compare B IRQ vector
 *
 * This function provides the timer 1 compare B IRQ vector
 *
 *****************************************************************************/
ISR( TIMER1_COMPB_vect )
{
  // call the common interrupt
  CommonCmpIrq( 1 );
}
#endif

#if defined( TIMER1_COMPC_vect )
/******************************************************************************
 * @function TIMER1_COMPC_vect
 *
 * @brief timer 1 compare C IRQ vector
 *
 * This function provides the timer 1 compare C IRQ vector
 *
 *****************************************************************************/
ISR( TIMER1_COMPC_vect )
{
  // call the common interrupt
  CommonCmpIrq( 1 );
}
#endif

#if defined( TIMER1_OVF_vect )
/******************************************************************************
 * @function TIMER1_OVF_vect
 *
 * @brief timer 1 overflow IRQ vector
 *
 * This function provides the timer 1 overflow  IRQ vector
 *
 *****************************************************************************/
ISR( TIMER1_OVF_vect )
{
  // call the common interrupt
  CommonOvfIrq( 1 );
}
#endif

/******************************************************************************
 * @function Setup16BitTimer
 *
 * @brief 16 biit timer initialization
 *
 * This function configures a 16 bit timer to its selected mode of operation
 *
 * @param[in]   eMode				timer mode
 * @param[in]		nPrescale		prescale
 * @param[in]		nReload			reload value
 *
 *****************************************************************************/
static void Setup8BitTimer( TIMERMODE8 eMode, U8 nPrescale, U8 nReload )
{
  // configure the modes
  switch( eMode )
  {
    case TIMER_MODE8_NORM :
      break;
  
    case TIMER_MODE8_PWMPCM :
      break;
      
    case TIMER_MODE8_CTC :
      // enable CTC mode
      TCCR0A = BIT( WGM01 );
      
      // set the compare value
      OCR0A = nReload;
      
			// enable the interrupts
      TIMSK |= BIT( OCIE0A );

			// turn on the timer
			TCCR0B |= nPrescale; 
      break;
      
    case TIMER_MODE8_PWMFASTM :
      break;
      
    case TIMER_MODE8_PWMPCO :
      break;
      
    case TIMER_MODE8_PWMFASTO :
      break;
      
    default :
      break;
  }
}

/******************************************************************************
 * @function Setup16BitTimer
 *
 * @brief 16 biit timer initialization
 *
 * This function configures a 16 bit timer to its selected mode of operation
 *
 * @param[in]   eTimer    timer selection
 *
 *****************************************************************************/

/******************************************************************************
 * @function CommonCmpIrq
 *
 * @brief common compare interrupt handler
 *
 * This function processes the compare interrupt
 *
 * @param[in]   eTimer    timer selection
 *
 *****************************************************************************/
static void CommonCmpIrq( U8 nIndex )
{
  PVTIMERCALLBACK pvCallback;

  // get the callback
  pvCallback = apvCallbacks[ nIndex ];
  if ( pvCallback != NULL )
  {
    // call it
    pvCallback( TIMER_CBEVENT_CMPUP, 0 );
  }
}

/******************************************************************************
 * @function CommonCapIrq
 *
 * @brief common capture interrupt handler
 *
 * This function processes the capture interrupt
 *
 * @param[in]   eTimer    timer selection
 *
 *****************************************************************************/
static void CommonCapIrq( U8 nIndex )
{
}

/******************************************************************************
 * @function CommonOvfIrq
 *
 * @brief common overflow interrupt handler
 *
 * This function processes the overflow interrupt
 *
 * @param[in]   eTimer    timer selection
 *
 *****************************************************************************/
static void CommonOvfIrq( U8 nIndex )
{
}

/**@} EOF Timers.c */
