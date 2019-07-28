/******************************************************************************
 * @file WINC1500Handler.c
 *
 * @brief WINC1500 handler implementation
 *
 * This file provides the implementation for the WINC1500 handler
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
 * \addtogroup WINC1500Handler
 * @{
 *****************************************************************************/

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "WINC1500Handler/WINC1500Handler.H"

// library includes -----------------------------------------------------------
#include "common/include/nm_common.h"
#include "driver/include/m2m_wifi.h"
#include "socket/include/socket.h"

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations ----------------------------------------------

// local parameter declarations -----------------------------------------------
static  BOOL  bConnected;
static  U32UN tIpAddress;

// local function prototypes --------------------------------------------------
static  void  WifiCallback( U8 nMsgType, PVOID pvMsg );
static  void  SocketCallback( SOCKET tSock, U8 nMsg, PVOID pvMsg );

// constant parameter initializations -----------------------------------------
static  const C8  pszSSID[ ] = { "CyberDevEL" };
static  const C8  pszAUTH[ ] = { "Cyber1209Snarf" };

/******************************************************************************
 * @function WINC1500Handler_Initialize
 *
 * @brief initialization
 *
 * This function will perform any neede configuration
 *
 *****************************************************************************/
void WINC1500Handler_Initialize( void )
{
  tstrWifiInitParam   tParam;
  struct sockaddr_in  tAddr;

  // clear the connected flag
  bConnected = FALSE;

  /// initialize the bsp
  nm_bsp_init();

  // clear the parameters
  memset( &tParam, 0, sizeof ( tstrWifiInitParam ));

  // initialize the parameters
  tParam.pfAppWifiCb = WifiCallback;

  // initialize the WIFI
  if ( m2m_wifi_init( &tParam ) == M2M_SUCCESS )
  {
    // initialize socket
    socketInit();
    registerSocketCallback( SocketCallback, NULL );

    // connect to the router
    m2m_wifi_connect((const PC8 )pszSSID, sizeof( pszSSID ), M2M_WIFI_SEC_WPA_PSK, ( PVOID )pszAUTH, M2M_WIFI_CH_ALL);
  }
}

/******************************************************************************
 * @function   WifiCallback
 *
 * @brief callback for the wifi status
 *
 * This function will process the callback for all WIFI related events
 *
 * @param[in]   nMsgType    message type
 * @param[in]   pvMsg       pointer to the message
 *
 *****************************************************************************/
static void WifiCallback( U8 nMsgType, PVOID pvMsg )
{
  tstrM2mWifiStateChanged* pstrWifiState;

  // determine the type
  switch ( nMsgType )
  {
    case M2M_WIFI_RESP_CON_STATE_CHANGED :
      // cast the message
      pstrWifiState = ( tstrM2mWifiStateChanged * )pvMsg;

      // detemine state
      if ( pstrWifiState->u8CurrState == M2M_WIFI_CONNECTED )
      {
        // request the DHCP client
        m2m_wifi_request_dhcp_client( );
      } 
      else if ( pstrWifiState->u8CurrState == M2M_WIFI_DISCONNECTED )
      {
        // clear the connection state
        bConnected = FALSE;
        
        // re-connect to the router
        m2m_wifi_connect((const PC8 )pszSSID, sizeof( pszSSID ), M2M_WIFI_SEC_WPA_PSK, ( PVOID )pszAUTH, M2M_WIFI_CH_ALL);
      }
      break;
  
    case M2M_WIFI_REQ_DHCP_CONF:
      // copy the address/set the connected state
      memcpy( &tIpAddress, pvMsg, sizeof( U32UN ));
      bConnected = TRUE;
      break;

    default:
      break;
  }
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
static void SocketCallback( SOCKET tSock, U8 nMsg, PVOID pvMsg )
{
}

/**@} EOF WINC1500Handler.c */
