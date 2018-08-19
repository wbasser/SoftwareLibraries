/******************************************************************************
 * @file EthernetHandler.h
 *
 * @brief ATMEL SAME Internet Interface
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
 * \addtogroup EthernetHandler
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _ETHERNETHANDLER_H
#define _ETHERNETHANDLER_H

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------

// library includes -----------------------------------------------------------
#include "lwip/netif.h"
#include "lwip/err.h"

// Macros and Defines ---------------------------------------------------------
/// define the number of events
#define ETHERNETHANDLER_NUM_EVENTS          ( 1 )

/// de4fine the number of RX buffers
#define ETHERNETHANDLER_NUM_RX_BUFFERS      ( 3 )

/// define the number of TX buffers
#define ETHERNETHANDLER_NUM_TX_BUFFERS      ( 3 )

/// define the size of the MAC address
#define ETHERNETHANDLER_MACADDR_SIZE        ( 6 )

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations -----------------------------------------------

// global function prototypes --------------------------------------------------
extern  err_t EthernetHandler_Initialize( struct netif *ptNetIf );
extern  void  EthernetHandler_StatusCallback( struct netif *ptNetIf );
extern  void  EthernetHandler_ProcessInput( void );

/**@} EOF EthernetHandler.h */

#endif  // _ETHERNETHANDLER_H/**

