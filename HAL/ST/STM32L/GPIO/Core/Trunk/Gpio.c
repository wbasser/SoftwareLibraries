/******************************************************************************
 * @file GPIO.c
 *
 * @brief GPIO implementation
 *
 * This file implements the GPIO subsystem 
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
typedef struct _IRQMAP
{
  GPIOPINENUM   ePinEnum;
} IRQMAP, *PIRQMAP;

// global parameter declarations ----------------------------------------------

// local parameter declarations -----------------------------------------------
static  IRQMAP  atIrqMaps[ 16 ];

// local function prototypes --------------------------------------------------
static  GPIO_TypeDef* GetPort( GPIOPORT ePort );
static  void          IrqCommonHandler( U8 nIrqLine );

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
  U32           uMask;
  GPIOPINENUM   ePinEnum;
  U8            nRegIdx;
  GPIO_TypeDef* ptPort;

  // for each pin def in the list
  for( ePinEnum = 0; ePinEnum < GPIO_PIN_ENUM_MAX; ePinEnum++ )
  {
    // get a pointer to the definition structure
    ptDef = ( PGPIOPINDEF )&atGpioPinDefs[ ePinEnum ];
    ptPort = GetPort( ptDef->ePort );

    // set the mask
    uMask = BIT( ptDef->nPin );

    // set up the mode
    ptPort->MODER &= ~BITS( GPIO_MODER_MODE0_Msk, ( ptDef->nPin * GPIO_MODER_MODE1_Pos ));
    ptPort->MODER |= BITS( ptDef->eMode,  ( ptDef->nPin * GPIO_MODER_MODE1_Pos )); 

    // set up the open drain
    ptPort->OTYPER &= ~BITS( GPIO_OTYPER_OT0_Msk, ( ptDef->nPin * GPIO_OTYPER_OT1_Pos ));
    ptPort->OTYPER |= BITS( ptDef->eOpenDrn, ( ptDef->nPin * GPIO_OTYPER_OT1_Pos ));

    // set up the speed
    ptPort->OSPEEDR &= ~BITS( GPIO_OSPEEDR_OSPEED0_Msk, ( ptDef->nPin * GPIO_OSPEEDR_OSPEED1_Pos ));
    ptPort->OSPEEDR |= BITS( ptDef->eSpeed, ( ptDef->nPin * GPIO_OSPEEDR_OSPEED1_Pos ));

    // set up the pull up/down
    ptPort->PUPDR &= ~BITS( GPIO_PUPDR_PUPD0_Msk, ( ptDef->nPin * GPIO_PUPDR_PUPD1_Pos ));
    ptPort->PUPDR |= BITS( ptDef->ePupPdn, ( ptDef->nPin * GPIO_PUPDR_PUPD1_Pos ));

    // set up the funcion if not zero
    if ( ptDef->eFunc != GPIO_FUNCMUX_00 )
    {
      // set the AFR index
      nRegIdx = ( ptDef->nPin / 8 );

      // now set the AFR index
      ptPort->AFR[ nRegIdx ] |= BITS( ptDef->eFunc, (( ptDef->nPin % 8 ) * GPIO_AFRL_AFSEL1_Pos ));
    }

    // if this is an output pin, set the initial state
    if ( ptDef->eMode == GPIO_MODE_OUTPUT )
    {
      // if initial on, set the appropriate level
      if ( ptDef->bInitialState ^ ptDef->bInvert )
      {
        // turn on the pin
        ptPort->ODR |= uMask;
      }
    }

    // check for interrupt
    if ( ptDef->eIrqSense != GPIO_SENSE_NONE )
    {
      // set the config register index
      nRegIdx = ( ptDef->nPin / 4 );

      // set the port for that pin/store the index in the interrupt mapping
      atIrqMaps[ ptDef->nPin ].ePinEnum = ePinEnum;
      SYSCFG->EXTICR[ nRegIdx ] |= BITS( ptDef->ePort, (( ptDef->nPin % 4 ) * SYSCFG_EXTICR1_EXTI1_Pos ));

      // now set the edges
      switch( ptDef->eIrqSense )
      {
        case GPIO_SENSE_FALL :
          EXTI->FTSR1 |= uMask;
          break;

        case GPIO_SENSE_RISE :
          EXTI->RTSR1 |= uMask;
          break;

        case GPIO_SENSE_BOTH :
          EXTI->FTSR1 |= uMask;
          EXTI->RTSR1 |= uMask;
          break;

        default :
          break;
      }

      // unmask the pin interrupt
      EXTI->IMR1 |= uMask;

      // now enable the interrupt
      switch( ptDef->nPin )
      {
        case 0 :
          uMask = EXTI0_IRQn;
          break;

        case 1 :
          uMask = EXTI1_IRQn;
          break;

        case 2 :
          uMask = EXTI2_IRQn;
          break;

        case 3 :
          uMask = EXTI3_IRQn;
          break;

        case 4 :
          uMask = EXTI4_IRQn;
          break;

        case 5 :
        case 6 :
        case 7 :
        case 8 :
        case 9 :
        case 10 :
          uMask = EXTI9_5_IRQn;

        case 11 :
        case 12 :
        case 13 :
        case 14 :
        case 15 :
          uMask = EXTI15_10_IRQn;
          break;

        default :
          break;
      }

      // now enable the interrupt
      NVIC_EnableIRQ( uMask );
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
  GPIOERR       eError = GPIO_ERR_NONE;
  PGPIOPINDEF   ptDef;
  U32           uMask;
  GPIO_TypeDef* ptPort;
  
  // test for valid GPIO select
  if ( eGpioSel < GPIO_PIN_ENUM_MAX )
  {
    // get a pointer to the definition/port
    ptDef = ( PGPIOPINDEF )&atGpioPinDefs[ eGpioSel ];
    ptPort = GetPort( ptDef->ePort );

    // check for an output pin
    if ( ptDef->eMode == GPIO_MODE_OUTPUT )
    {
      // compute the mask
      uMask = BIT( ptDef->nPin );
    
      // set the appropriate state
      if ( !( bState ^ ptDef->bInvert ))
      {
        // adjust the mask
        uMask <<= 16;
      }
      
      // output it
      ptPort->BSRR = uMask;
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
  GPIOERR       eError = GPIO_ERR_NONE;
  PGPIOPINDEF   ptDef;
  U32           uMask, uPinData;
  GPIO_TypeDef* ptPort;

  // test for valid GPIO select
  if ( eGpioSel < GPIO_PIN_ENUM_MAX )
  {
    // get a pointer to the definition
    ptDef = ( PGPIOPINDEF )&atGpioPinDefs[ eGpioSel ];
    ptPort = GetPort( ptDef->ePort );
    
    // compute the mask
    uMask = BIT( ptDef->nPin );

    // get the pin status
    uPinData = ( ptPort->IDR & uMask );

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
  U32         uMask, uPin;
  GPIO_TypeDef* ptPort;

  // test for valid GPIO select
  if ( eGpioSel < GPIO_PIN_ENUM_MAX )
  {
    // get a pointer to the definition
    ptDef = ( PGPIOPINDEF )&atGpioPinDefs[ eGpioSel ];
    ptPort = GetPort( ptDef->ePort );
    
    // compute the mask
    uMask = BIT( ptDef->nPin );

    // if pin is set, adjust maxk
    if ( ptPort->IDR & uMask )
    {
      // adjust the mask
      uMask <<= 16;
    }

    // now set/reset the bit
    ptPort->BSRR = uMask;
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
 * @function GetPort
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
static GPIO_TypeDef* GetPort( GPIOPORT ePort )
{
  GPIO_TypeDef* ptPort;
  
  // now determine the port
  switch( ePort )
  {
    case GPIO_PORT_A :
      ptPort = GPIOA;
      break;
      
    case GPIO_PORT_B :
      ptPort = GPIOB;
      break;
      
    case GPIO_PORT_C :
      ptPort = GPIOC;
      break;

    case GPIO_PORT_D :
      ptPort = GPIOD;
      break;

    case GPIO_PORT_E :
      ptPort = GPIOE;
      break;
      
    case GPIO_PORT_F :
      ptPort = GPIOF;
      break;

    case GPIO_PORT_G :
      ptPort = GPIOG;
      break;

    case GPIO_PORT_H :
      ptPort = GPIOH;
      break;

    default :
      break;
  }
  
  // return the pointer
  return( ptPort );
}

/******************************************************************************
 * @function EXTI0_IRQHandler
 *
 * @brief interrupt EXTI0 handler
 *
 * This function handles the interrupts from EXTI0
 *
 *****************************************************************************/
