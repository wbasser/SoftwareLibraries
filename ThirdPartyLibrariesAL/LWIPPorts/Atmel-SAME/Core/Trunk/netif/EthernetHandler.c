/******************************************************************************
 * @file EthernetHandler.c
 *
 * @brief ATMEL SAM4 Interface
 *
 * This file 
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
 * \addtogroup EthernetHandler
 * @{
 *****************************************************************************/

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------

// library includes -----------------------------------------------------------
#include "lwip/opt.h"
#include "lwip/def.h"
#include "lwip/mem.h"
#include "lwip/pbuf.h"
#include "lwip/sys.h"
#include "lwip/stats.h"
#include "lwip/snmp.h"
#include "netif/etharp.h"
#include "netif/ppp_oe.h"
#include "GMAC/Gmac.h"
#include "Clock/Clock.h"
#include "KSZ8081RNACAPhy/KSZ8081RNACAPhy.h"

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations ----------------------------------------------

// local parameter declarations -----------------------------------------------

// local function prototypes --------------------------------------------------

// constant parameter initializations -----------------------------------------

// local includes -------------------------------------------------------------
#include "netif/EthernetHandler.h"
//#include "netif/EthernetHandler_prm.h"

/// define the maximum transfer unit
#define NET_MTU                 ( 1500 )

/// define the network link speed
#define NET_LINK_SPEED          ( 100000000l )

/// define the interrupt priority/group/tx/rx error masks
//#define INT_PRIORITY_GMAC       ( 12 )
//#define GMAC_INT_GROUP_MASK     ( GMAC_ISR_RCOMP | GMAC_ISR_ROVR | GMAC_ISR_HRESP | GMAC_ISR_TCOMP | GMAC_ISR_TUR | GMAC_ISR_TFC )
//#define GMAC_TX_ERROR_MASK      ( GMAC_TSR_TFC | GMAC_TSR_UND | GMAC_TSR_HRESP )
//#define GMAC_RX_ERROR_MASK      ( GMAC_RSR_RXOVR | GMAC_RSR_HNO )
#define INT_PRIORITY_GMAC       ( 0 )
#define GMAC_INT_GROUP_MASK     ( 0 )
#define GMAC_TX_ERROR_MASK      ( 0 )
#define GMAC_RX_ERROR_MASK      ( 0 )

/// define the interface name
#define IFNAME0                 ( 'e' )
#define IFNAME1                 ( 'n' )

/// define the interrupt group
#define GMAC_INT_GROUP          ( 0 )

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------
/// define the structre
typedef struct _GMACDEVICE
{
  GMACRXDESCRIPTOR  atRxDescriptors[ ETHERNETHANDLER_NUM_RX_BUFFERS ];
  GMACTXDESCRIPTOR  atTxDescriptors[ ETHERNETHANDLER_NUM_TX_BUFFERS ];
  struct pbuf*      aptRxBuffers[ ETHERNETHANDLER_NUM_RX_BUFFERS ];
  U8                anTxBuffers[ ETHERNETHANDLER_NUM_TX_BUFFERS ][ GMAC_TX_UNITSIZE ];
  U32               uRxIndex;
  U32               uTxIndex;
  struct netif      *ptNetIf;
} GMACDEVICE, *PGMACDEVICE;
#define GMACDEVICE_SIZE         sizeof ( GMACDEVICE )

// global parameter declarations ----------------------------------------------

// local parameter declarations -----------------------------------------------
ALIGNED(8)
static  GMACDEVICE    tGmacDevice;

// local function prototypes --------------------------------------------------
static  void          LowLevelInitialize( struct netif* ptNetIf );
static  err_t         LowLevelOutput( struct netif* ptNetIf, struct pbuf* ptBuffer );
static  struct pbuf*  LowLevelInput( struct netif* ptNetIf );
static  void          RxPopulateQueue( PGMACDEVICE ptDevice );
static  void          RxInitialize( PGMACDEVICE ptDevice );
static  void          TxInitialize( PGMACDEVICE ptDevice );
static  U8            anMacAddress[ ETHERNETHANDLER_MACADDR_SIZE ];

/// global function prototypes ------------------------------------------------
void  GMAC_Handler( void );

// constant parameter initializations -----------------------------------------
#if LWIP_NETIF_HOSTNAME
/// declare the host name
static  const C8  szHostName = { "GMACDevice" };
#endif // LWIP_NETIF_HOSTNAME

/******************************************************************************
 * @function EthernetHandler_Initialize
 *
 * @brief initialization
 *
 * This function will perform all low level initialization
 *
 * @param[in]   ptNetIf   pointer to the network interface structure
 *
 *****************************************************************************/
