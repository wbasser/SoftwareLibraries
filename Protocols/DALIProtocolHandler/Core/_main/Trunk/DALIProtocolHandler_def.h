/****************************************7**************************************
 * @file  DALIProtocolHandler_def.h
 *
 * @brief  DALI message handler declarations
 *
 * This file provides the declarations for the DALI message handler
 *
 * @copyright Copyright (c) 2012 Cyber Integration
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
 * \addtogroup  DALIProtocolHandler
 * @{
 *****************************************************************************/

// ensure only one instantiation
#ifndef _DALIPROTOCOLHANDLER_DEF_H
#define _DALIPROTOCOLHANDLER_DEF_H

// system includes ------------------------------------------------------------

// library includes -----------------------------------------------------------
#include "Types/Types.h"

// local includes -------------------------------------------------------------
#include "DALIProtocolHandler/DALIProtocolHandler_prm.h"

// Macros and Defines ---------------------------------------------------------
/// define the size of the Global Trade Item Number
#define DALI_GTIN_SIZE            ( 6 )

/// define the serial number size
#define DALI_SNUM_SIZE            ( 4 )

/// define the maximum number of scenes
#define DALI_NUM_SCENES           ( 16 )

/// define the number of search/random address's
#define DALI_NUM_ADDR_BYTES       ( 3 )

/// define the maximum number of scenes
#define MAX_NUM_SCENES            ( 16 )

/// define the maximum number of fade times
#define MAX_NUM_NORMFADETIMES     ( 16 )

/// define the maximum number of fate rates
#define MAX_NUM_NORMFADERATES     ( 16 )

/// define the fast fade time multiplier
#define FAST_FADE_TIME_MULT       ( 25000 )

/// define the yes/no responses
#define  DALI_RESPONSE_NO         (  0 )
#define  DALI_RESPONSE_YES        ( 0xFF )

/// Define UP/DOWN fade time        
#define UP_DOWN_FADE_TIME         ( 200 )

/// define the macro for defining command table entries
#define  CMDTBL_NORMENT( repeat, response, handler ) \
  { \
    .tFlags = \
    { \
      .bRepeatFlag = repeat, \
      .eResponseType = response, \
      .bExecInTime = FALSE, \
      .bDevTypeEnabled = FALSE, \
    }, \
    .pvCmdHandler = handler \
  }

/// define the macro for defining special commands
#define  CMDTBL_SPCLENT( repeat, response, handler ) \
  { \
    .tFlags = \
    { \
      .bRepeatFlag = repeat, \
      .eResponseType = response, \
      .bExecInTime = TRUE, \
      .bDevTypeEnabled = FALSE, \
    }, \
    .pvCmdHandler = handler \
  }

/// define the macro for defining extended application commands
#define  CMDTBL_EXTNENT( repeat, response, handler ) \
  { \
    .tFlags = \
    { \
      .bRepeatFlag = repeat, \
      .eResponseType = response, \
      .bExecInTime = FALSE, \
      .bDevTypeEnabled = TRUE, \
    }, \
    .pvCmdHandler = handler \
  }

/// define the macro to get command type from the address
#define DALI_GET_TYPE( addr )     (( addr >> 4 ) & 0x0F )

/// define the macro to the get the command from the address
#define DALI_GET_CMND( addr )     ( addr & 0x0F )

/// define the minimum command value for special/extended commands
#define DALI_MIN_SPCLEXT_CMD      ( 0xA1 )

/// define the maximum command value for special/extended commnds
#define DALI_MAX_SPCLEXT_CMD      ( 0xE0 )

// enumerations ---------------------------------------------------------------
/// enumerate the random address byte locations
typedef enum _RANDOMADDR
{
  RANDOM_ADDR_L = LE_U32_LSB_IDX,
  RANDOM_ADDR_M = LE_U32_MS1_IDX,
  RANDOM_ADDR_H = LE_U32_MS2_IDX,
} RANDOMADDR;

