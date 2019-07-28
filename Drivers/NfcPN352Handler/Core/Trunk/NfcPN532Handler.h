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
#include "NfcPN532Handler/NfcPN532Handler_cfg.h"

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------
/// define the maximum UID length
#define NFCPN532HANDLER_MAX_UID_LENGTH                          ( 7 )

// enumerations ---------------------------------------------------------------
/// enumerate the pre-defined authentication keys
typedef enum _NFCPN532AUTHKEY
{
  NFCPN532_AUTHKEY_ALLZEROS = 0,
  NFCPN532_AUTHKEY_A0SEQ,
  NFCPN532_AUTHKEY_A1SEQ,
  NFCPN532_AUTHKEY_B0SEQ,
  NFCPN532_AUTHKEY_AASEQ,
  NFCPN532_AUTHKEY_ALLONES,
  NFCPN532_AUTHKEY_MAX,
  NFCPN532_AUTHKEY_USRDEF = 0xFF,
} NFCPN532AUTHKEY;

// structures -----------------------------------------------------------------
/// define the callback event
typedef void  ( *PVMSGCALLBACK )( BOOL );

// global parameter declarations -----------------------------------------------

// global function prototypes --------------------------------------------------
extern  void  NfcPN532Handler_Initialize( void );
extern  void  NfcPN532Handler_ProcessEvent( U16 wEvent );
extern  void  NfcPN532Handler_RequestFirwareRevision( PVMSGCALLBACK pvCallback );
extern  U32   NfcPN532Handler_GetFirmwareRevision( void ); 
extern  void  NfcPN532Handler_ConfigureSecureAccessModule( PVMSGCALLBACK pvCallback );
extern  void  NfcPN532Handler_ScanForCards( U8 nBaudrate, U16 wTimeoutMsecs, PVMSGCALLBACK pvCallback ); 
extern  void  NfcPN532Handler_GetLastUID( PU8 pnUid, PU8 pnLength );
extern  void  NfcPN532Handler_AuthenticateBlock( PU8 pnUid, U8 nUidLength, U32 uBblockNumber, U8 nKeyNumber, NFCPN532AUTHKEY eAuthKey, PU8 pnKeyData, PVMSGCALLBACK pvCallback );

/**@} EOF NfcPN532Handler.h */

#endif  // _NFCPN532HANDLER_H