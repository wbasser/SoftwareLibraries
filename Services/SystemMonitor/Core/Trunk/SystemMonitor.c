/******************************************************************************
 * @file SystemMonitor.c
 *
 * @brief System monitor implementation
 *
 * This file provides the system monitor implemenation routines
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
 * \addtogroup SystemMonitor
 * @{
 *****************************************************************************/

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "SystemMonitor/SystemMonitor.h"

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------
/// define the size of the local buffer
#define LCL_BUF_SIZE     ( SYSMON_ENUM_MAX * sizeof( U32 ))

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------
/// define the control structure
typedef struct _SYSMONCTL
{
  BOOL  bEnabled;
} SYSMONCTL, *PSYSMONCTL;
#define SYSMONCTL_SIZE      sizeof( SYSMONCTL )

// local parameter declarations -----------------------------------------------
static  SYSMONCTL         atCtls[ SYSMON_ENUM_MAX ];
static  U8                anLclBuffer[ LCL_BUF_SIZE ];
static  U16               wLclBufIndex;
static  PVSYSMONTRANSMIT  pvLclTransmitFunc;
static  U8                nLclCmdEnum;
static  BOOL              bValidAutoSetup;

// local function protoENTTYPEs --------------------------------------------------
static  SYSMONERRS  ProcessSetup( PU8 pnRcvBuffer, U16 wLength );
static  void        StuffEntryBuffer( SYSMONENUM eEntry, SYSMONENTTYPE eEntType, U32UN tValue );
static  SYSMONERRS  GetValue( PU8 pnRcvBuffer, U16 wRcvLength, PU8 pnXmtBuffer, PU16 pwXmtLength );
static  SYSMONERRS  PutValue( PU8 pnRcvBuffer, U16 wLength );

/******************************************************************************
 * @function SystemMonitor_Initialize
 *
 * @brief initialization
 *
 * This function will initialize the system monitor 
 *
 *****************************************************************************/
void SystemMonitor_Initialize( void )
{
  // set all enables to false
  memset( atCtls, 0, ( SYSMONCTL_SIZE * SYSMON_ENUM_MAX ));

  // clear the transmit function
  pvLclTransmitFunc = NULL;

  // clear the valid auto setup
  bValidAutoSetup = FALSE;

  // call the local iniailization
  SystemMonitor_LocalInitialize( );
}

/******************************************************************************
 * @function SystemMonitor_ProcessCommand
 *
 * @brief process a command
 *
 * This function will determine the command and respond accordingly
 *
 * @param[in]   nCommand      command to be processed
 * @param[io]   pnRcvBuffer   pointer to the receive buffer
 * @param[in]   wRcvLength    length of the receive buffer
 * @param[io]   pnXmtBuffer   pointer to the transmit buffer
 * @param[io]   pwXmtLength   pointer to the transmit buffer length, maximuum 
 *                            number of bytes on input, length of transmitted
 *                            message on exit
 * @param[in]   pvTransmitFunc  pointer to the transmit function to use during auto transmit
 *                              only needs to be valid when a start command is issued
 * @param[in]   nCmdEnum      command handler enumeration
 *
 * @return      appropriate error
 *
 *****************************************************************************/
SYSMONERRS SystemMonitor_ProcessCommand( U8 nCommand, PU8 pnRcvBuffer, U16 wRcvLength, PU8 pnXmtBuffer, PU16 pwXmtLength, PVSYSMONTRANSMIT pvTransmitFunc, U8 nCmdEnum )
{
  SYSMONERRS eError = SYSMON_ERRS_NONE;
  
  // determine the command
  switch( nCommand )
  {
    case SYSMON_CMDS_SETUP :
      // process the startup
      if (( eError = ProcessSetup( pnRcvBuffer, wRcvLength )) == SYSMON_ERRS_NONE )
      {
        // set the valid flag
        bValidAutoSetup = TRUE;
      }
      else
      {
        // clear the valid flag
        bValidAutoSetup = FALSE;
      }
      break;
      
    case SYSMON_CMDS_START :
      // save the transmit function
      pvLclTransmitFunc = pvTransmitFunc;
      nLclCmdEnum = nCmdEnum;

      // now check for valid auto poll and a transmit function
      if (( pvLclTransmitFunc != NULL) && ( bValidAutoSetup ))
      {
        // start the time
        SystemMonitor_EnableDisableTask( ON );
      }
      else
      {
        // set the error
        eError = SYSMON_ERRS_ILLSETUP;
      }
      break;
      
    case SYSMON_CMDS_STOP :
      // stop the timer
      SystemMonitor_EnableDisableTask( OFF );
      break;
      
    case SYSMON_CMDS_GET :
      // get a value/set the length
      eError = GetValue( pnRcvBuffer, wRcvLength, pnXmtBuffer, pwXmtLength );
      break;
      
    case SYSMON_CMDS_PUT :
      // put a value
      eError = PutValue( pnRcvBuffer, wRcvLength );
      break;
      
    default :
      // return the error
      eError = SYSMON_ERRS_ILLCMD;
      break;
  }
  
  // return the error
  return( eError );
}

