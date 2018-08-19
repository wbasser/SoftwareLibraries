/******************************************************************************
 * @file Sensormanager.c
 *
 * @brief sensor manager implementation
 *
 * This file provides the sensor manager implementations
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
 * \addtogroup Sensormanager
 * @{
 *****************************************************************************/

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "SensorManager/SensorManager.h"

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------
/// define the FIR filter control structure
typedef struct _FIRFILTCTL
{
} FIRFILTCTL, *PFIRFILTCTL;
#define FIRFILTCTL_SIZE   sizeof ( FIRFILTCTL )

/// define the IIR filter control structure
typedef struct _IIRFILTCTL
{
} IIRFILTCTL, *PIIRFILTCTL;
#define IIRFILTCTL_SIZE   sizeof ( IIRFILTCTL )

/// define the overall control structure
typedef struct _SENMANCTL
{
  U32         uDesExecCount;      ///< desired execuiton counts
  U32         uCurExecCount;      ///< current execution counts
  SENMANARG   xCurrentValue;      ///< current value
  BOOL        bValid;             ///< valid
  union
  {
    #if ( SENSORMANAGER_AVERAGE_FILTER_ENABLE == 1 )
    FILTAVERAGECTL  tAverage;   ///< average control
    #endif
    #if ( SENSORMANAGER_LEADLAG_FILTER_ENABLE == 1 )
      FILTLDLGCTL     tLeadLag;   ///< lead/lag control
    #endif
    #if ( SENSORMANAGER_FIR_FILTER_ENABLE == 1 )
      FIRFILTCTL      tFir;       ///< FIR control
    #endif
    #if ( SENSORMANAGER_IIR_FILTER_ENABLE == 1 )
      IIRFILTCTL      tIir;       ///< IIR control
    #endif
  } tFilters;
} SENMANCTL, *PSENMANCTL;
#define SENMANCTL_SIZE    sizeof( SENMANCTL )

// global parameter declarations ----------------------------------------------

// local parameter declarations -----------------------------------------------
static  SENMANCTL   atCtls[ SENMAN_ENUM_MAX ];

// local function prototypes --------------------------------------------------

// constant parameter initializations -----------------------------------------

/******************************************************************************
 * @function SensorManager_Initialize
 *
 * @brief sensor manager initialization
 *
 * This function will initialize the sensor manager
 *
 *****************************************************************************/
void SensorManager_Initialize( void )
{
  PSENMANDEF  ptDef;
  PSENMANCTL  ptCtl;
  SENMANENUM  eSensor;
  
  // call the local initialization
  SensorManager_LocalInitialize( );
  
  // for each sensor
  for ( eSensor = 0; eSensor < SENMAN_ENUM_MAX; eSensor++ )
  {
    // get a pointer to the definition/control
    ptDef = ( PSENMANDEF )&atSensorMngrDefs[ eSensor ];
    ptCtl = ( PSENMANCTL )&atCtls[ eSensor ];
    
    // now calculate the sample time
    ptCtl->uDesExecCount = PGM_RDWORD( ptDef->wSampleRate ) / SENSORMANAGER_EXECUTION_RATE_MSEC;
    ptCtl->uCurExecCount = 0;
    ptCtl->bValid = FALSE;
    
    // determine the type of filter
    switch( PGM_RDBYTE( ptDef->eFiltType ))
    {
      #if ( SENSORMANAGER_AVERAGE_FILTER_ENABLE == 1 )
      case SENMAN_FILTTYPE_AVERAGE :
        // clear the index/total
        ptCtl->tFilters.tAverage.nSampleIndex = 0;
        ptCtl->tFilters.tAverage.uSum = 0;
        ptCtl->tFilters.tAverage.tDef.pwBuffer = PGM_RDWORD( ptDef->tFilters.tAverage.pwBuffer );
        ptCtl->tFilters.tAverage.tDef.nNumberSamples = PGM_RDBYTE( ptDef->tFilters.tAverage.nNumberSamples );
        break;
    #endif // SENSORMANAGER_AVERAGE_FILTER_ENABLE
        
    #if ( SENSORMANAGER_LEADLAG_FILTER_ENABLE == 1 )
      case SENMAN_FILTTYPE_LEADLAG :
	  	// clear the values
	  	ptCtl->tFilters.tLeadLag.fPrvValue 	= 0.0;
	  	ptCtl->tFilters.tLeadLag.fPrvOutput = 0.0;
	  	ptCtl->tFilters.tLeadLag.fOutput    = 0.0;
        break;
    #endif
        
    #if ( SENSORMANAGER_FIR_FILTER_ENABLE == 1 )
      case SENMAN_FILTTYPE_FIR :
        break;
    #endif
        
    #if ( SENSORMANAGER_IIR_FILTER_ENABLE == 1 )
      case SENMAN_FILTTYPE_IIR :
        break;
    #endif
        
    #if ( SENSORMANAGER_USR_FILTER_ENABLE == 1 )
      case SENMAN_FILTTYPE_USER :
        break;
    #endif

      case SENMAN_FILTTYPE_NONE :
      default :
        break;
    }
  }
}

