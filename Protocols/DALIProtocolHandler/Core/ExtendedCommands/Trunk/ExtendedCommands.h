/******************************************************************************
 * @file ExtendedCommands.h
 *
 * @brief 
 *
 * This file 
 *
 * @copyright Copyright (c) 2012 Cyber Integration
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
 * \addtogroup DALIProtocolHandler
 *
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _EXTENDEDCOMMANDS_H
#define _EXTENDEDCOMMANDS_H

// system includes ------------------------------------------------------------

// library includes -----------------------------------------------------------

// local includes -------------------------------------------------------------
#include "DALIProtocolHandler/DALIProtocolHandler_def.h"

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations -----------------------------------------------
extern  const CODE CMDTBL	atExtnCmds[ ];
 
// global function prototypes --------------------------------------------------
void  ExtendedCommands_Initialize( void );

/**@} EOF ExtendedCommands.h */

#endif  // _EXTENDEDCOMMANDS_H