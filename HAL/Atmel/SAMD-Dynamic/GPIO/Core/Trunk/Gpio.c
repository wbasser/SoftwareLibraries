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
/// define the signature for GPIO pin enumerations
#define ENUM_PIN_SIGNATURE_VALUE                     ( 0xC33C )

/// define the signature for GPIO IRQ enumerations
#define ENUM_IRQ_SIGNATURE_VALUE                     ( 0xC3C3 )

/// define the macro for creating the port/pin enumeration
#define ENUM_PIN_GEN( port, pin, invert )   (( ENUM_PIN_SIGNATURE_VALUE << 16 ) |( port << 8 ) | ( pin << 1 ) | bInvert)

/// define the macro for creating the IRQ enumeration
#define ENUM_IRQ_GEN( reg, irq )   					(( ENUM_IRQ_SIGNATURE_VALUE << 16 ) |( reg << 8 ) | irq)

/// define the macros for extracting the port and pin
#define ENUM_TEST_PIN_SIG( enum )               (( enum >> 16 ) == ENUM_PIN_SIGNATURE_VALUE )
#define ENUM_TEST_IRQ_SIG( enum )               (( enum >> 16 ) == ENUM_IRQ_SIGNATURE_VALUE )
#define ENUM_GET_PORT( enum )                   (( enum >> 8 ) & 0x0FF )
#define ENUM_GET_PIN( enum )                    (( enum >> 1 ) & 0x01F )
#define ENUM_GET_INVERT( enum )                 ( enum & 0x01 )
#define	ENUM_GET_REG( enum )										(( enum >> 8 & 0xFF )
#define	ENUM_GET_IRQ( enum )										( enum & 0xFF )

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations ----------------------------------------------

// local parameter declarations -----------------------------------------------
static		PVGPIOIRQCALLBACK	apvCallbacks[ EIC_EXTINT_NUM ];

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
  return( ENUM_PIN_GEN( ePort, nPin, bInvert ));
}

/******************************************************************************
 * @function Gpio_Configure
 *
 * @brief configure a GPIO pin
 *
 * This function will configue a GPIO pin
 *
 * @param[in]   ePort       	port number
 * @param[in]   nPin        	pin number
 * @param[in]		eSense				interrupt sense
 * @param[in]   bPullupEnable	enable the pullup
 * @param[in]   bFilterEnable	enable the filter mode
 * @param[in]		bWakeup				wakeup enable
 * @param[in]		bInitOn				intially on
 * @param[in]   pvCallback		IRQ callback function
 *
 * @return      GPIO enum   GPIO enumeration for reference
 *
 *****************************************************************************/
GPIOENUM Gpio_ConfigureIRQ( GPIOPORT ePort, U8 nPin, GPIOIRQSENSE eSense, BOOL bPullupEnable, BOOL bFilterEnable, BOOL bWakeup, BOOL bInitOn, PVGPIOIRQCALLBACK pvCallback )
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
  tPinCfg.bit.PMUXEN = TRUE;
  tPinCfg.bit.INEN = TRUE;
  ptPort->PINCFG[ nPin ] = tPinCfg;
  
	// get the current value
	tPinMux = ptPort->PMUX[ ptIrq->nPin >> 1 ];
	
	// now determine which nibble we are processing
	if ( ptIrq->nPin & 0x01 )
	{
		// set the mux
		tPinMux.bit.PMUXO = 0;
	}
	else
	{
		// set the mux
		tPinMux.bit.PMUXE = 0;
	}
	
	// write it back out
	ptPort->PMUX[ ptIrq->nPin >> 1] = tPinMux;
	
	// set the direction bit
	if ( bPullupEnble )
	{
		// set the pull up
		ptPort->DIRSET.reg = uMask;
	}
      
	// test for the odd Port A
	if (( ptIrq->ePort == GPIO_PORT_A ) && ( ptIrq->nPin >= 24 ))
	{
		// adjust for pins 24-31
		if (( ptIrq->nPin >= 24 ) && ( ptIrq->nPin <=27 ))
		{
		// set the pin offset
			nPinOfs = (( ptIrq->nPin + 4 ) % 8 );
		}
		else
		{
		// set the pin offset
			nPinOfs = (( ptIrq->nPin - 4 ) % 8 );
		}
	}
	else
	{
		// set the pin offset
		nPinOfs = ptIrq->nPin % 8;
	}

	// get the register offset
	nRegOfs = ( ptIrq->nPin & 0x0F ) / 8;
	nPinOfs = ptIrq->nPin % 8;
	 
	// get the new value
	tEicCfg = EIC->CONFIG[ nRegOfs ];
	
	// set the sense and filter
	tEicCfg.reg |= ( BITS( bSense, ( EIC_CONFIG_SENSE1_Pos * nPinOfs )));
	tEicCfg.reg |= ( BITS( bFilterEnb, ( EIC_CONFIG_FILTEN0_Pos + ( nPinOfs * EIC_CONFIG_SENSE1_Pos ))));
	
	// write it back
	EIC->CONFIG[ nRegOfs ] = tEicCfg;
	
	// now set the wakeup
	nPinOfs = ptIrq->nPin % EIC_EXTINT_NUM;
	EIC->WAKEUP.reg |= ( bWakeupEnb ) ? BIT( nPinOfs ) : 0;
	
	// save the callback
	apvCallbacks[ nPinOfs ] = pvCallback;
	
		// if initial on
	if ( bInitOn )
	{
		// enable the interrupts
		EIC->INTENSET.reg |= BIT( nPinOfs );
	}

  // now return the GPIO enumration
  return( ENUM_IRQ_GEN( nRegOfx, nPinOfs ));
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
  if ( ENUM_TEST_PIN_SIG( uGpioSel ))
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
  if ( ENUM_TEST_PIN_SIG( uGpioSel ))
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
  if ( ENUM_TEST_PIN_SIG( uGpioSel ))
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
GPIOERR Gpio_IrqControl( GPIOENUM uGpioSel, BOOL bState )
{
  GPIOERR eError = GPIO_ERR_NONE;
	U8			nPinOfs, nRegOfs;

  // test valid pin
  if ( ENUM_TEST_IRQ_SIG( uGpioSel ))
  {
		//  get the register/pinoffset
		nRegOfs = ENUM_GET_REG( uGpioSel );
		nPinOfs = ENUM_GET_IRQ( uGpioSel );
		
		if ( bState )
		{
			// enable it
			EIC->INTENSET.reg |= BIT( nPinOfs );
		}
		else
		{
			// disable it
			EIC->INTENCLR.reg |= BIT( nPinOfs );
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
 * @function IrqCommandHandler
 *
 * @brief common IRQ handler
 *
 * This function processes pin interrupts
 *
 *****************************************************************************/
void EIC_Handler( void )
{
  U32         			uStatus, uMask;
  U8          			nIdx;
  PVGPIOIRQCALLBACK	pvCallback;
	
  // get the current IRQ status
  uStatus = EIC->INTFLAG.reg;
  
  // determine which interrupt
  nIdx = 0;
  while( uStatus != 0 )
  {
    // create the mask
    uMask = BIT( nIdx );

    // is this interrupt set
    if ( uStatus & uMask )
    {
      // clear the interrupt9
      EIC->INTFLAG.reg = uMask;
      
      // if callback is not null
      if (( pvCallback = apvCallbacks[ nIdx ] ) != NULL )
      {
        // process the callback
        pvCallback( nIdx );
      }

      // clear the status bit
      uStatus &= ~uMask;
    }

    // increment the bit
    nIdx++;
  }
}

/**@} EOF Gpio.c */
