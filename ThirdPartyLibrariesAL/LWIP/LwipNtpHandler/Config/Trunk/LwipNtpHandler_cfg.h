/******************************************************************************
 * @file LwipNtpHandler_cfg.h
 *
 * @brief LWIP Network Time Protocol (NTP) handler declarations 
 *
 * This file provides the declarations for the LWIP NTP handler
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
 * \addtogroup LwipNtpHandler
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _LWIPNTPHANDLER_CFG_H
#define _LWIPNTPHANDLER_CFG_H

// system includes ------------------------------------------------------------

// library includes -----------------------------------------------------------
#include "Types/Types.h"

// local includes -------------------------------------------------------------

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations -----------------------------------------------

// global function prototypes --------------------------------------------------
extern  void  LwipNtpHandler_ReportNetTime( U32 uNetTime );

/**@} EOF LwipNtpHandler_cfg.h */

#endif  // _LWIPNTPHANDLER_CFG_H