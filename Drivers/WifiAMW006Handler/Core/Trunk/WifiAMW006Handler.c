/******************************************************************************
 * @file WifiAMW006Handler.c
 *
 * @brief Silicon Labs Zentri-AMW006 WIfi chip implementation
 *
 * This file provides the implementation for the Silicon Labs Zentri-AMW006 WIFI
 * chip implementaton
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

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "WifiAMW006Handler/WifiAMW006Handler.h"

// library includes -----------------------------------------------------------
#include "GPIO/Gpio.h"
#include "QueueManager/QueueManager.h"
#include "RedirectionManager/RedirectionManager.h"
#include "UART/Uart.h"

///////////////////////////////////////////////////////////////////////////////
// FOR DEBUG ONLY
#include "DebugManager/DebugManager.h"
///////////////////////////////////////////////////////////////////////////////

// Macros and Defines ---------------------------------------------------------
/// define the out of bounds event masks
#define OOB_IOCONN_MASK             ( 0x0001 )
#define OOB_NETWORK_MASK            ( 0x0002 )
#define OOB_TCPSRVCONN_MASK         ( 0x0004 )
#define OOB_TCPSRVDATA_MASK         ( 0x0008 )
#define OOB_UDPSRVDATA_MASK         ( 0x0010 )
#define OOB_TCPCLNDATA_MASK         ( 0x0020 )
#define OOB_UDPCLNDATA_MASK         ( 0x0040 )

/// define the response timeout
#define RESP_TIMEOUT                ( TASK_TIME_MSECS( 250 ))

/// define the lengths of the SSID and the passkey
#define WLAN_SSID_MAX_LEN           ( 32 )
#define WLAN_PASS_MAX_LEN           ( 32 )

/// define the helper macro for creating entries in the tables
#define INITCMDDEFNONE( cmd ) \
  { \
    .pszCommand = ( const PC8 )cmd, \
    .eArgType = ARG_TYPE_NONE, \
    .tArguments = \
    { \
      .pszOption = NULL, \
    } \
  }

#define INITCMDDEFSTRING( cmd, option ) \
  { \
    .pszCommand = ( const PC8 )cmd, \
    .eArgType = ARG_TYPE_STRING, \
    .tArguments = \
    { \
      .pszOption = ( const PC8 )option, \
    } \
  }

#define INITCMDDEFCONFIG( cmd, option ) \
  { \
    .pszCommand = ( const PC8 )cmd, \
    .eArgType = ARG_TYPE_CONFIGENUM, \
    .tArguments = \
    { \
      .eConfigType = option, \
    } \
  }

/// define the message request put queue event
#define REQMSGXMIT_EVENT          QUEUEPUT_EVENT( QUEUE_ENUM_WIFIMSGS )

// enumerations ---------------------------------------------------------------
/// enumerate the local events
typedef enum _WIFIEVENT
{
  WIFI_EVENT_MSG_RCVD = 0x2300,
  WIFI_EVENT_REQ_INIT,
  WIFI_EVENT_REQ_CNFG,
} WIFIEVENT;

/// enumerate the argument types
typedef enum _ARGTYPE
{
  ARG_TYPE_NONE = 0,
  ARG_TYPE_STRING,
  ARG_TYPE_CONFIGENUM,
  ARG_TYPE_MAX
} ARGTYPE;

/// enumerate the configuration type
typedef enum _CFGTYPE
{
  CFG_TYPE_SSID = 0,
  CFG_TYPE_PASSKEY,
  CFG_TYPE_MAX
} CFGTYPE;

/// enumerate the response state
typedef enum _RESPSTATE
{
  RESP_STATE_IDLE = 0,
  RESP_STATE_ERROR,
  RESP_STATE_LENGTH,
  RESP_STATE_OPTION,
  RESP_STATE_MAX
} RESPSTATE;

/// enumerate the error type
typedef enum _ERRORTYPE
{
  ERROR_TYPE_NONE = 0,            /// no error
  ERROR_TYPE_CMDFAILED,
  ERROR_TYPE_PARSE,
  ERROR_TYPE_TOOFEWARGS,
  ERROR_TYPE_TOOMANYARGS,
  ERROR_TYPE_UNKOWNVAR,
  ERROR_TYPE_INVALIDARG,
  ERROR_TYPE_BUFFOVERFLOW,
} ERRORTYPE;

/// enumerate the control states
typedef enum _CTRLSTATE
{
  CTRL_STATE_IDLE = 0,
  CTRL_STATE_INIT,
  CTRL_STATE_CONFIG,
  CTRL_STATE_WAIT,
  CTRL_STATE_MAX
} CTRLSTATE;

// structures -----------------------------------------------------------------
/// define the initialization structure
typedef struct _INITCMDS
{
  PC8     pszCommand;             ///< pointer to the command
  ARGTYPE eArgType;
  union
  {
    CFGTYPE eConfigType;          ///< config type
    PC8     pszOption;            ///< option    
  } tArguments;
} INITCMDS, *PINITCMDS;
#define INITCMDS_SIZE                       sizeof( INITCMDS )

/// define the configuration structure
typedef struct _WIFICONFIG
{
  C8  acSSID[ WLAN_SSID_MAX_LEN ];
  C8  acPassword[ WLAN_PASS_MAX_LEN ];
} WIFICONFIG, *PWIFICONFIG;
#define WIFICONFIG_SIZE                     sizeof( WIFICONFIG )

/// define the socket control structure
typedef struct _SOCKETCTL
{
  U8              nSocketNum;
  PVWIFICALLBACK  pvCallback;
} SOCKETCTL, *PSOCKETCTL;
#define SOCKETCTL_SIZE                      sizeof( SOCKETCTL )

// global parameter declarations ----------------------------------------------

// local parameter declarations -----------------------------------------------
static  RESPSTATE       eRespState;
static  CTRLSTATE       eNxtCtrlState;
static  CTRLSTATE       eCurCtrlState;
static  ERRORTYPE       eLastError;
static  U16             wRecvLength;
static  U8              anRecvBuffer[ WIFIAWM006HANDLER_NUM_CHR_EVENTS ];
static  U16             wRecvBufIdx;
static  U8              anXmitBuffer[ WIFIAWM006HANDLER_NUM_CHR_EVENTS ];
static  U16             wXmitBufIdx;
static  U8              nCmdIndex;
static  WIFICONFIG      tWifiConfigActl;
static  PVWIFICALLBACK  pvCurCallback;
static  BOOL            bConnected;

// local function prototypes --------------------------------------------------
static  void  ProcessOobStatus( WIFIERROR eError, PVOID pvOption );
static  void  ProcessNetworkStatus( WIFIERROR eError, PVOID pvOption );
static  void  ProcessClientData( WIFIERROR eError, PVOID pvOption );
static  U8    AscHex( U8 nChar );
static  BOOL  SendCommandMessage( PINITCMDS ptInitCmd );
static  void  SendGetCommand( PC8 pszCommand );
static  void  OutputBuffer( PU8 pnBuffer, U16 wLength );
static  PC8   GetConfigValue( CFGTYPE eCfgType );

// constant parameter initializations -----------------------------------------
/// instantiate the default WIFI config values
static  const CODE WIFICONFIG tWifiConfigDflt[ ] =
{
  { "CyberDevLcl" },
  { "Cyber&%%)"   },
};

/// format strings
static  const CODE C8 szSetCmdOpt[ ]      = { "set %s %s\r\n" };
static  const CODE C8 szSetCmdNoOpt[ ]    = { "set %s \r\n" };
static  const CODE C8 szGetCmd[ ]         = { "get %s\r\n" };

/// commands
static  const CODE C8 szWlanSSID[ ]       = { "wl s" };
static  const CODE C8 szWlanPasskey[ ]    = { "wl p" };
static  const CODE C8 szWlanAutoJoin[ ]   = { "wl o e" };
static  const CODE C8 szWlanNetStatus[ ]  = { "wl n s" };
static  const CODE C8 szSystemOobSts[ ]   = { "sy o s" };
static  const CODE C8 szSystemOobGpio[ ]  = { "sy o g" };
static  const CODE C8 szSystemOobLevel[ ] = { "sy o l" };
static  const CODE C8 szSystemOobEdge[ ]  = { "sy o r" };
static  const CODE C8 szSystemOobMask[ ]  = { "sy o e" };
static  const CODE C8 szCmdSave[ ]        = { "save" };
static  const CODE C8 szCmdReboot[ ]     =  { "reboot" };
static  const CODE C8 szNetworkUp[ ]      = { "nup" };
static  const CODE C8 szNetworkDn[ ]      = { "ndo" };
static  const CODE C8 szNetworkRestart[ ] = { "nre" };
static  const CODE C8 szCommandMode[ ]    = { "sy c m" };
static  const CODE C8 szStreamPollAll[ ]  = { "poll all -r" };
static  const CODE C8 szClientConnect[ ]  = { "tcp_client %s %d" };

/// constant arguments
static  const CODE C8 szHuman[ ]          = { "human" };
static  const CODE C8 szMachine[ ]        = { "machine" };
static  const CODE C8 szTrue[ ]           = { "true" };
static  const CODE C8 szFalse[ ]          = { "false" };
static  const CODE C8 szOn[ ]             = { "1" };
static  const CODE C8 szOff[ ]            = { "0" };
static  const CODE C8 szEdgeMask[ ]       = { "0x60" };
static  const CODE C8 szStatusMask[ ]     = { "0x3F" };
static  const CODE C8 szGpioNum[ ]        = { "20" };
static  const CODE C8 szFmtHexVar[ ]      = { "%x" };

/// instantiate the setup commands structure
static  const CODE INITCMDS atInitCmds[ ]  = 
{
  INITCMDDEFSTRING( szCommandMode,    szMachine         ),
  INITCMDDEFSTRING( szSystemOobGpio,  szGpioNum         ),
  INITCMDDEFSTRING( szSystemOobLevel, szOn              ),
  INITCMDDEFSTRING( szSystemOobEdge,  szEdgeMask        ),
  INITCMDDEFSTRING( szSystemOobMask,  szStatusMask      ),
  INITCMDDEFNONE(   szCmdSave                           ),
  INITCMDDEFNONE(   szCmdReboot                         ),
  INITCMDDEFNONE(   NULL                                )
};

/// instantiate the network command structure
static  const CODE INITCMDS atNetworkCmds[ ] =
{
  INITCMDDEFNONE(   szNetworkDn                        ),
  INITCMDDEFSTRING( szWlanAutoJoin, szOn               ),
  INITCMDDEFCONFIG( szWlanSSID,     CFG_TYPE_SSID      ),
  INITCMDDEFCONFIG( szWlanPasskey,  CFG_TYPE_PASSKEY   ),
  INITCMDDEFNONE(   szNetworkUp                        ),
  INITCMDDEFNONE(   NULL                               )
};

/******************************************************************************
 * @function WifiAMW006Handler_Initialize
 *
 * @brief initializaton
 *
 * This function will perform any needed initializaton
 *
 *****************************************************************************/