err_t EthernetHandler_Initialize( struct netif *ptNetIf )
{
  // set the net interface pointer in the device
  tGmacDevice.ptNetIf = ptNetIf;

  // test and set the name
  #if LWIP_NETIF_HOSTNAME
  ptNetIf->hostname = szHostName;
  #endif // LWIP_NETIF_HOSTNAME}

  // store the pointer to the GMAC device into the state
  ptNetIf->state = &tGmacDevice;

  // set the name
  ptNetIf->name[ 0 ] = IFNAME0;
  ptNetIf->name[ 1 ] = IFNAME1;

  // set the output and linke output functions
  ptNetIf->output = etharp_output;
  ptNetIf->linkoutput = LowLevelOutput;

  // initialize the hardware
  LowLevelInitialize( ptNetIf );

  // return NO error
  return( ERR_OK );
}

/******************************************************************************
 * @function ProcessInput
 *
 * @brief process the input and dispatch to the lwip handler
 *
 * This function will process the input buffer
 *
 *****************************************************************************/
void EthernetHandler_ProcessInput( void )
{
	struct eth_hdr* ptEthHdr;
	struct pbuf*    ptBuffer;
  struct netif*   ptNetIf;
  
  // set the net interface pointer
  ptNetIf = tGmacDevice.ptNetIf;

	// receive the packet
  if (( ptBuffer = LowLevelInput( ptNetIf )) != NULL )
  {
    // set the header to point to the payload
    ptEthHdr = ptBuffer->payload;

    switch ( htons( ptEthHdr->type ))
    {
      case ETHTYPE_IP:
      case ETHTYPE_ARP:
      #if PPPOE_SUPPORT
      case ETHTYPE_PPPOEDISC:
      case ETHTYPE_PPPOE:
      #endif // PPPOE_SUPPORT
        //* Send packet to lwIP for processing
        if ( ptNetIf->input( ptBuffer, ptNetIf) != ERR_OK )
        {
          LWIP_DEBUGF( NETIF_DEBUG, ("EthernetHandler_ProcessInpu: IP input error\n"));
			  }
        break;

      default:
        break;
	  }

    // free the buffer
    pbuf_free( ptBuffer );
  }
  
  // return TRUE to flush event
  return( TRUE );
}

/******************************************************************************
 * @function LowLevelInitilaize
 *
 * @brief low level initialization
 *
 * This function will perform all low level initialization
 *
 * @param[in]   ptNetIf   pointer to the network interface structure
 *
 *****************************************************************************/
static void LowLevelInitialize( struct netif *ptNetIf )
{
  U32 uDelay;
  
  // set the MTU
  ptNetIf->mtu = NET_MTU;
  
  // set the device capabilities
  ptNetIf->flags |= NETIF_FLAG_BROADCAST | NETIF_FLAG_ETHARP;
  #if defined( DHCP_USED )
  ptNetIf->flags |= NETIF_FLAG_DHCP;
  #endif
  
  // wait for PHY to get ready
  uDelay = Clock_GetFreq( ) / 6;
  while( uDelay-- != 0 );
  
  // disable the GMAC/disable interrupt/clear statistics
  Gmac_NetworkControl( GMAC, 0 );
  Gmac_DisableInterrupt( GMAC, 0xFFFFFFFF );
  Gmac_ClearStatistics( GMAC );
  
  // clear all bits in the receive status register
  Gmac_ClearRxStatus( GMAC, GMAC_RSR_BNA | GMAC_RSR_REC | GMAC_RSR_RXOVR | GMAC_RSR_HNO );
  
  // clear all bits in the transmit status register
  Gmac_ClearTxStatus( GMAC, GMAC_TSR_UBR | GMAC_TSR_COL |GMAC_TSR_RLE | GMAC_TSR_TXGO | GMAC_TSR_TFC | GMAC_TSR_TXCOMP | GMAC_TSR_HRESP );

  // clear interrupts
  Gmac_GetInterruptStatus( GMAC );
  
  // endable copy of data but not braodcases
  Gmac_EnableCopyAll( GMAC, OFF );
  Gmac_DisableBroadcast( GMAC, OFF );
  
  // set RX buffer size to 1536
  Gmac_SetRxBufSize( GMAC, 0x18 );
  RxInitialize( &tGmacDevice );
  TxInitialize( &tGmacDevice );
  
  // enable RX/TX and statistics
  Gmac_EnableTransmit( GMAC, ON );
  Gmac_EnableReceive( GMAC, ON );
  Gmac_EnableStatisticsWrite( GMAC, ON );
  
  // set up the interrupts
  Gmac_EnableInterrupt( GMAC, GMAC_INT_GROUP );
  
  // write the gmac address
  Gmac_SetAddress( GMAC, 0, ptNetIf->hwaddr );
  
  // enable the NVIC GMAC interrupt
  NVIC_SetPriority( GMAC_IRQn, INT_PRIORITY_GMAC );
  NVIC_EnableIRQ( GMAC_IRQn );
  
  // initialize the PHY
  if ( KSZ8051MNLPhy_Initialize(( PU32 )GMAC, 0 ) != GMAC_STATUS_OK )
  {
    // report error
    LWIP_DEBUGF( NETIF_DEBUG, ( "LowLevelInit: PHY init error!\n" ));
  }
  else
  {
    // set auto negotiate
    if ( KSZ8081RNACAPhy_AutoNegotiate(( PU32 )GMAC, 0 ) != GMAC_STATUS_OK )
    {
      // report error
      LWIP_DEBUGF( NETIF_DEBUG, ( "LowLevelInit: PHY auto negotiate error\n" ));
    }
    else
    {
      // establish link
      while( KSZ8081RNACAPhy_SetLink(( PU32 )GMAC, 0, 1 ) != GMAC_STATUS_OK )
      {
        // report error
        LWIP_DEBUGF( NETIF_DEBUG, ( "LowLevelInit:: PHY set link error!\n" ));
      }
    }
  }
}

