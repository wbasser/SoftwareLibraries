/******************************************************************************
 * @file ManchesterCodec.c
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

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "ManchesterCodec/ManchesterCodec.h"

// library includes -----------------------------------------------------------
#include "Interrupt/Interrupt.h"
#if (( MANCHESTERCODEC_RCVDBG_ENABLE == 1 ) || ( MANCHESTERCODEC_XMTDBG_ENABLE == 1 ))
#include "DebugManager/DebugManager.h"
#endif // ( MANCHESTERCODEC_RCVDBG_ENABLE == 1 ) || ( MANCHESTERCODEC_XMTDBG_ENABLE == 1 )

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------
/// enumerate the message phase
typedef enum _XMTPHASE
{
  XMT_PHASE_SYNCFH  = 0,
  XMT_PHASE_SYNCSH,
  XMT_PHASE_DATAFH,
  XMT_PHASE_DATASH,
  XMT_PHASE_STOP,
  XMT_PHASE_DONE,
} XMTPHASE;

/// enumerate the receive phase
typedef enum _RCVPHASE
{
  RCV_PHASE_SYNC = 0,
  RCV_PHASE_DATA,
  RCV_PHASE_STOP,
  RCV_PHASE_DONE,
} RCVPHASE;

/// enumerate the bit decodes
typedef enum _DECSTATE
{
  DEC_STATE_P0_DHAH   = 0,    ///< 0 
  DEC_STATE_P0_DHAF,          ///< 1
  DEC_STATE_P0_DFAH,          ///< 2
  DEC_STATE_P0_AFDF,          ///< 3
  DEC_STATE_P1_DHAH,          ///< 4
  DEC_STATE_P1_DHAF,          ///< 5
  DEC_STATE_P1_DFAH,          ///< 6
  DEC_STATE_P1_AFDF,          ///< 7
} DECSTATE;

/// enumerate the bit state
typedef enum _BITSTATE
{
  BIT_STATE_H = 0,
  BIT_STATE_F,
  BIT_STATE_ERR = 0x20
} BITSTATE;

/// enumerate the bit positions
typedef enum _BITPOS
{
  BIT_POS_A = 0,    /// active
  BIT_POS_D,        // deactive
  BIT_POS_P         // previous
} BITPOS;

/// enumerate the shift bits
typedef enum _SHIFTBIT
{
  SHIFT_BIT_0 = 0,    ///< 0
  SHIFT_BIT_1,        ///< 1
  SHIFT_BIT_STOP,     ///< stop
} SHIFTBIT;

// structures -----------------------------------------------------------------

// global parameter declarations ----------------------------------------------

// local parameter declarations -----------------------------------------------
static  U16                 wHalfBitMinTime;      ///< half bit mean time
static  U16                 wHalfBitMaxTime;      ///< half bit max time
static  U16                 wFullBitMinTime;      ///< full bit mean time
static  U16                 wFullBitMaxTime;      ///< full bit max time
static  XMTPHASE            eXmtPhase;            ///< transmit phase
static  RCVPHASE            eRcvPhase;            ///< recieve phase
static  PU8                 pnXmtData;            ///< pointer to the transmit data
static  PU8                 pnRcvData;            ///< pointer to the receive data
static  U8                  nXmtLen;              ///< transmit length
static  U8                  nRcvLen;              ///< recieve length
static  U16                 wOnCapTime;           ///< on capture time
static  U16                 wOffCapTime;          ///< off capture time
static  U8                  nXmtMask;             ///< transmit bit mask
static  U8                  nXmtSyncBitCnt;       ///< sync bit count - transmit
static  U8                  nRcvSyncBitCnt;       ///< sync bit count - receive
static  U8                  nXmtStopBitCnt;       ///< stop bit count
static  U8                  nRcvStopBitCnt;       ///< receive stop bit count
static  U8                  nActRcvBits;          ///< actual number of receive bits
static  U8                  nCurData;             ///< current data byte
static  VU8                 nTimeout;             ///< timeout value
static  U8                  nRcvBitCnt;           ///< bit count
static  BOOL                bPrvBit;              ///< previous bit in decode
static  BOOL                bCurBit;              ///< current bit to decode
static  BOOL                bFirstEdge;           ///< first edge flag
static  VBOOL               bXmtInProgress;       ///< transmit in progress flag
static  U8                  nXmtBitCnt;           ///< transmit bit count
static  MANCHESTERCODECDEF  tDef;           

// local function prototypes --------------------------------------------------
static  void      ShiftRcvBit( SHIFTBIT eBit );
static  void      DecodeBit( void );
static  BITSTATE  DecodeBitTime( U16 wCapTime );

// constant parameter initializations -----------------------------------------

/******************************************************************************
 * @function ManchesterCodec_Initialize
 *
 * @brief initialize the manchester codec
 *
 * This function will perform all initialization for the manchester encoder/
 * decoder
 *
 * @param[in] ptDef       pointer to the definition structure
 *
 *****************************************************************************/
