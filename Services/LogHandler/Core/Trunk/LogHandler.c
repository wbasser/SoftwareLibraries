/******************************************************************************
 * @file LogHandler.c
 *
 * @brief log handler implementation
 *
 * This file provides the implementation of the log handler
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
 * \addtogroup LogHandler
 * @{
 *****************************************************************************/

// local includes -------------------------------------------------------------
#include "LogHandler/LogHandler.h"
#include "LogHandler/LogHandler_prm.h"

// library includes -----------------------------------------------------------
#include "EepromHandler/Eepromhandler.h"

// Macros and Defines ---------------------------------------------------------
#define WRIDXADR_EEP_OFFSET   ( EEPROMHANDLER_LOGBLOCK_BASE_ADDR )
#define WRIDXCHK_EEP_OFFSET   ( WRIDXADR_EEP_OFFSET + sizeof( U16 ))
#define RDIDXADR_EEP_OFFSET   ( WRIDXCHK_EEP_OFFSET + sizeof( U16 ))
#define RDIDXCHK_EEP_OFFSET   ( RDIDXADR_EEP_OFFSET + sizeof( U16 ))
#define LOGDATA_EEP_OFFSET    ( RDIDXCHK_EEP_OFFSET + sizeof( U16 ))

// structures -----------------------------------------------------------------
/// define the log data structure
typedef struct PACKED _LOGDATA
{
  LOGTYPE   eType;
  U64       hSystemTimeUsec;
  U32       uArg1;
  U32       uArg2;
} LOGDATA, *PLOGDATA;
#define LOGDATA_SIZE    sizeof( LOGDATA )

// local parameter declarations -----------------------------------------------
static  U16   wCurIndex;
static  U16   wMaxNumEntries;
static  U16   wCurNumEntries;
#if ( LOGHANDLER_ENABLE_DEBUGCOMMANDS == 1 )
  static  PC8   pcLclBuf;
#endif // LOGHANDLER_ENABLE_DEBUGCOMMANDS

// local function prototypes --------------------------------------------------
#if ( LOGHANDLER_ENABLE_DEBUGCOMMANDS == 1 )
  static  ASCCMDSTS CmdDmpLog( U8 nCmdEnum );
  static  ASCCMDSTS CmdRstLog( U8 nCmdEnum );
#endif // LOGHANDLER_ENABLE_DEBUGCOMMANDS

// constant parameter initializations -----------------------------------------
#if ( LOGHANDLER_ENABLE_DEBUGCOMMANDS == 1 )
  /// declare the command strings
  static  const CODE C8 szDmpLog[ ]   = { "DLOG" };
  static  const CODE C8 szRstLog[ ]   = { "RLOG" };

  /// define the string constants
  static  const CODE C8 szNewLine[ ]  = { "\n\r>" };
  static  const CODE C8 szDispFmt[ ]  = { "Time:%08lX%08lX, Type:%02X, Arg1:%08lX, Arg2:%08lX, Desc:%s\n\r" };

  /// instantiate the command table
  const CODE ASCCMDENTRY g_atLogHandlerCmdHandlerTable[ ] =
  {
    ASCCMD_ENTRY( szDmpLog, 4, 1, ASCFLAG_COMPARE_NONE, 0, CmdDmpLog ),
    ASCCMD_ENTRY( szRstLog, 4, 0, ASCFLAG_COMPARE_NONE, 0, CmdRstLog ),

    // the entry below must be here
    ASCCMD_ENDTBL( )
  };

#endif // LOGHANDLER_ENABLE_DEBUGCOMMANDS

/******************************************************************************
 * @function LogHandler_Inititliaze
 *
 * @brief initialize the log handler
 *
 * This function checks to see if there is a valid log by checking the read/
 * write indices for proper values.  If not, it will reset the log to empty
 *
 *****************************************************************************/
