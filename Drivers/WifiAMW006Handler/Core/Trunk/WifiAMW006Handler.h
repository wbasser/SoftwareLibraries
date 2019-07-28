/******************************************************************************
 * @file WifiAMW006Handler.h
 *
 * @brief SIlicon Labs Zentri AWM006 WIFI driver declarations
 *
 * This file provides the declarations for the SiliconLabs Zentri-AMW006 WIFI
 * driver
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
 * \addtogroup WifiAMW006Handler
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _WIFIAMW006HANDLER_H
#define _WIFIAMW006HANDLER_H

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------

// library includes -----------------------------------------------------------
#include "TaskManager/TaskManager.h"

// Macros and Defines ---------------------------------------------------------
/// define the RCV/XMT buffer sizes
#define WIFIAWM006HANDLER_XMTBUF_SIZE         ( 250 )
#define WIFIAWM006HANDLER_RCVBUF_SIZE         ( 0 )

/// define the number of character events
#define WIFIAWM006HANDLER_NUM_CHR_EVENTS      ( 250 )
#define WIFIAWM006HANDLER_NUM_CTL_EVENTS      ( 8 )

/// define the number of queue entries
#define WIFIAWM006HANDLER_NUM_QUEUE_ENTRIES   ( 4 )

// enumerations ---------------------------------------------------------------
/// define the socket type
typedef enum _WIFISKTTYPE
{
  WIFISKT_TYPE_UDP = 0,
  WIFISKT_TYPE_TCP,
  WIFISKT_TYPE_MAX
} WIFISKTTYPE;

/// enumerate the error code
typedef enum _WIFIERROR
{
  WIFI_ERROR_NONE = 0,
  WIFI_ERROR_NOTCONNECTED,
  WIFI_ERROR_ILLSOCKET,
} WIFIERROR;

// structures -----------------------------------------------------------------
/// define the socket type
typedef U32   WIFISKT;

/// define the socket callback function
typedef void  ( *PVWIFICALLBACK )( WIFIERROR, PVOID );

/// define the request message structure
typedef struct _WIFIMSGREQ
{
  PC8             pszMessage;
  U16             wMsgLength;
  PVWIFICALLBACK  pvCallback;
} WIFIMSGREQ, *PWIFIMSGREQ;
#define WIFIMSGREQ_SIZE                 sizeof( WIFIMSGREQ )

// global parameter declarations -----------------------------------------------

// global function prototypes --------------------------------------------------
extern  void      WifiAMW006Handler_Initialize( void );
extern  void      WifiAWM006Handler_InitialConfig( void );
extern  void      WifiAWM006Handler_PutChar( U8 nChar );
extern  BOOL      WifiAMW006Handler_ProcessCtrl( TASKARG xArg );
extern  BOOL      WifiAMW006Handler_ProcessChar( TASKARG xArg );
extern  void      WifiAMW006Handler_IrqCallback( U8 nIrq, U8 nEvent );
extern  BOOL      WifiAWM006Handler_GetConnectionStatus( void );
extern  WIFIERROR WifiAWM006Handler_OpenSocket( WIFISKTTYPE eSktType, PC8 pszAddress, U16 wPort, PVWIFICALLBACK pvCallback );
extern  WIFIERROR WifiAWM006Handler_ReadSocket( WIFISKT tSocket, PU8 pnBuffer, U16 wBufSize, PU16 wBytesRead );
extern  WIFIERROR WifiAWM006Handler_WriteSocket( WIFISKT tSocket, PU8 pnBuffer, U16 wBufSize, PU16 wBytesWritten );

/**@} EOF WifiAMW006Handler.h */

#endif  // _WIFIAMW006HANDLER_H