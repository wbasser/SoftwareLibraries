/******************************************************************************
 * @file AlsentisHssTouch.c
 *
 * @brief Alsentis HSS touch library implementation 
 *
 * This file provides the implmentation for the HSS touch library
 *
 * @copyright Copyright (c) 2012 Nan Technologies
 * This document contains proprietary data and information of Nan Technologies 
 * LLC. It is the exclusive property of Nan Technologies, LLC and will not be 
 * disclosed in any form to any party without prior written permission of 
 * Nan Technologies, LLC. This document may not be reproduced or further used 
 * without the prior written permission of Nan Technologies, LLC.
 *
 * $Date: $
 *
 * Version History
 * ======
 * $Rev: $
 * 
 *
 * \addtogroup AlsentisHssTouch
 * @{
 *****************************************************************************/

// system includes ------------------------------------------------------------

// library includes -----------------------------------------------------------
#include "StateExecutionEngine/StateExecutionEngine.h"
#if ( ALSENTISHSSTOUCH_OPER_METHOD == ALSENTISHSSTOUCH_OPER_I2CPERIPH )
#include "QueueManager/QueueManager.h"
#endif // ALSENTISHSSTOUCH_OPER_METHOD

// local includes -------------------------------------------------------------
#include "AlsentisHssTouch/AlsentisHssTouch.h"
#include "AlsentisHssTouch/AlsentisHssTouch_prv.h"

// Macros and Defines ---------------------------------------------------------
/// define the differential change from start initial value
#define DIFF_CHANGE_FROM_START_INIT         ( -8388607 )

/// define the macro to test for enabled sensor
#define IS_SENSOR_ENABLE( sensors, idx )    (( sensors & BIT( idx )) ? TRUE : FALSE )

// enumerations ---------------------------------------------------------------
/// enuemrate the states
typedef enum LCLSTATE
{
  LCL_STATE_IDLE = 0,         ///< idle state
  LCL_STATE_CHARGE,           ///< charge the sensors
  LCL_STATE_ADCCONV,          ///< process the ADC convert
  LCL_STATE_PROCESS,          ///< process
  LCL_STATE_MAX               
} LCLSTATE;

// structures -----------------------------------------------------------------
/// define the control structure
typedef struct _LCLCTL
{
  U16         wSampleValue;                 ///< sample value
  S32         lBtnAccum1;                   ///< button accumulator 1
  S32         lBtnAccum2;                   ///< button accumulator 2
  S32         lSortedNoDiffChangeFromStart; ///< sorted no diff change from start
  S32         lNoDiffCurrSample;            ///< differential current sample
  S32         lNoDiffChangeFromStart;       ///< differential change from start
  S32         lNoDiffStartValue;            ///< differential start value
  S32         lNoDiffLastValue;             ///< differential last value
  PS32        plBtnAccumRead;               ///< read pointer
  PS32        plBtnAccumWrite;              ///< write pointer
  S32         lLastSample;                  ///< last sample
  U32         uLatchUpCount;                ///< latch up count
  HSSBTNSTATE eBtnState;                    ///< button state
  S32         lStartValue;                  ///< start value
  S32         lEndValue;                    ///< end value
  S32         lBiggestDvdt;                 ///< biggest DVDT
  S16         iNoTouchZone;                 ///< no touch zone
  S8          cCurFastDvdtThreshold;        ///< current fast DVDT threshold
  S8          cCurAbortBoundaries;          ///< current abort boundaries
	U8          nlevelOffDownMin;             ///< level off down minimum
	BOOL        bIsFastTouch;                 ///< set if fast touch
	U8          nTooSlowCntr;                 ///< too slow counter
	U8          nMovementCntr;                ///< movement counter
	U8          nLevelOffCntr;                ///< level off counter
	U8          nAbortTouchCntr;              ///< abort touch counter
  U16         wNoTouchZone;                 ///< no touch zone
} LCLCTL, *PLCLCTL;
#define LCLCTL_SIZE       sizeof( LCLCTL )

// global parameter declarations ----------------------------------------------

// local parameter declarations -----------------------------------------------
static  ALSENTISCONFIG      tConfig;
static  SENSORVAR           xSensorStates;
static  U16                 wIntervalCount;
static  U16                 wIntervalTime;
static  U16                 awSampleValues[ ALSENTISHSSTOUCH_NUM_SENSORS ];
static  STATEEXECENGCONTROL tStateControl;
static  U8                  nSensorIndex;
static  U8                  nOutgoingChecksum;
static  U8                  nStatus;
static  U8                  nSampleIndex;
static  LCLCTL              atLclCtl[ ALSENTISHSSTOUCH_NUM_SENSORS ];

// local function prototypes --------------------------------------------------
#if ( ALSENTISHSSTOUCH_OPER_METHOD == ALSENTISHSSTOUCH_OPER_I2CPERIPH )
static  U8    AddToBuffer( U8 nData, PU8 pnBuffer, U8 nCurIndex, U8 nMaxSize );
static  U8    CalculateAndStuffChecksum( PU8 pnBuffer, U8 nCurSize );
#endif // ALSENTISHSSTOUCH_OPER_METHOD
static  void  ProcessData( void );

/// idle state functions
static  U8    LclStateIdle_Exc( STATEEXECENGARG xArg );
static  void  LclStateIdle_Ext( void );

/// AtoD convert state
static  void  LclStateAdcConv_Ent( void );
static  U8    LclStateAdcConv_Exc( STATEEXECENGARG xArg );
static  void  LclStateAdcConv_Ext( void );

/// Process state
static  void  LclStateProcess_Ent( void );

// constant parameter initializations -----------------------------------------
static  const CODE STATEEXECENGEVENT  atChargeEvents[ ] =
{
  STATEEXECENGEVENT_ENTRY( ALSENTISHSSTOUCH_EVENT_CHARGING_DONE, LCL_STATE_ADCCONV, FALSE ),
  STATEEXECENGEVENT_ENTRY( 0,                                     0,                FALSE ),
};

static  const CODE STATEEXECENGEVENT  atProcessEvents[ ] =
{
  STATEEXECENGEVENT_ENTRY( ALSENTISHSSTOUCH_EVENT_PROCESSING_DONE, LCL_STATE_IDLE, FALSE ),
  STATEEXECENGEVENT_ENTRY( 0,                                      0,              FALSE ),
};

/// define the state table
static  const CODE STATEEXECENGTABLE  atStates[ LCL_STATE_MAX ] =
{
  STATEXECENGETABLE_ENTRY( LCL_STATE_IDLE,    NULL,                LclStateIdle_Exc,    LclStateIdle_Ext,    NULL             ),
  STATEXECENGETABLE_ENTRY( LCL_STATE_CHARGE,  NULL,                NULL,                NULL,                &atChargeEvents  ),
  STATEXECENGETABLE_ENTRY( LCL_STATE_ADCCONV, LclStateAdcConv_Ent, LclStateAdcConv_Exc, LclStateAdcConv_Ext, NULL             ),
  STATEXECENGETABLE_ENTRY( LCL_STATE_PROCESS, LclStateProcess_Ent, NULL,                NULL,                &atProcessEvents ),
};

// defaults
#if ( ALSENTISHSSTOUCH_CONFIG_METHOD == ALSENTISHSSTOUCH_CONFIGMGR_SEL )
static  const CODE  ALSENTISCONFIG  tTouchDefDflts =
{
  .bIrqEnabled              = FALSE,
  .bDviMode                 = FALSE,
  .nLatchTimeoutSecs        = 0,
  .wProcessIntervalUsecs    = 5000,
  .wNumberSampleSets        = 1,
  .nDiffSensorNumber        = 0,
  #if ( ALSENTISHSSTOUCH_NUM_SENSORS > 8 )
  .xDiffSensors             = 0xFFFF,
  .xEnabledSensors          = 0xFFFF,
  #else
  .xDiffSensors             = 0xFF,
  .xEnabledSensors          = 0xFF,
  #endif
  .tId                      =
  {
    .wType                  = 20,
    .wVersion               = 1,
  },
  .tPulseConfig             =
  {
    .nNumberOfPulses        = 30,
    .tPulse                 =
    {
      .tFields              =
      {
        .wPulsePeriod       = 1000,
        .wDutyCycle         = ALSENTIS_DUTY_CYCLE_50,
      },
    },
  },
  .tPowerMode               =
  {
    .tFields                =
    {
      .nWake                = ALSENTIS_WAKE_MODE_DONT,
      .nPower               = ALSENTIS_POWER_MODE_NORMAL,
    },
  },
  .tHss1Config              = 
  {
    .nApproachContMin       = 3,
    .nSlowLvlOffCntMin      = 4,
    .nFastLvlOffCntMin      = 2,
    .nNoTouchDebounceMin    = 5,
  },
  .tHss2Config              =
  {
    .nFastTglDvDtDeviation  = 50,
    .nAbortTouchPercent     = 20,
    .nAbortTouchCountMax    = 1,
    .nTooSlowCountMax       = 3,
  },
  .tSleep                   =
  {
    .nOneTime               = 0,
    .nRepeat                = 0,
  },
  #if ( ALSENTISHSSTOUCH_NUM_SENSORS >= 1 )
  .atZones[ 0 ]             =
  {
    .wTrzMultiplier         = 5,
    .wTrzDivider            = 5,
    .wTrzTouchZone          = 60,
  },
  #endif // ALSENTISHSSTOUCH_NUM_SENSORS
  #if ( ALSENTISHSSTOUCH_NUM_SENSORS >= 2 )
  .atZones[ 1 ]             =
  {
    .wTrzMultiplier         = 5,
    .wTrzDivider            = 5,
    .wTrzTouchZone          = 60,
  },
  #endif // ALSENTISHSSTOUCH_NUM_SENSORS
  #if ( ALSENTISHSSTOUCH_NUM_SENSORS >= 3 )
  .atZones[ 2 ]             =
  {
    .wTrzMultiplier         = 5,
    .wTrzDivider            = 5,
    .wTrzTouchZone          = 60,
  },
  #endif // ALSENTISHSSTOUCH_NUM_SENSORS
  #if ( ALSENTISHSSTOUCH_NUM_SENSORS >= 4 )
  .atZones[ 3 ]             =
  {
    .wTrzMultiplier         = 5,
    .wTrzDivider            = 5,
    .wTrzTouchZone          = 60,
  },
  #endif // ALSENTISHSSTOUCH_NUM_SENSORS
  #if ( ALSENTISHSSTOUCH_NUM_SENSORS >= 5 )
  .atZones[ 4 ]             =
  {
    .wTrzMultiplier         = 5,
    .wTrzDivider            = 5,
    .wTrzTouchZone          = 60,
  },
  #endif // ALSENTISHSSTOUCH_NUM_SENSORS
  #if ( ALSENTISHSSTOUCH_NUM_SENSORS >= 6 )
  .atZones[ 5 ]             =
  {
    .wTrzMultiplier         = 5,
    .wTrzDivider            = 5,
    .wTrzTouchZone          = 60,
  },
  #endif // ALSENTISHSSTOUCH_NUM_SENSORS
  #if ( ALSENTISHSSTOUCH_NUM_SENSORS >= 7 )
  .atZones[ 6 ]             =
  {
    .wTrzMultiplier         = 5,
    .wTrzDivider            = 5,
    .wTrzTouchZone          = 60,
  },
  #endif // ALSENTISHSSTOUCH_NUM_SENSORS
  #if ( ALSENTISHSSTOUCH_NUM_SENSORS >= 8 )
  .atZones[ 7 ]             =
  {
    .wTrzMultiplier         = 5,
    .wTrzDivider            = 5,
    .wTrzTouchZone          = 60,
  },
  #endif // ALSENTISHSSTOUCH_NUM_SENSORS
  #if ( ALSENTISHSSTOUCH_NUM_SENSORS >= 9 )
  .atZones[ 8 ]             =
  {
    .wTrzMultiplier         = 5,
    .wTrzDivider            = 5,
    .wTrzTouchZone          = 60,
  },
  #endif // ALSENTISHSSTOUCH_NUM_SENSORS
  #if ( ALSENTISHSSTOUCH_NUM_SENSORS >= 10 )
  .atZones[ 9 ]             =
  {
    .wTrzMultiplier         = 5,
    .wTrzDivider            = 5,
    .wTrzTouchZone          = 60,
  },
  #endif // ALSENTISHSSTOUCH_NUM_SENSORS
  #if ( ALSENTISHSSTOUCH_NUM_SENSORS >= 11 )
  .atZones[ 10 ]             =
  {
    .wTrzMultiplier         = 5,
    .wTrzDivider            = 5,
    .wTrzTouchZone          = 60,
  },
  #endif // ALSENTISHSSTOUCH_NUM_SENSORS
  #if ( ALSENTISHSSTOUCH_NUM_SENSORS >= 12 )
  .atZones[ 11 ]             =
  {
    .wTrzMultiplier         = 5,
    .wTrzDivider            = 5,
    .wTrzTouchZone          = 60,
  },
  #endif // ALSENTISHSSTOUCH_NUM_SENSORS
  #if ( ALSENTISHSSTOUCH_NUM_SENSORS >= 13 )
  .atZones[ 12 ]             =
  {
    .wTrzMultiplier         = 5,
    .wTrzDivider            = 5,
    .wTrzTouchZone          = 60,
  },
  #endif // ALSENTISHSSTOUCH_NUM_SENSORS
  #if ( ALSENTISHSSTOUCH_NUM_SENSORS >= 14 )
  .atZones[ 13 ]             =
  {
    .wTrzMultiplier         = 5,
    .wTrzDivider            = 5,
    .wTrzTouchZone          = 60,
  },
  #endif // ALSENTISHSSTOUCH_NUM_SENSORS
  #if ( ALSENTISHSSTOUCH_NUM_SENSORS >= 15 )
  .atZones[ 14 ]             =
  {
    .wTrzMultiplier         = 5,
    .wTrzDivider            = 5,
    .wTrzTouchZone          = 60,
  },
  #endif // ALSENTISHSSTOUCH_NUM_SENSORS
  #if ( ALSENTISHSSTOUCH_NUM_SENSORS >= 16 )
  .atZones[ 15 ]             =
  {
    .wTrzMultiplier         = 5,
    .wTrzDivider            = 5,
    .wTrzTouchZone          = 60,
  },
  #endif // ALSENTISHSSTOUCH_NUM_SENSORS
  #if ( ALSENTISHSSTOUCH_NUM_SENSORS >= 1 )
  .atZones[ 0 ]             =
  {
    .wTrzMultiplier         = 5,
    .wTrzDivider            = 5,
    .wTrzTouchZone          = 60,
  },
  #endif // ALSENTISHSSTOUCH_NUM_SENSORS
};
#endif // ALSENTISHSSTOUCH_CONFIG_METHOD