/******************************************************************************
 * @function SystemMonitor_ProcessTransmit
 *
 * @brief transmit event handler
 *
 * This function will process the transmit handler event
 *
 *****************************************************************************/
void SystemMonitor_ProcessTransmit( void )
{
  SYSMONENUM          eEntry;
  SYSMONENTTYPE       eEntType;
  PSYSMONCTL          ptCtl;
  PSYSMONENTDEF       ptDef;
  U32UN               tValue;
  PVSYSMONGETU8       pvGetU8;
  PVSYSMONGETS8       pvGetS8;
  PVSYSMONGETU16      pvGetU16;
  PVSYSMONGETS16      pvGetS16;
  PVSYSMONGETU32      pvGetU32;
  PVSYSMONGETS32      pvGetS32;
  PVSYSMONGETFLOAT    pvGetFloat;
  PVSYSMONGETOPTU8    pvGetOptU8;
  PVSYSMONGETOPTS8    pvGetOptS8;
  PVSYSMONGETOPTU16   pvGetOptU16;
  PVSYSMONGETOPTS16   pvGetOptS16;
  PVSYSMONGETOPTU32   pvGetOptU32;
  PVSYSMONGETOPTS32   pvGetOptS32;
  PVSYSMONGETOPTFLOAT pvGetOptFloat;

  // clear the index
  wLclBufIndex = 0;

  // for each entry
  for ( eEntry = 0; eEntry < SYSMON_ENUM_MAX; eEntry++ )
  {
    // get a pointer to the control/def structures
    ptDef = ( PSYSMONENTDEF )&atSysMonDefs[ eEntry ];
    ptCtl = &atCtls[ eEntry ];
    
    // is this entry enabled
    if ( ptCtl->bEnabled )
    {
      // get the entry type
      eEntType = PGM_RDBYTE( ptDef->eType );
      
      // compute the size
      switch( eEntType )
      {
        case SYSMON_ENTTYPE_U8 :
          pvGetU8 = PGM_RDDWRD( ptDef->pvGetFuncs.pvGetU8 );
          tValue.anValue[ LE_U32_LSB_IDX ] = pvGetU8( );
          break;

        case SYSMON_ENTTYPE_S8 :
          pvGetS8 = PGM_RDDWRD( ptDef->pvGetFuncs.pvGetS8 );
          tValue.anValue[ LE_U32_LSB_IDX ] = pvGetS8( );
          break;
          
        case SYSMON_ENTTYPE_U16 :
          pvGetU16 = PGM_RDDWRD( ptDef->pvGetFuncs.pvGetU16 );
          tValue.awValue[ LE_U32_LSW_IDX ] = pvGetU16( );
          break;

        case SYSMON_ENTTYPE_S16 :
          pvGetS16 = PGM_RDDWRD( ptDef->pvGetFuncs.pvGetS16 );
          tValue.awValue[ LE_U32_LSW_IDX ] = pvGetS16( );
          break;
          
        case SYSMON_ENTTYPE_U32 :
          pvGetU32 = PGM_RDDWRD( ptDef->pvGetFuncs.pvGetU32 );
          tValue.uValue = pvGetU32( );
          break;

        case SYSMON_ENTTYPE_S32 :
          pvGetS32 = PGM_RDDWRD( ptDef->pvGetFuncs.pvGetS32 );
          tValue.uValue = pvGetS32( );
          break;

        case SYSMON_ENTTYPE_FLOAT :
          pvGetFloat = PGM_RDDWRD( ptDef->pvGetFuncs.pvGetFloat );
          tValue.fValue = pvGetFloat( );
          break;
          
        case SYSMON_ENTTYPE_OPT_U8 :
          pvGetOptU8 = PGM_RDDWRD( ptDef->pvGetFuncs.pvGetOptU8 );
          tValue.anValue[ LE_U32_LSB_IDX ] = pvGetOptU8( PGM_RDDWRD( ptDef->uOption ));
          break;

        case SYSMON_ENTTYPE_OPT_S8 :
          pvGetOptS8 = PGM_RDDWRD( ptDef->pvGetFuncs.pvGetOptS8 );
          tValue.anValue[ LE_U32_LSB_IDX ] = pvGetOptS8( PGM_RDDWRD( ptDef->uOption ));
          break;
          
        case SYSMON_ENTTYPE_OPT_U16 :
          pvGetOptU16 = PGM_RDDWRD( ptDef->pvGetFuncs.pvGetOptU16 );
          tValue.awValue[ LE_U32_LSW_IDX ] = pvGetOptU16( PGM_RDDWRD( ptDef->uOption ));
          break;

        case SYSMON_ENTTYPE_OPT_S16 :
          pvGetOptS16 = PGM_RDDWRD( ptDef->pvGetFuncs.pvGetOptS16 );
          tValue.awValue[ LE_U32_LSW_IDX ] = pvGetOptS16( PGM_RDDWRD( ptDef->uOption ));
          break;
          
        case SYSMON_ENTTYPE_OPT_U32 :
          pvGetOptU32 = PGM_RDDWRD( ptDef->pvGetFuncs.pvGetOptU32 );
          tValue.uValue = pvGetOptU32( PGM_RDDWRD( ptDef->uOption ));
          break;

        case SYSMON_ENTTYPE_OPT_S32 :
          pvGetOptS32 = PGM_RDDWRD( ptDef->pvGetFuncs.pvGetOptS32 );
          tValue.uValue = pvGetOptS32( PGM_RDDWRD( ptDef->uOption ));
          break;

        case SYSMON_ENTTYPE_OPT_FLOAT :
          pvGetOptFloat = PGM_RDDWRD( ptDef->pvGetFuncs.pvGetOptFloat );
          tValue.fValue = pvGetOptFloat( PGM_RDDWRD( ptDef->uOption ));
          break;
          
        default :
          break;
      }
      
      // stuff in buffer
      StuffEntryBuffer( eEntry, eEntType, tValue );
    }
  }
  
  // now transmit the buffer
  pvLclTransmitFunc( SYSMON_CMDS_RESPONSE, anLclBuffer, wLclBufIndex, nLclCmdEnum );
}

