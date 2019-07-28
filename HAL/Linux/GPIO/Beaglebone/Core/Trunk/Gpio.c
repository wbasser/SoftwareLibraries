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

// local includes -------------------------------------------------------------
#include "GPIO/Gpio.h"

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------
/// define the signature for GPIO enumerations
#define ENUM_SIGNATURE_VALUE                     ( 0xC33C )

/// define the macro for creating the port/pin enumeration
#define ENUM_PORT_PIN_GEN( port, pin, invert )   (( ENUM_SIGNATURE_VALUE << 16 ) |( port << 8 ) | ( pin << 1 ) | bInvert)

/// define the macros for extracting the port and pin
#define ENUM_TEST_SIG( enum )                   (( enum >> 16 ) == ENUM_SIGNATURE_VALUE )
#define ENUM_GET_PORT( enum )                   ( enum >> 8  & 0x0FF )
#define ENUM_GET_PIN( enum )                    (( enum >> 1 ) & 0x01F )
#define ENUM_GET_INVERT( enum )                 ( enum & 0x01 )

/// define the GpIO base registers
#define GPIO0_BASE_REG                          ( 0x44E07000 )
#define GPIO1_BASE_REG                          ( 0x4804C000 )
#define GPIO2_BASE_REG                          ( 0x481AC000 )
#define GPIO3_BASE_REG                          ( 0x481AE000 )

/// define the GPIO register length
#define GPIO_REG_LENGTH                         ( 0x1000 )

/// define the GPIO register offsets
#define GPIO_OUTENB_OFFSET                      ( 0x0134 )
#define GPIO_SETOUT_OFFSET                      ( 0x0194 )
#define GPIO_CLROUT_OFFSET                      ( 0x0190 )
#define GPIO_DATAIN_OFFSET                      ( 0x0138 )

/// define the maximum GPIO pin value
#define GPIO_PIN_MAX                            ( 32 )

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations ----------------------------------------------

// local parameter declarations -----------------------------------------------
static  PU32  puMappedGpioRegs[ GPIO_PORT_MAX ];
static  S32   iMemFileHandle;

// local function prototypes --------------------------------------------------

// constant declarations ------------------------------------------------------
static U32 const  auGpioRegionAddr[ GPIO_PORT_MAX ] =
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
  GPIOPORT      eRegIdx;
  
  // open a memory file
  iMemFileHandle = open( "/dev/mem", O_RDWR );
  
  // for each register
  for ( eRegIdx = GPIO_PORT_0; eRegIdx < GPIO_PORT_MAX; eRegIdx++ )
  {
    // map the register
    puMappedGpioRegs[ eRegIdx ] = mmap( NULL, GPIO_REG_LENGTH, PROT_READ | PROT_WRITE, MAP_SHARED, iMemFileHandle, auGpioRegionAddr );
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
 * @function Gpio_Configure
 *
 * @brief configure a GPIO pin
 *
 * This function will configue a GPIO pin
 *
 * @param[in]   ePort       port number
 * @param[in]   nPin        pin number
 * @param[in]   eMode       mode
 * @param[in]   bInvert     invert flag
 *
 * @return      GPIO enum   GPIO enumeration for reference
 *
 *****************************************************************************/
GPIOENUM Gpio_Configure( GPIOPORT ePort, U8 nPin, GPIOMODE eMode, BOOL bInvert, BOOL bIntial )
{
  GPIOENUM  uEnum = 0;
  BOOL      bState;
  
  // test for valid parameters
  if (( ePort < GPIO_PORT_MAX ) && ( nPin < GPIO_PIN_MAX ) && ( eMode < GPIO_MODE_MAX )) 
  (
    // set the direction
    switch( eMode )
    {
      case GPIO_MODE_INPUT :
        // disable the output enable
        *( puMappedGpioRegs[ ePort ] + GPIO_OUTENB_OFFSET ) &= ~( BIT( nPin ));
        break;
        
      case GPIO_MODE_OUTPUT :
        // enable the output enable
        *( puMappedGpioRegs[ ePort ] + GPIO_OUTENB_OFFSET ) |= BIT( nPin );
        
        // set the initial state
        if ( bInitial ^ bInvert )
        {
          // set the bit
'         *( puMappedGpioRegs[ ePort ] + GPIO_SETOUT_OFFSET ) = BIT( ptDef->nPin );
        }
        else
        {
          // clear the pin
          *( puMappedGpioRegs[ ePort ] + GPIO_CLROUT_OFFSET ) = BIT( ptDef->nPin );
        }
      }

      default :
        break;
    }

    // generate the enum
    uEnum = ENUM_PORT_PIN_GEN( ePort, nPin, bInvert );
  }
  // now return the GPIO enumration
  return( uEnum );
}

