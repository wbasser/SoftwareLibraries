/******************************************************************************
 * @file BinCommonCommands.c
 *
 * @brief 
 *
 * This file 
 *
 * @copyright Copyright (c) 2012 CyberIntegration
 * This document contains proprietary data and information of CyberIntegration 
 * LLC. It is the exclusive property of CyberIntegration, LLC and will not be 
 * disclosed in any form to any party without prior written permission of 
 * CyberIntegration, LLC. This document may not be reproduced or further used 
 * without the prior written permission of CyberIntegration, LLC.
 *
 * $Date: $
 *
 * Version History
 * ======
 * $Rev: $
 * 
 *
 * \addtogroup BinCommonCommands
 * @{
 *****************************************************************************/

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "BinCommonCommands/BinCommonCommands.h"

// library includes -----------------------------------------------------------
#include "BinaryCommandHandler/BinaryCommandHandler.h"
#include "ManufInfo/ManufInfo.h"
#include "LogHandler/LogHandler.h"
#include "ConfigManager/ConfigManager.h"
#include "SystemMonitor/SystemMonitor.h"

// Macros and Defines ---------------------------------------------------------
#define BINCMN_ERR_NOTIMPLEMENTED       ( 0xFF )
#define BINCMN_ERR_ILLEGALPASSWORD      ( 0xFE )
#define BINCMD_ERR_ILLEGALLOGENTRY      ( 0xFD )

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations ----------------------------------------------

// local parameter declarations -----------------------------------------------

// local function prototypes --------------------------------------------------
static  void  SendAutoResponse( U8 nSubCmd, PU8 pnBuffer, U16 wLength );

// local slave command handlers 
static  BINPARSESTS CmdGetPng( U8 nCmdEnum );
static  BINPARSESTS CmdGetRev( U8 nCmdEnum );
static  BINPARSESTS CmdSysMon( U8 nCmdEnum );
static  BINPARSESTS CmdRstDfl( U8 nCmdEnum );
static  BINPARSESTS CmdLogNum( U8 nCmdEnum );
static  BINPARSESTS CmdLogGet( U8 nCmdEnum );
static  BINPARSESTS CmdLogRst( U8 nCmdEnum );
static  BINPARSESTS CmdBootEn( U8 nCmdEnum );

// constant parameter initializations -----------------------------------------
const CODE BINCMDSLVENTRY atBinCommonCommandsTable[ ] =
{
  BINCMD_SLV_INTENTRY( BINCMN_CMD_GETPNG, 0, -1, BINCOMP_FLAG_NONE, 0, CmdGetPng ),
  BINCMD_SLV_INTENTRY( BINCMN_CMD_GETREV, 0, -1, BINCOMP_FLAG_NONE, 0, CmdGetRev ),
  BINCMD_SLV_INTENTRY( BINCMN_CMD_SYSMON, 0, -1, BINCOMP_FLAG_NONE, 0, CmdSysMon ),
  BINCMD_SLV_INTENTRY( BINCMN_CMD_RSTDFL, 0, -1, BINCOMP_FLAG_NONE, 0, CmdRstDfl ),
  BINCMD_SLV_INTENTRY( BINCMN_CMD_LOGNUM, 0, -1, BINCOMP_FLAG_NONE, 0, CmdLogNum ),
  BINCMD_SLV_INTENTRY( BINCMD_CMD_LOGOLD, 0, -1, BINCOMP_FLAG_NONE, 0, CmdLogGet ),
  BINCMD_SLV_INTENTRY( BINCMD_CMD_LOGRST, 0, -1, BINCOMP_FLAG_NONE, 0, CmdLogRst ),
  BINCMD_SLV_INTENTRY( BINCMD_CMD_BOOTEN, 0, -1, BINCOMP_FLAG_NONE, 0, CmdBootEn ),

  // do not remove below entry
  BINCMD_SLV_ENDENTRY( ) // end of table
};

