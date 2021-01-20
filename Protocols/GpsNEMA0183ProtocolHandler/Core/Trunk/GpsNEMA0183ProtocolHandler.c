/******************************************************************************
 * @file GpsNEMA0183ProtocolHandler.c
 *
 * @brief GPS NEMA-0183 protocol handler implementation
 *
 * This file provides the NEMA-0183 proocol handler
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
 * \addtogroup GpsNEMA0183ProtocolHandler
 * @{
 *****************************************************************************/

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "GpsNEMA0183ProtocolHandler/GpsNEMA0183ProtocolHandler_prm.h"
#include "GpsNEMA0183ProtocolHandler/GpsNEMA0183ProtocolHandler.h"

// library includes -----------------------------------------------------------
#include "StateExecutionEngine/StateExecutionEngine.h"

// Macros and Defines ---------------------------------------------------------
/// define the start character
#define CH_GPS_START                            ( '$' )

/// define the delimiter
#define CH_GPS_DELIM                            ( ',' )

/// define the data terminator
#define CH_GPS_TERM                             ( '*' )

/// define the end of line
#define CH_GPS_EOL                              ( '\n' )

/// define the maximum command length
#define CMD_MAX_LENGTH                          ( 8 )

/// define the maximum argument length
#define ARG_MAX_LENGTH                          ( 16 )

/// define the maximum number of arguments
#define CMD_NUM_ARGS                            ( 128 )

/// define the maximum number of satellites/messages
#define NUM_SATS_PER_MESSAGE                    ( 12 )
#define NUM_SAT_MESSAGES                        ( 3 )
#define MAX_NUM_SATELLITES                      ( NUM_SATS_PER_MESSAGE * NUM_SAT_MESSAGES )

// enumerations ---------------------------------------------------------------
/// enumerate the protocol states
typedef enum _GPSSTATE
{
  GPS_STATE_IDLE = 0,
  GPS_STATE_CMND,
  GPS_STATE_DATA,
  GPS_STATE_CHEK,
  GPS_STATE_MAX
} GPSSTATE;

// structures -----------------------------------------------------------------
/// define the GPS argument
typedef struct _GPSARG
{
  C8    acArgument[ ARG_MAX_LENGTH ];
} GPSARG, *PGPSARG;
#define GPSARG_SIZE                             sizeof( GPSARG )

/// define the command table
typedef struct _GPSCMDTBL
{
  PC8       pszCommand;               ///< command
  U8        nCompareLen;              ///< compare length
  void      ( *pvFunction )( void );  ///< command function 
} GPSCMDTBL, *PGPSCMDTBL;
#define GPSCMDTBL_SIZE                          sizeof( GPSCMDTBL )

// global parameter declarations ----------------------------------------------

// local parameter declarations -----------------------------------------------
static  GPSSTATE            eGpsState;
static  STATEEXECENGCONTROL tStateCtl;
static  C8                  acCommand[ CMD_MAX_LENGTH ];
static  U8                  nBufIndex;
static  U8                  nTalkerIndex;
static  U8                  nTalkerLength;
static  U8                  nArgCount;
static  GPSARG              atGpsArgs[ CMD_NUM_ARGS ];
static  GPSDATA             tGpsData;
static  GPSSATELLITE        atSatellites[ MAX_NUM_SATELLITES ];
static  U8                  anSatellitesInView[ NUM_SAT_MESSAGES ];

// local function prototypes --------------------------------------------------
static  void  CmdGGA( void );
static  void  CmdGLL( void );
static  void  CmdGSA( void );
static  void  CmdGSV( void );
static  void  CmdRMC( void );
static  void  CmdVTG( void );

// GPS_STATE_CMND functions
static  void  GpsStateCmndEnt( void );
static  U8    GpsStateCmndExc( STATEEXECENGARG xArg );
static  void  GpsStateCmndExt( void );

// GPS_STATE_DATA functions
static  void  GpsStateDataEnt( void );
static  U8    GpsStateDataExc( STATEEXECENGARG xArg );

// GPS_STATE_CHEK functions
static  void  GpsStateChekExt( void );

// constant parameter initializations -----------------------------------------
/// define the talker identifiers
static  const CODE C8 szTalker[ ] = GPSNEMA0183PROTOCOL_TALKER_IDENTIFIER;

/// define the sentence identifiers
static  const CODE C8 szCmdGGA[ ]  = { "NGGA" };
static  const CODE C8 szCmdGLL[ ]  = { "NGLL" };
static  const CODE C8 szCmdGSA[ ]  = { "NGSA" };
static  const CODE C8 szCmdGSV[ ]  = { "LGSV" };
static  const CODE C8 szCmdRMC[ ]  = { "NRMC" };
static  const CODE C8 szCmdVTG[ ]  = { "NVTG" };

