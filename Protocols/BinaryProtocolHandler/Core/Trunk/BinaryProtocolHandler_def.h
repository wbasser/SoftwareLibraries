/******************************************************************************
 * @file BinaryProtocolHandler_def.h
 *
 * @brief Binary Protocol Handler definition file 
 *
 * This file provides the binary protocol handler definition file
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
 * \addtogroup BinaryProtocolHandler
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _BINARYPROTOCOLHANDLER_DEF_H
#define _BINARYPROTOCOLHANDLER_DEF_H

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------

// library includes -----------------------------------------------------------
#include "Types/Types.h"

// Macros and Defines ---------------------------------------------------------
/// define the helper macro to create a protocol entry
#define BINPROT_DEFM( multidrop, seqenable, chkmode, devaddr, xmtsize, rcvsize, writefunc ) \
  { \
    .bMulDropMode     = multidrop, \
    .bSequenceEnable  = seqenable, \
    .eCheckMode       = chkmode, \
    .nDevAddress      = devaddr, \
    .wXmtBufferSize   = xmtsize, \
    .wRcvBufferSize   = rcvsize, \
    .pvWriteFunc      = ( PVBINWRITEFUNC )writefunc, \
  }

// enumerations ---------------------------------------------------------------
/// enumerate the check mode
typedef enum _BINPROTCHKMODE
{
  BINPROT_CHKMODE_EOR = 0,      ///< exclusive OR
  BINPROT_CHKMODE_CSM,          ///< 2s complement checksum
  BINPROT_CHKMODE_CRC,          ///< CRC-16 
  BINPROT_CHKMODE_MAX
} BINPROTCHKMODE;

// structures -----------------------------------------------------------------
/// define the write function prototype
typedef void  ( *PVBINWRITEFUNC )( PU8, U16 );

/// define the control structure
typedef struct _BINPROTDEF
{
  BOOL                    bMultiDropMode;   ///< multi drop mode
  BOOL                    bSequenceEnable;  ///< enable the sequence numbers
  BINPROTCHKMODE          eCheckMode;       ///< check mode
  U8                      nDevAddress;      ///< local device address for multidrop modes
  U16                     wXmtBufferSize;   ///< transmit buffer size
  U16                     wRcvBufferSize;   ///< receive buffer size
  PVBINWRITEFUNC          pvWriteFunc;      ///< pointer to a write char function
} BINPROTDEF, *PBINPROTDEF;
#define BINPROTDEF_SIZE   sizeof( BINPROTDEF )

/**@} EOF BinaryProtocolHandler_def.h */

#endif  // _BINARYPROTOCOLHANDLER_DEF_H