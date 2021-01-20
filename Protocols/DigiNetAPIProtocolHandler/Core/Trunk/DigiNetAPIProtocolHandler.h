/******************************************************************************
 * @file DigiNetAPIProtocolHandler.h
 *
 * @brief DIGI network handler declarations 
 *
 * This file provides the DIGI network devices declarations
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
 * \addtogroup DigiNetAPIProtocolHandler
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _DIGINETAPIPROTOCOLHANDLER_H
#define _DIGINETAPIPROTOCOLHANDLER_H

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------

// library includes -----------------------------------------------------------
#include "Types/Types.h"

// Macros and Defines ---------------------------------------------------------
/// define the frame types
#define DIGIFRAME_TYPE_ATCOMMAND                      ( 0x08 )
#define DIGIFRAME_TYPE_XMTSMS                         ( 0x1F )
#define DIGIFRAME_TYPE_XMTIP                          ( 0x20 )
#define DIGIFRAME_TYPE_ATRESPONSE                     ( 0x88 )
#define DIGIFRAME_TYPE_XMTSTATUS                      ( 0x89 )
#define DIGIFRAME_TYPE_MDMSTATUS                      ( 0x8A )
#define DIGIFRAME_TYPE_RCVSMS                         ( 0x9F )
#define DIGIFRAME_TYPE_RCVIP                          ( 0xB0 )

// enumerations ---------------------------------------------------------------
/// enumerate the frame types
typedef enum _DIGIFRAMETYPEENUM
{
  DIGIFRAME_TYPE_ENUM_ATCOMMAND = 0,
  DIGIFRAME_TYPE_ENUM_XMTSMS,
  DIGIFRAME_TYPE_ENUM_XMTIP,
  DIGIFRAME_TYPE_ENUM_ATRESPONSE,
  DIGIFRAME_TYPE_ENUM_XMTSTATUS,
  DIGIFRAME_TYPE_ENUM_MDMSTATUS,
  DIGIFRAME_TYPE_ENUM_RCVSMS,
  DIGIFRAME_TYPE_ENUM_RCVIP,
  DIGIFRAME_TYPE_ENUM_MAX
} DIGIFRAMETYPEENUM;

/// enumerate the association indications
typedef enum _DIGIASSOC
{
  DIGI_ASSOC_CONNECTED    = 0,    ///< connected
  DIGI_ASSOC_REGISTERING  = 0x22, ///< registering
  DIGI_ASSOC_CONNECTING,          ///< connecting
  DIGI_ASSOC_CELLMISSING,         ///< cellular component is missing
  DIGI_ASSOC_REGDENIED,           ///< registration denied
  DIGI_ASSOC_AIRPLANE     = 0x2A, ///< airplane mode
  DIGI_ASSOC_USBDIRECT,           ///< USB direct
  DIGI_ASSOC_BYPASS,              ///< bypass mode active
  DIGI_ASSOC_INITING      = 0xFF, ///< initializing
} DIGIASSOC;

/// enumerate the protocol type
typedef enum _DIGIPROTOCOLTYPE
{
  DIGIPROTOCOL_TYPE_UDP = 0,
  DIGIPROTOCOL_TYPE_TCP,
  DIGIPROTOCOL_TYPE_SSLTCP = 4,
} DIGIPROTOCOLTYPE;

/// enumerate the AT response status
typedef enum _DIGIATRESPSTS
{
  DIGIAT_RESPSTS_OK = 0,
  DIGIAT_RESPSTS_ERROR,
  DIGIAT_RESPSTS_INV_COMMAND,
  DIGIAT_RESPSTS_INV_PARAMETER,
} DIGIATRESPSTS;

/// enumerate the transmit status
typedef enum _DIGITXSTATUS
{
  DIGITX_STATUS_OK            = 0,
  DIGITX_STATUS_XMTCELLFAIL   = 0x21,
  DIGITX_STATUS_NOTREGED,
  DIGITX_STATUS_INVFRMVALUES  = 0x2C,
  DIGITX_STATUS_INTERNERROR   = 0x31,
  DIGITX_STATUS_RESOURCEERR,
  DIGITX_STATUS_MSGTOOLONG    = 0x74,
  DIGITX_STATUS_INVUDPPORT    = 0x78,
  DIGITX_STATUS_INVTCPPORT,
  DIGITX_STATUS_INVHOSTADDR,
  DIGITX_STATUS_INVDATAMODE,
  DIGITX_STATUS_CONNREFUSED   = 0x80,
  DIGITX_STATUS_SOCKETLOST,
  DIGITX_STATUS_NOSERVER,
  DIGITX_STATUS_SLOCKETCLOSED,
  DIGITX_STATUS_UNKNSERVER,
  DIGITX_STATUS_UNKNERROR,
} DIGITXSTATUS;

/// enumerate the modem status
typedef enum _DIGIMDSTATUS
{
  DIGIMD_STATUS_RESET        = 0,
  DIGIMD_STATUS_WDTRST,
  DIGIMD_STATUS_CELLREG,
  DIGIMD_STATUS_UNREGCELL,
  DIGIMD_STATUS_RMTMNGRCON   = 0x0E,
  DIGIMD_STATUS_RMTMNGRDIS,
} DIGIMDSTATUS;

/// enumerate the frame decode status
typedef enum _DIGIFRMDECSTS
{
  DIGI_FRMDEC_STS_NONE        = 0,
  DIGI_FRMDEC_STS_INPROG,
  DIGI_FRMDEC_STS_ATRESPONSE  = DIGIFRAME_TYPE_ATRESPONSE,
  DIGI_FRMDEC_STS_XMTSTATUS   = DIGIFRAME_TYPE_XMTSTATUS,
  DIGI_FRMDEC_STS_MDMSTATUS   = DIGIFRAME_TYPE_MDMSTATUS,
  DIGI_FRMDEC_STS_RCVSMS      = DIGIFRAME_TYPE_RCVSMS,
  DIGI_FRMDEC_STS_RCVIP       = DIGIFRAME_TYPE_RCVIP
} DIGIFRMDECSTS;

// structures -----------------------------------------------------------------
/// define the AT command data
typedef struct _DIGIATCOMMAND
{
  U8    anCmd[ 2 ];         ///< AT command
  U8    nParameter;         ///< AT parameter
} DIGIATCOMMAND, *PDIGIATCOMMAND;
#define DIGIATCOMMAND_SIZE                    sizeof( DIGIATCOMMAND )

/// define the transmit SMS data
typedef struct _DIGIXMTSMS
{
  U8    nOption;            ///< option
  C8    acPhone[ 20 ];      ///< phone number
} DIGIXMTSMS, *PDIGIXMTSMS;
#define DIGIXMTSMS_SIZE                       sizeof( DIGIXMTSMS )

/// define the transmit IP data
typedef struct _DIGIXMTIP
{
  U32UN             tDestAddr;     ///< destination address
  U16               wDstPort;      ///< destination port
  U16               wSrcPort;      ///< source port
  DIGIPROTOCOLTYPE  eProtType;     ///< protocol type
  struct
  {
    U8  bRes1       : 1;            ///< reserved
    U8  bTermSocket : 1;            ///< terminate socket on transmit complete
    U8  nRes2       : 6;            ///< reserved
  } tOptions;
} DIGIXMTIP, *PDIGIXMTIP;
#define DIGIXMTIP_SIZE                        sizeof( DIGIXMTIP )

/// define the AT command response
typedef struct _DIGIATRESPONSE
{
  U8            anCmd[ 2 ];         ///< AT command
  DIGIATRESPSTS eStatus;            ///< AT response status
  U8            nParameter;         ///< parameter value
} DIGIATRESPONSE, *PDIGIATRESPONSE;
#define DIGIATRESPONSE_SIZE                   sizeof( DIGIATRESPONSE)

/// define the transmit status
typedef struct _DIGIXMTSTATUS
{
  DIGITXSTATUS  eStatus;            ///< status
} DIGIXMTSTATUS, *PDIGIXMTSTATUS;
#define DIGIXMTSTATUS_SIZE                    sizeof( DIGIXMTSTATUS )

/// define the modem status
typedef struct _DIGIMDMSTATUS
{
  DIGIMDSTATUS  eStatus;            ///< status
} DIGIMDMSTATUS, *PDIGIMDMSTATUS;
#define DIGIMDMSTATUS_SIZE                    sizeof( DIGIMDMSTATUS )

/// define the receive SMS status
typedef struct _DIGIRCVSMS
{
  C8    acPhone[ 20 ];      ///< phone number
} DIGIRCVSMS, *PDIGIRCVSMS;
#define DIGIRCVSMS_SIZE                       sizeof( DIGIRCVSMS )

/// define the receive IP 
typedef struct _DIGIRCVIP
{
  U8  anData[ 1500u ];
} DIGIRCVIP, *PDIGIRCVIP;
#define DIGIRCVIP_SIZE                        sizeof( DIGIRCVIP )

/// define the frame data union
typedef union _DIGIAPIFRAMEDAT
{
  union
  {
    DIGIATCOMMAND   tAtCommand;
    DIGIXMTSMS      tXmtSms;
    DIGIXMTIP       tXmtIp;
    DIGIATRESPONSE  tAtResponse;
    DIGIXMTSTATUS   tXmtStatus;
    DIGIMDMSTATUS   tMdmStatus;
    DIGIRCVSMS      tRcvSms;
    DIGIRCVIP       tRcvIp;
  } tTypes;
  U8  anBuffer[ 1 ];
} DIGIAPIFRAMEDAT, *PDIGIAPIFRAMEDAT;
#define DIGIAPIFRAMEDAT_SIZE                sizeof( DIGIAPIFRAMEDAT )

// global parameter declarations -----------------------------------------------

// global function prototypes --------------------------------------------------
extern  void          DigiNetAPIProtocolHandler_Initialize( void );
extern  U16           DigiNetAPIProtocolHandler_SendCommand( DIGIFRAMETYPEENUM eFrameType, PU8 pnFrameData, U16 wFrameLength, PU8 pnXmtBuffer );
extern  DIGIFRMDECSTS DigiNetAPIProtocolHandler_ProcessRcvChar( PDIGIAPIFRAMEDAT ptFrameData, U8 nChar );

/**@} EOF DigiNetAPIProtocolHandler.h */

#endif  // _DIGINETAPIPROTOCOLHANDLER_H