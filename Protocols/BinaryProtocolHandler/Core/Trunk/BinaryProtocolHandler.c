/******************************************************************************
 * @file BinaryProtocol.c
 *
 * @brief binary protocol handler implementation
 *
 * This file provides the implementation for the binary protocol handler
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
 * \addtogroup BinaryProtocol
 * @{
 *****************************************************************************/

// system includes ------------------------------------------------------------
#include <semaphore.h>

// local includes -------------------------------------------------------------
#include "BinaryProtocol/BinaryProtocol.h"

// library includes -----------------------------------------------------------
#include "CRC16/CRC16.h"
#include "StateExecutionEngine/StateExecutionEngine.h"

// Macros and Defines ---------------------------------------------------------
/// define the control characters
#define CH_SOH                  ( 0x01 )
#define CH_STX                  ( 0x02 )
#define CH_EOT                  ( 0x04 )
#define CH_ACK                  ( 0x06 )
#define CH_DLE                  ( 0x10 )
#define CH_NAK                  ( 0x15 )
#define CH_SPC                  ( 0xFE )

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

// structures -----------------------------------------------------------------
/// define the local contro struture
typedef struct _LCLCTL
{
  sem_t               tRcvMsgSem;
  PU8                 pnXmtBuffer;
  PU8                 pnRcvBuffer;
  U16                 wXmtBufSize;
  U16                 wXmtBufIndex;
  U16                 wRcvBufIndex;
  BINPROTCHKMODE      eCheckMode;
  U8                  nDeviceAddr;
  BOOL                bMultiDropEnabled;
  BOOL                bSequenceEnabled;
  U16UN               tXmtCheckValue;
  U16UN               tRcvCheckValue;
  U16UN               tCalcCheckValue;
  BINPROTMSG          tRcvMessage;
  STATEEXECENGCONTROL tStateCtl;          ///< the state control structure
} LCLCTL, *PLCLCTL;
#define LCLCTL_SIZE           sizeof( LCLCTL )

// global parameter declarations ----------------------------------------------

// local parameter declarations -----------------------------------------------
static  LCLCTL  anLclCtl[ BINPROT_ENUM_MAX ];
static  PLCLCTL ptCurCtl;

// local function prototypes --------------------------------------------------
static  void  FormatMessage( PLCLCTL ptCtl, PBINPROTMSG ptMessage );
static  void  StuffXmitBuffer( PLCLCTL ptCtl, U8 nData, BOOL bStuffEnable );
static  BOOL  CheckAddress( U8 nAddr );
static  BOOL  StuffRcvData( U8 nRcvChar );

// RCV_STATE_IDLE functions
static  void  RcvStateIdleEnt( void );
static  void  RcvStateIdleExt( void );

// RCV_STATE_HDR1 functions
static  U8    RcvStateHdr1Exc( TASKARG xArg );

// RCV_STATE_DSTA functions
static  U8    RcvStateDstaExc( TASKARG xArg );

// RCV_STATE_SRCA functions
static  U8    RcvStateSrcaExc( TASKARG xArg );

// RCV_STATE_CMND functions
static  U8    RcvStateCmndExc( TASKARG xArg );

// RCV_STATE_OPTN functions
static  U8    RcvStateOptnExc( TASKARG xArg );

// RCV_STATE_SEQN functions
static  U8    RcvStateSeqnExc( TASKARG xArg );

// RCV_STATE_CTRL functions
static  U8    RcvStateCtrlExc( TASKARG xArg );

// RCV_STATE_DATA functions
static  U8    RcvStateDataExc( TASKARG xArg );

// RCV_STATE_CRCM functions
static  U8    RcvStateCrcmExc( TASKARG xArg );

