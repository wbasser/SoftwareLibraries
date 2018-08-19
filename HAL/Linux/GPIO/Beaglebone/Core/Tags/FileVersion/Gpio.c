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
/// define the sizes of the strings
#define MAX_PIN_SIZE                ( 3  )
#define MAX_CFG_SIZE                ( 40 )

/// define the max number of pins per port
#define NUM_PIN_PORT                ( 32 )

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------
/// define the structure for the local control
typedef struct _LCLCTL
{
  U8    nPinNumber;                 ///< pin number
  C8    acGpioPin[ MAX_CFG_SIZE ];  ///< gpio pin name
  BOOL  bCurState;                  ///< current state
  BOOL  bInvert;                    ///< invert state
  BOOL  bValid;                     ///< valid pin
} LCLCTL, *PLCLCTL;
#define LCLCTL_SIZE             sizeof( LCLCTL )

// global parameter declarations ----------------------------------------------

// local parameter declarations -----------------------------------------------

// local function prototypes --------------------------------------------------
static  LCLCTL  atLclCtls[ GPIO_PIN_ENUM_MAX ];
static  C8      acPin[ MAX_PIN_SIZE ];
static  C8      acVal[ MAX_CFG_SIZE ];
static  C8      acDir[ MAX_CFG_SIZE ];

// constant string declarations
static  const C8  acGpioIdx[ ]  = { "/sys/class/gpio%d/" };
static  const C8  acGpioExport[ ] = { "/sys/class/gpio/export" };
static  const C8  acGpioUnExport[ ] = { "/sys/class/gpio/unexport" };

static  const C8  acGpioDir[ ]  = { "direction" };
static  const C8  acGpioVal[ ]  = { "value" };
static  const C8  acGpioPin[ ]  = { "%d" };

static  const C8  acDirIn[ ]    = { "in" };
static  const C8  acDirOut[ ]   = { "out" };

