/******************************************************************************
 * @file AlsentisHssChipHandler.h
 *
 * @brief Alsentis HSS Touch CHip handler declarations 
 *
 * This file provides the delcarations for the Alsentis HSS chip touch chip
 * handler
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
 * \addtogroup AlsentisHssChipHandler
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _ALSENTISHSSCHIPHANDLER_H
#define _ALSENTISHSSCHIPHANDLER_H

// system includes ------------------------------------------------------------

// library includes -----------------------------------------------------------
#include "TaskManager/TaskManager.h"

// local includes -------------------------------------------------------------
#include "AlsentisHssChipHandler/AlsentisHssChipHandler_cfg.h"

// Macros and Defines ---------------------------------------------------------
/// define the number of events
#define ALSENTISHSSCHIPHANDLER_NUM_EVENTS           ( 4 )

// enumerations ---------------------------------------------------------------
typedef enum _ALSENTISCHIPERR
{
  ALSENTIS_CHIPERR_NONE = 0,
  ALSENTIS_CHIPERR_ILLENUM = 0xB0,
  ALSENTIS_CHIPERR_ILLSENSOR,
  ALSENTIS_CHIPERR_NOTVALID,
} ALSENTISCHIPERR;

// structures -----------------------------------------------------------------

// global parameter declarations -----------------------------------------------

// global function prototypes --------------------------------------------------
extern  void            AlsentisHssChipHandler_Initialize( void );
extern  void            AlsentisHssChipHandler_ProcessIrq( GPIOPINENUM eIrqEnum, GPIOIRQCBEVENT eEvent );
extern  BOOL            AlsentisHssChipHandler_ProcessEvent( TASKARG xArg );
extern  ALSENTISCHIPERR AlsentisHssChipHandler_GetSensors( ALENTISHSSDCHIPEVENUM eChipEnum, PU32 puSensors );
extern  ALSENTISCHIPERR AlsentisHssChipHandler_GetSensorStatus( ALENTISHSSDCHIPEVENUM eChipEnum, U8 nSensorIdx, PBOOL pbState );

/**@} EOF AlsentisHssChipHandler.h */

#endif  // _ALSENTISHSSCHIPHANDLER_H