void EXTI0_IRQHandler( void )
{
  // call the common handler
  IrqCommonHandler( 0 );
}

/******************************************************************************
 * @function EXTI1_IRQHandler
 *
 * @brief interrupt EXTI1 handler
 *
 * This function handles the interrupts from EXTI1
 *
 *****************************************************************************/
void EXTI1_IRQHandler( void )
{
}

/******************************************************************************
 * @function EXTI2_IRQHandler
 *
 * @brief interrupt EXTI2 handler
 *
 * This function handles the interrupts from EXTI2
 *
 *****************************************************************************/
void EXTI2_IRQHandler( void )
{
}

/******************************************************************************
 * @function EXTI3_IRQHandler
 *
 * @brief interrupt EXTI3 handler
 *
 * This function handles the interrupts from EXTI3
 *
 *****************************************************************************/
void EXTI3_IRQHandler( void )
{
}

/******************************************************************************
 * @function EXTI4_IRQHandler
 *
 * @brief interrupt EXTI4 handler
 *
 * This function handles the interrupts from EXTI4
 *
 *****************************************************************************/
void EXTI4_IRQHandler( void )
{
}

/******************************************************************************
 * @function EXTI59_IRQHandler
 *
 * @brief interrupt EXTI5-9 handler
 *
 * This function handles the interrupts from EXTI5-9
 *
 *****************************************************************************/