/******************************************************************************
 * @function ProcessSetup
 *
 * @brief process a setup command
 *
 * This function will check for valid entries and proper frame size
 *
 * @param[in]   pnMessage   pointer to the message content
 * @param[in]   nLength     length of the message content
 *
 * @return      appropriate error
 *
 *****************************************************************************/
static SYSMONERRS ProcessSetup( PU8 pnRcvBuffer, U16 wLength )
{
  SYSMONERRS    eError = SYSMON_ERRS_NONE;
  U16           wSize = 0;
  U16           wBufIdx;
  SYSMONENUM    eEntry;
  SYSMONENTTYPE eEntType;
  PSYSMONCTL    ptCtl;
  PSYSMONENTDEF ptDef;
  U16UN         tPollTime;

  // set all enables to false
  memset( atCtls, 0, ( SYSMONCTL_SIZE * SYSMON_ENUM_MAX ));

  // get the speed
  tPollTime.anValue[ LE_U16_LSB_IDX ] = *( pnRcvBuffer + 0 );
  tPollTime.anValue[ LE_U16_MSB_IDX ] = *( pnRcvBuffer + 1 );
  
  // now store it
  SystemMonitor_SetScanTaskTime( tPollTime.wValue );
  
  // now process each entry in the buffer
  for( wBufIdx = 2; wBufIdx < wLength; wBufIdx++ )
  {
    // check for a valid enumeration
    if (( eEntry = *( pnRcvBuffer + wBufIdx )) < SYSMON_ENUM_MAX )
    {
      // get a pointer to the control/def structures
      ptDef = ( PSYSMONENTDEF )&atSysMonDefs[ eEntry ];
      ptCtl = &atCtls[ eEntry ];
      
      // set the enable to true
      ptCtl->bEnabled = TRUE;
      
      // get the ENTTYPE
      eEntType = PGM_RDBYTE( ptDef->eType );
      
      // compute the size
      switch( eEntType )
      {
        case SYSMON_ENTTYPE_U8 :
        case SYSMON_ENTTYPE_S8 :
        case SYSMON_ENTTYPE_OPT_U8 :
        case SYSMON_ENTTYPE_OPT_S8 :
          wSize += 1;
          break;
          
        case SYSMON_ENTTYPE_U16 :
        case SYSMON_ENTTYPE_S16 :
        case SYSMON_ENTTYPE_OPT_U16 :
        case SYSMON_ENTTYPE_OPT_S16 :
          wSize += 2;
          break;
          
        case SYSMON_ENTTYPE_U32 :
        case SYSMON_ENTTYPE_S32 :
        case SYSMON_ENTTYPE_FLOAT :
        case SYSMON_ENTTYPE_OPT_U32 :
        case SYSMON_ENTTYPE_OPT_S32 :
        case SYSMON_ENTTYPE_OPT_FLOAT :
          wSize += 4;
          break;
          
        default :
          break;
      }
    }
    else
    {
      // set the error and break
      eError = SYSMON_ERRS_ILLENUM;
      break;
    }
  }
  
  // now determine if there is enough time to transmit this frame
  if ( wSize >= ( SYSTEMMONITOR_TRANSMIT_BYTE_RATE / 10 ))
  {
    // flag the error
    eError = SYSMON_ERRS_FRAMEOVF;
    
    // clear the enables
    memset( atCtls, 0, ( SYSMONCTL_SIZE * SYSMON_ENUM_MAX ));
  }
  
  // return the error
  return( eError );
}