/// enumerate the command type
typedef enum _COMMANDTYPE
{
  COMMAND_TYPE_SPCL = 0x01,
  COMMAND_TYPE_EXTN = 0x02,
} COMMANDTYPE;

/// enumerate the physical selection states
typedef enum _PHYSELSTATE
{
  PHYSEL_STATE_DISABLED = 0,
  PHYSEL_STATE_REQUESTED,
  PHYSEL_STATE_ENABLED
} PHYSELSTATE;

/// enumerate the fade direction
typedef enum _FADEDIR
{
  FADE_DIR_IDLE = 0,
  FADE_DIR_DN,
  FADE_DIR_UP
} FADEDIR;

/// enumerate the reponse types
typedef enum _RESPONSETYPE
{
  RESPONSE_TYPE_NONE = 0,         ///< no response
  RESPONSE_TYPE_SEND,             ///< send the response
  RESPONSE_TYPE_FLAG              ///< send response only if flag is set in control structure
} RESPONSETYPE;

/// enumerate the message handler states
typedef enum _MSGSTATE
{
  MSG_STATE_IDLE      = 0,    ///< idle
  MSG_STATE_WAITRCVDONE,      ///< message received
  MSG_STATE_WAITXMITDONE,     ///< wait for transmit done
  MSG_STATE_OFF,              ///< message state off
} MSGSTATE;

// structures -----------------------------------------------------------------
/// define the DALI address structure
typedef struct _DALIADDR
{
  U8  bValueCmd     : 1;          ///< value = 0, 1 = command
  U8  nAddress      : 6;          ///< address
  U8  bDirectGroup  : 1;          ///< direct = 0, 1 = group
} PACKED DALIADDR, *PDALIADDR;
#define DALIADDR_SIZE   sizeof( DALIADDR )

/// define the DALI event structure
typedef union _DALIMSG
{
  struct
  {
    DALIADDR    tAddr;          ///< address component of message
    U8          nData;          ///< data component of message
  } tFields;
  U8  anBuffer[ 2 ];            ///< buffer
} PACKED DALIMSG, *PDALIMSG;
#define DALIMSG_SIZE    sizeof( DALIMSG )

/// Define the gear type structure
typedef union PACKED _GEARTYPE
{
  struct
  {
    BOOL bLEDPSU     : 1;       ///< LED power supply integrated
    BOOL bLEDModule  : 1;       ///< LED module integrated 
    BOOL bACSupply   : 1;       ///< a.c. supply possible
    BOOL bDCSupply   : 1;       ///< d.c. supply possible
    BOOL bUnunsed1   : 1;
    BOOL bUnunsed2   : 1;
    BOOL bUnunsed3   : 1;
    BOOL bUnunsed4   : 1;
  } tFields;
  U8 nGearType;
} GEARTYPE, *PGEARTYPE;
#define  GEARTYPE_SIZE    sizeof( GEARTYPE )

/// Define the operating types structure
typedef union PACKED _OPTYPES
{
  struct
  {
    BOOL bPWMMode           : 1;    ///< PWM mode is possible
    BOOL bAMMode            : 1;    ///< AM mode is possible
    BOOL bCurrentControl    : 1;    ///< Output is current controlled
    BOOL bHighCurrentPulse  : 1;    ///< high current pulse mode
    BOOL bUnunsed1          : 1;
    BOOL bUnunsed2          : 1;
    BOOL bUnunsed3          : 1;
    BOOL bUnunsed4          : 1;
  } tFields;
  U8 nPossibleOpTypes;
} OPTYPES, *POPTYPES;
#define  OPTYPES_SIZE    sizeof( OPTYPES )

