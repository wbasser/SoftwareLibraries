/******************************************************************************
 * @file ManchesterCodec_cfg.h
 *
 * @brief Manchester Codec configuration declarations
 *
 * This file provides the declarations for the configuration of the
 * Manchester Codec
 *
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
 * $Rev: $
 * 
 *
 * \addtogroup ManchesterCodec
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _MANCHESTERCODEC_CFG_H
#define _MANCHESTERCODEC_CFG_H

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "ManchesterCodec/ManchesterCodec_def.h"

// library includes -----------------------------------------------------------
#include "Timers/Timers.h"

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations -----------------------------------------------

// global function prototypes --------------------------------------------------
extern  void  ManchesterCodec_LocalInitialize( U32 uHalfBitTime );
extern  U32   ManchesterCodec_GetClockFreq( void );
extern  void  ManchesterCodec_TransmitCallback( U8 nEvent, U8 nChan,  U16 wValue );
extern  void  ManchesterCodec_ReceiveCallback( U8 nEvent, U8 nChan,  U16 wValue );
extern  void  ManchesterCodec_RecvTimerControl( BOOL bState );
extern  void  ManchesterCodec_FlushRecvEvents( void );
extern  void  ManchesterCodec_OutputControl( BOOL bCurBit );
extern  void  ManchesterCodec_PostXmitEvent( U32 uEvent );
extern  void  ManchesterCodec_PostRecvEvent( U32 uEvent );

/**@} EOF ManchesterCodec_cfg.h */

#endif  // _MANCHESTERCODEC_CFG_H