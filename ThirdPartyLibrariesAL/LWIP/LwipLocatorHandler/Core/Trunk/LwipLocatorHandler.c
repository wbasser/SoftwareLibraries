/******************************************************************************
 * @file LwipLocatorHandler.c
 *
 * @brief Lwip Localtor handler implementation
 *
 * This file will implement a UDP socket on the designated port and listed for
 * a broadcast request for info and will respond with the program's information
 *
 * @copyright Copyright (c) 2012 Cyber Intergration
 * This document contains proprietary data and information of Endurance Products 
 * LLC. It is the exclusive property of Endurance Products, LLC and will not be 
 * disclosed in any form to any party without prior written permission of 
 * Endurance Products, LLC. This document may not be reproduced or further used 
 * without the prior written permission of Endurance Products, LLC.
 *
 * Version History
 * ======
 * $Log: $
 * 
 *
 * \addtogroup LwipLocatorHanler
 * @{
 *****************************************************************************/

// system includes ------------------------------------------------------------
#include "string.h"

// library includes -----------------------------------------------------------
#include "CRC32/CRC32.h"
#include "ConfigManager/ConfigManager.h"
#include "ManufInfo/ManufInfo.h"
#include "lwip/api.h"
#include "lwip/ip_addr.h"
#include "lwip/opt.h"
#include "lwip/mem.h"
#include "lwip/tcp.h"
#include "lwip/udp.h"

// local includes -------------------------------------------------------------
#include "LwipLocatorHandler/LwipLocatorHandler.h"
#include "LwipLocatorHandler/LwipLocatorHandler_cfg.h"
#include "LwipLocatorHandler/LwipLocatorHandler_prm.h"

// Macros and Defines ---------------------------------------------------------
/// define the MAC address size
#define MAC_ADDR_SIZE   ( 6 )

/// define the application title size
#define APP_TITLE_SIZE  ( 32 )

/// define the locator command
#define LOCATOR_CMD     ( 0x5A )

/// define the locator response
#define LOCATOR_RSP     ( 0xA5 )

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------
typedef union PACKED _LOCATORXMTPKT
{
  struct PACKED
  {
    U8    nResponse;
    U32UN tCliendAddress;
    U8    anMacAddress[ MAC_ADDR_SIZE ];
    U8    nHdwMajor;
    U8    nHdwMinor;
    U8    nSfwMajor;
    U8    nSfwMinor;
    C8    acName[ LOC_NAM_LEN ];
    C8    acTitle[ APP_TITLE_SIZE ];
    U8    nCheckSum;
  } tFields;
  U8  anBytes[ 1 ];
} LOCATORXMTPKT, *PLOCATORXMTPKT;
#define LOCATORXMTPKT_SIZE   sizeof( LOCATORXMTPKT )

typedef union PACKED _LOCATORRCVPKT
{
  struct PACKED
  {
    U8    nCommand;
    U32   uKey;
    U8    nCheckSum;
  } tFields;
  U8  anBytes[ 1 ];
} LOCATORRCVPKT, *PLOCATORRCVPKT;
#define LOCATORRCVPKT_SIZE  sizeof( LOCATORRCVPKT )

// global parameter declarations ----------------------------------------------

// local parameter declarations -----------------------------------------------
static  LOCATORXMTPKT   tLocatorPkt;
static  LWIPLOCATORDEF  tLocatorDef;
static  U32             uLocatorKey;

// local function prototypes --------------------------------------------------
static  void  LocatorReceive( PVOID pvArg, struct udp_pcb *ptPcb, struct pbuf *ptBuf, struct ip_addr *ptAddr, U16 wPport );

// constant parameter initializations -----------------------------------------

/******************************************************************************
 * @function LwipLocatorHandler_Initialize
 *
 * @brief initialization
 *
 * This function will initialize the locator packet
 *
 *****************************************************************************/
void LwipLocatorHandler_Initialize( void )
{
  PVOID pvPcb;

  // clear the packet
  memset( &tLocatorPkt, 0, LOCATORXMTPKT_SIZE );

  // compute the local hash value
  LwipLocatorHandler_UpdateConfiguration( );

  // set the response
  tLocatorPkt.tFields.nResponse = LOCATOR_RSP;

  // fill the version
  tLocatorPkt.tFields.nHdwMajor = ManufInfo_GetHdwMajor( );
  tLocatorPkt.tFields.nHdwMinor = ManufInfo_GetHdwMinor( );
  tLocatorPkt.tFields.nSfwMajor = ManufInfo_GetSfwMajor( );
  tLocatorPkt.tFields.nSfwMinor = ManufInfo_GetSfwMinor( );

  // fill the mac address
  LwipLocatorHandler_GetMacAddr( &tLocatorPkt.tFields.anMacAddress[ 0 ] );

   // create a listening UDB port
  pvPcb = udp_new( );
  udp_recv( pvPcb, LocatorReceive, NULL );
  udp_bind( pvPcb, IP_ADDR_ANY, tLocatorDef.wPort );
}