/// Define the features structure
typedef union PACKED _FEATURES
{
  struct
  {
    BOOL bShortCircuit       : 1;  ///< Short circuit detection can be queried
    BOOL bOpenCircuit        : 1;  ///< Open circuit detection can be queried
    BOOL bLoadDecrease       : 1;  ///< detection of load decrease can be queried
    BOOL bLoadIncrease       : 1;  ///< detection of load increase can be queried
    BOOL bCurrentProtector   : 1;  ///< current protector is implemented and can be queried
    BOOL bThermalShutdown    : 1;  ///< thermal shutdown can be queried
    BOOL bLightLevelReduce   : 1;  ///< thermal shut down can be queried
    BOOL bPhysicalSelection  : 1;  ///< physical selection supported
  } tFields;
  U8 nFeatures;
} FEATURES, *PFEATURES;
#define  FEATURES_SIZE    sizeof( FEATURES )

/// Define the failure status structure
typedef union PACKED _FAILURESTATUS
{
  struct
  {
    BOOL bShortCircuit      : 1;  ///< Short Circuit
    BOOL bOpenCircuit       : 1;  ///< Open Circuit
    BOOL bLoadDecrease      : 1;  ///< Load Decrease
    BOOL bLoadIncrease      : 1;  ///< Load Increase
    BOOL bCurrentProtector  : 1;  ///< Current Protector Active
    BOOL bThermalShutdown   : 1;  ///< Thermal Shutdown
    BOOL bThermalOverload   : 1;  ///< Thermal Overload with light level reduction
    BOOL bRefMeasureFail    : 1;  ///< Reference measurement failed
  } tFields;
  U8 nFailureStatus;
} FAILURESTATUS, *PFAILURESTATUS;
#define  FAILURESTATUS_SIZE    sizeof( FAILURESTATUS )

/// Define the operating mode structure
typedef union PACKED _OPMODE
{
  struct
  {
    BOOL bPWMMode           : 1;  ///< PWM Mode
    BOOL bAMMode            : 1;  ///< AM Mode
    BOOL bCurrentControl    : 1;  ///< Current control mode
    BOOL bHighCurrentPulse  : 1;  ///< High current pulse mode
    BOOL bDimCurve          : 1;  ///< Non-logarithmic dimming curve active
    BOOL bUnused1           : 1;
    BOOL bUnused2           : 1;
    BOOL bUnused3           : 1;
  } tFields;
  U8 nOpMode;
} OPMODE, *POPMODE;
#define  OPMODE_SIZE    sizeof( OPMODE )

/// define the dali parameter structure
typedef struct PACKED _DALIPRM
{
  U8              nPowerOnLevel;                  ///< power on level
  U8              nSystemFailureLevel;            ///< system failure level
  U8              nMinLevel;                      ///< minimum level
  U8              nMaxLevel;                      ///< maximum level
  U8              nLastReqLevel;                  ///< current level
  U8              nFadeRate;                      ///< fade rate
  U8              nFadeTime;                      ///< fade time
  U8              nShortAddress;                  ///< short address
  U32UN           tRandomAddr;                    ///< random address
  U16UN           tGroup;                         ///< group masks
  U8              anScenes[ DALI_NUM_SCENES ];    ///< scene array
  FAILURESTATUS   tFailureStatus;                 ///< failure status
  OPMODE          tOpMode;                        ///< Operating Mode
  U8              nFastFadeTime;                  ///< fast fade time
} DALIPRM, *PDALIPRM;
#define  DALIPRM_SIZE    sizeof( DALIPRM )

/// define the reset state structure - TRUE means parameter is out of reset state 
typedef union PACKED _RESETSTATE
{
  struct
  {
    BOOL bActualLevel           :  1;  ///< Actual Level
    BOOL bPowerOnLevel          :  1;  ///< Power On Level
    BOOL bSystemFailureLevel    :  1;  ///< System Failure Level
    BOOL bMinLevel              :  1;  ///< Minimum Level
    BOOL bMaxLevel              :  1;  ///< Maximum Level
    BOOL bFadeRate              :  1;  ///< Fade Rate
    BOOL bFadeTime              :  1;  ///< Fade Time
    BOOL bSearchAddrH           :  1;  ///< Search Address High Byte
    BOOL bSearchAddrM           :  1;  ///< Search Address Mid Byte
    BOOL bSearchAddrL           :  1;  ///< Search Address Low Byte
    BOOL bRandAddrH             :  1;  ///< Random Address High Byte
    BOOL bRandAddrM             :  1;  ///< Random Address Mid Byte
    BOOL bRandAddrL             :  1;  ///< Random Address Low Byte
    BOOL bGroup                 :  1;  ///< All Groups 
    U16  wScene                 : 16;  ///< scenes
  } tBits;
  U32 uWord;
} RESETSTATE, *PRESETSTATE;
#define  RESETSTATE_SIZE   sizeof( RESETSTATE )

