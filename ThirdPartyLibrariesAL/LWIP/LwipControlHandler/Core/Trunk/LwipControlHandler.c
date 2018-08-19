/******************************************************************************
 * @file LwipControlHandler.c
 *
 * @brief LWIP wrapper 
 *
 * This file provides the wrapper for the LWIP ethernet stack
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
 * \addtogroup LwipControlHandler
 * @{
 *****************************************************************************/

// system includes ------------------------------------------------------------
#include "Types/Types.h"

// local includes -------------------------------------------------------------
#include "LwipControlHandler/LwipControlHandler.h"

// library includes -----------------------------------------------------------
#include "lwip/api.h"
#include "lwip/dhcp.h"
#include "lwip/dns.h"
#include "lwip/init.h"
#include "lwip/ip_frag.h"
#include "lwip/stats.h"
#include "lwip/tcp.h"
#include "lwip/tcpip.h"
#include "lwip/tcp_impl.h"
#include "lwip/timers.h"
#include "ipv4/lwip/autoip.h"
#include "ipv4/lwip/inet.h"
#include "ipv4/lwip/ip.h"
#include "netif/etharp.h"
#include "ConfigManager/ConfigManager.h"

// Macros and Defines ---------------------------------------------------------
#undef LWIP_DHCP_AUTOIP_COOP
#define LWIP_DHCP_AUTOIP_COOP   ((LWIP_DHCP) && (LWIP_AUTOIP))

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------
/// define the timer structure
typedef struct _TIMDEF
{
  U32   uCurTime;
  U32   uTimeInterval;
  void  ( *pvTimerFunction )( void );
} TIMDEF, *PTIMDEF;
#define TIMEDEF_SIZE        sizeof ( TIMDEF )

// global parameter declarations ----------------------------------------------

// local parameter declarations -----------------------------------------------
static  LWIPCFGDEF    tLwipCfgActl;
static  struct netif  tNetIf;
static  U32           uLocalTimer;
static  TIMDEF        atTimers[ ] =
{
  { 0, TCP_TMR_INTERVAL,        tcp_tmr         },
  { 0, IP_TMR_INTERVAL,         ip_reass_tmr    },
  { 0, ARP_TMR_INTERVAL,        etharp_tmr      },
  #if LWIP_DHCP
  { 0, DHCP_COARSE_TIMER_MSECS, dhcp_coarse_tmr },
  { 0, DHCP_FINE_TIMER_MSECS,   dhcp_fine_tmr   },
  #endif
};
#if ( LWIPCONTROLHANDLER_ENABLE_DEBUGCMDS == 1 )
static  PC8             pcBuffer;
#endif // LWIPCONTROLHANDLER_ENABLE_DEBUGCMDS

// local function prototypes --------------------------------------------------
static  void  ProcessStatusCallback( struct netif * ptNetIf );

/// command handlers
#if ( LWIPCONTROLHANDLER_ENABLE_DEBUGCMDS == 1 )
static  ASCCMDSTS CmdQryNet( U8 nCmdEnum );
static  ASCCMDSTS CmdSetMac( U8 nCmdEnum );
static  ASCCMDSTS CmdQryMac( U8 nCmdEnum );
static  ASCCMDSTS CmdSetIpa( U8 nCmdEnum );
static  ASCCMDSTS CmdSetMsk( U8 nCmdEnum );
static  ASCCMDSTS CmdSetGwy( U8 nCmdEnum );
static  ASCCMDSTS CmdSetNtm( U8 nCmdEnum );
static  ASCCMDSTS CmdSetNet( U8 nCmdEnum );
#endif // LWIPCONTROLHANDLER_ENABLE_DEBUGCMDS

// constant parameter initializations -----------------------------------------

#if ( LWIPCONTROLHANDLER_ENABLE_DEBUGCMDS == 1 )
/// command strings
static  const CODE C8 szQryNet[ ]   = { "QNET" };
static  const CODE C8 szSetMac[ ]   = { "SMAC" };
static  const CODE C8 szQryMac[ ]   = { "QMAC" };
static  const CODE C8 szSetIpa[ ]   = { "SIPA" };
static  const CODE C8 szSetMsk[ ]   = { "SMSK" };
static  const CODE C8 szSetGwy[ ]   = { "SGWY" };
static  const CODE C8 szSetNtm[ ]   = { "SNTM" };
static  const CODE C8 szSetNet[ ]   = { "SNET" };
#endif // LWIPCONTROLHANDLER_ENABLE_DEBUGCMDS