void EXTI9_5_IRQHandler( void )
{
}

/******************************************************************************
 * @function EXTI15_10_IRQHandler
 *
 * @brief interrupt EXTI15-10 handler
 *
 * This function handles the interrupts from EXTI15-10
 *
 *****************************************************************************/
void EXTI15_10_IRQHandler( void )
{
}

/******************************************************************************
 * @function EXTI15_10_IRQHandler
 *
 * @brief interrupt EXTI15-10 handler
 *
 * This function handles the interrupts from EXTI15-10
 *
 *****************************************************************************/
static void IrqCommonHandler( U8 nIrqLine )
{
  GPIOPINENUM       ePinEnum;
  PGPIOPINDEF       ptDef;
  PVGPIOIRQCALLBACK pvCallback;
  GPIO_TypeDef*     ptPort;
  U8                nRemappedIrqLine;

  // determine the interrupt line
  switch( nIrqLine )
  {
    case 0 :
    case 1 :
    case 2 :
    case 3 : 
    case 4 :
      nRemappedIrqLine = nIrqLine;
      break;

    case 5 :
      if ( EXTI->PR1 & EXTI_PR1_PIF5 )
      {
        nRemappedIrqLine = 5;
      } 
      else if ( EXTI->PR1 & EXTI_PR1_PIF6 )
      {
        nRemappedIrqLine = 6;
      } 
      else if ( EXTI->PR1 & EXTI_PR1_PIF7 )
      {
        nRemappedIrqLine = 7;
      } 
      else if ( EXTI->PR1 & EXTI_PR1_PIF8 )
      {
        nRemappedIrqLine = 8;
      } 
      else if ( EXTI->PR1 & EXTI_PR1_PIF9 )
      {
        nRemappedIrqLine = 9;
      }
      break;

    case 10 :
      if ( EXTI->PR1 & EXTI_PR1_PIF10 )
      {
        nRemappedIrqLine = 10;
      } 
      else if ( EXTI->PR1 & EXTI_PR1_PIF11 )
      {
        nRemappedIrqLine = 11;
      } 
      else if ( EXTI->PR1 & EXTI_PR1_PIF12 )
      {
        nRemappedIrqLine = 12;
      } 
      else if ( EXTI->PR1 & EXTI_PR1_PIF13 )
      {
        nRemappedIrqLine = 13;
      } 
      else if ( EXTI->PR1 & EXTI_PR1_PIF14 )
      {
        nRemappedIrqLine = 14;
      } 
      else if ( EXTI->PR1 & EXTI_PR1_PIF15 )
      {
        nRemappedIrqLine = 15;
      }
      break;

    default :
      break;
  }

  // get a pointer to the definition
  ePinEnum = atIrqMaps[ nRemappedIrqLine ].ePinEnum;
  ptDef = ( PGPIOPINDEF )&atGpioPinDefs[ ePinEnum ];
  ptPort = GetPort( ptDef->ePort );

  // get the callback
  pvCallback = ( PVGPIOIRQCALLBACK )ptDef->pvCallback;

  // check for null
  if ( pvCallback != NULL )
  {
    // call it
    pvCallback( ePinEnum, ( ptPort->IDR & BIT( ptDef->nPin )) ? HI : LO );
  }

  // clear the interrupt
  EXTI->PR1 = BIT( nRemappedIrqLine );
}


/**@} EOF Gpio.c */