void WifiAMW006Handler_Initialize( void )
{
  // clear the response state/control state
  eRespState = RESP_STATE_IDLE;
  eCurCtrlState = CTRL_STATE_IDLE;
  bConnected = FALSE;

  // for now just copy the defaults to the actual
  memcpy(( PVOID )&tWifiConfigActl, ( PVOID )&tWifiConfigDflt, WIFICONFIG_SIZE );
}

/******************************************************************************
 * @function WifiAMW006Handler_InitialConfig
 *
 * @brief initial configuration
 *
 * This function will configure the modem initially
 *
 *****************************************************************************/
void WifiAMW006Handler_InitialConfig( void )
{
  // post a status changed event
  TaskManager_PostEvent( TASK_SCHD_ENUM_WIFCTRL, WIFI_EVENT_REQ_INIT );
}

/******************************************************************************
 * @function WifiAMW006Handler_PutCharacter
 *
 * @brief output a character
 *
 * This function will write a single character out of the port
 *
 *****************************************************************************/
void WifiAWM006Handler_PutChar( U8 nChar )
{
  // call the output buffer function
  OutputBuffer( &nChar, 1 );
}

/******************************************************************************
 * @function WifiAMW006Handler_ProcessCtrl
 *
 * @brief process a control event
 *
 * This function will process the control event
 *
 * @param[in]   xArg    task argument
 *
 * @return      TRUE to flush events
 *
 *****************************************************************************/
