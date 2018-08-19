/******************************************************************************
 * @file GoertzelToneDetect.h
 *
 * @brief Goertzel tone detect declarations 
 *
 * This file provides the declarations for the GOertzel tone detect algo
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
 
// ensure only one instantiation
#ifndef _GOERTZELTONEDETECT_H
#define _GOERTZELTONEDETECT_H

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------

// library includes -----------------------------------------------------------
#include "Types/Types.h"

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------
/// define the control structure
typedef struct _GOERTZELCTL
{
  FLOAT   fSampleRate;
  FLOAT   fDesiredTone;
  U16     wSampleSize;
  U16     wCurrentCount;
  FLOAT   fCoeef;
  FLOAT   fMagnitude;
  FLOAT   fCurSample;
  FLOAT   fPrvSample1;
  FLOAT   fPrvSample2;
} GOERTZELCTL, *PGOERTZELCTL;
#define GOERTZELCTL_SIZE                    sizeof( GOERTZELCTL )

// global parameter declarations -----------------------------------------------

// global function prototypes --------------------------------------------------
extern  void  GoertzelToneDetect_Initialize( PGOERTZELCTL ptCtl );
extern  BOOL  GoertzelToneDetect_ProcessSample( PGOERTZELCTL ptCtl, PBOOL pbToneDetect );

/**@} EOF GoertzelToneDetect.h */

#endif  // _GOERTZELTONEDETECT_H