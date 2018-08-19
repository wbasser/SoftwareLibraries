/******************************************************************************
 * @file Adc.c
 *
 * @brief ADC implementation 
 *
 * This file provides the implementation for the ADC
 *
 * @copyright Copyright (c) 2012 CyberIntegration
 * This document contains proprietary data and information of CyberIntegration 
 * LLC. It is the exclusive property of CyberIntegration, LLC and will not be 
 * disclosed in any form to any party without prior written permission of 
 * CyberIntegration, LLC. This document may not be reproduced or further used 
 * without the prior written permission of CyberIntegration, LLC.
 *
 * $Date: $
 *
 * Version History
 * ======
 * $Rev: $
 * 
 *
 * \addtogroup ADC
 * @{
 *****************************************************************************/

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "ADC/Adc.h"

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations ----------------------------------------------

// local parameter declarations -----------------------------------------------
static  VBOOL         bConvertInProgress;
static  PVADCCALLBACK pvCurCallback;
static  U16UN         tLastResult;

// local function prototypes --------------------------------------------------
static  ADCERRS   SetupNormalMode( PADCDEF ptDef );
static  ADCERRS   ValidateCommonParameters( PADCDEF ptDef );

// constant parameter initializations -----------------------------------------

/******************************************************************************
 * @function Adc_Initialize
 *
 * @brief ADC initialization
 *
 * This function will performm any given initialization for the A2D converter
 *
 *****************************************************************************/
void Adc_Initialize( void )
{
}

/******************************************************************************
 * @function Adc_Close
 *
 * @brief ADC close
 *
 * This function will close the ADC channel
 *
 *****************************************************************************/
void Adc_Close( void )
{
  // reset the ADC
  ADCSRA = 0;
}

/******************************************************************************
 * @function Adc_SetMux
 *
 * @brief set a mux to a channel
 *
 * This function will just set the mux to a given channel
 *
 * @param[in]   eAdcChan    channel definition
 *
 *****************************************************************************/
void Adc_SetMux( ADCPOSCHAN eChannel )
{
  U8  nMuxValue = 0;
  U8  nCsrBValue = 0;

  // process the channel
  if ( eChannel < ADC_POSCHAN_AIN08 )
  {
    // set the mux value
    nMuxValue |= eChannel;
    nCsrBValue = 0;
  }
  else
  {
    // set the mux value
    nMuxValue |= eChannel - ADC_POSCHAN_AIN08;
    nCsrBValue = BIT( MUX5 );
  }

  // set up the mux
  ADMUX = nMuxValue;
  ADCSRB = nCsrBValue;
}

/******************************************************************************
 * @function Adc_ConvertChannel
 *
 * @brief perform a conversion on a given AD channel
 *
 * This function will setup and perform a converstion on a A2D channel
 *
 * @param[in]   eAdcChan    channel definition
 * @param[io]   pwResult    result of the conversion if non-blocking
 *
 * @return      the current result of the operation
 *
 *****************************************************************************/
ADCERRS Adc_ConvertChannel( ADCENUM eAdcChan, PU16 pwResult )
{
  ADCERRS eError = ADC_ERR_NONE;
  PADCDEF ptDef;
  U8      nCsrAValue;

  // determine if we have a valid channel
  if ( eAdcChan < ADC_ENUM_MAX )
  {
    // get the definition
    ptDef = ( PADCDEF )&atAdcDefs[ eAdcChan ];

    // setup normal mode
    eError = SetupNormalMode( ptDef );

    // if no error start the conversion
    if ( eError == ADC_ERR_NONE )
    {
      // set the callback
      pvCurCallback = ( PVADCCALLBACK )PGM_RDWORD( ptDef->pvCallback );

      // set the conversion flag true
      bConvertInProgress = TRUE;

      // start the conversion 
      nCsrAValue = PGM_RDBYTE( ptDef->ePrescale );
      nCsrAValue |= ( pvCurCallback != NULL ) ? BIT( ADIF ) : 0;
      nCsrAValue |= BIT( ADEN ) | BIT( ADSC );
      
      // write the value
      ADCSRA = nCsrAValue;

      //  now determine blocking/non-blocking
      if ( pvCurCallback == NULL )
      {
        // wait till done
        while( bConvertInProgress );
        
        // get result
        *( pwResult ) = tLastResult.wValue;
      }
      else
      {
        // return blocking status
        eError = ADC_ERR_BLOCKING;
      }
    }
  }
  else
  {
    // illegal channel
    eError = ADC_ERR_ILLENUM;
  }

  // return the error
  return( eError );
}

