/******************************************************************************
 * @file GPIO.c
 *
 * @brief GPIO implementation
 *
 * This file implements the GPIO subsystem 
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
 * $Rev: $
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
/// define the interrupt control structure
//typedef struct _IRQMAP
//{
//  GPIOIRQENUM   eIrqEnum;
//} IRQMAP, *PIRQMAP;

// global parameter declarations ----------------------------------------------

// local parameter declarations -----------------------------------------------
//static  IRQMAP  atIrqMaps[ EIC_EXTINT_NUM ];

// local function prototypes --------------------------------------------------
static  Pio*  GetPioPointer( GPIOPORT ePort );

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
  PGPIOPINDEF   ptDef;
  PGPIOFNCDEF   ptFnc;
  //PGPIOIRQDEF   ptIrq;
  U32           uMask;
  Pio*          ptPio;
  GPIOPINENUM   ePin;
  U8            nIdx;

  // for each pin def in the list
  for( ePin = 0; ePin < GPIO_PIN_ENUM_MAX; ePin++ )
  {
    // get a pointer to the definition structure
    ptDef = ( PGPIOPINDEF )&atGpioPinDefs[ ePin ];

    // get the pointer to the structure
    ptPio = GetPioPointer( ptDef->ePort );
    
    // compute the mask
    uMask = BIT( ptDef->nPin );
    
    // enable the pin
    ptPio->PIO_PER = uMask;
    
    // now determine the mode
    switch( ptDef->eMode )
    {
      case GPIO_MODE_OUTPUT :
        // enable output
        ptPio->PIO_OER = uMask;
        
        // if initial on, set the appropriate level
        if ( ptDef->bInitialState ^ ptDef->bInvert )
        {
          // turn on the pin
          ptPio->PIO_SODR = uMask;
        }
        else
        {
          // turn off the pin
          ptPio->PIO_CODR = uMask;
        }
        break;
        
      case GPIO_MODE_INPUT :
      case GPIO_MODE_INPUT_PULLDN :
      case GPIO_MODE_INPUT_PULLUP :
        // disable output
        ptPio->PIO_ODR = uMask;
        
        // turn on the pull down if enabled
        if ( ptDef->eMode == GPIO_MODE_INPUT_PULLDN )
        {
          // enable the pull down
          ptPio->PIO_PPDER = uMask;
        }
        else
        {
          // disable the pull donw
          ptPio->PIO_PPDDR = uMask;
        }
        
        // turn on the pull up if enabled
        if ( ptDef->eMode == GPIO_MODE_INPUT_PULLUP )
        {
          // enable the pull UP
          ptPio->PIO_PUER = uMask;
        }
        else
        {
          // disable the pull UP
          ptPio->PIO_PUDR = uMask;
        }
        break;
      
      default :
        break;
    }
  } 
  
  // for each function def in the list
  for( nIdx = 0; nIdx < GPIO_FNC_ENUM_MAX; nIdx++ )
  {
    // get a pointer to the definition structure
    ptFnc = ( PGPIOFNCDEF )&atGpioFncDefs[ nIdx ];

    // get the pointer to the structure
    ptPio = GetPioPointer( ptFnc->ePort );
    
    // compute the mask
    uMask = BIT( ptFnc->nPin );
    
    // dsable the pin as I/O
    ptPio->PIO_PDR = uMask;
    
    // process the pull/updn
    switch( ptFnc->eMode )
    {
      case GPIO_MODE_FNC_PULLDN :
        ptPio->PIO_PPDER = uMask;
        ptPio->PIO_PUDR = uMask;
        break;

      case GPIO_MODE_FNC_PULLUP :
        ptPio->PIO_PPDDR = uMask;
        ptPio->PIO_PUER = uMask;
        break;

      case GPIO_MODE_FNC_PULLUPDN :
        ptPio->PIO_PPDER = uMask;
        ptPio->PIO_PUER = uMask;
        break;

      default :
        break;

    }

    // now determine the mode
    switch( ptFnc->eFunc )
    {
      case GPIO_FUNCMUX_A :
        ptPio->PIO_ABCDSR[ 0 ] &= ~uMask;
        ptPio->PIO_ABCDSR[ 1 ] &= ~uMask;
        break;
        
      case GPIO_FUNCMUX_B :
        ptPio->PIO_ABCDSR[ 0 ] |= uMask;
        ptPio->PIO_ABCDSR[ 1 ] &= ~uMask;
        break;
        
      case GPIO_FUNCMUX_C :
        ptPio->PIO_ABCDSR[ 0 ] &= ~uMask;
        ptPio->PIO_ABCDSR[ 1 ] |= uMask;
        break;
        
      case GPIO_FUNCMUX_D :
        ptPio->PIO_ABCDSR[ 0 ] |= uMask;
        ptPio->PIO_ABCDSR[ 1 ] |= uMask;
        break;
      
      default :
        break;
    }
  } 
}

/******************************************************************************
 * @function Gpio_Close
 *
 * @brief close all GPIO functions
 *
 * This function will turn off all GPIO interrups
 *
 *****************************************************************************/
