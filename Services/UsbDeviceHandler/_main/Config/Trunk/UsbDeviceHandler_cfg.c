/******************************************************************************
 * @file UsbDeviceHandler_cfg.c
 *
 * @brief USB device handler confgiuration implementation 
 *
 * This file provides the implementation for the device handler configuration
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

// system includes ------------------------------------------------------------
#include "UsbDeviceHandler/CDCClass/UsbDeviceHandlerCDC_def.h"

// local includes -------------------------------------------------------------
#include "UsbDeviceHandler/UsbDeviceHandler_cfg.h"
#include "UsbDeviceHandler/UsbDeviceHandler_prm.h"

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------
#define USB_VERSION_BCD(Major, Minor, Revision) \
        (((Major & 0xFF) << 8) | \
         ((Minor & 0x0F) << 4) | \
         (Revision & 0x0F))

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations ----------------------------------------------
U8  anUsbBufOut[ USB_CDC_IN_OUT_BUF_SIZE ];

// local parameter declarations -----------------------------------------------

// local function prototypes --------------------------------------------------

// constant parameter initializations -----------------------------------------
/// instantiate the device descriptor
const ALIGNED4  USBDEVICEDESCRIPTOR   tUsbDeviceDescriptor =
{
	.nLength                      = USBDEVICEDESCRIPTOR_SIZE,
	.nDescriptorType              = USB_DTYPE_DEVICE,
	.wCdUSB                       = 0x0200,
	.nDeviceClass                 = 0,    
	.nDeviceSubClass              = USB_CSCP_NODEVICESUNCLASS,
	.nDeviceProtocol              = USB_CSCP_NODEVICEPROTOCOL,
	.nMaxPacketSize0              = USB_EP0_BUF_SIZE, 
	.wVendor                      = USB_VENDOR_ID, 
	.wProduct                     = USB_PRODUCT_ID,
	.wDevice                      = USB_VERSION_BCD( 1, 1, 0 ),
	.nManufacturer                = USB_STRING_ID_MANUFACTURER,
	.nProduct                     = USB_STRING_ID_PRODUCT,
	.nSerialNumner                = USB_STRING_ID_SERIALNUM,
	.nNumConfigurations           = 1
};

/// populate the configuration descriptor
const ALIGNED4  USBCONFIGURATIONDESCR tUsbConfigDescriptor =
{
  // configuration descriptor
  .tConfigDesc                  =
  {
    .nLength                    = USBDEVICEDESCRIPTOR_SIZE,
    .nDescriptorType            = USB_DTYPE_CONFIGURATION,
    .wTotalLength               = USBCONFIGURATIONDESCR_SIZE,
    .nNumInterfaces             = 3,
    .nConfigurationValue        = 1,
    .nConfiguration             = 0,
    .nAttrinutes                = USB_CONFIG_ATTR_BUSPOWERED,
    .nMaxPower                  = USB_CONFIG_POWER_MA(100)
  },

  // interface description
  .tInterfaceDsesc              =
  {
    .nLength                    = USBINTERFACEDESCRIPTOR_SIZE,
    .nDescriptorType            = USB_DTYPE_INTERFACE,
    .nInterfaceNumber           = INTFACE_ENUM_VENDOR,
    .nAlternateSetting          = 0,
    .nNumEndpoints              = 0,
    .nInterfaceClass            = USB_CSCP_VENDORSPECIFICCLASS,
    .nInterfaceSunClass         = 0,
    .nInterfaceProtocol         = 0,
    .nInterface                 = 0,
  },

  // CDC tonrol description
  .tCdcControlDsesc             =
  {
    .nLength                    = USBINTERFACEDESCRIPTOR_SIZE,
    .nDescriptorType            = USB_DTYPE_INTERFACE,
    .nInterfaceNumber           = INTFACE_ENUM_CDC_CONTROL,
    .nAlternateSetting          = 0,
    .nNumEndpoints              = 1,
    .nInterfaceClass            = USB_CDC_INTERFACE_CLASS,
    .nInterfaceSunClass         = USB_CDC_INTERFACE_SUBCLASS_ACM,
    .nInterfaceProtocol         = 0,
    .nInterface                 = 0,
  },

  // CDC functional header
  .tCdcFuncHeader               =
  {
    .nLength                    = USBDEVCDCFUNCHEADDESC_SIZE,
    .nDescriptorType            = USB_DTYPE_CSINTERFACE,
    .nDescriptorSubtype         = USB_CDC_SUBTYPE_HEADER,
    .wCdCDC                     = USB_VERSION_BCD(  1, 1, 0 )
  },

  /// DCD funcation ACM
  .tCdcFuncAcm                  =
  {
    .nLength                    = USBDEVCDCFUNCACMDESC_SIZE,
    .nDescriptorType            = USB_DTYPE_CSINTERFACE,
    .nDescriptorSubtype         = USB_CDC_SUBTYPE_ACM,
    .nCapabilities              = 0,
  },

  /// CDC function Union
  .tCdcFuncUnion                =
  {
    .nLength                    = USBDEVCDCFUNCNIONDESC_SIZE,
    .nDescriptorType            = USB_DTYPE_CSINTERFACE,
    .nDescriptorSubtype         = USB_CDC_SUBTYPE_UNION,
    .nMasterInterface           = INTFACE_ENUM_CDC_CONTROL,
    .nSlaveInterface            = INTFACE_ENUM_CDC_DATA
  },

  /// CDC notification endpoint
  .tCdcNotificationEndpoint     =
  {
    .nLength                    = USBENDPOINTDESCRIPTOR_SIZE,
    .nDescriptorType            = USB_DTYPE_ENDPOINT,
    .nEndpointAddress           = USB_EP_CDC_NOTIFICATION,
    .nAttributes                = ( USB_EP_TYPE_INTERRUPT | ENDPOINT_ATTR_NO_SYNC  | ENDPOINT_USAGE_DATA ),
    .wMaxPacketSize             = 8,
    .nInterval                  = 0xFF
  },

  /// CDC data interface
  .tCdcDatInterface             =  
  {
    .nLength                    = USBINTERFACEDESCRIPTOR_SIZE,
    .nDescriptorType            = USB_DTYPE_INTERFACE,
    .nInterfaceNumber           = INTFACE_ENUM_CDC_DATA,
    .nAlternateSetting          = 0,
    .nNumEndpoints              = 2,
    .nInterfaceClass            = USB_CDC_INTERFACE_CLASS_DATA,
    .nInterfaceSunClass         = 0,
    .nInterfaceProtocol         = 0,
    .nInterface                 = 0,
  },

  /// CDC out endpoint
  .tCdcOutEndpoint              =
  {
    .nLength                    = USBENDPOINTDESCRIPTOR_SIZE,
    .nDescriptorType            = USB_DTYPE_ENDPOINT,
    .nEndpointAddress           = USB_EP_CDC_OUT,
    .nAttributes                = ( USB_EP_TYPE_BULK | ENDPOINT_ATTR_NO_SYNC  | ENDPOINT_USAGE_DATA ),
    .wMaxPacketSize             = USB_EP0_BUF_SIZE,
    .nInterval                  = 5,
  },

  /// CDC in endpoint
  .tCdcInENdpoint               =
  {
    .nLength                    = USBENDPOINTDESCRIPTOR_SIZE,
    .nDescriptorType            = USB_DTYPE_ENDPOINT,
    .nEndpointAddress           = USB_EP_CDC_IN,
    .nAttributes                = ( USB_EP_TYPE_BULK | ENDPOINT_ATTR_NO_SYNC  | ENDPOINT_USAGE_DATA ),
    .wMaxPacketSize             = USB_EP0_BUF_SIZE,
    .nInterval                  = 5,
  }
};

// instantiate the language string
const ALIGNED4  USBSTRINGDESCRIPTOR tUsbLangueString =
{
	.nLength                       = USB_STRING_LEN( 1 ),
	.nDescriptorType               = USB_DTYPE_STRING,
	.tString                       = { USB_LANGUAGE_EN_US },
};

/// instantiate the MSFT string
const ALIGNED4  USBSTRINGDESCRIPTOR tUsbMsftOSString =
{
	.nLength                      = USB_STRING_LEN( 18 ),
	.nDescriptorType              = USB_DTYPE_STRING,
	.tString                      = { u"MFST100\xEE" },
};

// declare the manufacturer product strings
const C8  szUsbManfString[ ] = { "Manufacturer" };
const C8  szUsbProdString[ ] = { "Product" };

// instantiate the miscosoft compatable descriptor
const ALIGNED4  USBMSFTCOMPDESCRIPTOR tUsbMstComp =
{
	.uLength                  = USBMSFTCOMPDESCRIPTOR_SIZE + ( 3 * USBMSFTCOMPDESCRIPTOR_SIZE ),
	.nCdVersion               = USB_VERSION_BCD( 1, 0, 0 ),
	.wIndex                   = 0x0004,
	.nCount                   = 3,
	.anReserved               =
  {
    0, 0, 0, 0, 0, 0, 0
  },

	.tInterfaces              =
  {
    {
      .nFirstIntfNumber      = 0,
      .nReserved1            = 1,
      .anCompatinleID         = 
      {
        "WINUSB\0\0",
      },
      .anSunCompatinleID       =
      {
        0, 0, 0, 0, 0, 0, 0, 0 
      },
      .anReserved2             = 
      {
        0, 0, 0, 0, 0 
      },
    },

    {
      .nFirstIntfNumber      = 1,
      .nReserved1            = 1,
      .anCompatinleID         = 
      {
        "WINUSB\0\0",
      },
      .anSunCompatinleID       =
      {
        0, 0, 0, 0, 0, 0, 0, 0 
      },
      .anReserved2             = 
      {
        0, 0, 0, 0, 0 
      },
    },

    {
      .nFirstIntfNumber      = 2,
      .nReserved1            = 1,
      .anCompatinleID         = 
      {
        "WINUSB\0\0",
      },
      .anSunCompatinleID       =
      {
        0, 0, 0, 0, 0, 0, 0, 0 
      },
      .anReserved2             = 
      {
        0, 0, 0, 0, 0 
      },
    },
  },
};

/// instantiate the Microsoft properties dewscrptor
const ALIGNED4  USBMSFTPROPDESCRIPTOR tUsbMstProd =
{
	.uLength                  = 146,
	.wBcdVersion              = USB_VERSION_BCD( 1, 0, 0 ),
	.wIndex                   = 5,
	.wCount                   = 1,
	.uPropLength              = 136,
	.uType                    = 7,
	.wNameLength              = 42,
	.awName                    =
  {
    u"DeviceInterfaceGUIDS\0",
  },
	.uDataLength              = 80,
	.awData                   =
  {
    u"{3c33bbfd-71f9-4815-8b8f-7cd1ef928b3d}\0\0",
  },
	.anPadding                =
  {
    0, 0
  },
};

/******************************************************************************
 * @function 
 *
 * @brief 
 *
 * This function 
 *
 * @param[in]   
 *
 * @return      
 *
 *****************************************************************************/
 
/**@} EOF UsbDeviceHandler_cfg.c */
