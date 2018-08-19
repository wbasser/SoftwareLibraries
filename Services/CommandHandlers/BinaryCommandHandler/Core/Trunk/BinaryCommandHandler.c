/******************************************************************************
 * @file BinaryCommandHandler.c
 *
 * @brief 
 *
 * This file 
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

// local includes -------------------------------------------------------------
#include "BinaryCommandHandler/BinaryCommandHandler.h"
#include "BinaryCommandHandler/BinaryCommandHandler_prm.h"

// library includes -----------------------------------------------------------
#include "CRC16/Crc16.h"
#include "StateExecutionEngine/StateExecutionEngine.h"
#if ( BINARYCOMMANDHANDLER_ENABLE_DEBUG == 1 )
#include "DebugManager/DebugManager.h"
#endif // BINARYCOMMANDHANDLER_ENABLE_DEBUG

// Macros and Defines ---------------------------------------------------------
/// define the special characters
#define CH_SOH                  ( 0x01 ) ///< start of header character
#define CH_STX                  ( 0x02 ) ///< start of data character
#define CH_EOT                  ( 0x04 ) ///< end of data character
#define CH_ACK                  ( 0x06 ) ///< acknowledge character
#define CH_DLE                  ( 0x10 ) ///< data link escape character
#define CH_NAK                  ( 0x15 ) ///< negative acknowledge character
#define CH_SPC                  ( 0xEF ) ///< special character for DLE DLE stuffing

/// define the broadcast address
#define BINPROT_BROADCAST_ADDR  ( 0 )

// enumerations ---------------------------------------------------------------
/// define the receive states
typedef enum _RCVSTATE
{
  RCV_STATE_IDLE = 0,     ///<  0 - idle state
  RCV_STATE_HDR1,         ///<  1 - header char 1 state
  RCV_STATE_DSTA,         ///<  2 - destination address state
  RCV_STATE_SRCA,         ///<  3 - source address state
  RCV_STATE_CMND,         ///<  4 - command byte state
  RCV_STATE_OPTN,         ///<  5 - option byte state
  RCV_STATE_SEQN,         ///<  6 - sequence number
  RCV_STATE_HDR2,         ///<  7 - header char 2 state
  RCV_STATE_CTRL,         ///<  8 - control byte state
  RCV_STATE_DATA,         ///<  9 - data byte state
  RCV_STATE_CRCM,         ///< 10 - MSB CRC byte state
  RCV_STATE_CRCL,         ///< 11 - LSB CRC byte state
  RCV_STATE_MAX
} RCVSTATE;

/// enumerate the local protocol status
typedef enum _LCLSTS
{
  LCL_STS_IDLE = 0,       ///< 0 - idle
  LCL_STS_BUSY,           ///< 1 - busy
  LCL_STS_RCVD,           ///< 2 - message received
  LCL_STS_CHKERR,         ///< 3 - check error
} LCLSTS;

/// enumerate the master mode events
typedef enum _MASTEVENT
{
  MAST_EVENT_SNDMSG = 0x80, ///< 0x80 - send message
  MAST_EVENT_RCVOK,         ///< 0x81 - receive OK
  MAST_EVENT_RCVERR,        ///< 0x82 - error
} MASTEVENT;

/// enumerate the master states
typedef enum _MASTSTATE
{
  MAST_STATE_IDLE = 0,    ///< 0 - idle state
  MAST_STATE_WAIT,        ///< 1 - wait state
} MASTSTATE;

// structures -----------------------------------------------------------------
/// define the check structure
typedef struct _CHECKCTL
{
  U16UN tCrc;      	      ///< current message CRC
  U8    nValue;           ///< EOR check 
} CHECKCTL, *PCHECKCTL;
#define CHECKCTL_SIZE   sizeof( CHECKCTL )

/// define the buffer structure
typedef struct _LCLBUFCTL
{
  U16             wIndex;       ///< current index into the buffer
  U16UN           tCheck;       ///< check mode
  U8              nCommand;     ///< command
  U8              nOption;      ///< option
  U8              nSrcDstAddr;  ///< the src addr on incoming and dst addr on outgoing
  U8              nSequence;    ///< sequence number
  BOOL            bDataBlock;   ///< data block present
  PU8             pnBuffer;     ///< pointer to the buffer
  BINCMDCHECKMODE eCheckMode;   ///< check mode
} LCLBUFCTL, *PLCLBUFCTL;
#define LCLBUFCTL_SIZE    sizeof( LCLBUFCTL )

/// define the control structure
typedef struct _PROTCTRL
  {
  LCLBUFCTL               tXmtBuffer;    	    ///< current write buffer control
  LCLBUFCTL               tRcvBuffer;         ///< current read buffer control
  U16UN                   tRcvActualCheck;    ///< current received actual CRC
  STATEEXECENGCONTROL     tStateCtl;          ///< the state control structure
  BINCMDSTS               eCurSts;            ///< current return status
  LCLSTS                  eLclSts;            ///< local status
  U8                      nLclAddr;           ///< this protocols instantiations address (multidrop mode)
  #if ( BINCMDHAND_ENABLE_MASTERMODE == 1 )
  BOOL                    bMasterInProgress;  ///< master in progress
  BINCMDMSTENTRY const *  ptCurMstCmdTable;   ///< the current master message
  MASTSTATE               eMastState;         ///< master state
  #endif // BINCMDHAND_ENABLE_MASTERMODE
  BINCMDSLVENTRY const *  atTableStack[ BINARYCOMMANDHANDLER_TABLE_STACK_DEPTH ]; ///< maximum number of tables
  U8                      nStackIndex;
} PROTCTRL, *PPROTCTRL;
#define PROTCTRL_SIZE     sizeof( PROTCTRL )

// global parameter declarations ----------------------------------------------

// local parameter declarations -----------------------------------------------
static  PROTCTRL                atCtrls[ BINCMD_ENUM_MAX ];
static  PPROTCTRL               ptLclCtl;  
static  PBINCMDDEF              ptLclDef;
static  PVBINWRITEFUNC          pvLclWriteFunc;

// local function prototypes --------------------------------------------------
  #if ( BINCMDHAND_ENABLE_MASTERMODE == 1 )
static  void        ProcessMasterCallback( BINCMDENUM eProtEnum, MASTEVENT eEvent );
  #endif // BINCMDHAND_ENABLE_MASTERMODE
static  BOOL        CheckAddress( U8 nAddr );
static  BOOL        StuffRcvData( U8 nRcvChar );
static  void        StuffXmtData( PLCLBUFCTL ptBufCtl, U8 nData, BOOL bEscapeEnb );
static  BINCMDSTS   ProcessRcvdMsg( BINCMDENUM eProtEnum, U8 nCompareValue );
static  BINPARSESTS ParseCommand( BINCMDENUM eProtEnum, U16 wRcvLen, U8 nCompareValue );

// RCV_STATE_IDLE functions
static  void  RcvStateIdleEnt( void );
static  void  RcvStateIdleExt( void );

// RCV_STATE_HDR1 functions
static  U8    RcvStateHdr1Exc( STATEEXECENGARG xArg );

// RCV_STATE_DSTA functions
static  U8    RcvStateDstaExc( STATEEXECENGARG xArg );

// RCV_STATE_SRCA functions
static  U8    RcvStateSrcaExc( STATEEXECENGARG xArg );

// RCV_STATE_CMND functions
static  U8    RcvStateCmndExc( STATEEXECENGARG xArg );

// RCV_STATE_OPTN functions
static  U8    RcvStateOptnExc( STATEEXECENGARG xArg );

// RCV_STATE_SEQN functions
static  U8    RcvStateSeqnExc( STATEEXECENGARG xArg );

// RCV_STATE_CTRL functions
static  U8    RcvStateCtrlExc( STATEEXECENGARG xArg );

// RCV_STATE_DATA functions
static  U8    RcvStateDataExc( STATEEXECENGARG xArg );

// RCV_STATE_CRCM functions
static  U8    RcvStateCrcmExc( STATEEXECENGARG xArg );

// RCV_STATE_CRCL functions
static  U8    RcvStateCrclExc( STATEEXECENGARG xArg );

// constant parameter initializations -----------------------------------------
/// initialize the idle events
static  const CODE STATEEXECENGEVENT  atIdleEvents[ ] =
{
 STATEEXECENGEVENT_ENTRY( CH_DLE, RCV_STATE_HDR1, TRUE  ),
 STATEEXECENGEVENT_END( )
};

/// intialize the any events
static  const CODE STATEEXECENGEVENT  atAnyStateEvents[ ] =
{
 STATEEXECENGEVENT_ENTRY( CH_DLE, RCV_STATE_CTRL, FALSE ),
 STATEEXECENGEVENT_END( )
};

/// initialize the control events
static  const CODE STATEEXECENGEVENT  atRcvCtrlEvents[ ] =
{
 STATEEXECENGEVENT_ENTRY( CH_STX, RCV_STATE_DATA, FALSE ),
 STATEEXECENGEVENT_ENTRY( CH_EOT, RCV_STATE_CRCM, FALSE ),
 STATEEXECENGEVENT_END( )
};

/// initialize the main state table
static  const CODE STATEEXECENGTABLE  atRcvStates[ RCV_STATE_MAX ] =
{
 STATEXECENGETABLE_ENTRY( RCV_STATE_IDLE, RcvStateIdleEnt, NULL,            RcvStateIdleExt, atIdleEvents        ),
 STATEXECENGETABLE_ENTRY( RCV_STATE_HDR1, NULL,            RcvStateHdr1Exc, NULL,            NULL                ),
 STATEXECENGETABLE_ENTRY( RCV_STATE_DSTA, NULL,            RcvStateDstaExc, NULL,            NULL                ),
 STATEXECENGETABLE_ENTRY( RCV_STATE_SRCA, NULL,            RcvStateSrcaExc, NULL,            atAnyStateEvents    ),
 STATEXECENGETABLE_ENTRY( RCV_STATE_CMND, NULL,            RcvStateCmndExc, NULL,            atAnyStateEvents    ),
 STATEXECENGETABLE_ENTRY( RCV_STATE_OPTN, NULL,            RcvStateOptnExc, NULL,            atAnyStateEvents    ),
 STATEXECENGETABLE_ENTRY( RCV_STATE_SEQN, NULL,            RcvStateSeqnExc, NULL,            atAnyStateEvents    ),
 STATEXECENGETABLE_ENTRY( RCV_STATE_HDR2, NULL,            NULL,            NULL,            atAnyStateEvents    ),
 STATEXECENGETABLE_ENTRY( RCV_STATE_CTRL, NULL,            RcvStateCtrlExc, NULL,            atRcvCtrlEvents     ),
 STATEXECENGETABLE_ENTRY( RCV_STATE_DATA, NULL,            RcvStateDataExc, NULL,            atAnyStateEvents    ),
 STATEXECENGETABLE_ENTRY( RCV_STATE_CRCM, NULL,            RcvStateCrcmExc, NULL,            NULL                ),
 STATEXECENGETABLE_ENTRY( RCV_STATE_CRCL, NULL,            RcvStateCrclExc, NULL,            NULL                ),
};

/******************************************************************************
 * @function BinaryCommandHandler_Initialize
 *
 * @brief protocol initialization
 *
 * This function initialize each protocol instantation in the configuration 
 * table
 *
 *****************************************************************************/
