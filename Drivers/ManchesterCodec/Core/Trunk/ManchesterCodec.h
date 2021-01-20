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

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations -----------------------------------------------

// global function prototypes --------------------------------------------------
#if ( MANCHESTERCODEC_ENALBE_DYNAMICINIT == OFF )
  extern  void  ManchesterCodec_Initialize( void );
#else
  extern  void  ManchesterCodec_Initialize( PMANCHESTERCODECDEF ptDef );
#endif // MANCHESTERCODEC_ENALBE_DYNAMICINIT
extern  void  ManchesterCodec_Xmit( PU8 pnData, U8 nLength );
extern  void  ManchesterCodec_Recv( PU8 pnData, U8 nLength );
extern  void  ManchesterCodec_StopRecv( void );
extern  void  ManchesterCodec_ProcessXmtTimer( void );
extern  void  ManchesterCodec_ProcessReceive( BOOL bTimeout, U16 wBitTime, MANCHESTERCODECEDGE eEdge );

/**@} EOF ManchesterCodec.h */

#endif  // _MANCHESTERCODEC_H
