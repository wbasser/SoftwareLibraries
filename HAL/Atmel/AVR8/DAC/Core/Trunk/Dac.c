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
  U8  nValue;
  
  // build the control value
  nValue = ( PGM_RDBYTE( tDacDef.bExtOutEnable ) == ON ) ? BIT( DAOE ) : 0;
  nValue |= ( PGM_RDBYTE( tDacDef.bLeftAdjust ) == ON ) ? BIT( DALA ) : 0;
  nValue |= BITS( PGM_RDBYTE( tDacDef.eTrigSel ), DATS0 );
  
  // set the value
  DACON = nValue;
  
  // check to see if we need to control analog reference
  #if ( DAC_CONTROL_REF == 1 )
  
  #endif // DAC_CONTROL_REF
  
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
  wOutputValue = ( wPercent * MAX_DAC_VALUE ) / 1000;

  // output the data
  DAC->DATA.reg = wOutputValue; 
   while( DAC->STATUS.bit.SYNCBUSY );
}

/**@} EOF Dac.c */