void BinaryCommandHandler_Initialize( void )
{
  BINCMDENUM eProtEnum;
  
  // for each protocol
  for( eProtEnum = 0; eProtEnum < BINCMD_ENUM_MAX; eProtEnum++ )
  {
    // get the pointers
    ptLclCtl = &atCtrls[ eProtEnum ];  
    ptLclDef = ( PBINCMDDEF )&atBinCmdDefs[ eProtEnum ];
    
    // set the state engine
    ptLclCtl->tStateCtl.ptStates = atRcvStates;
    
    // clear the indices
    ptLclCtl->tXmtBuffer.wIndex = 0;
    ptLclCtl->tRcvBuffer.wIndex = 0;
    
    // set the check mode
    ptLclCtl->tRcvBuffer.eCheckMode = PGM_RDBYTE( ptLclDef->eCheckMode );
    ptLclCtl->tXmtBuffer.eCheckMode = ptLclCtl->tRcvBuffer.eCheckMode;

    // clear the command table stack index
    ptLclCtl->nStackIndex = 0;
    
    // set the buffer addresses
    ptLclCtl->tRcvBuffer.pnBuffer = ( PU8 )PGM_RDWORD( ptLclDef->pnRcvBuffer );
    if ( PGM_RDBYTE( ptLclDef->bDualBufferMode ) == TRUE )
    {
      // set it to the transmit buffer
      ptLclCtl->tXmtBuffer.pnBuffer = ( PU8 )PGM_RDWORD( ptLclDef->pnXmtBuffer );
    }
    else
    {
      // set it to the receive buffer
      ptLclCtl->tXmtBuffer.pnBuffer = ( PU8 )PGM_RDWORD( ptLclDef->pnRcvBuffer );
    }
    
    // initialize the state
    StateExecutionEngine_Initialize( &ptLclCtl->tStateCtl, RCV_STATE_IDLE );

    // if multidrop mode, set the local address
    if ( PGM_RDBYTE( ptLclDef->bMultiDropMode ) == TRUE )
    {
      // set the local address
      ptLclCtl->nLclAddr = PGM_RDBYTE( ptLclDef->nDevAddress );
    }
  }
}

#if ( BINCMDHAND_ENABLE_MASTERMODE == 1 )
/******************************************************************************
 * @function BinaryCommandHandler_SendMstMessage
 *
 * @brief send a master message
 *
 * This function will post a master message to the appropriate master protocol
 *
 * @param[in]   eProtEnum   protocol enumeration
 * @param[in]   nMstCmdEnum master command enumeration
 * @param[in]   nOption     option to send
 * @param[in]   nDstAddr    destination address if multidrop mode
 * 
 * @return      appropriate protocol status/error
 *
 *****************************************************************************/
BINCMDSTS BinaryCommandHandler_SendMstMessage( BINCMDENUM eProtEnum, U8 nMstCmdEnum, U8 nOption, U8 nDstAddr )
{
  BINCMDSTS               eStatus = BINCMD_STS_IDLE;
  BINCMDMSTENTRY const *  ptCmdTbl;
  U8                      nCommand;

  // get the pointers to the control
  ptLclCtl = &atCtrls[ eProtEnum ];
    
  // check for a valid protocol
  if ( eProtEnum < BINCMD_ENUM_MAX )
  {
    // test for busy
    if ( ptLclCtl->eLclSts == LCL_STS_IDLE )
    {
      // test for master mode
      if ( PGM_RDBYTE( atBinCmdDefs[ eProtEnum ].bMasterMode ) == TRUE )
      {
        // test for a valid command
        if ( nMstCmdEnum < ( U8 )PGM_RDBYTE( atBinCmdDefs[ eProtEnum ].nCmdTblLen ))
        {
          // get a pointer to the command table
          ptCmdTbl = ( BINCMDMSTENTRY const * )PGM_RDWORD( atBinCmdDefs[ eProtEnum ].ptMstCmdTbl );  
          ptLclCtl->ptCurMstCmdTable = ( ptCmdTbl + nMstCmdEnum );

          // set master in progress
          ptLclCtl->bMasterInProgress = TRUE;

          // set the destination address for this protocol
          BinaryCommandHandler_SetDstAddr( eProtEnum, nDstAddr );
          
          // set the command byte
          if (( sSpecialCmd >= 0 ) && ( sSpecialCmd <= 0xFF ))
          {
            // set the command to the special command
            nCommand = ( U8 )sSpecialCmd;            
          }
          else if ( sSpecialCmd == -1 )
          {
            // set the command to the value from the table
            nCommand = PGM_RDBYTE( ptLclCtl->ptCurMstCmdTable->nCommand );
          }
          else
          {
            // illegal command character
            eStatus = BINCMD_STS_ILLMSTCMD;
          }
          
          // now check for continued processing
          if ( eStatus == BINCMD_STS_IDLE )
          {
            BinaryCommandHandler_BeginMessage( eProtEnum, nCommand, nOption );

            // process the callback
            ProcessMasterCallback( eProtEnum, MAST_EVENT_SNDMSG );
          }
        }
        else
        {
          // report the error
          eStatus = BINCMD_STS_ILLMSTCMD;
        }
      }
      else
      {
        // report error
        eStatus = BINCMD_STS_NOTMASTERDEV;
      }
    }
    else
    {
      // report busy error
      eStatus = BINCMD_STS_BUSBUSY;
    }
  }
  else
  {
    // set the error
    eStatus = BINCMD_STS_ILLPROTENUM;
  }

  // return the status
  return( eStatus );
}
#endif // BINCMDHAND_ENABLE_MASTERMODE

/******************************************************************************
 * @function BinaryCommandHandler_ProcessChar
 *
 * @brief process a character
 *
 * This function will process a character and report back the various states
 *
 * @param[in]   eProtEnum   protocol enumeration
 * @param[in]   nRcvChar    character to process
 *
 * @return      appropriate protocol status/error
 *
 *****************************************************************************/
