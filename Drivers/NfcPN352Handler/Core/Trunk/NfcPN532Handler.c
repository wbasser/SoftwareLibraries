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
#include "NfcPN532Handler/NfcPN532Handler_prm.h"

// library includes -----------------------------------------------------------
#include "I2C/I2c.h"

// Macros and Defines ---------------------------------------------------------
/// define the callback events
#define NFCPN532_EVENT_MSGRCVD                        ( 0xFC0E )
#define NFCPN532_EVENT_MSGXMIT                        ( 0xFC55 )

/// define the maximum transmit length
#define MAX_XMIT_LENGTH                               ( 128 )

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
static  U8              nXmitLength;
static  U16             wRcvTimeout;
static  COMMSTATE       eCommState;
static  PVMSGCALLBACK   pvMsgCallback;

// local function prototypes --------------------------------------------------
static  void  ReadData( PU8 pnBuffer, U8 nLength );
static  void  WriteCommand( PU8 pnBuffer, U8 nLength );
static  void  StuffBuffer( PU8 pnBuffer, U8 nData, BOOL bAddToChecksum );

// constant parameter initializations -----------------------------------------
static  const CODE  U8  nAckPacket[ ] = { 0x01, 0x00, 0x00, 0xFF, 0x00, 0xFF, 0x00 };
#define ACK_PACKET_SIZE                 ( sizeof( nAckPacket ))

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

  // initialize


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

  // send the command
  WriteCommand( &anXmitBuffer, nXmitLength );
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
 
/******************************************************************************
 * @function NfcPN532Handler_CommunicationsTask
 *
 * @brief communications task
 *
 * This function process the events from the communications task
 *
 * @param[in]   xArg      task argument
 *
 * @return      TRUE      flush event
 *
 *****************************************************************************/
BOOL NfcPN532Handler_CommunicationsTask( TASKARG xArg )
{
  BOOL  bMsgStatus;

  // process the comm state
  switch( eCommState )
  {
    case COMM_STATE_IDLE :
      // only process the transmit events
      if ( xArg == NFCPN532_EVENT_MSGXMIT )
      {
        // send the message
        WriteCommand( anXmitBuffer, nXmitLength );

        // start the timeout 
        TaskManager_StartTimer( NFCPN532_TASK_ENUM, TASK_TIME_MSECS( wRcvTimeout ));

        // go to wait
        eCommState = COMM_STATE_WAIT;
      }
      break;

    case COMM_STATE_WAIT :
      switch( xArg )
      {
        case TASK_TIMEOUT_EVENT :
          bMsgStatus = FALSE;
          break;

        case NFCPN532_EVENT_MSGRCVD :
          bMsgStatus = TRUE;
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
  TaskManager_PostEvent( NFCPN532_TASK_ENUM, NFCPN532_EVENT_MSGRCVD );
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
  U8    nAckBuffer [ ACK_PACKET_SIZE ];
  BOOL  bStatus = FALSE;

  // read the packet
  ReadData( nAckBuffer, ACK_PACKET_SIZE );

  // test for valid packet
  if ( STRNCMP_P( nAckBuffer, nAckPacket, ACK_PACKET_SIZE ) == 0 )
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
  I2CXFRCTL tXfrCtl;

  // fill the control structure
  tXfrCtl.nDevAddr = PN532_DEVICE_ADDR;
  tXfrCtl.nAddrLen = 0;
  tXfrCtl.tAddress.uValue = 0;
  tXfrCtl.pnData = pnBuffer;
  tXfrCtl.wDataLen = nLength;
  tXfrCtl.uTimeout = 100;

  // now read it
  I2c_Read( NFCPN532_I2C_DEV_ENUM, &tXfrCtl );
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
  I2CXFRCTL tXfrCtl;

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

    // now fill the transfer control structure
    tXfrCtl.nDevAddr = PN532_DEVICE_ADDR;
    tXfrCtl.nAddrLen = 0;
    tXfrCtl.tAddress.uValue = 0;
    tXfrCtl.pnData = pnXmitBuffer;
    tXfrCtl.wDataLen = nXmitBufIndex;
    tXfrCtl.uTimeout = 100;

    // now send it
    I2c_Write( NFCPN532_I2C_DEV_ENUM, &tXfrCtl );

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

/**@} EOF NfcPN532Handler.c */
