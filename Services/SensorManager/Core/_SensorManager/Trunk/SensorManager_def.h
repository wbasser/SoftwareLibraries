/******************************************************************************
 * @file SensorManager_def.h
 *
 * @brief Sensor Manager definitions 
 *
 * This file provides the definitions for the sensor manager configurations
 *
 * @copyright Copyright (c) 2012 CyberIntegration
 * This document contains proprietary data and information of CyberIntegration 
 * LLC. It is the exclusive property of CyberIntegration, LLC and will not be 
 * disclosed in any form to any party without prior written permission of 
 * CyberIntegration, LLC. This document may not be reproduced or further used 
 * without the prior written permission of CyberIntegration, LLC.
 *
 * Version History
 * ======
 * $Rev: $
 * 
 *
 * \addtogroup SensorManager
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _SENSORMANAGER_DEF_H
#define _SENSORMANAGER_DEF_H

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "SensorManager/SensorManager_prm.h"
#if ( SENSORMANAGER_AVERAGE_FILTER_ENABLE == 1 )
#include "SensorManager/Filters/FilterAveraging/FilterAveraging.h"
#endif // SENSORMANAGER_AVERAGE_FILTER_ENABLE
#if ( SENSORMANAGER_LEADLAG_FILTER_ENABLE == 1 )
#include "SensorManager/Filters/FilterLeadLag/FilterLeadLag.h"
#endif // SENSORMANAGER_LEADLAG_FILTER_ENABLE

// library includes -----------------------------------------------------------
#include "Types/Types.h"

// Macros and Defines ---------------------------------------------------------
/// define the helper macro for defining an internal channel no filter
#define SENMANINTNOFILT( chan, samprate, minraw, maxraw, minegu, maxegu ) \
  { \
    .eInpType = SENMAN_INPTYPE_INTANA, \
    .nChannel = chan, \
    .wSampleRate = samprate, \
    .xMinRawValue = minraw, \
    .xMaxRawValue = maxraw, \
    .xMinEguValue = minegu, \
    .xMaxEguValue = maxegu, \
    .eFiltType = SENMAN_FILTTYPE_NONE, \
  	.pvLinearize = NULL, \
}
  
/// define the helper macro for defining an internal channel no filter with linearization
#define SENMANINTNOFILTLIN( chan, samprate, minraw, maxraw, minegu, maxegu, linfunc, linopt ) \
  { \
	  .eInpType = SENMAN_INPTYPE_INTANA, \
	  .nChannel = chan, \
	  .wSampleRate = samprate, \
	  .xMinRawValue = minraw, \
    .xMaxRawValue = maxraw, \
    .xMinEguValue = minegu, \
	  .xMaxEguValue = maxegu, \
	  .eFiltType = SENMAN_FILTTYPE_NONE, \
    .pvLinearize = linfunc, \
    .nLinOption = linopt, \
  }
  
#if ( SENSORMANAGER_AVERAGE_FILTER_ENABLE == 1 )
/// define the helper macro for defining a internal channel with average filter
#define SENMANINTAVGFILT( chan, samprate, minraw, maxraw, minegu, maxegu, numsamps, bufname ) \
  { \
    .eInpType = SENMAN_INPTYPE_INTANA, \
    .nChannel = chan, \
    .wSampleRate = samprate, \
    .xMinRawValue = minraw, \
    .xMaxRawValue = maxraw, \
    .xMinEguValue = minegu, \
    .xMaxEguValue = maxegu, \
    .eFiltType = SENMAN_FILTTYPE_AVERAGE, \
    .tFilters.tAverage = \
	  { \
      .nNumberSamples = numsamps, \
      .pwBuffer = ( PU16 )&aw ## bufname ## AvgFilt, \
	  }, \
	  .pvLinearize = NULL, \
  }
  
 /// define the helper macro for defining a internal channel with average filter and linearization
#define SENMANINTAVGFILTLIN( chan, samprate, minraw, maxraw, minegu, maxegu, numsamps, bufname, linfunc, linopt ) \
  { \
	  .eInpType = SENMAN_INPTYPE_INTANA, \
	  .nChannel = chan, \
	  .wSampleRate = samprate, \
	  .xMinRawValue = minraw, \
	  .xMaxRawValue = maxraw, \
	  .xMinEguValue = minegu, \
	  .xMaxEguValue = maxegu, \
	  .eFiltType = SENMAN_FILTTYPE_AVERAGE, \
	  .tFilters.tAverage = \
	  { \
		  .nNumberSamples = numsamps, \
		  .pwBuffer = ( PU16 )&aw ## bufname ## AvgFilt, \
	  }, \
	  .pvLinearize = linfunc, \
    .nLinOption = linopt, \
  }
#endif // SENSORMANAGER_AVERAGE_FILTER_ENABLE
  
#if ( SENSORMANAGER_LEADLAG_FILTER_ENABLE == 1 )
/// define the helper macro for defining a internal channel with lead lag filter
#define SENMANINTLDLGFILT( chan, samprate, minraw, maxraw, minegu, maxegu, lagtc, leadtc ) \
  { \
	  .eInpType = SENMAN_INPTYPE_INTANA, \
	  .nChannel = chan, \
	  .wSampleRate = samprate, \
	  .xMinRawValue = minraw, \
    .xMaxRawValue = maxraw, \
	  .xMinEguValue = minegu, \
	  .xMaxEguValue = maxegu, \
	  .eFiltType = SENMAN_FILTTYPE_LEADLAG, \
	  .tFilters.tLeadLag = \
	  { \
		  .fLagTc = lagtc, \
		  .fLeadTc = leadtc, \
	  } \
	  .pvLinearize = NULL, \
  }

/// define the helper macro for defining a internal channel with lead lag filter and linearization
#define SENMANINTLDLGFILTLIN( chan, samprate, minraw, maxraw, minegu, maxegu, lagtc, leadtc, linfunc, linopt ) \
  { \
	  .eInpType = SENMAN_INPTYPE_INTANA, \
	  .nChannel = chan, \
	  .wSampleRate = samprate, \
	  .xMinRawValue = minraw, \
	  .xMaxRawValue = maxraw, \
	  .xMinEguValue = minegu, \
	  .xMaxEguValue = maxegu, \
	  .eFiltType = SENMAN_FILTTYPE_LEADLAG, \
	  .tFilters.tLeadLag = \
	  { \
		  .fLagTc = lagtc, \
		  .fLeadTc = leadtc, \
	  }, \
	  .pvLinearize = linfunc, \
    .nLinOption = linopt, \
  }
#endif // SENSORMANAGER_LEADLAG_FILTER_ENABLE

/// define the helper macro for defining an external channel no filter
#define SENMANEXTNOFILT( chan, samprate, minraw, maxraw, minegu, maxegu, getfunc ) \
  { \
    .eInpType = SENMAN_INPTYPE_EXTANA, \
    .nChannel = chan, \
    .wSampleRate = samprate, \
    .xMinRawValue = minraw, \
    .xMaxRawValue = maxraw, \
    .xMinEguValue = minegu, \
    .xMaxEguValue = maxegu, \
    .eFiltType = SENMAN_FILTTYPE_NONE, \
    .tGetFuncs.pvExt = getfunc, \
  	.pvLinearize = NULL, \
  }
  
/// define the helper macro for defining an external channel no filter with linearization
#define SENMANEXTNOFILTLIN( chan, samprate, minraw, maxraw, minegu, maxegu, getfunc, linfunc, linopt ) \
  { \
	  .eInpType = SENMAN_INPTYPE_EXTANA, \
	  .nChannel = chan, \
	  .wSampleRate = samprate, \
	  .xMinRawValue = minraw, \
	  .xMaxRawValue = maxraw, \
	  .xMinEguValue = minegu, \
	  .xMaxEguValue = maxegu, \
	  .eFiltType = SENMAN_FILTTYPE_NONE, \
	  .tGetFuncs.pvExt = getfunc, \
	  .pvLinearize = linfunc, \
    .nLinOption = linopt, \
  }
  
#if ( SENSORMANAGER_AVERAGE_FILTER_ENABLE == 1 )
/// define the helper macro for defining a external channel with average filter
#define SENMANEXTAVGFILT( chan, samprate, minraw, maxraw, minegu, maxegu, numsamps, bufname, getfunc ) \
  { \
    .eInpType = SENMAN_INPTYPE_EXTANA, \
    .nChannel = chan, \
    .wSampleRate = samprate, \
    .xMinRawValue = minraw, \
    .xMaxRawValue = maxraw, \
    .xMinEguValue = minegu, \
    .xMaxEguValue = maxegu, \
    .eFiltType = SENMAN_FILTTYPE_AVERAGE, \
    .tFilters.tAverage = \
     { \
      .nNumberSamples = numsamps, \
      .pwBuffer = ( PU16 )&aw ## bufname ## AvgFilt, \
     }, \
    .tGetFuncs.pvExt = getfunc, \
    .pvLinearize = NULL, \
  }

/// define the helper macro for defining a external channel with average filter and linearization
#define SENMANEXTAVGFILTLIN( chan, samprate, minraw, maxraw, minegu, maxegu, numsamps, bufname, getfunc, linfunc, linopt ) \
  { \
	  .eInpType = SENMAN_INPTYPE_EXTANA, \
	  .nChannel = chan, \
	  .wSampleRate = samprate, \
	  .xMinRawValue = minraw, \
	  .xMaxRawValue = maxraw, \
	  .xMinEguValue = minegu, \
	  .xMaxEguValue = maxegu, \
	  .eFiltType = SENMAN_FILTTYPE_AVERAGE, \
	  .tFilters.tAverage = \
	  { \
		  .nNumberSamples = numsamps, \
		  .pwBuffer = ( PU16 )&aw ## bufname ## AvgFilt, \
	  }, \
	  .tGetFuncs.pvExt = getfunc, \
	  .pvLinearize = linfunc, \
    .nLinOption = linopt, \
  }
#endif // SENSORMANAGER_AVERAGE_FILTER_ENABLE

#if ( SENSORMANAGER_LEADLAG_FILTER_ENABLE == 1 )
/// define the helper macro for defining a external channel with lead lag filter
#define SENMANEXTLDLGFILT( chan, samprate, minraw, maxraw, minegu, maxegu, lagtc, leadtc ) \
  { \
	  .eInpType = SENMAN_INPTYPE_EXTANA, \
	  .nChannel = chan, \
	  .wSampleRate = samprate, \
	  .xMinRawValue = minraw, \
	  .xMaxRawValue = maxraw, \
	  .xMinEguValue = minegu, \
	  .xMaxEguValue = maxegu, \
	  .eFiltType = SENMAN_FILTTYPE_LEADLAG, \
	  .tFilters.tLeadLag = \
	  { \
		  .fLagTc = lagtc, \
		  .fLeadTc = leadtc, \
	  } \
	  .pvLinearize = NULL, \
  }

/// define the helper macro for defining a external channel with lead lag filter and linearization
#define SENMANEXTLDLGFILTLIN( chan, samprate, minraw, maxraw, minegu, maxegu, lagtc, leadtc, linfunc, linopt ) \
  { \
	  .eInpType = SENMAN_INPTYPE_EXTANA, \
	  .nChannel = chan, \
	  .wSampleRate = samprate, \
	  .xMinRawValue = minraw, \
	  .xMaxRawValue = maxraw, \
	  .xMinEguValue = minegu, \
	  .xMaxEguValue = maxegu, \
	  .eFiltType = SENMAN_FILTTYPE_LEADLAG, \
	  .tFilters.tLeadLag = \
	  { \
		  .fLagTc = lagtc, \
		  .fLeadTc = leadtc, \
	  }, \
	  .pvLinearize = linfunc, \
    .nLinOption = linopt, \
  }
#endif // SENSORMANAGER_LEADLAG_FILTER_ENABLE
  
/// define the helper macro for defining a special channel no filter
#define SENMANSPCNOFILT( samprate, minraw, maxraw, minegu, maxegu, getfunc ) \
  { \
	  .eInpType = SENMAN_INPTYPE_SPCANA, \
	  .wSampleRate = samprate, \
	  .xMinRawValue = minraw, \
	  .xMaxRawValue = maxraw, \
	  .xMinEguValue = minegu, \
	  .xMaxEguValue = maxegu, \
	  .eFiltType = SENMAN_FILTTYPE_NONE, \
	  .tGetFuncs.pvSpc = getfunc, \
	  .pvLinearize = NULL, \
  }
  
/// define the helper macro for defining a special channel no filter with linearization
#define SENMANSPCNOFILTLIN( samprate, minraw, maxraw, minegu, maxegu, getfunc, linfunc, linopt ) \
  { \
   .eInpType = SENMAN_INPTYPE_SPCANA, \
   .wSampleRate = samprate, \
   .xMinRawValue = minraw, \
   .xMaxRawValue = maxraw, \
   .xMinEguValue = minegu, \
   .xMaxEguValue = maxegu, \
   .eFiltType = SENMAN_FILTTYPE_NONE, \
   .tGetFuncs.pvSpc = getfunc, \
   .pvLinearize = linfunc, \
   .nLinOption = linopt, \
  }
  
#if ( SENSORMANAGER_AVERAGE_FILTER_ENABLE == 1 )
/// define the helper macro for defining a special channel with average filter
#define SENMANSPCAVGFILT( samprate, minraw, maxraw, minegu, maxegu, numsamps, bufname, getfunc ) \
  { \
    .eInpType = SENMAN_INPTYPE_SPCANA, \
    .wSampleRate = samprate, \
    .xMinRawValue = minraw, \
    .xMaxRawValue = maxraw, \
    .xMinEguValue = minegu, \
    .xMaxEguValue = maxegu, \
    .eFiltType = SENMAN_FILTTYPE_AVERAGE, \
    .tFilters.tAverage = \
    { \
      .nNumberSamples = numsamps, \
      .pwBuffer = ( PU16 )&aw ## bufname ## AvgFilt, \
    }, \
    .tGetFuncs.pvSpc = getfunc, \
    .pvLinearize = NULL, \
  }
  
/// define the helper macro for defining a special channel with average filter and linearization
#define SENMANSPCAVGFILTLIN( samprate, minraw, maxraw, minegu, maxegu, numsamps, bufname, getfunc, linfunc, linopt ) \
  { \
	  .eInpType = SENMAN_INPTYPE_SPCANA, \
	  .wSampleRate = samprate, \
	  .xMinRawValue = minraw, \
	  .xMaxRawValue = maxraw, \
	  .xMinEguValue = minegu, \
	  .xMaxEguValue = maxegu, \
	  .eFiltType = SENMAN_FILTTYPE_AVERAGE, \
	  .tFilters.tAverage = \
	  { \
		  .nNumberSamples = numsamps, \
		  .pwBuffer = ( PU16 )&aw ## bufname ## AvgFilt, \
	  }, \
	  .tGetFuncs.pvSpc = getfunc, \
	  .pvLinearize = linfunc, \
    .nLinOption = linopt, \
  }
#endif // SENSORMANAGER_AVERAGE_FILTER_ENABLE

#if ( SENSORMANAGER_LEADLAG_FILTER_ENABLE == 1 )
/// define the helper macro for defining a special channel with lead lag filter
#define SENMANSPCLDLGFILT( samprate, minraw, maxraw, minegu, maxegu, lagtc, leadtc, getfunc ) \
  { \
	  .eInpType = SENMAN_INPTYPE_SPCANA, \
	  .wSampleRate = samprate, \
	  .xMinRawValue = minraw, \
	  .xMaxRawValue = maxraw, \
	  .xMinEguValue = minegu, \
	  .xMaxEguValue = maxegu, \
	  .eFiltType = SENMAN_FILTTYPE_LEADLAG, \
	  .tFilters.tLeadLag = \
	  { \
		  .fLagTc = lagtc, \
		  .fLeadTc = leadtc, \
	  }, \
	  .tGetFuncs.pvSpc = getfunc, \
	  .pvLinearize = NULL, \
  }

/// define the helper macro for defining a special channel with lead lag filter and linearization
#define SENMANSPCLDLGFILTLIN( samprate, minraw, maxraw, minegu, maxegu, lagtc, leadtc, getfunc, linfunc, linopt ) \
  { \
	  .eInpType = SENMAN_INPTYPE_SPCANA, \
	  .wSampleRate = samprate, \
	  .xMinRawValue = minraw, \
	  .xMaxRawValue = maxraw, \
	  .xMinEguValue = minegu, \
	  .xMaxEguValue = maxegu, \
	  .eFiltType = SENMAN_FILTTYPE_LEADLAG, \
	  .tFilters.tLeadLag = \
	  { \
		  .fLagTc = lagtc, \
		  .fLeadTc = leadtc, \
	  }, \
	  .tGetFuncs.pvSpc = getfunc, \
	  .pvLinearize = linfunc, \
    .nLinOption = linopt, \
  }
#endif // SENSORMANAGER_LEADLAG_FILTER_ENABLE
  
// enumerations ---------------------------------------------------------------
/// enumerate the analog input type
typedef enum _SENMANINPTYPE
{
  SENMAN_INPTYPE_INTANA = 0,      ///< internal analog
  SENMAN_INPTYPE_EXTANA,          ///< external analog
  SENMAN_INPTYPE_SPCANA,          ///< special analog
  SENMAN_INPTYPE_MAX
} SENMANINPTYPE;

/// enumerate the filter types
typedef enum _SENMANFILTTYPE
{
  SENMAN_FILTTYPE_NONE = 0,       ///< no filter applied
  #if ( SENSORMANAGER_AVERAGE_FILTER_ENABLE == 1 )
  SENMAN_FILTTYPE_AVERAGE,        ///< averaging filter
  #endif // SENSORMANAGER_AVERAGE_FILTER_ENABLE
  #if ( SENSORMANAGER_LEADLAG_FILTER_ENABLE == 1 )
    SENMAN_FILTTYPE_LEADLAG,        ///< lead lag filter
  #endif // SENSORMANAGER_LEADLAG_FILTER_ENABLE
  #if ( SENSORMANAGER_FIR_FILTER_ENABLE == 1 )
    SENMAN_FILTTYPE_FIR,            ///< FIR filter
  #endif // SENSORMANAGER_FIR_FILTER_ENABLE
  #if ( SENSORMANAGER_IIR_FILTER_ENABLE == 1 )
    SENMAN_FILTTYPE_IIR,            ///< IIR filter
  #endif // SENSORMANAGER_IIR_FILTER_ENABLE
  #if ( SENSORMANAGER_USR_FILTER_ENABLE == 1 )
    SENMAN_FILTTYPE_USER,           ///< user filter type
  #endif // SENSORMANAGER_USR_FILTER_ENABLE
  SENMAN_FILTTYPE_MAX,
} SENMANFILTTYPE;   

// structures -----------------------------------------------------------------
/// define the argument type
#if ( SENSORMANAGER_ARGUMENT_TYPE == SENSORMANAGER_TYPE_SIGNED16 )
  typedef S16     SENMANARG;
  typedef PS16    PSENMANARG;
#elif ( SENSORMANAGER_ARGUMENT_TYPE == SENSORMANAGER_TYPE_SIGNED32 )
  typedef S32     SENMANARG;
  typedef PS32    PSENMANARG;
#elif ( SENSORMANAGER_ARGUMENT_TYPE == SENSORMANAGER_TYPE_FLOAT )
  typedef FLOAT   SENMANARG;
  typedef PFLOAT  PSENMANARG;
#else
  typedef S16     SENMANARG;
  typedef PS16    PSENMANARG;
#endif // SENSORMANAGER_ARGUMENT_TYPE

/// define the external function prototype
typedef SENMANARG   ( *PVSENMANGEREXT )( U8 nChannel );
typedef SENMANARG   ( *PVSENMANGERSPC )( void );
typedef SENMANARG   ( *PVSENMANGERLIN )( U16 wValue, U8 nOption );

/// define the sensor definition structure
typedef struct _SENMANDEF
{
  SENMANINPTYPE     eInpType;     ///< sensor input type
  U8                nChannel;     ///< channel
  U16               wSampleRate;  ///< sample rate in millisecondes
  SENMANFILTTYPE    eFiltType;    ///< filter type
  SENMANARG         xMinRawValue; ///< minimum raw value from sensor
  SENMANARG         xMaxRawValue; ///< maximum raw value from sensor
  SENMANARG         xMinEguValue; ///< minimum engineering units from sensor
  SENMANARG         xMaxEguValue; ///< maximum engineering units from sensor
  union
  {
    #if ( SENSORMANAGER_AVERAGE_FILTER_ENABLE == 1 )
      FILTAVERAGEDEF  tAverage;     /// average definition
    #endif // SENSORMANAGER_AVERAGE_FILTER_ENABLE
    #if ( SENSORMANAGER_LEADLAG_FILTER_ENABLE == 1 )
      FILTLDLGDEF     tLeadLag;     /// lead/lag definition
    #endif // SENSORMANAGER_LEADLAG_FILTER_ENABLE
    #if ( SENSORMANAGER_FIR_FILTER_ENABLE == 1 )
      SENMANFIRFILT   tFir;         /// FIR definition
    #endif // SENSORMANAGER_FIR_FILTER_ENABLE
    #if ( SENSORMANAGER_IIR_FILTER_ENABLE == 1 )
      SENMANIIRFILT   tIir;         /// IIR definition
    #endif // SENSORMANAGER_IIR_FILTER_ENABLE
    #if ( SENSORMANAGER_USR_FILTER_ENABLE == 1 )
      SENMANUSERFILT  tUser;        /// user definition
    #endif // SENSORMANAGER_USR_FILTER_ENABLE
  } tFilters;
  union
  {
    PVSENMANGEREXT    pvExt;      /// external channel get function
    PVSENMANGERSPC    pvSpc;      /// special get function
  } tGetFuncs;
  PVSENMANGERLIN      pvLinearize;  ///< pointer to the lineraize function
  U8                  nLinOption; ///< lineratization option
} SENMANDEF, *PSENMANDEF;
#define SENMANDEF_SIZE        sizeof( SENMANDEF )

/**@} EOF SensorManager_def.h */

#endif  // _SENSORMANAGER_DEF_H