/******************************************************************************
 * @function StuffEntryBuffer
 *
 * @brief stuff an entry into the buffer
 *
 * This function will add an entry into the buffer
 *
 * @param[in]   eEntry      eEntry enumeration
 * @param[in]   eEntType       ENTTYPE of entry
 * @param[in]   tValue      value to stuff
 *
 *****************************************************************************/
static void StuffEntryBuffer( SYSMONENUM eEntry, SYSMONENTTYPE eEntType, U32UN tValue )
{
  // check for valid size
  if ( wLclBufIndex < LCL_BUF_SIZE )
  {
    // stuff the entry
    anLclBuffer[ wLclBufIndex++ ] = eEntry;
    
    // now determine the ENTTYPE
    switch( eEntType )
    {
      case SYSMON_ENTTYPE_U8 :
      case SYSMON_ENTTYPE_S8 :
      case SYSMON_ENTTYPE_OPT_U8 :
      case SYSMON_ENTTYPE_OPT_S8 :
        anLclBuffer[ wLclBufIndex++ ] = tValue.anValue[ LE_U32_LSB_IDX ];
        break;
        
      case SYSMON_ENTTYPE_U16 :
      case SYSMON_ENTTYPE_S16 :
      case SYSMON_ENTTYPE_OPT_U16 :
      case SYSMON_ENTTYPE_OPT_S16 :
        anLclBuffer[ wLclBufIndex++ ] = tValue.anValue[ LE_U32_LSB_IDX ];
        anLclBuffer[ wLclBufIndex++ ] = tValue.anValue[ LE_U32_MS1_IDX ];
        break;
        
      case SYSMON_ENTTYPE_U32 :
      case SYSMON_ENTTYPE_S32 :
      case SYSMON_ENTTYPE_FLOAT :
      case SYSMON_ENTTYPE_OPT_U32 :
      case SYSMON_ENTTYPE_OPT_S32 :
      case SYSMON_ENTTYPE_OPT_FLOAT :
        anLclBuffer[ wLclBufIndex++ ] = tValue.anValue[ LE_U32_LSB_IDX ];
        anLclBuffer[ wLclBufIndex++ ] = tValue.anValue[ LE_U32_MS1_IDX ];
        anLclBuffer[ wLclBufIndex++ ] = tValue.anValue[ LE_U32_MS2_IDX ];
        anLclBuffer[ wLclBufIndex++ ] = tValue.anValue[ LE_U32_MSB_IDX ];
        break;
        
      default :
        break;
    }
  }
}

/******************************************************************************
 * @function GetValue
 *
 * @brief get a value 
 *
 * This function will check for valid arguments and stuff the value
 * into the buffer, set the appropriate length
 *
 * @param[in]   pnMessage   pointer to the message content
 * @param[io]   pnLength    pointer to the length of the message content
 *
 * @return      appropriate error
 *
 *****************************************************************************/
