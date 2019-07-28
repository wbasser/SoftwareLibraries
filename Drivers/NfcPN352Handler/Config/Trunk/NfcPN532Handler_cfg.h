/******************************************************************************
 * @file NfcPN532Handler_cfg.c
 *
 * @brief NFC PN532 confgiuration declarations
 *
 * This file provides the declarations for the PN532 confgiuration file
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
 * \addtogroup NfcPN532Handler_cfg
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _NFCPN532HANDLER_CFG_H
#define _NFCPN532HANDLER_CFG_H

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "Types/Types.h"

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations -----------------------------------------------

// global function prototypes --------------------------------------------------
extern  void  NfcPN532Handler_LocalInitialize( void );
extern  void  NfcPN532Handler_I2CWrite( PU8 pnBuffer, U8 nLength );
extern  void  NfcPN532Handler_I2CRead( PU8 pnBuffer, U8 nLength );
extern  void  NfcPN532Handler_StartTaskTimer( U16 wTimeMsecs );
extern  void  NfcPN532Handler_PostEvent( U16 wEvent );
extern  void  NfcPN532Handler_SetReset( BOOL bState );
extern  void  NfcPN532Handler_DelayMSEC( U16 wDelayMsecs );

/**@} EOF NfcPN532Handler_cfg.h */

#endif  // _NFCPN532HANDLER_CFG_H