// RCV_STATE_CRCL functions
static  U8    RcvStateCrclExc( TASKARG xArg );

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
 * @function BinaryProtocol_Initialize
 *
 * @brief open port
 *
 * This function opens the port, creates the read thread
 *
 *****************************************************************************/
 void BinaryProtocol_Initialize( void ) 
{
  BOOL        bStatus = TRUE;
  BINPROTENUM eProtEnum;
  PBINPROTDEF ptDef;
  LCLCTL      ptCtl;
  
  // for each defined protocol
  for( eProtEnum = 0; eProtEnum < BINPROT_MAX_ENUM; eProtEnum++ )
  {
    // get the definition/control pointers
    ptDef = ( PBINPROTDEF )&atBinProtDefs[ eProtEnum ];
    ptCtl = &atLclCtl[ eProtEnum ];
    
    // set the checkmode/multdrop/sequence
    ptCtl->eCheckMode = ptDef->eCheckMode;
    ptCtl->bMultiDropEnabled = ptDef->bMultiDropEnable;
    ptCtl->bSequenceEnabled = ptDef->bSequenceEnable;
    ptCtl->nDeviceAddr = ptDef->nDeviceAddr
    
    // create the read thread/read semaphore
    sem_init( &( ptCtl->tRcvMsgSem ), 0, 0 );
    
    // allocate the buffers
    ptCtl->pnRcvBuffer = malloc( sizeof( U8 ) * ptDef->wXmtBufferSize );
    ptCtl->pnXmtBuffer = malloc( sizeof( U8 ) * ptDef->wRcvBufferSize );
    
    // set the state engine/initialize the state
    ptCtl->tStateCtl.ptStates = atRcvStates;
    StateExecutionEngine_Initialize( &( ptCtl->tStateCtl ), RCV_STATE_IDLE );
  }
}
 
/******************************************************************************
 * @function BinaryProtocol_Close
 *
 * @brief close the binary protocol
 *
 * This function will kill all threads, close the serial port
 *
 * @return      FALSE if OK, TRUE if error detected      
 *
 *****************************************************************************/
BINPROTMSGSTS BinaryProtocol_Close( BINPROTENUM eProtEnum )
{
  BINPROTMSGSTS eStatus = BINPROT_MSGSTS_OKAY;
  
  // return the status
  return( eStatus );
} 

/******************************************************************************
 * @function BinaryProtocol_ProcessCharCallback
 *
 * @brief process a received character
 *
 * This function will process a received character
 *
 * @param[in]   eProtEnum protocol enumeration
 * @param[in]   nData     data received
 *
 *****************************************************************************/
void BinaryProtocol_ProcessCharCallback( BINPROTENUM eProtEnum, U8 nData )
{
  // get the definition/control pointers
  ptCurCtl = &atLclCtl[ eProtEnum ];
  
  // now determine the check method
  switch( ptCurCtl->eCheckMode )
  {
    case BINPROT_CHECKMODE_EOR :
      // exclusive OR with previous
      ptCurCtl->tRcvCheckValue.anValue[ LE_U16_LSB_IDX ] ^= nData;
      break;
      
    case BINPROT_CHECKMODE_CSM :
      // just add
      ptCurCtl->tRcvCheckValue.anValue[ LE_U16_LSB_IDX ] += nData;
      break;
      
    case BINPROT_CHECKMODE_CRC :
      // add the value to the CRC
      ptCurCtl->tRcvCheckValue.wValue = CRC16_CalculateByte( ptCurCtl->tRcvCheckValue.wValue, nData );
      break;
      
    default :
      break;
  }
  
  // process the event through the state engine
  StateExecutionEngine_Process( &( ptCurCtl->tStateCtl ), nData );
  
  // determine if we have a message
  switch( eLclSts )
  {
    case LCL_STS_RCVD :
    case LCL_STS_CHKERR :
      // post the semaphore 
      sem_post( &( ptCurCtl->tRcvMsgSem ));
      break;
      
    default :
      break;
  }    
}
  
/******************************************************************************
 * @function BinaryProtocol_SendRcvMessage
 *
 * @brief send a message and wait for a return messageout on the serial port
 *
 * This function will send a message and wait for a returned message if timeout
 * is not negative
 *
 * @param[in]   eProtEnum protocol enumeration
 * @param[in]   ptXmtMessage  pointer to the message structure
 * @param[in]   ptRcvMessage  pointer to the message structure
 * @param[in]   sTimeoutMsec  timeout in milliseconds
 *
 * @return      appropriate message status
 *
 *****************************************************************************/
