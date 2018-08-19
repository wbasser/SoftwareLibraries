/******************************************************************************
 * @file LwipNtpHandler.c
 *
 * @brief LWIP Network Time Protocol( NTP ) handler implementation 
 *
 * This file provides the implementation for the NTP protocol handler
 *
 * @copyright Copyright (c) 2012 Nan Technologies
 * This document contains proprietary data and information of Nan Technologies 
 * LLC. It is the exclusive property of Nan Technologies, LLC and will not be 
 * disclosed in any form to any party without prior written permission of 
 * Nan Technologies, LLC. This document may not be reproduced or further used 
 * without the prior written permission of Nan Technologies, LLC.
 *
 * $Date: $
 *
 * Version History
 * ======
 * $Rev: $
 * 
 *
 * \addtogroup LwipNtpHandler
 * @{
 *****************************************************************************/

// system includes ------------------------------------------------------------

// library includes -----------------------------------------------------------
#include "ConfigManager/ConfigManager.h"

// local includes -------------------------------------------------------------
#include "LwipNtpHandler/LwipNtpHandler.h"
#include "LwipNtpHandler/LwipNtpHandler_cfg.h"

// Macros and Defines ---------------------------------------------------------
/// define the NTP port
#define NTP_PORT              ( 8888 )

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------
/// define the NTP structure
typedef struct
{
  union
  {
    struct
    {
      U8  nLeapIndicator  : 2;
      U8  nVersion        : 3;
      U8  nMode           : 5;
    } tFields;
    U8  nByte;
  } tMode;
  U8    nStratum;
  U8    nPollRate;
  U8    nPrecision;
  U32   uRootDelay;
  U32   uRootDispersion;
  U32UN tReferenceIdentifier;
  U64UN tReferenceTimeStamp;
  U64UN tOriginateTimeStamp;
  U64UN tReceiveTimeStamp;
  U64UN tTransmitTimeStamp;
} NTPPACKET, *PNTPACKET;
#define NTPPACKET_SIZE        sizeof( NTPPACKET )

// global parameter declarations ----------------------------------------------

// local parameter declarations -----------------------------------------------
static  U32UN   tNetTime;
static  PVOID   pvPcb;

// local function prototypes --------------------------------------------------
static  void  SendNtpRequest( void );
static  void  NtpReceiveCallback( PVOID pvArg, struct udp_pcb *ptPcb, struct pbuf *ptBuf, struct ip_addr *ptAddr, U16 wPport );

// constant parameter initializations -----------------------------------------
const LWIPNTPDEF  tLwipNtpDefDflts =
{
  .tIpAddr =
  {
    .anValue[ BE_U32_MSB_IDX ] = 129,
    .anValue[ BE_U32_MS2_IDX ] = 6,
    .anValue[ BE_U32_MS1_IDX ] = 15,
    .anValue[ BE_U32_LSB_IDX ] = 28
  }
};

/******************************************************************************
 * @function LwipNtpHandler_Initialize
 *
 * @brief initializion
 *
 * This function will get the configuration and create a UDP socket
 *
 *****************************************************************************/
void LwipNtpHandler_Initialize( void )
{

  // create a listening UDB port
  pvPcb = udp_new( );
  udp_recv( pvPcb, NtpReceiveCallback, NULL );
  udp_bind( pvPcb, &tLwipNtpDefDflts.tIpAddr, NTP_PORT );
}

/******************************************************************************
 * @function LwipNtpHandler_UpdateConfiguration
 *
 * @brief update configuration
 *
 * This function will get the latest configuration information
 *
 *****************************************************************************/
void LwipNtpHandler_UpdateConfiguration( void )
{
}

/******************************************************************************
 * @function LwipNtpHandler_GetNetworkTime
 *
 * @brief get the network time
 *
 * This function will force a get network time sequence
 *
 *****************************************************************************/
void LwipNtpHandler_GetNetworkTime( void )
{
  // post an event to cause a request to be sent
  TaskManager_PostEvent( LWIPNTPHANDLER_TASK_ENUM, 0 );
}

/******************************************************************************
 * @function LwipNptHandler_ProcessEvent
 *
 * @brief event handler
 *
 * This function will process the event 
 *
 * @param[in]   xArg      task argument
 *
 * @return      TRUE      flush event
 *
 *****************************************************************************/
BOOL LwipNptHandler_ProcessEvent( TASKARG xArg )
{
  // send the NTP request
  SendNtpRequest( );
  
  // return true
  return( TRUE );
}

/******************************************************************************
 * @function SendNtpRequest
 *
 * @brief send an NTP request
 *
 * This function will fill a request structure and send it out to the
 * designated address and port
 *
 *****************************************************************************/
static void SendNtpRequest( void )
{
  NTPPACKET     tReqPkt;
  struct pbuf*  ptXmtBuf;
  
  // fill the packet
  memset( &tReqPkt, 0, NTPPACKET_SIZE );
  tReqPkt.tMode.nByte = 0b11100011;
  tReqPkt.nPollRate = 6;
  tReqPkt.nPrecision = 0xEC;
  tReqPkt.tReferenceIdentifier.anValue[ LE_U32_LSB_IDX ] = 49;
  tReqPkt.tReferenceIdentifier.anValue[ LE_U32_MS1_IDX ] = 0x4E;
  tReqPkt.tReferenceIdentifier.anValue[ LE_U32_MS2_IDX ] = 49;
  tReqPkt.tReferenceIdentifier.anValue[ LE_U32_MSB_IDX ] = 52;
  
  // Allocate a new pbuf for sending the response.
  if (( ptXmtBuf = pbuf_alloc( PBUF_TRANSPORT, NTPPACKET_SIZE, PBUF_RAM )) != NULL )
  {
    // Copy the response packet data into the pbuf.
    memcpy( ptXmtBuf->payload, &tReqPkt, NTPPACKET_SIZE );

    // Send the response.
    udp_sendto( pvPcb, ptXmtBuf, tLwipNtpDefDflts.tIpAddr.uValue, NTP_PORT );

    // free the buffer
    pbuf_free( ptXmtBuf );
  }
}

/******************************************************************************
 * @function NtpReceiveCallback
 *
 * @brief UDP recieve callback 
 *
 * This function process's the UDP receive callback
 *
 * @param[in]   pvArg     pointer to an argument
 * @param[in]   ptPcb     pointer UDP control structure
 * @param[in]   ptBuf     pointer UDP recieve buffer
 * @param[in]   ptAddr    pointer to the address
 * @param[in]   wPort     recieved port
 *
 *****************************************************************************/
static void NtpReceiveCallback( PVOID pvArg, struct udp_pcb *ptPcb, struct pbuf *ptBuf, struct ip_addr *ptAddr, U16 wPport )
{
  PNTPACKET ptRcvPkt;

  // point to the payload
  ptRcvPkt = ( PNTPACKET )ptBuf->payload;
  
  // get the time
  tNetTime.uValue = ptRcvPkt->tTransmitTimeStamp.auValue[ LE_U64_LSU_IDX ];

  // The incoming pbuf is no longer needed, so free it.
  pbuf_free( ptBuf );

  // now report the new time
  LwipNtpHandler_ReportNetTime( tNetTime.uValue );
}

/**@} EOF LwipNtpHandler.c */
