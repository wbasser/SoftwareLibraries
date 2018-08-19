/******************************************************************************
 * @file NfcPN532Handler.h
 *
 * @brief NFC PN532 handler declarations
 *
 * This file provides the declaratons for the PN532 NFC chip
 *
 * @copyright Copyright (c) 2012 Cyber Intergration
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
 * \addtogroup NfcPN532Handler
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _NFCPN532HANDLER_H
#define _NFCPN532HANDLER_H

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------

// library includes -----------------------------------------------------------
#include "TaskManager/TaskManager.h"

// Macros and Defines ---------------------------------------------------------
/// define the number of events for the communications task
#define NFCPN532HANDLER_NUM_EVENTS                    ( 4 )

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------
/// define the callback event
typedef void  ( *PVMSGCALLBACK )( BOOL );

// global parameter declarations -----------------------------------------------

// global function prototypes --------------------------------------------------
extern  void  NfcPN532Handler_Initialize( void );
extern  BOOL  NfcPN532Handler_CommunicationsTask( TASKARG xArg );
extern  void  NfcPN352Handler_RequestFirwareRevision( PVMSGCALLBACK pvCallback );
extern  void  NfcPN352Handler_GetFirmwareRevision( PU8 

/**@} EOF NfcPN532Handler.h */

#endif  // _NFCPN532HANDLER_H