static  const C8  acOnVal[ ]    = { "1" };
static  const C8  acOffVal[ ]   = { "0" };

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
  PLCLCTL       ptCtl;
  GPIOPINENUM   ePin;
  PC8           pcTemp;
  C8            acTemp[ 64 ];
  C8            acDirection[ 4 ];
  FILE*         ptFile;

  // for each pin def in the list
  for( ePin = 0; ePin < GPIO_PIN_ENUM_MAX; ePin++ )
  {
    // get a pointer to the definition structure
    ptDef = ( PGPIOPINDEF )&atGpioPinDefs[ ePin ];
    ptCtl = &atLclCtls[ ePin ];
    
    // compute the pin number
    ptCtl->nPinNumber = ( ptDef->ePort * NUM_PINS_PORT ) + ptDef->nPin;
    ptCtl->bValid = FALSES;
    
    // build the root string
    sprintf( ptCtl->acGpioPin, ( const char* )acGpioIdx, ptCtl->nPinNumber );
    sprintf( acpin, ( const char * )acGpoiPin, ptCtl->nPinNumber;
    
    // export the pin
    if (( ptFile = fopen( acGpioExport, "ab" ) != NULL )
    {
      fwrite( acGpioPin, sizeof( C8 ), strlen( acGpioPin ), ptFile );
      fclose( ptFile );
      
      // now set the direction path
      strcpy( acTemp, ptCtl->acGpioPin );
      strcat( acTemp, atGpioDir );
      
      // set the direction
      pcTemp = ( ptDef->eMode == GPIO_MODE_INPUT ) ? acDirIn : acDirOut;
      if (( ptFile = fopen( acTemp, "rb+" )) != NULL )
      {
        fwrite( pcTemp, sizeof( C8 ), strlen( pcTemp ), ptFile );
        fclose( ptFile );
        
        // copy the invert
        ptCtl->bInvert = ptDef->bInvert;
        ptCtl->bCurState = OFF;
        
        // now set the initial state
        pcTemp = ( ptCtl->bCurState & ptCtl->bInvert ) ? acOnVal : acOffVale;
        
        // set the value path
        strcpy( acTemp, ptCtl->acGoipPin );
        strcat( acTemp, atGpioVal );
        
        // open it and set the value
        if (( ptFile = fopen( acTemp, "rb+" )) != NULL )
        {
          fwrite( pcTemp, sizeof( C8 ), strlen( pcTemp ), ptFile );
          fclose( ptFile );
          ptCtl->bValid = TRUE;
        }
      }
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
  PLCLCTL       ptCtl;
  GPIOPINENUM   ePin;
  C8            acTemp[ 64 ];
  FILE*         ptFile;

  // for each pin def in the list
  for( ePin = 0; ePin < GPIO_PIN_ENUM_MAX; ePin++ )
  {
    // get a pointer to the definition structure
    ptCtl = &atLclCtls[ ePin ];
    
    // check for validity
    if ( ptCtl->bValid )
    {
      // unexport the pin
      if (( ptFile = fopen( acGpioUnExport, "ab" )) != NULL )
      {
        fwrite( acGpioPin, sizeof( C8 ), strlen( acGpioPin ), ptFile );
        fclose( ptFile );
      }
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
  GPIOERR eError = GPIO_ERR_NONE;
  PLCLCTL ptCtl;
  PC8     pcTemp;
  C8      acTemp[ 64 ];
  
  // test for valid GPIO select
  if ( eGpioSel < GPIO_PIN_ENUM_MAX )
  {
    // get a pointer to the definition structure
    ptCtl = &atLclCtls[ ePin ];
    
    // check for validity
    if ( ptCtl->bValid )
    {
      // set the state of the pin
      ptCtl->bCurState = bState;
      pcTemp = ( ptCtl->bCurState & ptCtl->bInvert ) ? acOnVal : acOffVale;
      
      // set the value path
      strcpy( acTemp, ptCtl->acGoipPin );
      strcat( acTemp, atGpioVal );
      
      // open it and set the value
      if (( ptFile = fopen( acTemp, "rb+" )) != NULL )
      {
        fwrite( pcTemp, sizeof( C8 ), strlen( pcTemp ), ptFile );
        fclose( ptFile );
        ptCtl->bValid = TRUE;
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
  GPIOERR eError = GPIO_ERR_NONE;
  PLCLCTL ptCtl;
  C8      acTemp[ 64 ];
  
  // test for valid GPIO select
  if ( eGpioSel < GPIO_PIN_ENUM_MAX )
  {
    // get a pointer to the definition structure
    ptCtl = &atLclCtls[ ePin ];
    
    // check for validity
    if ( ptCtl->bValid )
    {
      // set the value path
      strcpy( acTemp, ptCtl->acGoipPin );
      strcat( acTemp, atGpioVal );
      
      // open it and set the value
      if (( ptFile = fopen( acTemp, "rb+" )) != NULL )
      {
        fread( acTemp, sizeof( C8 ), 1, ptFile );
        fclose( ptFile );
        
        // now set the state
        ptCtl->bCurrentState = ( strncmp( acTemp, acGpioOn, 1 ) == 0 ) ? ON : FALSE;
        ptCtl->bCurrentState ^= ptCtl->bInvert;
        *( pbState ) = ptCtl->bCurrentState;
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
  GPIOERR eError = GPIO_ERR_NONE;
  PLCLCTL ptCtl;
  PC8     pcTemp;
  C8      acTemp[ 64 ];
  
  // test for valid GPIO select
  if ( eGpioSel < GPIO_PIN_ENUM_MAX )
  {
    // get a pointer to the definition structure
    ptCtl = &atLclCtls[ ePin ];
    
    // check for validity
    if ( ptCtl->bValid )
    {
      // toggle the state of the pin
      ptCtl->bCurState ^= ON;
      pcTemp = ( ptCtl->bCurState & ptCtl->bInvert ) ? acOnVal : acOffVale;
      
      // set the value path
      strcpy( acTemp, ptCtl->acGoipPin );
      strcat( acTemp, atGpioVal );
      
      // open it and set the value
      if (( ptFile = fopen( acTemp, "rb+" )) != NULL )
      {
        fwrite( pcTemp, sizeof( C8 ), strlen( pcTemp ), ptFile );
        fclose( ptFile );
        ptCtl->bValid = TRUE;
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