/******************************************************************************
 * @function AlsentisHssTouch_Initialize
 *
 * @brief initialization
 *
 * This function will perform any necessary module initilaization
 *
 *****************************************************************************/
void AlsentisHssTouch_Initialize( void )
{
  // clear the sensor states
  xSensorStates = 0;
  nSampleIndex = 0;

  // set the current mode
  tStateControl.nCurState = LCL_STATE_IDLE;
  
  // set the state table in the state control structure
  tStateControl.ptStates = ( STATEEXECENGTABLE const* )&atStates;
  
  // initialize the state controller
  StateExecutionEngine_Initialize( &tStateControl, LCL_STATE_IDLE );
}

#if ( ALSENTISHSSTOUCH_CONFIG_METHOD == ALSENTISHSSTOUCH_CONFIGMGR_SEL )
/******************************************************************************
 * @function AlsentisHSSTouch_GetConfigDflt
 *
 * @brief get the configuration defaults
 *
 * This function will return a pointer to the default configuration
 *
 *****************************************************************************/
PALSENTISCONFIG AlsentisHSSTouch_GetConfigDflt( void )
{
  // return the default table
  return(( PALSENTISCONFIG )&tTouchDefDflts );
}

/******************************************************************************
 * @function AlsentisHSSTouch_GetConfigActl
 *
 * @brief get the configuration actuals
 *
 * This function will return a pointer to the actual configuration
 *
 *****************************************************************************/
PALSENTISCONFIG AlsentisHSSTouch_GetConfigActl( void )
{
  // return the actual table
  return(( PALSENTISCONFIG )&tConfig );
}
#endif // ALSENTISHSSTOUCH_CONFIG_METHOD

/******************************************************************************
 * @function AlsentisHssTouch_Configure
 *
 * @brief configuration
 *
 * This function will perform any configuration 
 *
 *****************************************************************************/
void AlsentisHssTouch_Configure( void )
{
  #if ( ALSENTISHSSTOUCH_CONFIG_METHOD == ALSENTISHSSTOUCH_PARAMMGR_SEL )
  PARAMARG  xParamVal;

  // get the global parameters from the parameter handler
  ParameterManager_GetValue( PARAMSEL_ENUM_HSS_IRQ_ENABLED, &xParamVal );
  tConfig.bIrqEnabled = ( BOOL )xParamVal;
  ParameterManager_GetValue( PARAMSEL_ENUM_HSS_DVIMODE_ENABLED, &xParamVal );
  tConfig.bDviMode = ( BOOL )xParamVal;
  ParameterManager_GetValue( PARAMSEL_ENUM_HSS_LATCH_TIMEOUT, &xParamVal );
  tConfig.nLatchTimeoutSecs = xParamVal;
  tConfig.wProcessIntervalUsecs = ParameterManager_GetU16Data( PARAMSEL_ENUM_HSS_PROC_INTERVAL_LSB );
  tConfig.wNumberSampleSets = ParameterManager_GetU16Data( PARAMSEL_ENUM_HSS_NUM_SAMPLES_LSB );

  // get the enabled sensors
  tConfig.xEnabledSensors = ParameterManager_GetU16Data( PARAMSEL_ENUM_HSS_ENABLED_SENSORS_LSB );

  // get the id type
  tConfig.tId.wType = ParameterManager_GetU16Data( PARAMSEL_ENUM_HSS_TYPE_ID_LSB );
  tConfig.tId.wVersion = ParameterManager_GetU16Data( PARAMSEL_ENUM_HSS_TYPE_VERSION_LSB );
  
  // get the pulse config
  ParameterManager_GetValue( PARAMSEL_ENUM_HSS_NUM_PULSES, &xParamVal );
  tConfig.tPulseConfig.nNumberOfPulses = xParamVal;
  tConfig.tPulseConfig.tPulse.tFields.wPulsePeriod = ParameterManager_GetU16Data( PARAMSEL_ENUM_HSS_PULSE_PERIOD_LSB );
  ParameterManager_GetValue( PARAMSEL_ENUM_HSS_PULSE_DUTYCYCLE, &xParamVal );
  tConfig.tPulseConfig.tPulse.tFields.wDutyCycle = xParamVal;

  // get the power mode
  ParameterManager_GetValue( PARAMSEL_ENUM_HSS_POWER_MODE_WAKE, &xParamVal );
  tConfig.tPowerMode.tFields.nPower = xParamVal;
  ParameterManager_GetValue( PARAMSEL_ENUM_HSS_POWER_MODE_POWER, &xParamVal );
  tConfig.tPowerMode.tFields.nWake = xParamVal;

  // get the HSS signature config #1
  ParameterManager_GetValue( PARAMSEL_ENUM_HSS_SIGCFG1_APPROACH_CNT_MIN, &xParamVal );
  tConfig.tHss1Config.nApproachContMin = xParamVal;
  ParameterManager_GetValue( PARAMSEL_ENUM_HSS_SIGCFG1_SLOW_LVLOFF_CNT_MIN, &xParamVal );
  tConfig.tHss1Config.nSlowLvlOffCntMin = xParamVal;
  ParameterManager_GetValue( PARAMSEL_ENUM_HSS_SIGCFG1_FAST_LVLOFF_CNT_MIN, &xParamVal );
  tConfig.tHss1Config.nFastLvlOffCntMin = xParamVal;
  ParameterManager_GetValue( PARAMSEL_ENUM_HSS_SIGCFG1_NOTOUCH_DBNC_MIN, &xParamVal );
  tConfig.tHss1Config.nNoTouchDebounceMin = xParamVal;

  // get the HSS signature config #2
  ParameterManager_GetValue( PARAMSEL_ENUM_HSS_SIGCFG2_FAST_TGLDVDT_DEV, &xParamVal );
  tConfig.tHss2Config.nFastTglDvDtDeviation = xParamVal;
  ParameterManager_GetValue( PARAMSEL_ENUM_HSS_SIGCFG2_ABORT_TOUCH_PCT, &xParamVal );
  tConfig.tHss2Config.nAbortTouchCountMax = xParamVal;
  ParameterManager_GetValue( PARAMSEL_ENUM_HSS_SIGCFG2_ABORT_TOUCH_COUNT_MAX, &xParamVal );
  tConfig.tHss2Config.nAbortTouchCountMax = xParamVal;
  ParameterManager_GetValue( PARAMSEL_ENUM_HSS_SIGCFG2_TOO_SLOW_COUNT_MAX, &xParamVal );
  tConfig.tHss2Config.nTooSlowCountMax = xParamVal;

  // get the sleep mode
  ParameterManager_GetValue( PARAMSEL_ENUM_HSS_SLEEP_ONE_TIME, &xParamVal );
  tConfig.tSleep.nOneTime = xParamVal;
  ParameterManager_GetValue( PARAMSEL_ENUM_HSS_SLEEP_REPEAT, &xParamVal );
  tConfig.tSleep.nRepeat = xParamVal;

  // get the zones
  for ( nSensorIndex = 0; nSensorIndex < ALSENTISHSSTOUCH_NUM_SENSORS; nSensorIndex++ )
  {
    // get the values
    tConfig.atZones[ nSensorIndex ].wTrzMultiplier = ParameterManager_GetU16Data( PARAMSEL_ENUM_HSS_ZONE00_TRZ_MULTIPLIER_LSB + nSensorIndex );
    tConfig.atZones[ nSensorIndex ].wTrzDivider = ParameterManager_GetU16Data( PARAMSEL_ENUM_HSS_ZONE00_TRZ_DIVIDER_LSB + nSensorIndex );
    tConfig.atZones[ nSensorIndex ].wTrzTouchZone = ParameterManager_GetU16Data( PARAMSEL_ENUM_HSS_ZONE00_TRZ_TOUCHZONE_LSB + nSensorIndex );
  }
  #endif //ALSENTISHSSTOUCH_CONFIG_METHOD

  // now setup the interval time
  wIntervalTime = tConfig.wProcessIntervalUsecs / ALSENTISHSSTOUCH_EXEC_RATE;
}

/******************************************************************************
 * @function AlsentisHssTouch_GetTouch
 *
 * @brief gets the status of a touch
 *
 * This function will return a status for a touch sensor
 *
 * @return      touch sensor status
 *
 *****************************************************************************/