/******************************************************************************
 * @function LowLevelOutput
 *
 * @brief performs the low level output
 *
 * This function will transmit the actual packet 
 *
 * @param[in]   ptNetIf     pointer to the network interface structure
 * @param[in]   ptBuffer    pointer to the buffer
 *
 * @return      
 *
 *****************************************************************************/
static err_t LowLevelOutput( struct netif* ptNetIf, struct pbuf* ptBuffer )
{
  PGMACDEVICE   ptDevice;
  struct pbuf*  ptLclBuf;
  PU8           pnBuffer;
  
  // clear the local pointers
  ptLclBuf = NULL;
  pnBuffer = NULL;
  
  // get the device pointer
  ptDevice = ( PGMACDEVICE )ptNetIf->state;
  
  // check for underrun or AHB errors
  if ( Gmac_GetTxStatus( GMAC ) & GMAC_TX_ERROR_MASK )
  {
    // report it
    #if LWIP_DEBUGF
    LWIP_DEBUGF( NETIF_DEBUG, ( "LowLevelOutput: GMAC ERROR, reint TX...\n" ));
    #endif

    // turn of the transmitter
    Gmac_EnableTransmit( GMAC, OFF );
    
    // update the stats
    LINK_STATS_INC( link.err );
    LINK_STATS_INC( link.drop );
    
    // reinit TX descriptors
    TxInitialize( ptDevice );
    
    // clear error status
    Gmac_ClearTxStatus( GMAC, GMAC_TX_ERROR_MASK );
    
    // re-enable transmit
    Gmac_EnableTransmit( GMAC, ON );
  }
  
  // get the pointer to the buffer
  pnBuffer = ( PU8 )ptDevice->atTxDescriptors[ ptDevice->uTxIndex ].uAddress;
  
  // for each pbuf in chain
  for ( ptLclBuf = ptBuffer; ptLclBuf != NULL; ptLclBuf = ptLclBuf->next )
  {
    // copy data to buffer
    memcpy( pnBuffer, ptLclBuf->payload, ptLclBuf->len );
    pnBuffer += ptLclBuf->len;
  }
  
  // set the lgnth and mark the buffer to be sent
  ptDevice->atTxDescriptors[ ptDevice->uTxIndex ].tStatus.tBmFields.uLen = ptBuffer->tot_len;
  ptDevice->atTxDescriptors[ ptDevice->uTxIndex ].tStatus.tBmFields.bUsed = FALSE;
  LWIP_DEBUGF( NETIF_DEBUG, ( "LowLevelOutput: DMA buffer sent, size=%d [idx-%u]\n", ptBuffer->tot_len, ptDevice->uTxIndex ));
  
  // increment the buffer index
  ptDevice->uTxIndex = ( ptDevice->uTxIndex + 1 ) % ETHERNETHANDLER_NUM_TX_BUFFERS;
  
  // now start the transmission
  Gmac_StartTransmission( GMAC );
  
  // update stats
  #if LWIP_STATUS
  lwip_tx_count += ptBuffer->tot_len;
  #endif
  LINK_STATS_INC( link.xmit );
  
  // return ok
  return( ERR_OK );
}

