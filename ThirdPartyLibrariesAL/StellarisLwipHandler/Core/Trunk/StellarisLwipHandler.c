/******************************************************************************
 * @file StellarisLwipHandler.c
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
 * \addtogroup StellarisLwipHandler
 * @{
 *****************************************************************************/

// system includes ------------------------------------------------------------
#include "Types/Types.h"

// library includes -----------------------------------------------------------
#include "inc/hw_ethernet.h"
#include "driverlib/ethernet.h"
#include "driverlib/flash.h"
#include "src/include/lwip/init.h"
#include "src/include/ipv4/lwip/inet.h"
#include "src/include/ipv4/lwip/ip.h"
#include "src/include/lwip/stats.h"
#include "src/include/lwip/snmp.h"
#include "src/include/lwip/dhcp.h"
#include "src/include/ipv4/lwip/autoip.h"
#include "src/include/netif/etharp.h"
#include "src/include/lwip/dns.h"
#include "ports/stellaris/include/netif/stellarisif.h"
#include "ConfigManager/ConfigManager.h"

// local includes -------------------------------------------------------------
#include "StellarisLwipHandler/StellarisLwipHandler.h"
#include "StellarisLwipHandler/StellarisLwipHandler_prm.h"

// Macros and Defines ---------------------------------------------------------
#undef LWIP_DHCP_AUTOIP_COOP
#define LWIP_DHCP_AUTOIP_COOP   ((LWIP_DHCP) && (LWIP_AUTOIP))

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations ----------------------------------------------

// local parameter declarations -----------------------------------------------
static  struct netif    tNetIf;
static  U32             uLocalTimer;
static  U32             uTcpTimer;
static  U32             uMdixTimer;
static  LWIPCONFIGMODE  eMode;
#if HOST_TMR_INTERVAL
static  U32             uHostTimer;
#endif
#if LWIP_ARP
static  U32             uArpTimer;
#endif
#if LWIP_AUTOIP
static  U32             uAutoIpTimer;
#endif
#if LWIP_DHCP
static  U32             uDhcpCoarseTimer;
static  U32             uDhcpFineTimer;
#endif
#if IP_REASSEMBLY
static  U32             uIpReassemblyTimer;
#endif
#if LWIP_IGMP
static  U32             uIgmpTimer;
#endif
#if LWIP_DNS
static  U32             uDnsTimer;
#endif
static  LWIPCFGDEF      tLwipCfgActl;
#if ( STELLARISLWIP_ENABLE_DEBUGCMDS == 1 )
static  PC8             pcBuffer;
#endif // STELLARISLWIP_ENABLE_DEBUGCMDS

// local function prototypes --------------------------------------------------
static  void      IrqHandler( void );

/// command handlers
#if ( STELLARISLWIP_ENABLE_DEBUGCMDS == 1 )
static  ASCCMDSTS CmdQryNet( U8 nCmdEnum );
static  ASCCMDSTS CmdSetMac( U8 nCmdEnum );
static  ASCCMDSTS CmdQryMac( U8 nCmdEnum );
static  ASCCMDSTS CmdSetIpa( U8 nCmdEnum );
static  ASCCMDSTS CmdSetMsk( U8 nCmdEnum );
static  ASCCMDSTS CmdSetGwy( U8 nCmdEnum );
static  ASCCMDSTS CmdSetNtm( U8 nCmdEnum );
static  ASCCMDSTS CmdSetNet( U8 nCmdEnum );

// constant parameter initializations -----------------------------------------
/// command strings
static  const CODE C8 szQryNet[ ]   = { "QNET" };
static  const CODE C8 szSetMac[ ]   = { "SMAC" };
static  const CODE C8 szQryMac[ ]   = { "QMAC" };
static  const CODE C8 szSetIpa[ ]   = { "SIPA" };
static  const CODE C8 szSetMsk[ ]   = { "SMSK" };
static  const CODE C8 szSetGwy[ ]   = { "SGWY" };
static  const CODE C8 szSetNtm[ ]   = { "SNTM" };
static  const CODE C8 szSetNet[ ]   = { "SNET" };
#endif // STELLARISLWIP_ENABLE_DEBUGCMDS