ALSENTISERR AlsentisHssTouch_GetTouch( U8 nSensor )
{
  ALSENTISERR eError;
  SENSORVAR   xMask;

  // determine if valid sensor number
  if ( nSensor < ALSENTISHSSTOUCH_NUM_SENSORS )
  {
    // generate the mask/set the status
    xMask = BIT( nSensor );
    eError = ( xSensorStates & xMask ) ? ALSENTISERR_TOUCH_PRESS : ALSENTISERR_TOUCH_NOPRESS;
  }
  else
  {
    // report the error
    eError = ALSENTISERR_ILLEGAL_TOUCH;
  }

  // return the status
  return( eError );
}

/******************************************************************************
 * @function AlsentisHssTouch_GetSensorStates
 *
 * @brief gets the status of all sensors
 *
 * This function will return the state of all sensors
 *
 * @return      touch sensor status
 *
 *****************************************************************************/
SENSORVAR AlsentisHssTouch_GetSensorStates( void )
{
  // just return the sensor states
  return( xSensorStates );
}

#if ( ALSENTISHSSTOUCH_OPER_METHOD == ALSENTISHSSTOUCH_OPER_I2CPERIPH )
/******************************************************************************
 * @function HSSTouchHandler_ProcessIncomingMsg
 *
 * @brief process an incoming message
 *
 * This function will process an incoming message and return the number of bytes 
 * if a response is required
 *
 * @param[in]   pnInBuffer    pointer to the incoming message buffer
 * @param[in]   nInSize       number of received bytes
 * @param[in]   pnOutBuffer   pointer to the outgoing message buffer
 * @param[in]   nMaxOutSize   maximum number of bytes for the outgoing message
 *
 * @return      the number of bytes in the outgoing message buffer
 *
 *****************************************************************************/
