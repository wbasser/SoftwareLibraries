/******************************************************************************
 * @file ManchesterCodec_prm.h
 *
 * @brief 
 *
 * This file 
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
#ifndef _MANCHESTERCODEC_PRM_H
#define _MANCHESTERCODEC_PRM_H

// local includes -------------------------------------------------------------

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------
/// define the debug enable
#define MANCHESTERCODEC_RCVDBG_ENABLE         ( 0 )

/// define the transmit debug enable
#define MANCHESTERCODEC_XMTDBG_ENABLE         ( 0 )

/// define the base value for transmit debug
#define MANCHESTERCODEC_DBGXMT_BASE           ( 0x4000 )

/// define the base value for receive debug
#define MANCHESTERCODEC_DBGRCV_BASE           ( 0x5000 )

/**@} EOF ManchesterCodec_prm.h */

#endif  // _MANCHESTERCODEC_PRM_H