#if ( MANCHESTERCODEC_ENALBE_DYNAMICINIT == OFF )
  extern  void  ManchesterCodec_Initialize( void )
#else
  extern  void  ManchesterCodec_Initialize( PMANCHESTERCODECDEF ptDef );
#endif // MANCHESTERCODEC_ENALBE_DYNAMICINIT
{
  U32 uHalfBitMeanTime, uTolerance;

#if ( MANCHESTERCODEC_ENALBE_DYNAMICINIT == ON )
  // copy the data
  memcpy( &tDef, ptDef, MANCHESTERCODECDEF_SIZE );
#else
  // set the data from the constants
  tDef.wBaudRate          = MANCHESTERCODEC_BAUD_RATE;
  tDef.nNumSyncBits       = MANCHESTERCODEC_NUM_SYNC_BITS;
  tDef.nNumStopBits       = MANCHESTERCODEC_NUM_STOP_BITS;
  tDef.nBitTolerancePct   = MANCHESTERCODEC_BIT_TOLERANCE_PCT;
  tDef.bRisingEdgeEnable  = MANCHESTERCODEC_ENABLE_RCVPOLARITY;
#endif // MANCHESTERCODEC_ENALBE_DYNAMICINIT

  // calculate the half bit mean time in CPU clock counts
  uHalfBitMeanTime = ManchesterCodec_GetClockFreq( ) /  ( tDef.wBaudRate << 2 );

  // calculate the tolerance
  uTolerance = ( tDef.nBitTolerancePct * uHalfBitMeanTime ) / 100;

  // call the local initialization
  ManchesterCodec_LocalInitialize( uHalfBitMeanTime );
  
   // calculate the min/max bit times
  wHalfBitMinTime = uHalfBitMeanTime - uTolerance;
  wHalfBitMaxTime = uHalfBitMeanTime + uTolerance;
  wFullBitMinTime = ( uHalfBitMeanTime * 2 ) - uTolerance;
  wFullBitMaxTime = ( uHalfBitMeanTime * 2 ) + uTolerance;

  // set the bit counts
  nXmtSyncBitCnt = tDef.nNumSyncBits;
  nXmtStopBitCnt = tDef.nNumStopBits * 2;

  // disable transmit in progress
  bXmtInProgress = FALSE;
  nRcvBitCnt = 0;
  bFirstEdge = TRUE;
  eRcvPhase = RCV_PHASE_DONE;
}

/******************************************************************************
 * @function ManchesterCodec_Xmit
 *
 * @brief transmit function
 *
 * This function will setup and transmit the data 
 *
 * @param[in]   pnData    pointer to the data to be sent
 * @param[in]   nLength   length of the data
 *
 *****************************************************************************/
void ManchesterCodec_Xmit( PU8 pnData, U8 nLength )
{
  // copy the parameters
  pnXmtData = pnData;
  nXmtLen = nLength;

  // reset the transmit state/bit
  eXmtPhase = XMT_PHASE_SYNCFH;
  bCurBit = ON;
  nXmtBitCnt = 0;

  #if ( MANCHESTERCODEC_XMTDBG_ENABLE == 1 )
  DebugManager_AddElement( MANCHESTERCODEC_DBGXMT_BASE, 0x0000 );
  DebugManager_AddElement( MANCHESTERCODEC_DBGRCV_BASE | 0x80, (( nXmtSyncBitCnt << 8 ) | nXmtStopBitCnt ));
  #endif // ( MANCHESTERCODEC_XMTDBG_ENABLE == 1 )

  // set transmit in progress
  Interrupt_Disable( );
  bXmtInProgress = TRUE;
  Interrupt_Enable( );
}

