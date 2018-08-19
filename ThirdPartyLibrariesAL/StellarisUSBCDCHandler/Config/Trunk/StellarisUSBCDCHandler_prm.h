/******************************************************************************
 * @file USBCDCHandler_prm.h
 *
 * @brief USB CDC handler parameter declarations
 *
 * This file provides the parameter declarations for the CDC handler
 *
 * @copyright Copyright (c) 2012 Cyber Intergration
 * This document contains proprietary data and information of Endurance Products 
 * LLC. It is the eUSBCDCHANDLER_PRMclusive property of Endurance Products, LLC and will not be 
 * disclosed in any form to any party without prior written permission of 
 * Endurance Products, LLC. This document may not be reproduced or further used 
 * without the prior written permission of Endurance Products, LLC.
 *
 * Version History
 * ======
 * $Log: $
 * 
 *
 * \addtogroup USBCDCHandler
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _USBCDCHANDLER_PRM_H
#define _USBCDCHANDLER_PRM_H

// local includes -------------------------------------------------------------
#include "TaskManager/TaskManager.h"
#include "GPIO/Gpio.h"

// Macros and Defines ---------------------------------------------------------
/// define the receive character processor task enumeration
#define USBCDCHANDLER_RCVCHR_TASK       ( TASK_ENUM_ILLEGAL )

/// define the GPIO enumeration for the USB connected LED
#define USBCDCHANDLER_USBCON_LED        ( GPIO_PIN_ENUM_ILLEGAL )

/**@} EOF USBCDCHandler_prm.h */

#endif  // _USBCDCHANDLER_PRM_H