/******************************************************************************
 * @file GpsNEMA0183ProtocolHandler.h
 *
 * @brief GPS NEMA-0183 protocol handler implemenation 
 *
 * This file provides the implementation for the NEMA-0183 protocol handler
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
 
// ensure only one instantiation
#ifndef _GPSNEMA0183PROTOCOLHANDLER_H
#define _GPSNEMA0183PROTOCOLHANDLER_H

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------

// library includes -----------------------------------------------------------
#include "Types/Types.h"

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------
/// enumerate the latitude north/south
typedef enum _GPSLATNS
{
  GPS_LATNS_NORTH = 0,
  GPS_LATNS_SOUTH
} GPSLATNS;

/// enumerate the longitude east/west
typedef enum _GPSLONEW
{
  GPS_LONEW_EAST = 0,
  GPS_LONEW_WEST,
} GPSLONEW;

// structures -----------------------------------------------------------------
/// define the GPS data structure
typedef struct _GPSDATA
{
  FLOAT     fUtcPosition;         ///< UTC position
  FLOAT     fLatitude;            ///< latitude
  GPSLATNS  eNorthSouth;          ///< North/South
  FLOAT     fLongitude;           ///< longitude
  GPSLONEW  eEastWest;            ///< east west
  FLOAT     fAltitude;            ///< altitude
  U8        nPosFixStatus;        ///< fix status
  U8        nNumOfSatelites;      ///< number of satellites
  FLOAT     fCourse;              ///< course
  FLOAT     fSpeed;               ///< speed
} GPSDATA, *PGPSDATA;
#define GPSDATA_SIZE              sizeof( GPSDATA )

/// define the GPS satellite structure
typedef struct _GPSSATELLITE
{
  U8      nId;                  ///< satellite id
  U8      nElevation;           ///< elevation in degrees
  U16     wAzimuth;             ///< azimuth in degrees
  U8      nSigNoiseRatio;       ///< signal to noise ratio
} GPSSATELLITE, *PGPSSATELLITE;
#define GPSSATELLITE_SIZE         sizeof( GPSSATELLITE )

// global parameter declarations -----------------------------------------------

// global function prototypes --------------------------------------------------
extern  void      GpsNEMA0183ProtocolHandler_Initialize( void );
extern  void      GpsNEMA0183ProtocolHandler_GetData( PGPSDATA ptData );
extern  void      GpsNEMA0183ProtocolHandler_GetSattelites( U8 nSatelliteIndex, PGPSSATELLITE ptSattelite );
extern  FLOAT     GpsNEMA0183ProtocolHandler_GetLatitude( void );
extern  GPSLATNS  GpsNEMA0183ProtocolHandler_GetNorthSouth( void );
extern  FLOAT     GpsNEMA0183ProtocolHandler_GetLongitude( void );
extern  GPSLONEW  GpsNEMA0183ProtocolHandler_GetEastWest( void );
extern  FLOAT     GpsNEMA0183ProtocolHandler_GetAltitude( void );
extern  FLOAT     GpsNEMA0183ProtocolHandler_GetCourse( void );
extern  FLOAT     GpsNEMA0183ProtocolHandler_GetSpeed( void );
extern  U8        GpsNEMA0183ProtocolHandler_GetNumSatellites( void );
extern  U8        GpsNEMA0183ProtocolHandler_GetFixStatus( void );
extern  void      GpsNEMA0183ProtocolHandler_ProcessChar( U8 nChar );

/**@} EOF GpsNEMA0183ProtocolHandler.h */

#endif  // _GPSNEMA0183PROTOCOLHANDLER_H