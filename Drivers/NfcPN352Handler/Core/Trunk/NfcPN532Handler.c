/******************************************************************************
 * @file NfcPN532Handler.c
 *
 * @brief NFC PN532 implementation
 *
 * This file provides the implementation for the PN532 NFC chip driver
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
 * \addtogroup NfcPN532Handler
 * @{
 *****************************************************************************/

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "NfcPN532Handler/NfcPN532Handler.h"

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------
/// define the callback events
#define NFCPN532_EVENT_MSGRCVD                        ( 0xFC0E )
#define NFCPN532_EVENT_MSGXMIT                        ( 0xFC55 )
#define NFCPN532_EVENT_WAITTIME                       ( 0xFC33 )

/// define the maximum transmit length
#define MAX_XMIT_LENGTH                               ( 128 )
#define MAX_RECV_LENGTH                               ( 128 )

/// define the address for the PN532  
#define PN532_DEVICE_ADDR                             ( 0x24 )

// define the message overhead size
#define XMTMSG_OVERHEAD_SIZE                          ( 8 )

/// define the message preamble/start/postamble characters
#define PN532_PREAMBLE_CHAR                           ( 0x00 )
#define PN532_STARTCD1_CHAR                           ( 0x00 )
#define PN532_STARTCD2_CHAR                           ( 0xFF )
#define PN532_PSTAMBLE_CHAR                           ( 0x00 )

/// define the message direction
#define PN532_DIR_HOSTTOPN532                         ( 0xD4 )
#define PN532_DIR_PN532TOHOST                         ( 0xD5 )

// define the commands
#define PN532_CMD_GETFIRMWAREREV                      ( 0x02 )
#define PN532_CMD_SAMCONFIGURATION                    ( 0x14 )
#define PN532_CMD_LISTPASSIVETARGET                   ( 0x4A )

#define PN532_CMD_LISTPASSIVETARGET1                  ( 0xFF )

/// define the firmware revision packet size
#define PN532_PKT_SIZE_FRMREV                         ( 12 )

/// define the SAM configuration results packets size
#define PN532_PKT_SIZE_SAMCONFIG                      ( 8 )

/// define the read UID buffer length
#define PN532_PKT_SIZE_LISTUID                        ( 20 )

/// define the key size
#define PN532_KEY_LENGTH                              ( 6 )

// enumerations ---------------------------------------------------------------
/// enumerate the communication tasks
typedef enum _COMMSTATE
{
  COMM_STATE_IDLE = 0,
  COMM_STATE_WAIT
} COMMSTATE;

// structures -----------------------------------------------------------------

// global parameter declarations ----------------------------------------------

// local parameter declarations -----------------------------------------------
static  U8              nXmitChecksum;
static  U8              nXmitBufIndex;
static  U8              anXmitBuffer[ MAX_XMIT_LENGTH ];
static  U8              anRecvBuffer[ MAX_RECV_LENGTH ];
static  U8              nXmitLength;
static  U16             wRcvTimeout;
static  COMMSTATE       eCommState;
static  PVMSGCALLBACK   pvMsgCallback;
static  U32UN           tFirmwareRevision;
static  U8              anLastUID[ NFCPN532HANDLER_MAX_UID_LENGTH ];
static  U8              nLastUIDLength;

// local function prototypes --------------------------------------------------
static  void  ReadData( PU8 pnBuffer, U8 nLength );
static  void  WriteCommand( PU8 pnBuffer, U8 nLength );
static  void  StuffBuffer( PU8 pnBuffer, U8 nData, BOOL bAddToChecksum );
static  BOOL  ProcessRcvMessage( void );

// constant parameter initializations -----------------------------------------
/// define the ack packet
static  const CODE  U8  anAckPacket[ ] = { 0x01, 0x00, 0xFF, 0x00, 0xFF, 0x00 };
#define ACK_PACKET_SIZE                 ( sizeof( anAckPacket )))

/// define the firmware revision packet
static  const CODE U8   anFrmPacket[ ] = { 0x00, 0xFF, 0x06, 0xFA, 0xD5, 0x03 };
#define FRM_PACKET_SIZE                 ( sizeof( anFrmPacket ))