/******************************************************************************
 * @function ADC_vect
 *
 * @brief ADC interrupt handler
 *
 * This function will process the ADC handler
 *
 *****************************************************************************/
ISR( ADC_vect )
{
  // get the last result
  tLastResult.anValue[ LE_U16_MSB_IDX ] = ADCH;
  tLastResult.anValue[ LE_U16_LSB_IDX ] = ADCL;

  // if callback do it
  if ( pvCurCallback != NULL )
  {
    // call it
    pvCurCallback( tLastResult.wValue );
  }

  // in all cases clear the convert in progress
  bConvertInProgress = FALSE;
}

/******************************************************************************
 * @function SetupNormLMode
 *
 * @brief setup for normal mode
 *
 * This function will setup the channel for a normal mode conversion
 *
 * @param[in]   ptDef         pointer to the definition
 *
 * @return      the current result of the operation
 *
 *****************************************************************************/
static ADCERRS SetupNormalMode( PADCDEF ptDef )
{
  ADCERRS eError = ADC_ERR_NONE;
  U8      nMuxValue = 0;
  U8      nCsrBValue = 0;

  // check for no error
  if (( eError  = ValidateCommonParameters( ptDef )) == ADC_ERR_NONE )
  {
    // build the value for the ADMUX register
    nMuxValue = PGM_RDBYTE( ptDef->eAdcRef );
    
    // get the mux value
    if ( PGM_RDBYTE( ptDef->eInpMode ) == ADC_INPMODE_SING )
    {
      // process the channel
      if ( PGM_RDBYTE( ptDef->ePosChan ) < ADC_POSCHAN_AIN08 )
      {
        // set the mux value
        nMuxValue |= PGM_RDBYTE( ptDef->ePosChan );
        nCsrBValue = 0;
      }
      else
      {
        // set the mux value
        nMuxValue |= PGM_RDBYTE( ptDef->ePosChan ) - ADC_POSCHAN_AIN08;
        nCsrBValue = BIT( MUX5 );
      }
    }
    else
    {
    
    }
    
    // now set the mux/csrB registers
    ADMUX = nMuxValue;
    ADCSRB = nCsrBValue;
  }

  // return the error
  return( eError );
}

/******************************************************************************
 * @function ValidateCommonParameters
 *
 * @brief setup for average mode
 *
 * This function will setup the channel for a average mode conversion
 *
 * @param[in]   ptDef         pointer to the definition
 *
 * @return      the current result of the operation
 *
 *****************************************************************************/
static ADCERRS ValidateCommonParameters( PADCDEF ptDef )
{
  ADCERRS eError = ADC_ERR_ILLPRM;

  // validate the parameters
  if (( ptDef->ePosChan < ADC_POSCHAN_INPMAX ) || (( ptDef->ePosChan >= ADC_POSCHAN_TEMP ) && ( ptDef->ePosChan < ADC_POSCHAN_MAX ))) 
  {
    if ( ptDef->eAdcRef < ADC_REF_MAX ) 
    {
      if ( ptDef->eGain <= ADC_GAIN_MAX )
      {
        // no error detected
        eError = ADC_ERR_NONE;
      }
    }
  }

  // return the error
  return( eError );
}

/**@} EOF Adc.c */
