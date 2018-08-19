/******************************************************************************
 * @file CommandCommandsBin.h
 *
 * @brief binary common commands 
 *
 * This file provides the declarations of the binary common commands
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
 * \addtogroup CommandCommandsBin
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef BINCOMMONCOMMANDS_H
#define BINCOMMONCOMMANDS_H

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------

// library includes -----------------------------------------------------------
#include "BinaryCommandHandler/BinaryCommandHandler.h"

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------
/// enumerate the commands
typedef enum _BINCMNCMDS
{
  // normal commands
  BINCMN_CMD_GETPNG = 0x80,       ///< ping and resond
  BINCMN_CMD_GETREV,              ///< get the software revision
  BINCMN_CMD_SYSMON,              ///< system monitor
  BINCMN_CMD_RSTDFL,              ///< reset defaults
  BINCMN_CMD_LOGNUM,              ///< get the number of log entries
  BINCMD_CMD_LOGOLD,              ///< get the oldest log entry
  BINCMD_CMD_LOGNXT,              ///< get the next log entry
  BINCMD_CMD_LOGPRV,              ///< get the previous log entry
  BINCMD_CMD_LOGNEW,              ///< get the newest log entry
  BINCMD_CMD_LOGRST,              ///< reset the log entries
  BINCMD_CMD_BOOTEN,              ///< enter boot mode

  // do not remove the below entry
  BINCMN_CMD_MAX
} BINCMNCMDS;

// structures -----------------------------------------------------------------

// global parameter declarations -----------------------------------------------
extern const CODE BINCMDSLVENTRY atBinCommonCommandsTable[ ];

// global function prototypes --------------------------------------------------

/**@} EOF BinCommandCommands.h */

#endif  // BINCOMMONCOMMANDS_H