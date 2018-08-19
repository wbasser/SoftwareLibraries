/******************************************************************************
 * @file GoertzelToneDetect.c
 *
 * @brief Goertzel tone detect implementation
 *
 * This file provides the implementation for the GOertzel Tone Detect algo
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
 * \addtogroup GoertzelToneDetect
 * @{
 *****************************************************************************/

// system includes ------------------------------------------------------------
#include <math.h>

// local includes -------------------------------------------------------------
#include "GoertzelToneDetect/GoertzelToneDetect.h"

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations ----------------------------------------------

// local parameter declarations -----------------------------------------------

// local function prototypes --------------------------------------------------

// constant parameter initializations -----------------------------------------

/******************************************************************************
 * @function GoertzelToneDetect_Initialize
 *
 * @brief initailization
 *
 * This function will perform any initializion necessary
 *
 *****************************************************************************/
void GoertzelToneDetect_Initialize( PGOERTZELCTL ptCtl )
{
  FLOAT fOmega;
  S32   lK;

  // clear the previous samples
  ptCtl->fPrvSample1 = ptCtl->fPrvSample2 = 0;

  // compute the parameters
  lK = ( S32 )( 0.5 + ( FLOAT )(( ptCtl->wSampleSize * ptCtl->fDesiredTone ) / ptCtl->fSampleRate ));
  fOmega = (( 2.0 * M_PI * lK ) / ( FLOAT )ptCtl->wSampleSize );
  ptCtl->fCoeef = 2.0 * cos( fOmega );

  // clear the current count
  ptCtl->wCurrentCount = ptCtl->wSampleSize;
}

/******************************************************************************
 * @function GoertzelToneDetect_ProcessSample
 *
 * @brief sample process
 *
 * This function take current sample and apply it to the algorithm
 *
 * @param[in]   ptCtl         pointer to the controls structure
 * @param[io]   pbToneDetect  pointer to the store the tone detect state
 *
 * @return    BOOL if processing is complete
 *
 *****************************************************************************/
BOOL GoertzelToneDetect_ProcessSample( PGOERTZELCTL ptCtl, PBOOL pbToneDetect )
{
  BOOL  bStatus = FALSE;

  // calculate the current sample
  ptCtl->fCurSample += ( ptCtl->fCoeef * ptCtl->fPrvSample1 ) - ptCtl->fPrvSample2;
  ptCtl->fPrvSample2 = ptCtl->fPrvSample1;
  ptCtl->fPrvSample1 = ptCtl->fCurSample;

  // adjust the count
  if ( --ptCtl->wCurrentCount == 0 )
  {
    // reset the count
    ptCtl->wCurrentCount = ptCtl->wSampleSize;

    // calculate the new magnitue
    ptCtl->fMagnitude = ( ptCtl->fPrvSample1 * ptCtl->fPrvSample1 ) + ( ptCtl->fPrvSample2 * ptCtl->fPrvSample2 ) - ( ptCtl->fPrvSample1 * ptCtl->fPrvSample2 * ptCtl->fCoeef );
    ptCtl->fMagnitude = sqrt( ptCtl->fMagnitude );
    ptCtl->fMagnitude /= 100.0;

    /// did we detect a tone
    *( pbToneDetect ) = ( ptCtl->fMagnitude >= ( ptCtl->fDesiredTone - 10 )) ? TRUE : FALSE;

    // set a good status
    bStatus = TRUE;
   }

  // return the status
  return( bStatus );
}
 
/**@} EOF GoertzelToneDetect.c */