/// define the authentication keys
static  const CODE U8   anAuthKeys[ ][ PN532_KEY_LENGTH ] =
{
  { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
  { 0xA0, 0xB0, 0xC0, 0xD0, 0xE0, 0xF0 },
  { 0xA1, 0xB1, 0xC1, 0xD1, 0xE1, 0xF1 },
  { 0xB0, 0xB1, 0xB2, 0xB3, 0xB4, 0xB5 },
  { 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF }
  { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF },
};

/******************************************************************************
 * @function NfcPN532Handler_Initialize
 *
 * @brief initialization
 *
 * This function will perform any required initialization
 *
 *****************************************************************************/
void NfcPN532Handler_Initialize( void )
{
  // reset the state
  eCommState = COMM_STATE_IDLE;

  // initialize by resetting the chip
  NfcPN532Handler_SetReset( OFF );
  NfcPN532Handler_SetReset( ON );
  NfcPN532Handler_DelayMSEC( 400 );
  NfcPN532Handler_SetReset( OFF );
}
 
/******************************************************************************
 * @function NfcPN532Handler_RequestFirmwareRevision
 *
 * @brief request a firmware revision
 *
 * This function requests the firmware revision from the device
 *
 * @param[in]   pvCallback    callback when the revision is returned
 *
 *****************************************************************************/
void NfcLPN352Handler_RequestFirwareRevision( PVMSGCALLBACK pvCallback )
{
  // store the callback
  pvMsgCallback = pvCallback;

  // build the command and send it
  nXmitLength = 0;
  anXmitBuffer[ nXmitLength++ ] = PN532_CMD_GETFIRMWAREREV;
  
  // queue a trnasmit 
  NfcPN532Handler_PostEvent( NFCPN532_EVENT_MSGXMIT );
}

/******************************************************************************
 * @function NfcPN352Handler_GetFirmwareRevision
 *
 * @brief get the last read firmware revision
 *
 * This function will return the last read revision
 *
 * @return    last read firmware revision
 *
 *****************************************************************************/
U32 NfcPN352Handler_GetFirmwareRevision( void )
{
  // return the firmware revision
  return( tFirmRevision.uValue );
}
 
/******************************************************************************
 * @function NfcPN532Handler_ConfigureSecureAccessModule
 *
 * @brief setup the Secure Access Module
 *
 * This function will set up the SAM to read tags
 *
 * @param[in]   pvCallback    callback when the revision is returned
 *
 *****************************************************************************/
VOID NfcPN532Handler_ConfigureSecureAccessModule( PVMSGCALLBACK pvCallback  )
{
  // store the callback
  pvMsgCallback = pvCallback;

  // build the command and send it
  nXmitLength = 0;
  anXmitBuffer[ nXmitLength++ ] = PN532_CMD_SAMCONFIGURATION;
  anXmitBuffer[ nXmitLength++ ] = 0x01; // normal mode;
  anXmitBuffer[ nXmitLength++ ] = 0x14; // timeout 50ms * 20 = 1 second
  anXmitBuffer[ nXmitLength++ ] = 0x01; // use IRQ pin!
  
  // queue a trnasmit 
  NfcPN532Handler_PostEvent( NFCPN532_EVENT_MSGXMIT );
}
 
/******************************************************************************
 * @function NfcPN532Handler_ConfigureSecureAccessModule
 *
 * @brief setup the Secure Access Module
 *
 * This function will set up the SAM to read tags
 *
 * @param[in]   nBaudrate     card baudrate
 * @param;in[   wTimeoutMsecs timeout in mseconds
 * @param[in]   pvCallback    callback when the revision is returned
 *
 *****************************************************************************/
void NfcPN532Handler_ScanForCards( U8 nBaudrate, U16 wTimeoutMsecs, PVMSGCALLBACK pvCallback )
{
  // store the callback
  pvMsgCallback = pvCallback;
  
  // set the timeout
  wRcvTimeout = wTimeoutMsecs;

  // build the command and send it
  nXmitLength = 0;
  anXmitBuffer[ nXmitLength++ ] = PN532_CMD_LISTPASSIVETARGET;
  anXmitBuffer[ nXmitLength++ ] = 1;    //number of cards
  anXmitBuffer[ nXmitLength++ ] = nBaudrate;
  
  // queue a trnasmit 
  NfcPN532Handler_PostEvent( NFCPN532_EVENT_MSGXMIT );
}
 
/******************************************************************************
 * @function NfcPN532Handler_GetLastUID
 *
 * @brief get the last UID
 *
 * This function will get the last UID read
 *
 * @param[io]   pnUid     pointer to store the UID
 * @param[io]   pnLength  length of the UID
 *
 *****************************************************************************/
void NfcPN532Handler_GetLastUID( PU8 pnUid, PU8 pnLength )
{
  U8  nIdx;
  
  // get the values
  for ( nIdx = 0; nIdx < nLastUIDLength; nIdx++ )
  {
    *( pnUid + nIdx ) = anLastUID[ nIdx ];
  }
  
  // set the length
  *( pnLength ) = nLastUIDLength;
}
 
/******************************************************************************
 * @function NfcPN532Handler_ConfigureSecureAccessModule
 *
 * @brief setup the Secure Access Module
 *
 * This function will set up the SAM to read tags
 *
 * @param[in]   pnUid         pointer to the UID
 * @param[in]   nUidLength    length of the uid
 * @param[in]   uBlockNumber  block number
 * @param[in]   nKeyNumber    key number
 * @param[in]   eAuthKey      authorization key to define either preset or user defined
 * @param[in]   pnKeyData     pointer to the key data
 * @param[in]   pvCallback    callback when the revision is returned
 *
 *****************************************************************************/
void NfcPN532Handler_AuthenticateBlock( PU8 pnUid, U8 nUidLength, U32 uBblockNumber, U8 nKeyNumber, NFCPN532AUTHKEY eAuthKey, PU8 pnKeyData, PVMSGCALLBACK pvCallback )
{
  U8  nIdx;
  PU8 pnKey;
  
  // store the callback
  pvMsgCallback = pvCallback;

  // build the command and send it
  nXmitLength = 0;
  anXmitBuffer[ nXmitLength++ ] = PN532_COMMAND_INDATAEXCHANGE;
  anXmitBuffer[ nXmitLength++ ] = 1;                              //number of cards
  anXmitBuffer[ nXmitLength++ ] = ( nKeyNumber != 0 ) ? MIFARE_CMD_AUTH_B : MIFARE_CMD_AUTH_A;
  anXmitBuffer[ nXmitLength++ ] = ( U8 )uBlockNumber;
  
  // set the pointer to the key
  if ( eAuthKey < NFCPN532_AUTHKEY_MAX )
  {
    pnKey = anAuthKeys[ nKeyNumber ][ 0 ];
  }
  else if ( eAuthKey == NFCPN532_AUTHKEY_USRDEF )
  {
    pnKey = pnKeyData;
  }
  else
  {
    pnKey = anAuthKeys[ 0 ][ 0 ];
  }

  // copy the key
  for ( nIdx = 0; nIdx < MAX_KEY_LEN; nIdx++ )
  {
    anXmitBuffer[ nXmitLength++ ] = *( pnKey  + nIdx );
  }
  
  // copy the uid
  for ( nIdx = 0; nIdx < nUidLength; nIdx++ )
  {
    anXmitBuffer[ nXmitLength++  ] = *( pnUid + nIdx );
  }

  // queue a trnasmit 
  NfcPN532Handler_PostEvent( NFCPN532_EVENT_MSGXMIT );

  if (! sendCommandCheckAck(pn532_packetbuffer, 10+_uidLen))
    return 0;

  // Read the response packet
  readdata(pn532_packetbuffer, 12);

  // check if the response is valid and we are authenticated???
  // for an auth success it should be bytes 5-7: 0xD5 0x41 0x00
  // Mifare auth error is technically byte 7: 0x14 but anything other and 0x00 is not good
  if (pn532_packetbuffer[7] != 0x00)
  {
    return 0;
  }

  return 1;
  
}
 
/******************************************************************************
 * @function NfcPN532Handler_CommunicationsTask
 *
 * @brief communications task
 *
 * This function process the events from the communications task
 *
 * @param[in]   wArg      task argument
 *
 * @return      TRUE      flush event
 *
 *****************************************************************************/
void NfcPN532Handler_ProcessEvent( U16 wArg )
{
  BOOL  bMsgStatus;

  // process the comm state
  switch( eCommState )
  {
    case COMM_STATE_IDLE :
      // only process the transmit events
      switch( wArg )
      {
        case NFCPN532_EVENT_MSGXMIT :
          // send the message
          WriteCommand( anXmitBuffer, nXmitLength );

          // start the timeout 
          TaskManager_StartTimer( NFCPN532_TASK_ENUM, TASK_TIME_MSECS( wRcvTimeout ));

          // go to wait
          eCommState = COMM_STATE_WAIT;
          break;
          
        case NFCPN532_EVENT_WAITTIME :
          // start the timeout 
          TaskManager_StartTimer( NFCPN532_TASK_ENUM, TASK_TIME_MSECS( wRcvTimeout ));

          // go to wait
          eCommState = COMM_STATE_WAIT;
          break;
          
        default :
          break;
      }
      break;

    case COMM_STATE_WAIT :
      switch( wArg )
      {
        case TASK_TIMEOUT_EVENT :
          bMsgStatus = FALSE;
          break;

        case NFCPN532_EVENT_MSGRCVD :
          // set the message status based on acknowledge
          if ( bMsgStatus = ReadAcknowledge( )) == TRUE )
          {
            // process the incoming message
            bMessageStatus = ProcessRcvMessage( );
          }
          break;

        default :
          bMsgStatus = FALSE;
          break;
      }

      // check for a valid callback
      if ( pvMsgCallback != NULL )
      {
        // do it
        pvMsgCallback( bMsgStatus );
      }

      // back to idle
      eCommState = COMM_STATE_IDLE;
      break;

    default :
      break;
  }
}