BINCMDSTS BinaryCommandHandler_ProcessChar( BINCMDENUM eProtEnum, U8 nRcvChar, U8 nCompareValue )
{
  BINCMDSTS       eStatus = BINCMD_STS_ILLPROTENUM;
  
  // check for a valid protocol
  if ( eProtEnum < BINCMD_ENUM_MAX )
  {
    // set the default status
    eStatus = BINCMD_STS_IDLE;
    
    // get the pointers to the control/definition structures
    ptLclCtl = &atCtrls[ eProtEnum ];  
    ptLclDef = ( PBINCMDDEF )&atBinCmdDefs[ eProtEnum ];
    
    // add to CRC if we are in the CRC state
    if ( ptLclCtl->tStateCtl.nCurState < RCV_STATE_CRCM )
    {
      // do it
      switch( ptLclCtl->tRcvBuffer.eCheckMode )
      {
        case BINCMD_CHECKMODE_EOR :
          ptLclCtl->tRcvBuffer.tCheck.anValue[ LE_U16_LSB_IDX ] ^= nRcvChar;
          break;
          
        case BINCMD_CHECKMODE_CMP :
          ptLclCtl->tRcvBuffer.tCheck.anValue[ LE_U16_LSB_IDX ] += nRcvChar;
          break;
          
        case BINCMD_CHECKMODE_CRC :
          ptLclCtl->tRcvBuffer.tCheck.wValue = CRC16_CalculateByte( ptLclCtl->tRcvBuffer.tCheck.wValue, nRcvChar );
          break;
          
        default :
          break;
      }
    }

    #if ( BINARYCOMMANDHANDLER_ENABLE_DEBUG == 1 )
    DebugManager_AddElement( BINARYCOMMANDHANDLER_DEBUG_BASE | ptLclCtl->tStateCtl.nCurState, nRcvChar );
    #endif // BINARYCOMMANDHANDLER_ENABLE_DEBUG
    // process the event through the state engine
    StateExecutionEngine_Process( &ptLclCtl->tStateCtl, nRcvChar );

    // determine if we have a message
    switch( ptLclCtl->eLclSts )
    {
      case LCL_STS_BUSY :
        // set the return status to message in progress
        eStatus = BINCMD_STS_MSG_INPROG;
        break;
        
      case LCL_STS_RCVD :
        #if ( BINCMDHAND_ENABLE_MASTERMODE == 1 )
        // check for master mode
        if ( ptLclCtl->bMasterInProgress )
        {
          // post a complete event
          ProcessMasterCallback( eProtEnum, MAST_EVENT_RCVOK );

          // clear the master mode
          ptLclCtl->bMasterInProgress = FALSE;
          ptLclCtl->eLclSts = LCL_STS_IDLE;
        }
        else
        #endif  // INCMDHAND_ENABLE_MASTERMODE
        {
          // process the received message
          eStatus = ProcessRcvdMsg( eProtEnum, nCompareValue );
          ptLclCtl->eLclSts = LCL_STS_IDLE;
        }
        break;
        
      case LCL_STS_CHKERR :
        #if ( BINCMDHAND_ENABLE_MASTERMODE == 1 )
        // if not master mode/send error response
        if ( ptLclCtl->bMasterInProgress )
        {
          // post a complete event
          ProcessMasterCallback( eProtEnum, MAST_EVENT_RCVERR );

          // clear the master mode
          ptLclCtl->bMasterInProgress = FALSE;
          ptLclCtl->eLclSts = LCL_STS_IDLE;
        }
        else
        #endif  // INCMDHAND_ENABLE_MASTERMODE
        {
          // send error/set status
          BinaryCommandHandler_BeginMessage( eProtEnum, CH_NAK, ptLclCtl->eLclSts );
          BinaryCommandHandler_SendMessage( eProtEnum );
        }
        eStatus = BINCMD_STS_CRCERR;
        ptLclCtl->eLclSts = LCL_STS_IDLE;
        break;
        
      default :
      case LCL_STS_IDLE :
        eStatus = BINCMD_STS_IDLE;
        break;
    }    
  }

  // return the status
  return( eStatus );
}

/******************************************************************************
 * @function BinaryCommandHandler_ResetProtocol
 *
 * @brief reset the protocol states
 *
 * This function will reset the protocol handler to its initialized state
 *
 * @param[in]   eProtEnum   protocol enumeration
 *
 * @return      appropriate protocol status/error
 *
 *****************************************************************************/
BINCMDSTS BinaryCommandHandler_ResetProtocol( BINCMDENUM eProtEnum )
{
  BINCMDSTS eStatus = BINCMD_STS_ILLPROTENUM;
  
  // check for a valid protocol
  if ( eProtEnum < BINCMD_ENUM_MAX )
  {
    // get the pointers to the control
    ptLclCtl = &atCtrls[ eProtEnum ];  
    
    // reset the state
    ptLclCtl->tStateCtl.nLastState = RCV_STATE_IDLE;
    ptLclCtl->tStateCtl.nCurState = RCV_STATE_IDLE;
    
    // reset the status(s)
    ptLclCtl->eCurSts = BINCMD_STS_IDLE;
    ptLclCtl->eLclSts = LCL_STS_IDLE;

    // if master mode
    #if ( BINCMDHAND_ENABLE_MASTERMODE == 1 )
    ptLclCtl->bMasterInProgress = FALSE;
    ptLclCtl->eMastState = MAST_STATE_IDLE;
    #endif // BINCMDHAND_ENABLE_MASTERMOD
    
    // return idle
    eStatus = BINCMD_STS_IDLE;
  }

  // return the status
  return( eStatus );
}

/******************************************************************************
 * @function BinaryCommandHandler_ResetXmtLength
 *
 * @brief reset the transmit length
 *
 * This function will reset the transmit length
 *
 * @param[in]   eProtEnum   protocol enumeration
 *
 * @return      appropriate protocol status/error
 *
 *****************************************************************************/
BINCMDSTS BinaryCommandHandler_ResetXmtLength( BINCMDENUM eProtEnum )
{
  BINCMDSTS eStatus = BINCMD_STS_ILLPROTENUM;
  
  // check for a valid protocol
  if ( eProtEnum < BINCMD_ENUM_MAX )
  {
    // get the pointers to the control/definition structures
    ptLclCtl = &atCtrls[ eProtEnum ];  
    
    // reset the transmit length
    ptLclCtl->tXmtBuffer.wIndex = 0;
    
    // set status to idle
    eStatus = BINCMD_STS_IDLE;
  }

  // return the status
  return( eStatus );
}

/******************************************************************************
 * @function BinaryCommandHandler_BeginMessage
 *
 * @brief begin a message
 *
 * This function will reset the length, reset to idle, set the command
 * set the option, destination and source address's if multidrop and
 * compute the offset
 *
 * @param[in]   eProtEnum   protocol enumeration
 * @param[in]   nCommand    command 
 * @param[in]   nOption     option
 *
 * @return      appropriate protocol status/error
 *
 *****************************************************************************/
BINCMDSTS BinaryCommandHandler_BeginMessage( BINCMDENUM eProtEnum, U8 nCommand, U8 nOption )
{
  BINCMDSTS       eStatus = BINCMD_STS_ILLPROTENUM;
  PLCLBUFCTL      ptBufCtl;

  // check for a valid protocol
  if ( eProtEnum < BINCMD_ENUM_MAX )
  {
    // get the pointers to the control/definition structures
    ptLclCtl = &atCtrls[ eProtEnum ];  
    ptLclDef = ( PBINCMDDEF )&atBinCmdDefs[ eProtEnum ];

    // get a pointer to the transmit buffer
    ptBufCtl = &ptLclCtl->tXmtBuffer;
    
    // reset the check
    switch( ptBufCtl->eCheckMode )
    {
      case BINCMD_CHECKMODE_EOR :
      case BINCMD_CHECKMODE_CMP :
      ptBufCtl->tCheck.anValue[ LE_U16_LSB_IDX ] = 0;
      break;
          
      case BINCMD_CHECKMODE_CRC :
      ptBufCtl->tCheck.wValue = CRC16_GetInitialValue( );
      break;
          
      default :
      break;
    }

    // now reset the length/clear data block
    ptBufCtl->wIndex = 0;
    ptBufCtl->bDataBlock = FALSE;
    
    // now add the header
    StuffXmtData( ptBufCtl, CH_DLE, FALSE );
    StuffXmtData( ptBufCtl, CH_SOH, FALSE );
    
    // test for multidrop
    if ( PGM_RDBYTE( ptLclDef->bMultiDropMode ) == TRUE)
    {
      // send the destination/source address's
      StuffXmtData( ptBufCtl, ptBufCtl->nSrcDstAddr, TRUE );
      StuffXmtData( ptBufCtl, ptLclCtl->nLclAddr, TRUE );
    }
    
    // send the command/option bytes
    StuffXmtData( ptBufCtl, nCommand, TRUE );
    StuffXmtData( ptBufCtl, nOption, TRUE );

    // check for sequence numbers
    if ( PGM_RDBYTE( ptLclDef->bSequenceEnable ))
    {
      // send the sequence number
      StuffXmtData( ptBufCtl, ptBufCtl->nSequence, TRUE );
    }
    
    // set status to idle
    eStatus = BINCMD_STS_IDLE;
  }

  // return the status
  return( eStatus );
}

/******************************************************************************
 * @function BinaryCommandHandler_SetOption
 *
 * @brief set the option bye
 *
 * This function will set the option byte in the transmit buffer
 *
 * @param[in]   eProtEnum   protocol enumeration
 * @param[in]   nOption     option byte
 *
 * @return      appropriate protocol status/error
 *
 *****************************************************************************/
BINCMDSTS BinaryCommandHandler_SetOption( BINCMDENUM eProtEnum, U8 nOption )
{
  BINCMDSTS eStatus = BINCMD_STS_ILLPROTENUM;
  
  // check for a valid protocol
  if ( eProtEnum < BINCMD_ENUM_MAX )
  {
    // get the pointers to the control/definition structures
    ptLclCtl = &atCtrls[ eProtEnum ];  
    ptLclDef = ( PBINCMDDEF )&atBinCmdDefs[ eProtEnum ];
    
    // check for dual buffer
    if ( PGM_RDBYTE( ptLclDef->bDualBufferMode ) == TRUE )
    {
      // set the option byte in transmit buffer
      ptLclCtl->tXmtBuffer.nOption = nOption;
    }
    else
    {
      // set the option byte in the receive buffer
      ptLclCtl->tRcvBuffer.nOption = nOption;
    }
    
    // set status to idle
    eStatus = BINCMD_STS_IDLE;
  }

  // return the status
  return( eStatus );
}

