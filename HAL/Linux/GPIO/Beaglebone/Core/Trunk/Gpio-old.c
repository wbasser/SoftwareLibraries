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
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcnt.h>
#include <errno.h>

// library includes -----------------------------------------------------------

// local includes -------------------------------------------------------------
#include "GPIO/Gpio.h"

// Macros and Defines ---------------------------------------------------------
/// define the GpIO base registers
#define GPIO0_BASE_REG            ( 0x44E07000 )
#define GPIO1_BASE_REG            ( 0x4804C000 )
#define GPIO2_BASE_REG            ( 0x481AC000 )
#define GPIO3_BASE_REG            ( 0x481AE000 )

/// define the GPIO register length
#define GPIO_REG_LENGTH           ( 0x1000 )

/// define the GPIO register offsets
#define GPIO_OUTENB_OFFSET        ( 0x0134 )
#define GPIO_SETOUT_OFFSET        ( 0x0194 )
#define GPIO_CLROUT_OFFSET        ( 0x0190 )
#define GPIO_DATAIN_OFFSET        ( 0x0138 )

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations ----------------------------------------------

// local parameter declarations -----------------------------------------------
static  PU32  puMappedGpioRegs[ GPIO_PORT_MAX ];
static  S32   iMemFileHandle;

// local function prototypes --------------------------------------------------

// constant declarations ------------------------------------------------------
static U32  auGpioRegionAddr[ GPIO_PORT_MAX ] =
{
  GPIO0, GPIO1, GPIO2, GPIO3 
};

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
  GPIOPINENUM   ePin;
  GPIOPORT      eRegIdx;
  
  // open a memory file
  iMemFileHandle = open( "/dev/mem", O_RDWR );
  
  // for each register
  for ( eRegIdx = GPIO_PORT_0; eRegIdx < GPIO_PORT_MAX; eRegIdx++ )
  {
    // map the register
    puMappedGpioRegs[ eRegIdx ] = mmap( NULL, GPIO_REG_LENGTH, PROT_READ | PROT_WRITE, MAP_SHARED, iMemFileHandle, auGpioRegionAddr );
  }

  // for each pin def in the list
  for( ePin = 0; ePin < GPIO_PIN_ENUM_MAX; ePin++ )
  {
    // get a pointer to the definition structure
    ptDef = ( PGPIOPINDEF )&atGpioPinDefs[ ePin ];
    
    // set the direction
    switch( ptDef->eMode )
    {
      case GPIO_MODE_INPUT :
        *( puMappedGpioRegs[ ptDef->eRegIdx ] + GPIO_OUTENB_OFFSET ) &= ~( BIT( ptDef->nPin ));
        break;
        
      case GPIO_MODE_OUTPUT :
        *( puMappedGpioRegs[ ptDef->eRegIdx ] + GPIO_OUTENB_OFFSET ) |= BIT( ptDef->nPin );
        Gpio_Set( ePin, ptDef->bInitialState );
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
  // close the memory file handle
  if ( iMemFileHandle != 0 )
  {
    // close it
    close( iMemFileHandle );
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
  GPIOERR     eError = GPIO_ERR_NONE;
  PGPIOPINDEF ptDef;
  GPIOPINENUM ePin;
  
  // test for valid GPIO select
  if ( eGpioSel < GPIO_PIN_ENUM_MAX )
  {
    // get a pointer to the definition structure
    ptDef = ( PGPIOPINDEF )&atGpioPinDefs[ ePin ];
    
    // check for an output
    if ( ptDef->eMode == GPIO_MODE_OUTPUT )
    {
      // determine the state
      switch( bState ^ ptDef->bInvert )
      {
        case OFF :
          *( puMappedGpioRegs + GPIO_SETOUT_OFFSET ) = BIT( ptDef->nPin );
          break;
          
        case ON :
          *( puMappedGpioRegs + GPIO_CLROUT_OFFSET ) = BIT( ptDef->nPin );
          break;
          
        default :
          break;
      }
    }
    else
    {
      // set the error
      eError = GPIO_ERR_ILLMODE;
    }
  }
  else
  {
    // set the error
    eError = GPIO_ERR_ILLGPIO;
  }
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
  GPIOPINENUM ePin;
  
  // test for valid GPIO select
  if ( eGpioSel < GPIO_PIN_ENUM_MAX )
  {
    // get a pointer to the definition structure
    ptDef = ( PGPIOPINDEF )&atGpioPinDefs[ ePin ];
    
    // get the value of the pin
    *( pbState ) = *( puMappedGpioRegs + GPIO_DATAIN_OFFSET ) & BIT( ptDef->nPin );
    *( pbState ) ^= ptDef->bInvert;
  }
  else
  {
    // set the error
    eError = GPIO_ERR_ILLGPIO;
  }
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
  BOOL        bCurState;
  
  
  // test for valid GPIO select
  if ( eGpioSel < GPIO_PIN_ENUM_MAX )
  {
    // get a pointer to the definition structure
    ptDef = ( PGPIOPINDEF )&atGpioPinDefs[ ePin ];
    
    // check for an output
    if ( ptDef->eMode == GPIO_MODE_OUTPUT )
    {
      // get the current state/toggle it/set the new output
      bCurState = *( puMappedGpioRegs + GPIO_DATAIN_OFFSET ) & BIT( ptDef->nPin );
      bCurState ^= ON;

      // determine the state
      switch( bCurState )
      {
        case OFF :
          *( puMappedGpioRegs + GPIO_SETOUT_OFFSET ) = BIT( ptDef->nPin );
          break;
          
        case ON :
          *( puMappedGpioRegs + GPIO_CLROUT_OFFSET ) = BIT( ptDef->nPin );
          break;
          
        default :
          break;
      }
    }
    else
    {
      // set the error
      eError = GPIO_ERR_NOTVALID;
    }
  }
  else
  {
    // set the error
    eError = GPIO_ERR_ILLGPIO;
  }
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

/**@} EOF Gpio.c */
