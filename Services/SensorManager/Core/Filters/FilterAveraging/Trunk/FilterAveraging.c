/******************************************************************************
 * @file FilterAveraging.c
 *
 * @brief 
 *
 * This file 
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
 * \addtogroup FilterAveraging
 * @{
 *****************************************************************************/

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "SensorManager/Filters/FilterAveraging/FilterAveraging.h"

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations ----------------------------------------------

// local parameter declarations -----------------------------------------------

// local function prototypes --------------------------------------------------

// constant parameter initializations -----------------------------------------

/******************************************************************************
 * @function FilterAveraging_Execute
 *
 * @brief averaging filter execution
 *
 * This function will perform an average filter iteration 
 *
 * @param[in]   ptCtl       pointer to a control structure
 * @oaran[in]   wNewSample  new sample to add
 *
 * @return      TRUE if valid filter value, FALSE if not
 *
 *****************************************************************************/
BOOL FilterAveraging_Execute( PFILTAVERAGECTL ptCtl, U16 wNewSample )
{
  BOOL bValidValue;

  // set the return status 
  bValidValue = ptCtl->bValid;

  // remove the oldest value
  ptCtl->uSum -= *( ptCtl->tDef.pwBuffer + ptCtl->nSampleIndex );

  // add the new value in
  *( ptCtl->tDef.pwBuffer + ptCtl->nSampleIndex ) = wNewSample;
  ptCtl->uSum += wNewSample;

  // adjust the index
  ptCtl->nSampleIndex++;

  // check for overflow
  if ( ptCtl->nSampleIndex >= ptCtl->tDef.nNumberSamples )
  {
    // set the local valid bi
    ptCtl->bValid = TRUE;
    
    // adjust the index
    ptCtl->nSampleIndex = 0; 
  }
  
  // return the valididty
  return( bValidValue );
}

/******************************************************************************
 * @function FilterAveraging_GetValue
 *
 * @brief get the averaged value
 *
 * This function will get the averaged value
 *
 * @param[in]   ptCtl       pointer to a control structure
 *
 * @return      current filtered value
 *
 *****************************************************************************/
U16 FilterAveraging_GetValue( PFILTAVERAGECTL ptCtl )
{
  U16 wValue;

  // get the new value
  wValue = ptCtl->uSum / ptCtl->tDef.nNumberSamples;

  // return the value
  return( wValue );
}

/**@} EOF FilterAveraging.c */
