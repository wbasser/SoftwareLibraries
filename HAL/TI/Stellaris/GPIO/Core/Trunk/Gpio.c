/******************************************************************************
 * @file GPIO.c
 *
 * @brief GPIO implementation
 *
 * This file implements the GPIO subsystem 
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
 * \addtogroup GPIO
 * @{
 *****************************************************************************/

// system includes ------------------------------------------------------------

// library includes -----------------------------------------------------------

// local includes -------------------------------------------------------------
#include "GPIO/Gpio.h"

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------
/// define the control structure
//typedef struct _GPIOCTL
//{
//  
//} GPIOCTL;

// global parameter declarations ----------------------------------------------

// local parameter declarations -----------------------------------------------

// local function prototypes --------------------------------------------------

/******************************************************************************
 * @function Gpio_Initialize
 *
 * @brief initialize the GPIO handler
 *
 * This function initialized the GPIO handler, configures each pin in the list
 *
 *****************************************************************************/
void Gpio_Initialize( void )
{
  GPIOPINENUM   eIdx;
  PGPIODEF      ptDef;
    
  // for each list in the list
  for( eIdx = 0; eIdx < GPIO_PIN_ENUM_MAX; eIdx++ )
  {
    // get a pointer to the definition structure
    ptDef = ( PGPIODEF )&atGpioDefs[ eIdx ];
    
    // enable the peripheral
    MAP_SysCtlPeripheralEnable( ptDef->uPeripheral );
    
    // set the direction
    MAP_GPIODirModeSet( ptDef->uPortBase, ptDef->nPin, ptDef->ePinDir );
    
    // configure the pin
    MAP_GPIOPadConfigSet( ptDef->uPortBase, ptDef->nPin, ptDef->ePinStrength, ptDef->ePinMode );

    // enable the interrupt for this module
    if ( ptDef->eIntType != GPIO_PIN_INT_TYPE_NONE )
    {
      MAP_GPIOPinIntEnable( ptDef->uPortBase, ptDef->nPin );
      MAP_GPIOIntTypeSet( ptDef->uPortBase, ptDef->nPin, ptDef->eIntType );
      
      // if callback is not null
      if ( ptDef->pvCallBack != NULL )
      {
        // set the callback
        GPIOPortIntRegister( ptDef->uPortBase, ( PVOID )ptDef->pvCallBack );
      }
    }

    // if an output pin
    if ( ptDef->ePinDir == GPIO_PIN_DIR_OUT )
    {
      // set the initial state
      MAP_GPIOPinWrite( ptDef->uPortBase, ptDef->nPin, ( ptDef->bInitial ^ ptDef->bInvert ) ? ptDef->nPin : 0 );
    }
  }
}

/******************************************************************************
 * @function Gpio_Set
 *
 * @brief set a GPIO pin 
 *
 * This function will set a GPIO pin to a given state
 *
 * @param[in]   eGpioSel  GPIO pin enumeration
 * @param[in]   bState    desired state of the pin
 *
 * @return      GPIOERR   appropriate error if any
 *
 *****************************************************************************/
GPIOERR Gpio_Set( GPIOPINENUM eGpioSel, BOOL bState )
{
  GPIOERR   eError = GPIO_ERR_NONE;
  PGPIODEF  ptDef;
  
  // test for valid GPIO select
  if ( eGpioSel < GPIO_PIN_ENUM_MAX )
  {
    // get a pointer to the def
    ptDef = ( PGPIODEF )&atGpioDefs[ eGpioSel ];
    
    // set the pin to the desired state
    MAP_GPIOPinWrite( ptDef->uPortBase, ptDef->nPin, ( bState ^ ptDef->bInvert ) ? ptDef->nPin : 0 );
  }
  else
  {
    // set the error
    eError = GPIO_ERR_ILLGPIO;
  }
  
  // return the error
  return( eError );
}

