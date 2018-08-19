/******************************************************************************
 * @file UsbDeviceHandler.h
 *
 * @brief USB device handler declarations 
 *
 * This file provides the declarations for the USB device handler
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
 * \addtogroup UsbDeviceHandler
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _USBDEVICEHANDLER_H
#define _USBDEVICEHANDLER_H

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "UsbDeviceHandler/UsbDeviceHandler_cfg.h"

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations -----------------------------------------------

// global function prototypes --------------------------------------------------
extern  void  UsbDeviceHandler_Initialize( void );
extern  void  UsbDeviceHandler_HandleSetup( PUSBSETUPPACKET ptData );

/**@} EOF UsbDeviceHandler.h */

#endif  // _USBDEVICEHANDLER_H