void LogHandler_Initialize( void )
{
  U16 wRdIdx, wWrIdx, wTemp;
  
  // read the write index address/add the check
  EepromHandler_RdWord( WRIDXADR_EEP_OFFSET, &wWrIdx );
  EepromHandler_RdWord( WRIDXCHK_EEP_OFFSET, &wTemp );
  wWrIdx += wTemp;
  
  // read the read index address/add the check
  EepromHandler_RdWord( RDIDXADR_EEP_OFFSET, &wRdIdx );
  EepromHandler_RdWord( RDIDXCHK_EEP_OFFSET, &wTemp );
  wRdIdx += wTemp;
  
  // are the indices valid
  if (( wRdIdx != 0 ) || ( wWrIdx != 0 ))
  {
    // invalid log - clear all entries
    LogHandler_ClearAllEntries( );
  }

  // re-read the indices
  EepromHandler_RdWord( WRIDXADR_EEP_OFFSET, &wWrIdx );
  EepromHandler_RdWord( RDIDXADR_EEP_OFFSET, &wRdIdx );

  // check for not full
  if ( wRdIdx < wWrIdx )
  {
    // compute the count
    wCurNumEntries = wWrIdx - wRdIdx;
  }
  else
  {
    // return the max count
    wCurNumEntries = wMaxNumEntries;
  }

  // set the current index to the beginning
  EepromHandler_RdWord( RDIDXADR_EEP_OFFSET, &wCurIndex );
  
  // compute the number of entries
  wMaxNumEntries = ( EEPROMHANDLER_DEV_SIZE - WRIDXADR_EEP_OFFSET ) / LOGDATA_SIZE;

  #if ( LOGHANDLER_ENABLE_DEBUGCOMMANDS == 1 )
  pcLclBuf = NULL;
  #endif // LOGHANDLER_ENABLE_DEBUGCOMMANDS
}

/******************************************************************************
 * @function LogHandler_AddEntry
 *
 * @brief add an entry into the log
 *
 * This function will get the system time, and add the log to the end of the
 * log.  It will update indices as neccesary
 *
 * @param[in]   eType   log entry enumeration
 * @param[in]   uArg1   log entry argument #1
 * @param[in]   uArg2   log entry argument #2
 *
 & @return      TRUE if no errors, FALSE if illegal log entry type
 *
 *****************************************************************************/
BOOL LogHandler_AddEntry( LOGTYPE eType, U32 uArg1, U32 uArg2 )
{
  U16   wRdIdx, wWrIdx;
  BOOL  bStatus = FALSE;
  
  // is this a valid log entry type
  if ( eType < LOG_TYPE_MAX )
  {
    // create the entry
    LOGDATA tData =
    {
      .eType = eType,
      .hSystemTimeUsec = LogHandler_GetSystemTime( ),
      .uArg1 = uArg1,
      .uArg2 = uArg2
    };
    
    // get the current write index
    EepromHandler_RdWord( WRIDXADR_EEP_OFFSET, &wWrIdx );
    
    // write the block
    EepromHandler_WrBlock(( LOGDATA_EEP_OFFSET + ( wWrIdx * LOGDATA_SIZE )), LOGDATA_SIZE, ( PU8 )&tData );
    
    // adjust the index/store it
    wWrIdx++;
    wWrIdx %= wMaxNumEntries;
    EepromHandler_WrWord( WRIDXADR_EEP_OFFSET, wWrIdx );
    EepromHandler_WrWord( WRIDXCHK_EEP_OFFSET, ( ~wWrIdx + 1 ));
    
    // check for wrap around
    EepromHandler_RdWord( RDIDXADR_EEP_OFFSET, &wRdIdx );
    if ( wWrIdx == wRdIdx )
    {
      // increment the read index/store it
      wRdIdx++;
      EepromHandler_WrWord( RDIDXADR_EEP_OFFSET, wRdIdx );
      EepromHandler_WrWord( RDIDXCHK_EEP_OFFSET, ( ~wRdIdx + 1 ));
    }
    
    // check for not full
    if ( wRdIdx < wWrIdx )
    {
      // compute the count
      wCurNumEntries = wWrIdx - wRdIdx;
    }
    else
    {
      // return the max count
      wCurNumEntries = wMaxNumEntries;
    }

    // set good status
    bStatus = TRUE;
  }
  
  // return the status
  return( bStatus );
}

