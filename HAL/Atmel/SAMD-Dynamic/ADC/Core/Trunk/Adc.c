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
#include "SystemConfig/SystemConfig_prm.h"

// local includes -------------------------------------------------------------
#include "ADC/Adc.h"
#include "ADC/Adc_prm.h"

// library includes -----------------------------------------------------------
#include "Clock/Clock.h"
#include "PowerManager/PowerManager.h"

// Macros and Defines ---------------------------------------------------------
/// define the macro to map the control pointer
#define MAP_HANDLE_TO_POINTER( handle )   (( PADCCTL )handle )

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------
/// define the control structure
typedef struct _ADCCTL
{
  ADC_REFCTRL_Type    tRefCtrl;
  ADC_AVGCTRL_Type    tAvgCtrl;
  ADC_SAMPCTRL_Type   tSampCtrl;
  ADC_CTRLB_Type      tCtrlRegB;
  ADC_INPUTCTRL_Type  tInpCtrl;
  PVADCCALLBACK       pvCallback;
} ADCCTL, *PADCCTL;
#define ADCCTL_SIZE                       sizeof( ADCCTL )

// global parameter declarations ----------------------------------------------

// local parameter declarations -----------------------------------------------
static  VBOOL         bConvertInProgress;
static  U16           wCurrentResults;
static  PVADCCALLBACK pvCurCallback;

// local function prototypes --------------------------------------------------
static  void      SetCommonParameters( PADCCHANDEF ptChanDef, PADCCTL ptAdcCtl );

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
  U32 uTemp;

  // turn on the peripheral
  PowerManager_DisableEnablePeriphC( PM_APBCMASK_ADC, ON );

  // turn on the clock
  Clock_SetGenClock( ADC_CLKGEN_SELECT, CLOCK_SRC_OSC8M, 4, CLOCK_DIVSEL_NUMERIC );
  Clock_PeriphEnable( CLOCK_MUXID_ADC, ADC_CLKGEN_SELECT );

  // reset the ADC
  ADC->CTRLA.bit.SWRST = TRUE;
  while( ADC->STATUS.bit.SYNCBUSY );

  // enable it
  ADC->CTRLA.bit.ENABLE = ON;
  while( ADC->STATUS.bit.SYNCBUSY );

  // enable the interrupts
  NVIC_EnableIRQ( ADC_IRQn );

  // check to see if 1V bandgap should be enabled
  #if ( ADC_BANDGAP1V_ENABLE == ON )
    SYSCTRL->VREF.reg |= SYSCTRL_VREF_BGOUTEN;
  #endif

  // check to see if external reference
  #if ( ADC_EXTREF_ENABLE == ON )
    // configure the GPIO pins
    Gpio_Configure( ADC_EXTREF_PORT_DEF, ADC_EXTREF_PIN_DEF, GPIO_MODE_INPUT, OFF, GPIO_FUNCMUX_B, OFF );
  #endif

  // read the non-volatile linearity/biascal values
  uTemp = ADC_CALIB_BIAS_CAL(( *( PU32 )ADC_FUSES_BIASCAL_ADDR >> ADC_FUSES_BIASCAL_Pos )) | ADC_CALIB_LINEARITY_CAL(( *( PU64 )ADC_FUSES_LINEARITY_0_ADDR >> ADC_FUSES_LINEARITY_0_Pos ));
  ADC->CALIB.reg = uTemp;
}

/******************************************************************************
 * @function Adc_Create
 *
 * @brief create an ADC chaqnnel
 *
 * This function will check parameters and create a ADC channel
 *
 * @param[in]   ptChanDef     pointer to channel definition
 *
 * @return      the handle for the ADC channel
 *
 *****************************************************************************/