U8 AlsentisHSSTouch_ProcessIncomingMsg( PU8 pnInBuffer, U8 nInSize, PU8 pnOutBuffer, U8 nMaxOutSize )
{
  U8          nBufIdx, nChecksum, nNumberOfBytesToSend, nBegSensor, nNumSensors;
  PARAMUPDATE tParamUpdate;
  
  // preclear the outgoing checksum/number of bytes to send
  nOutgoingChecksum = 0;
  nNumberOfBytesToSend = 0;

  // clear the parameter enum
  tParamUpdate.eBegParamEnum = PARAMSEL_ILLEGAL;
  
  // check for a valid message by calculating the checksum of the message
  nChecksum = 0;
  for ( nBufIdx = 0; nBufIdx < nInSize - 1; nBufIdx++ )
  {
    // add the character to the current checksum
    nChecksum += *( pnInBuffer + nBufIdx );
  }
  
  // now check for valid checksum
  if ( nChecksum == *( pnInBuffer + nInSize - 1 ))
  {
    // valid checksum, now process the message
    switch( *( pnInBuffer + 0 ))
    {
      case HSS_CMD_WR_ENB_SENSORS :
        // set the enabled sensors
        tConfig.xEnabledSensors = ( SENSORVAR )*( pnInBuffer + 1 );
        tParamUpdate.eBegParamEnum = PARAMSEL_ENUM_HSS_ENABLED_SENSORS_LSB;
        #if ( ALSENTISHSSTOUCH_NUM_SENSORS > 8 )
        tParamUpdate.eEndParamEnum = PARAMSEL_ENUM_HSS_ENABLED_SENSORS_MSB;
        #else
        tParamUpdate.eEndParamEnum = PARAMSEL_ENUM_HSS_ENABLED_SENSORS_LSB;
        #endif // ALSENTISHSSTOUCH_NUM_SENSORS
        break;
      
      case HSS_CMD_WR_LATCHUP_TIMEOUT :
        // set the latch up timer value
        tConfig.nLatchTimeoutSecs = ( U8 )*( pnInBuffer + 1 );
        tParamUpdate.eBegParamEnum = PARAMSEL_ENUM_HSS_LATCH_TIMEOUT;
        tParamUpdate.eEndParamEnum = PARAMSEL_ENUM_HSS_LATCH_TIMEOUT;
        break;
      
      case HSS_CMD_WR_TRZ :
        // get the start sensor and number of sensors
        nBegSensor = *( pnInBuffer + 1 );
        nNumSensors = *( pnInBuffer + 2 );
        
        // copy the data
        memcpy( &tConfig.atZones[ nBegSensor ], ( pnInBuffer + 3 ), ALSENTISZONE_SIZE * nNumSensors );
        tParamUpdate.eBegParamEnum = PARAMSEL_ENUM_HSS_ZONE00_TRZ_MULTIPLIER_LSB + ( ALSENTISZONE_SIZE * nBegSensor );
        tParamUpdate.eEndParamEnum = PARAMSEL_ENUM_HSS_ZONE00_TRZ_TOUCHZONE_MSB + ( ALSENTISZONE_SIZE * nNumSensors );
        break;
      
      case HSS_CMD_WR_DVI :
        // set the DVI state
        tConfig.bDviMode = *( pnInBuffer + 1 );
        tParamUpdate.eBegParamEnum = PARAMSEL_ENUM_HSS_DVIMODE_ENABLED;
        tParamUpdate.eEndParamEnum = PARAMSEL_ENUM_HSS_DVIMODE_ENABLED;
        break;
      
      case HSS_CMD_CLR_ERRORS :
        break;
      
      case HSS_CMD_WR_POWER_MODE :
        // set the power mode
        tConfig.tPowerMode.nByte = *( pnInBuffer + 1 );
        tParamUpdate.eBegParamEnum = PARAMSEL_ENUM_HSS_POWER_MODE_WAKE;
        tParamUpdate.eEndParamEnum = PARAMSEL_ENUM_HSS_POWER_MODE_POWER;
        break;
      
      case HSS_CMD_WR_PROC_INTVL :
        // set the process interval
        tConfig.wProcessIntervalUsecs = ( U16 )*( pnInBuffer + 1 );
        tParamUpdate.eBegParamEnum = PARAMSEL_ENUM_HSS_PROC_INTERVAL_LSB;
        tParamUpdate.eEndParamEnum = PARAMSEL_ENUM_HSS_PROC_INTERVAL_MSB;
        break;
      
      case HSS_CMD_WR_NUM_SAMP_SETS :
        // set the sample sets
        tConfig.wNumberSampleSets = ( U16 )*( pnInBuffer + 1 );
        tParamUpdate.eBegParamEnum = PARAMSEL_ENUM_HSS_NUM_SAMPLES_LSB;
        tParamUpdate.eEndParamEnum = PARAMSEL_ENUM_HSS_NUM_SAMPLES_MSB;
        break;
      
      case HSS_CMD_WR_PULSE_CONFIG :
        // set the pulse ocnfig
        memcpy( &tConfig.tPulseConfig, ( pnInBuffer + 1 ), ALSENTISPULSECONFIG_SIZE );
        tParamUpdate.eBegParamEnum = PARAMSEL_ENUM_HSS_PULSE_PERIOD_LSB;
        tParamUpdate.eEndParamEnum = PARAMSEL_ENUM_HSS_PULSE_DUTYCYCLE;
        break;
      
      case HSS_CMD_WR_SIG_CONFIG1 :
        // copy the buffer to the structure
        memcpy( &tConfig.tHss1Config, ( pnInBuffer + 1 ), ALSENTISHSS1CFG_SIZE );
        tParamUpdate.eBegParamEnum = PARAMSEL_ENUM_HSS_SIGCFG1_APPROACH_CNT_MIN;
        tParamUpdate.eEndParamEnum = PARAMSEL_ENUM_HSS_SIGCFG1_NOTOUCH_DBNC_MIN;
        break;
      
      case HSS_CMD_WR_SIG_CONFIG2 :
        // copy the buffer to the structure
        memcpy( &tConfig.tHss2Config, ( pnInBuffer + 1 ), ALSENTISHSS2CFG_SIZE );
        tParamUpdate.eBegParamEnum = PARAMSEL_ENUM_HSS_SIGCFG2_FAST_TGLDVDT_DEV;
        tParamUpdate.eEndParamEnum = PARAMSEL_ENUM_HSS_SIGCFG2_TOO_SLOW_COUNT_MAX;
        break;
      
      case HSS_CMD_WR_SLEEP_TIME :
        // set the sleep time
        memcpy( &tConfig.tSleep, ( pnInBuffer + 1 ), ALSENTISSLEEP_SIZE );
        tParamUpdate.eBegParamEnum = PARAMSEL_ENUM_HSS_SLEEP_ONE_TIME;
        tParamUpdate.eEndParamEnum = PARAMSEL_ENUM_HSS_SLEEP_REPEAT;
        break;
      
      case HSS_CMD_WR_IRQ_ENABLED :
        tConfig.bIrqEnabled = *( pnInBuffer + 1 );
        tParamUpdate.eBegParamEnum = PARAMSEL_ENUM_HSS_IRQ_ENABLED;
        tParamUpdate.eEndParamEnum = PARAMSEL_ENUM_HSS_IRQ_ENABLED;
        break;

      case HSS_CMD_WR_DIFF_CONFIG :
        // set the enabled sensors
        tConfig.xDiffSensors = ( SENSORVAR )*( pnInBuffer + 1 );
        tParamUpdate.eBegParamEnum = PARAMSEL_ENUM_HSS_DIFF_SENSORS_LSB;
        #if ( ALSENTISHSSTOUCH_NUM_SENSORS > 8 )
        tParamUpdate.eEndParamEnum = PARAMSEL_ENUM_HSS_DIFF_SENSORS_MSB;
        tConfig.nDiffSensorNumber = *( pnInBuffer + 3 );
        #else
        tParamUpdate.eEndParamEnum = PARAMSEL_ENUM_HSS_DIFF_SENSORS_LSB;
        tConfig.nDiffSensorNumber = *( pnInBuffer + 2 );
        #endif // ALSENTISHSSTOUCH_NUM_SENSORS
        break;
      
      case HSS_CMD_RESET :
        break;
      
      case HSS_CMD_RD_ID :
        // set the values in the buffer
        nNumberOfBytesToSend = AddToBuffer( nStatus, pnOutBuffer, nNumberOfBytesToSend, nMaxOutSize );
        nNumberOfBytesToSend = AddToBuffer( ALSENTISID_SIZE + 2, pnOutBuffer, nNumberOfBytesToSend, nMaxOutSize );
        
        // copy the buffer to the structure
        memcpy(( pnOutBuffer + nNumberOfBytesToSend ), &tConfig.tId, ALSENTISID_SIZE );
        nNumberOfBytesToSend += ALSENTISID_SIZE;

        // calculate the checksum
        nNumberOfBytesToSend = CalculateAndStuffChecksum( pnOutBuffer, nNumberOfBytesToSend );
        break;
      
      case HSS_CMD_RD_ENABLED_SENSORS :
        // set the values in the buffer
        nNumberOfBytesToSend = AddToBuffer( nStatus, pnOutBuffer, nNumberOfBytesToSend, nMaxOutSize );
        nNumberOfBytesToSend = AddToBuffer( 4, pnOutBuffer, nNumberOfBytesToSend, nMaxOutSize );

        // copy the enabled sensors to the structure
        memcpy(( pnOutBuffer + nNumberOfBytesToSend ), &tConfig.xEnabledSensors, sizeof( SENSORVAR ));
        nNumberOfBytesToSend += sizeof( SENSORVAR );

        // calculate the checksum
        nNumberOfBytesToSend = CalculateAndStuffChecksum( pnOutBuffer, nNumberOfBytesToSend );
        break;
      
      case HSS_CMD_RD_LATCHUP_TIMEOUT :
        // set the values in the buffer
        nNumberOfBytesToSend = AddToBuffer( nStatus, pnOutBuffer, nNumberOfBytesToSend, nMaxOutSize );
        nNumberOfBytesToSend = AddToBuffer( 4, pnOutBuffer, nNumberOfBytesToSend, nMaxOutSize );
        nNumberOfBytesToSend = AddToBuffer( tConfig.nLatchTimeoutSecs, pnOutBuffer, nNumberOfBytesToSend, nMaxOutSize );
        nNumberOfBytesToSend = AddToBuffer( nOutgoingChecksum, pnOutBuffer, nNumberOfBytesToSend, nMaxOutSize );
        break;
      
      case HSS_CMD_RD_TRZ :
        // get the start sensor and number of sensors
        nBegSensor = *( pnInBuffer + 1 );
        nNumSensors = *( pnInBuffer + 2 );
        
        // now start to fill the buffer
        nNumberOfBytesToSend = AddToBuffer( nStatus, pnOutBuffer, nNumberOfBytesToSend, nMaxOutSize );
        nNumberOfBytesToSend = AddToBuffer( ALSENTISZONE_SIZE * nNumSensors + 2, pnOutBuffer, nNumberOfBytesToSend, nMaxOutSize );
        
        // add the zones
        memcpy(( pnOutBuffer + nNumberOfBytesToSend ), &tConfig.atZones[ nBegSensor ], ALSENTISZONE_SIZE * nNumSensors );
        nNumberOfBytesToSend += ( ALSENTISZONE_SIZE * nNumSensors );
        
        // calculate the checksum
        nNumberOfBytesToSend = CalculateAndStuffChecksum( pnOutBuffer, nNumberOfBytesToSend );
        break;
      
      case HSS_CMD_RD_STATUS :
        break;
      
      case HSS_CMD_RD_SENSOR_STATES :
        // now start to fill the buffer
        nNumberOfBytesToSend = AddToBuffer( nStatus, pnOutBuffer, nNumberOfBytesToSend, nMaxOutSize );
        nNumberOfBytesToSend = AddToBuffer( 4, pnOutBuffer, nNumberOfBytesToSend, nMaxOutSize );

        // copy the enabled sensors to the structure
        memcpy(( pnOutBuffer + nNumberOfBytesToSend ), &xSensorStates, sizeof( SENSORVAR ));

        // calculate the checksum
        nNumberOfBytesToSend = CalculateAndStuffChecksum( pnOutBuffer, nNumberOfBytesToSend );
        break;
      
      case HSS_CMD_RD_POWER_MODE :
        // now start to fill the buffer
        nNumberOfBytesToSend = AddToBuffer( nStatus, pnOutBuffer, nNumberOfBytesToSend, nMaxOutSize );
        nNumberOfBytesToSend = AddToBuffer( 4, pnOutBuffer, nNumberOfBytesToSend, nMaxOutSize );
        nNumberOfBytesToSend = AddToBuffer( tConfig.tPowerMode.nByte, pnOutBuffer, nNumberOfBytesToSend, nMaxOutSize );
        nNumberOfBytesToSend = AddToBuffer( nOutgoingChecksum, pnOutBuffer, nNumberOfBytesToSend, nMaxOutSize );
        break;
      
      case HSS_CMD_RD_PROC_INTVL :
        // add the header
        nNumberOfBytesToSend = AddToBuffer( nStatus, pnOutBuffer, nNumberOfBytesToSend, nMaxOutSize );
        nNumberOfBytesToSend = AddToBuffer( 4, pnOutBuffer, nNumberOfBytesToSend, nMaxOutSize );
        
        // copy the process interval
        memcpy(( pnOutBuffer + nNumberOfBytesToSend ), &tConfig.wProcessIntervalUsecs, sizeof( U16 ));
        nNumberOfBytesToSend += sizeof( U16 );
        
        // calculate the checksum
        nNumberOfBytesToSend = CalculateAndStuffChecksum( pnOutBuffer, nNumberOfBytesToSend );
        break;
      
      case HSS_CMD_RD_NUM_SAMP_SETS :
        // add the header
        nNumberOfBytesToSend = AddToBuffer( nStatus, pnOutBuffer, nNumberOfBytesToSend, nMaxOutSize );
        nNumberOfBytesToSend = AddToBuffer( 4, pnOutBuffer, nNumberOfBytesToSend, nMaxOutSize );
        
        // copy the process interval
        memcpy(( pnOutBuffer + nNumberOfBytesToSend ), &tConfig.wNumberSampleSets, sizeof( U16 ));
        nNumberOfBytesToSend += sizeof( U16 );
        
        // calculate the checksum
        nNumberOfBytesToSend = CalculateAndStuffChecksum( pnOutBuffer, nNumberOfBytesToSend );
        break;
      
      case HSS_CMD_RD_PULSE_CONFIG :
        // add the header
        nNumberOfBytesToSend = AddToBuffer( nStatus, pnOutBuffer, nNumberOfBytesToSend, nMaxOutSize );
        nNumberOfBytesToSend = AddToBuffer( ALSENTISPULSECONFIG_SIZE + 2, pnOutBuffer, nNumberOfBytesToSend, nMaxOutSize );
        
        // copy the process interval
        memcpy(( pnOutBuffer + nNumberOfBytesToSend ), &tConfig.tPulseConfig, ALSENTISPULSECONFIG_SIZE );
        nNumberOfBytesToSend += ALSENTISPULSECONFIG_SIZE;
        
        // calculate the checksum
        nNumberOfBytesToSend = CalculateAndStuffChecksum( pnOutBuffer, nNumberOfBytesToSend );
        break;
      
      case HSS_CMD_RD_SIG_CONFIG1 :
        // add the status and byte count
        nNumberOfBytesToSend = AddToBuffer( nStatus, pnOutBuffer, nNumberOfBytesToSend, nMaxOutSize );
        nNumberOfBytesToSend = AddToBuffer( ALSENTISHSS1CFG_SIZE + 3, pnOutBuffer, nNumberOfBytesToSend, nMaxOutSize );
        
        // copy the buffer to the structure
        memcpy(( pnOutBuffer + nNumberOfBytesToSend ), &tConfig.tHss1Config, ALSENTISHSS1CFG_SIZE );
        nNumberOfBytesToSend += ALSENTISHSS1CFG_SIZE;

        // calculate the checksum
        nNumberOfBytesToSend = CalculateAndStuffChecksum( pnOutBuffer, nNumberOfBytesToSend );
        break;
      
      case HSS_CMD_RD_SIG_CONFIG2 :
        // add the status and byte count
        nNumberOfBytesToSend = AddToBuffer( nStatus, pnOutBuffer, nNumberOfBytesToSend, nMaxOutSize );
        nNumberOfBytesToSend = AddToBuffer( ALSENTISHSS2CFG_SIZE + 3, pnOutBuffer, nNumberOfBytesToSend, nMaxOutSize );
        
        // copy the buffer to the structure
        memcpy(( pnOutBuffer + nNumberOfBytesToSend ), &tConfig.tHss2Config, ALSENTISHSS2CFG_SIZE );
        nNumberOfBytesToSend += ALSENTISHSS2CFG_SIZE;

        // calculate the checksum
        nNumberOfBytesToSend = CalculateAndStuffChecksum( pnOutBuffer, nNumberOfBytesToSend );
        break;
      
      case HSS_CMD_RD_DVI :
        break;
      
      case HSS_CMD_RD_SLEEP_TIME :
        // add the header
        nNumberOfBytesToSend = AddToBuffer( nStatus, pnOutBuffer, nNumberOfBytesToSend, nMaxOutSize );
        nNumberOfBytesToSend = AddToBuffer( ALSENTISSLEEP_SIZE + 2, pnOutBuffer, nNumberOfBytesToSend, nMaxOutSize );
        
        // copy the process interval
        memcpy(( pnOutBuffer + nNumberOfBytesToSend ), &tConfig.tSleep, ALSENTISSLEEP_SIZE );
        nNumberOfBytesToSend += ALSENTISSLEEP_SIZE;

        // calculate the checksum
        nNumberOfBytesToSend = CalculateAndStuffChecksum( pnOutBuffer, nNumberOfBytesToSend );
        break;
      
      case HSS_CMD_RD_IRQ_ENABLED :
        // now start to fill the buffer
        nNumberOfBytesToSend = AddToBuffer( nStatus, pnOutBuffer, nNumberOfBytesToSend, nMaxOutSize );
        nNumberOfBytesToSend = AddToBuffer( 4, pnOutBuffer, nNumberOfBytesToSend, nMaxOutSize );
        nNumberOfBytesToSend = AddToBuffer( tConfig.bIrqEnabled, pnOutBuffer, nNumberOfBytesToSend, nMaxOutSize );
        nNumberOfBytesToSend = AddToBuffer( nOutgoingChecksum, pnOutBuffer, nNumberOfBytesToSend, nMaxOutSize );
        break;

      case HSS_CMD_RD_DIFF_CONFIG :
        // set the values in the buffer
        nNumberOfBytesToSend = AddToBuffer( nStatus, pnOutBuffer, nNumberOfBytesToSend, nMaxOutSize );
        nNumberOfBytesToSend = AddToBuffer( 3 + sizeof( SENSORVAR ), pnOutBuffer, nNumberOfBytesToSend, nMaxOutSize );

        // copy the enabled sensors to the structure
        memcpy(( pnOutBuffer + nNumberOfBytesToSend ), &tConfig.xDiffSensors, sizeof( SENSORVAR ));
        nNumberOfBytesToSend += sizeof( SENSORVAR );
        
        // now add the sensor number
        nNumberOfBytesToSend = AddToBuffer( tConfig.nDiffSensorNumber, pnOutBuffer, nNumberOfBytesToSend, nMaxOutSize );

        // calculate the checksum
        nNumberOfBytesToSend = CalculateAndStuffChecksum( pnOutBuffer, nNumberOfBytesToSend );
        break;
      
      default :
        *( pnOutBuffer + 0 ) = 0xFF;
        break;
    }

    // if parameter select is valid
    if ( tParamUpdate.eBegParamEnum != PARAMSEL_ILLEGAL )
    {
      // post the event
      QueueManager_PutTail( ALSENTISHSSTOUCH_UPDATE_QUEUE_ENUM, ( PU8 )&tParamUpdate );
    }
  }
  
  // return the number of bytes to send
  return( nNumberOfBytesToSend );
}
#endif // ALSENTISHSSTOUCH_OPER_METHOD

#if ( ALSENTISHSSTOUCH_CONFIG_METHOD == ALSENTISHSSTOUCH_PARAMMGR_SEL )
/******************************************************************************
 * @function AlsentisHssTouch_GetConfigValue
 *
 * @brief get a configuration value
 *
 * This function will return the confgiuration value
 *
 * @param[in]   eParameter    parameter number
 * 
 * @return      the current value
 *
 *****************************************************************************/