#if ( LWIPCONTROLHANDLER_ENABLE_DEBUGCMDS == 1 )
/// initialize the command table
const CODE ASCCMDENTRY atLwipHandlerCmdTable[ ] =
{
  ASCCMD_ENTRY( szQryNet, 4, 1, ASCFLAG_COMPARE_NONE, 0,                               CmdQryNet ),
  ASCCMD_ENTRY( szSetMac, 4, 2, ASCFLAG_COMPARE_EQ,   SYSCTRLMNGR_LCLMODE_DIAGNOSTICS, CmdSetMac ),
  ASCCMD_ENTRY( szQryMac, 4, 0, ASCFLAG_COMPARE_NONE, 0,                               CmdQryMac ),
  ASCCMD_ENTRY( szSetIpa, 4, 4, ASCFLAG_COMPARE_EQ,   SYSCTRLMNGR_LCLMODE_DIAGNOSTICS, CmdSetIpa ),
  ASCCMD_ENTRY( szSetMsk, 4, 4, ASCFLAG_COMPARE_EQ,   SYSCTRLMNGR_LCLMODE_DIAGNOSTICS, CmdSetMsk ),
  ASCCMD_ENTRY( szSetGwy, 4, 4, ASCFLAG_COMPARE_EQ,   SYSCTRLMNGR_LCLMODE_DIAGNOSTICS, CmdSetGwy ),
  ASCCMD_ENTRY( szSetNtm, 4, 1, ASCFLAG_COMPARE_EQ,   SYSCTRLMNGR_LCLMODE_DIAGNOSTICS, CmdSetNtm ),
  ASCCMD_ENTRY( szSetNet, 4, 0, ASCFLAG_COMPARE_EQ,   SYSCTRLMNGR_LCLMODE_DIAGNOSTICS, CmdSetNet ),

  // the entry below must be here
  ASCCMD_ENDTBL( )
};

/// define the response strings
static  const CODE C8 szRspNet[ ]   = { "RIPA, Mode: %d\n\r      Addr: %3d.%3d.%3d.%3d\n\r      Mask: %3d.%3d.%3d.%3d\n\r      Gway: %3d.%3d.%3d.%3d\n\r" };
static  const CODE C8 szRspMac[ ]   = { "RMAC, %02X:%02X:%02X:%02X:%02X:%02X\n\r" };

/// define the option strings
static  const CODE C8 szNetLcl[ ]   = { "LCL" };
static  const CODE C8 szNetNvm[ ]   = { "NVM" };
static  const CODE C8 szNetDfl[ ]   = { "DFL" };
#endif // LWIPCONTROLHANDLER_ENABLE_DEBUGCMDS

/******************************************************************************
 * @function LwipControlHandler_Initialize
 *
 * @brief stellaris LWIP handler initialization
 *
 * This function initializes the stellaris LWIP implementation.  It will also
 * initialize any protocols that are identified in the configuration
 *
 *****************************************************************************/
void LwipControlHandler_Initialize( void )
{
  #if ( LWIPCONTROLHANDLER_ENABLE_DEBUGCMDS == 1 )
  pcBuffer = NULL;
  #endif // LWIPCONTROLHANDLER_ENABLE_DEBUGCMDS

  // for debug only
  memcpy( &tLwipCfgActl, &tLwipCfgDflts, LWIPCFGDEF_SIZE );

  // call the local initialize
//  LwipControlHandler_LocalInitialize( );
}

/******************************************************************************
 * @function LwipControlHandler_GetConfigDflt
 *
 * @brief get the default config
 *
 * This function returns a pointer to the default config
 *
 * @return      pointer to the default config
 *
 *****************************************************************************/
PLWIPCFGDEF LwipControlHandler_GetConfigDflt( void )
{
  // return a pointer to the default
  return(( PLWIPCFGDEF )&tLwipCfgDflts );
}