/******************************************************************************
 * @function SensorManager_ProcessScan
 *
 * @brief scan all configured sensors
 *
 * This function will scan and process all configured sensors
 *
 *****************************************************************************/
void SensorManager_ProcessScan( void )
{
  PSENMANDEF      ptDef;
  PSENMANCTL      ptCtl;
  SENMANENUM      eSensor;
  S32             lSenValue;
  PVSENMANGEREXT  pvExtGet;
  PVSENMANGERSPC  pvSpcGet;
  
  // for each sensor
  for ( eSensor = 0; eSensor < SENMAN_ENUM_MAX; eSensor++ )
  {
    // get a pointer to the definition/control
    ptDef = ( PSENMANDEF )&atSensorMngrDefs[ eSensor ];
    ptCtl = ( PSENMANCTL )&atCtls[ eSensor ];
    
    // now increment the sensor time/check for ready
    if ( ++ptCtl->uCurExecCount == ptCtl->uDesExecCount )
    {
      // reset the time
      ptCtl->uCurExecCount = 0;
      
      // get a value based on type
      switch( PGM_RDBYTE( ptDef->eInpType ))
      {
        case SENMAN_INPTYPE_INTANA :
          lSenValue = ( U32 )SensorManager_InternalAdcConvertChannel( PGM_RDBYTE( ptDef->nChannel ) );
          break;
          
        case SENMAN_INPTYPE_EXTANA :
          pvExtGet = PGM_RDWORD( ptDef->tGetFuncs.pvExt );
          lSenValue = ( U32 )pvExtGet( PGM_RDBYTE( ptDef->nChannel ));
          break;
          
        case SENMAN_INPTYPE_SPCANA :
          pvSpcGet = PGM_RDWORD( ptDef->tGetFuncs.pvSpc );
          lSenValue = ( U32 )pvSpcGet( );
          break;
          
        default :
          lSenValue = 0;
          break;
      }
      
      // determine the type of filter
      switch( PGM_RDBYTE( ptDef->eFiltType ))
      {
      #if ( SENSORMANAGER_AVERAGE_FILTER_ENABLE == 1 )
        case SENMAN_FILTTYPE_AVERAGE :
          ptCtl->bValid = FilterAveraging_Execute( &ptCtl->tFilters.tAverage, lSenValue );
          break;
      #endif
          
      #if ( SENSORMANAGER_LEADLAG_FILTER_ENABLE == 1 )
        case SENMAN_FILTTYPE_LEADLAG :
          ptCtl->bValid = FilterLeadLag_Execute( &ptCtl->tFilters.tLeadLag, lSenValue, ( U32 )TASK_TIME_MSECS( PGM_RDWORD( ptDef->wSampleRate )));
          break;
      #endif
          
      #if ( SENSORMANAGER_FIR_FILTER_ENABLE == 1 )
        case SENMAN_FILTTYPE_FIR :
          break;
      #endif
          
      #if ( SENSORMANAGER_IIR_FILTER_ENABLE == 1 )
        case SENMAN_FILTTYPE_IIR :
          break;
      #endif
          
      #if ( SENSORMANAGER_USR_FILTER_ENABLE == 1 )
        case SENMAN_FILTTYPE_USER :
          break;
      #endif

        case SENMAN_FILTTYPE_NONE :
          ptCtl->bValid = TRUE;
          ptCtl->xCurrentValue = ( SENMANARG )lSenValue;
          break;
          
        default :
          break;
      }
    }
  }
}

