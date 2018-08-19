/******************************************************************************
 * @file LogHandler_def.h
 *
 * @brief log handler definitions
 *
 * This file provides the definitions for the log handler
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
 * \addtogroup LogHandler
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _LOGHANDLER_DEF_H
#define _LOGHANDLER_DEF_H

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "LogHandler/LogHandler_prm.h"

// library includes -----------------------------------------------------------
#include "Types/Types.h"

// Macros and Defines -------------------------{--------------------------------

/// define the helper macro for the initialization of the log entry
#define LOGENTRY_CREATE( title ) \
  { .szDescription = title, }

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------
/// define the log entry definition structure
typedef struct _LOGENTRY
{
  C8    szDescription[ LOGHANDLER_MAX_DESCR_LEN ];
} LOGENTRY, *PLOGENTRY;
#define LOGENTRY_SIZE   sizeof( LOGENTRY )

/**@} EOF LogHandler_def.h */

#endif  // _LOGHANDLER_DEF_H