/******************************************************************************
 * @function NfcPN532Handler_IrqCallback
 *
 * @brief IRQ callback handler
 *
 * This function processes the IRQ callback
 *
 * @param[in]   nIrqEnum
 * @param[in]   nEvent
 *
 *****************************************************************************/
void NfcPN532Handler_IrqCallback( U8 nIrqEnum, U8 nEvent )
{
  // post the event to the handler
  NfcPN532Handler_PostEvent( NFCPN532_EVENT_MSGRCVD );
}
 
/******************************************************************************
 * @function ReadAcknowledge
 *
 * @brief read the acknowledge packet
 *
 * This function will read the acknolowedge packet
 *
 * @return      TRUE if valid ack, FALSE if not
 *
 *****************************************************************************/
static BOOL ReadAcknowledge( void )
{
  U8    anAckBuffer [ ACK_PACKET_SIZE ];
  BOOL  bStatus = FALSE;

  // read the packet
  ReadData( anAckBuffer, ACK_PACKET_SIZE );

  // test for valid packet
  if ( MEMNCMP_P( anAckBuffer, anAckPacket, ACK_PACKET_SIZE ) == 0 )
  {
    // set good status
    bStatus = TRUE;
  }

  // return status
  return( bStatus );
}

/******************************************************************************
 * @function ReadData 
 *
 * @brief read data from the device
 *
 * This function will read the data from the device
 *
 * @param[in]   pnBuffer  pointer to the buffer
 * @param[in]   nLength   number of bytes to read
 *
 *****************************************************************************/