/******************************************************************************
 * @function LwipControlHandler_GetConfigActl
 *
 * @brief get the actual config
 *
 * This function returns a pointer to the actual config
 *
 * @return      pointer to the actual config
 *
 *****************************************************************************/
PLWIPCFGDEF LwipControlHandler_GetConfigActl( void )
{
  // return a pointer to the actual
  return( &tLwipCfgActl );
}

/******************************************************************************
 * @function LwipControlHandler_Congifure
 *
 * @brief stellaris LWIP handler initialization
 *
 * This function initializes the stellaris LWIP implementation.  It will also
 * initialize any protocols that are identified in the configuration
 *
 *****************************************************************************/
void LwipControlHandler_Configure( void )
{
  struct ip_addr  tIpAddr;
  struct ip_addr  tNetMask;
  struct ip_addr  tGateway;
  PLWIPPROTDEF    ptDef;
  LWIPPROTS       eProt;

  // initialize the lwip stack
  lwip_init( );

  // set the MAC address/length
  tNetIf.hwaddr[ 0 ] = tLwipCfgActl.anMac[ 0 ];
  tNetIf.hwaddr[ 1 ] = tLwipCfgActl.anMac[ 1 ];
  tNetIf.hwaddr[ 2 ] = tLwipCfgActl.anMac[ 2 ];
  tNetIf.hwaddr[ 3 ] = tLwipCfgActl.anMac[ 3 ];
  tNetIf.hwaddr[ 4 ] = tLwipCfgActl.anMac[ 4 ];
  tNetIf.hwaddr[ 5 ] = tLwipCfgActl.anMac[ 5 ];
  tNetIf.hwaddr_len = LWIP_MAC_LENGTH;

  // check for DHCP
  if ( tLwipCfgActl.eMode == LWIP_CONFIG_MODE_DHCP )
  {
    // clear the address
    tIpAddr.addr = 0;
    tNetMask.addr = 0;
    tGateway.addr = 0;
  }
  else
  {
    tIpAddr.addr = htonl( tLwipCfgActl.tAddr.uValue );
    tNetMask.addr = htonl( tLwipCfgActl.tMask.uValue );
    tGateway.addr = htonl( tLwipCfgActl.tGate.uValue );
  }

  // add the ethernet controller to the stack
  netif_add( &tNetIf, &tIpAddr, &tNetMask, &tGateway, NULL, LwipControlHandler_PortInitialize, ethernet_input );
  netif_set_default( &tNetIf );
  
  // set the status callback
  netif_set_status_callback( &tNetIf, ProcessStatusCallback );

  // if DHCP enabled, start it
  if ( tLwipCfgActl.eMode == LWIP_CONFIG_MODE_DHCP )
  {
    // start it
    dhcp_start( &tNetIf );
  }
  else
  {
    // bring the interface up
    netif_set_up( &tNetIf );
   }
  
  // if auot IP enabled, start it
  #if LWIP_AUTOIP
  if ( tLwipCfgActl.eMode == LWIP_CONFIG_MODE_AUTOIP )
  {
    // start it
    autoip_start( &tNetIf );
  }
  #endif
  
  // for each item in the protocol list
  for ( eProt = 0; eProt < LWIP_PROT_MAX; eProt++ )
  {
    // get a pointer to the initialization function
    ptDef = ( PLWIPPROTDEF )&atLwipProtDefs[ eProt ];
    
    // call the initialization function
    if ( ptDef->pvInit != NULL )
    {
      // execute it
      ptDef->pvInit( );
    }
  }
}

/******************************************************************************
 * @function LwipControlHandler_SetConfig
 *
 * @brief sets the current configuration
 *
 * This function sets the current configuration, address, mask and gateway and
 * stores them in the pointers passed as arguments
 *
 * @param[io]   ptLwipCfgDef  pointer to the config structure
 *
 * @return      error
 *
 *****************************************************************************/
