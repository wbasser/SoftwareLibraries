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
#define EIC_EXTINT_MAX                ( 16 )

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
static  void  ExtIrqCommonHandler( GPIOIRQENUM eIrqEnum );

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
        ptPort->OUTSET.reg = ( ptDef->eMode == GPIO_MODE_INPUT_PULLDN ) ? uMask : 0;
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
  
  if ( GPIO_IRQ_ENUM_MAX != 0 )
  {
    // enable the controller
    EIC->CTRLA.reg = EIC_CTRLA_ENABLE;
    while( EIC->SYNCBUSY.bit.ENABLE == TRUE );

    // for each interrupt
    for ( nIdx = 0; nIdx < GPIO_IRQ_ENUM_MAX; nIdx++ )
    {
      // get a pointer to the definition structure
      ptIrq = ( PGPIOIRQDEF )&g_atGpioIrqDefs[ nIdx ];
    
      // get the port
      ptPort = &PORT->Group[ ptIrq->ePort ];
    
      // set the cmux enable
      tPinCfg.reg = 0;
      tPinCfg.bit.PMUXEN = TRUE;
      tPinCfg.bit.INEN = TRUE;
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
    
      // write it back out
      ptPort->PMUX[ ptIrq->nPin >> 1] = tPinMux;
    
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
      tEicCfg.reg |= ( BITS( ptIrq->eSense, ( EIC_CONFIG_SENSE1_Pos * nPinOfs )));
      tEicCfg.reg |= ( BITS( ptIrq->bFilterEnb, ( EIC_CONFIG_FILTEN0_Pos + ( nPinOfs * EIC_CONFIG_SENSE1_Pos ))));
    
      // write it back
      EIC->CONFIG[ nRegOfs ] = tEicCfg;
    
      // now set the async
      nPinOfs = ptIrq->nPin % EIC_EXTINT_NUM;
      EIC->ASYNCH.reg |= ( ptIrq->bAsyncEnb ) ? BIT( nPinOfs ) : 0;
    
      // now set the enable
      EIC->EVCTRL.reg |= ( ptIrq->bEventType ) ? BIT( nPinOfs ) : 0;
    
      // enable the interrupts
      EIC->INTENSET.reg |= ( ptIrq->bInitOn ) ? BIT( nPinOfs ) : 0;

      // set the map
      atIrqMaps[ nPinOfs ].eIrqEnum = nIdx;

      // enable the IRQ
      NVIC_EnableIRQ( EIC_0_IRQn + nPinOfs );
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
  //// disable all external interrupts
  //EIC->CTRL.reg = 0;
  //while( EIC->STATUS.bit.SYNCBUSY == TRUE );
//
  //// register the interrupt
  //NVIC_DisableIRQ( EIC_IRQn );
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
        bAction = ( BOOL )PARAMU8( pvData );

        // get the pin definition
        ptIrq = ( PGPIOIRQDEF )&g_atGpioIrqDefs[ eGpioSel ];

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
 * @function EIC_0_Handler
 *
 * @brief EIC #0 handler
 *
 * This function processes pin interrupts
 *
 *****************************************************************************/
void EIC_0_Handler( void )
{
  // call the common handler
  ExtIrqCommonHandler( atIrqMaps[ 0 ].eIrqEnum );
}

/******************************************************************************
 * @function EIC_1_Handler
 *
 * @brief EIC #1 handler
 *
 * This function processes pin interrupts
 *
 *****************************************************************************/
void EIC_1_Handler( void )
{
  // call the common handler
  ExtIrqCommonHandler( atIrqMaps[ 1 ].eIrqEnum );
}

/******************************************************************************
 * @function EIC_2_Handler
 *
 * @brief EIC #2 handler
 *
 * This function processes pin interrupts
 *
 *****************************************************************************/
void EIC_2_Handler( void )
{
  // call the common handler
  ExtIrqCommonHandler( atIrqMaps[ 2 ].eIrqEnum );
}

/******************************************************************************
 * @function EIC_3_Handler
 *
 * @brief EIC #3 handler
 *
 * This function processes pin interrupts
 *
 *****************************************************************************/
void EIC_3_Handler( void )
{
  // call the common handler
  ExtIrqCommonHandler( atIrqMaps[ 3 ].eIrqEnum ); 
}

/******************************************************************************
 * @function EIC_4_Handler
 *
 * @brief EIC #4 handler
 *
 * This function processes pin interrupts
 *
 *****************************************************************************/
void EIC_4_Handler( void )
{
  // call the common handler
  ExtIrqCommonHandler( atIrqMaps[ 4 ].eIrqEnum );
}

/******************************************************************************
 * @function EIC_5_Handler
 *
 * @brief EIC #5 handler
 *
 * This function processes pin interrupts
 *
 *****************************************************************************/
void EIC_5_Handler( void )
{
  // call the common handler
  ExtIrqCommonHandler( atIrqMaps[ 5 ].eIrqEnum );
}

/******************************************************************************
 * @function EIC_6_Handler
 *
 * @brief EIC #6 handler
 *
 * This function processes pin interrupts
 *
 *****************************************************************************/
void EIC_6_Handler( void )
{
  // call the common handler
  ExtIrqCommonHandler( atIrqMaps[ 6 ].eIrqEnum );
}

/******************************************************************************
 * @function EIC_7_Handler
 *
 * @brief EIC #7 handler
 *
 * This function processes pin interrupts
 *
 *****************************************************************************/
void EIC_7_Handler( void )
{
  // call the common handler
  ExtIrqCommonHandler( atIrqMaps[ 7 ].eIrqEnum );
}

/******************************************************************************
 * @function EIC_8_Handler
 *
 * @brief EIC #8 handler
 *
 * This function processes pin interrupts
 *
 *****************************************************************************/
void EIC_8_Handler( void )
{
  // call the common handler
  ExtIrqCommonHandler( atIrqMaps[ 8 ].eIrqEnum );
}

/******************************************************************************
 * @function EIC_9_Handler
 *
 * @brief EIC #9 handler
 *
 * This function processes pin interrupts
 *
 *****************************************************************************/
void EIC_9_Handler( void )
{
  // call the common handler
  ExtIrqCommonHandler( atIrqMaps[ 9 ].eIrqEnum );
}

/******************************************************************************
 * @function EIC_10_Handler
 *
 * @brief EIC #10 handler
 *
 * This function processes pin interrupts
 *
 *****************************************************************************/
void EIC_10_Handler( void )
{
  // call the common handler
  ExtIrqCommonHandler( atIrqMaps[ 10 ].eIrqEnum );
}

/******************************************************************************
 * @function EIC_11_Handler
 *
 * @brief EIC #11 handler
 *
 * This function processes pin interrupts
 *
 *****************************************************************************/
void EIC_11_Handler( void )
{
  // call the common handler
  ExtIrqCommonHandler( atIrqMaps[ 11 ].eIrqEnum );
}

/******************************************************************************
 * @function EIC_12_Handler
 *
 * @brief EIC #12 handler
 *
 * This function processes pin interrupts
 *
 *****************************************************************************/
void EIC_12_Handler( void )
{
  // call the common handler
  ExtIrqCommonHandler( atIrqMaps[ 12 ].eIrqEnum );
}

/******************************************************************************
 * @function EIC_13_Handler
 *
 * @brief EIC #13 handler
 *
 * This function processes pin interrupts
 *
 *****************************************************************************/
void EIC_13_Handler( void )
{
  // call the common handler
  ExtIrqCommonHandler( atIrqMaps[ 13 ].eIrqEnum );
}

/******************************************************************************
 * @function EIC_14_Handler
 *
 * @brief EIC #14 handler
 *
 * This function processes pin interrupts
 *
 *****************************************************************************/
void EIC_14_Handler( void )
{
  // call the common handler
  ExtIrqCommonHandler( atIrqMaps[ 14 ].eIrqEnum );
}

/******************************************************************************
 * @function EIC_15_Handler
 *
 * @brief EIC #15 handler
 *
 * This function processes pin interrupts
 *
 *****************************************************************************/
void EIC_15_Handler( void )
{
  // call the common handler
  ExtIrqCommonHandler( atIrqMaps[ 15 ].eIrqEnum );
}

/******************************************************************************
 * @function ExtIrqCommonHandler
 *
 * @brief external interrupty common handler
 *
 * This function processes pin interrupts
 *
 * @param[in]   eIrqEnum    interrupt number
 *
 *****************************************************************************/
static void ExtIrqCommonHandler( GPIOIRQENUM eIrqEnum )
{
  PGPIOIRQDEF       ptIrq;
  PVGPIOIRQCALLBACK pvCallback;
  BOOL              bPinState;

  // get a pointer to the definition structure
  ptIrq = ( PGPIOIRQDEF )&g_atGpioIrqDefs[ eIrqEnum ];

  // get the pin state
  bPinState = ( EIC->PINSTATE.reg & BIT( eIrqEnum )) ? TRUE : FALSE;

  // get the callback
  if (( pvCallback = ptIrq->pvCallback ) != NULL )
  {
    // do it
    pvCallback( eIrqEnum, bPinState );
  }
}

/**@} EOF Gpio.c */
