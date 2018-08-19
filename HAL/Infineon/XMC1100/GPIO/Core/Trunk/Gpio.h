/******************************************************************************
 * @file Gpio.h
 *
 * @brief GPIO declarations
 *
 * This file provides the declarations to the GPIO API's
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
 * \addtogroup GPIO
 * @{
 *****************************************************************************/
 
// ensure only one instatiation
#ifndef _GPIO_H
#define _GPIO_H

// local includes -------------------------------------------------------------
#include "GPIO/Gpio_cfg.h"
#include "GPIO/Gpio_prm.h"

// enumerations ---------------------------------------------------------------
/// enumerate the GPIO errors
typedef enum _GPIOERR
{
  GPIO_ERR_NONE = 0,    ///< no error
  GPIO_ERR_ILLGPIO,     ///< illegal GPIO selection
  GPIO_ERR_ILLACT,      ///< illegal IOCTL action
  GPIO_ERR_ILLMODE,     ///< illegal mode, (I.e. trying to set an input pin )
  GPIO_ERR_ILLDIR,      ///< illegal direction, (i.e. tryint to change a direction on a non IO pin )
} GPIOERR;

/// enumerate the GPIO IOCTL actions
typedef enum _GPIOACT
{
  GPIO_ACT_NOOP = 0,    ///< no operation
  GPIO_ACT_MAX,         ///< maximum number
} GPIOACT;

// global function prototypes --------------------------------------------------
extern  void    Gpio_Initialize( void );
extern  GPIOERR Gpio_Set( GPIOPINENUM eGpioSel, BOOL bState );
extern  GPIOERR Gpio_Get( GPIOPINENUM eGpioSel, PBOOL pbState );
extern  GPIOERR Gpio_Toggle( GPIOPINENUM eGpioSel );
extern  GPIOERR Gpio_Ioctl( GPIOPINENUM eGpioSel, GPIOACT eGpioAct, PVOID pvData );
#if ( GPIO_ENABLE_PIN_REFRESH == 1 )
extern  void    Gpio_Refresh( void );
#endif

/**@} EOF Gpio.h */

#endif  // _GPIO_H