BINPROTMSGSTS BinaryProtocol_SendRcvMessage( BINPROTENUM eProtEnum, PBINPROTMSG ptXmtMessage, PBINPROTMSG ptRcvMessage, S16 sTimeoutMsec )
{
  BINPROTMSGSTS   eMsgStatus = BINPROT_MSGSTS_OK;
  U16             wBytesWritten;
  struct timespec tTime;
  int             iSemWaitResult;
  LCLCTL          ptCtl;
  
  // get the pointer to the local control
  ptCtl = &atLclCtl[ eProtEnum ];
  
  // format the message
  FormatMessage( ptCtl, ptXmtMessage );
  
  // now send it
  ptCtl->pvWriteFunc( anXmtBuffer, wXmtBufIndex );
  
  // now check for a timeout on receive
  if ( sTimeoutMsec > 0 )
  {
    // get the current time
    clock_gettime( CLOCK_REALTIME, &tTime );
    
    // add the time in nanoseconds
    set_normalized_timespec( &tTime, 0, ( sTimeoutMsec * 1000000 ));
    
    // wait for the receive message event
    while (( iSemWaitResult = sem_timedwait( &( ptCtl->tRcvMsgSem ), &tTime )) == -1 && ( errno == EINTR))
    {
      // restart if interrupt by a signal handler
      continue;
    }
    
    // now determine the result
    if ( iSemWaitResult == -1 )
    {
      // set the timeout error
      eMsgStatus = BINPROT_MSGSTS_TIMEOUT;
    }
    else
    {
      // copy the message to the receive pointer
      memcpy( ptRcvMessage, &( ptCtl->tRcvMessage ), BINPROTMSG_SIZE );
      
      // set message ok with receive
      eMsgStatus = BINPROT_MSGSTS_RCVOKAY;
    }
  }
  
  // return the message status
  return( eMsgStatus );
}
 
/******************************************************************************
 * @function StuffXmtBuffer
 *
 * @brief stuff the transmit buffer
 *
 * This function will stuff a value into the transmit buffer
 *
 * @param[in]   ptCtl     pointer to the control structure
 * @param[in]   ptpMessage  pointer to the message
 *
 *****************************************************************************/