LWIPERRORS LwipControlHandler_SetConfig( PLWIPCFGDEF ptLwipCfgDef )
{
  // copy the config structure
  memcpy( &tLwipCfgActl, ptLwipCfgDef, LWIPCFGDEF_SIZE );

  #if ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_TASKMANAGER )
  // update the config in memory
  TaskManager_PostEvent( LWIPCONTROLHANDLER_CONFIGUPDATE_TASK, LWIPCONTROLHANDLER_CONFIG_ENUM );
  #elif ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_FREERTOS )
  U8  nValue = CONFIG_TYPE_LWIPCFG;
  xQueueSend( xConfigUpdateQueue, &nValue, portMAX_DELAY );
  #endif // SYSTEMDEFINE_OS_SELECTION

  // return 0
  return( 0 );
}

/******************************************************************************
 * @function LwipControlHandler_GetCurrentNetConfig
 *
 * @brief gets the current network config
 *
 * This function gets and returns the current network configuration
 *
 * @param[io]   ptLwipCfgDef  pointer to the config structure
 *
 *****************************************************************************/
void LwipControlHandler_GetCurrentNetConfig( PLWIPCFGDEF ptConfig )
{
  // now copy the current network interface parameters
  ptConfig->tAddr.uValue = ntohl( tNetIf.ip_addr.addr );
  ptConfig->tMask.uValue = ntohl( tNetIf.netmask.addr );
  ptConfig->tGate.uValue = ntohl( tNetIf.gw.addr );
}

/******************************************************************************
 * @function LwipControlHandler_GetMacAddr
 *
 * @brief gets the current mac address
 *
 * This function gets the current MAC address
 *
 * @param[io]   pnMacAddress   pointer to the address
 *
 *****************************************************************************/
void LwipControlHandler_GetMacAddr( PU8 pnMacAddress )
{
  // return the mac address
  memcpy( pnMacAddress, &tLwipCfgActl.anMac[ 0 ], LWIP_MAC_LENGTH );
}

/******************************************************************************
 * @function LwipControlHandler_SetMacAddr
 *
 * @brief gets the current mac address
 *
 * This function gets the current MAC address
 *
 * @param[io]   pnMacAddress   pointer to the address
 *
 *****************************************************************************/
void LwipControlHandler_SetMacAddr( PU8 pnMacAddress )
{
  // return the mac address
  memcpy( &tLwipCfgActl.anMac[ 0 ], pnMacAddress, LWIP_MAC_LENGTH );
}

/******************************************************************************
 * @function LwipControlHandler_ProcessTimer
 *
 * @brief process the timer task
 *
 * This function processes the LWIP timer 
 *
 * @param[in]   arg   task argument
 *
 * @return      TRUE always
 *
 *****************************************************************************/
void LwipControlHandler_ProcessTimer( void )
{
  PLWIPPROTDEF  ptDef;
  LWIPPROTS     eProt;
  U8            nTimerIdx;
  PTIMDEF       ptTimer;

  // update the local timer
  uLocalTimer += LWIPCTLHANDLER_EXEC_RATE / TASK_TIME_MSECS( 1 );
  
  // now process timer table
  for( nTimerIdx = 0; nTimerIdx < ( sizeof( atTimers ) / TIMEDEF_SIZE ); nTimerIdx++ )
  {
    // set the pointer to the timer
    ptTimer = &atTimers[ nTimerIdx ];
    
    // has time expired on this entry
    if (( uLocalTimer - ptTimer->uCurTime ) >= ptTimer->uTimeInterval )
    {
      // reset the timer interval
      ptTimer->uCurTime = uLocalTimer;
      
      // check for a null function
      if ( ptTimer->pvTimerFunction != NULL )
      {
        // execut it
        ptTimer->pvTimerFunction( );
      }
    }
  }
  
  // for each item in the protocol list
  for ( eProt = 0; eProt < LWIP_PROT_MAX; eProt++ )
  {
    // get a pointer to the process list function
    ptDef = ( PLWIPPROTDEF )&atLwipProtDefs[ eProt ];
    
    // call the process function
    if ( ptDef->pvProc != NULL )
    {
      // execute it
      ptDef->pvProc( );
    }
  }
}

/******************************************************************************
 * @function ProcessStatusCallback
 *
 * @brief process the status callback
 *
 * This function processes the status callback
 *
 * @param[in]   ptNetIf   pointer to th net interface
 *
 *****************************************************************************/
