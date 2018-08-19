/******************************************************************************
 * @file SystemMonitor_def.h
 *
 * @brief System monitor definitions declarations 
 *
 * This file provides the definitions for the control and definition structures
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
 
// ensure only one instantiation
#ifndef _SYSTEMMONITOR_DEF_H
#define _SYSTEMMONITOR_DEF_H

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "SystemMonitor/SystemMonitor_prm.h"

// library includes -----------------------------------------------------------
#include "Types/Types.h"

// Macros and Defines ---------------------------------------------------------
/// define the helper macro for creating entries
#define SYSMONTYPEU8( get, put ) \
  { \
    .eType = SYSMON_ENTTYPE_U8, \
    .pvGetFuncs.pvGetU8 = get, \
    .pvPutFuncs.pvPutU8 = put, \
    .uOption = 0xFFFFFFFF, \
  }

#define SYSMONTYPES8( get, put ) \
  { \
    .eType = SYSMON_ENTTYPE_S8, \
    .pvGetFuncs.pvGetS8 = get, \
    .pvPutFuncs.pvPutS8 = put, \
    .uOption = 0xFFFFFFFF, \
  }

#define SYSMONTYPEU16( get, put ) \
  { \
    .eType = SYSMON_ENTTYPE_U16, \
    .pvGetFuncs.pvGetU16 = get, \
    .pvPutFuncs.pvPutU16 = put, \
    .uOption = 0xFFFFFFFF, \
  }

#define SYSMONTYPES16( get, put ) \
  { \
    .eType = SYSMON_ENTTYPE_S16, \
    .pvGetFuncs.pvGetS16 = get, \
    .pvPutFuncs.pvPutS16 = put, \
    .uOption = 0xFFFFFFFF, \
  }

#define SYSMONTYPEU32( get, put ) \
  { \
    .eType = SYSMON_ENTTYPE_U32, \
    .pvGetFuncs.pvGetU32 = get, \
    .pvPutFuncs.pvPutU32 = put, \
    .uOption = 0xFFFFFFFF, \
  }

#define SYSMONTYPES32( get, put ) \
  { \
    .eType = SYSMON_ENTTYPE_S32, \
    .pvGetFuncs.pvGetS32 = get, \
    .pvPutFuncs.pvPutS32 = put, \
    .uOption = 0xFFFFFFFF, \
  }

#define SYSMONTYPEFLOAT( get, put ) \
  { \
    .eType = SYSMON_ENTTYPE_FLOAT, \
    .pvGetFuncs.pvGetFloat = get, \
    .pvPutFuncs.pvPutFloat = put, \
    .uOption = 0xFFFFFFFF, \
  }

#define SYSMONTYPEOPTU8( get, put, option ) \
  { \
    .eType = SYSMON_ENTTYPE_OPT_U8, \
    .pvGetFuncs.pvGetOptU8 = ( PVSYSMONGETOPTU8 )get, \
    .pvPutFuncs.pvPutOptU8 = ( PVSYSMONPUTOPTU8 )put, \
    .uOption = ( U32 )option, \
  }

#define SYSMONTYPEOPTS8( get, put, option ) \
  { \
    .eType = SYSMON_ENTTYPE_OPT_S8, \
    .pvGetFuncs.pvGetOptS8 = ( PVSYSMONGETOPTS8 )get, \
    .pvPutFuncs.pvPutOptS8 = ( PVSYSMONPUTOPTS8 )put, \
    .uOption = ( U32 )option, \
  }

#define SYSMONTYPEOPTU16( get, put, option ) \
  { \
    .eType = SYSMON_ENTTYPE_OPT_U16, \
    .pvGetFuncs.pvGetOptU16 = ( PVSYSMONGETOPTU16 )get, \
    .pvPutFuncs.pvPutOptU16 = ( PVSYSMONPUTOPTU16 )put, \
    .uOption = ( U32 )option, \
  }

#define SYSMONTYPEOPTS16( get, put, option ) \
  { \
    .eType = SYSMON_ENTTYPE_OPT_S16, \
    .pvGetFuncs.pvGetOptS16 = ( PVSYSMONGETOPTS16 )get, \
    .pvPutFuncs.pvPutOptS16 = ( PVSYSMONPUTOPTS16 )put, \
    .uOption = ( U32 )option, \
  }

#define SYSMONTYPEOPTU32( get, put, option ) \
  { \
    .eType = SYSMON_ENTTYPE_OPT_U32, \
    .pvGetFuncs.pvGetOptU32 = ( PVSYSMONGETOPTU32 )get, \
    .pvPutFuncs.pvPutOptU32 = ( PVSYSMONPUTOPTU32 )put, \
    .uOption = ( U32 )option, \
  }

#define SYSMONTYPEOPTS32( get, put, option ) \
  { \
    .eType = SYSMON_ENTTYPE_OPT_S32, \
    .pvGetFuncs.pvGetOptS32 = ( PVSYSMONGETOPTS32 )get, \
    .pvPutFuncs.pvPutOptS32 = ( PVSYSMONPUTOPTS32 )put, \
    .uOption = ( U32 )option, \
  }

#define SYSMONTYPEOPTFLOAT( get, put, option ) \
  { \
    .eType = SYSMON_ENTTYPE_OPT_FLOAT, \
    .pvGetFuncs.pvGetOptFloat = ( PVSYSMONGETOPTFLOAT )get, \
    .pvPutFuncs.pvPutOptFloat = ( PVSYSMONPUTOPTFLOAT )put, \
    .uOption = ( U32 )option, \
  }

// enumerations ---------------------------------------------------------------
/// enumerate the entry types
typedef enum _SYSMONENTTYPE
{
  SYSMON_ENTTYPE_U8 = 0,          ///< unsigned 8 bit
  SYSMON_ENTTYPE_S8,              ///< signed 8 bit
  SYSMON_ENTTYPE_U16,             ///< unsigned 16 bit
  SYSMON_ENTTYPE_S16,             ///< signed 16 bit
  SYSMON_ENTTYPE_U32,             ///< unsigned 32 bit
  SYSMON_ENTTYPE_S32,             ///< signed 32 bit
  SYSMON_ENTTYPE_FLOAT,           ///< float
  SYSMON_ENTTYPE_OPT_U8,          ///< unsigned 8 bit
  SYSMON_ENTTYPE_OPT_S8,          ///< signed 8 bit
  SYSMON_ENTTYPE_OPT_U16,         ///< unsigned 16 bit
  SYSMON_ENTTYPE_OPT_S16,         ///< signed 16 bit
  SYSMON_ENTTYPE_OPT_U32,         ///< unsigned 32 bit
  SYSMON_ENTTYPE_OPT_S32,         ///< signed 32 bit
  SYSMON_ENTTYPE_OPT_FLOAT,       ///< float
  SYSMON_ENTTYPE_MAX,
} SYSMONENTTYPE;

// structures -----------------------------------------------------------------
/// define the get value protototypes
typedef U8    ( *PVSYSMONGETU8 )( void );
typedef S8    ( *PVSYSMONGETS8 )( void );
typedef U16   ( *PVSYSMONGETU16 )( void );
typedef S16   ( *PVSYSMONGETS16 )( void );
typedef U32   ( *PVSYSMONGETU32 )( void );
typedef S32   ( *PVSYSMONGETS32 )( void );
typedef FLOAT ( *PVSYSMONGETFLOAT )( void );

/// define the get value prototypes with option
typedef U8    ( *PVSYSMONGETOPTU8 )( U32 );
typedef S8    ( *PVSYSMONGETOPTS8 )( U32 );
typedef U16   ( *PVSYSMONGETOPTU16 )( U32 );
typedef S16   ( *PVSYSMONGETOPTS16 )( U32 );
typedef U32   ( *PVSYSMONGETOPTU32 )( U32 );
typedef S32   ( *PVSYSMONGETOPTS32 )( U32 );
typedef FLOAT ( *PVSYSMONGETOPTFLOAT )( U32 );

/// define the put value prototypes
typedef BOOL  ( *PVSYSMONPUTU8 )( U8 );
typedef BOOL  ( *PVSYSMONPUTS8 )( S8 );
typedef BOOL  ( *PVSYSMONPUTU16 )( U16 );
typedef BOOL  ( *PVSYSMONPUTS16 )( S16 );
typedef BOOL  ( *PVSYSMONPUTU32 )( U32 );
typedef BOOL  ( *PVSYSMONPUTS32 )( S32 );
typedef BOOL  ( *PVSYSMONPUTFLOAT )( FLOAT );

/// define the put value prototypes
typedef BOOL  ( *PVSYSMONPUTOPTU8 )( U32, U8 );
typedef BOOL  ( *PVSYSMONPUTOPTS8 )( U32, S8 );
typedef BOOL  ( *PVSYSMONPUTOPTU16 )( U32, U16 );
typedef BOOL  ( *PVSYSMONPUTOPTS16 )( U32, S16 );
typedef BOOL  ( *PVSYSMONPUTOPTU32 )( U32, U32 );
typedef BOOL  ( *PVSYSMONPUTOPTS32 )( U32, S32 );
typedef BOOL  ( *PVSYSMONPUTOPTFLOAT )( U32, FLOAT );

/// define the get function union
typedef union _SYSMONGETFUNCS
{
  PVSYSMONGETU8       pvGetU8;        ///< fetch a U8
  PVSYSMONGETS8       pvGetS8;        ///< fetch a S8
  PVSYSMONGETU16      pvGetU16;       ///< fetch a U16
  PVSYSMONGETS16      pvGetS16;       ///< fetch a S16
  PVSYSMONGETU32      pvGetU32;       ///< fetch a U32
  PVSYSMONGETS32      pvGetS32;       ///< fetch a S32
  PVSYSMONGETFLOAT    pvGetFloat;     ///< fetch a float
  PVSYSMONGETOPTU8    pvGetOptU8;     ///< fetch a U8 w/option
  PVSYSMONGETOPTS8    pvGetOptS8;     ///< fetch a S8 w/option
  PVSYSMONGETOPTU16   pvGetOptU16;    ///< fetch a U16 w/option
  PVSYSMONGETOPTS16   pvGetOptS16;    ///< fetch a S1 w/option
  PVSYSMONGETOPTU32   pvGetOptU32;    ///< fetch a U3 w/option
  PVSYSMONGETOPTS32   pvGetOptS32;    ///< fetch a S3 w/option
  PVSYSMONGETOPTFLOAT pvGetOptFloat;  ///< fetch a float w/option
} SYSMONGETFUNCS;

/// define the put function union
typedef union _SYSMONPUTFUNCS
{
  PVSYSMONPUTU8       pvPutU8;        ///< put a U8
  PVSYSMONPUTS8       pvPutS8;        ///< put a S8
  PVSYSMONPUTU16      pvPutU16;       ///< put a U16
  PVSYSMONPUTS16      pvPutS16;       ///< put a S16
  PVSYSMONPUTU32      pvPutU32;       ///< put a U32
  PVSYSMONPUTS32      pvPutS32;       ///< put a S32
  PVSYSMONPUTFLOAT    pvPutFloat;     ///< put a float
  PVSYSMONPUTOPTU8    pvPutOptU8;     ///< fetch a U8 w/option
  PVSYSMONPUTOPTS8    pvPutOptS8;     ///< fetch a S8 w/option
  PVSYSMONPUTOPTU16   pvPutOptU16;    ///< fetch a U16 w/option
  PVSYSMONPUTOPTS16   pvPutOptS16;    ///< fetch a S1 w/option
  PVSYSMONPUTOPTU32   pvPutOptU32;    ///< fetch a U3 w/option
  PVSYSMONPUTOPTS32   pvPutOptS32;    ///< fetch a S3 w/option
  PVSYSMONPUTOPTFLOAT pvPutOptFloat;  ///< fetch a float w/option
} SYSMONPUTFUNCS;

/// define the monitor entry structure
typedef struct _SYSMONENTDEF
{
  SYSMONENTTYPE     eType;            ///< type of 
  SYSMONGETFUNCS    pvGetFuncs;       ///< get functions
  SYSMONPUTFUNCS    pvPutFuncs;       ///< put functions
  U32               uOption;          ///< option
} SYSMONENTDEF, *PSYSMONENTDEF;
#define SYSMONENTDEF_SIZE     sizeof( SYSMONENTDEF )

/// define the transmit function for auto report
typedef void ( *PVSYSMONTRANSMIT )( U8, PU8, U16, U8 );

/**@} EOF SystemMonitor_def.h */

#endif  // _SYSTEMMONITOR_DEF_H