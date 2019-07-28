/******************************************************************************
 * @file USBCBCHandler.h
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
 * \addtogroup USBCBCHandler
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _USBCBCHANDLER_H
#define _USBCBCHANDLER_H

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

// global function prototypes --------------------------------------------------
extern  void  USBCBCHandler_Initialize( void );

/**@} EOF USBCBCHandler.h */

#endif  // _USBCBCHANDLER_H