static void ProcessStatusCallback( struct netif* ptNetIf )
{
  LWIPCFGDEF  tLclConfig;

  // test for valid network
  if ( netif_is_up( ptNetIf ))
  {
    // set the net address
    tLclConfig.tAddr.uValue = ntohl( tNetIf.ip_addr.addr );
    tLclConfig.tMask.uValue = ntohl( tNetIf.netmask.addr );
    tLclConfig.tGate.uValue = ntohl( tNetIf.gw.addr );
  }

  // call the default handler
  LwipControlHandler_ProcessConnectCallback( ptNetIf );
}

#if ( LWIPCONTROLHANDLER_ENABLE_DEBUGCMDS == 1 )
/******************************************************************************
 * @function CmdQryNet
 *
 * @brief query the net parameters
 *
 * This function reports the current net parameters
 *
 * @return  A
 *****************************************************************************/
static ASCCMDSTS CmdQryNet( U8 nCmdEnum )
{
  PC8         pszOption;
  LWIPCFGDEF  tLclConfig;

  // test for an empty buffer
  if ( pcBuffer == NULL )
  {
    // fetch it 
    AsciiCommandHandler_GetBuffer( nCmdEnum, &pcBuffer );
  }

  // get the option
  AsciiCommandHandler_GetArg( nCmdEnum, 0, &pszOption );

  // test for local
  if ( strcmp( pszOption, szNetNvm ) == 0 )
  {
    // get the stored value
    memcpy( &tLclConfig, &tLwipCfgActl, LWIPCFGDEF_SIZE );
  }
  else if ( strcmp( pszOption, szNetDfl ) == 0 )
  {
    // get the actual value
    memcpy( &tLclConfig, &tLwipCfgDflts, LWIPCFGDEF_SIZE );
  }
  else
  {
    tLclConfig.tAddr.uValue = ntohl( tNetIf.ip_addr.addr );
    tLclConfig.tMask.uValue = ntohl( tNetIf.netmask.addr );
    tLclConfig.tGate.uValue = ntohl( tNetIf.gw.addr );
    tLclConfig.eMode = tLwipCfgActl.eMode;
  }

  // output the error
  sprintf(( char *)pcBuffer, ( char const * )szRspNet, 
           tLclConfig.eMode, 
           tLclConfig.tAddr.anValue[ LE_U32_MSB_IDX ],
           tLclConfig.tAddr.anValue[ LE_U32_MS2_IDX ],
           tLclConfig.tAddr.anValue[ LE_U32_MS1_IDX ],
           tLclConfig.tAddr.anValue[ LE_U32_LSB_IDX ],
           tLclConfig.tMask.anValue[ LE_U32_MSB_IDX ],
           tLclConfig.tMask.anValue[ LE_U32_MS2_IDX ],
           tLclConfig.tMask.anValue[ LE_U32_MS1_IDX ],
           tLclConfig.tMask.anValue[ LE_U32_LSB_IDX ],
           tLclConfig.tGate.anValue[ LE_U32_MSB_IDX ],
           tLclConfig.tGate.anValue[ LE_U32_MS2_IDX ],
           tLclConfig.tGate.anValue[ LE_U32_MS1_IDX ],
           tLclConfig.tGate.anValue[ LE_U32_LSB_IDX ]
         );

  // return the error
  return( ASCCMD_STS_OUTPUTBUFFER );
}

/******************************************************************************
 * @function CmdSetMac
 *
 * @brief set the MAC in USER 0/1
 *
 * This function sets the MAC address
 *
 * @return  
 *****************************************************************************/
