/******************************************************************************
 * @file LUFACDCHandler_prm.h
 *
 * @brief LUFA CDC Handler parameter declarations
 *
 * This file provides the declarations for the LUFA CDC handler
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
 * $Log: $
 * 
 *
 * \addtogroup LUFA
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _LUFACDCHANDLER_PRM_H
#define _LUFACDCHANDLER_PRM_H

// libary includes -------------------------------------------------------------
#include "TaskManager/TaskManager.h"

// Macros and Defines ---------------------------------------------------------
/// Endpoint address of the CDC device-to-host notification IN endpoint.
#define CDC_NOTIFICATION_EPADDR        ( ENDPOINT_DIR_IN  | 2 )

/// Endpoint address of the CDC device-to-host data IN endpoint.
#define CDC_TX_EPADDR                  ( ENDPOINT_DIR_IN  | 3 )

/// Endpoint address of the CDC host-to-device data OUT endpoint.
#define CDC_RX_EPADDR                  ( ENDPOINT_DIR_OUT | 4 )

/// Size in bytes of the CDC device-to-host notification IN endpoint.
#define CDC_NOTIFICATION_EPSIZE        ( 8 )

/// Size in bytes of the CDC data IN and OUT endpoints.
#define CDC_TXRX_EPSIZE                ( 16 )

/// define the task to call upon receipt of a character
#define LUFACDC_PROCESSCHAR_TASK        ( TASK_SCHD_ILLEGAL )

/// define the task handler to process connection status
#define LUFACDC_PROCESSCON_TASK         ( TASK_SCHD_ILLEGAL )

/// define the event to post on connect
#define LUFACDC_CONNECT_EVENT            ( 0 )
  
/// define the event to post on disconnect
#define LUFACDC_DISCONNECT_EVENT         ( 0 )
    
/**@} EOF LUFACDCHandler_prm.h */

#endif  // _LUFACDCHANDLER_PRM_H