/******************************************************************************
 * @function BinaryCommandHandler_SetSequence
 *
 * @brief set the sequence number
 *
 * This function set the sequence number in the buffer
 *
 * @param[in]   eProtEnum   protocol enumeration
 * @param[in]   nSequence   sequence number
 *
 * @return      appropriate protocol status/error
 *
 *****************************************************************************/
BINCMDSTS BinaryCommandHandler_SetSequence( BINCMDENUM eProtEnum, U8 nSequence )
{
  BINCMDSTS eStatus = BINCMD_STS_ILLPROTENUM;
  
  // check for a valid protocol
  if ( eProtEnum < BINCMD_ENUM_MAX )
  {
    // get the pointers to the control/definition structures
    ptLclCtl = &atCtrls[ eProtEnum ];  
    
    // set the option byte in transmit buffer
    ptLclCtl->tXmtBuffer.nSequence = nSequence;
    
    // set status to idle
    eStatus = BINCMD_STS_IDLE;
  }

  // return the status
  return( eStatus );
}

/******************************************************************************
 * @function BinaryCommandHandler_SetDstAddr
 *
 * @brief set the destination addres
 *
 * This function set the destination address
 *
 * @param[in]   eProtEnum   protocol enumeration
 * @param[in]   nDstAddr    destination address
 *
 * @return      appropriate protocol status/error
 *
 *****************************************************************************/
BINCMDSTS BinaryCommandHandler_SetDstAddr( BINCMDENUM eProtEnum, U8 nDstAddr )
{
  BINCMDSTS eStatus = BINCMD_STS_ILLPROTENUM;
  
  // check for a valid protocol
  if ( eProtEnum < BINCMD_ENUM_MAX )
  {
    // get the pointers to the control/definition structures
    ptLclCtl = &atCtrls[ eProtEnum ];  
    
    // set the option byte in transmit buffer
    ptLclCtl->tXmtBuffer.nSrcDstAddr = nDstAddr;
    
    // set status to idle
    eStatus = BINCMD_STS_IDLE;
  }

  // return the status
  return( eStatus );
}

/******************************************************************************
 * @function BinaryCommandHandler_SetLclAddr
 *
 * @brief set the local address
 *
 * This function sets the local address
 *
 * @param[in]   eProtEnum   protocol enumeration
 * @param[in]   nLclAddr    local address
 *
 * @return      appropriate protocol status/error
 *
 *****************************************************************************/
BINCMDSTS BinaryCommandHandler_SetLclAddr( BINCMDENUM eProtEnum, U8 nLclAddr )
{
  BINCMDSTS eStatus = BINCMD_STS_ILLPROTENUM;
  
  // check for a valid protocol
  if ( eProtEnum < BINCMD_ENUM_MAX )
  {
    // get the pointers to the control/definition structures
    ptLclCtl = &atCtrls[ eProtEnum ];  
    
    // set the option byte in transmit buffer
    ptLclCtl->nLclAddr = nLclAddr;
    
    // set status to idle
    eStatus = BINCMD_STS_IDLE;
  }

  // return the status
  return( eStatus );
}

/******************************************************************************
 * @function BinaryCommandHandler_GetCommand
 *
 * @brief get the current command
 *
 * This function will return the current command byte
 *
 * @param[in]   eProtEnum   protocol enumeration
 * @param[io]   pnCommand   pointer to store the command byte in
 *
 * @return      appropriate protocol status/error
 *
 *****************************************************************************/
BINCMDSTS BinaryCommandHandler_GetCommand( BINCMDENUM eProtEnum, PU8 pnCommand )
{
  BINCMDSTS eStatus = BINCMD_STS_ILLPROTENUM;
  
  // check for a valid protocol
  if ( eProtEnum < BINCMD_ENUM_MAX )
  {
    // get the pointers to the control/definition structures
    ptLclCtl = &atCtrls[ eProtEnum ];  

    // get the command byte in the receive buffer
    *pnCommand = ptLclCtl->tRcvBuffer.nCommand;
    
    // set status to idle
    eStatus = BINCMD_STS_IDLE;
  }

  // return the status
  return( eStatus );
}

/******************************************************************************
 * @function BinaryCommandHandler_GetOption
 *
 * @brief get the option byte
 *
 * This function will return the current option byte
 *
 * @param[in]   eProtEnum   protocol enumeration
 * @param[io]   pnCommand   pointer to store the command byte in
 *
 * @return      appropriate protocol status/error
 *
 *****************************************************************************/
BINCMDSTS BinaryCommandHandler_GetOption( BINCMDENUM eProtEnum, PU8 pnOption )
{
  BINCMDSTS eStatus = BINCMD_STS_ILLPROTENUM;
  
  // check for a valid protocol
  if ( eProtEnum < BINCMD_ENUM_MAX )
  {
    // get the pointers to the control/definition structures
    ptLclCtl = &atCtrls[ eProtEnum ];  
    
    // get the option byte in the receive buffer
    *pnOption = ptLclCtl->tRcvBuffer.nOption;
    
    // set status to idle
    eStatus = BINCMD_STS_IDLE;
  }

  // return the status
  return( eStatus );
}

/******************************************************************************
 * @function BinaryCommandHandler_GetSequence
 *
 * @brief get sequence number
 *
 * This function returns the sequence number
 *
 * @param[in]   eProtEnum   protocol enumeration
 * @param[io]   pnSequence  pointer to store the sequence number
 *
 * @return      appropriate protocol status/error
 *
 *****************************************************************************/
BINCMDSTS BinaryCommandHandler_GetSequence( BINCMDENUM eProtEnum, PU8 pnSequence )
{
  BINCMDSTS eStatus = BINCMD_STS_ILLPROTENUM;
  
  // check for a valid protocol
  if ( eProtEnum < BINCMD_ENUM_MAX )
  {
    // get the pointers to the control/definition structures
    ptLclCtl = &atCtrls[ eProtEnum ];  
    
    // get the option byte in the receive buffer
    *pnSequence = ptLclCtl->tRcvBuffer.nSequence;
    
    // set status to idle
    eStatus = BINCMD_STS_IDLE;
  }

  // return the status
  return( eStatus );
}

/******************************************************************************
 * @function BinaryCommandHandler_GetSrcAddr
 *
 * @brief get the source address
 *
 * This function will return the source address
 *
 * @param[in]   eProtEnum   protocol enumeration
 * @param[io]   pnSrcAddr   pointer to the srouce address
 *
 * @return      appropriate protocol status/error
 *
 *****************************************************************************/
BINCMDSTS BinaryCommandHandler_GetSrcAddr( BINCMDENUM eProtEnum, PU8 pnSrcAddr )
{
  BINCMDSTS eStatus = BINCMD_STS_ILLPROTENUM;
  
  // check for a valid protocol
  if ( eProtEnum < BINCMD_ENUM_MAX )
  {
    // get the pointers to the control
    ptLclCtl = &atCtrls[ eProtEnum ];  
    
    // return the source address from the receive buffer
    *pnSrcAddr = ptLclCtl->tRcvBuffer.nSrcDstAddr;
    
    // set status to idle
    eStatus = BINCMD_STS_IDLE;
  }

  // return the status
  return( eStatus );
}

/******************************************************************************
 * @function BinaryCommandHandler_GetLclAddr
 *
 * @brief get the local address
 *
 * This function will return the source address
 *
 * @param[in]   eProtEnum   protocol enumeration
 * @param[io]   pnLclAddr   pointer to the local address
 *
 * @return      appropriate protocol status/error
 *
 *****************************************************************************/
BINCMDSTS BinaryCommandHandler_GetLclAddr( BINCMDENUM eProtEnum, PU8 pnLclAddr )
{
  BINCMDSTS eStatus = BINCMD_STS_ILLPROTENUM;
  
  // check for a valid protocol
  if ( eProtEnum < BINCMD_ENUM_MAX )
  {
    // get the pointers to the control
    ptLclCtl = &atCtrls[ eProtEnum ];  
    
    // return the source address from the receive buffer
    *pnLclAddr = ptLclCtl->nLclAddr;
    
    // set status to idle
    eStatus = BINCMD_STS_IDLE;
  }

  // return the status
  return( eStatus );
}

/******************************************************************************
 * @function BinaryCommandHandler_GetRcvLength
 *
 * @brief get the receive length
 *
 * This function will return the receive lenth
 *
 * @param[in]   eProtEnum   protocol enumeration
 * @param[io]   pwRcvLenth  pointer to the receive length
 *
 * @return      appropriate protocol status/error
 *
 *****************************************************************************/
BINCMDSTS BinaryCommandHandler_GetRcvLength( BINCMDENUM eProtEnum, PU16 pwRcvLength )
{
  BINCMDSTS eStatus = BINCMD_STS_ILLPROTENUM;
  
  // check for a valid protocol
  if ( eProtEnum < BINCMD_ENUM_MAX )
  {
    // get the pointers to the control
    ptLclCtl = &atCtrls[ eProtEnum ];  
    
    // get the receive length
    *pwRcvLength = ptLclCtl->tRcvBuffer.wIndex;
    
    // set status to idle
    eStatus = BINCMD_STS_IDLE;
  }

  // return the status
  return( eStatus );
}

