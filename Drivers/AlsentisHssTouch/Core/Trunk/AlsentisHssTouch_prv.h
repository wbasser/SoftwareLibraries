/******************************************************************************
 * @file AlsentisHssTouch_prv.h
 *
 * @brief  Alsentis HSS touch private declarations
 *
 * This file provides the definitiona for the Alsentix HSS touch chip
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
#ifndef _ALSENTISHSSTOUCH_PRV_H
#define _ALSENTISHSSTOUCH_PRV_H

// library includes -----------------------------------------------------------
#include "Types/Types.h"

// local includes

// Macros and Defines ---------------------------------------------------------
/// define the commands
#define HSS_CMD_WR_ENB_SENSORS        ( 0x0A )  ///< write enabled sensors
#define HSS_CMD_WR_LATCHUP_TIMEOUT    ( 0x0C )  ///< write latch up timeout
#define HSS_CMD_WR_TRZ                ( 0x0E )  ///< write TRZ for some number of sensors
#define HSS_CMD_WR_DVI                ( 0x16 )  ///< write DVI
#define HSS_CMD_CLR_ERRORS            ( 0x1E )  ///< clear errors
#define HSS_CMD_WR_POWER_MODE         ( 0x40 )  ///< write power mode
#define HSS_CMD_WR_PROC_INTVL         ( 0x41 )  ///< write process interval
#define HSS_CMD_WR_NUM_SAMP_SETS      ( 0x43 )  ///< write number of sample sets
#define HSS_CMD_WR_PULSE_CONFIG       ( 0x44 )  ///< write the pulse configuration
#define HSS_CMD_WR_SIG_CONFIG1        ( 0x45 )  ///< write signature configuration #1
#define HSS_CMD_WR_SIG_CONFIG2        ( 0x46 )  ///< write signature configuration #2
#define HSS_CMD_WR_SLEEP_TIME         ( 0x48 )  ///< write sleep time
#define HSS_CMD_WR_DIFF_CONFIG        ( 0x49 )  ///< differential mode configuration
#define HSS_CMD_WR_IRQ_ENABLED        ( 0x4A )  ///< write IRQ enable
#define HSS_CMD_RESET                 ( 0x52 )  ///< reset
#define HSS_CMD_RD_ID                 ( 0x81 )  ///< read ID
#define HSS_CMD_RD_ENABLED_SENSORS    ( 0x8A )  ///< read enabled sensors
#define HSS_CMD_RD_LATCHUP_TIMEOUT    ( 0x8C )  ///< read latchup timeout
#define HSS_CMD_RD_TRZ                ( 0x8E )  ///< read TRZ
#define HSS_CMD_RD_STATUS             ( 0x9E )  ///< read status
#define HSS_CMD_RD_SENSOR_STATES      ( 0x9F )  ///< read sensor states
#define HSS_CMD_RD_POWER_MODE         ( 0xC0 )  ///< read power mode
#define HSS_CMD_RD_PROC_INTVL         ( 0xC1 )  ///< read process interval
#define HSS_CMD_RD_NUM_SAMP_SETS      ( 0xC3 )  ///< read number of sample sets
#define HSS_CMD_RD_PULSE_CONFIG       ( 0xC4 )  ///< read the pulse configuration
#define HSS_CMD_RD_SIG_CONFIG1        ( 0xC5 )  ///< read signature configuration #1
#define HSS_CMD_RD_SIG_CONFIG2        ( 0xC6 )  ///< read signature configuration #2
#define HSS_CMD_RD_DVI                ( 0xC7 )  ///< read DVI
#define HSS_CMD_RD_SLEEP_TIME         ( 0xC8 )  ///< read sleep time
#define HSS_CMD_RD_DIFF_CONFIG        ( 0xC9 )  ///< differential config mode
#define HSS_CMD_RD_IRQ_ENABLED        ( 0xCA )  ///< read IRQ enable

// enumerations ---------------------------------------------------------------
/// enumerate the button states
typedef enum _HSSBTNSTATE
{
  HSS_BTNSTATE_NOTTOUCHED = 0,        ///< not touched
  HSS_BTNSTATE_DVDT,                  ///< changing state
  HSS_BTNSTATE_TRAP,                  ///< trapped state
  HSS_BTNSTATE_TOUCHED,               ///< touched
} HSSBTNSTATE;

// structures -----------------------------------------------------------------

/**@} EOF AlsentisHssTouch_prv.h */

#endif  // _ALSENTISHSSTOUCH_PRV_H