/// define the status structure
typedef union PACKED _DALISTS
{
  struct
  {
    BOOL  bBallastFailure     : 1;      ///< ballast failure
    BOOL  bLampFailure        : 1;      ///< lamp failure
    BOOL  bLampOn             : 1;      ///< lamp on
    BOOL  bLimitError         : 1;      ///< limit error
    BOOL  bFadeRunning        : 1;      ///< fade running
    BOOL  bResetState         : 1;      ///< reset state
    BOOL  bMissingShortAddr   : 1;      ///< missing short address
    BOOL  bPowerFailure       : 1;      ///< power failure
  } tBits;
  U8  nByte;
} DALISTS, *PDALISTS;
#define  DALISTS_SIZE    sizeof( DALISTS )

/// define the dali control structure
typedef struct PACKED _DALICTL
{
  U8            nDataTransReg;          ///< data transfer register
  U8            nDataTransReg1;         ///< data transfer register #1
  U8            nDataTransReg2;         ///< data transfer register #2
  U8            nActualDimLevel;        ///< actual dim level
  U8            nRequestedLevel;        ///< requested level
  U32UN         tSearchAddr;            ///< search address
  U8            nResponseValue;         ///< response value
  U32           uFadePeriod;            ///< fade period
  U32           uFadeCounter;           ///< fade counter
  U32           uFadeIncrement;         ///< fade increment - (Microseconds/step)
  FADEDIR       eFadeDir;               ///< fade direction
  PHYSELSTATE   ePhysicalSelEnabled;    ///< physical selection enabled
  DALIPRM       tDaliPrm;               ///< DALI parameters
  DALISTS       tDaliSts;               ///< current status
  RESETSTATE    tResetState;            ///< Reset State indicator
  DALIMSG       tPrvMessage;            ///< previous message
  DALIMSG       tCurMessage;            ///< current message
  U8            nMinFastFadeTime;       ///< minimum fast fade time
  GEARTYPE      tGearType;              ///< gear type
  OPTYPES       tPossibleOpTypes;       ///< possible operating types
  FEATURES      tFeatures;              ///< features
  U8            nExtendedVerNum;        ///< extended version number
  U8            nDeviceType;            ///< device type
  U8            nPhysicalMinLvl;        ///< Physical Minimum Level
  U8            nCurFadeTime;           ///< current fade time
  MSGSTATE      eMsgState;              ///< message state
  U8            nBusFailCount;          ///< define the bus fail count 
  U8            nDeviceIndex;           ///< device index
  struct
  {
    BOOL  bDAPCSeqEnabled           : 1;  ///< DAPC sequence enabled
    BOOL  b15MinTimerRunning        : 1;  ///< 15 minute timer is running
    BOOL  bCompareModeEnabled       : 1;  ///< compare mode enabled
    BOOL  bWriteMemoryEnabled       : 1;  ///< write memory enabled
    BOOL  bOutputChangeRequested    : 1;  ///< output change requested
    BOOL  bResetRequested           : 1;  ///< reset requested
    BOOL  bResponsePending          : 1;  ///< response pending
    BOOL  bResponseRequested        : 1;  ///< response requested
    BOOL  bRepeatTimerRequested     : 1;  ///< repeat timer requested
    BOOL  bAppExtRequested          : 1;  ///< Application extended command requested
    BOOL  bDAPCSequenceRunning      : 1;  ///< DAPC sequence running
    BOOL  bFadeRateReq              : 1;  ///< Fade Rate Request Flag
    BOOL  bDaliInitialized          : 1;  ///< DALI has been initialized
    BOOL  bForcedMessage            : 1;  ///< flag indicates that is was a forced message
    BOOL  bTestBusPowerOn           : 1;  ///< test bus power on
  } tFlags;
} DALICTL, *PDALICTL;
#define  DALICTL_SIZE    sizeof( DALICTL )

