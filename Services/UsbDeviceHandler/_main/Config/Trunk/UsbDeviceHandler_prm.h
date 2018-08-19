/******************************************************************************
 * @file UsbDeviceHandler_prm.h
 *
 * @brief USB device handler parameters 
 *
 * This file provides the declarations for the USB device hadnler
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
#ifndef _USBDEVICEHANDLER_PRM_H
#define _USBDEVICEHANDLER_PRM_H

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------
/// define the vendor/product ID
#define USB_VENDOR_ID                       ( 0x0000 )
#define USB_PRODUCT_ID                      ( 0x0000 )

/// define the endpoints for the CDC
#define USB_EP_CDC_NOTIFICATION             ( 0x83 )
#define USB_EP_CDC_IN                       ( 0x84 )
#define USB_EP_CDC_OUT                      ( 0x04 )

/**@} EOF UsbDeviceHandler_prm.h */

#endif  // _USBDEVICEHANDLER_PRM_H