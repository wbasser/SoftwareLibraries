/******************************************************************************
 * @file GPIO.c
 *
 * @brief GPIO implementation
 *
 * This file implements the GPIO subsystem 
 *
 * @copyright Copyright (c) 2012 Nan Technologies
 * This document contains proprietary data and information of Nan Technologies 
 * LLC. It is the exclusive property of Nan Technologies, LLC and will not be 
 * disclosed in any form to any party without prior written permission of 
 * Nan Technologies, LLC. This document may not be reproduced or further used 
 * without the prior written permission of Nan Technologies, LLC.
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
#if ( GPIO_ENABLE_PIN_REFRESH == 1 )
/// define the control structure
typedef struct _GPIOCTL
{

} GPIOCTL;
#endif

// global parameter declarations ----------------------------------------------

// local parameter declarations -----------------------------------------------
#if ( GPIO_ENABLE_PIN_REFRESH == 1 )
static  GPIOCTL atGpioCtl[ GPIO_PIN_ENUM_MAX ];
#endif

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
  GPIOPINENUM   ePinIdx;
  GPIOFUNCENUM  eFncIdx;
  PGPIODEF      ptDef;
  U8            nCtlIdx;
  U8            nHysIdx;
  U32           uCtlMask;
  U32           uHysMask;
  U32           uPinMask;

  // for each list in the pin list
  for( ePinIdx = 0; ePinIdx < GPIO_PIN_ENUM_MAX; ePinIdx++ )
  {
    // get the pointer
    ptDef = ( PGPIODEF )&atGpioDefs[ ePinIdx ];

    // compute the control/hysteresis port index
    nCtlIdx = ptDef->nPin / 4;
    nHysIdx = ptDef->nPin / 8;

    // compute the control/hysteresis mask index
    uCtlMask = BITS( ptDef->eMode, ((( ptDef->nPin % 4 ) * 8 ) + PORT0_IOCR0_PC0_Pos ));
    uHysMask = BITS( ptDef->eHyst, ((( ptDef->nPin % 8 ) * 4 ) + PORT0_PHCR0_PH0_Pos ));
    uPinMask = BIT( ptDef->nPin );
    
    // determine the control port register
    switch( ptDef->ePort )
    {
      case GPIO_PORT_P0 :
        // set the mode
        switch( nCtlIdx )
        {
          case 0 :
            PORT0->IOCR0 |= uCtlMask;
            break;

          case 1 :
            PORT0->IOCR4 |= uCtlMask;
            break;

          case 2 :
            PORT0->IOCR8 |= uCtlMask;
            break;

          case 3 :
            PORT0->IOCR12 |= uCtlMask;
            break;

          default :
            break;
        }

        // set the hystersis
        switch( nHysIdx )
        {
          case 0 :
            PORT0->PHCR0 |= ( ptDef->eHyst == GPIO_HYST_LRG ) ? uHysMask : 0;
            break;

          case 1 :
            PORT0->PHCR1 |= ( ptDef->eHyst == GPIO_HYST_LRG ) ? uHysMask : 0;
            break;

          default :
            break;
        }

        // set the initial value
        if (( ptDef->eMode > GPIO_MODE_INP_NRMPU ) && ( ptDef->bInitialState ^ ptDef->bInvert ))
        {
          PORT0->OMR |= uPinMask;
        }
        break;
        
      case GPIO_PORT_P1 :
        // set the mode
        switch( nCtlIdx )
        {
          case 0 :
            PORT1->IOCR0 |= uCtlMask;
            break;

          case 1 :
            PORT1->IOCR4 |= uCtlMask;
            break;

          default :
            break;
        }

        // set the hystersis
        switch( nHysIdx )
        {
          case 0 :
            PORT1->PHCR0 |= ( ptDef->eHyst == GPIO_HYST_LRG ) ? uHysMask : 0;
            break;

          default :
            break;
        }

        // set the initial value
        if (( ptDef->eMode > GPIO_MODE_INP_NRMPU ) && ( ptDef->bInitialState ^ ptDef->bInvert ))
        {
          PORT1->OMR |= uPinMask;
        }
        break;

      case GPIO_PORT_P2 :
        // set the mode
        switch( nCtlIdx )
        {
          case 0 :
            PORT2->IOCR0 |= uCtlMask;
            break;

          case 1 :
            PORT2->IOCR4 |= uCtlMask;
            break;

          case 2 :
            PORT2->IOCR8 |= uCtlMask;
            break;

          default :
            break;
        }
        
        // set the hystersis
        switch( nHysIdx )
        {
          case 0 :
            PORT2->PHCR0 |= ( ptDef->eHyst == GPIO_HYST_LRG ) ? uHysMask : 0;
            break;

          case 1 :
            PORT2->PHCR1 |= ( ptDef->eHyst == GPIO_HYST_LRG ) ? uHysMask : 0;
            break;

          default :
            break;
        }

        // set the initial value
        if (( ptDef->eMode > GPIO_MODE_INP_NRMPU ) && ( ptDef->bInitialState ^ ptDef->bInvert ))
        {
          PORT2->OMR |= uPinMask;
        }

        // set digital mode
        PORT2->PDISC &= ~uPinMask;
        break;
        
      default :
        break;
    }
  }
  
  // for each list in the function list
  for( eFncIdx = 0; eFncIdx < GPIO_FUNC_ENUM_MAX; eFncIdx++ )
  {
    // get the pointer
    ptDef = ( PGPIODEF )&atGpioFuncs[ eFncIdx ];

    // compute the control/hysteresis port index
    nCtlIdx = ptDef->nPin / 4;

    // compute the control/hysteresis mask index
    uCtlMask = BITS( ptDef->eMode, ((( ptDef->nPin % 4 ) * 8 ) + PORT0_IOCR0_PC0_Pos ));
    uPinMask = BIT( ptDef->nPin );

    // determine the control port register
    switch( ptDef->ePort )
    {
      case GPIO_PORT_P0 :
        // set the mode
        switch( nCtlIdx )
        {
          case 0 :
            PORT0->IOCR0 |= uCtlMask;
            break;

          case 1 :
            PORT0->IOCR4 |= uCtlMask;
            break;

          case 2 :
            PORT0->IOCR8 |= uCtlMask;
            break;

          case 3 :
            PORT0->IOCR12 |= uCtlMask;
            break;

          default :
            break;
        }

        // set the hystersis
        switch( nHysIdx )
        {
          case 0 :
            PORT0->PHCR0 |= ( ptDef->eHyst == GPIO_HYST_LRG ) ? uHysMask : 0;
            break;

          case 1 :
            PORT0->PHCR1 |= ( ptDef->eHyst == GPIO_HYST_LRG ) ? uHysMask : 0;
            break;

          default :
            break;
        }

        // ensure off
        PORT0->OMR |= ( uPinMask << 16 );
        break;

      case GPIO_PORT_P1 :
        // set the mode
        switch( nCtlIdx )
        {
          case 0 :
            PORT1->IOCR0 |= uCtlMask;
            break;

          case 1 :
            PORT1->IOCR4 |= uCtlMask;
            break;

          default :
            break;
        }

        // set the hystersis
        switch( nHysIdx )
        {
          case 0 :
            PORT1->PHCR0 |= ( ptDef->eHyst == GPIO_HYST_LRG ) ? uHysMask : 0;
            break;

          default :
            break;
        }

        // ensure off
        PORT1->OMR |= ( uPinMask << 16 );
        break;

      case GPIO_PORT_P2 :
        // set the mode
        switch( nCtlIdx )
        {
          case 0 :
            PORT2->IOCR0 |= uCtlMask;
            break;

          case 1 :
            PORT2->IOCR4 |= uCtlMask;
            break;

          case 2 :
            PORT2->IOCR8 |= uCtlMask;
            break;

          default :
            break;
        }
        
        // set the hystersis
        switch( nHysIdx )
        {
          case 0 :
            PORT2->PHCR0 |= ( ptDef->eHyst == GPIO_HYST_LRG ) ? uHysMask : 0;
            break;

          case 1 :
            PORT2->PHCR1 |= ( ptDef->eHyst == GPIO_HYST_LRG ) ? uHysMask : 0;
            break;

          default :
            break;
        }

        // ensure off
        PORT2->OMR |= ( uPinMask << 16 );

        // set digital mode
        PORT2->PDISC &= ~uPinMask;
        break;

      default :
        break;
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
  U32       uMask;
  
  // test for valid GPIO select
  if ( eGpioSel < GPIO_PIN_ENUM_MAX )
  {
    // get the pointer
    ptDef = ( PGPIODEF )&atGpioDefs[ eGpioSel ];
    
    // check for an output pin
    if (( ptDef->eMode == GPIO_MODE_OUT_NRMPP ) || ( ptDef->eMode == GPIO_MODE_OUT_NRMOD ))
    {
      // compute the mask
      uMask = BIT( ptDef->nPin );
      if ( !( bState ^ ptDef->bInvert ))
      {
        // adjust for reset
        uMask <<= 16;
      }

      // now set the appropriate port
      switch( ptDef->ePort )
      {
        case GPIO_PORT_P0 :
          PORT0->OMR |= uMask;
          break;

        case GPIO_PORT_P1 :
          PORT1->OMR |= uMask;
          break;

        case GPIO_PORT_P2 :
          PORT2->OMR |= uMask;
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
  U32       uMask;
  U32       uValue = 0;

  // test for valid GPIO select
  if ( eGpioSel <GPIO_PIN_ENUM_MAX )
  {
    // get the pointer
    ptDef = ( PGPIODEF )&atGpioDefs[ eGpioSel ];

    // compute the mask
    uMask = BIT( ptDef->nPin );

    // now set the appropriate port
    switch( ptDef->ePort )
    {
      case GPIO_PORT_P0 :
        uValue = PORT0->IN;
        break;

      case GPIO_PORT_P1 :
        uValue = PORT1->IN;
        break;

      case GPIO_PORT_P2 :
        uValue = PORT2->IN;
        break;

      default :
        break;
    }
    
    // return the state of the pin
    *pbState = ( uValue & uMask ) ? TRUE : FALSE;
    if ( ptDef->bInvert )
    {
      *pbState = !*pbState;
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
  GPIOERR   eError = GPIO_ERR_NONE;
  PGPIODEF  ptDef;
  U32       uMask;
  
  // test for valid GPIO select
  if ( eGpioSel < GPIO_PIN_ENUM_MAX )
  {
    // get the pointer
    ptDef = ( PGPIODEF )&atGpioDefs[ eGpioSel ];

    // check for an output pin
    if (( ptDef->eMode == GPIO_MODE_OUT_NRMPP ) || ( ptDef->eMode == GPIO_MODE_OUT_NRMOD ))
    {
      // compute the mask
      uMask = BIT( ptDef->nPin ) | ( BIT( ptDef->nPin ) << 16 );

      // now set the appropriate port
      switch( ptDef->ePort )
      {
        case GPIO_PORT_P0 :
          PORT0->OMR |= uMask;
          break;

        case GPIO_PORT_P1 :
          PORT1->OMR |= uMask;
          break;

        case GPIO_PORT_P2 :
          PORT2->OMR |= uMask;
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
  GPIOERR   eError = GPIO_ERR_NONE;
  
  // test for valid GPIO select
  if ( eGpioSel < GPIO_PIN_ENUM_MAX )
  {
    // perform the action
    switch( eGpioAct )
    {
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

#if ( GPIO_ENABLE_PIN_REFRESH == 1 )
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
#endif

/**@} EOF Gpio.c */