static SYSMONERRS GetValue( PU8 pnRcvBuffer, U16 wRcvLength, PU8 pnXmtBuffer, PU16 pwXmtLength )
{
  SYSMONERRS          eError = SYSMON_ERRS_NONE;
  SYSMONENUM          eEntry;
  SYSMONENTTYPE       eEntType;
  PSYSMONENTDEF       ptDef;
  U32UN               tValue;
  PVSYSMONGETU8       pvGetU8;
  PVSYSMONGETS8       pvGetS8;
  PVSYSMONGETU16      pvGetU16;
  PVSYSMONGETS16      pvGetS16;
  PVSYSMONGETU32      pvGetU32;
  PVSYSMONGETS32      pvGetS32;
  PVSYSMONGETFLOAT    pvGetFloat;
  PVSYSMONGETOPTU8    pvGetOptU8;
  PVSYSMONGETOPTS8    pvGetOptS8;
  PVSYSMONGETOPTU16   pvGetOptU16;
  PVSYSMONGETOPTS16   pvGetOptS16;
  PVSYSMONGETOPTU32   pvGetOptU32;
  PVSYSMONGETOPTS32   pvGetOptS32;
  PVSYSMONGETOPTFLOAT pvGetOptFloat;
  U16                 wRcvBufIdx, wXmtBufIdx;
  U8                  nLength, nValIdx;

  // clear the transmit length
  wXmtBufIdx = 0;
  
  // now process each entry in the buffer
  for( wRcvBufIdx = 0; wRcvBufIdx < wRcvLength; wRcvBufIdx++ )
  {
    // check for a valid enumeration
    if (( eEntry = *( pnRcvBuffer + wRcvBufIdx )) < SYSMON_ENUM_MAX )
    {
      // get a pointer to the definition
      ptDef = ( PSYSMONENTDEF )&atSysMonDefs[ eEntry ];
    
      // now get the ENTTYPE
      eEntType = PGM_RDBYTE( ptDef->eType );
    
      // compute the size
      switch( eEntType )
      {
        case SYSMON_ENTTYPE_U8 :
          // get the get function/get the value/set the length
          pvGetU8 = PGM_RDDWRD( ptDef->pvGetFuncs.pvGetU8 );
          tValue.anValue[ LE_U32_LSB_IDX ] = pvGetU8( );
          nLength = 1;
          break;

        case SYSMON_ENTTYPE_S8 :
          // get the get function/get the value/set the length
          pvGetS8 = PGM_RDDWRD( ptDef->pvGetFuncs.pvGetS8 );
          tValue.anValue[ LE_U32_LSB_IDX ] = pvGetS8( );
          nLength = 1;
          break;
        
        case SYSMON_ENTTYPE_U16 :
          // get the get function/get the value/set the length
          pvGetU16 = PGM_RDDWRD( ptDef->pvGetFuncs.pvGetU16 );
          tValue.awValue[ LE_U32_LSW_IDX ] = pvGetU16( );
          nLength = 2;
          break;

        case SYSMON_ENTTYPE_S16 :
          // get the get function/get the value/set the length
          pvGetS16 = PGM_RDDWRD( ptDef->pvGetFuncs.pvGetS16 );
          tValue.awValue[ LE_U32_LSW_IDX ] = pvGetS16( );
          nLength = 2;
          break;
        
        case SYSMON_ENTTYPE_U32 :
          // get the get function/get the value/set the length
          pvGetU32 = PGM_RDDWRD( ptDef->pvGetFuncs.pvGetU32 );
          tValue.uValue = pvGetU32( );
          nLength = 4;
          break;

        case SYSMON_ENTTYPE_S32 :
          // get the get function/get the value/set the length
          pvGetS32 = PGM_RDDWRD( ptDef->pvGetFuncs.pvGetS32 );
          tValue.uValue = pvGetS32( );
          nLength = 4;
          break;

        case SYSMON_ENTTYPE_FLOAT :
          // get the get function/get the value/set the length
          pvGetFloat = PGM_RDDWRD( ptDef->pvGetFuncs.pvGetFloat );
          tValue.fValue = pvGetFloat( );
          nLength = 4;
          break;
        
        case SYSMON_ENTTYPE_OPT_U8 :
          // get the get function/get the value/set the length
          pvGetOptU8 = PGM_RDDWRD( ptDef->pvGetFuncs.pvGetOptU8 );
          tValue.anValue[ LE_U32_LSB_IDX ] = pvGetOptU8( PGM_RDDWRD( ptDef->uOption ));
          nLength = 1;
          break;

        case SYSMON_ENTTYPE_OPT_S8 :
          // get the get function/get the value/set the length
          pvGetOptS8 = PGM_RDDWRD( ptDef->pvGetFuncs.pvGetOptS8 );
          tValue.anValue[ LE_U32_LSB_IDX ] = pvGetOptS8( PGM_RDDWRD( ptDef->uOption ));
          nLength = 1;
          break;
          
        case SYSMON_ENTTYPE_OPT_U16 :
          // get the get function/get the value/set the length
          pvGetOptU16 = PGM_RDDWRD( ptDef->pvGetFuncs.pvGetOptU16 );
          tValue.awValue[ LE_U32_LSW_IDX ] = pvGetOptU16( PGM_RDDWRD( ptDef->uOption ));
          nLength = 2;
          break;

        case SYSMON_ENTTYPE_OPT_S16 :
          // get the get function/get the value/set the length
          pvGetOptS16 = PGM_RDDWRD( ptDef->pvGetFuncs.pvGetOptS16 );
          tValue.awValue[ LE_U32_LSW_IDX ] = pvGetOptS16( PGM_RDDWRD( ptDef->uOption ));
          nLength = 2;
          break;
          
        case SYSMON_ENTTYPE_OPT_U32 :
          // get the get function/get the value/set the length
          pvGetOptU32 = PGM_RDDWRD( ptDef->pvGetFuncs.pvGetOptU32 );
          tValue.uValue = pvGetOptU32( PGM_RDDWRD( ptDef->uOption ));
          nLength = 4;
          break;

        case SYSMON_ENTTYPE_OPT_S32 :
          // get the get function/get the value/set the length
          pvGetOptS32 = PGM_RDDWRD( ptDef->pvGetFuncs.pvGetOptS32 );
          tValue.uValue = pvGetOptS32( PGM_RDDWRD( ptDef->uOption ));
          nLength = 4;
          break;

        case SYSMON_ENTTYPE_OPT_FLOAT :
          // get the get function/get the value/set the length
          pvGetOptFloat = PGM_RDDWRD( ptDef->pvGetFuncs.pvGetOptFloat );
          tValue.fValue = pvGetOptFloat( PGM_RDDWRD( ptDef->uOption ));
          nLength = 4;
          break;
          
        default :
          nLength = 0;
          break;
      }
    
      // now copy the values into the buffer
      for( nValIdx = 0; nValIdx < nLength; nValIdx++ )
      {
        // stuff the byte
        *( pnXmtBuffer + wXmtBufIdx++ ) = tValue.anValue[ nValIdx ];
      }

      // update the transmit length
      *( pwXmtLength ) = wXmtBufIdx;

      // set the send response error
      eError = SYSMON_ERRS_SENDRESP;
    }
    else
    {
      // set the error
      eError = SYSMON_ERRS_ILLENUM;
      break;
    }
  }

  // return the error
  return( eError );
}