PARAMARG AlsentisHssTouch_GetConfigValue( PARAMSELENUM eParameter )
{
  PARAMARG  xValue = 0;
  U8        nSensorNum;

  switch( eParameter )
  {
    case PARAMSEL_ENUM_HSS_IRQ_ENABLED :
      xValue = tConfig.bIrqEnabled;
      break;

    case PARAMSEL_ENUM_HSS_DVIMODE_ENABLED :
      xValue = tConfig.bDviMode;
      break;

    case PARAMSEL_ENUM_HSS_LATCH_TIMEOUT :
      xValue = tConfig.nLatchTimeoutSecs;
      break;

    case PARAMSEL_ENUM_HSS_PROC_INTERVAL_LSB :
      xValue = LO16( tConfig.wProcessIntervalUsecs );
      break;

    case PARAMSEL_ENUM_HSS_PROC_INTERVAL_MSB :
      xValue = HI16( tConfig.wProcessIntervalUsecs );
      break;

    case PARAMSEL_ENUM_HSS_NUM_SAMPLES_LSB :
      xValue = LO16( tConfig.wNumberSampleSets );
      break;

    case PARAMSEL_ENUM_HSS_NUM_SAMPLES_MSB :
      xValue = HI16( tConfig.wNumberSampleSets );
      break;

    case PARAMSEL_ENUM_HSS_NUM_PULSES :
      xValue = tConfig.tPulseConfig.nNumberOfPulses;
      break;

    case PARAMSEL_ENUM_HSS_PULSE_PERIOD_LSB :
      xValue = LO16( tConfig.tPulseConfig.tPulse.tFields.wPulsePeriod );
      break;

    case PARAMSEL_ENUM_HSS_PULSE_PERIOD_MSB :
      xValue = HI16( tConfig.tPulseConfig.tPulse.tFields.wPulsePeriod );
      break;

    case PARAMSEL_ENUM_HSS_PULSE_DUTYCYCLE :
      xValue = tConfig.tPulseConfig.tPulse.tFields.wDutyCycle;
      break;

    case PARAMSEL_ENUM_HSS_POWER_MODE_WAKE :
      xValue = tConfig.tPowerMode.tFields.nPower;
      break;

    case PARAMSEL_ENUM_HSS_POWER_MODE_POWER :
      xValue = tConfig.tPowerMode.tFields.nWake;
      break;

    case PARAMSEL_ENUM_HSS_SIGCFG1_APPROACH_CNT_MIN :
      xValue = tConfig.tHss1Config.nApproachContMin;
      break;

    case PARAMSEL_ENUM_HSS_SIGCFG1_SLOW_LVLOFF_CNT_MIN :
      xValue = tConfig.tHss1Config.nSlowLvlOffCntMin;
      break;

    case PARAMSEL_ENUM_HSS_SIGCFG1_FAST_LVLOFF_CNT_MIN :
      xValue = tConfig.tHss1Config.nFastLvlOffCntMin;
      break;

    case PARAMSEL_ENUM_HSS_SIGCFG1_NOTOUCH_DBNC_MIN :
      xValue = tConfig.tHss1Config.nNoTouchDebounceMin;
      break;

    case PARAMSEL_ENUM_HSS_SIGCFG2_FAST_TGLDVDT_DEV :
      xValue = tConfig.tHss2Config.nFastTglDvDtDeviation;
      break;

    case PARAMSEL_ENUM_HSS_SIGCFG2_ABORT_TOUCH_PCT :
      xValue = tConfig.tHss2Config.nAbortTouchPercent;
      break;

    case PARAMSEL_ENUM_HSS_SIGCFG2_ABORT_TOUCH_COUNT_MAX :
      xValue = tConfig.tHss2Config.nAbortTouchCountMax;
      break;

    case PARAMSEL_ENUM_HSS_SIGCFG2_TOO_SLOW_COUNT_MAX :
      xValue = tConfig.tHss2Config.nTooSlowCountMax;
      break;

    case PARAMSEL_ENUM_HSS_SLEEP_ONE_TIME :
      xValue = tConfig.tSleep.nOneTime;
      break;

    case PARAMSEL_ENUM_HSS_SLEEP_REPEAT :
      xValue = tConfig.tSleep.nRepeat;
      break;

    case PARAMSEL_ENUM_HSS_TYPE_ID_LSB :
      xValue = LO16( tConfig.tId.wType );
      break;

    case PARAMSEL_ENUM_HSS_TYPE_ID_MSB :
      xValue = HI16( tConfig.tId.wType );
      break;

    case PARAMSEL_ENUM_HSS_TYPE_VERSION_LSB :
      xValue = LO16( tConfig.tId.wVersion );
      break;

    case PARAMSEL_ENUM_HSS_TYPE_VERSION_MSB :
      xValue = HI16( tConfig.tId.wVersion );
      break;

    case PARAMSEL_ENUM_HSS_ENABLED_SENSORS_LSB :
      xValue = LO16( tConfig.xEnabledSensors );
      break;

    case PARAMSEL_ENUM_HSS_DIFF_SENSORS_LSB :
      xValue = LO16( tConfig.xDiffSensors );
      break;

  #if ( ALSENTISHSSTOUCH_NUM_SENSORS > 8 )
    case PARAMSEL_ENUM_HSS_ENABLED_SENSORS_MSB :
      xValue = HI16( tConfig.xEnabledSensors );
      break;

    case PARAMSEL_ENUM_HSS_DIF_SENSORS_MSB :
      xValue = HI16( tConfig.xDiffSensors );
      break;
  #endif // ALSENTISHSSTOUCH_NUM_SENSORS

    #if ( ALSENTISHSSTOUCH_NUM_SENSORS >= 1 )
    case PARAMSEL_ENUM_HSS_ZONE00_TRZ_MULTIPLIER_LSB :
    #endif // ALSENTISHSSTOUCH_NUM_SENSORS
    #if ( ALSENTISHSSTOUCH_NUM_SENSORS >= 2 )
    case PARAMSEL_ENUM_HSS_ZONE01_TRZ_MULTIPLIER_LSB :
    #endif // ALSENTISHSSTOUCH_NUM_SENSORS
    #if ( ALSENTISHSSTOUCH_NUM_SENSORS >= 3 )
    case PARAMSEL_ENUM_HSS_ZONE02_TRZ_MULTIPLIER_LSB :
    #endif // ALSENTISHSSTOUCH_NUM_SENSORS
    #if ( ALSENTISHSSTOUCH_NUM_SENSORS >= 4 )
    case PARAMSEL_ENUM_HSS_ZONE03_TRZ_MULTIPLIER_LSB :
    #endif // ALSENTISHSSTOUCH_NUM_SENSORS
    #if ( ALSENTISHSSTOUCH_NUM_SENSORS >= 5 )
    case PARAMSEL_ENUM_HSS_ZONE04_TRZ_MULTIPLIER_LSB :
    #endif // ALSENTISHSSTOUCH_NUM_SENSORS
    #if ( ALSENTISHSSTOUCH_NUM_SENSORS >= 6 )
    case PARAMSEL_ENUM_HSS_ZONE05_TRZ_MULTIPLIER_LSB :
    #endif // ALSENTISHSSTOUCH_NUM_SENSORS
    #if ( ALSENTISHSSTOUCH_NUM_SENSORS >= 7 )
    case PARAMSEL_ENUM_HSS_ZONE06_TRZ_MULTIPLIER_LSB :
    #endif // ALSENTISHSSTOUCH_NUM_SENSORS
    #if ( ALSENTISHSSTOUCH_NUM_SENSORS >= 8 )
    case PARAMSEL_ENUM_HSS_ZONE07_TRZ_MULTIPLIER_LSB :
    #endif // ALSENTISHSSTOUCH_NUM_SENSORS
    #if ( ALSENTISHSSTOUCH_NUM_SENSORS >= 9 )
    case PARAMSEL_ENUM_HSS_ZONE08_TRZ_MULTIPLIER_LSB :
    case PARAMSEL_ENUM_HSS_ZONE09_TRZ_MULTIPLIER_LSB :
    case PARAMSEL_ENUM_HSS_ZONE10_TRZ_MULTIPLIER_LSB :
    case PARAMSEL_ENUM_HSS_ZONE11_TRZ_MULTIPLIER_LSB :
    case PARAMSEL_ENUM_HSS_ZONE12_TRZ_MULTIPLIER_LSB :
    case PARAMSEL_ENUM_HSS_ZONE13_TRZ_MULTIPLIER_LSB :
    case PARAMSEL_ENUM_HSS_ZONE14_TRZ_MULTIPLIER_LSB :
    case PARAMSEL_ENUM_HSS_ZONE15_TRZ_MULTIPLIER_LSB :
    #endif // ALSENTISHSSTOUCH_NUM_SENSORS
      // compute the sensor number
      nSensorNum = eParameter - PARAMSEL_ENUM_HSS_ZONE00_TRZ_MULTIPLIER_LSB;
      xValue = LO16( tConfig.atZones[ nSensorNum ].wTrzMultiplier );
      break;

    #if ( ALSENTISHSSTOUCH_NUM_SENSORS >= 1 )
    case PARAMSEL_ENUM_HSS_ZONE00_TRZ_MULTIPLIER_MSB :
    #endif // ALSENTISHSSTOUCH_NUM_SENSORS
    #if ( ALSENTISHSSTOUCH_NUM_SENSORS >= 2 )
    case PARAMSEL_ENUM_HSS_ZONE01_TRZ_MULTIPLIER_MSB :
    #endif // ALSENTISHSSTOUCH_NUM_SENSORS
    #if ( ALSENTISHSSTOUCH_NUM_SENSORS >= 3 )
    case PARAMSEL_ENUM_HSS_ZONE02_TRZ_MULTIPLIER_MSB :
    #endif // ALSENTISHSSTOUCH_NUM_SENSORS
    #if ( ALSENTISHSSTOUCH_NUM_SENSORS >= 4 )
    case PARAMSEL_ENUM_HSS_ZONE03_TRZ_MULTIPLIER_MSB :
    #endif // ALSENTISHSSTOUCH_NUM_SENSORS
    #if ( ALSENTISHSSTOUCH_NUM_SENSORS >= 5 )
    case PARAMSEL_ENUM_HSS_ZONE04_TRZ_MULTIPLIER_MSB :
    #endif // ALSENTISHSSTOUCH_NUM_SENSORS
    #if ( ALSENTISHSSTOUCH_NUM_SENSORS >= 6 )
    case PARAMSEL_ENUM_HSS_ZONE05_TRZ_MULTIPLIER_MSB :
    #endif // ALSENTISHSSTOUCH_NUM_SENSORS
    #if ( ALSENTISHSSTOUCH_NUM_SENSORS >= 7 )
    case PARAMSEL_ENUM_HSS_ZONE06_TRZ_MULTIPLIER_MSB :
    #endif // ALSENTISHSSTOUCH_NUM_SENSORS
    #if ( ALSENTISHSSTOUCH_NUM_SENSORS >= 8 )
    case PARAMSEL_ENUM_HSS_ZONE07_TRZ_MULTIPLIER_MSB :
    #endif // ALSENTISHSSTOUCH_NUM_SENSORS
    #if ( ALSENTISHSSTOUCH_NUM_SENSORS > 8 )
    case PARAMSEL_ENUM_HSS_ZONE08_TRZ_MULTIPLIER_MSB :
    case PARAMSEL_ENUM_HSS_ZONE09_TRZ_MULTIPLIER_MSB :
    case PARAMSEL_ENUM_HSS_ZONE10_TRZ_MULTIPLIER_MSB :
    case PARAMSEL_ENUM_HSS_ZONE11_TRZ_MULTIPLIER_MSB :
    case PARAMSEL_ENUM_HSS_ZONE12_TRZ_MULTIPLIER_MSB :
    case PARAMSEL_ENUM_HSS_ZONE13_TRZ_MULTIPLIER_MSB :
    case PARAMSEL_ENUM_HSS_ZONE14_TRZ_MULTIPLIER_MSB :
    case PARAMSEL_ENUM_HSS_ZONE15_TRZ_MULTIPLIER_MSB :
    #endif // ALSENTISHSSTOUCH_NUM_SENSORS
      // compute the sensor number
      nSensorNum = eParameter - PARAMSEL_ENUM_HSS_ZONE00_TRZ_MULTIPLIER_MSB;
      xValue = HI16( tConfig.atZones[ nSensorNum ].wTrzMultiplier );
      break;

    #if ( ALSENTISHSSTOUCH_NUM_SENSORS >= 1 )
    case PARAMSEL_ENUM_HSS_ZONE00_TRZ_DIVIDER_LSB :
    #endif // ALSENTISHSSTOUCH_NUM_SENSORS
    #if ( ALSENTISHSSTOUCH_NUM_SENSORS >= 2 )
    case PARAMSEL_ENUM_HSS_ZONE01_TRZ_DIVIDER_LSB :
    #endif // ALSENTISHSSTOUCH_NUM_SENSORS
    #if ( ALSENTISHSSTOUCH_NUM_SENSORS >= 3 )
    case PARAMSEL_ENUM_HSS_ZONE02_TRZ_DIVIDER_LSB :
    #endif // ALSENTISHSSTOUCH_NUM_SENSORS
    #if ( ALSENTISHSSTOUCH_NUM_SENSORS >= 4 )
    case PARAMSEL_ENUM_HSS_ZONE03_TRZ_DIVIDER_LSB :
    #endif // ALSENTISHSSTOUCH_NUM_SENSORS
    #if ( ALSENTISHSSTOUCH_NUM_SENSORS >= 5 )
    case PARAMSEL_ENUM_HSS_ZONE04_TRZ_DIVIDER_LSB :
    #endif // ALSENTISHSSTOUCH_NUM_SENSORS
    #if ( ALSENTISHSSTOUCH_NUM_SENSORS >= 6 )
    case PARAMSEL_ENUM_HSS_ZONE05_TRZ_DIVIDER_LSB :
    #endif // ALSENTISHSSTOUCH_NUM_SENSORS
    #if ( ALSENTISHSSTOUCH_NUM_SENSORS >= 7 )
    case PARAMSEL_ENUM_HSS_ZONE06_TRZ_DIVIDER_LSB :
    #endif // ALSENTISHSSTOUCH_NUM_SENSORS
    #if ( ALSENTISHSSTOUCH_NUM_SENSORS >= 8 )
    case PARAMSEL_ENUM_HSS_ZONE07_TRZ_DIVIDER_LSB :
    #endif // ALSENTISHSSTOUCH_NUM_SENSORS
    #if ( ALSENTISHSSTOUCH_NUM_SENSORS > 8 )
    case PARAMSEL_ENUM_HSS_ZONE08_TRZ_DIVIDER_LSB :
    case PARAMSEL_ENUM_HSS_ZONE09_TRZ_DIVIDER_LSB :
    case PARAMSEL_ENUM_HSS_ZONE10_TRZ_DIVIDER_LSB :
    case PARAMSEL_ENUM_HSS_ZONE11_TRZ_DIVIDER_LSB :
    case PARAMSEL_ENUM_HSS_ZONE12_TRZ_DIVIDER_LSB :
    case PARAMSEL_ENUM_HSS_ZONE13_TRZ_DIVIDER_LSB :
    case PARAMSEL_ENUM_HSS_ZONE14_TRZ_DIVIDER_LSB :
    case PARAMSEL_ENUM_HSS_ZONE15_TRZ_DIVIDER_LSB :
    #endif // ALSENTISHSSTOUCH_NUM_SENSORS
      // compute the sensor number
      nSensorNum = eParameter - PARAMSEL_ENUM_HSS_ZONE00_TRZ_DIVIDER_LSB;
      xValue = LO16( tConfig.atZones[ nSensorNum ].wTrzDivider );
      break;

    #if ( ALSENTISHSSTOUCH_NUM_SENSORS >= 1 )
    case PARAMSEL_ENUM_HSS_ZONE00_TRZ_DIVIDER_MSB :
    #endif // ALSENTISHSSTOUCH_NUM_SENSORS
    #if ( ALSENTISHSSTOUCH_NUM_SENSORS >= 2 )
    case PARAMSEL_ENUM_HSS_ZONE01_TRZ_DIVIDER_MSB :
    #endif // ALSENTISHSSTOUCH_NUM_SENSORS
    #if ( ALSENTISHSSTOUCH_NUM_SENSORS >= 3 )
    case PARAMSEL_ENUM_HSS_ZONE02_TRZ_DIVIDER_MSB :
    #endif // ALSENTISHSSTOUCH_NUM_SENSORS
    #if ( ALSENTISHSSTOUCH_NUM_SENSORS >= 4 )
    case PARAMSEL_ENUM_HSS_ZONE03_TRZ_DIVIDER_MSB :
    #endif // ALSENTISHSSTOUCH_NUM_SENSORS
    #if ( ALSENTISHSSTOUCH_NUM_SENSORS >= 5 )
    case PARAMSEL_ENUM_HSS_ZONE04_TRZ_DIVIDER_MSB :
    #endif // ALSENTISHSSTOUCH_NUM_SENSORS
    #if ( ALSENTISHSSTOUCH_NUM_SENSORS >= 6 )
    case PARAMSEL_ENUM_HSS_ZONE05_TRZ_DIVIDER_MSB :
    #endif // ALSENTISHSSTOUCH_NUM_SENSORS
    #if ( ALSENTISHSSTOUCH_NUM_SENSORS >= 7 )
    case PARAMSEL_ENUM_HSS_ZONE06_TRZ_DIVIDER_MSB :
    #endif // ALSENTISHSSTOUCH_NUM_SENSORS
    #if ( ALSENTISHSSTOUCH_NUM_SENSORS >= 8 )
    case PARAMSEL_ENUM_HSS_ZONE07_TRZ_DIVIDER_MSB :
    #endif // ALSENTISHSSTOUCH_NUM_SENSORS
    #if ( ALSENTISHSSTOUCH_NUM_SENSORS > 8 )
    case PARAMSEL_ENUM_HSS_ZONE08_TRZ_DIVIDER_MSB :
    case PARAMSEL_ENUM_HSS_ZONE09_TRZ_DIVIDER_MSB :
    case PARAMSEL_ENUM_HSS_ZONE10_TRZ_DIVIDER_MSB :
    case PARAMSEL_ENUM_HSS_ZONE11_TRZ_DIVIDER_MSB :
    case PARAMSEL_ENUM_HSS_ZONE12_TRZ_DIVIDER_MSB :
    case PARAMSEL_ENUM_HSS_ZONE13_TRZ_DIVIDER_MSB :
    case PARAMSEL_ENUM_HSS_ZONE14_TRZ_DIVIDER_MSB :
    case PARAMSEL_ENUM_HSS_ZONE15_TRZ_DIVIDER_MSB :
    #endif // ALSENTISHSSTOUCH_NUM_SENSORS
      // compute the sensor number
      nSensorNum = eParameter - PARAMSEL_ENUM_HSS_ZONE00_TRZ_DIVIDER_MSB;
      xValue = HI16( tConfig.atZones[ nSensorNum ].wTrzDivider );
      break;

    #if ( ALSENTISHSSTOUCH_NUM_SENSORS >= 1 )
    case PARAMSEL_ENUM_HSS_ZONE00_TRZ_TOUCHZONE_LSB :
    #endif // ALSENTISHSSTOUCH_NUM_SENSORS
    #if ( ALSENTISHSSTOUCH_NUM_SENSORS >= 2 )
    case PARAMSEL_ENUM_HSS_ZONE01_TRZ_TOUCHZONE_LSB :
    #endif // ALSENTISHSSTOUCH_NUM_SENSORS
    #if ( ALSENTISHSSTOUCH_NUM_SENSORS >= 3 )
    case PARAMSEL_ENUM_HSS_ZONE02_TRZ_TOUCHZONE_LSB :
    #endif // ALSENTISHSSTOUCH_NUM_SENSORS
    #if ( ALSENTISHSSTOUCH_NUM_SENSORS >= 4 )
    case PARAMSEL_ENUM_HSS_ZONE03_TRZ_TOUCHZONE_LSB :
    #endif // ALSENTISHSSTOUCH_NUM_SENSORS
    #if ( ALSENTISHSSTOUCH_NUM_SENSORS >= 5 )
    case PARAMSEL_ENUM_HSS_ZONE04_TRZ_TOUCHZONE_LSB :
    #endif // ALSENTISHSSTOUCH_NUM_SENSORS
    #if ( ALSENTISHSSTOUCH_NUM_SENSORS >= 6 )
    case PARAMSEL_ENUM_HSS_ZONE05_TRZ_TOUCHZONE_LSB :
    #endif // ALSENTISHSSTOUCH_NUM_SENSORS
    #if ( ALSENTISHSSTOUCH_NUM_SENSORS >= 7 )
    case PARAMSEL_ENUM_HSS_ZONE06_TRZ_TOUCHZONE_LSB :
    #endif // ALSENTISHSSTOUCH_NUM_SENSORS
    #if ( ALSENTISHSSTOUCH_NUM_SENSORS >= 8 )
    case PARAMSEL_ENUM_HSS_ZONE07_TRZ_TOUCHZONE_LSB :
    #endif // ALSENTISHSSTOUCH_NUM_SENSORS
    #if ( ALSENTISHSSTOUCH_NUM_SENSORS > 8 )
    case PARAMSEL_ENUM_HSS_ZONE08_TRZ_TOUCHZONE_LSB :
    case PARAMSEL_ENUM_HSS_ZONE09_TRZ_TOUCHZONE_LSB :
    case PARAMSEL_ENUM_HSS_ZONE10_TRZ_TOUCHZONE_LSB :
    case PARAMSEL_ENUM_HSS_ZONE11_TRZ_TOUCHZONE_LSB :
    case PARAMSEL_ENUM_HSS_ZONE12_TRZ_TOUCHZONE_LSB :
    case PARAMSEL_ENUM_HSS_ZONE13_TRZ_TOUCHZONE_LSB :
    case PARAMSEL_ENUM_HSS_ZONE14_TRZ_TOUCHZONE_LSB :
    case PARAMSEL_ENUM_HSS_ZONE15_TRZ_TOUCHZONE_LSB :
    #endif // ALSENTISHSSTOUCH_NUM_SENSORS
      // compute the sensor number
      nSensorNum = eParameter - PARAMSEL_ENUM_HSS_ZONE00_TRZ_TOUCHZONE_LSB;
      xValue = LO16( tConfig.atZones[ nSensorNum ].wTrzTouchZone );
      break;

    #if ( ALSENTISHSSTOUCH_NUM_SENSORS >= 1 )
    case PARAMSEL_ENUM_HSS_ZONE00_TRZ_TOUCHZONE_MSB :
    #endif // ALSENTISHSSTOUCH_NUM_SENSORS
    #if ( ALSENTISHSSTOUCH_NUM_SENSORS >= 2 )
    case PARAMSEL_ENUM_HSS_ZONE01_TRZ_TOUCHZONE_MSB :
    #endif // ALSENTISHSSTOUCH_NUM_SENSORS
    #if ( ALSENTISHSSTOUCH_NUM_SENSORS >= 3 )
    case PARAMSEL_ENUM_HSS_ZONE02_TRZ_TOUCHZONE_MSB :
    #endif // ALSENTISHSSTOUCH_NUM_SENSORS
    #if ( ALSENTISHSSTOUCH_NUM_SENSORS >= 4 )
    case PARAMSEL_ENUM_HSS_ZONE03_TRZ_TOUCHZONE_MSB :
    #endif // ALSENTISHSSTOUCH_NUM_SENSORS
    #if ( ALSENTISHSSTOUCH_NUM_SENSORS >= 5 )
    case PARAMSEL_ENUM_HSS_ZONE04_TRZ_TOUCHZONE_MSB :
    #endif // ALSENTISHSSTOUCH_NUM_SENSORS
    #if ( ALSENTISHSSTOUCH_NUM_SENSORS >= 6 )
    case PARAMSEL_ENUM_HSS_ZONE05_TRZ_TOUCHZONE_MSB :
    #endif // ALSENTISHSSTOUCH_NUM_SENSORS
    #if ( ALSENTISHSSTOUCH_NUM_SENSORS >= 7 )
    case PARAMSEL_ENUM_HSS_ZONE06_TRZ_TOUCHZONE_MSB :
    #endif // ALSENTISHSSTOUCH_NUM_SENSORS
    #if ( ALSENTISHSSTOUCH_NUM_SENSORS >= 8 )
    case PARAMSEL_ENUM_HSS_ZONE07_TRZ_TOUCHZONE_MSB :
    #endif // ALSENTISHSSTOUCH_NUM_SENSORS
    #if ( ALSENTISHSSTOUCH_NUM_SENSORS > 8 )
    case PARAMSEL_ENUM_HSS_ZONE08_TRZ_TOUCHZONE_MSB :
    case PARAMSEL_ENUM_HSS_ZONE09_TRZ_TOUCHZONE_MSB :
    case PARAMSEL_ENUM_HSS_ZONE10_TRZ_TOUCHZONE_MSB :
    case PARAMSEL_ENUM_HSS_ZONE11_TRZ_TOUCHZONE_MSB :
    case PARAMSEL_ENUM_HSS_ZONE12_TRZ_TOUCHZONE_MSB :
    case PARAMSEL_ENUM_HSS_ZONE13_TRZ_TOUCHZONE_MSB :
    case PARAMSEL_ENUM_HSS_ZONE14_TRZ_TOUCHZONE_MSB :
    case PARAMSEL_ENUM_HSS_ZONE15_TRZ_TOUCHZONE_MSB :
    #endif // ALSENTISHSSTOUCH_NUM_SENSORS
      // compute the sensor number
      nSensorNum = eParameter - PARAMSEL_ENUM_HSS_ZONE00_TRZ_TOUCHZONE_MSB;
      xValue = HI16( tConfig.atZones[ nSensorNum ].wTrzTouchZone );
      break;

    default :
      break;
  }
  // return the value
  return( xValue );
}
#endif //ALSENTISHSSTOUCH_CONFIG_METHOD

