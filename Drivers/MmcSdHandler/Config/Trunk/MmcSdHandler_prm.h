/******************************************************************************
 * @file MmcSdHandler_prm.h
 *
 * @brief MMC/SD card handler parameters
 *
 * This file provides the card handler parameters
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
 * \addtogroup MmcSdHandler
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _MMCSDHANDLER_PRM_H
#define _MMCSDHANDLER_PRM_H

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------
/// define the card detect enumeration
#define MMCSDHANDLER_CARD_DETECT_ENUM           ( GPIO_PIN_ENUM_ILLEGAL )

/// define the card select enumeration
#define MMCSDHANLDER_CHIP_SELECT_ENUM           ( GPIO_PIN_ENUM_ILLEGAL )

/// define the execution rate for the MMC/SD handler timer
#define MMCSDHANDLER_TIMER_MSECS                ( 10 )

/// define the card detect options
#define MMCSDHANDLER_CARDDETECT_MODE_NONE       ( 0 )
#define MMCSDHANDLER_CARDDETECT_MODE_POLL       ( 1 )
#define MMCSDHANDLER_CARDDETECT_MODE_IRQ        ( 2 )

/// set the macro bebow to one of the above modes
#define MCSDHANDLER_CARDDETECT_MODE             ( MMCSDHANDLER_CARDDETECT_MODE_NONE )

/// define the task handler to post card detect events
#define MCSDHANDLER_CARDDETECT_TASK_ENUM        ( TASK_SCHD_ILLEGAL )

/// define the task event
#define MCDSDHANDLER_CARDDETECT_INSERTED_EVENT  ( 0 )
#define MCDSDHANDLER_CARDDETECT_REMOVED_EVENT   ( 0 )

/**@} EOF MmcSdHandler_prm.h */

#endif  // _MMCSDHANDLER_PRM_H