BOOL WifiAMW006Handler_ProcessCtrl( TASKARG xArg )
{
  PINITCMDS   ptCmds;
  WIFIMSGREQ  tMsgReq;

///////////////////////////////////////////////////////////////////////////////
// FOR DEBUG ONLY
DebugManager_AddElement( 0x8000 | eCurCtrlState, xArg );
///////////////////////////////////////////////////////////////////////////////
  // determine the state
  switch( eCurCtrlState )
  {
    case CTRL_STATE_IDLE :
      // determine the argument
      switch( xArg )
      {
        case WIFI_EVENT_REQ_INIT :
          // clear the command index
          nCmdIndex = 0;

          // send the message 
          ptCmds = ( PINITCMDS )&atInitCmds[ nCmdIndex ]; 
          SendCommandMessage( ptCmds );

          // set the callback to null
          pvCurCallback = NULL;

          // goto init state
          eNxtCtrlState = CTRL_STATE_INIT;
          eCurCtrlState = CTRL_STATE_WAIT;
          break;

        case WIFI_EVENT_REQ_CNFG :
          // clear the command index
          nCmdIndex = 0;

          // send the message
          ptCmds = ( PINITCMDS )&atNetworkCmds[ nCmdIndex ];
          SendCommandMessage( ptCmds );

          // set the callback to null
          pvCurCallback = NULL;

          // goto init state
          eNxtCtrlState = CTRL_STATE_CONFIG;
          eCurCtrlState = CTRL_STATE_WAIT;
          break;

        case REQMSGXMIT_EVENT :
          // get the queue entry
          QueueManager_Get( QUEUE_ENUM_WIFIMSGS, ( PU8 )&tMsgReq );

          // set the callback
          pvCurCallback = tMsgReq.pvCallback;

          // send the message
          SendGetCommand( tMsgReq.pszMessage );
          
          // set the next and current states
          eNxtCtrlState = CTRL_STATE_IDLE;
          eCurCtrlState = CTRL_STATE_WAIT;
          break;

        default :
          break;
      }
      break;

    case CTRL_STATE_INIT :
      // increment the command index/send the next command out
      ptCmds = ( PINITCMDS )&atInitCmds[ ++nCmdIndex ];

      // send the message
      eCurCtrlState = SendCommandMessage( ptCmds ) ? CTRL_STATE_WAIT : CTRL_STATE_IDLE;

      eNxtCtrlState = CTRL_STATE_INIT;
      break;

    case CTRL_STATE_CONFIG :
      // increment the command index/send the next command out
      ptCmds = ( PINITCMDS )&atNetworkCmds[ ++nCmdIndex ];

      // send the message
      eCurCtrlState = SendCommandMessage( ptCmds ) ? CTRL_STATE_WAIT : CTRL_STATE_IDLE;

      // goto init state
      eNxtCtrlState = CTRL_STATE_CONFIG;
      break;

    case CTRL_STATE_WAIT :
      // determine the argument
      switch( xArg )
      {
        case TASK_TIMEOUT_EVENT :
          break;

        case WIFI_EVENT_MSG_RCVD :
          // check to see if callback is not null
          if ( pvCurCallback != NULL )
          {
            // process it
            pvCurCallback( WIFI_ERROR_NONE, NULL );
          }

          // set the state to the next state
          eCurCtrlState = eNxtCtrlState;
          break;

        default :
          break;
      }
      break;

    default :
      eCurCtrlState = CTRL_STATE_IDLE;
      break;
  }

  // return true to flush event
  return( TRUE );
}