/******************************************************************************
 * @function AlsentisHssTouch_ProcessEvent
 *
 * @brief process's the event
 *
 * This function will process a posted event
 *
 * @param[in]   xArg        event argument
 * 
 * @return      always returns true to flush event
 *
 *****************************************************************************/
BOOL AlsentisHssTouch_ProcessEvent( TASKARG xArg )
{
  // determine the argument
  switch( xArg )
  {
    case TASK_TIMEOUT_EVENT :
      if ( ++wIntervalCount == wIntervalTime )
      {
        // now post a start event
        TaskManager_PostEvent( ALSENTISHSSTOUCH_TASK_ENUM, ALSENTISHSSTOUCH_EVENT_START_CHARGING );

        // reset the count
        wIntervalCount = 0;
      }
      break;

    default :
      #if ( ALSENTISHSSTOUCH_DEBUG_ENABLE == 1 )
      DebugManager_AddElement( ALSENTISHSSTOUCH_DEBUG_BASE + tStateControl.nCurState, xArg );
      #endif
      // process the event
      StateExecutionEngine_Process( &tStateControl, xArg );
      break;
  }

  // always return true
  return( TRUE );
}

/******************************************************************************
 * @function LclStateAdcConv_Exc
 *
 * @brief ADC convert execute
 *
 * This function will process the end of adn ADC conversion
 *
 * @param[in]   xArg        event argument
 * 
 * @return      next state
 *
 *****************************************************************************/
