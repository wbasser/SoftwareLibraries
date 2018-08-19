/******************************************************************************
 * @file SpecialCommands.h
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
#ifndef _SPECIALCOMMANDS_H
#define _SPECIALCOMMANDS_H

// system includes ------------------------------------------------------------

// library includes -----------------------------------------------------------

// local includes -------------------------------------------------------------
#include "DALIMessageHandler/DALIMessageHandler_def.h"

// Macros and Defines ---------------------------------------------------------
/// define the write memory command offset
#define SPECIALCOMMANDS_WRITEMEMLOC_CMDOFS    ( 3 )

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations -----------------------------------------------
extern  const CODE CMDTBL	atScplCmds[ ];
 
// global function prototypes --------------------------------------------------
extern  void  SpecialCommands_Initialize( void );

/**@} EOF SpecialCommands.h */

#endif  // _SpecialCommands_H