/******************************************************************************
 * @file AlsentisHssTouch.h
 *
 * @brief Alsentis HSS driver declarations 
 *
 * This file provides the declarations for the Alsentis HSS driver
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
 
// ensure only one instantiation
#ifndef _ALSENTISHSSTOUCH_H
#define _ALSENTISHSSTOUCH_H

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "AlsentisHssTouch/AlsentisHssTouch_cfg.h"

// library includes -----------------------------------------------------------
#include "TaskManager/TaskManager.h"
#if ( ALSENTISHSSTOUCH_CONFIG_METHOD == ALSENTISHSSTOUCH_PARAMMGR_SEL )
#include "ParameterManager/ParameterManager.h"
#endif // ALSENTISHSSTOUCH_CONFIG_METHOD

// Macros and Defines ---------------------------------------------------------
/// define the number of events
#define ALSENTISHSSTOUCH_NUM_EVENTS         ( 4 )

/// define the execution rate
#define ALSENTISHSSTOUCH_EXEC_RATE          ( TASK_TIME_MSECS( 1 ))

/// define the slave address
#define ALSENTISHSSTOUCH_I2CSLV_ADDRESS     ( 0x58 )

// enumerations ---------------------------------------------------------------
/// enumerate the errors
typedef enum _ALSENTISERR
{
  ALSENTISERR_TOUCH_NOPRESS = 0,        ///< no press
  ALSENTISERR_TOUCH_PRESS,              ///< press
  ALSENTISERR_ILLEGAL_TOUCH = 0xD0,     ///< illegal sensor
} ALSENTISERR;

// structures -----------------------------------------------------------------

// global parameter declarations -----------------------------------------------

// global function prototypes --------------------------------------------------
extern  void            AlsentisHssTouch_Initialize( void );
extern  ALSENTISERR     AlsentisHssTouch_GetTouch( U8 nSensor );
extern  SENSORVAR       AlsentisHssTouch_GetSensorStates( void );
extern  void            AlsentisHssTouch_Configure( void );
extern  BOOL            AlsentisHssTouch_ProcessEvent( TASKARG xArg );
#if ( ALSENTISHSSTOUCH_OPER_METHOD == ALSENTISHSSTOUCH_OPER_I2CPERIPH )
extern  U8              AlsentisHSSTouch_ProcessIncomingMsg( PU8 pnInBuffer, U8 nInSize, PU8 pnOutBuffer, U8 nMaxOutSize );
#else
extern  PALSENTISCONFIG AlsentisHSSTouch_GetConfigDflt( void );
extern  PALSENTISCONFIG AlsentisHSSTouch_GetConfigActl( void );
#endif // ALSENTISHSSTOUCH_OPER_METHOD
#if ( ALSENTISHSSTOUCH_CONFIG_METHOD == ALSENTISHSSTOUCH_PARAMMGR_SEL )
extern  PARAMARG        AlsentisHssTouch_GetConfigValue( PARAMSELENUM eParameter );
#endif // ALSENTISHSSTOUCH_CONFIG_METHOD

/**@} EOF AlsentisHssTouch.h */

#endif  // _ALSENTISHSSTOUCH_H