/******************************************************************************
 * @function SendAutoResponse
 *
 * @brief respond with data for the system monitor
 *
 * This responds with data for the system monitor
 *
 * @param[in]  nSubCmd
 * @param[in]  pnBuffer
 * @param[in]  wLength
 *
 *****************************************************************************/
static void SendAutoResponse( U8 nSubCmd, PU8 pnBuffer, U16 wLength )
{
  // begin the messsage/fill the buffer/send the message
  BinaryCommandHandler_SetMessageBlock( BINCMD_ENUM_DBGCMD, pnBuffer, wLength );
  BinaryCommandHandler_SendMessage( BINCMD_ENUM_DBGCMD );
}

/******************************************************************************
 * @function CmdQryPng
 *
 * @brief ping
 *
 * This function will respond with an ACK
 *
 * @return      appropriate protocol status/error
 *
 *****************************************************************************/
static BINPARSESTS CmdGetPng( U8 nCmdEnum )
{
  // send an ACK
  return( BINPARSE_STS_SND_ACK );
}

/******************************************************************************
 * @function CmdQryVer
 *
 * @brief respond with the software version
 *
 * This function respond with the software title and version
 *
 * @return      appropriate protocol status/error
 *
 *****************************************************************************/
static BINPARSESTS CmdGetRev( U8 nCmdEnum )
{
  PC8  pszTemp;

  // now set the major/minor/
  BinaryCommandHandler_SetMessageByte( nCmdEnum, ManufInfo_GetHdwMajor( ));
  BinaryCommandHandler_SetMessageByte( nCmdEnum, ManufInfo_GetHdwMinor( ));
  BinaryCommandHandler_SetMessageByte( nCmdEnum, ManufInfo_GetSfwMajor( ));
  BinaryCommandHandler_SetMessageByte( nCmdEnum, ManufInfo_GetSfwMinor( ));
  
  // copy the title
  pszTemp = ManufInfo_GetTitle( );
  BinaryCommandHandler_SetMessageBlock( nCmdEnum, ( PU8 )pszTemp, strlen( pszTemp ));
  
  // copy the title
  pszTemp = ManufInfo_GetPartNum2( );
  BinaryCommandHandler_SetMessageBlock( nCmdEnum, ( PU8 )pszTemp, strlen( pszTemp ));

  // now force a message to be sent
  return( BINPARSE_STS_SND_RESP );
}

/******************************************************************************
 * @function CmdSysMon
 *
 * @brief   system monitor command interfae
 *
 * This function will call the system monitor command interface
 *
 * @return      appropriate protocol status/error
 *
 *****************************************************************************/
static BINPARSESTS CmdSysMon( U8 nCmdEnum )
{
  BINPARSESTS eRespStatus = BINPARSE_STS_NONE;
  SYSMONERRS  eSysMonStatus;
  U8          nSubCommand;
  U16         wRcvBufLength, wXmtBufLength;
  PU8         pnRcvBuffer;
  U8          anLclBuffer[ sizeof( U32 ) ];

  // get the arguments
  BinaryCommandHandler_GetOption( nCmdEnum, &nSubCommand );
  BinaryCommandHandler_GetRcvBufferPointer( nCmdEnum, &pnRcvBuffer );
  BinaryCommandHandler_GetRcvLength( nCmdEnum, &wRcvBufLength );
  BinaryCommandHandler_GetXmtBufferSize( nCmdEnum, &wXmtBufLength );

  // process the command
  eSysMonStatus = SystemMonitor_ProcessCommand( nSubCommand, pnRcvBuffer, wRcvBufLength, anLclBuffer, &wXmtBufLength, SendAutoResponse );

  // now process the status
  switch( eSysMonStatus )
  {
    case SYSMON_ERRS_ILLCMD :
    case SYSMON_ERRS_ILLENUM :
    case SYSMON_ERRS_ILLLENGTH :
    case SYSMON_ERRS_FRAMEOVF :
    BinaryCommandHandler_SetOption( nCmdEnum, eSysMonStatus );
    eRespStatus = BINPARSE_STS_SND_NAK;
    break;

    case SYSMON_ERRS_SENDRESP :
    // set the length of the transmit
    BinaryCommandHandler_SetMessageBlock( nCmdEnum, anLclBuffer, wXmtBufLength );
    eRespStatus = BINPARSE_STS_SND_RESP;
    break;

    default :
    eRespStatus = BINPARSE_STS_SND_ACK;
    break;
  }

  // return the parse status
  return( eRespStatus );
}

