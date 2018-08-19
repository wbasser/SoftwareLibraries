/******************************************************************************
 * @file GPIO.c
 *
 * @brief GPIO implementation
 *
 * This file implements the GPIO subsystem 
 *
 * @copyright Copyright (c) 2012 Guardhat
 * This document contains proprietary data and information of Guardhat 
 * LLC. It is the exclusive property of Guardhat, LLC and will not be 
 * disclosed in any form to any party without prior written permission of 
 * Guardhat, LLC. This document may not be reproduced or further used 
 * without the prior written permission of Guardhat, LLC.
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

// local includes -------------------------------------------------------------
#include "GPIO/Gpio.h"

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------
/// define the interrupt control structure
typedef struct _IRQMAP
{
  GPIOPINENUM   eIrqEnum;
} IRQMAP, *PIRQMAP;

// global parameter declarations ----------------------------------------------

// local parameter declarations -----------------------------------------------
//static  IRQMAP  atIrqMaps[ EIC_EXTINT_NUM ];

// local function prototypes --------------------------------------------------
static  PORT_Type*  GetPortBase( GPIOPORT ePort );
static  GPIO_Type*  GetGpioBase( GPIOPORT ePort );

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
  PGPIOPINDEF ptDef;
  U8          nIdx;
  PORT_Type*  ptPort;
  GPIO_Type*  ptGpio;

  // clear the IRQ maps
//  for ( nIdx = 0; nIdx < EIC_EXTINT_NUM; nIdx++ )
//  {
//    atIrqMaps[ nIdx ].eIrqEnum = 0xFF;
//  }
  
  // for each pin def in the list
  for( nIdx = 0; nIdx < GPIO_PIN_ENUM_MAX; nIdx++ )
  {
    // get a pointer to the definition structure
    ptDef = ( PGPIOPINDEF )&atGpioPinDefs[ nIdx ];
    ptPort = GetPortBase( ptDef->ePort );
    ptGpio = GetGpioBase( ptDef->ePort );
    
    // set up the port configuration register
    ptPort->PCR[ ptDef->nPin ] = PORT_PCR_MUX( ptDef->eFunc ) |
                                 BITS( ptDef->bHiDriveEnb, PORT_PCR_DSE_SHIFT ) |
                                 BITS( ptDef->bFilterEnb, PORT_PCR_PFE_SHIFT ) |
                                 BITS( ptDef->bPullEnable, PORT_PCR_PE_SHIFT ) |
                                 ptDef->bPullDir;
                                                    
    // now configure the GPIO direction reigster
    ptGpio->PDDR |= ( ptDef-> eDir ) ? GPIO_PDDR_PDD( ptDef->nPin ) : 0; 
    
    // now set the initial value
    if ( ptDef->bInitialState ^ ptDef->bInvert )
    {
      // turn on the pin
      ptGpio->PSOR = GPIO_PSOR_PTSO( ptDef->nPin );
    }
    else
    {
      // turn off the pin
      ptGpio->PCOR = GPIO_PCOR_PTCO( ptDef->nPin );
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
  GPIO_Type*  ptGpio;
  
  // test for valid GPIO select
  if ( eGpioSel < GPIO_PIN_ENUM_MAX )
  {
    // get a pointer to the definition
    ptDef = ( PGPIOPINDEF )&atGpioPinDefs[ eGpioSel ];

    // get the pointer to the GPIO Base
    ptGpio = GetGpioBase( ptDef->ePort );

    // check for an output pin
    if ( ptDef->eDir == GPIO_DIR_OUT )
    {
      // now set the initial value
      if ( ptDef->bInitialState ^ ptDef->bInvert )
      {
        // turn on the pin
        ptGpio->PSOR = GPIO_PSOR_PTSO( ptDef->nPin );
      }
      else
      {
        // turn off the pin
        ptGpio->PCOR = GPIO_PCOR_PTCO( ptDef->nPin );
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
  GPIO_Type*  ptGpio;
  
  // test for valid GPIO select
  if ( eGpioSel < GPIO_PIN_ENUM_MAX )
  {
    // get a pointer to the definition
    ptDef = ( PGPIOPINDEF )&atGpioPinDefs[ eGpioSel ];
    
    // get the pointer to the GPIO Base
    ptGpio = GetGpioBase( ptDef->ePort );

    // get the value
    *( pbState ) = ( BITS( ptGpio->PDIR, ptDef->nPin ) & 0x01 );
    if ( ptDef->bInvert )
    {
      // invert bit
      *( pbState ) ^= 1;
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
  GPIO_Type*  ptGpio;
  
  // test for valid GPIO select
  if ( eGpioSel < GPIO_PIN_ENUM_MAX )
  {
    // get a pointer to the definition
    ptDef = ( PGPIOPINDEF )&atGpioPinDefs[ eGpioSel ];
    
    // get the pointer to the GPIO Base
    ptGpio = GetGpioBase( ptDef->ePort );

    // toggle the port
    ptGpio->PTOR = GPIO_PTOR_PTTO( ptDef->nPin );
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
        
      case GPIO_ACT_SETMODE :
        break;
        
      case GPIO_ACT_ENBDSBIRQ :
        break;

      case GPIO_ACT_SETFUNCMUX :
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
 * @function GetPortBase
 *
 * @brief get the base address for the desired port
 *
 * This function return the base address for the desired port
 * 
 * @param[in]   ePort     desired port
 *
 * @return    base address for the port
 *
  *****************************************************************************/
static PORT_Type* GetPortBase( GPIOPORT ePort )
{
  PORT_Type*  aptPorts[ GPIO_PORT_MAX ] = PORT_BASES;

  // return the port type
  return( aptPorts[ ePort ] );
}

/******************************************************************************
 * @function GetGpioBase
 *
 * @brief get the base address for the desired GPIO
 *
 * This function return the base address for the desired GPIO
 * 
 * @param[in]   ePort     desired port
 *
 * @return    base address for the GPIO
 *
  *****************************************************************************/
static GPIO_Type* GetGpioBase( GPIOPORT ePort )
{
  GPIO_Type*  aptGpios[ GPIO_PORT_MAX ] = GPIO_BASES;

  // return the GPIO base
  return( aptGpios[ ePort ] );
}

/**@} EOF Gpio.c */