/// define the command table
static  const CODE GPSCMDTBL  atCmdTable[ ] =
{
  { ( PC8 )&szCmdGGA, 4, CmdGGA },
  { ( PC8 )&szCmdGLL, 4, CmdGLL },
  { ( PC8 )&szCmdGSA, 4, CmdGSA },
  { ( PC8 )&szCmdGSV, 4, CmdGSV },
  { ( PC8 )&szCmdRMC, 4, CmdRMC },
  { ( PC8 )&szCmdVTG, 4, CmdVTG },
  { NULL,             0, NULL   }
};

/// initialize the IDLE events
static  const CODE STATEEXECENGEVENT  atIdleEvents[ ] =
{
  STATEEXECENGEVENT_ENTRY( CH_GPS_START, GPS_STATE_CMND, TRUE  ),
  STATEEXECENGEVENT_END( )
};

/// initialize the CMND events
static  const CODE STATEEXECENGEVENT  atCmndEvents[ ] =
{
  STATEEXECENGEVENT_ENTRY( CH_GPS_DELIM, GPS_STATE_DATA, TRUE  ),
  STATEEXECENGEVENT_ENTRY( CH_GPS_EOL,   GPS_STATE_IDLE, TRUE  ),
};

/// initialize the DATA events
static  const CODE STATEEXECENGEVENT  atDataEvents[ ] =
{
  STATEEXECENGEVENT_ENTRY( CH_GPS_TERM, GPS_STATE_CHEK, TRUE  ),
  STATEEXECENGEVENT_ENTRY( CH_GPS_EOL,  GPS_STATE_IDLE, TRUE  ),
};

/// initialize the CHEK events
static  const CODE STATEEXECENGEVENT  atChekEvents[ ] =
{
  STATEEXECENGEVENT_ENTRY( CH_GPS_EOL, GPS_STATE_IDLE, TRUE  ),
};

/// initialize the main state table
static  const CODE STATEEXECENGTABLE  atGpsStates[ GPS_STATE_MAX ] =
{
  STATEXECENGETABLE_ENTRY( GPS_STATE_IDLE, NULL,            NULL,            NULL,            atIdleEvents ),
  STATEXECENGETABLE_ENTRY( GPS_STATE_CMND, GpsStateCmndEnt, GpsStateCmndExc, GpsStateCmndExt, atCmndEvents ),
  STATEXECENGETABLE_ENTRY( GPS_STATE_DATA, GpsStateDataEnt, GpsStateDataExc, NULL,            atDataEvents ),
  STATEXECENGETABLE_ENTRY( GPS_STATE_CHEK, NULL,            NULL,            GpsStateChekExt, atChekEvents ),
};

/******************************************************************************
 * @function GpsNEMA0183ProtocolHandler_Initialize
 *
 * @brief intialization
 *
 * This function initializes the GPS protocol handler
 *
 *****************************************************************************/
void GpsNEMA0183ProtocolHandler_Initialize( void )
{
  // initialize the states
  tStateCtl.ptStates = atGpsStates;
  
  // calculate the length of the talker identifier
  nTalkerLength = strlen( szTalker );

  // initialize the state
  StateExecutionEngine_Initialize( &tStateCtl, GPS_STATE_IDLE );
}

/******************************************************************************
 * @function GpsNEMA0183ProtocolHandler_GetData
 *
 * @brief get the data
 *
 * This function will copy the current data
 *
 * @param[io]   ptData      pointer to the data to store the current data in
 *
 *****************************************************************************/
void GpsNEMA0183ProtocolHandler_GetData( PGPSDATA ptData )
{
  // copy the current data
  memcpy( ptData, &tGpsData, GPSDATA_SIZE );
}

/******************************************************************************
 * @function GpsNEMA0183ProtocolHandler_GetLatitude
 *
 * @brief get the latitude
 *
 * This function will return the latitude
 *
 * @return  current latitude
 *
 *****************************************************************************/
FLOAT GpsNEMA0183ProtocolHandler_GetLatitude( void )
{
  // return the latitude
  return( tGpsData.fLatitude );
}

/******************************************************************************
 * @function GpsNEMA0183ProtocolHandler_GetNorthSouth
 *
 * @brief get the North SOuth indicator
 *
 * This function will return the north/south indicator
 *
 * @return north/south
 *
 *****************************************************************************/
GPSLATNS GpsNEMA0183ProtocolHandler_GetNorthSouth( void )
{
  // return the north south
  return( tGpsData.eNorthSouth );
}

/******************************************************************************
 * @function GpsNEMA0183ProtocolHandler_GetLongitude
 *
 * @brief get the longitude
 *
 * This function will return the current longitude
 *
 * @return longitude
 *
 *****************************************************************************/