/******************************************************************************
 * @function WifiAMW006Handler_ProcessChar
 *
 * @brief process a character
 *
 * This function will process an incoming character
 *
 * @param[in]   xArg    task argument
 *
 * @return      TRUE to flush events
 *
 *****************************************************************************/
BOOL WifiAMW006Handler_ProcessChar( TASKARG xArg )
{
  // process redirection
  if ( !RedirectionManager_RedirectChar( REDIRECT_ENUM_WIFI, ( U8 )xArg ))
  {
    // determine the state
    switch( eRespState )
    {
      case RESP_STATE_IDLE :
        // is this an R
        if ( xArg == 'R' )
        {
          // clear the last error/length
          eLastError = ERROR_TYPE_NONE;
          wRecvLength = 0;
          wRecvBufIdx = 0;

          // move to error
          eRespState = RESP_STATE_ERROR;
        }
        break;

      case RESP_STATE_ERROR :
        // save the error
        eLastError = ( ERRORTYPE )xArg;

        // goto length
        eRespState = RESP_STATE_LENGTH;
        break;
    
      case RESP_STATE_LENGTH :
        // check for terminating character
        if ( xArg == '\r' )
        {
          // go to next state based on length
          eRespState = ( wRecvLength != 0 ) ? RESP_STATE_OPTION : RESP_STATE_IDLE;

          // post recieve event
          if ( eRespState == RESP_STATE_IDLE )
          {
            // post the ressage receive event
            TaskManager_PostEvent( TASK_SCHD_ENUM_WIFCTRL, WIFI_EVENT_MSG_RCVD );
          }
        }
        else
        {
          // build the length
          wRecvLength *= 10;
          wRecvLength += AscHex( xArg );
        }
        break;

      case RESP_STATE_OPTION :
        // check for termination
        if (( xArg == '\n' ) || ( wRecvBufIdx == wRecvLength ))
        {
          // back to idle
          eRespState = RESP_STATE_IDLE;

          // post recieve event
          TaskManager_PostEvent( TASK_SCHD_ENUM_WIFCTRL, WIFI_EVENT_MSG_RCVD );
        }

        // check for overflow
        if ( wRecvLength < WIFIAWM006HANDLER_NUM_CHR_EVENTS )
        {
          // save the character
          anRecvBuffer[ wRecvBufIdx++ ] = xArg;
        }
        break;

      default :
        eRespState = RESP_STATE_IDLE;
    }
  }

  // return TRUE to flush event
  return( TRUE );
}