static void ReadData( PU8 pnBuffer, U8 nLength )
{
  // now read it
  NfcPN532Handler_I2CWrite( pnBuffer, nLength );
}

/******************************************************************************
 * @function WriteCommand
 *  
 * @brief   write a command sequence to the device
 *
 * This function will write the designated command sequence to the device
 *
 * @param[in]   pnBuffer      pointer to the command sequence
 * @param[in]   nLength       length of the data
 *
 *****************************************************************************/
static void WriteCommand( PU8 pnBuffer, U8 nLength )
{
  PU8       pnXmitBuffer;
  U8        nTemp;

  // clear the checksum/index
  nXmitChecksum = 0;
  nXmitBufIndex = 0;

  // allocate space for the transmit buffer
  if (( pnXmitBuffer = malloc( nLength + XMTMSG_OVERHEAD_SIZE )) != NULL )
  {
    // stuff the header
    StuffBuffer( pnXmitBuffer, PN532_PREAMBLE_CHAR, TRUE );
    StuffBuffer( pnXmitBuffer, PN532_STARTCD1_CHAR, TRUE );
    StuffBuffer( pnXmitBuffer, PN532_STARTCD2_CHAR, TRUE );

    // now send the command length/2's complement of the length
    nTemp = nLength + 1;
    StuffBuffer( pnXmitBuffer, nTemp, FALSE );
    nTemp = ~nTemp;
    nTemp++;
    StuffBuffer( pnXmitBuffer, nTemp, FALSE );

    // send the host option
    StuffBuffer( pnXmitBuffer, PN532_DIR_HOSTTOPN532, TRUE );

    // if there is data
    for ( nTemp = 0; nTemp < nLength; nTemp++ )
    {
      StuffBuffer( pnXmitBuffer, *( pnBuffer + nTemp ), TRUE );
    }

    // stuff the complement of the checksum
    StuffBuffer( pnXmitBuffer, ~nXmitChecksum, FALSE );

    // stuff the postamble
    StuffBuffer( pnXmitBuffer, PN532_PSTAMBLE_CHAR, FALSE );

    // now send it
    NfcPN532Handler_I2CWrite( pnXmitBuffer, nXmitBufIndex );

    // free the buffer
    free( pnXmitBuffer );
  }
}
 