/******************************************************************************
 * @function LogHandler_GetEntry
 *
 * @brief get an entry from the log handler
 *
 * This function will get a value from the log handler based on position and
 * offset if an absolute position
 *
 * @param[in]   ePos    position type
 * @param[in]   peType  pointer to store the log type
 * @param[in]   puArg1  pointer to store the log argument #1
 * @param[in]   puArg2  pointer to store the log argument #2
 * @param[in]   phTime  pointer to store the log entry system time
 *
 * @return      TRUE if no errors, FALSE otherwise
 *
 *****************************************************************************/
BOOL LogHandler_GetEntry( LOGPOS ePos, PLOGTYPE peType, PU32 puArg1, PU32 puArg2, PU64 phTime )
{
  BOOL    bStatus = TRUE;
  LOGDATA tData;
  U16     wTemp;
  
  // determine the position
  switch( ePos )
  {
    case LOG_POS_OLDEST :
      // set the current index to the read index
      EepromHandler_RdWord( RDIDXADR_EEP_OFFSET, &wCurIndex );
      break;
      
    case LOG_POS_NEXT :
      // increment the index
      wCurIndex++;
      wCurIndex %= wMaxNumEntries;
      
      // check for none
      EepromHandler_RdWord( WRIDXADR_EEP_OFFSET, &wTemp );
      if ( wCurIndex == wTemp )
      {
        // end of records - set error
        bStatus = FALSE;
      }
      break;
      
    case LOG_POS_PREV :
      // derement the index
      if ( wCurIndex > 0 )
      {
        wCurIndex--;
      }
      else
      {
        // end of records - set error
        bStatus = FALSE;
      }
      break;
      
    case LOG_POS_NEWEST :
      // get the current write index
      EepromHandler_RdWord( WRIDXADR_EEP_OFFSET, &wCurIndex );
      
      // adjust to the previous
      if ( wCurIndex > 0 )
      {
        // decrement the current index
        wCurIndex--;
      }
      else
      {
        // set to the read index
        EepromHandler_RdWord( RDIDXADR_EEP_OFFSET, &wCurIndex );
      }
      break;
      
    default :
      break;
  }
  
  // check for OK
  if ( bStatus == TRUE )
  {
    // now read the entry
    EepromHandler_RdBlock( LOGDATA_EEP_OFFSET + ( wCurIndex * LOGDATA_SIZE ), LOGDATA_SIZE, ( PU8 )&tData );
    
    // copy the values
    *peType = tData.eType;
    *puArg1 = tData.uArg1;
    *puArg2 = tData.uArg2;
    *phTime = tData.hSystemTimeUsec;
  }
  
  // return the status
  return( bStatus );
}

/******************************************************************************
 * @function LogHandler_GetCount
 *
 * @brief get the number of entries in the log
 *
 * This function will return the number of entries in the log
 *
 * @return      the number of entries in the log
 *
 *****************************************************************************/
U16 LogHandler_GetCount( void )
{
  // return the count
  return( wCurNumEntries );
}

/******************************************************************************
 * @function LogHandler_GetDescription
 *
 * @brief get a log entries description
 *
 * This function will return the log entries description
 *
 * @param[in]   eLog    log entry type
 *
 * @return      
 *
 *****************************************************************************/
PC8 LogHandler_GetDescription( LOGTYPE eType )
{
  PC8 pszDescription = NULL;
  
  // is this a valid log entry type
  if ( eType < LOG_TYPE_MAX )
  {
    // get the description
    pszDescription = ( PC8 )PGM_RDWORD( atLogEntries[ eType - LOG_TYPE_INIT ].szDescription );
  }
  
  // return the description
  return( pszDescription );
}

/******************************************************************************
 * @function LogHandler_ClearAllEntries
 *
 * @brief clear all entries
 *
 * This function will reset the indices and create an initialization entry
 * in the log
 *
 *****************************************************************************/