static U8 LclStateIdle_Exc( STATEEXECENGARG xArg )
{
  U8  nNextState = STATEEXECENG_STATE_NONE;

  // check for enabled sensors
  if ( tConfig.xEnabledSensors != 0 )
  {
    // set the next state
    nNextState = LCL_STATE_CHARGE;
  }

  // return the next state
  return( nNextState );
}

/******************************************************************************
 * @function LclStateIdle_Ext
 *
 * @brief idle state exit
 *
 * This function will process the exit routines for the idle state
 *
 *****************************************************************************/
static void LclStateIdle_Ext( void )
{
  // clear the sample index
  nSampleIndex = 0;

  // erase all previous samples
  memset( awSampleValues, 0, ( sizeof( U16 ) * ALSENTISHSSTOUCH_NUM_SENSORS ));
}

/******************************************************************************
 * @function LclStateAdcConv_Ent
 *
 * @brief ADC convert entry
 *
 * This function will clear the ADC index and start a conversion
 *
 *****************************************************************************/
static void LclStateAdcConv_Ent( void )
{
  // clear the sensor index
  nSensorIndex = 0;

  // find a enabled channel
  for ( nSensorIndex = 0; nSensorIndex < ALSENTISHSSTOUCH_NUM_SENSORS; nSensorIndex++ )
  {
    // is sensor enabled
    if ( tConfig.xEnabledSensors & BIT( nSensorIndex ))
    {
      // break
      break;
    }
  }

  // float all input pins
  AlsentisHssTouch_EnableSensors( );

  // start the charging process
  AlsentisHssTouch_StartCharging( &tConfig.tPulseConfig );

  // now start a sample
  AlsentisHssTouch_StartAdcConvert(( ADCENUM )nSensorIndex, &awSampleValues[ nSensorIndex ] );
}

/******************************************************************************
 * @function LclStateAdcConv_Exc
 *
 * @brief ADC convert execute
 *
 * This function will process the end of adn ADC conversion
 *
 * @param[in]   xArg        event argument
 * 
 * @return      next state
 *
 *****************************************************************************/
static U8 LclStateAdcConv_Exc( STATEEXECENGARG xArg )
{
  U8 nNextState = STATEEXECENG_STATE_NONE;

  // ensure valid argument
  if ( xArg == ALSENTISHSSTOUCH_EVENT_ADCCONV_DONE )
  {
    // find next sensor
    do 
    {
      // increment the index
      nSensorIndex++;

    } while ( !( tConfig.xEnabledSensors & BIT( nSensorIndex )) && ( nSensorIndex < ALSENTISHSSTOUCH_NUM_SENSORS ));

    // check for valid sensor
    if ( nSensorIndex < ALSENTISHSSTOUCH_NUM_SENSORS )
    {
      // start a conversion
      AlsentisHssTouch_StartAdcConvert(( ADCENUM )nSensorIndex, &awSampleValues[ nSensorIndex ] );
    }
    else
    {
      // check for number of smaples
      if ( ++nSampleIndex == tConfig.wNumberSampleSets )
      {
        // clear the sample index back to 0
        nSampleIndex = 0;

        // change state to process
        nNextState = LCL_STATE_PROCESS;
      }
      else
      {
        // back to idle
        nNextState = LCL_STATE_IDLE;
      }
    }
  }
  else
  {
    // we have incured an error
    nNextState = LCL_STATE_IDLE;
  }

  // return the next state
  return( nNextState );
}

/******************************************************************************
 * @function LclStateAdcConv_Ext
 *
 * @brief ADC Convert state exit
 *
 * This function will process the exit routines for the Adc exit state
 *
 *****************************************************************************/
static void LclStateAdcConv_Ext( void )
{
  // discharge all sensors
  AlsentisHssTouch_DischargeSensors( );
}

/******************************************************************************
 * @function LclStateProcess_Ent
 *
 * @brief process samples execute
 *
 * This function will process the ADC values
 *
 *****************************************************************************/
static void LclStateProcess_Ent( void )
{
  // for each analog value
  for ( nSensorIndex = 0; nSensorIndex < ALSENTISHSSTOUCH_NUM_SENSORS; nSensorIndex++ )
  {
    ProcessData( );
    // test for a positive value
    if ( awSampleValues[ nSensorIndex ] > 512 )
    {
      // set the appropriate bit
      xSensorStates |= BIT( nSensorIndex );
    }
    else
    {
      // clear the appropriate bit
      xSensorStates &= ~BIT( nSensorIndex );
    }
  }

  // process the touch sensor led state
  AlsentisHssTouch_SetTouchStatusLed(( xSensorStates != 0 ));

  // go back to idle
  TaskManager_PostEvent( TASK_SCHD_ENUM_HSSTOUCH, ALSENTISHSSTOUCH_EVENT_PROCESSING_DONE );
}

#if ( ALSENTISHSSTOUCH_OPER_METHOD == ALSENTISHSSTOUCH_OPER_I2CPERIPH )
/******************************************************************************
 * @function AddToBuffer
 *
 * @brief add a byte to the buffer
 *
 * This function will add a byte to the buffer at the designated index if 
 * space available and return the current number of bytes in buffer
 *
 * @param[in]   nData         data to add
 * @param[in]   pnBuffer      pointer to the buffer
 * @param[in]   nCurIndex     buffer index
 * @param[in]   nMaxSize   maximum number of bytes for the outgoing message
 *
 * @return      the number of bytes in the current buffer
 *
 *****************************************************************************/
static U8 AddToBuffer( U8 nData, PU8 pnBuffer, U8 nCurIndex, U8 nMaxSize )
{
  // check for room
  if ( nCurIndex < nMaxSize )
  {
    // theres room, add the data
    *( pnBuffer + nCurIndex++ ) = nData;
    
    // add to checksum
    nOutgoingChecksum += nData;
  }
  
  // return the current index
  return( nCurIndex );
}

/******************************************************************************
 * @function CalculateAndStuffChecksum
 *
 * @brief calculate and stuff the checksum into the buffer
 *
 * This function will calculate the checksum of a buffer and add the checksum
 * to the end of the buffer
 *
 * @param[in]   pnBuffer      pointer to the buffer
 * @param[in]   nCurSize      current size of the buffer
 *
 * @return      the new buffer size
 *
 *****************************************************************************/
static U8 CalculateAndStuffChecksum( PU8 pnBuffer, U8 nCurSize )
{
  U8 nChecksum = 0;
  U8 nBufIdx;
  
  // for each byte in the buffer
  for ( nBufIdx = 0; nBufIdx < nCurSize; nBufIdx++ )
  {
    // add the byte to the checksum
    nChecksum += *( pnBuffer + nBufIdx );
  }

  // now stuff the byte into the buffer
  *( pnBuffer + nBufIdx++ ) = nChecksum;
  
  // return the new buffer size
  return( nBufIdx );
}
#endif // ALSENTISHSSTOUCH_OPER_METHOD

/******************************************************************************
 * @function ProcessData
 *
 * @brief process the sampled sensor data
 *
 * This function will process the last sampled sensor data and determine
 * if there are sensor touches
 *
 *****************************************************************************/