/******************************************************************************
 * @function WifiAMW006Handler_IrqCallback
 *
 * @brief interrupt callback
 *
 * This function will process the IRQ callback
 *
 * @param[in]   nIrq    IRQ number
 * @param[in]   nEvent  event
 *
 *****************************************************************************/
void WifiAMW006Handler_IrqCallback( U8 nIrq, U8 nEvent )
{
  WIFIMSGREQ  tMessage;

///////////////////////////////////////////////////////////////////////////////
// FOR DEBUG ONLY
DebugManager_AddElement( 0x9000, 0 );
///////////////////////////////////////////////////////////////////////////////
  // set a message
  tMessage.pszMessage = ( PC8 )szSystemOobSts;
  tMessage.wMsgLength = strlen( szSystemOobSts );
  tMessage.pvCallback = ProcessOobStatus;

  // post a status changed event
  QueueManager_PutTail( QUEUE_ENUM_WIFIMSGS, ( PU8 )&tMessage );
}

/******************************************************************************
 * @function WifiAMW006Handler_GetConnectionStatus
 *
 * @brief get the current connection status
 *
 * This function will get the current connection status
 *
 * @return      current connection status
 *
 *****************************************************************************/
BOOL WifiAWM006Handler_GetConnectionStatus( void )
{
  // return the connection status
  return( bConnected );
}

/******************************************************************************
 * @function WifiAMW006Handler_OpenSocket
 *
 * @brief open a socket
 *
 * This function will open a socket for communication
 *
 * @param[in]   eSktTYpe    socket type (UDP/TCP)
 * @param[in]   pszAddress  address to connect to
 * @param[in]   wPort       port number
 * @param[in]   pvCallback  callback function
 *
 * @return      error
 *
 *****************************************************************************/
WIFIERROR WifiAWM006Handler_OpenSocket( WIFISKTTYPE eSktType, PC8 pszAddress, U16 wPort, PVWIFICALLBACK pvCallback )
{
  WIFIERROR eError = WIFI_ERROR_NONE;

  // check for connected
  if ( bConnected )
  {

  }
  else
  {
    // set the error
    eError = WIFI_ERROR_NOTCONNECTED;
  }

  // return the socket
  return( eError );
}

/******************************************************************************
 * @function WifiAMW006Handler_ReadSocket
 *
 * @brief read data from the socket
 *
 * This function will read data from the socket
 *
 * @param[in]   tSocket   socket number
 * @param[in]   pnBuffer  pointer to the data storage
 * @param[in]   wBufSize  maximum number of bytes to read
 *
 * @return      error
 *
 *****************************************************************************/
WIFIERROR WifiAWM006Handler_ReadSocket( WIFISKT tSocket, PU8 pnBuffer, U16 wBufSize, PU16 wBytesRead  )
{
  WIFIERROR eError = WIFI_ERROR_NONE;

  // check for connected
  if ( bConnected )
  {

  }
  else
  {
    // set the error
    eError = WIFI_ERROR_NOTCONNECTED;
  }

  // return the socket
  return( eError );
}

