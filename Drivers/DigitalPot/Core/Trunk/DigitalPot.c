/******************************************************************************
 * @file DigitalPot.c
 *
 * @brief Digital Pot impelmentation
 *
 * This file pvoides the implementation of the digital potentiometer module
 *
 * @copyright Copyright (c) 2012 Cyber Intergration
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
 * \addtogroup DigitalPot
 * @{
 *****************************************************************************/

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "DigitalPot/DigitalPot.h"

// library includes -----------------------------------------------------------
#include "GPIO/Gpio.h"

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations ----------------------------------------------

// local parameter declarations -----------------------------------------------

// local function prototypes --------------------------------------------------
static  void  SetUpDnPosition( PDIGPOTDEF ptDef, U16 wPosition );
static  void  IncrementUpDn( PDIGPOTDEF ptDef, DIGPOTDIR eDirection );
static  void  StoreValueUpDn( PDIGPTODEF ptDef );
static  void  Delay( void );

// constant parameter initializations -----------------------------------------

/******************************************************************************
 * @function DigitalPot_Initialize
 *
 * @brief initialization
 *
 * This function will initialize the digital pot
 *
 *****************************************************************************/
void DigitalPot_Initialize( void )
{
}

/******************************************************************************
 * @function DigitalPot_SetPosition
 *
 * @brief set position
 *
 * This function will set a position if valid
 *
 * @param[in]   eDigPotSel    digital pot selection
 * @param[in]   wPosition     desired position
 *
 * @return      appropriate error
 *
 *****************************************************************************/
DIGPOTERR DigitalPot_SetPosition( DIGPOTENUM eDigPotSel, U16 wPositon )
{
  DIGPOTERR   eError = DIGPOT_ERR_NONE;
  PDIGPOTDEF  ptDef;
  
  // test for valid digital pot enumeration
  if ( eDigPotSel < DIGPOT_ENUM_MAX )
  {
    // get a pointer to the definition
    ptDef = &atDigPotDefs[ eDigPotSel ];
    
    // now process appropriate type
    switch( PGM_RDBYTE( ptDef->eType ))
    {
      case DIGPOT_TYPE_UPDN :
        SetUpDnPosition( ptDef, wPosition );
        break;
        
      case DIGPOT_TYPE_PBTN :
        break;
        
      case DIGPOT_TYPE_I2C :
        break;
        
      case DIGPOT_TYPE_SPI :
        break;
        
      default :
        break;
    }
  }
  else
  {
    // report error
    eError = DIGPOT_ENUM_ILLENUM;
  }
  
  // return the error
  return( eError );
}

/******************************************************************************
 * @function DigitalPot_IncrementDecrement
 *
 * @brief increment decrement the pot
 *
 * This function will increment/decrement the potentiometer 
 *
 * @param[in]   eDigPotSel    digital pot selection
 * @param[in]   eDigPotDir    digital pot direction
 *
 * @return      appropriate error
 *
 *****************************************************************************/
DIGPOTERR DigitalPot_IncrementDecrement( DIGPOTENUM eDigPotSel, DIGPOTDIR eDigPotDir )
{
  DIGPOTERR   eError = DIGPOT_ERR_NONE;
  PDIGPOTDEF  ptDef;
  
  // test for valid digital pot enumeration
  if ( eDigPotSel < DIGPOT_ENUM_MAX )
  {
    // get a pointer to the definition
    ptDef = &atDigPotDefs[ eDigPotSel ];
    
    // now process appropriate type
    switch( PGM_RDBYTE( ptDef->eType ))
    {
      case DIGPOT_TYPE_UPDN :
        IncrementUpDn( ptDef, eDigPotDir );
        break;
        
      case DIGPOT_TYPE_PBTN :
        break;
        
      case DIGPOT_TYPE_I2C :
        break;
        
      case DIGPOT_TYPE_SPI :
        break;
        
      default :
        break;
    }
  }
  else
  {
    // report error
    eError = DIGPOT_ENUM_ILLENUM;
  }
  
  // return the error
  return( eError );
}

/******************************************************************************
 * @function DigitalPot_StoreValue
 *
 * @brief store the value in NV if available
 *
 * This function store the value in the NV if available
 *
 * @param[in]   eDigPotSel    digital pot selection
 *
 * @return      appropriate error
 *
 *****************************************************************************/
