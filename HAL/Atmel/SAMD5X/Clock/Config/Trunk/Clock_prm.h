/******************************************************************************
 * @file Clock_prm.h
 *
 * @brief CLock parameter declarations 
 *
 * This file provides the declarations for the configuration of the CLock module
 *
 * @copyright Copyright (c) 2012 Guardhat
 * This document contains proprietary data and information of Guardhat 
 * LLC. It is the exclusive property of Guardhat, LLC and will not be 
 * disclosed in any form to any party without prior written permission of 
 * Guardhat, LLC. This document may not be reproduced or further used 
 * without the prior written permission of Guardhat, LLC.
 *
 * Version History
 * ======
 * $Rev: $
 * 
 *
 * \addtogroup Clock
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _CLOCK_PRM_H
#define _CLOCK_PRM_H

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------
/// define the external crystal frequency
#define CLOCK_EXT_CRYSTAL_FREQ              ( 8000000 )

/**@} EOF Clock_prm.h */

#endif  // _CLOCK_PRM_H