/******************************************************************************
 * @function WifiAMW006Handler_WriteSocket
 *
 * @brief write data to the socket
 *
 * This function will write data to the socket
 *
 * @param[in]   tSocket   socket number
 * @param[in]   pnBuffer  pointer to the data storage
 * @param[in]   wBufSize  number of bytes to write
 *
 * @return      error
 *
 *****************************************************************************/
WIFIERROR WifiAWM006Handler_WriteSocket( WIFISKT tSocket, PU8 pnBuffer, U16 wBufSize, PU16 wBytesWritten  )
{
  WIFIERROR eError = WIFI_ERROR_NONE;

  // check for connected
  if ( bConnected )
  {

  }
  else
  {
    // set the error
    eError = WIFI_ERROR_NOTCONNECTED;
  }

  // return the socket
  return( eError );
}

/******************************************************************************
 * @function WifiAWM006Handler_SetConnectParams
 *
 * @brief set connection parameters
 *
 * This function will set the connection parameters
 *
 * @param[in]   pcSSID      pointer to the SSID
 * @param[in]   pcPasskey   ponter to the pass key
 *
 *****************************************************************************/
void WifiAWM006Handler_SetConnectParams( PC8 pcSSID, PC8 pcPasskey )
{
  // copy the SSID and passkey
  memcpy(( PC8 )&tWifiConfigActl.acSSID, pcSSID, STRLEN_P( pcSSID ));
  memcpy(( PC8 )&tWifiConfigActl.acPassword, pcPasskey, STRLEN_P( pcPasskey ));

  // post a status changed event
  TaskManager_PostEvent( TASK_SCHD_ENUM_WIFCTRL, WIFI_EVENT_REQ_INIT );
}

/******************************************************************************
 * @function ProcessOobStatus
 *
 * @brief process the status
 *
 * This function will process the last status
 *
 * @param[in]     eError      error
 *
 *****************************************************************************/
static void ProcessOobStatus( WIFIERROR eError, PVOID pvOption )
{
  U16         wStatus;
  WIFIMSGREQ  tMessage;

  // get the status 
  sscanf((PC8 )&anRecvBuffer, szFmtHexVar, ( unsigned int* )&wStatus );
///////////////////////////////////////////////////////////////////////////////
// FOR DEBUG ONLY
DebugManager_AddElement( 0xA000, wStatus );
///////////////////////////////////////////////////////////////////////////////

  // now determine the change
  if ( wStatus & OOB_IOCONN_MASK )
  {
  }
  else if ( wStatus & OOB_NETWORK_MASK )
  {
    // set a message
    tMessage.pszMessage = ( PC8 )szWlanNetStatus;
    tMessage.wMsgLength = strlen( szWlanNetStatus );
    tMessage.pvCallback = ProcessNetworkStatus;

    // post a status changed event
    QueueManager_PutTail( QUEUE_ENUM_WIFIMSGS, ( PU8 )&tMessage );
  }
  else if ( wStatus & OOB_TCPSRVCONN_MASK )
  {
  }
  else if ( wStatus & OOB_TCPSRVDATA_MASK )
  {
  }
  else if ( wStatus & OOB_UDPSRVDATA_MASK )
  {
  }
  else if (( wStatus & OOB_TCPCLNDATA_MASK ) || ( wStatus & OOB_UDPCLNDATA_MASK ))
  {
    // set a message
    tMessage.pszMessage = ( PC8 )szStreamPollAll;
    tMessage.wMsgLength = strlen( szStreamPollAll );
    tMessage.pvCallback = ProcessClientData;

    // post a status changed event
    QueueManager_PutTail( QUEUE_ENUM_WIFIMSGS, ( PU8 )&tMessage );
  }
}

/******************************************************************************
 * @function ProcessNetworkStatus
 *
 * @brief process the network status response
 *
 * This function will process the new client data
 *
 * @param[in]     eError      error
 *
 *****************************************************************************/
static void ProcessNetworkStatus( WIFIERROR eError, PVOID pvOption )
{
  // set the status
  bConnected = ( anRecvBuffer[ 0 ] == 2 ) ? TRUE : FALSE;
}

/******************************************************************************
 * @function ProcessClientData
 *
 * @brief process the client data response
 *
 * This function will process the new client data
 *
 * @param[in]     eError      error
 *
 *****************************************************************************/
