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
static  VU16          wLastResult;

// local function prototypes --------------------------------------------------
static  ADCERRS   SetupNormalMode( PADCDEF ptDef );
static  ADCERRS   SetupAcumAvrgMode( PADCDEF ptDef );
static  ADCERRS   ValidateCommonParameters( PADCDEF ptDef );
static  void      SetCommonParameters( PADCDEF ptDef, ADC_CTRLB_Type* ptCtlB );

// constant parameter initializations -----------------------------------------

/******************************************************************************
 * @function Adc_Initialize
 *
 * @brief ADC initialization
 *
 * This function will perform any given initialization for the A2D converter
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
  ADC->CTRLA.bit.SWRST = TRUE;
  while( ADC->STATUS.bit.SYNCBUSY );

  // disable interrupt
  NVIC_DisableIRQ( ADC_IRQn );
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

  // determine if we have a valid channel
  if ( eAdcChan < ADC_ENUM_MAX )
  {
    // get the definition
    ptDef = ( PADCDEF )&atAdcDefs[ eAdcChan ];

    // disable the interrupt
    NVIC_DisableIRQ( ADC_IRQn );

    // reset the ADC
    ADC->CTRLA.bit.SWRST = TRUE;
    while( ADC->STATUS.bit.SYNCBUSY );

    // now determine the mode
    switch( ptDef->eMode )
    {
      case ADC_MODE_NORMAL :
        eError = SetupNormalMode( ptDef );
        break;

      case ADC_MODE_ACCUM :
      case ADC_MODE_AVERAGE :
        eError = SetupAcumAvrgMode( ptDef );
        break;

      case ADC_MODE_DECIMATE :
        break;

      default :
        eError = ADC_ERR_ILLMODE;
        break;
    }

    // if no error start the conversion
    if ( eError == ADC_ERR_NONE )
    {
      // set the callback
      pvCurCallback = ptDef->pvCallback;

      // enable the interrupts
      ADC->INTENSET.bit.RESRDY = ON;
      NVIC_EnableIRQ( ADC_IRQn );

      // enable it
      ADC->CTRLA.bit.ENABLE = ON;
      while( ADC->STATUS.bit.SYNCBUSY );

      // set the conversion flag true
      bConvertInProgress = TRUE;

      // start the conversion 
      ADC->SWTRIG.bit.START = ON;
      while( ADC->STATUS.bit.SYNCBUSY );

      //  now determine blocking/non-blocking
      if ( ptDef->pvCallback == NULL )
      {
        // wait till done
        while( bConvertInProgress );
        
        // get result
        *( pwResult ) = wLastResult;
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
 * @function Adc_ControlEventChannel
 *
 * @brief perform a stop/start an event driven A2D channel
 *
 * This function will setup and perform a converstion on a A2D channel
 *
 * @param[in]   eAdcChan    channel definition
 * @param[in]   bState      desired state
 *
 * @return      the current result of the operation
 *
 *****************************************************************************/