/******************************************************************************
 * @function Gpio_Set
 *
 * @brief set a GPIO pin 
 *
 * This function will set a GPIO pin to a given state
 *
 * @param[in]   eGpioEnum GPIO pin enumeration
 * @param[in]   bState    desired state of the pin
 *
 * @return      GPIOERR   appropriate error if any
 *
 *****************************************************************************/
GPIOERR Gpio_Set( GPIOENUM eGpioEnum, BOOL bState )
{
  U32     uMask, uPort;
  GPIOERR eError = GPIO_ERR_NONE;
  
  // test valid pin
  if ( ENUM_TEST_SIG( eGpioEnum ))
  {
    // compute the mask
    uMask = BIT( ENUM_GET_PIN( eGpioEnum ));
    uPort = ENUM_GET_PORT( eGpioEnum );

    // set the appropriate state
    if ( bState ^ ENUM_GET_INVERT( eGpioEnum ))
    {
      PORT->Group[ uPort ].OUTSET.reg = uMask;
    }
    else
    {
      PORT->Group[ uPort ].OUTCLR.reg = uMask;
    }
  }
  else
  {
    // set the error
    eError = GPIO_ERR_ILLENUM;
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
 * @param[in]   eGpioEnum   GPIO pin enumeration
 * @param[io]   pbState     pointer to store the pin state
 *
 * @return      GPIOERR   appropriate error if any
 *
 *****************************************************************************/
GPIOERR Gpio_Get( GPIOENUM eGpioEnum, PBOOL pbState )
{
  U32     uMask, uPort;
  GPIOERR eError = GPIO_ERR_NONE;
  
  // test valid pin
  if ( ENUM_TEST_SIG( eGpioEnum ))
  {
    // compute the mask
    uMask = BIT( ENUM_GET_PIN( eGpioEnum ));
    uPort = ENUM_GET_PORT( eGpioEnum );

    // get the value
    *( pbState ) = *( puMappedGpioRegs[ uPort ] + GPIO_DATAIN_OFFSET ) & uMask;
    *( pbState ) ^= ENUM_GET_INVERT( eGPioEnum );
  }
  else
  {
    // set the error
    eError = GPIO_ERR_ILLENUM;
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
 * @param[in]   eGpioEnum  GPIO pin enumeration
 *
 * @return      GPIOERR   appropriate error if any
 *
 *****************************************************************************/
GPIOERR Gpio_Toggle( GPIOENUM eGpioEnum )
{
  U32     uMask, uPort;
  GPIOERR eError = GPIO_ERR_NONE;
  
  // test valid pin
  if ( ENUM_TEST_SIG( eGpioEnum ))
  {
    // compute the mask
    uMask = BIT( ENUM_GET_PIN( eGpioEnum ));
    uPort = ENUM_GET_PORT( eGpioEnum );

    // check for an output
    if ( ENUM_GET_MODE( eGpioEnum ) == GPIO_MODE_OUTPUT )
    {
      // get the current state/toggle it/set the new output
      bCurState = *( puMappedGpioRegs[ uPort ] + GPIO_DATAIN_OFFSET ) & nMask;

      // determine the state
      if ( bCurState )
      {
        // clear the pin
        *( puMappedGpioRegs[ uPort ] + GPIO_CLROUT_OFFSET ) = nMask;
      }
      else
      {
        // set the pin
        *( puMappedGpioRegs[ uPort ] + GPIO_SETOUT_OFFSET ) = BIT( ptDef->nPin );
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
    eError = GPIO_ERR_ILLENUM;
  }

   // return the error
   return( eError );
}

/**@} EOF Gpio.c */