/******************************************************************************
 * @function Gpio_Get
 *
 * @brief get a GPIO pin's value
 *
 * This function will return a GPIO's pin value
 *
 * @param[in]   eGpioSel  GPIO pin enumeration
 * @param[io]   pbState   pointer to the strorage element to return the state
 *                        of the GPIO pin
 *
 * @return      GPIOERR   appropriate error if any
 *
 *****************************************************************************/
GPIOERR Gpio_Get( GPIOPINENUM eGpioSel, PBOOL pbState )
{
  GPIOERR   eError = GPIO_ERR_NONE;
  PGPIODEF  ptDef;
  BOOL      bState;

  // test for valid GPIO select
  if ( eGpioSel < GPIO_PIN_ENUM_MAX )
  {
    // get a pointer to the def
    ptDef = ( PGPIODEF )&atGpioDefs[ eGpioSel ];
    
    // set the return value
    bState = ( MAP_GPIOPinRead( ptDef->uPortBase, ptDef->nPin ) != 0 ) ? TRUE : FALSE;
    
    // store it
    *( pbState ) = bState ^ ptDef->bInvert;
  }
  else
  {
    // set the error
    eError = GPIO_ERR_ILLGPIO;
  }

  // return the error
  return( eError );
}

/******************************************************************************
 * @function Gpio_Toggle
 *
 * @brief toggle a GPIO pin
 *
 * This function will toggle the state of a GPIO pin
 *
 * @param[in]   eGpioSel  GPIO pin enumeration
 *
 * @return      GPIOERR   appropriate error if any
 *
 *****************************************************************************/
GPIOERR Gpio_Toggle( GPIOPINENUM eGpioSel )
{
  GPIOERR   eError = GPIO_ERR_NONE;
  U8        nValue;
  PGPIODEF  ptDef;
  
  // test for valid GPIO select
  if ( eGpioSel < GPIO_PIN_ENUM_MAX )
  {
    // get a pointer to the def
    ptDef = ( PGPIODEF )&atGpioDefs[ eGpioSel ];
    
    // toggle the state
    nValue = ( U8 )GPIOPinRead( ptDef->uPortBase, ptDef->nPin ) ^ ptDef->nPin;
    MAP_GPIOPinWrite( ptDef->uPortBase, ptDef->nPin, nValue );
  }
  else
  {
    // set the error
    eError = GPIO_ERR_ILLGPIO;
  }

  // return the error
  return( eError );
}

/******************************************************************************
 * @function Gpio_Ioctl
 *
 * @brief GPIO I/O control
 *
 * This function allows the state, interrupt level, interrupt callback and
 * other parameters to be set/retrieved
 *
 * @param[in]   eGpioSel  GPIO pin enumeration
 * @param[in]   eGpioAct  IOCTL action
 * @param[i0]   pvData    pointer to the data to set/retrieve
 *
 * @return      GPIOERR   appropriate error if any
 *
 *****************************************************************************/
GPIOERR Gpio_Ioctl( GPIOPINENUM eGpioSel, GPIOACT eGpioAct, PVOID pvData )
{
  GPIOERR eError = GPIO_ERR_NONE;
  
  // test for valid GPIO select
  if ( eGpioSel < GPIO_PIN_ENUM_MAX )
  {
    // perform the action
    switch( eGpioAct )
    {
      case GPIO_ACT_SETDIR :
        break;
        
      case GPIO_ACT_IRQCTL :
        break;
        
      case GPIO_ACT_REGCLB :
        break;
        
      default :
        // set the error
        eError = GPIO_ERR_ILLACT;
        break;
    }
  }
  else
  {
    // set the error
    eError = GPIO_ERR_ILLGPIO;
  }

  // return the error
  return( eError );
}

/******************************************************************************
 * @function Gpio_Refresh
 *
 * @brief refresh the state of all the GPIO pins
 *
 * This function sell update the direction and current state of all GPIO pins
 *
  *****************************************************************************/
void Gpio_Refresh( void )
{
}

/**@} EOF Gpio.c */