/******************************************************************************
 * @function ProcessSetup
 *
 * @brief process a setup command
 *
 * This function will check for valid entries and proper frame size
 *
 * @param[in]   pmRcvBuffer  pointer to the message content
 * @param[in]   wLength         length of the message content
 *
 * @return      appropriate error
 *
 *****************************************************************************/
static SYSMONERRS PutValue( PU8 pnRcvBuffer, U16 wLength )
{
  SYSMONERRS          eError = SYSMON_ERRS_NONE;
  SYSMONENUM          eEntry;
  SYSMONENTTYPE       eEntType;
  PSYSMONENTDEF       ptDef;
  U32UN               tValue;
  PVSYSMONPUTU8       pvPutU8;
  PVSYSMONPUTS8       pvPutS8;
  PVSYSMONPUTU16      pvPutU16;
  PVSYSMONPUTS16      pvPutS16;
  PVSYSMONPUTU32      pvPutU32;
  PVSYSMONPUTS32      pvPutS32;
  PVSYSMONPUTFLOAT    pvPutFloat;
  PVSYSMONPUTOPTU8    pvPutOptU8;
  PVSYSMONPUTOPTS8    pvPutOptS8;
  PVSYSMONPUTOPTU16   pvPutOptU16;
  PVSYSMONPUTOPTS16   pvPutOptS16;
  PVSYSMONPUTOPTU32   pvPutOptU32;
  PVSYSMONPUTOPTS32   pvPutOptS32;
  PVSYSMONPUTOPTFLOAT pvPutOptFloat;
  U16                 wRcvBufIdx;

  // clear the transmit length
  wRcvBufIdx = 0;
  
  // now process each entry in the buffer
  while( wRcvBufIdx < wLength )
  {
    // check for a valid enumeration
    if (( eEntry = *( pnRcvBuffer + wRcvBufIdx++ )) < SYSMON_ENUM_MAX )
    {
      // get a pointer to the definition
      ptDef = ( PSYSMONENTDEF )&atSysMonDefs[ eEntry ];
    
      // now get the ENTTYPE
      eEntType = PGM_RDBYTE( ptDef->eType );
    
      // compute the size
      switch( eEntType )
      {
        case SYSMON_ENTTYPE_U8 :
          // get the put function/put the value
          pvPutU8 = PGM_RDDWRD( ptDef->pvPutFuncs.pvPutU8 );
          eError = ( pvPutU8( *( pnRcvBuffer + wRcvBufIdx++ ))) ? SYSMON_ERRS_ILLVALUE : SYSMON_ERRS_NONE;
          break;

        case SYSMON_ENTTYPE_S8 :
          // get the put function/put the value
          pvPutS8 = PGM_RDDWRD( ptDef->pvPutFuncs.pvPutS8 );
          eError = ( pvPutS8( *( pnRcvBuffer + wRcvBufIdx++ ))) ? SYSMON_ERRS_ILLVALUE : SYSMON_ERRS_NONE;
          break;
        
        case SYSMON_ENTTYPE_U16 :
          // get the put function/get the value/put the value
          pvPutU16 = PGM_RDDWRD( ptDef->pvPutFuncs.pvPutU16 );
          tValue.anValue[ LE_U32_LSB_IDX ] = *( pnRcvBuffer + wRcvBufIdx++ );
          tValue.anValue[ LE_U32_MS1_IDX ] = *( pnRcvBuffer + wRcvBufIdx++ );
          eError = ( pvPutU16( tValue.awValue[ LE_U32_LSW_IDX ])) ? SYSMON_ERRS_ILLVALUE : SYSMON_ERRS_NONE;
          break;

        case SYSMON_ENTTYPE_S16 :
          // get the put function/get the value/put the value
          pvPutS16 = PGM_RDDWRD( ptDef->pvPutFuncs.pvPutS16 );
          tValue.anValue[ LE_U32_LSB_IDX ] = *( pnRcvBuffer + wRcvBufIdx++ );
          tValue.anValue[ LE_U32_MS1_IDX ] = *( pnRcvBuffer + wRcvBufIdx++ );
          eError = ( pvPutS16( tValue.awValue[ LE_U32_LSW_IDX ])) ? SYSMON_ERRS_ILLVALUE : SYSMON_ERRS_NONE;
          break;
        
        case SYSMON_ENTTYPE_U32 :
          // get the put function/get the value/put the value
          pvPutU32 = PGM_RDDWRD( ptDef->pvPutFuncs.pvPutU32 );
          tValue.anValue[ LE_U32_LSB_IDX ] = *( pnRcvBuffer + wRcvBufIdx++ );
          tValue.anValue[ LE_U32_MS1_IDX ] = *( pnRcvBuffer + wRcvBufIdx++ );
          tValue.anValue[ LE_U32_MS2_IDX ] = *( pnRcvBuffer + wRcvBufIdx++ );
          tValue.anValue[ LE_U32_MSB_IDX ] = *( pnRcvBuffer + wRcvBufIdx++ );
          eError = ( pvPutU32( tValue.uValue )) ? SYSMON_ERRS_ILLVALUE : SYSMON_ERRS_NONE;
          break;

        case SYSMON_ENTTYPE_S32 :
          // get the put function/get the value/put the value
          pvPutS32 = PGM_RDDWRD( ptDef->pvPutFuncs.pvPutS32 );
          tValue.anValue[ LE_U32_LSB_IDX ] = *( pnRcvBuffer + wRcvBufIdx++ );
          tValue.anValue[ LE_U32_MS1_IDX ] = *( pnRcvBuffer + wRcvBufIdx++ );
          tValue.anValue[ LE_U32_MS2_IDX ] = *( pnRcvBuffer + wRcvBufIdx++ );
          tValue.anValue[ LE_U32_MSB_IDX ] = *( pnRcvBuffer + wRcvBufIdx++ );
          eError = ( pvPutS32( tValue.uValue )) ? SYSMON_ERRS_ILLVALUE : SYSMON_ERRS_NONE;
          break;

        case SYSMON_ENTTYPE_FLOAT :
          // get the put function/get the value/put the value
          pvPutFloat = PGM_RDDWRD( ptDef->pvPutFuncs.pvPutFloat );
          tValue.anValue[ LE_U32_LSB_IDX ] = *( pnRcvBuffer + wRcvBufIdx++ );
          tValue.anValue[ LE_U32_MS1_IDX ] = *( pnRcvBuffer + wRcvBufIdx++ );
          tValue.anValue[ LE_U32_MS2_IDX ] = *( pnRcvBuffer + wRcvBufIdx++ );
          tValue.anValue[ LE_U32_MSB_IDX ] = *( pnRcvBuffer + wRcvBufIdx++ );
          eError = ( pvPutFloat( tValue.fValue )) ? SYSMON_ERRS_ILLVALUE : SYSMON_ERRS_NONE;
          break;
        
        case SYSMON_ENTTYPE_OPT_U8 :
          // get the put function/put the value
          pvPutOptU8 = PGM_RDDWRD( ptDef->pvPutFuncs.pvPutOptU8 );
          eError = ( pvPutOptU8( PGM_RDDWRD( ptDef->uOption ), *( pnRcvBuffer + wRcvBufIdx++ ))) ? SYSMON_ERRS_ILLVALUE : SYSMON_ERRS_NONE;
          break;

        case SYSMON_ENTTYPE_OPT_S8 :
          // get the put function/put the value
          pvPutOptS8 = PGM_RDDWRD( ptDef->pvPutFuncs.pvPutOptS8 );
          eError = ( pvPutOptS8( PGM_RDDWRD( ptDef->uOption ), *( pnRcvBuffer + wRcvBufIdx++ ))) ? SYSMON_ERRS_ILLVALUE : SYSMON_ERRS_NONE;
          break;
        
        case SYSMON_ENTTYPE_OPT_U16 :
          // get the put function/get the value/put the value
          pvPutOptU16 = PGM_RDDWRD( ptDef->pvPutFuncs.pvPutOptU16 );
          tValue.anValue[ LE_U32_LSB_IDX ] = *( pnRcvBuffer + wRcvBufIdx++ );
          tValue.anValue[ LE_U32_MS1_IDX ] = *( pnRcvBuffer + wRcvBufIdx++ );
          eError = ( pvPutOptU16( PGM_RDDWRD( ptDef->uOption ), tValue.awValue[ LE_U32_LSW_IDX ])) ? SYSMON_ERRS_ILLVALUE : SYSMON_ERRS_NONE;
          break;

        case SYSMON_ENTTYPE_OPT_S16 :
          // get the put function/get the value/put the value
          pvPutOptS16 = PGM_RDDWRD( ptDef->pvPutFuncs.pvPutOptS16 );
          tValue.anValue[ LE_U32_LSB_IDX ] = *( pnRcvBuffer + wRcvBufIdx++ );
          tValue.anValue[ LE_U32_MS1_IDX ] = *( pnRcvBuffer + wRcvBufIdx++ );
          eError = ( pvPutOptS16( PGM_RDDWRD( ptDef->uOption ), tValue.awValue[ LE_U32_LSW_IDX ])) ? SYSMON_ERRS_ILLVALUE : SYSMON_ERRS_NONE;
          break;
        
        case SYSMON_ENTTYPE_OPT_U32 :
          // get the put function/get the value/put the value
          pvPutOptU32 = PGM_RDDWRD( ptDef->pvPutFuncs.pvPutOptU32 );
          tValue.anValue[ LE_U32_LSB_IDX ] = *( pnRcvBuffer + wRcvBufIdx++ );
          tValue.anValue[ LE_U32_MS1_IDX ] = *( pnRcvBuffer + wRcvBufIdx++ );
          tValue.anValue[ LE_U32_MS2_IDX ] = *( pnRcvBuffer + wRcvBufIdx++ );
          tValue.anValue[ LE_U32_MSB_IDX ] = *( pnRcvBuffer + wRcvBufIdx++ );
          eError = ( pvPutOptU32( PGM_RDDWRD( ptDef->uOption ), tValue.uValue )) ? SYSMON_ERRS_ILLVALUE : SYSMON_ERRS_NONE;
          break;

        case SYSMON_ENTTYPE_OPT_S32 :
          // get the put function/get the value/put the value
          pvPutOptS32 = PGM_RDDWRD( ptDef->pvPutFuncs.pvPutOptS32 );
          tValue.anValue[ LE_U32_LSB_IDX ] = *( pnRcvBuffer + wRcvBufIdx++ );
          tValue.anValue[ LE_U32_MS1_IDX ] = *( pnRcvBuffer + wRcvBufIdx++ );
          tValue.anValue[ LE_U32_MS2_IDX ] = *( pnRcvBuffer + wRcvBufIdx++ );
          tValue.anValue[ LE_U32_MSB_IDX ] = *( pnRcvBuffer + wRcvBufIdx++ );
          eError = ( pvPutOptS32( PGM_RDDWRD( ptDef->uOption ), tValue.uValue )) ? SYSMON_ERRS_ILLVALUE : SYSMON_ERRS_NONE;
          break;

        case SYSMON_ENTTYPE_OPT_FLOAT :
          // get the put function/get the value/put the value
          pvPutOptFloat = PGM_RDDWRD( ptDef->pvPutFuncs.pvPutOptFloat );
          tValue.anValue[ LE_U32_LSB_IDX ] = *( pnRcvBuffer + wRcvBufIdx++ );
          tValue.anValue[ LE_U32_MS1_IDX ] = *( pnRcvBuffer + wRcvBufIdx++ );
          tValue.anValue[ LE_U32_MS2_IDX ] = *( pnRcvBuffer + wRcvBufIdx++ );
          tValue.anValue[ LE_U32_MSB_IDX ] = *( pnRcvBuffer + wRcvBufIdx++ );
          eError = ( pvPutOptFloat( PGM_RDDWRD( ptDef->uOption ), tValue.fValue )) ? SYSMON_ERRS_ILLVALUE : SYSMON_ERRS_NONE;
          break;
        
        default :
          break;
      }
    }
    else
    {
      // set the error
      eError = SYSMON_ERRS_ILLENUM;
    }
  }  

  // return the error
  return( eError );
}

/**@} EOF SystemMonitor.c */