/******************************************************************************
 * @function ManchesterCodec_Recv
 *
 * @brief set up and receive the data
 *
 * This function will setup and receive data,
 *
 * @param[in]   pnData        pointer to the storage for the incoming data
 * @param[in]   nLength       expected length of the incoming data
 *
 *****************************************************************************/
void ManchesterCodec_Recv( PU8 pnData, U8 nLength )
{
  // copy the parameters
  pnRcvData = pnData;
  nRcvLen = nLength;
  
  #if ( MANCHESTERCODEC_RCVDBG_ENABLE == 1 )
  DebugManager_AddElement( MANCHESTERCODEC_DBGRCV_BASE, 0x0000 );
  #endif // ( MANCHESTERCODEC_RCVDBG_ENABLE == 1 )

  // reset the recevie state/sync bit count and first edge
  eRcvPhase = RCV_PHASE_SYNC;
  nRcvSyncBitCnt = 0;
  bFirstEdge = TRUE;
  nTimeout = 0;
  nRcvBitCnt = 0;

  // flush the queue/task manager too
  ManchesterCodec_FlushRecvEvents( );

  // turn on the timer to start the process
  ManchesterCodec_RecvTimerControl( ON );
}

/******************************************************************************
 * @function ManchesterCodec_StopRecv
 *
 * @brief stop receiving data
 *
 * This function will stop recieving data
 *
 *****************************************************************************/
void ManchesterCodec_StopRecv( void )
{
  #if ( MANCHESTERCODEC_RCVDBG_ENABLE == 1 )
  DebugManager_AddElement( MANCHESTERCODEC_DBGRCV_BASE, 0xFFFF );
  #endif // ( MANCHESTERCODEC_RCVDBG_ENABLE == 1 )

  // turn on the timer to start the process
  ManchesterCodec_RecvTimerControl( OFF );
}

/******************************************************************************
 * @function ManchesterCodec_ProcessXmtTimer
 *
 * @brief process a manchester output 
 *
 * This function executes at every half bit time and will set the appropriate
 * level of the output pin
 *
 *****************************************************************************/
