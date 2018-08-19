/******************************************************************************
 * @file LUFACDCHandler_cfg.c
 *
 * @brief 
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
 * \addtogroup LUFA
 * @{
 *****************************************************************************/

// system includes ------------------------------------------------------------

// library includes -----------------------------------------------------------

// local includes -------------------------------------------------------------
#include "LUFACDCHandler/LUFACDCHandler_cfg.h"

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations ----------------------------------------------

// local parameter declarations -----------------------------------------------

// local function prototypes --------------------------------------------------

// constant parameter initializations -----------------------------------------
const CODE USB_Descriptor_Device_t		tDeviceDescriptor =
{
  .Header                   = 
  {
    .Size				            = sizeof( USB_Descriptor_Device_t ), 
    .Type				            = DTYPE_Device
  },
  .USBSpecification         = VERSION_BCD(1,1,0),
  .Class                    = USB_CSCP_IADDeviceClass,
  .SubClass                 = USB_CSCP_IADDeviceSubclass,
  .Protocol                 = USB_CSCP_IADDeviceProtocol,
  .Endpoint0Size            = FIXED_CONTROL_ENDPOINT_SIZE,
  .VendorID                 = 0x03EB,
  .ProductID                = 0x2200,
  .ReleaseNumber            = VERSION_BCD(0,0,1),
  .ManufacturerStrIndex     = STRING_ID_MANUFACTURER,
  .ProductStrIndex          = STRING_ID_PRODUCT,
  .SerialNumStrIndex        = USE_INTERNAL_SERIAL,
  .NumberOfConfigurations   = FIXED_NUM_CONFIGURATIONS
};