ADCERRS Adc_ControlEventChannel( ADCENUM eAdcChan, BOOL bState )
{
  ADCERRS eError = ADC_ERR_NONE;
  PADCDEF ptDef;

  // determine if we have a valid channel
  if ( eAdcChan < ADC_ENUM_MAX )
  {
    // get the definition
    ptDef = ( PADCDEF )&atAdcDefs[ eAdcChan ];

    // check for on
    if ( bState )
    {
      // now determine the mode
      switch( ptDef->eMode )
      {
        case ADC_MODE_NORMAL :
          eError = SetupNormalMode( ptDef );
          break;

        case ADC_MODE_ACCUM :
        case ADC_MODE_AVERAGE :
          eError = SetupAcumAvrgMode( ptDef );
          break;

        case ADC_MODE_DECIMATE :
          break;

        default :
          eError = ADC_ERR_ILLMODE;
          break;
      }

      // if no error start the conversion
      if ( eError == ADC_ERR_NONE )
      {
        // set the callback
        pvCurCallback = ptDef->pvCallback;

        // enable it
        ADC->CTRLA.bit.ENABLE = ON;
        while( ADC->STATUS.bit.SYNCBUSY );
        
        // enable the event
        ADC->EVCTRL.reg = ADC_EVCTRL_STARTEI;

        // enable the interrupts
        ADC->INTENSET.bit.RESRDY = ON;
        NVIC_EnableIRQ( ADC_IRQn );
      }
    }
    else
    {
      // disable the interrupt
      NVIC_DisableIRQ( ADC_IRQn );

      // reset the ADC
      ADC->CTRLA.bit.SWRST = TRUE;
      while( ADC->STATUS.bit.SYNCBUSY );
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
 * @function ADC_Handler
 *
 * @brief ADC interrupt handler
 *
 * This function will process the ADC handler
 *
 *****************************************************************************/
void ADC_Handler( void )
{
  // get the last result
  wLastResult = ADC->RESULT.reg;

  // if callback do it
  if ( pvCurCallback != NULL )
  {
    // call it
    pvCurCallback( wLastResult );
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
  ADCERRS             eError = ADC_ERR_NONE;
  ADC_CTRLB_Type      tCtlB;

  // check for no error
  if (( eError  = ValidateCommonParameters( ptDef )) == ADC_ERR_NONE )
  {
    // set up the common parameters
    SetCommonParameters( ptDef, &tCtlB );

    // set the refernce/average/samples
    ADC->AVGCTRL.reg = 0;

    // set up  control B
    tCtlB.bit.RESSEL = 0;
    ADC->CTRLB = tCtlB;
    while( ADC->STATUS.bit.SYNCBUSY );
  }

  // return the error
  return( eError );
}

/******************************************************************************
 * @function SetupAcumAvrgMode
 *
 * @brief setup for accum/average mode
 *
 * This function will setup the channel for a average mode conversion
 *
 * @param[in]   ptDef         pointer to the definition
 *
 * @return      the current result of the operation
 *
 *****************************************************************************/
static ADCERRS SetupAcumAvrgMode( PADCDEF ptDef )
{
  ADCERRS           eError = ADC_ERR_NONE;
  ADC_CTRLB_Type    tCtlB;
  ADC_AVGCTRL_Type  tAvgCtl;
  U8                nAdjRes;

  // check for parameter errors
  eError = ValidateCommonParameters( ptDef );

  // check for no error
  if ( eError == ADC_ERR_NONE )
  {
    // set up common parameters
    SetCommonParameters( ptDef, &tCtlB );

    // check for mode parameters
    if ( ptDef->eNumSamples < ADC_NUMSAMP_MAX )
    {
      // set the refernce/average/samples
      tAvgCtl.reg = ptDef->eNumSamples;

      // if this is average mode
      if ( ptDef->eMode == ADC_MODE_AVERAGE )
      {
        // now set the resolution adjust
        switch( ptDef->eNumSamples )
        {
          case ADC_NUMSAMP_1 :
            nAdjRes = 0;
            break;

          case ADC_NUMSAMP_2 :
            nAdjRes = 1;
            break;

          case ADC_NUMSAMP_4 :
            nAdjRes = 2;
            break;

          case ADC_NUMSAMP_8 :
            nAdjRes = 3;
            break;

          case ADC_NUMSAMP_16 :
          case ADC_NUMSAMP_32 :
          case ADC_NUMSAMP_64 :
          case ADC_NUMSAMP_128 :
          case ADC_NUMSAMP_256 :
          case ADC_NUMSAMP_512 :
          case ADC_NUMSAMP_1024 :
            nAdjRes = 4;
            break;

          default :
            nAdjRes = 0;
            break;
        }

        // now set the resolution
        tAvgCtl.reg |= ADC_AVGCTRL_ADJRES( nAdjRes ); 
      }

      // store the average control
      ADC->AVGCTRL = tAvgCtl;

      // set up  control B
      tCtlB.reg |= ADC_CTRLB_RESSEL_16BIT;
      ADC->CTRLB = tCtlB;
      while( ADC->STATUS.bit.SYNCBUSY );
    }
    else
    {
      // set the error
      eError = ADC_ERR_ILLPRM;
    }
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
      if (( ptDef->eGain <= ADC_GAIN_16 ) || ( ptDef->eGain == ADC_GAIN_HALF ))
      {
        if ( ptDef->nSampLength <= MAX_SAMPLE_LEN )
        {
          // no error detected
          eError = ADC_ERR_NONE;
        }
      }
    }
  }

  // return the error
  return( eError );
}

/******************************************************************************
 * @function SetCommonParameters
 *
 * @brief set the common parameters
 *
 * This function will set the common parameters
 *
 * @param[in]   ptDef         pointer to the definition
 *
 *****************************************************************************/
static void SetCommonParameters( PADCDEF ptDef, ADC_CTRLB_Type* ptCtlB )
{
  ADC_INPUTCTRL_Type  tInpCtrl;

  // set the refernce/average/samples
  ADC->REFCTRL.bit.REFSEL = ptDef->eAdcRef;
  ADC->SAMPCTRL.reg = ptDef->nSampLength;

  // set up the input mux
  tInpCtrl.bit.MUXPOS = ptDef->ePosChan;
  tInpCtrl.bit.MUXNEG = ptDef->eNegChan;
  tInpCtrl.bit.INPUTSCAN = 0;
  tInpCtrl.bit.INPUTOFFSET = 0,
  tInpCtrl.bit.GAIN = ptDef->eGain;
  ADC->INPUTCTRL = tInpCtrl;
  while( ADC->STATUS.bit.SYNCBUSY );

  // set up  control B
  ptCtlB->bit.DIFFMODE = ptDef->eInpMode;
  ptCtlB->bit.LEFTADJ = OFF;
  ptCtlB->bit.FREERUN = OFF;
  ptCtlB->bit.CORREN = OFF;
  ptCtlB->bit.PRESCALER = ptDef->ePrescale;
}

/**@} EOF .c */
