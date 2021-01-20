/******************************************************************************
 * @file USBCDCHandler_cfg.c
 *
 * @brief USB-CDC handler configuration implementation
 *
 * This file provides the configuration implementation for the USB-CDC handler
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

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "USBCDCHandler/USBCDCHandler_cfg.h"

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------
/// enumerate the DCD interface descriptors
typedef enum _INTFDESCENUM
{
  INTFDESC_ENUM_CDCCCI = 0,
  INTFDESC_ENUM_CDCDCI,
} INTFDESCENUM;

/// enumerate the standard request types

// structures -----------------------------------------------------------------

// global parameter declarations ----------------------------------------------

// local parameter declarations -----------------------------------------------

// local function prototypes --------------------------------------------------

// constant parameter initializations -----------------------------------------
const ALIGNED4 USBDEVDESCRIPTOR g_tUsbDevDescriptor =
{
  .tHeader                =
  {
    .nLength              = USBDEVDESCRIPTOR_SIZE,
    .nDescriptorType      = USB_DESCTYPES_DEVICE,
  },
  .wBcdUSB                = USB_VERSION_BCD( 1, 1, 0 ),
  .nDeviceClass           = USB_CDCDESCCSCP_CDCCLASS,
  .nDeviceSubClass        = USB_CDCDESCCSCP_NOSPECIFICSUBCLASS,
  .nDeviceProtocol        = USB_CDCDESCCSCP_NOSPECIFICPROTOCOL,
  .nMaxPacketSize         = USB_ENDPOINT_SIZE,
  .wVendor                = 0x03EB,
  .wProduct               = 0x2044,
  .wBcdDevice             = USB_VERSION_BCD( 0, 0, 1 ),
  .nManufacturer          = 0,
  .nProduct               = 0,
  .nSerialNumber          = 0,
  .nNumConfigurations     = 1,
};

const ALIGNED4 USBCONFIGURATION g_tUsbConfiguration =
{
  // configuration descriptor
  .tCfgDescriptor         =
  {
    .tHeader              =
    {
      .nLength            = USBINTERFACEDESCRIPTOR_SIZE,
      .nDescriptorType    = USB_DESCTYPES_CONFIGURATION,
    },
    .wTotalLength         = USBCONFIGURATION_SIZE,
    .nNumInterfaces       = 2,
    .nConfigurationValue  = 1,
    .nConfiguration       = 0,
    .nAttributes          = 0xA0,
    .nMaxPower            = USB_CONFIG_POWER_MA( 250 ),
  },


  // Interface descriptor
  .tCdcCCIInterface       =
  {
    .tHeader              =
    {
      .nLength            = USBINTERFACEDESCRIPTOR_SIZE,
      .nDescriptorType    = USB_DESCTYPES_INTERFACE,
    },
    .nInterfaceNumber     = INTFDESC_ENUM_CDCCCI,
    .nAlternateSetting    = 0,
    .nNumEndpoints        = 1,
    .nInterfaceClass      = USB_CDCDESCCSCP_CDCCLASS,
    .nInterfaceSubClass   = USB_CDCDESCCSCP_ACMSUBCLASS,
    .nInterfaceProtocol   = USB_CDCDESCCSCP_ATCOMMANDPROTOCOL,
    .nInterface           = 0,
  },
  
  // function header
  .tCdcFuncHeader         =
  {
    .tHeader              =
    {
      .nLength            = USBHDRFUNCDESCIPTOR_SIZE,
      .nDescriptorType    = USB_DESCTYPES_CSINTERFACE,
    },
    .nDescriptorSubType   = USB_CDCDSUBTYPES_CSINTF_HEADER,
    .wBcdCDC              = USB_VERSION_BCD( 1, 1, 0 ),
  },

  // function ACM
  .tCdcFuncAcm            =
  {
    .tHeader              =
    {
      .nLength            = USBACMDESCRIPTOR_SIZE,
      .nDescriptorType    = USB_DESCTYPES_CSINTERFACE,
    },
    .nDescriptorSubType   = USB_CDCDSUBTYPES_CSINTF_ACM,
    .nCapabilities        = 0x06,
  },

  // function union
  .tCdcFuncUnion          =
  {
    .tHeader              =
    {
      .nLength            = USBUNIONFUNCDESCRIPTOR_SIZE,
      .nDescriptorType    = USB_DESCTYPES_CSINTERFACE,
    },
    .nDescriptorSubType   = USB_CDCDSUBTYPES_CSINTF_UNION,
    .nMasterInterface     = INTFDESC_ENUM_CDCCCI, 
    .nSlaveInterface      = INTFDESC_ENUM_CDCDCI,
  },

  // control endpoint
  .tCdcControlEndpoint    =
  {
    .tHeader              =
    {
      .nLength            = USBENDPOINTDESCRIPTOR_SIZE,
      .nDescriptorType    = USB_DESCTYPES_ENDPOINT,
    },
    .nEndpointAddress     = USB_ENDPOINT_COMM | USB_DIR_IN,
    .nAttributes          = USB_EPTYPE_INTERRUPT,
    .wMaxPacketSize       = USB_ENDPOINT_SIZE,
    .nInterval            = 0xFF,
  },

  // CDC Data Interface
  .tCdcDCIInterface       =
  {
    .tHeader              =
    {
      .nLength            = USBINTERFACEDESCRIPTOR_SIZE,
      .nDescriptorType    = USB_DESCTYPES_INTERFACE,
    },
    .nInterfaceNumber     = INTFDESC_ENUM_CDCDCI,
    .nAlternateSetting    = 0,
    .nNumEndpoints        = 2,
    .nInterfaceClass      = USB_CDCDESCCSCP_CDCDATACLASS,
    .nInterfaceSubClass   = USB_CDCDESCCSCP_NODATASUBCLASS,
    .nInterfaceProtocol   = USB_CDCDESCCSCP_NODATAPROTOCOL,
    .nInterface           = 0,
  },

  // data in endpoint CDCTX
  .tCdcDataInEndpoint     =
  {
    .tHeader              =
    {
      .nLength            = USBENDPOINTDESCRIPTOR_SIZE,
      .nDescriptorType    = USB_DESCTYPES_ENDPOINT,
    },
    .nEndpointAddress     = USB_ENDPOINT_IN | USB_DIR_IN,
    .nAttributes          = USB_EPTYPE_BULK,
    .wMaxPacketSize       = USB_ENDPOINT_SIZE,
    .nInterval            = 0,
  },

  // data out endpoint CDCRX
  .tCdcDataOutEndpoint    =
  {
    .tHeader              =
    {
      .nLength            = USBENDPOINTDESCRIPTOR_SIZE,
      .nDescriptorType    = USB_DESCTYPES_ENDPOINT,
    },
    .nEndpointAddress     = USB_ENDPOINT_OUT,
    .nAttributes          = USB_EPTYPE_BULK,
    .wMaxPacketSize       = USB_ENDPOINT_SIZE,
    .nInterval            = 0,
  },
};

/**@} EOF USBCDCHandler_cfg.c */