void ManchesterCodec_ProcessXmtTimer( void )
{
  // test for transmit in progress
  if ( bXmtInProgress == TRUE )
  {
    // output the current bit
    ManchesterCodec_OutputControl( bCurBit );

    #if ( MANCHESTERCODEC_XMTDBG_ENABLE == 1 )
    DebugManager_AddElement( MANCHESTERCODEC_DBGXMT_BASE | 0x08 | bCurBit, ( nXmtBitCnt << 8 ) | eXmtPhase );
    #endif // ( MANCHESTERCODEC_XMTDBG_ENABLE == 1 )

    // increment the bit count
    nXmtBitCnt++;

    // determine which phase we are in
    switch( eXmtPhase )
    {
      case XMT_PHASE_SYNCFH :
        // set the current bit to a 1
        bCurBit = OFF;
      
        // determine the next phase
        if ( nXmtBitCnt == nXmtSyncBitCnt )
        {
          // set up for the first data
          nXmtMask = 0x80;
          eXmtPhase = XMT_PHASE_DATAFH;
        }
        else
        {
          // go to sync first half
          eXmtPhase = XMT_PHASE_SYNCSH;
        }
        break;
      
      case XMT_PHASE_SYNCSH :
        // clear bit
        bCurBit = ON;
        eXmtPhase = XMT_PHASE_SYNCFH;
        break;
      
      case XMT_PHASE_DATAFH :
        // determine the next bit/adjust the mask
        bCurBit = ( *( pnXmtData ) & nXmtMask ) ? ON : OFF;
        nXmtMask >>= 1;
        if ( nXmtMask == 0 )
        {
          // reset mask/adjust the pointer
          nXmtMask = 0x80;
          pnXmtData++;
        }

        // goto data second half
        eXmtPhase = XMT_PHASE_DATASH;
        break;

      case XMT_PHASE_DATASH :
        // toggle the current bit/back to first half
        bCurBit ^= 1;
      
        // check for test for end
        if ( nXmtMask == 0x80 )
        {
          // check for done
          if ( --nXmtLen == 0 )
          {
            // goto to stop
            eXmtPhase = XMT_PHASE_STOP;

            // clear the bit count
            nXmtBitCnt = 0;
          }
          else
          {
            // back to DATA first half
            eXmtPhase = XMT_PHASE_DATAFH;
          }
        }
        else
        {
          // just back to first half
          eXmtPhase = XMT_PHASE_DATAFH;
        }
        break;
      
      case XMT_PHASE_STOP :
        bCurBit = OFF;
        if ( nXmtBitCnt == nXmtStopBitCnt )
        {
          // goto done
          eXmtPhase = XMT_PHASE_DONE;
        }
        break;

      case XMT_PHASE_DONE :
        // clear transmit in progress flag
        bXmtInProgress = FALSE;

        // post done to the appropriate task
        ManchesterCodec_PostXmitEvent( MANCHESTERCODEC_XMIT_DONE );
      
        #if ( MANCHESTERCODEC_XMTDBG_ENABLE == 1 )
        DebugManager_AddElement( MANCHESTERCODEC_DBGXMT_BASE, 0xD0EE );
        #endif

        // go to the next state
        eXmtPhase = XMT_PHASE_SYNCFH;
        break;
      
      default :
        break;
    }
  }    
  else
  {
    // increment and check for post bit time
    if ((( ++nRcvBitCnt & 0x01 ) == 0 ) && ( eRcvPhase != RCV_PHASE_DONE ))
    {
      // post a bit time event to the receive task
      ManchesterCodec_ProcessReceive( TRUE, 0, 0 );
    }
  }
}

/******************************************************************************
 * @function ManchesterCodec_ProcessReceive
 *
 * @brief process a receive value
 *
 * This function will process a receive value
 *
 * @param[in]   bTimeout    timeout detected
 * @param[in]   wBitTime    bit time
 * @param[in]   eEdge       edge
 *
 *****************************************************************************/
