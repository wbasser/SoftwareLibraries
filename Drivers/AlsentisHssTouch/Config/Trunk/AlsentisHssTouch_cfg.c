/******************************************************************************
 * @file ALsentisHssTouch_cfg.c
 *
 * @brief Alsentis HSS touch configuration implementation 
 *
 * This file provides the touch configuration implementation
 *
 * @copyright Copyright (c) 2012CyberIntegration
 * This document contains proprietary data and information ofCyberIntegration 
 * LLC. It is the exclusive property ofCyberIntegration, LLC and will not be 
 * disclosed in any form to any party without prior written permission of 
 *CyberIntegration, LLC. This document may not be reproduced or further used 
 * without the prior written permission ofCyberIntegration, LLC.
 *
 * $Date: $
 *
 * Version History
 * ======
 * $Rev: $
 * 
 *
 * \addtogroup ALsentisHssTouch
 * @{
 *****************************************************************************/

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "AlsentisHssTouch/AlsentisHssTouch_cfg.h"

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations ----------------------------------------------

// local parameter declarations -----------------------------------------------

// local function prototypes --------------------------------------------------

// constant parameter initializations -----------------------------------------

/******************************************************************************
 * @function AlsentisHssTouch_StartCharging
 *
 * @brief start charging timers
 *
 * This function will start the timers for sensor charge
 *
 * @param[in[   ptPulseCOnfig   pointer to the pulse configuration 
 *
 *****************************************************************************/
void AlsentisHssTouch_StartCharging( PALSENTISPULSECONFIG ptPulseConfig )
{
}

/******************************************************************************
 * @function AlsentisHssTouch_StartAdcConvert
 *
 * @brief start an ADC conversion
 *
 * This function will set the address to store the conversion into and
 * start an ADC conversion
 *
 * @param[in]     eAdcEnum      ADC enumeration
 * @param[in]     pwValue       pointer for conversion results storage
 *
 *****************************************************************************/
void AlsentisHssTouch_StartAdcConvert( ADCENUM eAdcEnum, PU16 pwValue )
{
}

/******************************************************************************
 * @function AlsentisHssTouch_ProcessAdcCallback
 *
 * @brief ADC conversion callback handler
 *
 * This function will process the callback from the completion of the 
 * ADC conversion
 *
 * @param[in]     wValue      value of the conversion  
 *
 *****************************************************************************/
void AlsentisHssTouch_ProcessAdcCallback( U16 wValue )
{
}

/******************************************************************************
 * @function AlsentisHssTouch_PulseEnableCallback
 * 
 * @brief pulse enable callback
 *
 * This function will process the callback from the completion of the 
 * pulse timer
 *
 * @param[in]     eEvent      callback event
 * @param[in]     uOption     option
 *
 *****************************************************************************/
void AlsentisHssTouch_PulseEnableCallback( TIMERCBEVENT eEvent, U16 uOption )
{
}

/******************************************************************************
 * @function AlsentisHssTouch_DischargeSensors
 * 
 * @brief discharage sensors
 *
 * This function will force the sensor inputs to outputs and ground them
 * to allow discharge
 *
 *****************************************************************************/
void AlsentisHssTouch_DischargeSensors( void )
{
  // make all pins outputs

  // turn them all low
}

/******************************************************************************
 * @function AlsentisHssTouch_EnableSensors
 * 
 * @brief enable sensor inputs
 *
 * This function will force the sensor pins to be inputs to allow for
 * charging and sensing
 *
 *****************************************************************************/
void AlsentisHssTouch_EnableSensors( void )
{
  // set the ADC to a dummy channel

  // float all input pins
}

/******************************************************************************
 * @function AlsentisHssTouch_SetTouchStatusLed
 * 
 * @brief sets the state of the touch status led
 *
 * This function will eighter turn on/off the touch status led
 *
 * @param[in]   bState      state of the LED
 *****************************************************************************/
void AlsentisHssTouch_SetTouchStatusLed( BOOL bState )
{
  // set the state of the LED
}


/**@} EOF ALsentisHssTouch_cfg.c */