/******************************************************************************
 * @function BinaryCommandHandler_SetXmtLength
 *
 * @brief set the transmit length
 *
 * This function will set the transmit length
 *
 * @param[in]   eProtEnum     protocol enumeration
 * @param[io]   wXmtLength  transmit length
 *
 * @return      appropriate protocol status/error
 *
 *****************************************************************************/
BINCMDSTS BinaryCommandHandler_SetXmtLength( BINCMDENUM eProtEnum, U16 wXmtLength )
{
  BINCMDSTS eStatus = BINCMD_STS_ILLPROTENUM;
  
  // check for a valid protocol
  if ( eProtEnum < BINCMD_ENUM_MAX )
  {
    // get the pointers to the control
    ptLclCtl = &atCtrls[ eProtEnum ];
    
    // set the transmit length
    ptLclCtl->tXmtBuffer.wIndex = wXmtLength;
    
    // set status to idle
    eStatus = BINCMD_STS_IDLE;
  }

  // return the status
  return( eStatus );
}

/******************************************************************************
 * @function BinaryCommandHandler_SetMessageByte
 *
 * @brief set a message byte
 *
 * This function will set a message byte int he buffer
 *
 * @param[in]   eProtEnum   protocol enumeration
 * @parma[in]   nXmtChar    transmit character
 *
 * @return      appropriate protocol status/error
 *
 *****************************************************************************/
BINCMDSTS BinaryCommandHandler_SetMessageByte( BINCMDENUM eProtEnum, U8 nXmtChar )
{
  BINCMDSTS   eStatus = BINCMD_STS_ILLPROTENUM;
    
  // check for a valid protocol
  if ( eProtEnum < BINCMD_ENUM_MAX )
  {
    // get the pointers to the control/definition structures
    ptLclCtl = &atCtrls[ eProtEnum ];  
    
    // check for data hdear
    if ( ptLclCtl->tXmtBuffer.bDataBlock == FALSE )
    {
      // set it true
      ptLclCtl->tXmtBuffer.bDataBlock = TRUE;

      // stuff the data header
      StuffXmtData( &ptLclCtl->tXmtBuffer, CH_DLE, FALSE );
      StuffXmtData( &ptLclCtl->tXmtBuffer, CH_STX, FALSE );
    }
    
    // now stuff the data
    StuffXmtData( &ptLclCtl->tXmtBuffer, nXmtChar, TRUE );
  }

  // return the status
  return( eStatus );
}

/******************************************************************************
 * @function BinaryCommandHandler_GetMessageByte
 *
 * @brief get a message byte from the buffer
 *
 * This function returns a message byte
 *
 * @param[in]   eProtEnum   protocol enumeration
 * @param[in]   wIndex      buffer index
 * @param[in]   pnRcvChar   pointer to store the character
 *
 * @return      appropriate protocol status/error
 *
 *****************************************************************************/
BINCMDSTS BinaryCommandHandler_GetMessageByte( BINCMDENUM eProtEnum, U16 wIndex, PU8 pnRcvChar )
{
  BINCMDSTS  eStatus = BINCMD_STS_ILLPROTENUM;
  
  // check for a valid protocol
  if ( eProtEnum < BINCMD_ENUM_MAX )
  {
    // get the pointers to the control/definition structures
    ptLclCtl = &atCtrls[ eProtEnum ];  
  
    // check for buffer empty
    if ( ptLclCtl->tRcvBuffer.wIndex != 0 )
    {
      // check for index greater than buffer
      if ( wIndex < ptLclCtl->tRcvBuffer.wIndex )
      {
        // return the value
        *pnRcvChar = *( ptLclCtl->tRcvBuffer.pnBuffer + wIndex );
        
        // set good status
        eStatus = BINCMD_STS_IDLE;
      }
      else
      {
        // set status to illegal index
        eStatus = BINCMD_STS_ILLBUFIDX;
      }
    }
    else
    {
      // set status to impty
      eStatus = BINCMD_STS_BUFEMPTY;
    }
    
  }

  // return the status
  return( eStatus );
}

#ifdef __ATMEL_AVR__
/******************************************************************************
 * @function BinaryCommandHandler_SetMessageBlockP
 *
 * @brief set the message block
 *
 * This function will copy a message block into the transmit buffer
 *
 * @param[in]   eProtEnum   protocol enumeration
 *
 * @return      appropriate protocol status/error
 *
 *****************************************************************************/
BINCMDSTS BinaryCommandHandler_SetMessageBlockP( BINCMDENUM eProtEnum, PFU8 pnData, U16 wLength )
{
  BINCMDSTS   eStatus = BINCMD_STS_ILLPROTENUM;
  U16         wIndex;
  PLCLBUFCTL  ptBufCtl;
  
  // check for a valid protocol
  if ( eProtEnum < BINCMD_ENUM_MAX )
  {
    // get the pointers to the control/definition structures
    ptLclCtl = &atCtrls[ eProtEnum ];  
    ptBufCtl = &ptLclCtl->tXmtBuffer;

    // check for data hdear
    if ( ptLclCtl->tXmtBuffer.bDataBlock == FALSE )
    {
      // stuff the data header
      StuffXmtData( ptBufCtl, CH_DLE, FALSE );
      StuffXmtData( ptBufCtl, CH_STX, FALSE );
    }

    // for each byte
    for ( wIndex = 0; wIndex < wLength; wIndex++ )    
    {
      // stuff the data
      StuffXmtData( ptBufCtl, PGM_RDBYTE( *( pnData++ )), TRUE );
    }

    // set status to idle
    eStatus = BINCMD_STS_IDLE;
  }

  // return the status
  return( eStatus );
}
#endif

/******************************************************************************
 * @function BinaryCommandHandler_SetMessageBlock
 *
 * @brief set the message block
 *
 * This function will copy a message block into the transmit buffer
 *
 * @param[in]   eProtEnum   protocol enumeration
 *
 * @return      appropriate protocol status/error
 *
 *****************************************************************************/
BINCMDSTS BinaryCommandHandler_SetMessageBlock( BINCMDENUM eProtEnum, PU8 pnData, U16 wLength )
{
  BINCMDSTS   eStatus = BINCMD_STS_ILLPROTENUM;
  U16         wIndex;
  PLCLBUFCTL  ptBufCtl;
  
  // check for a valid protocol
  if ( eProtEnum < BINCMD_ENUM_MAX )
  {
    // get the pointers to the control/definition structures
    ptLclCtl = &atCtrls[ eProtEnum ];  
    ptBufCtl = &ptLclCtl->tXmtBuffer;
    
    // check for data hdear
    if ( ptBufCtl->bDataBlock == FALSE )
    {
      // set it true
      ptBufCtl->bDataBlock = TRUE;

      // stuff the data header
      StuffXmtData( ptBufCtl, CH_DLE, FALSE );
      StuffXmtData( ptBufCtl, CH_STX, FALSE );
    }

    // for each byte
    for ( wIndex = 0; wIndex < wLength; wIndex++ )
    {
      // stuff the data
      StuffXmtData( ptBufCtl, *( pnData++ ), TRUE );
    }
    
    // set status to idle
    eStatus = BINCMD_STS_IDLE;
  }

  // return the status
  return( eStatus );
}


/******************************************************************************
 * @function BinaryCommandHandler_GetMessagePointer
 *
 * @brief 
 *
 * This function 
 *
 * @param[in]   eProtEnum   protocol enumeration
 *
 * @return      appropriate protocol status/error
 *
 *****************************************************************************/
BINCMDSTS BinaryCommandHandler_GetRcvBufferPointer( BINCMDENUM eProtEnum, PU8* ppnBuffer )
{
  BINCMDSTS eStatus = BINCMD_STS_ILLPROTENUM;
  
  // check for a valid protocol
  if ( eProtEnum < BINCMD_ENUM_MAX )
  {
    // get the pointers to the control
    ptLclCtl = &atCtrls[ eProtEnum ];  

    // get the receive buffer size
    *ppnBuffer = ( PU8 )PGM_RDWORD( ptLclDef->pnRcvBuffer );

    // set status to idle
    eStatus = BINCMD_STS_IDLE;
  }

  // return the status
  return( eStatus );
}

/******************************************************************************
 * @function BinaryCommandHandler_GetXmtBufferPointer
 *
 * @brief 
 *
 * This function 
 *
 * @param[in]   eProtEnum   protocol enumeration
 *
 * @return      appropriate protocol status/error
 *
 *****************************************************************************/
BINCMDSTS BinaryCommandHandler_GetXmtBufferPointer( BINCMDENUM eProtEnum, PU8* ppnBuffer )
{
  BINCMDSTS eStatus = BINCMD_STS_ILLPROTENUM;
  
  // check for a valid protocol
  if ( eProtEnum < BINCMD_ENUM_MAX )
  {
    // get the pointers to the control
    ptLclCtl = &atCtrls[ eProtEnum ];
    if ( PGM_RDBYTE( ptLclDef->bDualBufferMode ) == TRUE )
    {
      // get the transmit buffer size
      *ppnBuffer = ( PU8 )PGM_RDWORD( ptLclDef->pnXmtBuffer );
    }
    else
    {
      *ppnBuffer = ( PU8 )PGM_RDWORD( ptLclDef->pnRcvBuffer );
    }

    // set status to idle
    eStatus = BINCMD_STS_IDLE;
  }

  // return the status
  return( eStatus );
}

