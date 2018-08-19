/******************************************************************************
 * @file LwipNtpHandler.h
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
#ifndef _LWIPNTPHANDLER_H
#define _LWIPNTPHANDLER_H

// system includes ------------------------------------------------------------

// library includes -----------------------------------------------------------
#include "Types/Types.h"

// local includes -------------------------------------------------------------

// Macros and Defines ---------------------------------------------------------
/// define the number of events/execution rate for the background task
#define LWIP_NTP_EXEC_RATE            ( TASK_TIME_MINS( 720 ))
#define LWIP_NTP_NUM_EVENTS           ( 1 )

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------
/// define the configuration structure
typedef struct PACKED _LWIPNTPDEF
{
  U32UN   tIpAddr;
} LWIPNTPDEF, *PLWIPNTPDEF;
#define LWIPNTPDEF_SIZE   sizeof( LWIPNTPDEF )

// global parameter declarations -----------------------------------------------
extern  const LWIPNTPDEF  tLwipNtpDefDflts;

// global function prototypes --------------------------------------------------
extern  void  LwipNtpHandler_Initialize( void );
extern  void  LwipNtpHandler_UpdateConfiguration( void );
extern  void  LwipNtpHandler_GetNetworkTime( void );
extern  BOOL  LwipNptHandler_ProcessEvent( TASKARG xArg );

/**@} EOF LwipNtpHandler.h */

#endif  // _LWIPNTPHANDLER_H