/// IP defaults
const LWIPCFGDEF  tLwipCfgDflts =
{
  .eMode = LWIP_CONFIG_MODE_DHCP,
  .tAddr =
  {
    .anValue[ LE_U32_MSB_IDX ] = 192,
    .anValue[ LE_U32_MS2_IDX ] = 177,
    .anValue[ LE_U32_MS1_IDX ] = 1,
    .anValue[ LE_U32_LSB_IDX ] = 125,
  },
  .tMask =
  {
    .anValue[ LE_U32_MSB_IDX ] = 255,
    .anValue[ LE_U32_MS2_IDX ] = 255,
    .anValue[ LE_U32_MS1_IDX ] = 255,
    .anValue[ LE_U32_LSB_IDX ] = 0,
  },
  .tGate =
  {
    .anValue[ LE_U32_MSB_IDX ] = 192,
    .anValue[ LE_U32_MS2_IDX ] = 177,
    .anValue[ LE_U32_MS1_IDX ] = 1,
    .anValue[ LE_U32_LSB_IDX ] = 1,
  },
};

#if ( STELLARISLWIP_ENABLE_DEBUGCMDS == 1 )
/// initialize the command table
const CODE ASCCMDENTRY atLwipHandlerCmdTable[ ] =
{
  ASCCMD_ENTRY( szQryNet, 4, 1, ASCFLAG_COMPARE_NONE, 0,                             CmdQryNet ),
  ASCCMD_ENTRY( szSetMac, 4, 2, ASCFLAG_COMPARE_EQ,   STELLARISLWIP_DIAGMODE_ENUM,   CmdSetMac ),
  ASCCMD_ENTRY( szQryMac, 4, 0, ASCFLAG_COMPARE_NONE, 0,                             CmdQryMac ),
  ASCCMD_ENTRY( szSetIpa, 4, 4, ASCFLAG_COMPARE_EQ,   STELLARISLWIP_DIAGMODE_ENUM,   CmdSetIpa ),
  ASCCMD_ENTRY( szSetMsk, 4, 4, ASCFLAG_COMPARE_EQ,   STELLARISLWIP_DIAGMODE_ENUM,   CmdSetMsk ),
  ASCCMD_ENTRY( szSetGwy, 4, 4, ASCFLAG_COMPARE_EQ,   STELLARISLWIP_DIAGMODE_ENUM,   CmdSetGwy ),
  ASCCMD_ENTRY( szSetNtm, 4, 1, ASCFLAG_COMPARE_EQ,   STELLARISLWIP_DIAGMODE_ENUM,   CmdSetNtm ),
  ASCCMD_ENTRY( szSetNet, 4, 0, ASCFLAG_COMPARE_EQ,   STELLARISLWIP_DIAGMODE_ENUM,   CmdSetNet ),

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
#endif // STELLARISLWIP_ENABLE_DEBUGCMDS

/******************************************************************************
 * @function StellarisLwipHandler_Initialize
 *
 * @brief stellaris LWIP handler initialization
 *
 * This function initializes the stellaris LWIP implementation.  It will also
 * initialize any protocols that are identified in the configuration
 *
 *****************************************************************************/
void StellarisLwipHandler_Initialize( void )
{
  U32UN           tUser0, tUser1;
  U8              anMacAddress[ 6 ];
  
  // clear all timers
  uLocalTimer = uTcpTimer = uMdixTimer = 0;
  #if HOST_TMR_INTERVAL
  uHostTimer = 0;
  #endif
  #if LWIP_ARP
  uArpTimer = 0;
  #endif
  #if LWIP_AUTOIP
  uAutoIpTimer = 0;
  #endif
  #if LWIP_DHCP
  uDhcpCoarseTimer = 0;
  uDhcpFineTimer = 0;
  #endif
  #if IP_REASSEMBLY
  uIpReassemblyTimer = 0;
  #endif
  #if LWIP_IGMP
  uIgmpTimer = 0;
  #endif
  #if LWIP_DNS
  uDnsTimer = 0;
  #endif
  
  // enable and reset the ethernet controller
  MAP_SysCtlPeripheralEnable( SYSCTL_PERIPH_ETH );
  MAP_SysCtlPeripheralReset( SYSCTL_PERIPH_ETH );
  
  // check for leds
  #if ( STELLARISLWIP_ETHLED_PERIPH != 0 )
  MAP_SysCtlPeripheralEnable( STELLARISLWIP_ETHLED_PERIPH );
  GPIOPinTypeEthernetLED( STELLARISLWIP_PORT_BASE, STELLARISLWIP_ETHLED_LINK_PIN | STELLARISLWIP_ETHLED_ACT_PIN );
  #endif
  
  // register the interrupt handler
  EthernetIntRegister( ETH_BASE, IrqHandler );
  
  // get the MAC address from the user registers/set the MAC address in the PHY
  MAP_FlashUserGet( &tUser0.uValue, &tUser1.uValue );
  anMacAddress[ 0 ] = tUser0.anValue[ LE_U32_LSB_IDX ];
  anMacAddress[ 1 ] = tUser0.anValue[ LE_U32_MS1_IDX ];
  anMacAddress[ 2 ] = tUser0.anValue[ LE_U32_MS2_IDX ];
  anMacAddress[ 3 ] = tUser1.anValue[ LE_U32_LSB_IDX ];
  anMacAddress[ 4 ] = tUser1.anValue[ LE_U32_MS1_IDX ];
  anMacAddress[ 5 ] = tUser1.anValue[ LE_U32_MS2_IDX ];
  EthernetMACAddrSet( ETH_BASE, ( unsigned char *)&anMacAddress );

  #if ( STELLARISLWIP_ENABLE_DEBUGCMDS == 1 )
  pcBuffer = NULL;
  #endif // STELLARISLWIP_ENABLE_DEBUGCMDS
}

/******************************************************************************
 * @function StellarisLwipHandler_GetConfigDflt
 *
 * @brief get the default config
 *
 * This function returns a pointer to the default config
 *
 * @return      pointer to the default config
 *
 *****************************************************************************/
PLWIPCFGDEF StellarisLwipHandler_GetConfigDflt( void )
{
  // return a pointer to the default
  return(( PLWIPCFGDEF )&tLwipCfgDflts );
}

/******************************************************************************
 * @function StellarisLwipHandler_GetConfigActl
 *
 * @brief get the actual config
 *
 * This function returns a pointer to the actual config
 *
 * @return      pointer to the actual config
 *
 *****************************************************************************/
PLWIPCFGDEF StellarisLwipHandler_GetConfigActl( void )
{
  // return a pointer to the actual
  return( &tLwipCfgActl );
}

/******************************************************************************
 * @function StellarisLwipHandler_Congifure
 *
 * @brief stellaris LWIP handler initialization
 *
 * This function initializes the stellaris LWIP implementation.  It will also
 * initialize any protocols that are identified in the configuration
 *
 *****************************************************************************/
void StellarisLwipHandler_Configure( void )
{
  struct ip_addr  tIpAddr;
  struct ip_addr  tNetMask;
  struct ip_addr  tGatewayAddr;
  PLWIPPROTDEF    ptDef;
  LWIPPROTS       eProt;

  // initialize the lwip stack
  lwip_init( );

  // determine if static or DHCP
  tIpAddr.addr = htonl( tLwipCfgActl.tAddr.uValue );
  tNetMask.addr = htonl( tLwipCfgActl.tMask.uValue );
  tGatewayAddr.addr = htonl( tLwipCfgActl.tGate.uValue );
  
  // add the ethernet controller to the stack
  netif_add( &tNetIf, &tIpAddr, &tNetMask, &tGatewayAddr, NULL, stellarisif_init, ip_input );
  netif_set_default( &tNetIf );
  
  // if DHCP enabled, start it
  #if LWIP_DHCP
  if ( tLwipCfgActl.eMode == LWIP_CONFIG_MODE_DHCP )
  {
    // start it
    dhcp_start( &tNetIf );
  }
  #endif
  
  // if auot IP enabled, start it
  #if LWIP_AUTOIP
  if ( tLwipCfgActl.eMode == LWIP_CONFIG_MODE_AUTOIP )
  {
    // start it
    autoip_start( &tNetIf );
  }
  #endif
  
  // bring the interface up
  netif_set_up( &tNetIf );
  
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
 * @function StellarisLwipHandler_SetConfig
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
LWIPERRORS StellarisLwipHandler_SetConfig( PLWIPCFGDEF ptLwipCfgDef )
{
  // copy the config structure
  memcpy( &tLwipCfgActl, ptLwipCfgDef, LWIPCFGDEF_SIZE );
}

/******************************************************************************
 * @function StellarisLwipHandler_GetCurrentNetConfig
 *
 * @brief gets the current network config
 *
 * This function gets and returns the current network configuration
 *
 * @param[io]   ptLwipCfgDef  pointer to the config structure
 *
 *****************************************************************************/
void StellarisLwipHandler_GetCurrentNetConfig( PLWIPCFGDEF ptConfig )
{
  // now copy the current network interface parameters
  ptConfig->tAddr.uValue = tNetIf.ip_addr.addr;
  ptConfig->tMask.uValue = tNetIf.netmask.addr;
  ptConfig->tGate.uValue = tNetIf.gw.addr;
}

/******************************************************************************
 * @function StellarisLwipHandler_GetMacAddr
 *
 * @brief gets the current mac address
 *
 * This function gets the current MAC address
 *
 * @param[io]   pnMacAddress   pointer to the address
 *
 *****************************************************************************/
void StellarisLwipHandler_GetMacAddr( PU8 pnMacAddress )
{
  // return the mac address
  EthernetMACAddrGet( ETH_BASE, pnMacAddress );
}

/******************************************************************************
 * @function StellarisLwipHandler_ProcessTimer
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
BOOL StellarisLwipHandler_ProcessTimer( TASKARG arg )
{
  PLWIPPROTDEF    ptDef;
  LWIPPROTS       eProt;

  // update the local timer
  uLocalTimer += STELLARISLWIP_EXEC_RATE / TASK_TIME_MSECS( 1 );
  
  // generate an thernet interrupt
  HWREG( NVIC_SW_TRIG ) | INT_ETH - 16;
  
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
  
  // always return true
  return( TRUE );
}

/******************************************************************************
 * @function IrqHandler
 *
 * @brief Ethernet LWIP handler
 *
 * This function processes the interrupt from the ethernet controller
 *
 *****************************************************************************/
static void IrqHandler( void )
{
  U32 uStatus;
  
  // read and clear the interrupt
  uStatus = EthernetIntStatus( ETH_BASE, FALSE );
  EthernetIntClear( ETH_BASE, uStatus );
  
  // check for the need to run the low level interrupt handler
  if ( uStatus != 0 )
  {
    // call the low level interrupt function
    stellarisif_interrupt( &tNetIf );
  }
  
  // service the MDIX timer
  if (( EthernetPHYRead( ETH_BASE, PHY_MR1 ) & PHY_MR1_LINK ) == 0 )
  {
    // has the link been gone for 2 seconds
    if (( uLocalTimer - uMdixTimer ) >= 2000 )
    {
      // flip the MDI/MDIX switch
      HWREG( ETH_BASE + MAC_O_MDIX ) ^= MAC_MDIX_EN;
      
      // reset the timer
      uMdixTimer = uLocalTimer;
    }
  }
  else
  {
    // link active, reset the MDIX timer
    uMdixTimer = uLocalTimer;
  }
  
  // service the ARP timer
  #if LWIP_ARP
  if (( uLocalTimer - uArpTimer ) >= ARP_TMR_INTERVAL )
  {
    // reset the timer/call the ARP handler
    uArpTimer = uLocalTimer;
    etharp_tmr( );
  }
  #endif
  
  // service the TCP timer
  #if LWIP_TCP
  if (( uLocalTimer - uTcpTimer ) >= TCP_TMR_INTERVAL )
  {
    // reset the timer/call the TCP handler
    uTcpTimer = uLocalTimer;
    tcp_tmr( );
  }
  #endif
  
  // service the auto IP timer
  #if LWIP_AUTOIP
  if (( uLocalTimer - uAutoIpTimer ) >= AUTOIP_TMR_INTERVAL )
  {
    // reset the timer/call the auto IP handler
    uAutoIpTimer = uLocalTimer;
    autoip_tmr( );
  }
  #endif
  
  // service the DCHP coarse timer
  #if LWIP_DHCP
  if (( uLocalTimer - uDhcpCoarseTimer ) >= DHCP_COARSE_TIMER_MSECS )
  {
    // reser the timer/call the coarse handler
    uDhcpCoarseTimer = uLocalTimer;
    dhcp_coarse_tmr( );
  }
  
  if (( uLocalTimer - uDhcpFineTimer ) >= DHCP_FINE_TIMER_MSECS )
  {
    // reser the timer/call the fine handler
    uDhcpFineTimer = uLocalTimer;
    dhcp_fine_tmr( );
  }
  #endif
  
  // service the IP reassembly timer
  #if LWIP_REASSEMBLY
  if (( uLocalTimer - uIpReassemblyTimer ) >= IP_TMR_INTERVAL )
  {
    // reset the timer/call the handler
    uIpReassemblyTimer = uLocalTimer;
    ip_reass_tmr( );
  }
  #endif
  
  // servie the IGMP timer
  #if LWIP_IGMP
  if (( uLocalTimer - uIgmpTimer ) >= IGMP_TMR_INTERVAL )
  {
    // reset the timer/call the handler
    uIgmpTimer = uLocalTimer;
    igmp_tmr( );
  }
  #endif
  
  // service the DNS timer
  #if LWIP_DNS
  if (( uLocalTimer - uDnsTimer ) >= DNS_TMR_INTERVAL )
  {
    // reser the timer/call the handler
    uDnsTimer = uLocalTimer;
    dns_tmr( );
  }
  #endif

  // service the host timer
  #if HOST_TMR_INTERVAL
  if (( uLocalTimer - uHostTimer ) >= HOST_TMR_INTERVAL )
  {
    // reset the timer/call the handler
    uHostTimer = uLocalTimer;

    // check to see if a function was defined
    #ifdef STELLARISLWIP_HOST_FUNCTION
      STELLARISLWIP_HOST_FUNCTION( );
    #endif

  }
  #endif
}

#if ( STELLARISLWIP_ENABLE_DEBUGCMDS == 1 )
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
    tLclConfig.tAddr.uValue = tNetIf.ip_addr.addr;
    tLclConfig.tMask.uValue = tNetIf.netmask.addr;
    tLclConfig.tGate.uValue = tNetIf.gw.addr;
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
  U32UN     tUser0;
  U32UN     tUser1;
  
  // get the base address, length
  AsciiCommandHandler_GetValue( nCmdEnum, 0, &tUser0.uValue );
  AsciiCommandHandler_GetValue( nCmdEnum, 1, &tUser1.uValue );

  // now write it
  MAP_FlashUserSet( tUser0.uValue, tUser1.uValue );

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

  // get the mac address
  MAP_FlashUserGet( &tUser0.uValue, &tUser1.uValue );

  // now output it
  sprintf( pcBuffer, szRspMac, tUser0.anValue[ LE_U32_LSB_IDX ],
                               tUser0.anValue[ LE_U32_MS1_IDX ],
                               tUser0.anValue[ LE_U32_MS2_IDX ],
                               tUser1.anValue[ LE_U32_LSB_IDX ],
                               tUser1.anValue[ LE_U32_MS1_IDX ],
                               tUser1.anValue[ LE_U32_MS2_IDX ] );

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
  // call the function to set the net config
  StellarisLwipHandler_SetConfig( &tLwipCfgActl );
}
#endif // STELLARISLWIP_ENABLE_DEBUGCMDS

/**@} EOF StellarisLiwpHandler.c */
