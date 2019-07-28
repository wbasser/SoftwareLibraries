/******************************************************************************
 * @file BinaryCommandHandler_def.h
 *
 * @brief binary command handler definitions
 *
 * This file provides the definitions for the configuration enumeration and
 * structures
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
 * $Log: $
 * 
 *
 * \addtogroup BinaryCommandHandler
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _BINARYCOMMANDHANDLER_DEF_H
#define _BINARYCOMMANDHANDLER_DEF_H

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "BinaryCommandHandler/BinaryCommandHandler_prm.h"

// library includes -----------------------------------------------------------
#include "Types/Types.h"

// Macros and Defines ---------------------------------------------------------
/// define the special characters
#define BINCMD_CHACK                  ( 0x06 )
#define BINCMD_CHNAK                  ( 0x15 )

/// define the helper macro for creating storage for a single buffer
#define BINCMD_SNGBUF( name, bufsize ) \
  static U8 anBuf ## name[ bufsize ];
    
/// deifne the helper macro for creating storage for a dual buffer
#define BINCMD_DBLBUF( name, xmtsize, rcvsize ) \
  static  U8  anXmt ## name[ xmtsize ]; \
  static  U8  anRcv ## name[ rcvsize ];

/// define the helpmacro for creating storage for a master/slave device
#define BINCMD_MSVBUF( name, mstsize, slvsize ) \
  static  U8  anMst ## name[ mstsize ]; \
  static  U8  anSlv ## name[ slvsize ];

/// define the helper macro for creating a slave entry/single buffer/point to point
#define BINCMD_SLV_DEFSBPP( name, bufsize, chkmode, seqenb, writefunc, cmdtbl ) \
  { \
    .bMasterMode = FALSE, \
    .bMultiDropMode = FALSE, \
    .bDualBufferMode = FALSE, \
    .eCheckMode = chkmode, \
    .bSequenceEnable = seqenb, \
    .wRcvBufferSize = bufsize, \
    .pnRcvBuffer = ( PU8 )&anBuf ## name, \
    .pvWriteFunc = writefunc, \
    .ptSlvCmdTbl = ( PVOID )cmdtbl, \
  }

/// define the helper macro for creating a slave entry/dual buffer/point to point
#define BINCMD_SLV_DEFDBPP( name, xmtsize, rcvsize, chkmode, seqenb, writefunc, cmdtbl ) \
  { \
    .bMasterMode = FALSE, \
    .bMultiDropMode = FALSE, \
    .bDualBufferMode = TRUE, \
    .eCheckMode = chkmode, \
    .bSequenceEnable = seqenb, \
    .wXmtBufferSize = xmtsize, \
    .wRcvBufferSize = rcvsize, \
    .pnXmtBuffer = ( PU8 )&anXmt ## name, \
    .pnRcvBuffer = ( PU8 )&anRcv ## name, \
    .pvWriteFunc = writefunc, \
    .ptSlvCmdTbl = ( PVOID )cmdtbl, \
  }

/// define the helper macro for creating a slave entry/single buffer/multidrop
#define BINCMD_SLV_DEFSBMD( name, bufsize, chkmode, seqenb, devadr, writefunc, cmdtbl ) \
  { \
    .bMasterMode = FALSE, \
    .bMultiDropMode = TRUE, \
    .bDualBufferMode = FALSE, \
    .eCheckMode = chkmode, \
    .bSequenceEnable = seqenb, \
    .nDevAddress = devadr, \
    .wRcvBufferSize = bufsize, \
    .pnRcvBuffer = ( PU8 )&anBuf ## name, \
    .pvWriteFunc = writefunc, \
    .ptSlvCmdTbl = ( PVOID )cmdtbl, \
  }

/// define the helper macro for creating a slave entry/dual buffer/multidrop
#define BINCMD_SLV_DEFDBMD( name, xmtsize, rcvsize, chkmode, seqenb, devadr, writefunc, cmdtbl ) \
  { \
    .bMasterMode = FALSE, \
    .bMultiDropMode = TRUE, \
    .bDualBufferMode = TRUE, \
    .eCheckMode = chkmode, \
    .bSequenceEnable = seqenb, \
    .nDevAddress = devadr, \
    .wXmtBufferSize = xmtsize, \
    .wRcvBufferSize = rcvsize, \
    .pnXmtBuffer = ( PU8 )&anXmt ## name, \
    .pnRcvBuffer = ( PU8 )&anRcv ## name, \
    .pvWriteFunc = writefunc, \
    .ptSlvCmdTbl = ( PVOID )cmdtbl, \
  }

#if ( BINCMDHAND_ENABLE_MASTERMODE == 1 )
/// define the helper macro for creating a master entry/single buffer/point to point
#define BINCMD_MST_DEFSBPP( name, bufsize, chkmode, seqenb, numretry, writefunc, tbllen, cmdtbl ) \
  { \
    .bMasterMode = TRUE, \
    .bMultiDropMode = FALSE, \
    .bDualBufferMode = FALSE, \
    .eCheckMode = chkmode, \
    .bSequenceEnable = seqenb, \
    .nNumRetries = numretry, \
    .wRcvBufferSize = bufsize, \
    .pnRcvBuffer = ( PU8 )&anBuf ## name, \
    .pvWriteFunc = writefunc, \
    .nCmdTblLen = tbllen, \
    .ptMstCmdTbl = cmdtbl, \
  }

/// define the helper macro for creating a master entry/dual buffer/point to point
#define BINCMD_MST_DEFDBPP( name, xmtsize, rcvsize, chkmode, seqenb, numretry, writefunc, tbllen, cmdtbl ) \
  { \
    .bMasterMode = TRUE, \
    .bMultiDropMode = FALSE, \
    .bDualBufferMode = TRUE, \
    .eCheckMode = chkmode, \
    .bSequenceEnable = seqenb, \
    .nNumRetries = numretry, \
    .wXmtBufferSize = xmtsize, \
    .wRcvBufferSize = rcvsize, \
    .pnXmtBuffer = ( PU8 )&anXmt ## name, \
    .pnRcvBuffer = ( PU8 )&anRcv ## name, \
    .pvWriteFunc = writefunc, \
    .nCmdTblLen = tbllen, \
    .ptMstCmdTbl = cmdtbl, \
  }

/// define the helper macro for creating a master entry/single buffer/multidrop
#define BINCMD_MST_DEFSBMD( name, bufsize, chkmode, seqenb, devadr, numretry, writefunc, tbllen, cmdtbl ) \
  { \
    .bMasterMode = TRUE, \
    .bMultiDropMode = TRUE, \
    .bDualBufferMode = FALSE, \
    .eCheckMode = chkmode, \
    .bSequenceEnable = seqenb, \
    .nDevAddress = devadr, \
    .nNumRetries = numretry, \
    .wRcvBufferSize = bufsize, \
    .pnRcvBuffer = ( PU8 )&anBuf ## name, \
    .pvWriteFunc = writefunc, \
    .nCmdTblLen = tbllen, \
    .ptMstCmdTbl = cmdtbl, \
  }

/// define the helper macro for creating a master entry/dual buffer/multidrop
#define BINCMD_MST_DEFDBMD( name, xmtsize, rcvsize, chkmode, seqenb, devadr, numretry, writefunc, tbllen, cmdtbl ) \
  { \
    .bMasterMode = TRUE, \
    .bMultiDropMode = TRUE, \
    .bDualBufferMode = TRUE, \
    .eCheckMode = chkmode, \
    .bSequenceEnable = seqenb, \
    .nDevAddress = devadr, \
    .nNumRetries = numretry, \
    .wXmtBufferSize = xmtsize, \
    .wRcvBufferSize = rcvsize, \
    .pnXmtBuffer = ( PU8 )&anXmt ## name, \
    .pnRcvBuffer = ( PU8 )&anRcv ## name, \
    .pvWriteFunc = writefunc, \
    .nCmdTblLen = tbllen, \
    .ptMstCmdTbl = cmdtbl, \
  }

/// define the helper macro for creating a master/slave entry/double buffer/multidrop
#define BINCMD_MSV_DEFDBMD( name, mstsize, slvsize, chkmode, seqenb, mstadr, numretry, writefunc, msttbllen, mstcmdtbl, slvcmdtbl ) \
{ \
  .bMasterMode = TRUE, \
  .bMultiDropMode = TRUE, \
  .bDualBufferMode = TRUE, \
  .eCheckMode = chkmode, \
  .bSequenceEnable = seqenb, \
  .nDevAddress = mstadr, \
  .nNumRetries = numretry, \
  .wXmtBufferSize = mstsize, \
  .wRcvBufferSize = slvsize, \
  .pnXmtBuffer = ( PU8 )&anMst ## name, \
  .pnRcvBuffer = ( PU8 )&anSlv ## name, \
  .pvWriteFunc = writefunc, \
  .nCmdTblLen = msttbllen, \
  .ptMstCmdTbl = mstcmdtbl, \
  .ptSlvCmdTbl = slvcmdtbl, \
}

/// define the helper macro for creating a master/slave/entry/double buffer/point to point
#define BINCMD_MSV_DEFDBPP( name, mstsize, slvsize, chkmode, seqenb, numretry, writefunc, msttbllen, mstcmdtbl, slvcmdtbl ) \
{ \
  .bMasterMode = TRUE, \
  .bMultiDropMode = FALSE, \
  .bDualBufferMode = TRUE, \
  .eCheckMode = chkmode, \
  .bSequenceEnable = seqenb, \
  .nNumRetries = numretry, \
  .wXmtBufferSize = mstsize, \
  .wRcvBufferSize = slvsize, \
  .pnXmtBuffer = ( PU8 )&anMst ## name, \
  .pnRcvBuffer = ( PU8 )&anSlv ## name, \
  .pvWriteFunc = writefunc, \
  .nCmdTblLen = msttbllen, \
  .ptMstCmdTbl = mstcmdtbl, \
  .ptSlvCmdTbl = slvcmdtbl, \
}
#endif // BINCMDHAND_ENABLE_MASTERMODE

/// define the end of the table value
#define BINCMD_END_TABLE  0xFF

/// define the external table entry
#define BINCMD_EXT_TABLE  0xFE

/// helper macro to create slave internal entries
#define BINCMD_SLV_INTENTRY( cmd, mask, explen, flag, value, handler ) \
  { \
    .nCommand = cmd, \
    .nCmdMask = mask, \
    .iExpLength = explen, \
    .eCompareFlag = flag, \
    .nCompareValue = value, \
    .pvCmdFunc = ( PVBINCMDFUNC )handler \
  }

/// helper macro to create slave external entries
#define BINCMD_SLV_EXTENTRY( exttable ) \
{ \
  .nCommand = BINCMD_EXT_TABLE, \
  .pvExtTable = ( PVOID )exttable, \
}

/// helper macro to create the end of table
#define BINCMD_SLV_ENDENTRY( ) \
{ \
  .nCommand = BINCMD_END_TABLE, \
}

/// helper macro to create master entries
#define BINCMD_MST_ENTRY( cmd, explen, cmdhandler, rsphandler, errhandler ) \
  { \
    .nCommand = cmd, \
    .iExpLength = explen, \
    .ptMstCmdTblFunc = ( PVBINMSTCMDRSPFUNC )cmdhandler, \
    .pvMstRspFunc = ( PVBINMSTCMDRSPFUNC )rsphandler, \
    .pvMstErrFunc = ( PVBINMSTCMDRSPFUNC )errhandler, \
  }

// enumerations ---------------------------------------------------------------
/// enumerate the check mode
typedef enum  _BINCMDCHECKMODE
{
  BINCMD_CHECKMODE_EOR = 0,      ///< exclusive OR mode
  BINCMD_CHECKMODE_CMP,          ///< 2's complement
  BINCMD_CHECKMODE_CRC,          ///< CRC mode
  BINCMD_CHECKMODE_MAX,
} BINCMDCHECKMODE;

/// enumerate the sub opcodes
typedef enum _BINCMDSUBOPCS
{
  BINCMD_SUBOPC_GET = 0,         ///< get a value
  BINCMD_SUBOPC_SET,             ///< set a vlue
  BINCMD_SUBOPC_RSP,             ///< response value
  BINCMD_SUPOPC_ACK = 0x06,      ///< send an ACK
} BINCMDSUBOPCS;

/// enumerate the parse status
typedef enum _BINPARSESTS
{
  BINPARSE_STS_NONE = 0,          ///< no action
  BINPARSE_STS_SND_RESP,          ///< send a reponse - data
  BINPARSE_STS_SND_RESP_OPTION,   ///< send a response - option byte
  BINPARSE_STS_SND_ACK,           ///< send an ack
  BINPARSE_STS_SND_NAK,           ///< send an NAK
  BINPARSE_STS_SND_NORESP,        ///< no response
  BINPARSE_STS_ERR_ILLCMD = 0xF0, ///< command not found in table
  BINPARSE_STS_ERR_ILLFLG,        ///< command cannont execute becasue of flags compare
  BINPARSE_STS_ERR_ILLLEN,        ///< illegal command length
  BINPARSE_STS_ERR_ILLVAL,        ///< illegal value
  BINPARSE_STS_ERR_ILLHAND,       ///< illegal command handler
  BINPARSE_STS_ERR_NOCMDTBL,      ///< no command table
  BINPARSE_STS_ERR_TOOMANYTBLS,   ///< to many nested tables
  BINPARSE_STS_ERR_ILLMODE,       ///< illegal mode
} BINPARSESTS;

/// enumerate the compare flags
typedef enum _BINCOMPFLAG
{
  BINCOMP_FLAG_NONE = 0,        ///< no compare
  BINCOMP_FLAG_EQ,              ///< test flags for equal
  BINCOMP_FLAG_NE,              ///< test flags for not equal
  BINCOMP_FLAG_GT,              ///< test flags for greater than
  BINCOMP_FLAG_GE,              ///< test flags for greater than or equal
  BINCOMP_FLAG_LT,              ///< test flags for less than
  BINCOMP_FLAG_LE,              ///< test flags for less than or equal 
} BINCOMPFLAG;

// structures -----------------------------------------------------------------
/// define the write function prototype
typedef void        ( *PVBINWRITEFUNC )( PU8, U16 );

/// define the command handler function
typedef BINPARSESTS ( *PVBINCMDFUNC )( U8 );

#if ( BINCMDHAND_ENABLE_MASTERMODE == 1 )
/// define the master command response function
typedef void        ( *PVBINMSTCMDRSPFUNC )( void );
#endif // BINCMDHAND_ENABLE_MASTERMODE

/// define the command structure
typedef struct _BINCMDSLVENTRY
{
  U8                        nCommand;       ///< command
  U8                        nCmdMask;       ///< command mask
  S16                       iExpLength;     ///< expected length of message
  BINCOMPFLAG               eCompareFlag;   ///< compare flag
  U8                        nCompareValue;  ///< compare value
  PVBINCMDFUNC              pvCmdFunc;      ///< pointer to the command handler
  PVOID                     pvExtTable;     ///< pointer to an external table
} BINCMDSLVENTRY, *PBINCMDSLVENTRY;
#define BINCMDSLVENTRY_SIZE    sizeof( BINCMDSLVENTRY )

#if ( BINCMDHAND_ENABLE_MASTERMODE == 1 )
/// define the command structure for a master command
typedef struct _BINCMDMSTENTRY
{
  U8                  nCommand;             ///< master command
  S16                 iExpLength;           ///< expected length of response
  PVBINMSTCMDRSPFUNC  ptMstCmdTblFunc;      ///< pointer to the master command handler
  PVBINMSTCMDRSPFUNC  pvMstRspFunc;         ///< pointer to the command handler
  PVBINMSTCMDRSPFUNC  pvMstErrFunc;         ///< pointer to an optional error handler
} BINCMDMSTENTRY, *PBINCMDMSTENTRY;
#define BINCMDMSTENTRY_SIZE     sizeof( BINCMDMSTENTRY )
#endif // BINCMDHAND_ENABLE_MASTERMODE

/// define the control structure
typedef struct _BINCMDDEF
{
  BOOL                    bMultiDropMode;   ///< multi drop mode
  BOOL                    bDualBufferMode;  ///< dual buffer mode
  BOOL                    bSequenceEnable;  ///< enable the sequence numbers
  BOOL                    bMasterMode;      ///< master mode
  BINCMDCHECKMODE         eCheckMode;       ///< check mode
  U8                      nDevAddress;      ///< local device address for multidrop modes
  U8                      nNumRetries;      ///< number of retries
  U16                     wXmtBufferSize;   ///< transmit buffer size
  U16                     wRcvBufferSize;   ///< receive buffer size
  PU8                     pnXmtBuffer;      ///< pointer to the transmit buffer
  PU8                     pnRcvBuffer;      ///< pointer to the receive buffer
  PVBINWRITEFUNC          pvWriteFunc;      ///< pointer to a write char function
  BINCMDSLVENTRY const *  ptSlvCmdTbl;      ///< pointer to the command table
  #if ( BINCMDHAND_ENABLE_MASTERMODE == 1 )
  BINCMDMSTENTRY const *  ptMstCmdTbl;      ///< pointer to the master command table
  U8                      nCmdTblLen;       ///< command table legnth
  #endif // BINCMDHAND_ENABLE_MASTERMODE
} BINCMDDEF, *PBINCMDDEF;
#define BINCMDDEF_SIZE   sizeof( BINCMDDEF )

/**@} EOF BinaryCommandHandler_def.h */

#endif  // _BINARYCOMMANDHANDLER_DEF_H
