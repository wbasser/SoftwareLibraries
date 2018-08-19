/******************************************************************************
 * @file EepromHandler_cfg.h
 *
 * @brief EEPROM handler configuration declarations 
 *
 * This file provides the declarations for the EEPROM handler
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
 * \addtogroup EepromHandler
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _EEPROMHANDLER_CFG_H
#define _EEPROMHANDLER_CFG_H

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "EepromHandler/EepromHandler_prm.h"

// library includes -----------------------------------------------------------
#if (( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_TASKMANAGER ) && ( EEPROMHANDLER_ENABLE_BACKGROUND_WRITES == 1 ))
#include "TaskManager/TaskManager.h"
#endif // ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_TASKMANAGER )

// Macros and Defines ---------------------------------------------------------
#if ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_TASKMANAGER )
#define EEPROM_BACKGROUND_WRITE_EXEC_RATE         ( TASK_TIME_MSECS( EEPROMHANDLER_PAGE_WRITE_MSECS ))
#define EEPROM_BACKGROUND_WRITE_NUM_EVENTS        ( 4 )
#endif // ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_TASKMANAGER )

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations -----------------------------------------------

// global function prototypes --------------------------------------------------
extern  void  EepromHandler_LclInitialize( void );
#if (( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_TASKMANAGER ) && ( EEPROMHANDLER_ENABLE_BACKGROUND_WRITES == 1 ))
extern  BOOL  EepromHandler_ProcessBackgroundWrite( TASKARG xArg );
#endif // ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_TASKMANAGER )

/**@} EOF EepromHandler_cfg.h */

#endif  // _EEPROMHANDLER_CFG_H