void ManchesterCodec_ProcessReceive( BOOL bTimeout, U16 wBitTime, MANCHESTERCODECEDGE eEdge )
{
  // check for a timeout
  if ( bTimeout )
  {
    // if we are in the middle of a decode and we have hit a timeout - reset
    if (( bFirstEdge == FALSE ) && ( eRcvPhase != RCV_PHASE_STOP ) && ( eRcvPhase != RCV_PHASE_DONE ))
    {
      #if ( MANCHESTERCODEC_RCVDBG_ENABLE == 1 )
      DebugManager_AddElement( MANCHESTERCODEC_DBGRCV_BASE | 0x0008, nTimeout );
      #endif // ( MANCHESTERCODEC_RCVDBG_ENABLE == 1 )

      if ( ++nTimeout >= 4 )
      {
        #if ( MANCHESTERCODEC_RCVDBG_ENABLE == 1 )
        DebugManager_AddElement( MANCHESTERCODEC_DBGRCV_BASE | 0x0009, 0xD0EE );
        #endif // ( MANCHESTERCODEC_RCVDBG_ENABLE == 1 )

        // reset the receiver
        eRcvPhase = RCV_PHASE_DONE;
        ManchesterCodec_RecvTimerControl( OFF );
        ManchesterCodec_PostRecvEvent( MANCHESTERCODEC_RECV_DONE );        
      }
    }

    // if we are in stop bit detect, process bit
    if ( eRcvPhase == RCV_PHASE_STOP )
    {
      ShiftRcvBit( SHIFT_BIT_STOP );
    }
  }
  else
  {
    // edge received, clear the timeout
    nTimeout = 0;

    #if ( MANCHESTERCODEC_RCVDBG_ENABLE == 1 )
    DebugManager_AddElement( MANCHESTERCODEC_DBGRCV_BASE | ( bFirstEdge << 4 ) | tData.eEdge, tData.wValue );
    #endif // ( MANCHESTERCODEC_RCVDBG_ENABLE == 1 )

    // ensure we are not in a stop bit phase
    if ( eRcvPhase != RCV_PHASE_STOP )
    {
      if ((( ON == tDef.bRisingEdgeEnable ) && ( MANCHESTERCODEC_EDGE_RISE == eEdge )) ||
          (( OFF == tDef.bRisingEdgeEnable ) && ( MANCHESTERCODEC_EDGE_FALL == eEdge )))
      {
        // first pulse
        if ( bFirstEdge )
        {
          // clear the first edge
          bFirstEdge = FALSE;

          // set the previous bit to a 1
          bPrvBit = ON;
          wOffCapTime = wHalfBitMinTime;
          wOnCapTime = wHalfBitMinTime;

          // post an edge event
          ManchesterCodec_PostRecvEvent( MANCHESTERCODEC_RECV_EDGE );
        }
        else
        {
          // get the high time/decode
          wOffCapTime = wBitTime;
        }
      }
      else if ( bFirstEdge == FALSE )
      {
        // get the lo time
        wOnCapTime = wBitTime;

        // decode the bit
        DecodeBit( );
      }
    }
    else
    {
      // turn off the receiver/flush any events/post the event
      ManchesterCodec_RecvTimerControl( OFF );
      ManchesterCodec_FlushRecvEvents( );
      ManchesterCodec_PostRecvEvent( MANCHESTERCODEC_RECV_EROR );        

      // reset the state
      eRcvPhase = RCV_PHASE_DONE;
      bFirstEdge = TRUE;

      #if ( MANCHESTERCODEC_RCVDBG_ENABLE == 1 )
      DebugManager_AddElement( MANCHESTERCODEC_DBGRCV_BASE, 0xC0DE );
      #endif // ( MANCHESTERCODEC_RCVDBG_ENABLE == 1 )
    }
  }
}

/******************************************************************************
 * @function DecodeBit
 
 * @brief decode the bit 
 *
 * This function will determine which pulse pairs were received
 *
 *****************************************************************************/
static void DecodeBit( void )
{
  U8  nDecState;

  // build the action
  nDecState = ( bPrvBit << BIT_POS_P );
  nDecState |= ( DecodeBitTime( wOffCapTime ) << BIT_POS_D );
  nDecState |= ( DecodeBitTime( wOnCapTime ) << BIT_POS_A );

  #if ( MANCHESTERCODEC_RCVDBG_ENABLE == 1 )
  DebugManager_AddElement( MANCHESTERCODEC_DBGRCV_BASE | 0x000A, nDecState );
  #endif // ( MANCHESTERCODEC_RCVDBG_ENABLE == 1 )

  // now determine the bits to shift
  switch( nDecState )
  {
    case DEC_STATE_P0_DHAH :
    case DEC_STATE_P1_DFAH :
       // shift a 0
      ShiftRcvBit( SHIFT_BIT_0 );
      break;

    case DEC_STATE_P1_DHAH :
      // shift a 1
      ShiftRcvBit( SHIFT_BIT_1 );
      break;

    case DEC_STATE_P0_DHAF :
    case DEC_STATE_P1_AFDF :
      // shift a 0,1
      ShiftRcvBit( SHIFT_BIT_0 );
      ShiftRcvBit( SHIFT_BIT_1 );
      break;

    default :
      // clear the bit count - indicate an error
      #if ( MANCHESTERCODEC_RCVDBG_ENABLE == 1 )
      DebugManager_AddElement(  MANCHESTERCODEC_DBGRCV_BASE | 0x000B, 0xFFFF );
      #endif // ( MANCHESTERCODEC_RCVDBG_ENABLE == 1 )

      // turn off the receive/post event
      eRcvPhase = RCV_PHASE_DONE;
      ManchesterCodec_RecvTimerControl( OFF );
      ManchesterCodec_PostRecvEvent( MANCHESTERCODEC_RECV_EROR );        
      break;
  }
}

