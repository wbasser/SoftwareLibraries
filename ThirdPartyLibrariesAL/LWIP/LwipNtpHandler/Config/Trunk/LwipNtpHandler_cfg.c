/******************************************************************************
 * @file LwipNtpHandler_cfg.c
 *
 * @brief LWIP Network Time Protocol( NTP ) handler implementation 
 *
 * This file provides the implementation for the NTP protocol handler
 *
 * @copyright Copyright (c) 2012 Nan Technologies
 * This document contains proprietary data and information of Nan Technologies 
 * LLC. It is the exclusive property of Nan Technologies, LLC and will not be 
 * disclosed in any form to any party without prior written permission of 
 * Nan Technologies, LLC. This document may not be reproduced or further used 
 * without the prior written permission of Nan Technologies, LLC.
 *
 * $Date: $
 *
 * Version History
 * ======
 * $Rev: $
 * 
 *
 * \addtogroup LwipNptHandler
 * @{
 *****************************************************************************/

// system includes ------------------------------------------------------------

// library includes -----------------------------------------------------------

// local includes -------------------------------------------------------------
#include "LwipNtpHandler/LwipNtpHandler_cfg.h"

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations ----------------------------------------------

// local parameter declarations -----------------------------------------------

// local function prototypes --------------------------------------------------

// constant parameter initializations -----------------------------------------

/******************************************************************************
 * @function LwipNtpHandler_ReportNetTime
 *
 * @brief report the net time change
 *
 * This function will call the user defined function to set the new time
 *
 *****************************************************************************/
void LwipNtpHandler_ReportNetTime( U32 uNetTime )
{
  /// generate code to handle the new time in the application
}


/**@} EOF LwipNtpHandler_cfg.c */
