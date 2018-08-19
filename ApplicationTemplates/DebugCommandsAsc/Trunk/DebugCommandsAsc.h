/******************************************************************************
 * @file DebugCommandsAsc.h
 *
 * @brief Debug command Handler
 *
 * This file 
 *
 * @copyright Copyright (c) 2012CyberIntegration
 * This document contains proprietary data and information of Cyber
 * LLC. It is the exclusive property ofCyberIntegration, LLC and will not be
 * disclosed in any form to any party without prior written permission of
 *CyberIntegration, LLC. This document may not be reproduced or further used
 * without the prior written permission ofCyberIntegration, LLC.
 *
 * Version History
 * ======
 * $Log: $
 * 
 *
 * \addtogroup DebugCommandsAsc
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _DEBUGCOMMANDSASC_H
#define _DEBUGCOMMANDSASC_H

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------

// library includes -----------------------------------------------------------
#include "CommandHandlers/ASCII/AsciiCommandHandler.h"
#include "TaskManager/TaskManager.h"

// Macros and Defines ---------------------------------------------------------
/// define the RX/TX buffer sizes
#define DEBUGCOMMANDSASC_RCVBUF_SIZE      ( 0 )
#define DEBUGCOMMANDSASC_XMTBUF_SIZE      ( 80 )

/// define the number of events/arguments
#define DEBUGCOMMANDSASC_CMDBUF_SIZE      ( 160 )
#define DEBUGCOMMANDSASC_NUM_ARGS         ( 8 ) 

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations -----------------------------------------------
extern  const CODE ASCCMDENTRY atDebugCommandsAscTable[ ];

// global function prototypes --------------------------------------------------
extern  void  DebugCommandsAsc_Initialize( void );
extern  BOOL  DebugCommandsAsc_ProcessEvent( TASKARG xArg );
extern  void  DebugCommandsAsc_PutChar( U8 nChar );
extern  void  DebugCommandsAsc_Write( PC8 pcData, U16 wLength );

/**@} EOF DebugCommandsAsc.h */

#endif  // _DEBUGCOMMANDSASC_H