/******************************************************************************
 * @file Dac.c
 *
 * @brief DAC implementation
 *
 * This file provides the implementation for DAC module
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
 * \addtogroup DAC
 * @{
 *****************************************************************************/

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "DAC/Dac.h"

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------
#define MAX_DAC_VALUE     ( 1024 )

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations ----------------------------------------------

// local parameter declarations -----------------------------------------------

// local function prototypes --------------------------------------------------

// constant parameter initializations -----------------------------------------

/******************************************************************************
 * @function Dac_Initialize
 *
 * @brief DAC initlization
 *
 * This function will initialize the DAC
 *
 *****************************************************************************/
void Dac_Initialize( void )
{
  DAC_CTRLB_Type  tCtl;

  // reset the DAC
  DAC->CTRLA.bit.SWRST = ON;
  while( DAC->STATUS.bit.SYNCBUSY );

  // setup the DAC
  tCtl.reg = 0;
  tCtl.bit.EOEN = tDacDef.bExtOutEnable;
  tCtl.bit.IOEN = tDacDef.bIntOutEnable;
  tCtl.bit.VPD = tDacDef.bVPumpDisable;
  tCtl.bit.REFSEL = tDacDef.eRefSelect;
  DAC->CTRLB = tCtl;

  // enable it
  DAC->CTRLA.bit.ENABLE = ON;
  while( DAC->STATUS.bit.SYNCBUSY );
}

/******************************************************************************
 * @function Dac_SetOutput
 *
 * @brief set the desired output to the DAC
 *
 * This function will test for a valid value and output the new value
 *
 * @param[in]   wValue    the desired DAC value
 *
 * @return        FALSE if value is in range, TRUE if error
 *****************************************************************************/
BOOL Dac_SetOutput( U16 wValue )
{
  BOOL bStatus = TRUE;

  // check for valid value
  if ( wValue < MAX_DAC_VALUE )
  {
    // output the data
    DAC->DATA.reg = wValue; 
    while( DAC->STATUS.bit.SYNCBUSY );

    // set good status
    bStatus = FALSE;
  }

  // return the status
  return( bStatus );
}

/******************************************************************************
 * @function Dac_SetPercentOutput
 *
 * @brief set the desired percentage output to the DAC
 *
 * This function will compute the output value based on percent
 *
 * @param[in]   wValue    the desired DAC value
 *
 *****************************************************************************/
void  Dac_SetPercentOutput( U16 wPercent )
{
  U16 wOutputValue;

  // compute the output value
  //wOutputValue = SCALE( wPercent, 1000, MAX_DAC_VALUE );
  wOutputValue = wPercent;

  // output the data
  DAC->DATA.reg = wOutputValue; 
  while( DAC->STATUS.bit.SYNCBUSY );
}

/**@} EOF Dac.c */