/// define the command handler function
typedef void  ( *PVCMDHANDLERFUNC )( void );

/// define the command table structure
typedef struct PACKED _CMDTBL
{
  PVCMDHANDLERFUNC  pvCmdHandler;  ///< command handler
  struct
  {
    BOOL          bRepeatFlag       : 1;    ///< repeat message
    RESPONSETYPE  eResponseType     : 2;    ///< send response
    BOOL          bExecInTime       : 1;    ///< execute in the 15 initialization window
    BOOL          bDevTypeEnabled   : 1;    ///< device type enabled
    BOOL          bClearResetState  : 1;    ///< clear reset state
  } tFlags;
} CMDTBL, *PCMDTBL;
#define  CMDTBL_SIZE      sizeof( CMDTBL )

// constant parameter initializations -----------------------------------------
/// fade times in microseconds
static  const CODE U32 auFadeTimes[ MAX_NUM_NORMFADETIMES ] =
{
        0,   700000,  1000000,  1400000,  2000000,  2800000,  4000000,  5700000,
  8000000, 11300000, 16000000, 22600000, 32000000, 45300000, 64000000, 90500000
};

/// fade rates in Microseconds per step
static  const CODE U32 auFadeRates[ MAX_NUM_NORMFADERATES ] =
{
       0,   2793,   3953,   5587,    7874,  11186,  15797,  22371,
   31646,  44643,  66667,  89286, 1216582, 178571, 250000, 357142
};

// logarithmic dimming curve in tenths of a percent
static	const CODE	U16 awDimPercent[ ] =
{
    0,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,
    1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   2,   2,   2,   2,   2,
    2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   3,   3,   3,   3,   3,   3,
    3,   3,   3,   3,   4,   4,   4,   4,   4,   4,   4,   4,   5,   5,   5,   5,
    5,   5,   5,   6,   6,   6,   6,   6,   6,   7,   7,   7,   7,   7,   8,   8,
    8,   8,   9,   9,   9,   9,  10,  10,  10,  11,  11,  11,  12,  12,  12,  13,
   13,  13,  14,  14,  14,  15,  15,  16,  16,  17,  17,  18,  18,  19,  19,  20,
   20,  21,  21,  22,  23,  23,  24,  25,  25,  26,  27,  27,  28,  29,  30,  31,
   32,  32,  33,  34,  35,  36,  37,  38,  39,  40,  42,  43,  44,  45,  46,  48,
   49,  50,  52,  53,  55,  56,  58,  60,  61,  63,  65,  67,  68,  70,  72,  74,
   76,  78,  81,  83,  85,  88,  90,  92,  95,  98, 100, 103, 106, 109, 112, 115,
  118, 122, 125, 129, 132, 136, 140, 143, 147, 151, 156, 160, 164, 169, 174, 171,
  184, 189, 194, 199, 205, 210, 216, 222, 228, 235, 241, 248, 255, 262, 269, 277,
  284, 292, 300, 309, 317, 326, 335, 344, 354, 364, 374, 384, 395, 406, 417, 428,
  440, 453, 465, 478, 491, 505, 519, 533, 548, 563, 579, 595, 611, 628, 646, 663,
  682, 701, 720, 740, 761, 782, 803, 826, 848, 872, 896, 921, 946, 973, 1000
};

/**@} EOF DALIProtocolHandler_def.h */

#endif  // _DALIPROTOCOLHANDLER_DEF_H