/******************************************************************************
 * @function BinaryCommandHandler_GetXmtBufferPointer
 *
 * @brief 
 *
 * This function 
 *
 * @param[in]   eProtEnum   protocol enumeration
 *
 * @return      appropriate protocol status/error
 *
 *****************************************************************************/
BINCMDSTS BinaryCommandHandler_GetRcvBufferSize( BINCMDENUM eProtEnum, PU16 pwBufSize )
{
  BINCMDSTS eStatus = BINCMD_STS_ILLPROTENUM;
  
  // check for a valid protocol
  if ( eProtEnum < BINCMD_ENUM_MAX )
  {
    // get the pointers to the control
    ptLclDef = ( PBINCMDDEF )&atBinCmdDefs[ eProtEnum ];
    *( pwBufSize ) = PGM_RDWORD( ptLclDef->wRcvBufferSize );

    // set status to idle
    eStatus = BINCMD_STS_IDLE;
  }

  // return the status
  return( eStatus );
}

/******************************************************************************
 * @function BinaryCommandHandler_GetXmtBufferPointer
 *
 * @brief 
 *
 * This function 
 *
 * @param[in]   eProtEnum   protocol enumeration
 *
 * @return      appropriate protocol status/error
 *
 *****************************************************************************/
BINCMDSTS BinaryCommandHandler_GetXmtBufferSize( BINCMDENUM eProtEnum, PU16 pwBufSize )
{
  BINCMDSTS eStatus = BINCMD_STS_ILLPROTENUM;
  
  // check for a valid protocol
  if ( eProtEnum < BINCMD_ENUM_MAX )
  {
    // get the pointers to the control
    ptLclDef = ( PBINCMDDEF )&atBinCmdDefs[ eProtEnum ];
    *( pwBufSize ) = PGM_RDWORD( ptLclDef->wRcvBufferSize );

    // set status to idle
    eStatus = BINCMD_STS_IDLE;
  }

  // return the status
  return( eStatus );
}

/******************************************************************************
 * @function BinaryCommandHandler_SendMessage
 *
 * @brief sene the message
 *
 * This function formats and sends the message 
 *
 * @param[in]   eProtEnum   protocol enumeration
 *
 * @return      appropriate protocol status/error
 *
 *****************************************************************************/
BINCMDSTS BinaryCommandHandler_SendMessage( BINCMDENUM eProtEnum )
{
  BINCMDSTS   eStatus = BINCMD_STS_ILLPROTENUM;
  PLCLBUFCTL  ptBufCtl;
  
  // check for a valid protocol
  if ( eProtEnum < BINCMD_ENUM_MAX )
  {
    // get the pointers to the control/definition structures
    ptLclCtl = &atCtrls[ eProtEnum ];  
    ptBufCtl = &ptLclCtl->tXmtBuffer;
      
    // set the write function
    pvLclWriteFunc = ( PVBINWRITEFUNC )PGM_RDWORD( ptLclDef->pvWriteFunc );
    
    // send the trailer
    StuffXmtData( ptBufCtl, CH_DLE, FALSE );
    StuffXmtData( ptBufCtl, CH_EOT, FALSE );
    
    // reset the check
    switch( ptBufCtl->eCheckMode )
    {
      case BINCMD_CHECKMODE_EOR :
        // send the character
        StuffXmtData( ptBufCtl, ptBufCtl->tCheck.anValue[ LE_U16_LSB_IDX ], FALSE );
        break;

      case BINCMD_CHECKMODE_CMP :
        // send the two's complement
        ptBufCtl->tCheck.anValue[ LE_U16_LSB_IDX ] = ~ptBufCtl->tCheck.anValue[ LE_U16_LSB_IDX ];
        ptBufCtl->tCheck.anValue[ LE_U16_LSB_IDX ]++;
        StuffXmtData( ptBufCtl, ptBufCtl->tCheck.anValue[ LE_U16_LSB_IDX ], FALSE );
        break;
        
      case BINCMD_CHECKMODE_CRC :
        // just just the CRC MSB first
        StuffXmtData( ptBufCtl, ptBufCtl->tCheck.anValue[ LE_U16_MSB_IDX ], FALSE );
        StuffXmtData( ptBufCtl, ptBufCtl->tCheck.anValue[ LE_U16_LSB_IDX ], FALSE );
        break;
        
      default :
        break;
    }
    
    // now send the message
    pvLclWriteFunc( ptBufCtl->pnBuffer, ptBufCtl->wIndex );
  }

  // return the status
  return( eStatus );
}

/******************************************************************************
 * RCV_STATE_IDLE functions
 *****************************************************************************/

static void RcvStateIdleEnt( void )
{
  // reset the states
  // ptLclCtl->eLclSts = LCL_STS_IDLE;
}

static void RcvStateIdleExt( void )
{
  // set the states to busy
  ptLclCtl->eLclSts = LCL_STS_BUSY;
}

/******************************************************************************
 * RCV_STATE_HDR1 functions
 *****************************************************************************/
static U8 RcvStateHdr1Exc( STATEEXECENGARG xArg )
{
  U8  nNextState;
  
  // check for a SOH character
  if ( xArg == CH_SOH )
  {
    // reset the buffer index
    ptLclCtl->tRcvBuffer.wIndex = 0;
    
    // reset the CRC
    switch( PGM_RDBYTE( ptLclDef->eCheckMode ))
    {
      case BINCMD_CHECKMODE_EOR :
        ptLclCtl->tRcvBuffer.tCheck.anValue[ LE_U16_LSB_IDX ] = CH_DLE;
        ptLclCtl->tRcvBuffer.tCheck.anValue[ LE_U16_LSB_IDX ] ^= CH_SOH;
        break;
        
      case BINCMD_CHECKMODE_CMP :
        ptLclCtl->tRcvBuffer.tCheck.anValue[ LE_U16_LSB_IDX ] = CH_DLE + CH_SOH;
        break;
        
      case BINCMD_CHECKMODE_CRC :
        ptLclCtl->tRcvBuffer.tCheck.wValue = CRC16_GetInitialValue( );
        ptLclCtl->tRcvBuffer.tCheck.wValue = CRC16_CalculateByte( ptLclCtl->tRcvBuffer.tCheck.wValue, CH_DLE );
        ptLclCtl->tRcvBuffer.tCheck.wValue = CRC16_CalculateByte( ptLclCtl->tRcvBuffer.tCheck.wValue, CH_SOH );
        break;
        
      default :
        break;
    }
    
    // set the next state
    nNextState = ( PGM_RDBYTE( ptLclDef->bMultiDropMode ) == FALSE ) ? RCV_STATE_CMND : RCV_STATE_DSTA;
  }
  else
  {
    // back to idle
    nNextState = RCV_STATE_IDLE;
  }
  
  // return next state
  return( nNextState );
}

/******************************************************************************
 * RCV_STATE_DSTA functions
 *****************************************************************************/
static U8 RcvStateDstaExc( STATEEXECENGARG xArg )
{
  // if this is our address, next state is source/otherwise idle
  return(( CheckAddress( xArg ) == TRUE ) ? RCV_STATE_SRCA : RCV_STATE_IDLE );
}

/******************************************************************************
 * RCV_STATE_SRCA functions
 *****************************************************************************/
static U8 RcvStateSrcaExc( STATEEXECENGARG xArg )
{
  // store the source address/goto to RCV_STATE_CMND
  ptLclCtl->tRcvBuffer.nSrcDstAddr = xArg;
  return( RCV_STATE_CMND );
}

/******************************************************************************
 * RCV_STATE_CMND functions
 *****************************************************************************/
static U8 RcvStateCmndExc( STATEEXECENGARG xArg )
{
  // store the command/goto to RCV_STATE_OPTN
  ptLclCtl->tRcvBuffer.nCommand = xArg;
  return( RCV_STATE_OPTN );
}

/******************************************************************************
 * RCV_STATE_OPTN functions
 *****************************************************************************/
static U8 RcvStateOptnExc( STATEEXECENGARG xArg )
{
  U8  nNextState;
  
  // store the option/determine next state
  ptLclCtl->tRcvBuffer.nOption = xArg;
  
  // set the next state
  nNextState = ( PGM_RDBYTE( ptLclDef->bSequenceEnable ) == TRUE ) ? RCV_STATE_SEQN : RCV_STATE_HDR2;
  
  // return next state
  return( nNextState );
}

/******************************************************************************
 * RCV_STATE_SEQN functions
 *****************************************************************************/
static U8 RcvStateSeqnExc( STATEEXECENGARG xArg )
{
  // store the sequence/goto header 2
  ptLclCtl->tRcvBuffer.nSequence = xArg;
  return( RCV_STATE_HDR2 );
}

/******************************************************************************
 * RCV_STATE_CTRL functions
 *****************************************************************************/