/******************************************************************************
 * @function LowLevelInput
 *
 * @brief gets and returns the new incoming packet
 *
 * This function will get the new packet and return it
 *
 * @param[in]   ptNetIf     pointer to the network interface
 *
 * @return      pointer to the new buffer
 *
 *****************************************************************************/
static struct pbuf* LowLevelInput( struct netif* ptNetIf )
{
  PGMACDEVICE       ptDevice;
  struct pbuf*      ptLclBuf;
  U32               uLength, uIndex;
  PGMACRXDESCRIPTOR ptRxDescriptor;
  
  // clear the local pointers
  ptLclBuf = NULL;
  
  // get the device pointer
  ptDevice = ( PGMACDEVICE )ptNetIf->state;
  
  // get the current RX descriptor
  ptRxDescriptor = &ptDevice->atRxDescriptors[ ptDevice->uRxIndex ];
  
  // check for ovverrun and AHB errors
  if ( Gmac_GetRxStatus( GMAC ) & GMAC_RX_ERROR_MASK )
  {
    // turn off the receiver
    Gmac_EnableReceive( GMAC, OFF );
    
    // update link status
    LINK_STATS_INC( link.err );
    LINK_STATS_INC( link.drop );
    
    // free all RX buffers
    for( uIndex = 0; uIndex < ETHERNETHANDLER_NUM_RX_BUFFERS; uIndex++ )
    {
      // check for not empty
      if ( ptDevice->aptRxBuffers[ uIndex ] != NULL )
      {
        // free the buffer
        pbuf_free( ptDevice->aptRxBuffers[ uIndex ] );
        ptDevice->aptRxBuffers[ uIndex ] = NULL;
      }
    }
    
    // re-initialize the RX descriptors
    RxInitialize( ptDevice );
    
    // clear error status
    Gmac_ClearRxStatus( GMAC, GMAC_RX_ERROR_MASK );
    
    // re-=enable receiver
    Gmac_EnableReceive( GMAC, ON );
  }
  
  // check for a received packet 
  if (( ptRxDescriptor->tAddr.uValue & GMAC_RXD_OWNERSHIP ) == GMAC_RXD_OWNERSHIP )
  {
    // get the length
    uLength = ptRxDescriptor->tStatus.uValue & GMAC_RXD_LEN_MASK;
    
    // fetch a pre-allocated buffer/set the length
    ptLclBuf = ptDevice->aptRxBuffers[ ptDevice->uRxIndex ];
    ptLclBuf->len = uLength;
    
    // remove buffer from descriptor
    ptDevice->aptRxBuffers[ ptDevice->uRxIndex ] = NULL;
    LWIP_DEBUGF( NETIF_DEBUG, ( "LowLevelInput: DMA buffer %p received, size=%u [idx=%u]\n", ptLclBuf, uLength, ptDevice->uRxIndex ));
    
    // set total packet size
    ptLclBuf->tot_len = uLength;
    LINK_STATS_INC( link.recv );
    
    // fill empty descriptors with new pbufs
    RxPopulateQueue( ptDevice );
    
    // mark the descriptor ready for transfer
    ptRxDescriptor->tAddr.uValue &= ~( GMAC_RXD_OWNERSHIP );
    
    // adjust the receive index
    ptDevice->uRxIndex = ( ptDevice->uRxIndex + 1 ) % ETHERNETHANDLER_NUM_RX_BUFFERS;
    
    // adjust state
//    #if LWIP_STATS
//    lwip_rx_count += uLength;
//    #endif
  }

  // return the new pointer to the buffer
  return( ptLclBuf );
}

/******************************************************************************
 * @function RxInitialize
 *
 * @brief intialize the rx descriptors
 *
 * This function will initialize the RX descriptors
 *
 * @param[in]   ptDevice    pointer to the Device
 *
 *****************************************************************************/
static void RxInitialize( PGMACDEVICE ptDevice )
{
  U32 uIndex;
  
  // clear the index
  ptDevice->uRxIndex = 0;
  
  // for each descriptior
  for ( uIndex = 0; uIndex < ETHERNETHANDLER_NUM_RX_BUFFERS; uIndex++ )
  {
    // clear the buffer ointer/address /status
    ptDevice->aptRxBuffers[ uIndex ] = NULL;    
    ptDevice->atRxDescriptors[ uIndex ].tAddr.uValue = 0;
    ptDevice->atRxDescriptors[ uIndex ].tStatus.uValue = 0;
  }

  // set the wrap on last one
  ptDevice->atRxDescriptors[ uIndex - 1 ].tAddr.uValue |= GMAC_RXD_WRAP;
  
  // build RX uffer and descriptors
  RxPopulateQueue( ptDevice );
  
  // set receive buffer queue base address pointer
  Gmac_SetRxQueue( GMAC, ( U32 )&ptDevice->atRxDescriptors[ 0 ] );
}

