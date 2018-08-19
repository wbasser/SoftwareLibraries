/******************************************************************************
 * @file AtmelSAM7LwipHandler.c
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
 * \addtogroup AtmelSAM7LwipHandler
 * @{
 *****************************************************************************/

// system includes ------------------------------------------------------------
#include "Types/Types.h"

// library includes -----------------------------------------------------------
#include "src/include/lwip/init.h"
#include "src/include/ipv4/lwip/inet.h"
#include "src/include/ipv4/lwip/ip.h"
#include "src/include/lwip/stats.h"
#include "src/include/lwip/snmp.h"
#include "src/include/lwip/dhcp.h"
#include "src/include/ipv4/lwip/autoip.h"
#include "src/include/netif/etharp.h"
#include "src/include/lwip/dns.h"
#include "ports//include/netif/atmelsam4.h"
#include "ConfigManager/ConfigManager.h"

// local includes -------------------------------------------------------------
#include "AtmelSAM7LwipHandler/AtmelSAM7LwipHandler.h"
#include "AtmelSAM7LwipHandler/AtmelSAM7LwipHandler_prm.h"

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
#define TIMEDEF_SIZE        sizeof ( TIMEDEF )

// global parameter declarations ----------------------------------------------

// local parameter declarations -----------------------------------------------
static  struct netif    tNetIf;
static  U32             uLocalTimer;
static  TIMEDEF         atTimers[ ] =
{
  { 0, TCP_TMR_INTERVAL,        tcp_timer       },
  ( 0, IP_TMR_INTERVAL,         ip_reass_tmr    },
  { 0, TCP_FAST_INTERVAL,       tcp_fasttmr     },
  { 0, TCP_SLOW_INTERVAL,       tcp_slowtmr     },
  { 0, ARP_TMR_INTERVAL,        etharp_tmr      },
  #if LWIP_DHCP
  { 0, DHCP_COARSE_TIMER_MSECS, dchp_coarse_tmr },
  { 0, DHCP_FINE_TIMER_MSECS,   dhcp_fine_tmr   },
  #enidf
};

// local function prototypes --------------------------------------------------

// constant parameter initializations -----------------------------------------
/// define the configuration defaults
static  const LWIPCFGDEF  tLwipCfgDflts =
{
  .eMode = LWIP_CONFIG_MODE_STATIC,
  .tAddr =
  {
    .anValue[ LE_U32_MSB_IDX ] = 192,
    .anValue[ LE_U32_MS2_IDX ] = 168,
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
    .anValue[ LE_U32_MS2_IDX ] = 168,
    .anValue[ LE_U32_MS1_IDX ] = 1,
    .anValue[ LE_U32_LSB_IDX ] = 1,
  },
  .anMac =
  { 
    0x00, 
    0x0F, 
    0x54, 
    0xFF, 
    0xFF, 
    0xFE 
  },
};

/******************************************************************************
 * @function AtmelSAM7LwipHandler_Initialize
 *
 * @brief stellaris LWIP handler initialization
 *
 * This function initializes the stellaris LWIP implementation.  It will also
 * initialize any protocols that are identified in the configuration
 *
 *****************************************************************************/
void AtmelSAM7LwipHandler_Initialize( void )
{
}

/******************************************************************************
 * @function AtmelSAM7LwipHandler_GetConfigDflt
 *
 * @brief get the default config
 *
 * This function returns a pointer to the default config
 *
 * @return      pointer to the default config
 *
 *****************************************************************************/
PLWIPCFGDEF AtmelSAM7LwipHandler_GetConfigDflt( void )
{
  // return a pointer to the default
  return(( PLWIPCFGDEF )&tLwipCfgDflts );
}

/******************************************************************************
 * @function AtmelSAM7LwipHandler_GetConfigActl
 *
 * @brief get the actual config
 *
 * This function returns a pointer to the actual config
 *
 * @return      pointer to the actual config
 *
 *****************************************************************************/
PLWIPCFGDEF AtmelSAM7LwipHandler_GetConfigActl( void )
{
  // return a pointer to the actual
  return( &tLwipCfgActl );
}

/******************************************************************************
 * @function AtmelSAM7LwipHandler_Congifure
 *
 * @brief stellaris LWIP handler initialization
 *
 * This function initializes the stellaris LWIP implementation.  It will also
 * initialize any protocols that are identified in the configuration
 *
 *****************************************************************************/
void AtmelSAM7LwipHandler_Configure( void )
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
 * @function AtmelSAM7LwipHandler_SetConfig
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
LWIPERRORS AtmelSAM7LwipHandler_SetConfig( PLWIPCFGDEF ptLwipCfgDef )
{
  // copy the config structure
  memcpy( &tLwipCfgActl, ptLwipCfgDef, LWIPCFGDEF_SIZE );
}

/******************************************************************************
 * @function AtmelSAM7LwipHandler_GetCurrentNetConfig
 *
 * @brief gets the current network config
 *
 * This function gets and returns the current network configuration
 *
 * @param[io]   ptLwipCfgDef  pointer to the config structure
 *
 *****************************************************************************/
void AtmelSAM7LwipHandler_GetCurrentNetConfig( PLWIPCFGDEF ptConfig )
{
  // now copy the current network interface parameters
  ptConfig->tAddr.uValue = tNetIf.ip_addr.addr;
  ptConfig->tMask.uValue = tNetIf.netmask.addr;
  ptConfig->tGate.uValue = tNetIf.gw.addr;
}

/******************************************************************************
 * @function AtmelSAM7LwipHandler_GetMacAddr
 *
 * @brief gets the current mac address
 *
 * This function gets the current MAC address
 *
 * @param[io]   pnMacAddress   pointer to the address
 *
 *****************************************************************************/
void AtmelSAM7LwipHandler_GetMacAddr( PU8 pnMacAddress )
{
  // return the mac address
}

/******************************************************************************
 * @function AtmelSAM7LwipHandler_ProcessTimer
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
BOOL AtmelSAM7LwipHandler_ProcessTimer( TASKARG arg )
{
  PLWIPPROTDEF  ptDef;
  LWIPPROTS     eProt;
  U8            nTimerIdx;
  PTIMERDEF     ptTimer;

  // update the local timer
  uLocalTimer += ATMELSAM7LWIP_EXEC_RATE / TASK_TIME_MSECS( 1 );
  
  // now process timer table
  for( nTimerIdx = 0; nTimerIdx < ( sizeof( atTimers ) / TIMERDEF_SIZE ); nTimerIdx++ )
  {
    // set the pointer to the timer
    ptTimer = atTimers[ nTimerIdx ];
    
    // has time expired on this entry
    if (( uLocalTimer - ptTimer->uCurTime ) >= ptTimer->uTimeInterval )
    {
      // reset the timer interval
      ptTimer->uCurTime = uLocalTimer;
      
      // check for a null function
      if ( ptTimer->pvTimeFunction != NULL )
      {
        // execut it
        ptTimer->pvTimeFunction( );
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

/**@} EOF AtmelSAM7LwipHandler.c */