static void  ProcessData( void )
{
  PLCLCTL       ptLclCtl;
  PALSENTISZONE ptZone;
  S32           lDiffAverage, lCurrSample, lCurrDvDt, lCurrChangeFromStart, lEndChangeFromStart, lTemp;
  U8            nJdx, nIdx;

  // clear the current sample
  lCurrSample = lDiffAverage = 0l;

  // check for differential sensors
  if ( tConfig.nDiffSensorNumber != 0 )
  {
    // for the number of sensors
    for ( nSensorIndex = 0; nSensorIndex < ALSENTISHSSTOUCH_NUM_SENSORS; nSensorIndex++ )
    {
      // set the pointer to the control/zone
      ptLclCtl = &atLclCtl[ nSensorIndex ];
      ptZone = &tConfig.atZones[ nSensorIndex ];

      // fill the change from start array with a large negative number
      ptLclCtl->lSortedNoDiffChangeFromStart = DIFF_CHANGE_FROM_START_INIT;

      // only perform on enabled sensors
      if ( IS_SENSOR_ENABLE( tConfig.xEnabledSensors, nSensorIndex ))  
      {
        // swap the read/write ointer
        if ( ptLclCtl->plBtnAccumRead == &ptLclCtl->lBtnAccum1 )
        {
          ptLclCtl->plBtnAccumRead = &ptLclCtl->lBtnAccum2;
          ptLclCtl->plBtnAccumWrite = &ptLclCtl->lBtnAccum1;  
        }
        else
        {
          ptLclCtl->plBtnAccumRead = &ptLclCtl->lBtnAccum1;
          ptLclCtl->plBtnAccumWrite = &ptLclCtl->lBtnAccum2;
        }

        // calcualte the differential current sample
        ptLclCtl->lNoDiffChangeFromStart = ( *( ptLclCtl->plBtnAccumRead ) * ptZone->wTrzMultiplier ) / ptZone->wTrzDivider;
      }          

      // clear the accumulated read
      *( ptLclCtl->plBtnAccumRead ) = 0;
      ptLclCtl->lNoDiffChangeFromStart = ptLclCtl->lNoDiffStartValue - ptLclCtl->lNoDiffCurrSample;

      // add the non differential change from start only if belongs in the differential sample set
      if ( IS_SENSOR_ENABLE( tConfig.xDiffSensors, nSensorIndex ))
      {
        ptLclCtl->lSortedNoDiffChangeFromStart = ptLclCtl->lNoDiffChangeFromStart;
      }
    }

    // clear the diff average
    lDiffAverage = 0;

    // sort the sorting array
    for( nIdx = 0; nIdx < ALSENTISHSSTOUCH_NUM_SENSORS; nIdx++ )
    {
      for( nJdx = 0; nJdx < ALSENTISHSSTOUCH_NUM_SENSORS - 1; nJdx++ )
      {
        // swap if current is less than next
        if ( atLclCtl[ nJdx ].lSortedNoDiffChangeFromStart < atLclCtl[ nJdx + 1].lSortedNoDiffChangeFromStart )
        {
          lTemp = atLclCtl[ nJdx + 1 ].lSortedNoDiffChangeFromStart;
          atLclCtl[ nJdx + 1 ].lSortedNoDiffChangeFromStart = atLclCtl[ nJdx ].lSortedNoDiffChangeFromStart;
          atLclCtl[ nJdx ].lSortedNoDiffChangeFromStart = lTemp;
        }
      }
    }

    // for the number of sensors
    for ( nSensorIndex = 0; nSensorIndex < ALSENTISHSSTOUCH_NUM_SENSORS; nSensorIndex++ )
    {
      // set the pointer to the control/zone
      ptLclCtl = &atLclCtl[ nSensorIndex ];

      // if the direction is negative add to avergage
      if ( ptLclCtl->lSortedNoDiffChangeFromStart <= 0 )
      {
        // add to average
        lDiffAverage += ptLclCtl->lSortedNoDiffChangeFromStart;
      }
    }

    // now divide by the number of sensors
    lDiffAverage /= tConfig.nDiffSensorNumber;
  }

  // main sensor loop
  for ( nSensorIndex = 0; nSensorIndex < ALSENTISHSSTOUCH_NUM_SENSORS; nSensorIndex++ )
  {
    // set the pointer to the control/zone
    ptLclCtl = &atLclCtl[ nSensorIndex ];
    ptZone = &tConfig.atZones[ nSensorIndex ];

    // only perform on enabled sensors
    if ( IS_SENSOR_ENABLE( tConfig.xEnabledSensors, nSensorIndex ))
    {
      // is this differential
      if ( tConfig.nDiffSensorNumber != 0 )
      {
        // get differntial current sample
        if (( ptLclCtl->lNoDiffChangeFromStart - lDiffAverage ) <= 0 )
        {
          lCurrSample = ptLclCtl->lNoDiffCurrSample + lDiffAverage;
        }
        else
        {
          lCurrSample = ptLclCtl->lNoDiffStartValue;
        }
      }
    }
    else
    {
      // just get current sample
      lCurrSample = ( *( ptLclCtl->plBtnAccumRead ) * ptZone->wTrzMultiplier ) / ptZone->wTrzDivider;
    }

    // clear out the accumaltor
    *( ptLclCtl->plBtnAccumRead ) = 0;

    // set the current DVDT
    lCurrDvDt = ptLclCtl->lLastSample - lCurrSample;

    // determine the state
    switch( ptLclCtl->eBtnState )
    {
      case HSS_BTNSTATE_NOTTOUCHED :
        // clear the latch up counter
        ptLclCtl->uLatchUpCount = 0;

        // are we moving towards a touch
        if ( lCurrDvDt < 0 )
        {
          // set this DVDT as the largest
          ptLclCtl->lBiggestDvdt = lCurrDvDt;

          // copy the last value to the start value
          ptLclCtl->lStartValue = ptLclCtl->lLastSample;

          // set next state to DVDT
          ptLclCtl->eBtnState = HSS_BTNSTATE_DVDT;

          // reset level off/tooslow/aborttouch counters
          ptLclCtl->nLevelOffCntr = 0;
          ptLclCtl->nTooSlowCntr = 0;
          ptLclCtl->nAbortTouchCntr = 0;

          // set the movement counter to 1
          ptLclCtl->nMovementCntr = 1;

          // check for a fast touch
          if ( lCurrDvDt < ptLclCtl->cCurFastDvdtThreshold )
          {
            // set fast touch
            ptLclCtl->bIsFastTouch = TRUE;
            ptLclCtl->nlevelOffDownMin = tConfig.tHss1Config.nFastLvlOffCntMin;
          }
          else
          {
            // set slow touch
            ptLclCtl->bIsFastTouch = FALSE;
            ptLclCtl->nlevelOffDownMin = tConfig.tHss1Config.nSlowLvlOffCntMin;
          }
        }
        else
        {
          // just set the start value to the current sample
          ptLclCtl->lStartValue = lCurrSample;
        }
        break;

      case HSS_BTNSTATE_DVDT :
        // clear the latch up counter
        ptLclCtl->uLatchUpCount = 0;

        // calculate the current change from start/largest change from start
        lCurrChangeFromStart = ptLclCtl->lStartValue - lCurrSample;
        lEndChangeFromStart = ptLclCtl->lStartValue - ptLclCtl->lEndValue;

        // keep track of the largest DVDT value so far
        if ( lCurrDvDt < ptLclCtl->lBiggestDvdt )
        {
          // set the largest to this value
          ptLclCtl->lBiggestDvdt = lCurrDvDt;
        }

        // check for furthest from start point
        if ( lCurrSample > ptLclCtl->lEndValue )
        {
          // set new end oint/reset level offf/too slow/abort counters
          ptLclCtl->lEndValue = lCurrSample;
          ptLclCtl->nLevelOffCntr = 0;
          ptLclCtl->nTooSlowCntr = 0;
          ptLclCtl->nAbortTouchCntr = 0;

          // check for fast if not
          if ( !ptLclCtl->bIsFastTouch )
          {
            // is DVDT less than fast threshold
            if ( lCurrDvDt < ptLclCtl->cCurFastDvdtThreshold )
            {
              // set fast touch
              ptLclCtl->bIsFastTouch = TRUE;
              ptLclCtl->nlevelOffDownMin = tConfig.tHss1Config.nFastLvlOffCntMin;
            }
          }

          // increment the movement counter
          ptLclCtl->nMovementCntr++;
        }
        else
        {
          // is the current change from start in the trap zone or no touch zone
          if ( lCurrChangeFromStart > ptZone->wTrzTouchZone )
          {
            // is the current furthest from start point in the trap zone or no touch zone
            if ( lEndChangeFromStart > ptZone->wTrzTouchZone )
            {
              // now check for fast or too slow counter exceeds max
              if ( ptLclCtl->bIsFastTouch || ( ++ptLclCtl->nTooSlowCntr > tConfig.tHss2Config.nTooSlowCountMax ))
              {
                // change state to no touch
                ptLclCtl->eBtnState = HSS_BTNSTATE_NOTTOUCHED;
              }
              else if (( lCurrChangeFromStart - lEndChangeFromStart ) > ptLclCtl->cCurAbortBoundaries )
              {
                // does the abort counter hit max
                if ( ++ptLclCtl->cCurAbortBoundaries > tConfig.tHss2Config.nAbortTouchCountMax )
                {
                  // set state to not touched
                  ptLclCtl->eBtnState = HSS_BTNSTATE_NOTTOUCHED;
                }
              }
              else
              {
                // change state to no touch
                ptLclCtl->eBtnState = HSS_BTNSTATE_NOTTOUCHED;
              }
            }
          }
          else
          {
            // has the sensor leveled off
            if ( ptLclCtl->nLevelOffCntr >= ptLclCtl->nlevelOffDownMin )
            {
              // is this a touch
              if ( ptLclCtl->nMovementCntr >= tConfig.tHss1Config.nApproachContMin )
              {
                // set the state to touched/set the button to touched/clear the level off/movement counter to 0
                ptLclCtl->eBtnState = HSS_BTNSTATE_TOUCHED;
                xSensorStates |= BIT( nSensorIndex );
                ptLclCtl->nLevelOffCntr = 0;
                ptLclCtl->nMovementCntr = 0;                
              }
              else
              {
                // change state to no touch
                ptLclCtl->eBtnState = HSS_BTNSTATE_NOTTOUCHED;
              }
            }
          }
        }
        break;

      case HSS_BTNSTATE_TRAP :
        break;

      case HSS_BTNSTATE_TOUCHED :
        // increment the latch up counter
        if ( ptLclCtl->uLatchUpCount < tConfig.nLatchTimeoutSecs )
        {
          // increment the latch up count
          ptLclCtl->uLatchUpCount++;
        }

        // if the current change from start is in the no touch zone
        if (( ptLclCtl->lStartValue - lCurrSample ) > ptLclCtl->wNoTouchZone )
        {
          // has level off counter greater than no touch debounce count
          if ( ++ptLclCtl->nLevelOffCntr >= tConfig.tHss1Config.nNoTouchDebounceMin )
          {
            // clear the latch up count/set thest sate to not touched/clear the sensor
            ptLclCtl->uLatchUpCount = 0;
            ptLclCtl->eBtnState = HSS_BTNSTATE_NOTTOUCHED;
            xSensorStates &= ~BIT( nSensorIndex );
          }
        }
        else
        {
          // reset the debounce counter
          ptLclCtl->nLevelOffCntr = 0;
        }
        break;

      default :
        break;
    }

    // keep track of the lass sample
    ptLclCtl->lLastSample = lCurrSample;
    ptLclCtl->lNoDiffLastValue = ptLclCtl->lNoDiffCurrSample;

    // if a sensor's latch up counter has reached its limit
    if (( tConfig.nLatchTimeoutSecs != 0 ) && ( ptLclCtl->uLatchUpCount >= tConfig.nLatchTimeoutSecs ))
    {
      // clear the latch up count/set thest sate to not touched/clear the sensor
      ptLclCtl->uLatchUpCount = 0;
      ptLclCtl->eBtnState = HSS_BTNSTATE_NOTTOUCHED;
      xSensorStates &= ~BIT( nSensorIndex );
    }
  }
}

/**@} EOF AlsentisHssTouch.c */