void Gpio_Close( void )
{
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
  GPIOERR     eError = GPIO_ERR_NONE;
  PGPIOPINDEF ptDef;
  U32         uMask;
  Pio*        ptPio;
  
  // test for valid GPIO select
  if ( eGpioSel < GPIO_PIN_ENUM_MAX )
  {
    // get a pointer to the definition
    ptDef = ( PGPIOPINDEF )&atGpioPinDefs[ eGpioSel ];

    // get the pointer to the structure
    ptPio = GetPioPointer( ptDef->ePort );
    
    // check for an output pin
    if ( ptDef->eMode == GPIO_MODE_OUTPUT )
    {
      // compute the mask
      uMask = BIT( ptDef->nPin );
    
      // set the appropriate state
      if ( bState ^ ptDef->bInvert )
      {
        // turn on the pin
        ptPio->PIO_SODR = uMask;
      }
      else
      {
        // turn off the pin
        ptPio->PIO_CODR = uMask;
      }
    }
    else
    {
      // set the error
      eError = GPIO_ERR_ILLDIR;
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
  GPIOERR     eError = GPIO_ERR_NONE;
  PGPIOPINDEF ptDef;
  U32         uMask, uPinData;
  Pio*        ptPio;

  // test for valid GPIO select
  if ( eGpioSel < GPIO_PIN_ENUM_MAX )
  {
    // get a pointer to the definition
    ptDef = ( PGPIOPINDEF )&atGpioPinDefs[ eGpioSel ];
    
    // get the pointer to the structure
    ptPio = GetPioPointer( ptDef->ePort );
    
    // compute the mask
    uMask = BIT( ptDef->nPin );

    // get the pin status
    uPinData = ptPio->PIO_ODSR & uMask;

    // check for invert
    if ( ptDef->bInvert )
    {
      // invert bit
      uPinData ^= uMask;
    }
    *pbState = ( uPinData ) ? TRUE : FALSE;
  }
  else
  {
    // set the error
    eError = GPIO_ERR_ILLGPIO;
  }

  // return the error
  return( eError );
}

///******************************************************************************
// * @function Gpio_GetIrq
// *
// * @brief get a GPIO IRQ pin's value
// *
// * This function will return a GPIO's pin value
// *
// * @param[in]   eGpioSel  GPIO pin enumeration
// * @param[io]   pbState   pointer to the strorage element to return the state
// *                        of the GPIO pin
// *
// * @return      GPIOERR   appropriate error if any
// *
// *****************************************************************************/
//GPIOERR Gpio_GetIrq( GPIOIRQENUM eGpioSel, PBOOL pbState )
//{
//  GPIOERR     eError = GPIO_ERR_NONE;
//  PGPIOIRQDEF ptDef;
//  U32         uMask, uPort;
//  
//  // test for valid GPIO select
//  if ( eGpioSel < GPIO_IRQ_ENUM_MAX )
//  {
//    // get a pointer to the definition
//    ptDef = ( PGPIOIRQDEF )&atGpioIrqDefs[ eGpioSel ];
//    
//    // compute the mask
//    uMask = BIT( ptDef->nPin );
//    
//    // get the value
//    uPort = PORT->Group[ ptDef->ePort ].IN.reg;
//    uPort &= uMask;
//    *pbState = ( uPort ) ? TRUE : FALSE;
//  }
//  else
//  {
//    // set the error
//    eError = GPIO_ERR_ILLGPIO;
//  }
//
//  // return the error
//  return( eError );
//}

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
  GPIOERR     eError = GPIO_ERR_NONE;
  PGPIOPINDEF ptDef;
  U32         uMask;
  Pio*        ptPio;

  // test for valid GPIO select
  if ( eGpioSel < GPIO_PIN_ENUM_MAX )
  {
    // get a pointer to the definition
    ptDef = ( PGPIOPINDEF )&atGpioPinDefs[ eGpioSel ];
    
    // get the pointer to the structure
    ptPio = GetPioPointer( ptDef->ePort );

    // compute the mask
    uMask = BIT( ptDef->nPin );

    // determine the present state
    if ( ptPio->PIO_ODSR & uMask )
    {
      // clear the pin
      ptPio->PIO_CODR = uMask;
    }
    else
    {
      // set the pin
      ptPio->PIO_SODR = uMask;
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
  GPIOERR     eError = GPIO_ERR_NONE;
  // PGPIOPINDEF ptDef;
  
  // test for valid GPIO select
  if ( eGpioSel < GPIO_PIN_ENUM_MAX )
  {
    // perform the action
    switch( eGpioAct )
    {
      case GPIO_ACT_SETDIR :
        break;
        
      case GPIO_ACT_SETMODE :
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

/******************************************************************************
 * @function GetPioPointer
 *
 * @brief get the PIO pointer
 *
 * This function will return the pointer to the register sturcture
 *
 * @param[in]   ePort     port enumeration
 *
 * @return      pointer to the register structure
 *
  *****************************************************************************/
static Pio* GetPioPointer( GPIOPORT ePort )
{
  Pio*  ptPio;
  
  // now determine the port
  switch( ePort )
  {
    case GPIO_PORT_A :
      ptPio = PIOA;
      break;
      
    case GPIO_PORT_B :
      ptPio = PIOB;
      break;
      
    case GPIO_PORT_C :
      ptPio = PIOC;
      break;

    #ifdef PIOD
    case GPIO_PORT_D :
      ptPio = PIOD;
      break;
    #endif // PIOD

    #ifdef PIOE
    case GPIO_PORT_E :
      ptPio = PIOE;
      break;
    #endif // PIOE
      
    default :
      ptPio = NULL;
      break;
  }
  
  // return the pointer
  return( ptPio );
}

/**@} EOF Gpio.c */