static ASCCMDSTS CmdSetMac( U8 nCmdEnum )
{
  U32UN     tTemp;
  
  // get the base address, length
  AsciiCommandHandler_GetValue( nCmdEnum, 0, &tTemp.uValue );
  tLwipCfgActl.anMac[ 0 ] = tTemp.anValue[ LE_U32_LSB_IDX ];
  AsciiCommandHandler_GetValue( nCmdEnum, 1, &tTemp.uValue );
  tLwipCfgActl.anMac[ 1 ] = tTemp.anValue[ LE_U32_LSB_IDX ];
  AsciiCommandHandler_GetValue( nCmdEnum, 2, &tTemp.uValue );
  tLwipCfgActl.anMac[ 2 ] = tTemp.anValue[ LE_U32_LSB_IDX ];
  AsciiCommandHandler_GetValue( nCmdEnum, 3, &tTemp.uValue );
  tLwipCfgActl.anMac[ 3 ] = tTemp.anValue[ LE_U32_LSB_IDX ];
  AsciiCommandHandler_GetValue( nCmdEnum, 4, &tTemp.uValue );
  tLwipCfgActl.anMac[ 4 ] = tTemp.anValue[ LE_U32_LSB_IDX ];
  AsciiCommandHandler_GetValue( nCmdEnum, 5, &tTemp.uValue );
  tLwipCfgActl.anMac[ 5 ] = tTemp.anValue[ LE_U32_LSB_IDX ];

 // return the status
  return( ASCCMD_STS_NONE );
}

/******************************************************************************
 * @function CmdQryMac
 *
 * @brief queries the MAC address in 
 *
 * This function will return the mac address
 *
 * @return  
 *****************************************************************************/
static ASCCMDSTS CmdQryMac( U8 nCmdEnum )
{
  U32UN     tUser0;
  U32UN     tUser1;

  // test for an empty buffer
  if ( pcBuffer == NULL )
  {
    // fetch it 
    AsciiCommandHandler_GetBuffer( nCmdEnum, &pcBuffer );
  }

  // now output it
  sprintf( pcBuffer, szRspMac, tLwipCfgActl.anMac[ 0 ],
                               tLwipCfgActl.anMac[ 1 ],
                               tLwipCfgActl.anMac[ 2 ],
                               tLwipCfgActl.anMac[ 3 ],
                               tLwipCfgActl.anMac[ 4 ],
                               tLwipCfgActl.anMac[ 5 ] );

  // return the status
  return( ASCCMD_STS_OUTPUTBUFFER );
}

/******************************************************************************
 * @function CmdSetIpa
 *
 * @brief Set the IPA
 *
 * This function will set the IPA value in the local config structure
 *
 * @return  
 *****************************************************************************/
static ASCCMDSTS CmdSetIpa( U8 nCmdEnum )
{
  U32UN tTemp;

  // get the arguments
  AsciiCommandHandler_GetValue( nCmdEnum, 0, &tTemp.uValue );
  tLwipCfgActl.tAddr.anValue[ LE_U32_MSB_IDX ] = tTemp.anValue[ LE_U32_LSB_IDX ];
  AsciiCommandHandler_GetValue( nCmdEnum, 1, &tTemp.uValue );
  tLwipCfgActl.tAddr.anValue[ LE_U32_MS2_IDX ] = tTemp.anValue[ LE_U32_LSB_IDX ];
  AsciiCommandHandler_GetValue( nCmdEnum, 2, &tTemp.uValue );
  tLwipCfgActl.tAddr.anValue[ LE_U32_MS1_IDX ] = tTemp.anValue[ LE_U32_LSB_IDX ];
  AsciiCommandHandler_GetValue( nCmdEnum, 3, &tTemp.uValue );
  tLwipCfgActl.tAddr.anValue[ LE_U32_LSB_IDX ] = tTemp.anValue[ LE_U32_LSB_IDX ];

  // return no output
  return( ASCCMD_STS_NONE );
}

/******************************************************************************
 * @function CmdSetMsk
 *
 * @brief Set the mask value
 *
 * This function will set the mask value in the local config structure
 *
 * @return  
 *****************************************************************************/