/******************************************************************************
 * @function SensorManager_GetValue
 *
 * @brief get a value
 *
 * This function will return the last good value of a given sensor
 *
 * @param[in]   eSenNum   sensor enumeration
 * @param[io]   pxValue   pointer to the storage to return the value
 *
 * @return      appropriate error
 *
 *****************************************************************************/
SENMANERROR SensorManager_GetValue( SENMANENUM eSenEnum, PSENMANARG pxValue )
{
  SENMANERROR eError = SENMAN_ERROR_NONE;
  PSENMANDEF      ptDef;
  PSENMANCTL      ptCtl;
  SENMANARG       xMinRawValue, xMaxRawValue, xMinEguValue, xMaxEguValue;
  
  // clear the return value
  *( pxValue ) = 0;
  
  // check for a valid sensor enum
  if ( eSenEnum < SENMAN_ENUM_MAX )
  {
    // get a pointer to the definition/control
    ptDef = ( PSENMANDEF )&atSensorMngrDefs[ eSenEnum ];
    ptCtl = ( PSENMANCTL )&atCtls[ eSenEnum ];
    
    // check for valid
    if ( ptCtl->bValid )
    {
      // Determine the filter type
      switch( PGM_RDBYTE( ptDef->eFiltType ))
      {
        // get the value
        #if ( SENSORMANAGER_AVERAGE_FILTER_ENABLE == 1 )
        case SENMAN_FILTTYPE_AVERAGE :
          ptCtl->xCurrentValue = FilterAveraging_GetValue( &ptCtl->tFilters.tAverage );
          break;
      #endif

      #if ( SENSORMANAGER_LEADLAG_FILTER_ENABLE == 1 )
        case SENMAN_FILTTYPE_LEADLAG :
          ptCtl->xCurrentValue = FilterLeadLag_GetValue( &ptCtl->tFilters.tLeadLag );
          break;
      #endif
        
        default :
          break;
      }
      
      // apply the scaling correction 
      xMinRawValue = PGM_RDWORD( ptDef->xMinRawValue );
      xMaxRawValue = PGM_RDWORD( ptDef->xMaxRawValue );
      xMinEguValue = PGM_RDWORD( ptDef->xMinEguValue );
      xMaxEguValue = PGM_RDWORD( ptDef->xMaxEguValue );
      if (( xMinRawValue != xMinEguValue ) && ( xMaxRawValue != xMaxEguValue ))
      {
        // scale it
        *( pxValue ) = MAP( ptCtl->xCurrentValue, xMinRawValue, xMaxRawValue, xMinEguValue, xMaxEguValue );
      }
      else
      {
        // return the current value
        *( pxValue )= ptCtl->xCurrentValue;
      }
    }
    else
    {
      // return the invalid value error
      eError = SENMAN_ERROR_VALNOTVALID;
    }
  }
  else
  {
    // illegal enumeration
    eError = SENMAN_ERROR_ILLENUM;
  }
  
  // return the error
  return( eError );
}

/**@} EOF Sensormanager.c */
