/******************************************************************************
 * @file FilterAveraging.h
 *
 * @brief Filter averaing declarations
 *
 * This file provides the declaration for the average filter implementation
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
 
// ensure only one instantiation
#ifndef _FILTERAVERAGING_H
#define _FILTERAVERAGING_H

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------

// library includes -----------------------------------------------------------
#include "Types/Types.h"

// Macros and Defines ---------------------------------------------------------
/// define the helper macro for defining the average filter buffer
#define SENMANAVGFILTSTORAGE( bufname, size ) \
  static  U16 aw ## bufname ## AvgFilt[ size ];

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------
/// define the average filter definition structure
typedef struct _FILTAVERAGEDEF
{
  PU16  pwBuffer;         ///< pointer to the buffer
  U8    nNumberSamples;   ///< number of samples
} FILTAVERAGEDEF, *PFILTAVERAGEDEF;
#define FILTAVERAGEDEF_SIZE   sizeof( FILTAVERAGEDEF )

/// define the average filter control structure
typedef struct _FILTAVERAGECTL
{
  FILTAVERAGEDEF  tDef;           ///< definition
  U8              nSampleIndex;   ///< sample index
  U32             uSum;           ///< sum of all samples
  BOOL            bValid;         ///< valid
} FILTAVERAGECTL, *PFILTAVERAGECTL;
#define FILTAVERAGECTL_SIZE   sizeof( FILTAVERAGECTL )

// global parameter declarations -----------------------------------------------

// global function prototypes --------------------------------------------------
extern  BOOL  FilterAveraging_Execute( PFILTAVERAGECTL ptCtl, U16 wNewSample );
extern  U16   FilterAveraging_GetValue( PFILTAVERAGECTL ptCtl );

/**@} EOF FilterAveraging.h */

#endif  // _FILTERAVERAGING_H