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

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------
/// define the interrupt control structure
typedef struct _IRQMAP
{
  GPIOIRQENUM   eIrqEnum;
} IRQMAP, *PIRQMAP;

// global parameter declarations ----------------------------------------------

// local parameter declarations -----------------------------------------------
static  IRQMAP  atIrqMaps[ EIC_EXTINT_NUM ];

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
  PGPIOPINDEF       ptDef;
  PGPIOFNCDEF       ptFnc;
  PGPIOIRQDEF       ptIrq;
  U8                nIdx, nRegOfs, nPinOfs;
  BOOL              bTemp;
  PortGroup*        ptPort;
  PORT_PINCFG_Type  tPinCfg;
  PORT_PMUX_Type    tPinMux;
  EIC_CONFIG_Type   tEicCfg;
  U32               uMask;
  
  // clear the IRQ maps
  for ( nIdx = 0; nIdx < EIC_EXTINT_NUM; nIdx++ )
  {
    atIrqMaps[ nIdx ].eIrqEnum = 0xFF;
  }
  
  // for each pin def in the list
  for( nIdx = 0; nIdx < GPIO_PIN_ENUM_MAX; nIdx++ )
  {
    // get a pointer to the definition structure
    ptDef = ( PGPIOPINDEF )&g_atGpioPinDefs[ nIdx ];
    
    // get the port/pin mask
    ptPort = &PORT->Group[ ptDef->ePort ];
    uMask = BIT( ptDef->nPin );
    
    // set the config
    tPinCfg.reg = 0;
    tPinCfg.bit.PMUXEN = FALSE;
    bTemp = (( ptDef->eMode == GPIO_MODE_INPDSB_PULLDN ) || ( ptDef->eMode == GPIO_MODE_INPDSB_PULLUP ) || ( ptDef->eMode == GPIO_MODE_OUTPUT_INPDSB )) ? FALSE : TRUE;
    tPinCfg.bit.INEN = bTemp;
    bTemp = (( ptDef->eMode == GPIO_MODE_INPUT_PULLDN ) || ( ptDef->eMode == GPIO_MODE_INPUT_PULLUP )) ? TRUE : FALSE;
    tPinCfg.bit.PULLEN = bTemp;
    tPinCfg.bit.DRVSTR = ptDef->bHiDriveEnb;
    ptPort->PINCFG[ ptDef->nPin ] = tPinCfg;
    
    // set the direction
    switch( ptDef->eMode )
    {
      case GPIO_MODE_INPDSB_PULLDN :
      case GPIO_MODE_INPDSB_PULLUP :
      case GPIO_MODE_INPUT :
      case GPIO_MODE_INPUT_PULLDN :
      case GPIO_MODE_INPUT_PULLUP :
        // clear the direction bit
        ptPort->DIRCLR.reg = uMask;
        ptPort->OUTSET.reg = ( ptDef->eMode == GPIO_MODE_INPUT_PULLUP ) ? uMask : 0;
        break;
        
      case GPIO_MODE_OUTPUT_INPDSB :
      case GPIO_MODE_OUTPUT_INPENB :
        // set the direction bit
        ptPort->DIRSET.reg = uMask;
        break;
        
      default :
        break;
    }
    
    // now check for initial on
    if ( ptDef->bInitialState ^ ptDef->bInvert )
    {
      // set it  on
      ptPort->OUTSET.reg = uMask;
    }
    else
    {
      // clear it
      ptPort->OUTCLR.reg = uMask;
    }
  }

  // set up the function pins
  for ( nIdx = 0; nIdx < GPIO_FNC_ENUM_MAX; nIdx++ )
  {
    // get a pointer to the definition structure
    ptFnc = ( PGPIOFNCDEF )&g_atGpioFncDefs[ nIdx ];
    
    // get the port/mask
    ptPort = &PORT->Group[ ptFnc->ePort ];
    uMask = BIT( ptFnc->nPin );
    
    // set the cmux enable
    tPinCfg.reg = PORT_PINCFG_PMUXEN;
    tPinCfg.reg |= ( ptFnc->bAllowInput ) ? PORT_PINCFG_INEN : 0;
    tPinCfg.reg |= ( ptFnc->bPullUpEnable ) ? PORT_PINCFG_PULLEN : 0;
    ptPort->PINCFG[ ptFnc->nPin ] = tPinCfg;
    
    // get the current value
    tPinMux = ptPort->PMUX[ ptFnc->nPin >> 1 ];
    
    // now determine which nibble we are processing
    if ( ptFnc->nPin & 0x01 )
    {
      // set the mux
      tPinMux.bit.PMUXO = ptFnc->eFunc;
    }
    else
    {
      // set the mux
      tPinMux.bit.PMUXE = ptFnc->eFunc;
    }
    
    // write it back out
    ptPort->PMUX[ ptFnc->nPin >> 1] = tPinMux;
  }
  
  // for each interrupt
  bTemp = FALSE;
  for ( nIdx = 0; nIdx < GPIO_IRQ_ENUM_MAX; nIdx++ )
  {
    // get a pointer to the definition structure
    ptIrq = ( PGPIOIRQDEF )&g_atGpioIrqDefs[ nIdx ];
    bTemp = TRUE;
    
    // get the port
    ptPort = &PORT->Group[ ptIrq->ePort ];
    uMask = BIT( ptIrq->nPin );

    // set the cmux enable
    tPinCfg.reg = 0;
    tPinCfg.bit.PMUXEN = TRUE;
    tPinCfg.bit.INEN = TRUE;

    // determine the pull up down
    switch( ptIrq->ePupdn )
    {
      case GPIO_IRQPUPDN_PULLDN :
        // set the pull up enable in the config
        tPinCfg.bit.PULLEN = TRUE;
        break;

      case GPIO_IRQPUPDN_PULLUP :
        // set the pull up enable in the config
        tPinCfg.bit.PULLEN = TRUE;
        ptPort->OUTSET.reg = uMask;
        break;

      default :
        break;
    }

    // write the pin config
    ptPort->PINCFG[ ptIrq->nPin ] = tPinCfg;
    
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
    
    // get the register offset
    nRegOfs = ( ptIrq->nPin & 0x0F ) / 8;
    nPinOfs = ptIrq->nPin % 8;
     
    // get the new value
    tEicCfg = EIC->CONFIG[ nRegOfs ];
    
    // set the sense and filter
    tEicCfg.reg |= ( BITS( ptIrq->eSense, ( EIC_CONFIG_SENSE1_Pos * nPinOfs )));
    tEicCfg.reg |= ( BITS( ptIrq->bFilterEnb, ( EIC_CONFIG_FILTEN0_Pos + ( nPinOfs * EIC_CONFIG_SENSE1_Pos ))));
    
    // write it back
    EIC->CONFIG[ nRegOfs ] = tEicCfg;
    
    // now set the wakeup
    nPinOfs = ptIrq->nPin % EIC_EXTINT_NUM;
    EIC->WAKEUP.reg |= ( ptIrq->bWakeupEnb ) ? BIT( nPinOfs ) : 0;
    
    // now set the enable
    if ( ptIrq->bEventType )
    {
      // only set event flag if event type
      EIC->EVCTRL.reg |= BIT( nPinOfs );
    }
    
    // if initial on
    if ( ptIrq->bInitOn )
    {
      // enable the interrupts
      EIC->INTENSET.reg |= BIT( nPinOfs );
    }

    // set the map
    atIrqMaps[ nPinOfs ].eIrqEnum = nIdx;
  }
  
  // enable the interrupt controller if there are events
  if ( bTemp )
  {
    // register the interrupt
    NVIC_EnableIRQ( EIC_IRQn );
    
    // enable the controller
    EIC->CTRL.reg = EIC_CTRL_ENABLE;
    while( EIC->STATUS.bit.SYNCBUSY == TRUE );
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
  // disable all external interrupts
  EIC->CTRL.reg = 0;
  while( EIC->STATUS.bit.SYNCBUSY == TRUE );

  // register the interrupt
  NVIC_DisableIRQ( EIC_IRQn );
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
  
  // test for valid GPIO select
  if ( eGpioSel < GPIO_PIN_ENUM_MAX )
  {
    // get a pointer to the definition
    ptDef = ( PGPIOPINDEF )&g_atGpioPinDefs[ eGpioSel ];

    // check for an output pin
    if (( ptDef->eMode != GPIO_MODE_OUTPUT_INPDSB ) || ( ptDef->eMode != GPIO_MODE_OUTPUT_INPENB ))
    {
      // compute the mask
      uMask = BIT( ptDef->nPin );
    
      // set the appropriate state
      if ( bState ^ ptDef->bInvert )
      {
        PORT->Group[ ptDef->ePort ].OUTSET.reg = uMask;
      }
      else
      {
        PORT->Group[ ptDef->ePort ].OUTCLR.reg = uMask;
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
  U32         uMask, uPort;
  
  // test for valid GPIO select
  if ( eGpioSel < GPIO_PIN_ENUM_MAX )
  {
    // get a pointer to the definition
    ptDef = ( PGPIOPINDEF )&g_atGpioPinDefs[ eGpioSel ];
    
    // compute the mask
    uMask = BIT( ptDef->nPin );
    
    // get the value
    uPort = PORT->Group[ ptDef->ePort ].IN.reg;
    uPort &= uMask;
    if ( ptDef->bInvert )
    {
      // invert bit
      uPort ^= uMask;
    }
    *pbState = ( uPort ) ? TRUE : FALSE;
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
 * @function Gpio_GetIrq
 *
 * @brief get a GPIO IRQ pin's value
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
GPIOERR Gpio_GetIrq( GPIOIRQENUM eGpioSel, PBOOL pbState )
{
  GPIOERR     eError = GPIO_ERR_NONE;
  PGPIOIRQDEF ptDef;
  U32         uMask, uPort;
  
  // test for valid GPIO select
  if ( eGpioSel < GPIO_IRQ_ENUM_MAX )
  {
    // get a pointer to the definition
    ptDef = ( PGPIOIRQDEF )&g_atGpioIrqDefs[ eGpioSel ];
    
    // compute the mask
    uMask = BIT( ptDef->nPin );
    
    // get the value
    uPort = PORT->Group[ ptDef->ePort ].IN.reg;
    uPort &= uMask;
    *pbState = ( uPort ) ? TRUE : FALSE;
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
  U32         uMask;
  
  // test for valid GPIO select
  if ( eGpioSel < GPIO_PIN_ENUM_MAX )
  {
    // get a pointer to the definition
    ptDef = ( PGPIOPINDEF )&g_atGpioPinDefs[ eGpioSel ];
    
    // compute the mask
    uMask = BIT( ptDef->nPin );
    
    // toggle the port
    PORT->Group[ ptDef->ePort ].OUTTGL.reg = uMask;
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
  GPIOERR           eError = GPIO_ERR_NONE;
  PGPIOFNCDEF       ptFnc;
  PGPIOPINDEF       ptPin;
  PGPIOIRQDEF       ptIrq;
  PortGroup*        ptPort;
  PORT_PINCFG_Type  tPinCfg;
  PORT_PMUX_Type    tPinMux;
  GPIOFUNCMUX       eFunction;
  GPIODIR           eDir;
  U32               uMask;
  BOOL              bAction;
  U8                nPinOfs;
  
  // test for valid GPIO select
  if ( eGpioSel < GPIO_PIN_ENUM_MAX )
  {
    // perform the action
    switch( eGpioAct )
    {
      case GPIO_ACT_SETDIR :
        // get the new dirction
        eDir = ( GPIODIR )PARAMU8( pvData );
      
        // get a pointer to the definition structure
        ptPin = ( PGPIOPINDEF )&g_atGpioPinDefs[ eGpioSel ];

        // get the port/mask
        ptPort = &PORT->Group[ ptPin->ePort ];
        uMask = BIT( ptPin->nPin );
        
        // set/clear based on direction
        if ( eDir == GPIO_DIR_IN )
        {
          // clear the direction
          ptPort->DIRCLR.reg = uMask;   
        }
        else
        {
          // set the direction
          ptPort->DIRSET.reg = uMask;
        }

        break;
        
      case GPIO_ACT_SETMODE :
        break;
        
      case GPIO_ACT_ENBDSBIRQ :
        // get the state
        bAction = ( BOOL )PARAMBOOL( pvData );

        // get the pin definition
        ptIrq = ( PGPIOIRQDEF )&g_atGpioIrqDefs[ eGpioSel ];

        // get the register offset
        nPinOfs = ptIrq->nPin % EIC_EXTINT_NUM;

        // test fpr actopm
        if ( bAction )
        {
          // enable it
          EIC->INTENSET.reg |= BIT( nPinOfs );
        }
        else
        {
          // disable it
          EIC->INTENCLR.reg |= BIT( nPinOfs );
        }
        break;

      case GPIO_ACT_SETFUNCMUX :
        // get the function
        eFunction = ( GPIOFUNCMUX )PARAMU8( pvData );

        // get a pointer to the definition structure
        ptFnc = ( PGPIOFNCDEF )&g_atGpioFncDefs[ eGpioSel ];

        // get the port/mask
        ptPort = &PORT->Group[ ptFnc->ePort ];
   
        // set the cmux enable
        tPinCfg.reg = PORT_PINCFG_PMUXEN;
        tPinCfg.reg |= ( ptFnc->bAllowInput ) ? PORT_PINCFG_INEN : 0;
        tPinCfg.reg |= PORT_PINCFG_DRVSTR;
        ptPort->PINCFG[ ptFnc->nPin ] = tPinCfg;
    
        // get the current value
        tPinMux = ptPort->PMUX[ ptFnc->nPin >> 1 ];
    
        // now determine which nibble we are processing
        if ( ptFnc->nPin & 0x01 )
        {
          // set the mux
          tPinMux.bit.PMUXO = eFunction;
        }
        else
        {
          // set the mux
          tPinMux.bit.PMUXE = eFunction;
        }
    
        // write it back out
        ptPort->PMUX[ ptFnc->nPin >> 1] = tPinMux;
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
 * @function IrqCommandHandler
 *
 * @brief common IRQ handler
 *
 * This function processes pin interrupts
 *
 *****************************************************************************/
void EIC_Handler( void )
{
  U32         uStatus, uMask, uPort, uBitMask;
  U8          nIdx;
  PGPIOIRQDEF ptIrq;
  BOOL        bState;
  
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
      
      // get the map
      ptIrq = ( PGPIOIRQDEF )&g_atGpioIrqDefs[ atIrqMaps[ nIdx ].eIrqEnum ];
      
      // if callback is not null
      if ( ptIrq->pvCallback != NULL )
      {
        // compute the mask
        uBitMask = BIT( ptIrq->nPin );
    
        // get the value
        uPort = PORT->Group[ ptIrq->ePort ].IN.reg;
        uPort &= uBitMask;
        bState = ( uPort ) ? TRUE : FALSE;
        
        // process the callback
        ptIrq->pvCallback( nIdx, ptIrq->nEvent, bState );
      }

      // clear the status bit
      uStatus &= ~uMask;
    }

    // increment the bit
    nIdx++;
  }
}

/**@} EOF Gpio.c */
