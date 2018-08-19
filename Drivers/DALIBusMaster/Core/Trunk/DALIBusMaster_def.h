/******************************************************************************
 * @file DALIBusMaster_def.h
 *
 * @brief DALI Bus manager definitions 
 *
 * This file provides the definitions for the DALI bus master
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
 * \addtogroup DALIBusMaster
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _DALIBUSMASTER_DEF_H
#define _DALIBUSMASTER_DEF_H

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------

// library includes -----------------------------------------------------------
#include "Types/Types.h"

// Macros and Defines ---------------------------------------------------------
/// define the maximum number of devices
#define DALI_MAX_NUM_OF_DEVICES                   ( 64 )

/// define the bus status size
#define DALI_DEV_SIZE_NUMBITS                     ( 2 )

// enumerations ---------------------------------------------------------------
/// enumerate for the device status
typedef enum _DALIBUSMASTERDEVSTS
{
  DALIBUSMASTER_DEVSTS_NOTPRESENT = 0,    ///< no device present
  DALIBUSMASTER_DEVSTS_PRESENT,           ///< device present
  DALIBUSMASTER_DEVSTS_COLLISION,         ///< device present - collision detected
  DALIBUSMASTER_DEVSTS_BAD,               ///< device not responding
  DALIBUSMASTER_DEVSTS_MAX
} DALIBUSMASTERDEVSTS;

/// enumerate the errors
typedef enum _DALIBUSMASTERSTS
{
  DALIBUSMASTER_STS_NOERROR = 0,      ///< 0x00 - OK transmit, no receive expected
  DALIBUSMASTER_STS_NOERRRCV,         ///< 0x01 - OK transmit, OK receive expected
  DALIBUSMASTER_STS_NOERRNORCV,       ///< 0x02 - OK transmit, RCV Expected, no receive
  DALIBUSMASTER_STS_TMOXMT = 0xF0,    ///< 0xF0 - timeout on transmit
  DALIBUSMASTER_STS_BUSERR,           ///< 0xF1 - bus error - no receive echo
  DALIBUSMASTER_STS_CMPERR,           ///< 0xF2 - echo received, compare error
  DALIBUSMASTER_STS_TMORCV,           ///< 0xF3 - timeout on receive
  DALIBUSMASTER_STS_COLLISION,        ///< 0xF4 - bus activity - collision
  DALIBUSMASTER_STS_ILLOPERATION,     ///< 0xF5 - illegal operation, (i.e. a broadcast query request)
  DALIBUSMASTER_STS_BUSMONITORBAD,    ///< 0xF6 - bus monitor indicates bad bus
  DALIBUSMASTER_STS_MAX,
} DALIBUSMASTERSTS;

// structures -----------------------------------------------------------------
/// define the device status union
typedef union _DALIBUSMASTERDEVSTATUS
{
  struct
  {
    U8 nDevice00 : DALI_DEV_SIZE_NUMBITS;  
    U8 nDevice01 : DALI_DEV_SIZE_NUMBITS;  
    U8 nDevice02 : DALI_DEV_SIZE_NUMBITS;  
    U8 nDevice03 : DALI_DEV_SIZE_NUMBITS;  
    U8 nDevice04 : DALI_DEV_SIZE_NUMBITS;  
    U8 nDevice05 : DALI_DEV_SIZE_NUMBITS;  
    U8 nDevice06 : DALI_DEV_SIZE_NUMBITS;  
    U8 nDevice07 : DALI_DEV_SIZE_NUMBITS;  
    U8 nDevice08 : DALI_DEV_SIZE_NUMBITS;  
    U8 nDevice09 : DALI_DEV_SIZE_NUMBITS;  
    U8 nDevice10 : DALI_DEV_SIZE_NUMBITS;  
    U8 nDevice11 : DALI_DEV_SIZE_NUMBITS;  
    U8 nDevice12 : DALI_DEV_SIZE_NUMBITS;  
    U8 nDevice13 : DALI_DEV_SIZE_NUMBITS;  
    U8 nDevice14 : DALI_DEV_SIZE_NUMBITS;  
    U8 nDevice15 : DALI_DEV_SIZE_NUMBITS;  
    U8 nDevice16 : DALI_DEV_SIZE_NUMBITS;  
    U8 nDevice17 : DALI_DEV_SIZE_NUMBITS;  
    U8 nDevice18 : DALI_DEV_SIZE_NUMBITS;  
    U8 nDevice19 : DALI_DEV_SIZE_NUMBITS;  
    U8 nDevice20 : DALI_DEV_SIZE_NUMBITS;  
    U8 nDevice21 : DALI_DEV_SIZE_NUMBITS;  
    U8 nDevice22 : DALI_DEV_SIZE_NUMBITS;  
    U8 nDevice23 : DALI_DEV_SIZE_NUMBITS;  
    U8 nDevice24 : DALI_DEV_SIZE_NUMBITS;  
    U8 nDevice25 : DALI_DEV_SIZE_NUMBITS;  
    U8 nDevice26 : DALI_DEV_SIZE_NUMBITS;  
    U8 nDevice27 : DALI_DEV_SIZE_NUMBITS;  
    U8 nDevice28 : DALI_DEV_SIZE_NUMBITS;  
    U8 nDevice29 : DALI_DEV_SIZE_NUMBITS;  
    U8 nDevice30 : DALI_DEV_SIZE_NUMBITS;  
    U8 nDevice31 : DALI_DEV_SIZE_NUMBITS;  
    U8 nDevice32 : DALI_DEV_SIZE_NUMBITS;  
    U8 nDevice33 : DALI_DEV_SIZE_NUMBITS;  
    U8 nDevice34 : DALI_DEV_SIZE_NUMBITS;  
    U8 nDevice35 : DALI_DEV_SIZE_NUMBITS;  
    U8 nDevice36 : DALI_DEV_SIZE_NUMBITS;  
    U8 nDevice37 : DALI_DEV_SIZE_NUMBITS;  
    U8 nDevice38 : DALI_DEV_SIZE_NUMBITS;  
    U8 nDevice39 : DALI_DEV_SIZE_NUMBITS;  
    U8 nDevice40 : DALI_DEV_SIZE_NUMBITS;  
    U8 nDevice41 : DALI_DEV_SIZE_NUMBITS;  
    U8 nDevice42 : DALI_DEV_SIZE_NUMBITS;  
    U8 nDevice43 : DALI_DEV_SIZE_NUMBITS;  
    U8 nDevice44 : DALI_DEV_SIZE_NUMBITS;  
    U8 nDevice45 : DALI_DEV_SIZE_NUMBITS;  
    U8 nDevice46 : DALI_DEV_SIZE_NUMBITS;  
    U8 nDevice47 : DALI_DEV_SIZE_NUMBITS;  
    U8 nDevice48 : DALI_DEV_SIZE_NUMBITS;  
    U8 nDevice49 : DALI_DEV_SIZE_NUMBITS;  
    U8 nDevice50 : DALI_DEV_SIZE_NUMBITS;  
    U8 nDevice51 : DALI_DEV_SIZE_NUMBITS;  
    U8 nDevice52 : DALI_DEV_SIZE_NUMBITS;  
    U8 nDevice53 : DALI_DEV_SIZE_NUMBITS;  
    U8 nDevice54 : DALI_DEV_SIZE_NUMBITS;  
    U8 nDevice55 : DALI_DEV_SIZE_NUMBITS;  
    U8 nDevice56 : DALI_DEV_SIZE_NUMBITS;  
    U8 nDevice57 : DALI_DEV_SIZE_NUMBITS;  
    U8 nDevice58 : DALI_DEV_SIZE_NUMBITS;  
    U8 nDevice59 : DALI_DEV_SIZE_NUMBITS;  
    U8 nDevice60 : DALI_DEV_SIZE_NUMBITS;  
    U8 nDevice61 : DALI_DEV_SIZE_NUMBITS;  
    U8 nDevice62 : DALI_DEV_SIZE_NUMBITS;  
    U8 nDevice63 : DALI_DEV_SIZE_NUMBITS;  
  } tFields;
  union
  {
    U8    anValues[ 1 ];
    U16UN atwValues[ 1 ];
    U32UN atuValues[ 1 ];
  } tValues;
} DALIBUSMASTERDEVSTATUS, *PDALIBUSMASTERDEVSTATUS;
#define DALIBUSMASTERDEVSTATUS_SIZE          ( sizeof( DALIBUSMASTERDEVSTATUS ))

/// define the DALI message structure
typedef union _DALIXMTMSG
{
  struct
  {
    struct
    {
      BOOL     bLvlCmd : 1;       ///< 0-level, 1-command
      U8       nAddr   : 6;       ///< address
      BOOL     bDirGrp : 1;       ///< 0-direct address, 1-group
    } tAddr;
    U8  nDataCmd;                 ///< data or command
  } tFields;
  U8    anBuffer[ 2 ];
} DALIXMTMSG, *PDALIXMTMSG;
#define DALIXMTMSG_SIZE    sizeof( DALIXMTMSG )

/// define the return message
typedef struct _DALIXMTRCV
{
  DALIBUSMASTERSTS  eStatus;
  DALIXMTMSG        tXmtMsg;
  U8                nRcvMsg;
} DALIXMTRCV, *PDALIXMTRCV;
#define DALIXMTRCV_SIZE     sizeof( DALIXMTRCV )

/// define the callback function
typedef  void ( *PVDALIBUSMSTRCB )( PVOID );

/// define the message structure
typedef struct _DALIBUSMASTERMSG
{
  PVDALIBUSMSTRCB pvCallbackFunc;
  U8              nOption;
  DALIXMTRCV      tDaliXmtRcvMsg;
} DALIBUSMASTERMSG, *PDALIBUSMASTERMSG;
#define DALIBUSMASTERMSG_SIZE               sizeof ( DALIBUSMASTERMSG )

/**@} EOF DALIBusMaster_def.h */

#endif  // _DALIBUSMASTER_DEF_H