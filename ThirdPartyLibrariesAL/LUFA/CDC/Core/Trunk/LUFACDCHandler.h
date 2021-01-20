/******************************************************************************
 * @file LUFACDCHandler.h
 *
 * @brief LUFA CDC handler declarations
 *
 * This file 
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
 * \addtogroup 
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _LUFACDCHANDLER_H
#define _LUFACDCHANDLER_H

// library includes -----------------------------------------------------------
#include "TaskManager/TaskManager.h"

// local includes -------------------------------------------------------------
#include "LUFACDCHandler/LUFACDCHandler_cfg.h"

// global parameter declarations
extern  FILE* g_ptLUFACDCStream;

// global function prototypes --------------------------------------------------
extern  void  LUFACDCHandler_Initialize( void );
extern  void  LUFACDCHandler_ProcessIdle( void );
extern  void  LUFACDCHandler_PutChar( U8 nChar );
extern  void  LUFACDCHandler_WriteData( PU8 pnData, U16 wLength );
extern  void  EVENT_USB_Device_Connect( void );
extern  void  EVENT_USB_Device_Disconnect( void );
extern  void  EVENT_USB_Device_ConfigurationChanged( void );
extern  void  EVENT_USB_Device_ControlRequest( void );
extern  U16   CALLBACK_USB_GetDescriptor(const U16 wValue, const U16 wIndex, const void** const DescriptorAddress) ATTR_WARN_UNUSED_RESULT ATTR_NON_NULL_PTR_ARG( 3 );

/**@} EOF LUFACDCHandler.h */

#endif  // _LUFACDCHANDLER_H