FLOAT GpsNEMA0183ProtocolHandler_GetLongitude( void )
{
  // return the longitude
  return( tGpsData.fLongitude );
}

/******************************************************************************
 * @function GpsNEMA0183ProtocolHandler_GetEastWest
 *
 * @brief get the east/west indicator
 *
 * This function will return the east/west indicator
 *
 * @return current value
 *
 *****************************************************************************/
GPSLONEW GpsNEMA0183ProtocolHandler_GetEastWest( void )
{
  // return the latitude
  return( tGpsData.eEastWest );
}

/******************************************************************************
 * @function GpsNEMA0183ProtocolHandler_GetAltitude
 *
 * @brief get the altitude
 *
 * This function will return the current value of the altitude
 *
 * @return altidude
 *
 *****************************************************************************/
FLOAT GpsNEMA0183ProtocolHandler_GetAltitude( void )
{
  // return the latitude
  return( tGpsData.fAltitude );
}

/******************************************************************************
 * @function GpsNEMA0183ProtocolHandler_GetCourse
 *
 * @brief get the course
 *
 * This function will return the current course
 *
 * @return current course
 *
 *****************************************************************************/
FLOAT GpsNEMA0183ProtocolHandler_GetCourse( void )
{
  // return the latitude
  return( tGpsData.fCourse );
}

/******************************************************************************
 * @function GpsNEMA0183ProtocolHandler_GetSpeed
 *
 * @brief get the speed
 *
 * This function will return the current speed
 *
 * @return current speed
 *
 *****************************************************************************/
FLOAT GpsNEMA0183ProtocolHandler_GetSpeed( void )
{
  // return the latitude
  return( tGpsData.fSpeed );
}

/******************************************************************************
 * @function GpsNEMA0183ProtocolHandler_GetSpeed
 *
 * @brief get the speed
 *
 * This function will return the current number of satellites
 *
 * @return number of satellites
 *
 *****************************************************************************/
U8 GpsNEMA0183ProtocolHandler_GetNumSatellites( void )
{
  // return the number of satellites
  return( tGpsData.nNumOfSatelites );
}

/******************************************************************************
 * @function GPsNEMA0183ProtocolHandler_GetFixStatus
 *
 * @brief get the fix status
 *
 * This function will return the current fix status
 *
 * @return fix status
 *
 *****************************************************************************/
U8 GpsNEMA0183ProtocolHandler_GetFixStatus( void )
{
  // return the fix status
  return( tGpsData.nPosFixStatus );
}

/******************************************************************************
 * @function GpsNEMA0183ProtocolHandler_ProcessChar
 *
 * @brief process character
 *
 * This function will process the character 
 *
 * @param[in]   nChar   received chracter to process
 *
 *****************************************************************************/
void GpsNEMA0183ProtocolHandler_ProcessChar( U8 nChar )
{
  // process the event through the state engine
  StateExecutionEngine_Process( &tStateCtl, nChar );
}

/******************************************************************************
// GPS_STATE_CMND functions
 *****************************************************************************/
static void GpsStateCmndEnt( void )
{
  // clear the index
  nBufIndex = 0;
  nTalkerIndex = 0;
}

static U8 GpsStateCmndExc( STATEEXECENGARG xArg )
{
  U8  nNextState = STATEEXECENG_STATE_NONE;
  
  // skip the talker identifier
  if ( nTalkerIndex++ >= nTalkerLength )
  {
    // if room, add character to buffer
    if ( nBufIndex < ( CMD_MAX_LENGTH - 1 ))
    {
      // add it to buffer
      acCommand[ nBufIndex++ ] = ( C8 )xArg;
    }
  }
  
  // return next state
  return( nNextState );
}

static void GpsStateCmndExt( void )
{
  // terminate the string
  acCommand[ nBufIndex ] = '\0';
}

/******************************************************************************
// GPS_STATE_DATA functions
 *****************************************************************************/
static void GpsStateDataEnt( void )
{
  // clear the index/argument count
  nBufIndex = 0;
  nArgCount = 0;
}

static U8 GpsStateDataExc( STATEEXECENGARG xArg )
{
  U8  nNextState = STATEEXECENG_STATE_NONE;
  
  // is there room in the argument array
  if ( nArgCount < CMD_NUM_ARGS )
  {
    // check for delimited
    if ( xArg != CH_GPS_DELIM )
    {
      // if room, add character to buffer
      if ( nBufIndex < ( ARG_MAX_LENGTH - 1 ))
      {
        // add it to buffer
        atGpsArgs[ nArgCount ].acArgument[ nBufIndex++ ] = ( C8 )xArg;
      }
    }
    else
    {
      // terminate the string
      atGpsArgs[ nArgCount ].acArgument[ nBufIndex ] = '\0';
      
      // increment the argument count
      nArgCount++;
      
      // reset the buffer index
      nBufIndex = 0;
    }
  }
  
  // return next state
  return( nNextState );
}