/******************************************************************************
 * @function CmdRstDfl
 *
 * @brief     reset all configuration to their default value
 *
 * This function will reset all configurations to their defualt value
 *
 * @return      appropriate protocol status/error
 *
 *****************************************************************************/
static BINPARSESTS CmdRstDfl( U8 nCmdEnum )
{
  BINPARSESTS eStatus = BINPARSE_STS_SND_ACK;
  U32         uPassword;
  
  // get the password/compare
  BinaryCommandHandler_GetRcvBufferPointer( nCmdEnum, (PU8*)&uPassword );
  if ( uPassword == SYSTEMDEFINE_CONFIG_RESET_DEFAULT )
  {
    // reset the configuration to defaults
    ConfigManager_ResetDefaults( );
  }
  else
  {
    // set the error
    BinaryCommandHandler_SetOption( nCmdEnum, BINCMN_ERR_ILLEGALPASSWORD );
    eStatus = BINPARSE_STS_SND_NAK;
  }
  
  // return the status
  return( eStatus );
}

/******************************************************************************
 * @function  CmdLogNum
 *
 * @brief   get the number of log entries
 *
 * This function will return the numbe of log entries
 *
 * @return      appropriate protocol status/error
 *
 *****************************************************************************/
static BINPARSESTS CmdLogNum( U8 nCmdEnum )
{
  BINPARSESTS eStatus = BINPARSE_STS_SND_RESP;
  U16UN       tNumLogEntries;
  
  // get the number of log entries
  tNumLogEntries.wValue = LogHandler_GetCount( );
  
  // stuff in buffer
  BinaryCommandHandler_SetMessageByte( nCmdEnum, tNumLogEntries.anValue[ LE_U16_LSB_IDX ] );
  BinaryCommandHandler_SetMessageByte( nCmdEnum, tNumLogEntries.anValue[ LE_U16_MSB_IDX ] );
  
  // return the status
  return( eStatus );
}

/******************************************************************************
 * @function  CmdLogGet
 *
 * @brief   get a log entry
 *
 * This function will return a log entry
 *
 * @return      appropriate protocol status/error
 *
 *****************************************************************************/