/******************************************************************************
 * @function ShiftRcvBit
 *
 * @brief shift the bit into the data
 *
 * This function will shift a bit in the current data value, store the data
 * in the buffer and adjust the expected length
 *
 * @param[in]   bBit      current bit value
 *
 *****************************************************************************/
static void ShiftRcvBit( SHIFTBIT eBit )
{
  #if ( MANCHESTERCODEC_RCVDBG_ENABLE == 1 )
  DebugManager_AddElement(  MANCHESTERCODEC_DBGRCV_BASE | 0x000C, ( eRcvPhase << 4 ) | eBit );
  #endif // ( MANCHESTERCODEC_RCVDBG_ENABLE == 1 )

  // update previous bit
  bPrvBit = ( BOOL )eBit;

  // determine the state
  switch( eRcvPhase )
  {
    case RCV_PHASE_SYNC :
      if ( ++nRcvSyncBitCnt == tDef.nNumSyncBits )
      {
        // go to the data state/reset the bit count
        eRcvPhase = RCV_PHASE_DATA;
        nActRcvBits = 0;
      }
      break;
      
    case RCV_PHASE_DATA :
      // shift and add the bit
      nCurData <<= 1;
      nCurData |= eBit;
    
      // test for done
      if ( ++nActRcvBits == 8 )
      {
        // reset the receive bits
        nActRcvBits = 0;

        // store the data
        *( pnRcvData++ ) = nCurData;
        if ( --nRcvLen == 0 )
        {
          #if ( MANCHESTERCODEC_RCVDBG_ENABLE == 1 )
          U16UN tTemp;
          tTemp.anValue[ LE_U16_MSB_IDX ] = *( pnRcvData - 2 );
          tTemp.anValue[ LE_U16_LSB_IDX ] = *( pnRcvData - 1 );
          DebugManager_AddElement(  MANCHESTERCODEC_DBGRCV_BASE | 0x000E, tTemp.wValue );
          #endif // ( MANCHESTERCODEC_RCVDBG_ENABLE == 1 )

          // reset the state
          nRcvStopBitCnt = 0;
          eRcvPhase = RCV_PHASE_STOP;
        }
      }
      break;

    case RCV_PHASE_STOP :
      if ( eBit == SHIFT_BIT_STOP )
      {
        // test for done
        if ( ++nRcvStopBitCnt == ( tDef.nNumStopBits + 2 ))
        {
          // turn off the receiver/post the event
          ManchesterCodec_RecvTimerControl( OFF );
          ManchesterCodec_FlushRecvEvents( );
          ManchesterCodec_PostRecvEvent( MANCHESTERCODEC_RECV_DONE );        
          
          // reset the state/set first edge flag
          eRcvPhase = RCV_PHASE_DONE;
          bFirstEdge = TRUE;

          #if ( MANCHESTERCODEC_RCVDBG_ENABLE == 1 )
          DebugManager_AddElement(  MANCHESTERCODEC_DBGRCV_BASE | 0x000F, 0xD0EE  );
          #endif // ( MANCHESTERCODEC_RCVDBG_ENABLE == 1 )
        }
      }
      break;
      
    case RCV_PHASE_DONE :
      break;

    default :
      // should never happen
      eRcvPhase = RCV_PHASE_SYNC;
      break;
  }
}

/******************************************************************************
 * @function DecodeBitTime
 *
 * @brief decode the bit time
 *
 * This function determine what time of bit was receiveid
 *
 * @param[in]   wCapTime      current bit value
 *
 * @return      bit state ( ERR,H, F )
 *
 *****************************************************************************/
static BITSTATE DecodeBitTime( U16 wCapTime )
{
  BITSTATE eBitState = BIT_STATE_ERR;

  // determine if this is a valid half bit time
  if (( wCapTime >= wHalfBitMinTime ) && ( wCapTime <= wHalfBitMaxTime ))
  {
    // bit state half
    eBitState = BIT_STATE_H;
  }
  else if (( wCapTime >= wFullBitMinTime ) && ( wCapTime <= wFullBitMaxTime ))
  {
    // bit state full
    eBitState = BIT_STATE_F;
  }
  
  // return the dec state
  return( eBitState );
}

/**@} EOF ManchesterCodec.c */
