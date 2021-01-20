/******************************************************************************
 * @file USBCDCHandler.h
 *
 * @brief USB-CDC handler declarations
 *
 * This file provides the declarations for the USB-CDC handler
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
 * \addtogroup USBCDCHandler
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _USBCDCHANDLER_H
#define _USBCDCHANDLER_H

// system includes ------------------------------------------------------------
#include "USBCDCHandler/USBCDCHandler_cfg.h"

// local includes -------------------------------------------------------------

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations -----------------------------------------------

// global function prototypes --------------------------------------------------
extern  void  USBCDCHandler_Initialize( void );
extern  BOOL  USBCDCHandler_AreCharsAvailable( U8 nDev );
extern  BOOL  USBCDCHandler_IsConfigured( U8 nDev );
extern  U16   USBCDCHandler_GetData( U8 nDev, PU8 pnData, U16 wLength );
extern  U16   USBCDCHandler_PutData( U8 nDev, PU8 pnData, U16 wLength );
extern  void  USBCDCHandler_Close( U8 nDev );

/**@} EOF USBCDCHandler.h */

#endif  // _USBCDCHANDLER_H