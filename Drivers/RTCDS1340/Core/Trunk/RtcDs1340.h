/******************************************************************************
 * @file RtcDs1340.h
 *
 * @brief RTC 1340 declarations 
 *
 * This file provides the declarations for the DS1340 Real TIme Clock
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
 * \addtogroup RtcDs1340
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _RTCDS1340_H
#define _RTCDS1340_H

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------

// library includes -----------------------------------------------------------
#include "TimeHandler/TimeHandler.h"

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------
/// enumerate the errors
typedef enum _RTCDS1340ERR
{
  RTCDS1340_ERR_NONE = 0,       ///< no error
  RTCDS1340_ERR_XFR   = 0xB0,  ///< transfer error
} RTCDS1340ERR;

// structures -----------------------------------------------------------------

// global parameter declarations -----------------------------------------------

// global function prototypes --------------------------------------------------
extern	void          RtcDs1340_Initialize( void );
extern	RTCDS1340ERR  RtcDs1340_SetDateTime( PDATETIME ptDateTime );
extern	RTCDS1340ERR  RtcDs1340_GetDateTime( PDATETIME ptDateTime );
extern	U64	          RtcDs1340_GetDateTimeHuge( void );

/**@} EOF RtcDs1340.h */

#endif  // _RTCDS1340_H