/******************************************************************************
 * @function TxInitialize
 *
 * @brief initialize the TX descriptors
 *
 * This function will set up the TX descriptors
 *
 * @param[in]   ptDevice    pointer to the Device
 *
 *****************************************************************************/
static void TxInitialize( PGMACDEVICE ptDevice )
{
  U32 uIndex;
  
  // clear the index
  ptDevice->uTxIndex = 0;
  
  // for each TX descriptor
  for ( uIndex = 0; uIndex < ETHERNETHANDLER_NUM_TX_BUFFERS; uIndex++ )
  {
    // set the address/clear the status
    ptDevice->atTxDescriptors[ uIndex ].uAddress = ( U32 )&ptDevice->anTxBuffers[ uIndex ][ 0 ];
    ptDevice->atTxDescriptors[ uIndex ].tStatus.uValue = GMAC_TXD_USED | GMAC_TXD_LAST;
  }

  // set the wrap on last one
  ptDevice->atTxDescriptors[ uIndex - 1 ].tStatus.uValue |= GMAC_TXD_WRAP;
  
  // set base address pointer
  Gmac_SetTxQueue( GMAC, ( U32 )&ptDevice->atTxDescriptors[ 0 ] );
}

/******************************************************************************
 * @function RxPopulateQueue
 *
 * @brief populate the RX queues  
 *
 * This function will populate the RX queues
 *
 * @param[in]   ptDevice      pointer to the device structure
 *
 *****************************************************************************/
static void RxPopulateQueue( PGMACDEVICE ptDevice )
{
  U32           uIndex;
  struct pbuf*  ptBuf = NULL;
  
  // for each RS descriptors
  for ( uIndex = 0; uIndex < ETHERNETHANDLER_NUM_RX_BUFFERS; uIndex++ )
  {
    // is this buffer empty
    if ( ptDevice->aptRxBuffers[ uIndex ] == NULL )
    {
      // allocate a new buffer with the minimum size
      ptBuf = pbuf_alloc( PBUF_RAW, ( U16 )GMAC_FRAME_LENTGH_MAX, PBUF_POOL );
      if ( ptBuf == NULL )
      {
        // generate an error
        #if LWIP_DEBUG
        //LWIP_DEBUG( NETIF_DEBUG( "RxPopulateQueue: pbuf allocation failure\n" ));
        #endif
        break;
      }
      
      // make sure LWIP is well configured so one pbuf can cantain the maximum packet size.
      LWIP_ASSERT( "RxPopulateQueue: pbuf size to small!", pbuf_clen( ptBuf ) <= 1 );
      
      // make sure the payload buffer is properly alighned.
      LWIP_ASSERT( "RxPopulateQueue: unaligned payload buffer address", ((( U32 )ptBuf->payload & 0xFFFFFFC ) == ( U32 )ptBuf->payload ));
      
      // set wrap if not last buffer
      if ( uIndex == ( ETHERNETHANDLER_NUM_RX_BUFFERS - 1 ))
      {
        // set address with wrap
        ptDevice->atRxDescriptors[ uIndex ].tAddr.uValue = ( U32 )ptBuf->payload | GMAC_RXD_WRAP;
      }
      else
      {
        // set address 
        ptDevice->atRxDescriptors[ uIndex ].tAddr.uValue = ( U32 )ptBuf->payload;
      }
      
      // reset status value
      ptDevice->atRxDescriptors[ uIndex ].tStatus.uValue = 0;
      
      // save pointer
      ptDevice->aptRxBuffers[ uIndex ] = ptBuf;
      
      // report it
      #if LWIP_DEDBUG
      LWIP_DEBUG( NETIF_DEBUG, "RxPopulateQueue: new pbuf allocation: %p [ idx=%u]\n", ptBuf, uIndex );
      #endif
    }
  }
}

/******************************************************************************
 * @function GMAC_Handler
 *
 * @brief IRQ handler
 *
 * This function will handle the IRQ from the GMAC
 *
 *****************************************************************************/
void GMAC_Handler( void )
{
  NVIC_DisableIRQ( GMAC_IRQn );
//  U32 uStatus;
//
//  // get the interrupt status
//  uStatus = Gmac_GetInterruptStatus( GMAC );
//
//  // test for RX interrupts
//  if ( uStatus & GMAC_INT_GROUP_MASK )
//  {
//    // post the process receive task
//    TaskManager_PostEvent( ETHERNETHANDLER_PROCESS_RCV_TASK_ENUM, 0 );
//  }
}


/**@} EOF .c */