static void ProcessClientData( WIFIERROR eError, PVOID pvOption )
{
}

/******************************************************************************
 * @function AscHex
 *
 * @brief asii to hex
 *
 * This function will convert an ascii character to hex value
 *
 * @param[in]   nChar   character value
 *
 * @return      hex value
 *
 *****************************************************************************/
static U8 AscHex( U8 nChar )
{
  U8 nValue = 0;

  // convert to value
  nValue = nChar -= '0';
  if ( nValue > 9 )
  {
    // adjust for A-9
    nValue -= 'A' - '9' - 1;
  }

  // return the value
  return( nValue );
}

/******************************************************************************
 * @function SendCommandMessage
 *
 * @brief send a command message
 *
 * This function format and send out the appropriate command message
 *
 * @param[in]   ptInitCmd   pointer to the message and option to send ouot
 *
 * @return      FALSE if no more entries
 *
 *****************************************************************************/
static BOOL SendCommandMessage( PINITCMDS ptInitCmd )
{
  BOOL  bStatus = TRUE;
  PC8   pszConfigValue;

  // test for done
  if ( ptInitCmd->pszCommand != NULL )
  {
    // process the command entry
    switch( ptInitCmd->eArgType )
    {
      case ARG_TYPE_STRING :
        wXmitBufIdx = sprintf(( PC8 )&anXmitBuffer[ wXmitBufIdx ], szSetCmdOpt, ptInitCmd->pszCommand, ptInitCmd->tArguments.pszOption );
        break;

      case ARG_TYPE_CONFIGENUM :
        pszConfigValue = GetConfigValue( ptInitCmd->tArguments.eConfigType );
        wXmitBufIdx = sprintf(( PC8 )&anXmitBuffer[ wXmitBufIdx ], szSetCmdOpt, ptInitCmd->pszCommand, pszConfigValue );
        break;

      default :
        break;
    }

    // output it
    OutputBuffer( anXmitBuffer, wXmitBufIdx );

    // start a timer
    TaskManager_StartTimer( TASK_SCHD_ENUM_WIFCTRL, RESP_TIMEOUT );
  }
  else
  {
    // set the return status to false
    bStatus = FALSE;
  }

  // return the status
  return( bStatus );
}

/******************************************************************************
 * @function SendGetCommand
 *
 * @brief send a general message
 *
 * This function format and send out the appropriate command message
 *
 * @param[in]   pszCommand  pointer to the command
 *
 *****************************************************************************/
static void SendGetCommand( PC8 pszCommand )
{
  // put the set into the buffer
  wXmitBufIdx = sprintf(( PC8 )anXmitBuffer, szGetCmd, pszCommand );

  // output it
  OutputBuffer( anXmitBuffer, wXmitBufIdx );

  // start a timer
  TaskManager_StartTimer( TASK_SCHD_ENUM_WIFCTRL, RESP_TIMEOUT );
}

/******************************************************************************
 * @function OutputBuffer
 *
 * @brief output the transmit buffer
 *
 * This function will output the transmit buffer
 *
 * @param[in]   pnBuffer    pointer to the buffer
 * @param[in]   wLength     length of the buffer
 *
 *****************************************************************************/
static void OutputBuffer( PU8 pnBuffer, U16 wLength )
{
  U16   wBytesWritten;

  while( wLength != 0 )
  {
    // write it/adjust index
    Uart_Write( UART_DEV_ENUM_WIFI, pnBuffer, wLength, &wBytesWritten );
    wLength -= wBytesWritten;
  }
}

/******************************************************************************
 * @function GetConfigValue
 *
 * @brief get a configuraiton entries value
 *
 * This function will return a pointer to a string for a config value
 *
 * @param[in]   eCfgType      config type
 *
 * @return      pointer to the string value
 *
 *****************************************************************************/
static PC8 GetConfigValue( CFGTYPE eCfgType )
{
  PC8 pszValue;

  // determine the type
  switch( eCfgType )
  {
    case CFG_TYPE_SSID :
      pszValue = ( PC8 )&tWifiConfigActl.acSSID;
      break;

    case CFG_TYPE_PASSKEY :
      pszValue = ( PC8 )&tWifiConfigActl.acPassword;
      break;

    default :
      pszValue = NULL;
  }

  // return the pointer
  return( pszValue );
 }

/**@} EOF WifiAMW006Handler.c */