/******************************************************************************
 * @function LwipLocatorHandler_GetConfigDflt
 *
 * @brief get the configuration defaults
 *
 * This function will return a pointer to the default configuration
 *
 *****************************************************************************/
PLWIPLOCATORDEF LwipLocatorHandler_GetConfigDflt( void )
{
  // return the default table
  return(( PLWIPLOCATORDEF )&tLwipLocDefDflts );
}

/******************************************************************************
 * @function LwipLocatorHandler_GetConfigActl
 *
 * @brief get the current configuration 
 *
 * This function will return a pointer to the current configuration
 *
 *****************************************************************************/
PLWIPLOCATORDEF LwipLocatorHandler_GetConfigActl( void )
{
  // return the default table
  return( &tLocatorDef );
}

/******************************************************************************
 * @function LwipLocatorHandler_SetConfig
 *
 * @brief update the current config
 *
 * This function will update the config
 *
 *****************************************************************************/
void LwipLocatorHandler_SetConfig( PLWIPLOCATORDEF ptLwipDef )
{
  // update config
  LwipLocatorHandler_UpdateConfiguration( );

  #if ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_TASKMANAGER )
  // update the config in memory
  TaskManager_PostEvent( LWIPLOCATORHANDLER_CONFIGUPDATE_TASK, LWIPLOCATORHANDLER_CONFIG_ENUM );
  #elif ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_FREERTOS )
  U8  nValue = CONFIG_TYPE_LWIPLOC;
  xQueueSend( xConfigUpdateQueue, &nValue, portMAX_DELAY );
  #endif // SYSTEMDEFINE_OS_SELECTION
}

/******************************************************************************
 * @function LwipLocatorHandler_ClientIPSet
 *
 * @brief set the client IP address
 *
 * This function will set the IP of the current device
 *
 * @param[in]   tIpAddr = address of the IP
 *
 *****************************************************************************/
void LwipLocatorHandler_ClientIPSet( U32UN tIpAddr )
{
  // store the IP address
  tLocatorPkt.tFields.tCliendAddress.uValue = tIpAddr.uValue;
}

/******************************************************************************
 * @function LwipLocatorHandler_UpdateConfiguration
 *
 * @brief update configuration
 *
 * This function will cause an update of the configuration data
 *
 *****************************************************************************/
void LwipLocatorHandler_UpdateConfiguration( void )
{
  U8  nIdx, nChar;

  // compute the local hash value
  uLocatorKey = CRC32_GetInitialValue( );
  nIdx = 0;
  while(( nChar = tLocatorDef.acKey[ nIdx++ ]) != '\0' )
  {
    uLocatorKey = CRC32_CalculateByte( uLocatorKey, nChar );
  }
  
  // update the device's name
  memcpy( tLocatorPkt.tFields.acName,  tLocatorDef.acNameLoc, MIN( LOC_NAM_LEN, strlen( tLocatorDef.acNameLoc )));
}

/******************************************************************************
 * @function LocatorReceive
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
static void LocatorReceive( PVOID pvArg, struct udp_pcb *pvPcb, struct pbuf *ptBuf, struct ip_addr *ptAddr, U16 wPort )
{
  U8              nIdx, nChecksum;
  PLOCATORRCVPKT  ptRcvPkt;
  struct pbuf*    ptXmtBuf;

  // point to the payload
  ptRcvPkt = ptBuf->payload;

  // compute the checksum
  if ( ptBuf->len == LOCATORRCVPKT_SIZE )
  {
    // check the checksum
    nChecksum = 0;
    for ( nIdx = 0; nIdx < LOCATORRCVPKT_SIZE; nIdx++ )
    {
      // compute the checksum
      nChecksum -= ptRcvPkt->anBytes[ nIdx ];
    }

    // if checksum is zero
    if ( nChecksum == 0 )
    {
      // compare the strings
      if (( ptRcvPkt->tFields.uKey == uLocatorKey ) && ( ptRcvPkt->tFields.nCommand == LOCATOR_CMD ))
      {
        // free the receive buffer
        pbuf_free( ptBuf );
  
        // Allocate a new pbuf for sending the response.
        if (( ptXmtBuf = pbuf_alloc( PBUF_TRANSPORT, LOCATORXMTPKT_SIZE, PBUF_RAM )) != NULL )
        {
          // Calcuate and fill in the checksum on the response packet.
          tLocatorPkt.tFields.nCheckSum = 0;
          for( nIdx = 0; nIdx < LOCATORXMTPKT_SIZE - 1; nIdx++ )
          {
            // subtract each value to calulate 2's complement checksum
            tLocatorPkt.tFields.nCheckSum -= tLocatorPkt.anBytes[ nIdx ];
          }

          // Copy the response packet data into the pbuf.
          memcpy( ptXmtBuf->payload, &tLocatorPkt, LOCATORXMTPKT_SIZE );

          // Send the response.
          udp_sendto( pvPcb, ptXmtBuf, ptAddr, wPort );

          // free the buffer
          pbuf_free( ptXmtBuf );
        }
      }
    }
  }

  // The incoming pbuf is no longer needed, so free it.
  pbuf_free( ptBuf );
}

/**@} EOF LwipLocatorHandler.c */
