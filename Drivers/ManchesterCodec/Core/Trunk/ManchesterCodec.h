/******************************************************************************
 * @file ManchesterCodec.h
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
#ifndef _MANCHESTERCODEC_H
#define _MANCHESTERCODEC_H

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "ManchesterCodec/ManchesterCodec_cfg.h"

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------

/// define the number of queue events
#define MANCHESTERCODEC_QUEUE_SIZE        ( 16 )

/// define the number of events
#define MANCHESTERCODEC_NUM_EVENTS        ( MANCHESTERCODEC_QUEUE_SIZE + 4 )

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations -----------------------------------------------

// global function prototypes --------------------------------------------------
extern  void  ManchesterCodec_Initialize( void );
extern  BOOL  ManchesterCodec_ProcessEvent( TASKARG xArg );
extern  void  ManchesterCodec_Xmit( PU8 pnData, U8 nLength, TASKSCHDENUMS eTaskEnum );
extern  void  ManchesterCodec_Recv( PU8 pnData, U8 nLength, TASKSCHDENUMS eTaskEnum );
extern  void  ManchesterCodec_StopRecv( void );
extern  void  ManchesterCodec_ProcessXmtTimer( void );

/**@} EOF ManchesterCodec.h */

#endif  // _MANCHESTERCODEC_H