static U8 RcvStateCtrlExc( STATEEXECENGARG xArg )
{
  U8  nNextState;
  
  // if this character is not a special character, we have detected a protocol error
  if ( xArg != CH_SPC )
  {
    // set the state back to idle
    nNextState = RCV_STATE_IDLE;
  }
  else
  {
    // determine the last state
    switch( ptLclCtl->tStateCtl.nLastState )
    {
      case RCV_STATE_DSTA :
        // if this is our address, next state is source/otherwise idle
        nNextState = ( CheckAddress( CH_DLE ) == TRUE ) ? RCV_STATE_SRCA : RCV_STATE_IDLE;
        break;
        
      case RCV_STATE_SRCA :
        // store the source address/goto to RCV_STATE_CMND
        ptLclCtl->tRcvBuffer.nSrcDstAddr = CH_DLE;
        nNextState = RCV_STATE_CMND;
        break;
        
      case RCV_STATE_CMND :
        // store the command/goto to RCV_STATE_OPTN
        ptLclCtl->tRcvBuffer.nCommand = CH_DLE;
        nNextState = RCV_STATE_OPTN;
        break;
        
      case RCV_STATE_OPTN :
        // store the option/determine next state
        ptLclCtl->tRcvBuffer.nOption = CH_DLE;
        
        // set the next state
        nNextState = ( PGM_RDBYTE( ptLclDef->bSequenceEnable ) == TRUE ) ? RCV_STATE_SEQN : RCV_STATE_HDR2;
        break;
        
      case RCV_STATE_SEQN :
        // store the sequence/goto header 2
        ptLclCtl->tRcvBuffer.nSequence = CH_DLE;
        return( RCV_STATE_HDR2 );
        break;
        
      case RCV_STATE_DATA :
        // stuff the data if okay, we got  data, otherwise back to idle
        nNextState = ( StuffRcvData( CH_DLE ) == TRUE ) ? RCV_STATE_DATA : RCV_STATE_IDLE;
        break;
        
      default :
        // error
        nNextState = RCV_STATE_IDLE;
        break;
    }
  }

  // return next state
  return( nNextState );
}

/******************************************************************************
 * RCV_STATE_DATA functions
 *****************************************************************************/
static U8 RcvStateDataExc( STATEEXECENGARG xArg )
{
  // stuff the data if okay, we got  data, otherwise back to idle
  return(( StuffRcvData( xArg ) == TRUE ) ? RCV_STATE_DATA : RCV_STATE_IDLE );
}

/******************************************************************************
 * RCV_STATE_CRCM functions
 *****************************************************************************/
static U8 RcvStateCrcmExc( STATEEXECENGARG xArg )
{
  U8  nNextState = RCV_STATE_IDLE;

  // determine the checksum
  switch( PGM_RDBYTE( ptLclDef->eCheckMode ))
  {
    case BINCMD_CHECKMODE_EOR :
      // compare for good
      ptLclCtl->eLclSts = ( ptLclCtl->tRcvBuffer.tCheck.anValue[ LE_U16_LSB_IDX ] == xArg ) ? LCL_STS_RCVD : LCL_STS_CHKERR;
      nNextState = RCV_STATE_IDLE;
      break;

    case BINCMD_CHECKMODE_CMP :
      // complement it
      ptLclCtl->tRcvBuffer.tCheck.anValue[ LE_U16_LSB_IDX ] = ~ptLclCtl->tRcvBuffer.tCheck.anValue[ LE_U16_LSB_IDX ];
      ptLclCtl->tRcvBuffer.tCheck.anValue[ LE_U16_LSB_IDX ]++;

      // compare for good
      ptLclCtl->eLclSts = ( ptLclCtl->tRcvBuffer.tCheck.anValue[ LE_U16_LSB_IDX ] == xArg ) ? LCL_STS_RCVD : LCL_STS_CHKERR;
      nNextState = RCV_STATE_IDLE;
      break;

    case BINCMD_CHECKMODE_CRC :
      // store MSB and got LSB
      ptLclCtl->tRcvActualCheck.anValue[ LE_U16_MSB_IDX ] = xArg;
      nNextState = RCV_STATE_CRCL;
      break;
        
    default :
      // error
      nNextState = RCV_STATE_IDLE;
      break;
  }

  // return next state
  return( nNextState );
}

/******************************************************************************
 * RCV_STATE_CRCL functions
 *****************************************************************************/
static U8 RcvStateCrclExc( STATEEXECENGARG xArg )
{
  // store the value
  ptLclCtl->tRcvBuffer.tCheck.anValue[ LE_U16_LSB_IDX ] = xArg;
  
  // check for valid
  ptLclCtl->eLclSts = ( ptLclCtl->tRcvBuffer.tCheck.wValue == ptLclCtl->tRcvActualCheck.wValue ) ? LCL_STS_RCVD : LCL_STS_CHKERR;
  
  // back to idle
  return( RCV_STATE_IDLE );
}

#if ( BINCMDHAND_ENABLE_MASTERMODE == 1 )
/******************************************************************************
 * @function ProcessMasterCallback
 *
 * @brief process an a master mode callback
 * 
 * This function will process the callback for handling master transmissions
 *
 * @param[in]   eProtEnum   protocol enumeration
 * @param[in]   eEvent      master event
 *
 *****************************************************************************/
static void ProcessMasterCallback( BINCMDENUM eProtEnum, MASTEVENT eEvent )
{
  PVBINMSTCMDRSPFUNC ptMstCmdTblRspFunc;
  
  // test for a valid protocol
  if ( eProtEnum < BINCMD_ENUM_MAX )
  {
    // now get pointers
    ptLclCtl = &atCtrls[ eProtEnum ];  

    // get the command table entry
    
    // process the state
    switch( ptLclCtl->eMastState )
    {
      case MAST_STATE_IDLE :
        // only allow transmit commands from here
        if ( eEvent == MAST_EVENT_SNDMSG )
        {
          // test for data
          if (( ptMstCmdTblRspFunc = ( PVBINMSTCMDRSPFUNC )PGM_RDWORD( ptLclCtl->ptCurMstCmdTable->ptMstCmdTblFunc )) != NULL )
          {
            // process the command data
            ptMstCmdTblRspFunc( );
          }
          
          // send the command
          BinaryCommandHandler_SendMessage( eProtEnum );
          
          // go to wait states
          ptLclCtl->eMastState = MAST_STATE_WAIT;
        }
        break;
        
      case MAST_STATE_WAIT :
        // process the event
        switch( eEvent )
        {
          case MAST_EVENT_RCVOK :
            // test for response handler
            if (( ptMstCmdTblRspFunc = ( PVBINMSTCMDRSPFUNC )PGM_RDWORD( ptLclCtl->ptCurMstCmdTable->pvMstRspFunc )) != NULL )
            {
              // process the response data
              ptMstCmdTblRspFunc( );
            }
            
            // back to idle
            ptLclCtl->eMastState = MAST_STATE_IDLE;
            break;
            
          case MAST_EVENT_RCVERR :
            break;
            
          default :
            break;
        }
        break;
        
      default :
        break;
    }
  }
}
#endif // BINCMDHAND_ENABLE_MASTERMODE

/******************************************************************************
 * @function CheckAddress
 *
 * @brief check the address
 *
 * This function checks to see if this message matches our address 
 *
 * @param[in]   nAddr   address received
 *
 * @return      TRUE if address matches our address or broadcast
 *
 *****************************************************************************/
static BOOL CheckAddress( U8 nAddr )
{
  BOOL bAddrMatch = FALSE;
  
  // check to see if this is our local address or a broadcast address
  if (( nAddr == ptLclCtl->nLclAddr ) || ( nAddr == BINPROT_BROADCAST_ADDR ))
  {
    // set address match
    bAddrMatch = TRUE;
  }
  
  // return the address match
  return( bAddrMatch );
}

/******************************************************************************
 * @function StuffRcvData
 *
 * @brief stuff data into the transmite buffer
 *
 * This function stuffs a byte of data into the receive buffer if room 
 *
 * @param[in]   nRcvChar    character received
 *
 * @return      TRUE if no error, FALSE if no room
 *
 *****************************************************************************/
static BOOL StuffRcvData( U8 nRcvChar )
{
  BOOL  eStatus = FALSE;
  PU8   pnBuffer;
  
  // check for room
  if ( ptLclCtl->tRcvBuffer.wIndex < PGM_RDWORD( ptLclDef->wRcvBufferSize ))
  {
    // get the pointer to the buffer
    pnBuffer = ( PU8 )PGM_RDWORD( ptLclDef->pnRcvBuffer );
    
    // store it
    *( pnBuffer + ptLclCtl->tRcvBuffer.wIndex++ ) = nRcvChar;

    // set good status
    eStatus = TRUE;
  }
  
  // return status
  return( eStatus );
}

/******************************************************************************
 * @function SendData
 *
 * @brief send a byte of data
 *
 * This function adds the value to the check value, determines if it needs to
 * be escaped and send the appropriate characters
 *
 * @param[in]   ptBufCtl    pointer to the local buffer control
 * @param[in]   nData       transmit character
 * @param[in]   bEscapeEnb  enable the escape sequence
 *
 *****************************************************************************/
