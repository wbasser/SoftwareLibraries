/******************************************************************************
 * @file ManchesterCodec_def.h
 *
 * @brief Manchester encoder/decoder definitions
 *
 * This file provides the definitions for the manchester encoder and decoder
 *
 * @copyright Copyright (c) 2012 CyberIntegration
 * This document contains proprietary data and information of CyberIntegration 
 * LLC. It is the exclusive property of CyberIntegration, LLC and will not be 
 * disclosed in any form to any party without prior written permission of 
 * CyberIntegration, LLC. This document may not be reproduced or further used 
 * without the prior written permission of CyberIntegration, LLC.
 *
 * Version History
 * ======
 * $Log: $
 * 
 *
 * \addtogroup ManchesterCodec
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _MANCHESTERCODEC_DEF_H
#define _MANCHESTERCODEC_DEF_H

// system includes ------------------------------------------------------------
#include "Types/Types.h"

// local includes -------------------------------------------------------------
#include "ManchesterCodec/ManchesterCodec_cfg.h"

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------
/// define the transmit/recieve done events
#if ( MANCHESTERCODEC_ARG_SIZE_BYTES == 1 )
  #define MANCHESTERCODEC_XMIT_DONE       ( 0xCC )
  #define MANCHESTERCODEC_RECV_EDGE       ( 0XCD )
  #define MANCHESTERCODEC_RECV_DONE       ( 0xCE )
  #define MANCHESTERCODEC_RECV_EROR       ( 0xCF )
#elif ( MANCHESTERCODEC_ARG_SIZE_BYTES == 2 )
  #define MANCHESTERCODEC_XMIT_DONE       ( 0xCCCC )
  #define MANCHESTERCODEC_RECV_EDGE       ( 0XCDCD )
  #define MANCHESTERCODEC_RECV_DONE       ( 0xCECE )
  #define MANCHESTERCODEC_RECV_EROR       ( 0xCFCF )
#elif ( MANCHESTERCODEC_ARG_SIZE_BYTES == 4 )
  #define MANCHESTERCODEC_XMIT_DONE       ( 0xCCCCCCCC )
  #define MANCHESTERCODEC_RECV_EDGE       ( 0XCDCDCDCD )
  #define MANCHESTERCODEC_RECV_DONE       ( 0xCECECECE )
  #define MANCHESTERCODEC_RECV_EROR       ( 0xCFCFCFCF )
#else
  #define MANCHESTERCODEC_XMIT_DONE       ( 0xCC )
  #define MANCHESTERCODEC_RECV_EDGE       ( 0XCD )
  #define MANCHESTERCODEC_RECV_DONE       ( 0xCE )
  #define MANCHESTERCODEC_RECV_EROR       ( 0xCF )
#endif // MANCHESTERCODEC_ARG_SIZE_BYTES

// enumerations ---------------------------------------------------------------
/// enumerate the rising/falling edge events
typedef enum _MANCHESTERCODECEDGE
{
  MANCHESTERCODEC_EDGE_FALL = 0,        ///< falling edge
  MANCHESTERCODEC_EDGE_RISE,            ///< rising edge
} MANCHESTERCODECEDGE;

// structures -----------------------------------------------------------------
/// define the initialization structure
typedef struct _MANCHESTERCODECDEF
{
  U8    nNumSyncBits;                     ///< number of sync bits
  U8    nNumStopBits;                     ///< number of stop bits
  U8    nBitTolerancePct;                 ///< bit tolerance percent
  U16   wBaudRate;                        ///< baud rate
  BOOL  bRisingEdgeEnable;                ///< rising edge enable
} MANCHESTERCODECDEF, *PMANCHESTERCODECDEF;
#define MANCHESTERCODECDEF_SIZE                 sizeof( MANCHESTERCODECDEF )

/**@} EOF ManchesterCodec_def.h */

#endif  // _MANCHESTERCODEC_DEF_H