static BINPARSESTS CmdLogGet( U8 nCmdEnum )
{
  BINPARSESTS eStatus = BINPARSE_STS_SND_NAK;
  LOGPOS      eLogPosition;
  LOGTYPE     eLogType;
  U32UN       tLogArg1;
  U32UN       tLogArg2;
  U64UN       tLogTime;
  
  // get the log position
  BinaryCommandHandler_GetOption( nCmdEnum, ( PU8 )&eLogPosition );
  
  // get the log entry
  if ( LogHandler_GetEntry( eLogPosition, &eLogType, &tLogArg1.uValue, &tLogArg2.uValue, &tLogTime.hValue ) == TRUE )
  {
    // valid log entry - stuff
    BinaryCommandHandler_SetMessageByte( nCmdEnum, ( U8 )eLogType );  
    BinaryCommandHandler_SetMessageByte( nCmdEnum, tLogArg1.anValue[ LE_U32_LSB_IDX ] );
    BinaryCommandHandler_SetMessageByte( nCmdEnum, tLogArg1.anValue[ LE_U32_MS1_IDX ] );
    BinaryCommandHandler_SetMessageByte( nCmdEnum, tLogArg1.anValue[ LE_U32_MS2_IDX ] );
    BinaryCommandHandler_SetMessageByte( nCmdEnum, tLogArg1.anValue[ LE_U32_MSB_IDX ] );
    BinaryCommandHandler_SetMessageByte( nCmdEnum, tLogArg2.anValue[ LE_U32_LSB_IDX ] );
    BinaryCommandHandler_SetMessageByte( nCmdEnum, tLogArg2.anValue[ LE_U32_MS1_IDX ] );
    BinaryCommandHandler_SetMessageByte( nCmdEnum, tLogArg2.anValue[ LE_U32_MS2_IDX ] );
    BinaryCommandHandler_SetMessageByte( nCmdEnum, tLogArg2.anValue[ LE_U32_MSB_IDX ] );
    BinaryCommandHandler_SetMessageByte( nCmdEnum, tLogTime.anValue[ LE_U64_LSB_IDX ] );  
    BinaryCommandHandler_SetMessageByte( nCmdEnum, tLogTime.anValue[ LE_U64_MS1_IDX ] );
    BinaryCommandHandler_SetMessageByte( nCmdEnum, tLogTime.anValue[ LE_U64_MS2_IDX ] );
    BinaryCommandHandler_SetMessageByte( nCmdEnum, tLogTime.anValue[ LE_U64_MS3_IDX ] );
    BinaryCommandHandler_SetMessageByte( nCmdEnum, tLogTime.anValue[ LE_U64_MS4_IDX ] );
    BinaryCommandHandler_SetMessageByte( nCmdEnum, tLogTime.anValue[ LE_U64_MS5_IDX ] );
    BinaryCommandHandler_SetMessageByte( nCmdEnum, tLogTime.anValue[ LE_U64_MS6_IDX ] );
    BinaryCommandHandler_SetMessageByte( nCmdEnum, tLogTime.anValue[ LE_U64_MSB_IDX ] );
    
    // set the good status
    eStatus = BINPARSE_STS_SND_RESP;
  }
  else
  {
    // set the error
    BinaryCommandHandler_SetOption( nCmdEnum, BINCMD_ERR_ILLEGALLOGENTRY );
  }
  
  // return the status
  return( eStatus );
}

/******************************************************************************
 * @function CmdLogRst
 *
 * @brief   reset the log
 *
 * This function will reset the log
 *
 * @return      appropriate protocol status/error
 *
 *****************************************************************************/
static BINPARSESTS CmdLogRst( U8 nCmdEnum )
{
  BINPARSESTS eStatus = BINPARSE_STS_SND_ACK;
  U32         uPassword;
  
  // get the password/compare
  BinaryCommandHandler_GetRcvBufferPointer( nCmdEnum, (PU8*)&uPassword );
  if ( uPassword == SYSTEMDEFINE_LOGENTRIES_RESET )
  {
    // reset the configuration to defaults
    LogHandler_ClearAllEntries( );
  }
  else
  {
    // set the error
    BinaryCommandHandler_SetOption( nCmdEnum, BINCMN_ERR_ILLEGALPASSWORD );
    eStatus = BINPARSE_STS_SND_NAK;
  }
  
  // return the status
  return( eStatus );
}

/******************************************************************************
 * @function CmdBootEn
 *
 * @brief   enter the boot mode
 *
 * This function  will enter the boot mode
 *
 * @return      appropriate protocol status/error
 *
 *****************************************************************************/
static BINPARSESTS CmdBootEn( U8 nCmdEnum )
{
  BINPARSESTS eStatus = BINPARSE_STS_SND_NAK;
  
  // set the error code for now
  BinaryCommandHandler_SetOption( nCmdEnum, BINCMN_ERR_NOTIMPLEMENTED );

  // return the status
  return( eStatus );
}

/**@} EOF BinCommonCommands.c */
