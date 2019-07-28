/******************************************************************************
 * @file USBCDCHandler_cfg.h
 *
 * @brief   USB CDC Handler configuration declarations
 *
 * This file provides the declarations for the USB CDC Handler
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
#ifndef _USBCDCHANDLER_CFG_H
#define _USBCDCHANDLER_CFG_H

// system includes ------------------------------------------------------------
#include "Types/Types.h"

// local includes -------------------------------------------------------------

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------
/// define the endpoint size
#define USB_ENDPOINT_SIZE                      ( 64 )

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations -----------------------------------------------
extern  ALIGNED4  const U8  g_anUSBCDCDevDescriptor[ ];
extern  ALIGNED4  const U8  g_anUSBCDCCfgDescriptor[ ];

// global function prototypes --------------------------------------------------
extern  U16 USBCDCHandler_GetDevDescSize( void );
extern  U16 USBCDCHandler_GetCfgDescSize( void );

/**@} EOF USBCDCHandler_cfg.h */

#endif  // _USBCDCHANDLER_CFG_H