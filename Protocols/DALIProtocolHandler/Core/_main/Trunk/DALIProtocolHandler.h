/****************************************7**************************************
 * @file  DALIProtocolHandler.h
 *
 * @brief  DALI message handler declarations
 *
 * This file provides the declarations for the DALI message handler
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
 * \addtogroup  DALIProtocolHandler
 * @{
 *****************************************************************************/

// ensure only one instantiation
#ifndef _DALIPROTOCOLHANDLER_H
#define _DALIPROTOCOLHANDLER_H

// system includes ------------------------------------------------------------

// library includes -----------------------------------------------------------

// local includes -------------------------------------------------------------

// Macros and Defines ---------------------------------------------------------

/// define the timer execution rate
#define	DALIMESSAGE_TIMER_EXEC_RATE		( TASK_TIME_MINS( 15 ))
#define DALIMESSAGE_OUTPUT_EXEC_RATE  ( TASK_TIME_MSECS( 1 ))
#define DALIMESSAGE_BUSFAIL_EXEC_RATE ( TASK_TIME_MSECS( 5 ))

// enumerations ---------------------------------------------------------------
/// enumerate the process event return values
typedef enum _DALIPROTEVNSTS
{
  DALIPROT_EVNSTS_NONE = 0,         ///< do nothing
  DALIPROT_EVNSTS_XMTMSG,           ///< transmit message
  DALIPROT_EVNSTS_ENBRCV,           ///< enable receive
  DALIPROT_EVNSTS_REQ100MSTIMER,    ///< request a 100MS timer
  DALIPROT_EVNSTS_REQ200MSTIMER,    ///< request a 200MS timer
  DALIPROT_EVNSTS_REQ100MSTIMER,    ///< request a 100MS timer
  DALIPROT_EVNSTS_REQ200MSTIMER,    ///< request a 200MS timer
  DALIPROT_EVNSTS_CLEARALLTIMERS,   ///< clear all timers
} DALIPROTEVNSTS;

// structures -----------------------------------------------------------------

// global parameter declarations -----------------------------------------------
extern  PDALICTL  g_ptDaliCtl;  ///< pointer to the control structure

// global function prototypes --------------------------------------------------
extern  void            DALIProtocolHandler_Initialize( PDALICTL ptDaliCtl );
extern	void            DALIProtocolHandler_Control( PDALICTL ptDaliCtl, BOOL bState );
extern  DALIPROTEVNSTS  DALIProtocolHandler_ProcessEvent( PDALICTL ptDaliCtl, TASKARG xArg );
extern  void            DALIProtocolHandler_ProcessTimer( PDALICTL ptDaliCtl );
extern	void            DALIProtocolHandler_ProcessOutput( PDALICTL ptDaliCtl, U32 uTimeIncrement );

/**@} EOF DALIProtocolHandler.h */

#endif  // _DALIPROTOCOLHANDLER_H
