/******************************************************************************
 * @file USBCDCHandler_cfg.c
 *
 * @brief USB CDC Hnadler configuration implementation
 *
 * This file provides the implementation for the USB CDC handler
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

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "USBCDCHandler/USBCDCHandler_cfg.h"

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations ----------------------------------------------

// local parameter declarations -----------------------------------------------

// local function prototypes --------------------------------------------------

// constant parameter initializations -----------------------------------------
ALIGNED4  const U8  g_anUSBCDCDevDescriptor[ ] =
{
	0x12,   // bLength
	0x01,   // bDescriptorType
	0x10,   // bcdUSBL
	0x01,   //
	0xEF,   // bDeviceClass:    CDC class code
	0x02,   // bDeviceSubclass: CDC class sub code
	0x01,   // bDeviceProtocol: CDC Device protocol
	0x40,   // bMaxPacketSize0
	0xEB,   // idVendorL
	0x03,   //
	0x00,   // idProductL
	0x22,   //
	0x10,   // bcdDeviceL
	0x01,   //
	0x00,   // iManufacturer    // 0x01
	0x00,   // iProduct
	0x00,   // SerialNumber
	0x01    // bNumConfigs
};

ALIGNED4  const U8  g_anUSBCDCCfgDescriptor[ ] =
{
	// Configuration 1 descriptor
	0x09,   // CbLength
	0x02,   // CbDescriptorType
	0x43,   // CwTotalLength 2 EP + Control
	0x00,
	0x02,   // CbNumInterfaces
	0x01,   // CbConfigurationValue
	0x00,   // CiConfiguration
	0xC0,   // CbmAttributes 0xA0
	0x00,   // CMaxPower

	// Communication Class Interface Descriptor Requirement
	0x09,   // bLength
	0x04,   // bDescriptorType
	0x00,   // bInterfaceNumber
	0x00,   // bAlternateSetting
	0x01,   // bNumEndpoints
	0x02,   // bInterfaceClass
	0x02,   // bInterfaceSubclass
	0x00,   // bInterfaceProtocol
	0x00,   // iInterface

	// Header Functional Descriptor
	0x05,   // bFunction Length
	0x24,   // bDescriptor type: CS_INTERFACE
	0x00,   // bDescriptor subtype: Header Func Desc
	0x10,   // bcdCDC:1.1
	0x01,

	// ACM Functional Descriptor
	0x04,   // bFunctionLength
	0x24,   // bDescriptor Type: CS_INTERFACE
	0x02,   // bDescriptor Subtype: ACM Func Desc
	0x00,   // bmCapabilities

	// Union Functional Descriptor
	0x05,   // bFunctionLength
	0x24,   // bDescriptorType: CS_INTERFACE
	0x06,   // bDescriptor Subtype: Union Func Desc
	0x00,   // bMasterInterface: Communication Class Interface
	0x01,   // bSlaveInterface0: Data Class Interface

	// Call Management Functional Descriptor
	0x05,   // bFunctionLength
	0x24,   // bDescriptor Type: CS_INTERFACE
	0x01,   // bDescriptor Subtype: Call Management Func Desc
	0x00,   // bmCapabilities: D1 + D0
	0x01,   // bDataInterface: Data Class Interface 1

	// Endpoint 1 descriptor
	0x07,   // bLength
	0x05,   // bDescriptorType
	0x83,   // bEndpointAddress, Endpoint 03 - IN
	0x03,   // bmAttributes      INT
	0x08,   // wMaxPacketSize
	0x00,
	0xFF,   // bInterval

	// Data Class Interface Descriptor Requirement
	0x09,   // bLength
	0x04,   // bDescriptorType
	0x01,   // bInterfaceNumber
	0x00,   // bAlternateSetting
	0x02,   // bNumEndpoints
	0x0A,   // bInterfaceClass
	0x00,   // bInterfaceSubclass
	0x00,   // bInterfaceProtocol
	0x00,   // iInterface

	// Endpoint 1 descriptor
	0x07,   // bLength
	0x05,   // bDescriptorType
  0x81,   // bEndpointAddress, Endpoint 01 - IN
	0x02,   // bmAttributes      BULK
	USB_ENDPOINT_SIZE,   // wMaxPacketSize
	0x00,
	0x00,   // bInterval

	// Endpoint 2 descriptor
	0x07,   // bLength
	0x05,   // bDescriptorType
	0x02,   // bEndpointAddress, Endpoint 02 - OUT
	0x02,   // bmAttributes      BULK
	USB_ENDPOINT_SIZE,   // wMaxPacketSize
	0x00,
	0x00    // bInterval
};

/******************************************************************************
 * @function USBCDCHandler_GetDevDescSize
 *
 * @brief get the device descriptor size
 *
 * This function returns the size of the device descriptor
 *
 * @return      size of the device descriptor
 *
 *****************************************************************************/
U16 USBCDCHandler_GetDevDescSize( void )
{
  // return the size
  return( sizeof( g_anUSBCDCDevDescriptor )); 
}

/******************************************************************************
 * @function USBCDCHandler_GetCfgDescSize
 *
 * @brief get the config descriptor size
 *
 * This function returns the size of the config descriptor
 *
 * @return      sze of the config descriptor
 *
 *****************************************************************************/
U16 USBCDCHandler_GetCfgDescSize( void )
{
  // return the size
  return( sizeof( g_anUSBCDCCfgDescriptor )); 
}

/**@} EOF USBCDCHandler.c */
