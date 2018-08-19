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

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations ----------------------------------------------

// local parameter declarations -----------------------------------------------

// local function prototypes --------------------------------------------------

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
 * @param[in]   bHiDriveEnb hi drive enable
 * @param[in]   eMux        mux
 * @param[in]   bInvert     invert flag
 *
 * @return      GPIO enum   GPIO enumeration for reference
 *
 *****************************************************************************/
U32 Gpio_Configure( GPIOPORT ePort, U8 nPin, GPIOMODE eMode, BOOL bHiDriveEnb, GPIOFUNCMUX eFunc, BOOL bInvert )
{
  U8                nIdx, nRegOfs, nPinOfs;
  PortGroup*        ptPort;
  PORT_PINCFG_Type  tPinCfg;
  PORT_PMUX_Type    tPinMux;
  U32               uMask;
  
  // get the port/pin mask
  ptPort = &PORT->Group[ ePort ];
  uMask = BIT( nPin );
  
  // set the config
  tPinCfg.reg = 0;
  tPinCfg.bit.PMUXEN = ( eFunc != GPIO_FUNCMUX_A ) ? TRUE : FALSE;
  tPinCfg.bit.INEN = (( eMode == GPIO_MODE_INPDSB_PULLDN ) || ( eMode == GPIO_MODE_INPDSB_PULLUP ) || ( eMode == GPIO_MODE_OUTPUT_INPDSB )) ? FALSE : TRUE;
  tPinCfg.bit.DRVSTR = bHiDriveEnb;
  ptPort->PINCFG[ nPin ] = tPinCfg;
  
  // set the direction
  switch( eMode )
  {
    case GPIO_MODE_INPDSB_PULLDN :
    case GPIO_MODE_INPDSB_PULLUP :
    case GPIO_MODE_INPUT :
    case GPIO_MODE_INPUT_PULLDN :
    case GPIO_MODE_INPUT_PULLUP :
      // clear the direction bit
      ptPort->DIRCLR.reg = uMask;
      break;
      
    case GPIO_MODE_OUTPUT_INPDSB :
    case GPIO_MODE_OUTPUT_INPENB :
      // set the direction bit
      ptPort->DIRSET.reg = uMask;
      break;
      
    default :
      break;
  }

  // set the current mux
  tPinMux = ptPort->PMUX[ nPin >> 1 ];
  
  // now determine which nibble we are processing
  if ( nPin & 0x01 )
  {
    // set the mux
    tPinMux.bit.PMUXO = eFunc;
  }
  else
  {
    // set the mux
    tPinMux.bit.PMUXE = eFunc;
  }
    
  // write it back out
  ptPort->PMUX[ nPin >> 1] = tPinMux;

  // now return the GPIO enumration
  return( ENUM_PORT_PIN_GEN( ePort, nPin, bInvert ));
}

/******************************************************************************
 * @function Gpio_Set
 *
 * @brief set a GPIO pin 
 *
 * This function will set a GPIO pin to a given state
 *
 * @param[in]   uGpioSel  GPIO pin enumeration
 * @param[in]   bState    desired state of the pin
 *
 * @return      GPIOERR   appropriate error if any
 *
 *****************************************************************************/
GPIOERR Gpio_Set( GPIOENUM uGpioSel, BOOL bState )
{
  U32     uMask, uPort;
  GPIOERR eError = GPIO_ERR_NONE;
  
  // test valid pin
  if ( ENUM_TEST_SIG( uGpioSel ))
  {
    // compute the mask
    uMask = BIT( ENUM_GET_PIN( uGpioSel ));
    uPort = ENUM_GET_PORT( uGpioSel );

    // set the appropriate state
    if ( bState ^ ENUM_GET_INVERT( uGpioSel ))
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
 * @param[in]   uGpioSel    GPIO pin enumeration
 * @param[io]   pbState     pointer to store the pin state
 *
 * @return      GPIOERR   appropriate error if any
 *
 *****************************************************************************/
GPIOERR Gpio_Get( GPIOENUM uGpioSel, PBOOL pbState )
{
  U32     uMask, uPort, uTemp;
  GPIOERR eError = GPIO_ERR_NONE;
  
  // test valid pin
  if ( ENUM_TEST_SIG( uGpioSel ))
  {
    // compute the mask
    uMask = BIT( ENUM_GET_PIN( uGpioSel ));
    uPort = ENUM_GET_PORT( uGpioSel );

    // get the value
    uTemp = PORT->Group[ uPort ].IN.reg;
    uTemp &= uMask;
    if ( ENUM_GET_INVERT( uGpioSel ))
    {
      // invert bit
      uTemp ^= uMask;
    }

    // store the pin state
    *( pbState ) = ( uTemp ) ? TRUE : FALSE;
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
 * @param[in]   uGpioSel  GPIO pin enumeration
 *
 * @return      GPIOERR   appropriate error if any
 *
 *****************************************************************************/
GPIOERR Gpio_Toggle( GPIOENUM uGpioSel )
{
  U32     uMask, uPort;
  GPIOERR eError = GPIO_ERR_NONE;
  
  // test valid pin
  if ( ENUM_TEST_SIG( uGpioSel ))
  {
    // compute the mask
    uMask = BIT( ENUM_GET_PIN( uGpioSel ));
    uPort = ENUM_GET_PORT( uGpioSel );

    // toggle pin
    PORT->Group[ uPort ].OUTTGL.reg = uMask;
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