/******************************************************************************
// GPS_STATE_CHEK functions
 *****************************************************************************/
static void GpsStateChekExt( void )
{
  PC8 pszTblCmd;
  U8  nIdx;
  
  // find the command in the table
  nIdx = 0;
  while(( pszTblCmd = ( PC8 )PGM_RDWORD( atCmdTable[ nIdx ].pszCommand )) != NULL )
  {
    // is this our command
    if ( STRNCMP_P(( char const* )acCommand, ( char const* )pszTblCmd, atCmdTable[ nIdx ].nCompareLen ) == 0 )
    {
      // get the function and execute it/ exit
      atCmdTable[ nIdx ].pvFunction( );
      break;
    }
    else
    {
      // increment the index
      nIdx++;
    }
  }
}

/******************************************************************************
 * @function CmdGGA
 *
 * @brief GGA command handler
 *
 *****************************************************************************/
static void CmdGGA( void )
{
  // parse the message
  tGpsData.fUtcPosition = ( FLOAT )atof( atGpsArgs[ 0 ].acArgument );
  tGpsData.fLatitude = ( FLOAT )atof( atGpsArgs[ 1 ].acArgument );
  tGpsData.eNorthSouth = ( atGpsArgs[ 2 ].acArgument[ 0 ] == 'N' ) ? GPS_LATNS_NORTH : GPS_LATNS_SOUTH;
  tGpsData.fLongitude = ( FLOAT )atof( atGpsArgs[ 3 ].acArgument );
  tGpsData.eEastWest = ( atGpsArgs[ 4 ].acArgument[ 0 ] == 'E' ) ? GPS_LONEW_EAST : GPS_LONEW_WEST;
  tGpsData.nPosFixStatus = ( U8 )atoi( atGpsArgs[ 5 ].acArgument );
  tGpsData.nNumOfSatelites = ( U8 )atoi( atGpsArgs[ 6 ].acArgument );
}

/******************************************************************************
 * @function CmdGLL
 *
 * @brief GLL command handler
 *
 *****************************************************************************/
static void CmdGLL( void )
{
}

/******************************************************************************
 * @function CmdGSA
 *
 * @brief GSA command handler
 *
 *****************************************************************************/
static void CmdGSA( void )
{
}

/******************************************************************************
 * @function CmdGSV
 *
 * @brief GSV command handler
 *
 *****************************************************************************/
static void CmdGSV( void )
{
  U8  nMsgNum, nSatIdx, nDataIdx;

  // determine the message number
  nMsgNum = ( U8 )atoi( atGpsArgs[ 1 ].acArgument );

  // set the satellites in view
  anSatellitesInView[ nMsgNum ] = ( U8 )atoi( atGpsArgs[ 2 ].acArgument );

  // now for each satellite
  for ( nSatIdx = 0; nSatIdx < anSatellitesInView[ nMsgNum ]; nSatIdx++ )
  {
    // now compute the data index
    nDataIdx = ( nMsgNum * NUM_SATS_PER_MESSAGE ) + nSatIdx;

    // store the data
    atSatellites[ nDataIdx ].nId = ( U8 )atoi( atGpsArgs[ 3 + ( nSatIdx * GPSSATELLITE_SIZE ) ].acArgument );
    atSatellites[ nDataIdx ].nElevation = ( U8 )atoi( atGpsArgs[ 4 + ( nSatIdx * GPSSATELLITE_SIZE ) ].acArgument );
    atSatellites[ nDataIdx ].wAzimuth = ( U16 )atoi( atGpsArgs[ 5 + ( nSatIdx * GPSSATELLITE_SIZE ) ].acArgument );
    atSatellites[ nDataIdx ].nSigNoiseRatio = ( U8 )atoi( atGpsArgs[ 6 + ( nSatIdx * GPSSATELLITE_SIZE ) ].acArgument );
  }
}

/******************************************************************************
 * @function CmdRMC
 *
 * @brief RMC command handler
 *
 *****************************************************************************/
static void CmdRMC( void )
{
}

/******************************************************************************
 * @function CmdVTG
 *
 * @brief VTG command handler
 *
 *****************************************************************************/
static void CmdVTG( void )
{
  // parse the message
  tGpsData.fCourse = ( FLOAT )atof( atGpsArgs[ 0 ].acArgument );
  tGpsData.fSpeed = ( FLOAT )atof( atGpsArgs[ 1 ].acArgument );
}

/**@} EOF GpsNEMA0183ProtocolHandler.c */
