/******************************************************************************
 * @file ExtendedCommands.h
 *
 * @brief 
 *
 * This file 
 *
 * @copyright Copyright (c) 2012 Nan Technologies
 * This document contains proprietary data and information of Nan Technologies 
 * LLC. It is the exclusive property of Nan Technologies, LLC and will not be 
 * disclosed in any form to any party without prior written permission of 
 * Nan Technologies, LLC. This document may not be reproduced or further used 
 * without the prior written permission of Nan Technologies, LLC.
 *
 * Version History
 * ======
 * $Rev: $
 * 
 *
 * \addtogroup DALIMessageHandler
 *
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _EXTENDEDCOMMANDS_H
#define _EXTENDEDCOMMANDS_H

// system includes ------------------------------------------------------------

// library includes -----------------------------------------------------------

// local includes -------------------------------------------------------------
#include "DALIMessageHandler/DALIMessageHandler_def.h"

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations -----------------------------------------------
extern  const CODE CMDTBL	atExtnCmds[ ];
 
// global function prototypes --------------------------------------------------
void  ExtendedCommands_Initialize( void );

/**@} EOF ExtendedCommands.h */

#endif  // _EXTENDEDCOMMANDS_H