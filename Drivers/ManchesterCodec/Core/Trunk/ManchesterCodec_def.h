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

// local includes -------------------------------------------------------------

// library includes -----------------------------------------------------------
#include "TaskManager/TaskManager.h"

// Macros and Defines ---------------------------------------------------------
/// define the transmit/recieve done events
#if ( TASK_TSKARG_SIZE_BYTES == 1 )
  #define MANCHESTERCODEC_XMIT_DONE       ( 0xCC )
  #define MANCHESTERCODEC_RECV_EDGE       ( 0XCD )
  #define MANCHESTERCODEC_RECV_DONE       ( 0xCE )
  #define MANCHESTERCODEC_RECV_EROR       ( 0xCF )
#elif ( TASK_TSKARG_SIZE_BYTES == 2 )
  #define MANCHESTERCODEC_XMIT_DONE       ( 0xCCCC )
  #define MANCHESTERCODEC_RECV_EDGE       ( 0XCDCD )
  #define MANCHESTERCODEC_RECV_DONE       ( 0xCECE )
  #define MANCHESTERCODEC_RECV_EROR       ( 0xCFCF )
#elif ( TASK_TSKARG_SIZE_BYTES == 4 )
  #define MANCHESTERCODEC_XMIT_DONE       ( 0xCCCCCCCC )
  #define MANCHESTERCODEC_RECV_EDGE       ( 0XCDCDCDCD )
  #define MANCHESTERCODEC_RECV_DONE       ( 0xCECECECE )
  #define MANCHESTERCODEC_RECV_EROR       ( 0xCFCFCFCF )
#else
  #define MANCHESTERCODEC_XMIT_DONE       ( 0xCC )
  #define MANCHESTERCODEC_RECV_EDGE       ( 0XCD )
  #define MANCHESTERCODEC_RECV_DONE       ( 0xCE )
  #define MANCHESTERCODEC_RECV_EROR       ( 0xCF )
#endif // TASK_TASKARG_SIZE_BYTES

// enumerations ---------------------------------------------------------------
/// enumerate the rising/falling edge events
typedef enum _MANCHESTERCODECEDGE
{
  MANCHESTERCODEC_EDGE_FALL = 0,        ///< falling edge
  MANCHESTERCODEC_EDGE_RISE,            ///< rising edge
} MANCHESTERCODECEDGE;

// structures -----------------------------------------------------------------
typedef struct _MANCHESTERCODECDATA
{
  MANCHESTERCODECEDGE eEdge;     ///< edge
  U16                 wValue;    ///< captured value
} MANCHESTERCCODECDATA, *PMANCHESTERCODECDATA;
#define MANCHESTERCODECDATA_SIZE   sizeof( MANCHESTERCCODECDATA )

/**@} EOF ManchesterCodec_def.h */

#endif  // _MANCHESTERCODEC_DEF_H