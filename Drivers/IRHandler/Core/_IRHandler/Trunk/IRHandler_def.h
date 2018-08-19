/******************************************************************************
 * @file IRHandler_def.h
 *
 * @brief IR handler definition declarations 
 *
 * This file provides the definitions for the structures and enumerations used
 * to define IR handler configurations
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
 * \addtogroup IRHandler
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _IRHANDLER_DEF_H
#define _IRHANDLER_DEF_H

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "IRHandler/IRHandler_prm.h"

// library includes -----------------------------------------------------------
#include "TaskManager/TaskManager.h"
#include "QueueManager/QueueManager.h"

// Macros and Defines ---------------------------------------------------------
/// define the helper macro for creating callback code entries
#define IRHAND_DEFCB_ENTRY( code, callback ) \
  { \
    .eRptMethod = IRHAND_RPTMETHOD_CB, \
    .wCode = code, \
    .pvCallback = callback, \
  }
  
/// define the helper macro for creaing event code entries  
#define IRHAND_DEFEVENT_ENTRY( code, task ) \
  { \
    .eRptMethod = IRHAND_RPTMETHOD_EVENT, \
    .wCode = code, \
    .eTaskEnum = task, \
  }
  
// enumerations ---------------------------------------------------------------
/// enumerate the report methods
typedef enum _IRHANDRPTMETHOD
{
  IRHAND_RPTMETHOD_CB = 0,        ///< use callback to report commands
  IRHAND_RPTMETHOD_EVENT,         ///< use event posting
} IRHANDRPTMETHOD;

/// enumerate the indices for the IR capture
typedef enum _IRCAPINDEX
{
  IRCAP_INDEX_ONTIME = LE_U32_LSW_IDX,
  IRCAP_INDEX_OFFTIME = LE_U32_MSW_IDX
} IRCAPINDEX;

// structures -----------------------------------------------------------------
/// define the callback function declaration
typedef void  ( *PVIRHANDCALLBACK )( U8 );

/// define the structure for defining IR codes
typedef struct _IRHANDCMDDEF
{
  IRHANDRPTMETHOD   eRptMethod;   ///< reporting method
  U16               wCode;        ///< code
  TASKSCHDENUMS     eTaskEnum;    ///< task enumeration
  PVIRHANDCALLBACK  pvCallback;   ///< callback
} IRHANDCMDDEF, *PIRHANDCMDDEF;
#define IRHANDCMDDEF_SIZE        sizeof( IRHANDCMDDEF )

/**@} EOF IRHandler_def.h */

#endif  // _IRHANDLER_DEF_H