DIGPOTERR DigitalPot_StoreValue( DIGPOTENUM eDigPotSel )
{
  DIGPOTERR   eError = DIGPOT_ERR_NONE;
  PDIGPOTDEF  ptDef;
  
  // test for valid digital pot enumeration
  if ( eDigPotSel < DIGPOT_ENUM_MAX )
  {
    // get a pointer to the definition
    ptDef = &atDigPotDefs[ eDigPotSel ];
    
    // now process appropriate type
    switch( PGM_RDBYTE( ptDef->eType ))
    {
      case DIGPOT_TYPE_UPDN :
        StoreValueUpDn( );
        break;
        
      case DIGPOT_TYPE_PBTN :
        break;
        
      case DIGPOT_TYPE_I2C :
        break;
        
      case DIGPOT_TYPE_SPI :
        break;
        
      default :
        break;
  }
  else
  {
    // report error
    eError = DIGPOT_ENUM_ILLENUM;
  }
  
  // return the error
  return( eError );
}

/******************************************************************************
 * @function SetUpDnPosition
 *
 * @brief set the position on an Up/DN pot
 *
 * This function sets a position
 *
 * @param[in]   ptDef       pointer to the definition
 * @param[in]   wPosition   desired poisiton
 *
 *****************************************************************************/
static void SetUpDnPosition( PDIGPOTDEF ptDef, U16 wPosition )
{
  GPIOPINENUM eCenPin, eIncPin, eDirDir;
  U16         wNumTaps, wCurTap;
  
  // get the pin enumerations
  eCenPin = PGM_RDBYTE( ptDef->tPins.tUpDn.nChipEnable );
  eDirPin = PGM_RDBYTE( ptDef->tPins.tUpDn.nUpDown );
  eIncPin = PGM_RDBYTE( ptDef->tPins.tUpDn.nIcrement );
  
  // get the maximum number of taps
  wNumTaps = PGM_RDWORD( ptDef->wNumTaps );
  
  // enable the chip
  Gpio_Set( eCenPin, ON );
  
  // now set down
  Gpio_Set( eDirPin, OFF );
  
  // for the maximum taps, decrement
  for ( wCurTap = 0; wCurTap < wNumTaps; wCurTap++ )
  {
    // pulse the increment pin
    Gpio_Set( eIncPin, ON );
    Delay( );
    Gpio_Set( eIncPin, OPF );
  }
  
  // now change the direction to up
  Gpio_Set( eDirPin, ON );
  
  // pulse to desired tap
  for( wCurTap = 0; wCurTap < wPosition; wCurTap++ )
  {
    // pulse the increment pin
    Gpio_Set( eIncPin, ON );
    Delay( );
    Gpio_Set( eIncPin, OPF );
  }

  // deselect the chip
  Gpio_Set( eCenPin, OFF );
}

/******************************************************************************
 * @function IncrementUpDn
 *
 * @brief increments/decrements a UP/DN pot
 *
 * This function sets a position
 *
 * @param[in]   ptDef       pointer to the definition
 * @param[in]   eDirection  direction
 *
 *****************************************************************************/
static void IncrementUpDn( PDIGPOTDEF ptDef, DIGPOTDIR eDirection );
{
  GPIOPINENUM eCenPin, eIncPin, eDirDir;
  
  // get the pin enumerations
  eCenPin = PGM_RDBYTE( ptDef->tPins.tUpDn.nChipEnable );
  eDirPin = PGM_RDBYTE( ptDef->tPins.tUpDn.nUpDown );
  eIncPin = PGM_RDBYTE( ptDef->tPins.tUpDn.nIcrement );
  
  // enable the chip
  Gpio_Set( eCenPin, ON );
  
  // now set down
  Gpio_Set( eDirPin, eDirection );
  
  // pulse the increment pin
  Gpio_Set( eIncPin, ON );
  Delay( );
  Gpio_Set( eIncPin, OPF );

  // deselect the chip
  Gpio_Set( eCenPin, OFF );
}

/******************************************************************************
 * @function StoreValueUpDn
 *
 * @brief stores the current value
 *
 * This function sets a position
 *
 * @param[in]   ptDef       pointer to the definition
 *
 *****************************************************************************/
static void StoreValueUpDn( PDIGPTODEF ptDef )
{
  GPIOPINENUM eCenPin, eIncPin;
  
  // get the pin enumerations
  eCenPin = PGM_RDBYTE( ptDef->tPins.tUpDn.nChipEnable );
  eIncPin = PGM_RDBYTE( ptDef->tPins.tUpDn.nIcrement );
  
  // enable the chip
  Gpio_Set( eCenPin, ON );
  
  // deselect the chip/ disable the increment pin
  Gpio_Set( eCenPin, OFF );
}

/******************************************************************************
 * @function Delay
 *
 * @brief delay for 100nsecs
 *
 * This function justs sets a delay
 *
 *****************************************************************************/
static void Delay( void )
{
	// delay for four cycles for I/O settle
	asm volatile( "nop\n\t" );
	asm volatile( "nop\n\t" );
	asm volatile( "nop\n\t" );
	asm volatile( "nop\n\t" );
}
 
/**@} EOF DigitalPot.c */