static void FormatMessage( PLCLCTL ptCtl, PBINPROTMSG ptMessage )
{
  // clear the buffer pointer
  ptCtl->wXmtBufIndex = 0;
  
  // initialize the check value
  if ( ptCtl->eCheckMode == BINPROT_CHECKMODE_CRC )
  {
    // initialize the CRC initial value
    ptCtl->tXmtCheckValue.wValue = CRC16_GetDefaultValue( );
  }
  else
  {
    // just clear it
    ptCtl->tXmtCheckValue.anValue[ LE_U16_LSB_IDX ] = 0;
  }
  
  // stuff the header
  StuffMsgBuffer( ptCtl, CH_DLE, FALSE );
  StuffMsgBuffer( ptCtl, CH_SOH, FALSE );
  
  // now determine if we are in multidrop mode
  if ( ptCtl->bMultiDropEnabled )
  {
    // stuff the destination/source address
    StuffMsgBuffer( ptCtl, ptMessage->nDstAdr, TRUE );
    StuffMsgBuffer( ptCtl, ptMessage->nSrcAdr, TRUE );
  }
  
  // stuff the command/option bytes
  StuffMsgBuffer( ptCtl, ptMessage->nCommand, TRUE );
  StuffMsgBuffer( ptCtl, ptMessage->nOption, TRUE );
  
  // check for sequence
  if ( ptCtl->bSequenceEnabled )
  {
    // stuff it
    StuffMsgBuffer( ptCtl, ptMessage->nSequence, TRUE );
  }
  
  // now test for data
  if ( ptMessage->wLength != 0 )
  {
    // stuff the data control sequence
    StuffMsgBuffer( ptCtl, CH_DLE, FALSE );
    StuffMsgBuffer( ptCtl, CH_STX, FALSE );
    
    // now stuff the data
    while( ptMessage->wLength-- != 0 )
    {
      StuffMsgBuffer( ptCtl, *( ptMessage->pnMessage++ ), TRUE );
    }
  }
  
  // stuff the end of message sequence
  StuffMsgBuffer( ptCtl, CH_DLE, FALSE );
  StuffMsgBuffer( ptCtl, CH_ETX, FALSE );
  
  // now determine the check method
  switch( ptCtl->eCheckMode )
  {
    case BINPROT_CHECKMODE_EOR :
      // stuff it
      StuffMsgBuffer( ptCtl, ptCtl->tXmtCheckValue.anValue[ LE_U16_LSB_IDX ];
      break;
      
    case BINPROT_CHECKMODE_CSM :
      // 2's complement the checksum
      ptCtl->tXmtCheckValue.anValue[ LE_U16_LSB_IDX ] = ~ptCtl->tXmtCheckValue.anValue[ LE_U16_LSB_IDX ];
      ptCtl->tXmtCheckValue.anValue[ LE_U16_LSB_IDX ]++;
      
      // stuff it
      StuffMsgBuffer( ptCtl, tXmtCheckValue.anValue[ LE_U16_LSB_IDX ];
      break;
      
    case BINPROT_CHECKMODE_CRC :
      // stuff the CRC - big endian
      StuffMsgBuffer( ptCtl, ptCtl->tXmtCheckValue.anValue[ LE_U16_MSB_IDX ]);
      StuffMsgBuffer( ptCtl, ptCtl->tXmtCheckValue.anValue[ LE_U16_LSB_IDX ]);
      break;
      
    default :
      break;
  }
}

/******************************************************************************
 * @function StuffXmtBuffer
 *
 * @brief stuff the transmit buffer
 *
 * This function will stuff a value into the transmit buffer
 *
 * @param[in]   ptCtl     pointer to the control structure
 * @param[in]   nData     data to stuff
 * @param[in]   bEnable   enable the data stuff
 *
 *****************************************************************************/
void StuffXmtBuffer( PLCLCTL ptCtl, PLCLCTL ptCtl, U8 nData, BOOL bEnable )
{
  // ensure there is room
  if ( ptCtl->wXmtBufIndex < MAX_XMTBUF_SIZE )
  {
    // stuff the value
    ptCtl->anXmtBuffer[ ptCtl->wXmtBufIndex++ ] = nData;
    
    // now determine the check method
    switch( ptCtl->eCheckMode )
    {
      case BINPROT_CHECKMODE_EOR :
        // exclusive OR with previous
        ptCtl->tXmtCheckValue.anValue[ LE_U16_LSB_IDX ] ^= nData;
        break;
        
      case BINPROT_CHECKMODE_CSM :
        // just add
        ptCtl->tXmtCheckValue.anValue[ LE_U16_LSB_IDX ] += nData;
        break;
        
      case BINPROT_CHECKMODE_CRC :
        // add the value to the CRC
        ptCtl->tXmtCheckValue.wValue = CRC16_CalculateByte( ptCtl->tXmtCheckValue.wValue, nData );
        break;
        
      default :
        break;
    }
    
    // now check for stuff enable and data equal to CH_DLE
    if (( bEnable == TRUE ) && ( nData == CH_DLE ))
    {
      // stuff the special character
      StuffXmtBuffer( ptCtl, CH_SPC, FALSE );
    }
  }
}

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
static BOOL CheckAddress( PLCLCTL ptCtl, U8 nAddr )
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
static BOOL StuffRcvData( PLCLCTL ptCtl, U8 nRcvChar )
{
  BOOL  eStatus = FALSE;
  
  // check for room
  if ( ptLclCtl->tRcvBuffer.wIndex < ptLclDef->wRcvBufferSize )
  {
    // get the pointer to the buffer
    pnBuffer = ( PU8 )ptLclDef->pnRcvBuffer;
    
    // store it
    *( pnBuffer + ptLclCtl->tRcvBuffer.wIndex++ ) = nRcvChar;

    // set good status
    eStatus = TRUE;
  }
  
  // return status
  return( eStatus );
}

 /******************************************************************************
 * RCV_STATE_IDLE functions
 *****************************************************************************/

static void RcvStateIdleEnt( void )
{
  // reset the states
  // eLclSts = LCL_STS_IDLE;
}

static void RcvStateIdleExt( void )
{
  // set the states to busy
  ptCurCtl->eLclSts = LCL_STS_BUSY;
}

/******************************************************************************
 * RCV_STATE_HDR1 functions
 *****************************************************************************/
static U8 RcvStateHdr1Exc( TASKARG xArg )
{
  U8  nNextState;
  
  // check for a SOH character
  if ( xArg == CH_SOH )
  {
    // reset the buffer index
    ptCurCtl->wRcvBufIndex = 0;
    
    // reset the CRC
    switch( ptCurCtl->eCheckMode )
    {
      case BINCMD_CHECKMODE_EOR :
        ptCurCtl->tRcvChkValue.anValue[ LE_U16_LSB_IDX ] = CH_DLE ^ CH_SOH;
        break;
        
      case BINCMD_CHECKMODE_CMP :
        ptCurCtl->tRcvChkValue.anValue[ LE_U16_LSB_IDX ] = CH_DLE + CH_SOH;
        break;
        
      case BINCMD_CHECKMODE_CRC :
        ptCurCtl->tRcvChkValue.wValue = CRC16_GetInitialValue( );
        ptCurCtl->tRcvChkValue.wValue = CRC16_CalculateByte( ptCurCtl->tRcvChkValue.wValue, CH_DLE );
        ptCurCtl->tRcvChkValue.wValue = CRC16_CalculateByte( ptCurCtl->tRcvChkValue.wValue, CH_SOH );
        break;
        
      default :
        break;
    }
    
    // set the next state
    nNextState = ( ptCurCtl->bMultiDropModeEnabled == TRUE ) ? RCV_STATE_DSTA : RCV_STATE_CMND;
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
static U8 RcvStateDstaExc( TASKARG xArg )
{
  // if this is our address, next state is source/otherwise idle
  return(( CheckAddress( ptCurCtl, xArg ) == TRUE ) ? RCV_STATE_SRCA : RCV_STATE_IDLE );
}

/******************************************************************************
 * RCV_STATE_SRCA functions
 *****************************************************************************/
static U8 RcvStateSrcaExc( TASKARG xArg )
{
  // store the source address/goto to RCV_STATE_CMND
  ptCurCtl->tRcvMessage.nSrcAddr = xArg;
  return( RCV_STATE_CMND );
}

/******************************************************************************
 * RCV_STATE_CMND functions
 *****************************************************************************/
static U8 RcvStateCmndExc( TASKARG xArg )
{
  // store the command/goto to RCV_STATE_OPTN
  ptCurCtl->tRcvMessage.nCommand = xArg;
  return( RCV_STATE_OPTN );
}

/******************************************************************************
 * RCV_STATE_OPTN functions
 *****************************************************************************/
static U8 RcvStateOptnExc( TASKARG xArg )
{
  U8  nNextState;
  
  // store the option/determine next state
  ptCurCtl->tRcvMessage.nOption = xArg;
  
  // set the next state
  nNextState = ( ptCurCtl->bSequenceEnabled == TRUE ) ? RCV_STATE_SEQN : RCV_STATE_HDR2;
  
  // return next state
  return( nNextState );
}

/******************************************************************************
 * RCV_STATE_SEQN functions
 *****************************************************************************/
static U8 RcvStateSeqnExc( TASKARG xArg )
{
  // store the sequence/goto header 2
  ptCurCtl->tRcvMessage.nSequence = xArg;
  return( RCV_STATE_HDR2 );
}

/******************************************************************************
 * RCV_STATE_CTRL functions
 *****************************************************************************/
static U8 RcvStateCtrlExc( TASKARG xArg )
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
    switch( ptCurCtl->tStateCtl.nLastState )
    {
      case RCV_STATE_DSTA :
        // if this is our address, next state is source/otherwise idle
        nNextState = ( CheckAddress( ptCurCtl, CH_DLE ) == TRUE ) ? RCV_STATE_SRCA : RCV_STATE_IDLE;
        break;
        
      case RCV_STATE_SRCA :
        // store the source address/goto to RCV_STATE_CMND
        ptCurCtl->tRcvMessage.nSrcAddr = CH_DLE;
        nNextState = RCV_STATE_CMND;
        break;
        
      case RCV_STATE_CMND :
        // store the command/goto to RCV_STATE_OPTN
        ptCurCtl->tRcvMessage.nCommand = CH_DLE;
        nNextState = RCV_STATE_OPTN;
        break;
        
      case RCV_STATE_OPTN :
        // store the option/determine next state
        ptCurCtl->tRcvMessage.nOption = CH_DLE;
        
        // set the next state
        nNextState = ( ptCurCtl->bSequenceEnabled == TRUE ) ? RCV_STATE_SEQN : RCV_STATE_HDR2;
        break;
        
      case RCV_STATE_SEQN :
        // store the sequence/goto header 2
        ptCurCtl->tRcvMessage.nSequence = CH_DLE;
        nNextState = RCV_STATE_HDRS;
        break;
        
      case RCV_STATE_DATA :
        // stuff the data if okay, we got  data, otherwise back to idle
        nNextState = ( ptCurCtl, StuffRcvData( CH_DLE ) == TRUE ) ? RCV_STATE_DATA : RCV_STATE_IDLE;
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
static U8 RcvStateDataExc( TASKARG xArg )
{
  // stuff the data if okay, we got  data, otherwise back to idle
  return(( StuffRcvData( ptCurCtl, xArg ) == TRUE ) ? RCV_STATE_DATA : RCV_STATE_IDLE );
}

/******************************************************************************
 * RCV_STATE_CRCM functions
 *****************************************************************************/
static U8 RcvStateCrcmExc( TASKARG xArg )
{
  U8  nNextState = RCV_STATE_IDLE;

  // determine the checksum
  switch( ptCurCtl->eCheckMode )
  {
    case BINCMD_CHECKMODE_EOR :
      // compare for good
      ptCurCtl->eLclSts = ( ptCurCtl->tRcvChkValue.anValue[ LE_U16_LSB_IDX ] == xArg ) ? LCL_STS_RCVD : LCL_STS_CHKERR;
      nNextState = RCV_STATE_IDLE;
      break;

    case BINCMD_CHECKMODE_CMP :
      // complement it
      tRcvChkValue.anValue[ LE_U16_LSB_IDX ] = ~tRcvChkValue.anValue[ LE_U16_LSB_IDX ];
      tRcvChkValue.anValue[ LE_U16_LSB_IDX ]++;

      // compare for good
      ptCurCtl->eLclSts = ( ptCurCtl->tRcvChkValue.anValue[ LE_U16_LSB_IDX ] == xArg ) ? LCL_STS_RCVD : LCL_STS_CHKERR;
      nNextState = RCV_STATE_IDLE;
      break;

    case BINCMD_CHECKMODE_CRC :
      // store MSB and got LSB
      ptCurCtl->tRcvChkValue.anValue[ LE_U16_MSB_IDX ] = xArg;
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
static U8 RcvStateCrclExc( TASKARG xArg )
{
  // store the value
  ptCurCtl->tRcvChkValue.anValue[ LE_U16_LSB_IDX ] = xArg;
  
  // check for valid
  ptCurCtl->eLclSts = ( ptCurCtl->tRcvMessage.tCheck.wValue == ptCurCtl->tCalcCheckValue.wValue ) ? LCL_STS_RCVD : LCL_STS_CHKERR;
  
  // back to idle
  return( RCV_STATE_IDLE );
}

/**@} EOF BinaryProtocol.c */