static void StuffXmtData( PLCLBUFCTL ptBufCtl, U8 nData, BOOL bEscapeEnb  )
{
  // add the value to the check
  switch( ptBufCtl->eCheckMode )
  {
    case BINCMD_CHECKMODE_EOR :
      // exclusive or it
      ptBufCtl->tCheck.anValue[ LE_U16_LSB_IDX ] ^= nData;
      break;
      
    case BINCMD_CHECKMODE_CMP :
      // add it
      ptBufCtl->tCheck.anValue[ LE_U16_LSB_IDX ] += nData;
      break;
      
    case BINCMD_CHECKMODE_CRC :
      ptBufCtl->tCheck.wValue = CRC16_CalculateByte( ptBufCtl->tCheck.wValue, nData );
      break;
      
    default :
      break;
  }
  
  // add the value
  *( ptBufCtl->pnBuffer + ptBufCtl->wIndex++ ) = nData;
  
  // check for stuff enabled and a stuff character is required
  if (( bEscapeEnb ) && ( nData == CH_DLE ))
  {
    // call ourselves again
    StuffXmtData( ptBufCtl, CH_SPC, FALSE ); 
  }
}


/******************************************************************************
 * @function ProcessRcvdMsg
 *
 * @brief process the received message
 *
 * This function parses and executes the received message
 *
 * @param[in]   eProtEnum   protocol enumeration
 * @param[in]   nCompareValue value to use for the flag compare
 *
 * @return      appropriate protocol status/error
 *
 *****************************************************************************/
static BINCMDSTS ProcessRcvdMsg( BINCMDENUM eProtEnum, U8 nCompareValue )
{
  BINCMDSTS           eStatus = BINCMD_STS_MSGRCVD_NOTPROC;
  BINPARSESTS         eParseStatus;
  U16                 wRcvLength;
  
  // get the receive length
  BinaryCommandHandler_GetRcvLength( eProtEnum, &wRcvLength );

  // is this multidrop mode
  if ( PGM_RDBYTE( ptLclDef->bMultiDropMode ))
  {
    // set the destination address to the received address
    ptLclCtl->tXmtBuffer.nSrcDstAddr = ptLclCtl->tRcvBuffer.nSrcDstAddr;
  }
  
  // begin the message
  BinaryCommandHandler_BeginMessage( eProtEnum, CH_ACK, 0 );
        
  // parse the command table
  eParseStatus = ParseCommand( eProtEnum, wRcvLength, nCompareValue );
        
  // process the status
  switch ( eParseStatus )
  {
    case BINPARSE_STS_SND_ACK :
    case BINPARSE_STS_SND_RESP :
      BinaryCommandHandler_SendMessage( eProtEnum );
      eStatus = BINCMD_STS_MSGRCVD_PROC;
      break;
          
    case BINPARSE_STS_SND_NAK :
      BinaryCommandHandler_BeginMessage( eProtEnum, CH_NAK,  ptLclCtl->tXmtBuffer.nOption );
      BinaryCommandHandler_SendMessage( eProtEnum );
      eStatus = BINCMD_STS_MSGRCVD_ERR;
      break;
          
    case BINPARSE_STS_SND_RESP_OPTION :
      BinaryCommandHandler_BeginMessage( eProtEnum, CH_ACK, ptLclCtl->tXmtBuffer.nOption );
      BinaryCommandHandler_SendMessage( eProtEnum );
      eStatus = BINCMD_STS_MSGRCVD_PROC;
      break;
      
    case BINPARSE_STS_ERR_ILLCMD :
    case BINPARSE_STS_ERR_ILLFLG :
    case BINPARSE_STS_ERR_ILLLEN :
    case BINPARSE_STS_ERR_ILLVAL :
    case BINPARSE_STS_ERR_ILLHAND :
    case BINPARSE_STS_ERR_ILLMODE :
      BinaryCommandHandler_BeginMessage( eProtEnum, CH_NAK, eParseStatus );
      BinaryCommandHandler_SendMessage( eProtEnum );
      eStatus = BINCMD_STS_MSGRCVD_ERR;
      break;
      
    case BINPARSE_STS_SND_NORESP :
      eStatus = BINCMD_STS_IDLE;
      break;

    default :
      BinaryCommandHandler_BeginMessage( eProtEnum, CH_NAK, 0xFF );
      BinaryCommandHandler_SendMessage( eProtEnum );
      eStatus = BINCMD_STS_MSGRCVD_ERR;
      break;
  }
  
  // return the status
  return( eStatus );
}

/******************************************************************************
 * @function ParseCommand
 *
 * @brief parse the command
 *
 * This function processes the command and if a command is found in the table
 * the command is checked for length and flags
 *
 * @param[in]   eProtEnum   protocol enumeration
 * @param[in]   wRcvLen       length of the received command
 * @param[in]   nCompareValue value to use for the flag compare
 *
 * @return      appropriate protocol status/error
 *
 *****************************************************************************/
static BINPARSESTS ParseCommand( BINCMDENUM eProtEnum, U16 wRcvLen, U8 nCompareValue )
{
  BINCMDSLVENTRY const *  ptCommandTable;
  BINCMDSLVENTRY          tTableEntry;
  U8                      nIdx;
  BOOL                    bRunFlag = TRUE;
  BOOL                    bCompareResults;
  BINPARSESTS             eStatus = BINPARSE_STS_ERR_NOCMDTBL;

  // reset the index/stack index
  nIdx = 0;
  ptLclCtl->nStackIndex = 0;

  // get a pointer to the command table
  ptCommandTable = ( BINCMDSLVENTRY const * )PGM_RDWORD( ptLclDef->ptSlvCmdTbl );
  if ( ptCommandTable != NULL )
  {
    // loop till end of table or command
    while( bRunFlag )
    {
      // get a copy of the table entry
      MEMCPY_P( &tTableEntry, ( ptCommandTable + nIdx++ ), BINCMDSLVENTRY_SIZE );

      // test for end of table
      if ( tTableEntry.nCommand == BINCMD_END_TABLE )
      {
        // check if is the last table
        if ( ptLclCtl->nStackIndex == 0 )
        {
          // report the error as an illegal command
          eStatus = BINPARSE_STS_ERR_ILLCMD;
          bRunFlag = FALSE;
        }
        else
        {
          // get the table pointer from the stack
          ptCommandTable = ptLclCtl->atTableStack[ --ptLclCtl->nStackIndex ];
        }
      }
      else if ( tTableEntry.nCommand == BINCMD_EXT_TABLE )
      {
        // check for room on stack
        if ( ptLclCtl->nStackIndex < BINARYCOMMANDHANDLER_TABLE_STACK_DEPTH )
        {
          // save the table pointer on the stack/get the next table/set the index back to 0
          ptLclCtl->atTableStack[ ptLclCtl->nStackIndex++ ] = ptCommandTable;
          ptCommandTable = ( BINCMDSLVENTRY const *  )tTableEntry.pvExtTable;
          nIdx = 0;

          // check for table
          if ( ptCommandTable == NULL )
          {
            // set the error/exit
            eStatus = BINPARSE_STS_ERR_NOCMDTBL;
            bRunFlag = FALSE;
          }
        }
        else
        {
          // report an error
          eStatus = BINPARSE_STS_ERR_TOOMANYTBLS;
          bRunFlag = FALSE;
        }
      }
      else
      {
        // determine if this command matches the command in the table
        if ( tTableEntry.nCommand == ( ptLclCtl->tRcvBuffer.nCommand & ~tTableEntry.nCmdMask ))
        {
          // now check for proper length
          if (( tTableEntry.iExpLength == -1 ) || ( tTableEntry.iExpLength == wRcvLen ))
          {
            // now compare flag values
            switch( tTableEntry.eCompareFlag )
            {
              case BINCOMP_FLAG_EQ :
                bCompareResults = ( nCompareValue == tTableEntry.nCompareValue ) ? TRUE : FALSE;
                break;

              case BINCOMP_FLAG_NE :
                bCompareResults = ( nCompareValue != tTableEntry.nCompareValue ) ? TRUE : FALSE;
                break;

              case BINCOMP_FLAG_GT :
                bCompareResults = ( nCompareValue > tTableEntry.nCompareValue ) ? TRUE : FALSE;
                break;

              case BINCOMP_FLAG_GE :
                bCompareResults = ( nCompareValue >= tTableEntry.nCompareValue ) ? TRUE : FALSE;
                break;

              case BINCOMP_FLAG_LT :
                bCompareResults = ( nCompareValue < tTableEntry.nCompareValue ) ? TRUE : FALSE;
                break;

              case BINCOMP_FLAG_LE :
                bCompareResults = ( nCompareValue <= tTableEntry.nCompareValue ) ? TRUE : FALSE;
                break;

              case BINCOMP_FLAG_NONE :
              default :
                bCompareResults = TRUE;
                break;
            }
          
            // now test the results
            if ( bCompareResults )
            {
              // if valid handler
              if ( tTableEntry.pvCmdFunc != NULL )
              {
                // execute it
                eStatus = tTableEntry.pvCmdFunc( eProtEnum );
              }
              else
              {
                // set error flag
                eStatus = BINPARSE_STS_ERR_ILLHAND;
              }

              // in all cases, clear the run flag
              bRunFlag = FALSE;
            }
            else
            {
              // illegal flag
              eStatus = BINPARSE_STS_ERR_ILLFLG;
              bRunFlag = FALSE;
            }
          }
          else
          {
            // command lengths does not match
            eStatus = BINPARSE_STS_ERR_ILLLEN;
            bRunFlag = FALSE;
          }
        }
      }
    }
  }
  
  // return the status
  return ( eStatus );
}

/**@} EOF BinaryCommandHandler.c */