PADCHANDLE Adc_Create( PADCCHANDEF ptChanDef )
{
  PADCHANDLE  pvAdcHandle = NULL;
  PADCCTL     ptAdcNew;
  U8          nAdjRes;

  // check for valid parameters
  if (( ptChanDef->eInpMode < ADC_INPMODE_MAX ) && ( ptChanDef->ePosChan < ADC_POSCHAN_MAX ) && 
      ( ptChanDef->eNegChan < ADC_NEGCHAN_MAX ) && ( ptChanDef->eRef < ADC_REF_MAX ) && 
      ( ptChanDef->ePreScale < ADC_PRESCALE_MAX ) && ( ptChanDef->eNumSamps < ADC_NUMSAMP_MAX ) && 
      ( ptChanDef->eGain < ADC_GAIN_MAX ) && ( ptChanDef->eAdcMode < ADC_MODE_MAX ) && ( ptChanDef->eDecRes < ADC_DECRES_MAX ))
  {
    // allocate space for the control
    if (( ptAdcNew = malloc( ADCCTL_SIZE )) != NULL )
    {
      // set the handle
      pvAdcHandle = ( PADCHANDLE )ptAdcNew;

      // configure the GPIO pins
      Gpio_Configure( ptChanDef->eDevPort, ptChanDef->nAinPin, GPIO_MODE_INPUT, OFF, GPIO_FUNCMUX_B, OFF );

      // setup common parameters
      SetCommonParameters( ptChanDef, ptAdcNew );
      ptAdcNew->pvCallback = ptChanDef->pvCallback;

      // now determine the mode
      switch( ptChanDef->eAdcMode )
      {
        case ADC_MODE_NORMAL :
          // set the refernce/average/samples
          ptAdcNew->tAvgCtrl.reg = 0;

          // set up the result select
          ptAdcNew->tCtrlRegB.bit.RESSEL = 0;
          break;

        case ADC_MODE_ACCUM :
        case ADC_MODE_AVERAGE :
          // set the refernce/average/samples
          ptAdcNew->tAvgCtrl.reg = ptChanDef->eNumSamps;

          // if this is average mode
          if ( ptChanDef->eAdcMode == ADC_MODE_AVERAGE )
          {
            // now set the resolution adjust
            switch( ptChanDef->eNumSamps )
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
          }

          // now set the resolution
          ptAdcNew->tAvgCtrl.reg |= ADC_AVGCTRL_ADJRES( nAdjRes ); 

          // set up  control B
          ptAdcNew->tCtrlRegB.reg |= ADC_CTRLB_RESSEL_16BIT;
          break;

        case ADC_MODE_DECIMATE :
          break;

        default :
          break;
      }
    }
  }

  // return the handle
  return( pvAdcHandle );
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
ADCERRS Adc_ConvertChannel( PADCHANDLE pvHandle, PU16 pwResult )
{
  ADCERRS eError = ADC_ERR_NONE;
  PADCCTL ptAdcCtl;

  // map the handle and test for validity
  if (( ptAdcCtl = MAP_HANDLE_TO_POINTER( pvHandle )) != NULL )
  {
    // set the callback
    pvCurCallback = ptAdcCtl->pvCallback;

    // enable the interrupts
    ADC->INTENSET.bit.RESRDY = ON;

    // set the registers
    ADC->AVGCTRL.reg = ptAdcCtl->tAvgCtrl.reg;
    ADC->REFCTRL.reg = ptAdcCtl->tRefCtrl.reg;
    ADC->SAMPCTRL.reg = ptAdcCtl->tSampCtrl.reg;
    
    // set the input control/wait sync
    ADC->INPUTCTRL.reg = ptAdcCtl->tInpCtrl.reg;
    while( ADC->STATUS.bit.SYNCBUSY );

    // set control B 
    ADC->CTRLB.reg = ptAdcCtl->tCtrlRegB.reg;
    while( ADC->STATUS.bit.SYNCBUSY );

    // set the conversion flag true
    bConvertInProgress = TRUE;

    // start the conversion 
    ADC->SWTRIG.bit.START = ON;
    while( ADC->STATUS.bit.SYNCBUSY );

    //  now determine blocking/non-blocking
    if ( ptAdcCtl->pvCallback == NULL )
    {
      // wait till done
      while( bConvertInProgress );
  
      // get result
      *( pwResult ) = wCurrentResults;
    }
    else
    {
      // return blocking status
      eError = ADC_ERR_BLOCKING;
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
  wCurrentResults = ADC->RESULT.reg;

  // if callback do it
  if ( pvCurCallback != NULL )
  {
    // call it
    pvCurCallback( wCurrentResults );
  }

  // in all cases clear the convert in progress
  bConvertInProgress = FALSE;
}

/******************************************************************************
 * @function SetCommonParameters
 *
 * @brief set the common parameters
 *
 * This function will set the common parameters
 *
 * @param[in]   ptChanDefDef  pointer to the channel definition
 * @param[io]   ptAdcCtl      pointer to the control
 *
 *****************************************************************************/
static void SetCommonParameters( PADCCHANDEF ptChanDef, PADCCTL ptAdcCtl )
{
  // set the refernce/average/samples
  ptAdcCtl->tRefCtrl.bit.REFSEL = ptChanDef->eRef;
  ptAdcCtl->tSampCtrl.reg = ptChanDef->eNumSamps;

  // set up the input mux
  ptAdcCtl->tInpCtrl.bit.MUXPOS = ptChanDef->ePosChan;
  ptAdcCtl->tInpCtrl.bit.MUXNEG = ptChanDef->eNegChan;
  ptAdcCtl->tInpCtrl.bit.INPUTSCAN = 0;
  ptAdcCtl->tInpCtrl.bit.INPUTOFFSET = 0,
  ptAdcCtl->tInpCtrl.bit.GAIN = ptChanDef->eGain;

  // set up  control B
  ptAdcCtl->tCtrlRegB.bit.DIFFMODE = OFF;
  ptAdcCtl->tCtrlRegB.bit.LEFTADJ = OFF;
  ptAdcCtl->tCtrlRegB.bit.FREERUN = OFF;
  ptAdcCtl->tCtrlRegB.bit.CORREN = OFF;
  ptAdcCtl->tCtrlRegB.bit.PRESCALER = ptChanDef->ePreScale;
}

/**@} EOF .c */