static ASCCMDSTS CmdSetMsk( U8 nCmdEnum )
{
  U32UN tTemp;

  // get the arguments
  AsciiCommandHandler_GetValue( nCmdEnum, 0, &tTemp.uValue );
  tLwipCfgActl.tMask.anValue[ LE_U32_MSB_IDX ] = tTemp.anValue[ LE_U32_LSB_IDX ];
  AsciiCommandHandler_GetValue( nCmdEnum, 1, &tTemp.uValue );
  tLwipCfgActl.tMask.anValue[ LE_U32_MS2_IDX ] = tTemp.anValue[ LE_U32_LSB_IDX ];
  AsciiCommandHandler_GetValue( nCmdEnum, 2, &tTemp.uValue );
  tLwipCfgActl.tMask.anValue[ LE_U32_MS1_IDX ] = tTemp.anValue[ LE_U32_LSB_IDX ];
  AsciiCommandHandler_GetValue( nCmdEnum, 3, &tTemp.uValue );
  tLwipCfgActl.tMask.anValue[ LE_U32_LSB_IDX ] = tTemp.anValue[ LE_U32_LSB_IDX ];

  // return no output
  return( ASCCMD_STS_NONE );
}

/******************************************************************************
 * @function CmdSetGwy
 *
 * @brief Set the gateway
 *
 * This function will set the gateway in the local config structure
 *
 * @return  
 *****************************************************************************/
static ASCCMDSTS CmdSetGwy( U8 nCmdEnum )
{
  U32UN tTemp;

  // get the arguments
  AsciiCommandHandler_GetValue( nCmdEnum, 0, &tTemp.uValue );
  tLwipCfgActl.tGate.anValue[ LE_U32_MSB_IDX ] = tTemp.anValue[ LE_U32_LSB_IDX ];
  AsciiCommandHandler_GetValue( nCmdEnum, 1, &tTemp.uValue );
  tLwipCfgActl.tGate.anValue[ LE_U32_MS2_IDX ] = tTemp.anValue[ LE_U32_LSB_IDX ];
  AsciiCommandHandler_GetValue( nCmdEnum, 2, &tTemp.uValue );
  tLwipCfgActl.tGate.anValue[ LE_U32_MS1_IDX ] = tTemp.anValue[ LE_U32_LSB_IDX ];
  AsciiCommandHandler_GetValue( nCmdEnum, 3, &tTemp.uValue );
  tLwipCfgActl.tGate.anValue[ LE_U32_LSB_IDX ] = tTemp.anValue[ LE_U32_LSB_IDX ];

  // return no output
  return( ASCCMD_STS_NONE );
}

/******************************************************************************
 * @function CmdSetNtm
 *
 * @brief Set the network mode
 *
 * This function will set the network mode in the local config structure
 *
 * @return  
 *****************************************************************************/
static ASCCMDSTS CmdSetNtm( U8 nCmdEnum )
{
  U32UN     tTemp;
  ASCCMDSTS eStatus = ASCCMD_STS_NONE;

  // test for an empty buffer
  if ( pcBuffer == NULL )
  {
    // fetch it 
    AsciiCommandHandler_GetBuffer( nCmdEnum, &pcBuffer );
  }

  // get the arguments
  AsciiCommandHandler_GetValue( nCmdEnum, 0, &tTemp.uValue );

  // check for valid
  if ( tTemp.uValue < LWIP_CONFIG_MODE_MAX )
  {
    // store the mode
    tLwipCfgActl.eMode = tTemp.anValue[ LE_U32_LSB_IDX ];
  }
  else
  {
    // output the error
    sprintf( pcBuffer, g_szAsciiErrStrn, -1, -1 );
    eStatus = ASCCMD_STS_OUTPUTBUFFER;
  }

  // return the status
  return( eStatus );
}

/******************************************************************************
 * @function CmdSetNet
 *
 * @brief Set the network config
 *
 * This function will set the network config
 *
 * @return  
 *****************************************************************************/
static ASCCMDSTS CmdSetNet( U8 nCmdEnum )
{
  #if ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_TASKMANAGER )
  // update the config in memory
  TaskManager_PostEvent( LWIPCONTROLHANDLER_CONFIGUPDATE_TASK, CONFIG_TYPE_LWIPCFG );
  #elif ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_FREERTOS )
  xQueueSend( ConfigManagerQueueHandle, &eConfigType, portMAX_DELAY );
  #endif // SYSTEMDEFINE_OS_SELECTION
}
#endif // LWIPCONTROLHANDLER_ENABLE_DEBUGCMDS

/**@} EOF LwipControlHandler.c */