void LogHandler_ClearAllEntries( void )
{
  U16 wIndex;
  
  // clear the indices
  wIndex = 0;
  
  // write the read index
  EepromHandler_WrWord( RDIDXADR_EEP_OFFSET, wIndex );
  EepromHandler_WrWord( RDIDXCHK_EEP_OFFSET, ~wIndex + 1 );
  
  // create an entry
  LOGDATA tData =
  {
    .eType = LOG_TYPE_INIT,
    .uArg1 = 0,
    .uArg2 = 0,
    .hSystemTimeUsec = LogHandler_GetSystemTime( ),
  };
  
  // write it
  EepromHandler_WrBlock( LOGDATA_EEP_OFFSET + ( wIndex * LOGDATA_SIZE ), LOGDATA_SIZE, ( PU8 )&tData );
  
  // adjust the index
  wIndex++;

  // write the write index
  EepromHandler_WrWord( WRIDXADR_EEP_OFFSET, wIndex );
  EepromHandler_WrWord( WRIDXCHK_EEP_OFFSET, ~wIndex + 1 );

  // set the count
  wCurNumEntries = 1;
}

#if ( LOGHANDLER_ENABLE_DEBUGCOMMANDS == 1 )
  /******************************************************************************
   * @function CmdDmpLog
   *
   * @brief dump the event log
   *
   * This function dumps the event log 
   *
   * @return  A
   *****************************************************************************/
  static ASCCMDSTS CmdDmpLog( U8 nCmdEnum )
  {
    U16     wCount;
    LOGPOS  eFirstPos, eNextPos;
    PC8     pcDescription;
    LOGTYPE eType;
    U32     uArg1, uArg2;
    U64UN   tTime;
    U32UN   tTemp;

    // fetch rhw buffer/get the argument
    AsciiCommandHandler_GetBuffer( nCmdEnum, &pcLclBuf );
    AsciiCommandHandler_GetValue( nCmdEnum, 0, &tTemp.uValue );
  
    // set the default next position
    eNextPos = LOG_POS_NEXT;
    
    // determine the mode
    switch( tTemp.anValue[ LE_U32_LSB_IDX ])
    {
      case LOG_DISPMODE_NEWEST_ONLY :
        eFirstPos = LOG_POS_NEWEST;
        wCount = 1;
        break;

      case LOG_DISPMODE_NEWEST_LAST16 :
        eFirstPos = LOG_POS_NEWEST;
        eNextPos = LOG_POS_PREV;
        wCount = MIN( wCurNumEntries, 16 );
        break;

      case LOG_DISPMODE_OLDEST_ONLY :
        eFirstPos = LOG_POS_OLDEST;
        wCount = 1;
        break;

      case LOG_DISPMODE_OLDEST_NEXT16 :
        eFirstPos = LOG_POS_OLDEST;
        eNextPos = LOG_POS_NEXT;
        wCount = MIN( wCurNumEntries, 16 );
        break;

      default :
      case LOG_DISPMODE_ALL :
        eFirstPos = LOG_POS_OLDEST;
        eNextPos = LOG_POS_NEXT;
        wCount = wCurNumEntries;
        break;
    }

    // loop
    while( wCount-- != 0 )
    {
      // get the entry/description
      LogHandler_GetEntry( eFirstPos, &eType, &uArg1, &uArg2, &tTime.hValue );
      pcDescription = LogHandler_GetDescription( eType );

      // now display it
      SPRINTF_P( pcLclBuf, ( char const *)szDispFmt, ( unsigned long )tTime.auValue[ LE_U64_MSU_IDX ], ( unsigned long )tTime.auValue[ LE_U64_LSU_IDX ], eType, ( unsigned long )uArg1, ( unsigned long )uArg2, pcDescription );
      AsciiCommandHandler_OutputBuffer( nCmdEnum );

      // adjust the position
      eFirstPos = eNextPos;
    }

    // new line
    AsciiCommandHandler_OutputString( nCmdEnum, ( PC8 )szNewLine );

    // return no error
    return( ASCCMD_STS_NONE );	
  }

  /******************************************************************************
   * @function CmdRstLog
   *
   * @brief reset the event log
   *
   * This function resets the event log 
   *
   * @return  A
   *****************************************************************************/
  static ASCCMDSTS CmdRstLog( U8 nCmdEnum )
  {
    // dump the debug data
    LogHandler_ClearAllEntries( );
    
    // return no error
    return( ASCCMD_STS_NONE );	
  }
#endif // LOGHANDLER_ENABLE_DEBUGCOMMANDS

/**@} EOF LogHandler.c */