/******************************************************************************
 * @function StuffBuffer
 *  
 * @brief   stuff a byte of data into the transmit buffer
 *
 * This function stuff a byte to the transmit buffer and adjust the checksum
 * if allowed
 *
 * @param[in]   pnBuffer        pointer to the transmit buffer
 * @param[in]   nData           data byte to add to buffer
 * @param[in]   bAddToChecksum  add to checksum enable flag
 *
 *****************************************************************************/
static void StuffBuffer( PU8 pnBuffer, U8 nData, BOOL bAddToChecksum )
{
  // stuff the data/add to checksum
  *( pnBuffer + nXmitBufIndex++ ) = nData;

  // check for add to checksm
  if ( bAddToChecksum == TRUE )
  {
    // add it
    nXmitChecksum += nData;
  }
}

/******************************************************************************
 * @function ProcessRcvMessage
 *
 * @brief process the data on a received message
 *
 * This function will process ther ecieve data
 *
 * @return  TRUE if OK, FALSE if not
 * 
 *****************************************************************************/
static BOOL ProcessRcvMessage( void )
{
  U8    nIdx;
  BOOL  bResult = FALSE:

  // determine what was the last command
  switch( anXmitBuffer[ 0 ] )
  {
    case PN532_CMD_GETFIRMWAREREV :
      // read the data
      ReadData( anRecvBuffer, PN532_PKT_SIZE_FRMREV );
      
      // check for basic packet stuff
      if ( MEMCMP_P( anRcvBuffer, anFrmPacket, FRM_PACKET_SIZE ) == 0 )
      {
        // valid response, build the firmware revision
        for( nIdx = U32UN_LSB_IDX; nIdx <= U32UN_MSB_IDX; nIdx++ )
        {
          // copy the data
          tFirmwareRevision.anValues[ nIdx ] = anRcvBuffer[ FRM_PACKET_SIZE + nIdx ];
        }
        
        // set the result to TRUE
        bREsult = TRUE;
      }
      break;
      
    case PN532_CMD_SAMCONFIGURATION :
      // read the data
      ReadData( anRecvBuffer, PN532_PKT_SIZE_SAMCONFIG );
      
      // check for valid config
      if ( anRcvBuffer[ 6 ] = 0x15 )
      {
        // good config
        bResult = TRUE;
      }
      break;
      
    case PN532_CMD_LISTPASSIVETARGET :
      // modify the transmit buffer to go next
      anXmitBuffer[ 0 ] = PN532_CMD_LISTPASSIVETARGET1;
      
      // restart a timeout
      NfcPN532Handler_PostEvent( NFCPN532_EVENT_WAITTIME );
      
      // clear the result
      bResult = FALSE;
      break;

    case PN532_CMD_LISTPASSIVETARGET1 :
      // read the data
      ReadData( anRecvBuffer, PN532_PKT_SIZE_LISTUID );
      
      // now extract the length
      nLastUIDLength = anRcvBuffer[ 12 ];
      
      // now extract the UID
      memcpy( anLastUID, &anRcvBuffer[ 13 ], nLastUIDLength );
      
      // set good result
      bResult = TRUE;
      break;
      
    default :
      break;
  }
  
  // return the result
  return( bResult );
}


/******************************************************************************
 * @function 
 *
 * @brief 
 *
 * This function 
 *
 * @param[in]   
 *
 * @return      
 *
 *****************************************************************************/
/**@} EOF NfcPN532Handler.c */