// define the Configuration descriptor structure
const CODE USB_Descriptor_Configuration_t	tConfigurationDescriptor =
{
  .Config                   =
  {
    .Header					        = 
    {
      .Size				          = sizeof( USB_Descriptor_Configuration_Header_t ), 
      .Type				          = DTYPE_Configuration
    },

    .TotalConfigurationSize = sizeof( USB_Descriptor_Configuration_t ),
    .TotalInterfaces        = 2,
    .ConfigurationNumber    = 1,
    .ConfigurationStrIndex  = NO_DESCRIPTOR,
    .ConfigAttributes       = ( USB_CONFIG_ATTR_RESERVED | USB_CONFIG_ATTR_SELFPOWERED ),
    .MaxPowerConsumption    = USB_CONFIG_POWER_MA( 100 )
  },

  .CDC_IAD                  =
  {
    .Header                 =
    {
      .Size                 = sizeof( USB_Descriptor_Interface_Association_t ),
      .Type                 = DTYPE_InterfaceAssociation
    },
    .FirstInterfaceIndex    = INTERFACE_ID_CDC_CCI,
    .TotalInterfaces        = 2,
    .Class                  = CDC_CSCP_CDCClass,
    .SubClass               = CDC_CSCP_ACMSubclass,
    .Protocol               = CDC_CSCP_ATCommandProtocol,
    .IADStrIndex            = NO_DESCRIPTOR
  },
  
  .CDC_CCI_Interface			  =
  {
    .Header                 = 
    {
      .Size				          = sizeof( USB_Descriptor_Interface_t ), 
      .Type				          = DTYPE_Interface
    },
    .InterfaceNumber        = INTERFACE_ID_CDC_CCI,
    .AlternateSetting       = 0,
    .TotalEndpoints         = 1,
    .Class                  = CDC_CSCP_CDCClass,
    .SubClass               = CDC_CSCP_ACMSubclass,
    .Protocol               = CDC_CSCP_ATCommandProtocol,
    .InterfaceStrIndex      = NO_DESCRIPTOR
  },

  .CDC_Functional_Header		=
  {
    .Header                 = 
    {
      .Size				          = sizeof( USB_CDC_Descriptor_FunctionalHeader_t ), 
      .Type				          = DTYPE_CSInterface
    },
    .Subtype                = CDC_DSUBTYPE_CSInterface_Header,
    .CDCSpecification       = VERSION_BCD( 1,1,0 ),
  },

  .CDC_Functional_ACM			  =
  {
    .Header                 = 
    {
      .Size				          = sizeof( USB_CDC_Descriptor_FunctionalACM_t ), 
      .Type				          = DTYPE_CSInterface
    },
    .Subtype                = CDC_DSUBTYPE_CSInterface_ACM,
    .Capabilities           = 0x06,
  },

  .CDC_Functional_Union		  =
  {
    .Header                 = 
    {
      .Size				          = sizeof( USB_CDC_Descriptor_FunctionalUnion_t ), 
      .Type				          = DTYPE_CSInterface
    },
    .Subtype                = CDC_DSUBTYPE_CSInterface_Union,
    .MasterInterfaceNumber  = INTERFACE_ID_CDC_CCI,
    .SlaveInterfaceNumber   = INTERFACE_ID_CDC_DCI,
  },

  .CDC_NotificationEndpoint	=
  {
    .Header                 = 
    {
      .Size				          = sizeof( USB_Descriptor_Endpoint_t ), 
      .Type				          = DTYPE_Endpoint
    },
    .EndpointAddress		    = CDC_NOTIFICATION_EPADDR,
    .Attributes				      = ( EP_TYPE_INTERRUPT | ENDPOINT_ATTR_NO_SYNC | ENDPOINT_USAGE_DATA ),
    .EndpointSize			      = CDC_NOTIFICATION_EPSIZE,
    .PollingIntervalMS      = 0xFF
  },

  .CDC_DCI_Interface			  =
  {
    .Header                 = 
    {
      .Size				          = sizeof( USB_Descriptor_Interface_t ), 
      .Type				          = DTYPE_Interface
    },
    .InterfaceNumber        = INTERFACE_ID_CDC_DCI,
    .AlternateSetting       = 0,
    .TotalEndpoints         = 2,
    .Class                  = CDC_CSCP_CDCDataClass,
    .SubClass               = CDC_CSCP_NoDataSubclass,
    .Protocol               = CDC_CSCP_NoDataProtocol,
    .InterfaceStrIndex      = NO_DESCRIPTOR
  },

  .CDC_DataOutEndpoint		  =
  {
    .Header                 = 
    {
      .Size				          = sizeof( USB_Descriptor_Endpoint_t ), 
      .Type				          = DTYPE_Endpoint
    },
    .EndpointAddress        = CDC_RX_EPADDR,
    .Attributes             = ( EP_TYPE_BULK | ENDPOINT_ATTR_NO_SYNC | ENDPOINT_USAGE_DATA ),
    .EndpointSize           = CDC_TXRX_EPSIZE,
    .PollingIntervalMS      = 0x05
  },

  .CDC_DataInEndpoint       =
  {
    .Header                 = 
    {
      .Size				          = sizeof( USB_Descriptor_Endpoint_t ), 
      .Type				          = DTYPE_Endpoint
    },
    .EndpointAddress        = CDC_TX_EPADDR,
    .Attributes             = ( EP_TYPE_BULK | ENDPOINT_ATTR_NO_SYNC | ENDPOINT_USAGE_DATA ),
    .EndpointSize           = CDC_TXRX_EPSIZE,
    .PollingIntervalMS      = 0x05
  },
};

// define the language descriptor structure
const CODE USB_Descriptor_String_t			tLanguageString = USB_STRING_DESCRIPTOR_ARRAY( LANGUAGE_ID_ENG );

// define the manufacturer descriptor string
const CODE USB_Descriptor_String_t			tManufacturerString = USB_STRING_DESCRIPTOR( L"Nan Technologies" );

// define the product descriptor string
const CODE USB_Descriptor_String_t			tProductString = USB_STRING_DESCRIPTOR( L"DALI-DMX Master" );

/**@} EOF LUFACDCHandler_cfg.c */
