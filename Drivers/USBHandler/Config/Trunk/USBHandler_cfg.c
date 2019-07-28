/******************************************************************************
 * @file USBHandler_cfg.c
 *
 * @brief USB handler configuration implementation 
 *
 * This file provides the implementation for the configuration of the USB
 * handler
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
 * \addtogroup USBHandler
 * @{
 *****************************************************************************/

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "USBHandler/USBHandler_cfg.h"

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------
/// define the number of configurations
#define NUMBER_OF_CONFIGS                   ( 1 )

/// define the endpoints for the CDC device
#define CDC_NOTIFICATION_EPADDR             ( USB_DIRECTION_IN | 1 )
#define CDC_TX_EPADDR                       ( USB_DIRECTION_IN | 2 )
#define CDC_RX_EPADDR                       ( USB_DIRECTION_OUT | 3 )

/// define the size fo the endoints
#define CDC_NOTIFICATION_EPSIZE             ( 8 )
#define CDC_TXRX_SIZE                       ( 16 )

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations ----------------------------------------------

// local parameter declarations -----------------------------------------------

// local function prototypes --------------------------------------------------

// constant parameter initializations -----------------------------------------
// instantiate the device descriptor
const USBDEVDESC g_tDeviceDescriptor
{
  .tHeader            =
  {
    .nLength          = USBDEVDESC_SIZE,
    .nType            = USB_DESCTYPES_DEVICE
  },
  .wBcdUSB            = USB_VERSION_BCD( 1, 1, 0 ),
  .nDeviceClass       = USB_DESCCSCP_IADDEVICECLASS, 
  .nDeviceSubClass    = USB_DESCCSCP_IADDEVICESUBCLASS,
  .nDeviceProtocol    = USB_DESCCSCP_IADDEVICEPROTOCOL
  .nMaxPacketSize0    = USB_EP0_SIZE,
  .wVendor            = 0x03EB,
  .wProduct           = 0x2200,
  .wBcdDevice         = USB_VERSION_BCD( 0, 0, 1 ),
  .nManufacturer      = STRING_ID_MANUFACUTRER,
  .nProduct           = STRING_ID_PRODUCT,
  .nSerialNumber      = USB_USEINTERNAL_SERIALNUMBER,
  .nNumConfigurations = NUMBER_OF_CONFIGS
};

// instantiate the configuration descriptor
const USBCONFDESC g_tConfigDescriptor
{
  .tConfig                    =
  {
    .tHeader                  =
    {
      .nLength                = USBDESCHEADER,
      .nType                  = USB_DESCTYPES_CONFIGURATION
    },
    .wTotalLength             = USBCONFDESC_SIZE,
    .nNumInterfaces           = 2,
    .nConfigurationValue      = 1,
    .nConfiguration           = USB_NO_DESCRIPTOR,
    .nAttributes              = ( USB_CONFIG_ATTR_RESERVED | USB_CONFIG_ATTR_SELFPOWERED ),
    .nMaxPower                = USB_CONFIG_POWER_MA( 100 )
  },
  
  .tCdcIAd                    =
  {
    .tHeader                  =
    {
      .nLength                = USBINTFASSCDESC_SIZE,
      .nType                  = USB_DESCTYPES_INTERFACEASSOCIATION 
    },
    .nFirstInterface          = INTERFACEID_CDCCCI,
    .nInterfaceCount          = 2,
    .nFunctionClass           = USB_CDCDESCCSCP_CDCCLASS,
    .nFunctionSubClass        = USB_CDCDESCCSCP_ACMSUBCLASS,
    .nFunctionProtocol        = USB_CDCDESCCSCP_ATCOMMANDPROTOCOL, 
    .nFunction                = USB_NO_DESCRIPTOR
 },
  
  .tCdcCcdInterface           =
  {
    .tHeader                  =
    {
      .nLength                = USBCONFDESC_SIZE,
      .nType                  = USB_DESCTYPES_INTERFACE
    },
    .nInterfaceNumber         = INTERFACE_CDCCCI, 
    .nAlternateSetting        = 0,
    .nNumEndpoints            = 1,
    .nInterfaceClass          = USB_CDCDESCCSCP_CDCCLASS,
    .nInterfaceSubClass       = USB_CDCDESCCSCP_ACMSUBCLASS,
    .nInterfaceProtocol       = USB_CDCDESCCSCP_ATCOMMANDPROTOCOL,
    .nInterface               = USB_NO_DESCRIPTOR
  },
  
  .tCdcFuncionHeader          =
  {
    .tHeader                  =
    {
      .nLength                = USBCDFUNCCDESC_SIZE,
      .nType                  = USB_DESCTYPES_INTERFACE
    },
    .nSubType                 = USB_CDCDSUBTYPES_CSINTF_HEADER,
    .wCdcSpecification        = USB_VERSION_BCD( 1, 1, 0 )
  },
  
  .tCdcFunctionAcm            =
  {
    .tHeader                  =
    {
      .nLength                = USBCDCFUNCACMDESC_SIZE,
      .nType                  = USB_DESCTYPES_INTERFACE
    },
    .nSubType                 = USB_CDCDSUBTYPES_CSINTF_ACM,
    .nCapabilities            = 0x06
  },
  
  .tCdcFunctionUnion          =
  {
    .tHeader                  =
    {
      .nLength                = USBCDCFUNCUNIONDESC_SIZE,
      .nType                  = USB_DESCTYPES_INTERFACE
    },
    .nSubType                 = USB_CDCDSUBTYPES_CSINTF_UNION,
    .nMasterInterfaceNumber   = INTERFACE_ID_CDCCCI,
    .nSlaveInterfaceNumber    = INTERFACE_ID_CDCDCI
  },
  
  .tCdcNotificationEndpoint   =
  {
    .tHeader                  =
    {
      .nLength                = USBENDDESCR_SIZE,
      .nType                  = USB_DESCTYPES_ENDPOINT
    },
    .nEndpointAddress         = CDC_NOTIFICATION_EPADDR,
    .nAttributes              = ( USB_EPTYPE_INTERRUPT | USB_EPATTRIB_NOSYNC | USB_EPUSAGE_DATA )
    .wMaxPacketSize           = CDC_NOTIFICATION_EPSIZE,
    .nInterval                = 0xFF
  },
  
  .tCdcDciInterface           =
  {
    .tHeader                  =
    {
      .nLength                = USBCONFDESC_SIZE,
      .nType                  = USB_DESCTYPES_INTERFACE
    },
    .nInterfaceNumber         = INTERFACE_ID_CDCDCI, 
    .nAlternateSetting        = 0,
    .nNumEndpoints            = 2,
    .nInterfaceClass          = USB_CDCDESCCSCP_CDCDATACLASS,
    .nInterfaceSubClass       = USB_CDCDESCCSCP_NODATASUBCLASS,
    .nInterfaceProtocol       = USB_CDCDESCCSCP_NODATAPROTOCOL,
    .nInterface               = USB_NO_DESCRIPTOR
  },
  
  .tCdcDataOutEndpoint        =
  {
    .tHeader                  =
    {
      .nLength                = USBENDDESCR_SIZE,
      .nType                  = USB_DESCTYPES_ENDPOINT
    },
    .nEndpointAddress         = CDC_RX_EPADDR,
    .nAttributes              = ( USB_EPTYPE_BULK | USB_EPATTRIB_NOSYNC | USB_EPUSAGE_DATA )
    .wMaxPacketSize           = CDC_TXRX_EPSIZE,
    .nInterval                = 0x05
  },
  
  .tCdcDataInEndpoint         =
  {
    .tHeader                  =
    {
      .nLength                = USBENDDESCR_SIZE,
      .nType                  = USB_DESCTYPES_ENDPOINT
    },
    .nEndpointAddress         = CDC_TX_EPADDR,
    .nAttributes              = ( USB_EPTYPE_BULK | USB_EPATTRIB_NOSYNC | USB_EPUSAGE_DATA )
    .wMaxPacketSize           = CDC_TXRX_EPSIZE,
    .nInterval                = 0x05
  }
};

// instantiate the language string
const USBSTRDESC g_tLanguageString = USB_STRING_DESCRIPTOR( "0409" );

// instantiate the manufacturer string
const USBSTRDESC g_tManufacuterString = USB_STRING_DESCRIPTOR( "Cyber Integration" );

// instantiate the product string
const USBSTRDESC g_tProductString = USB_STRING_DESCFIPTOR( "USB Master" );

/******************************************************************************
 * @function USBHandler_SetConfigurtionCB
 *
 * @brief set configuration call back
 *
 * This function will determine if this is a special configuration and return
 * TRUE if handled, FALSE otherwise
 *
 * @param[in] wConfig     configuration value   
 *
 * @return      TRUE if handled, FALSE if not
 *
 *****************************************************************************/
BOOL USBHandler_SetConfigurtionCB( U16 wConfig )
{
  BOOL bHandled = FALSE;
  
  // return the handled flag
  return( bHandled );
}

/******************************************************************************
 * @function USBHandler_SetInterfaceCB
 *
 * @brief set interface call back
 *
 * This function will determine if this is a special interface and return
 * TRUE if handled, FALSE otherwise
 *
 * @param[in] wInterface     interface value   
 *
 * @return      TRUE if handled, FALSE if not
 *
 *****************************************************************************/
BOOL USBHandler_SetInterfaceCB( U16 wInterface )
{
  BOOL bHandled = FALSE;
  
  // return the handled flag
  return( bHandled );
}

/******************************************************************************
 * @function USBHandler_ControlOutCB
 *
 * @brief locall control out callback
 *
 * This function will perform any aditional control out
 *
 *****************************************************************************/
void USBHandler_ControlOutCB( void )
{
}

/******************************************************************************
 * @function USBHandler_ControlInCB
 *
 * @brief locall control in callback
 *
 * This function will perform any aditional control out
 *
 *****************************************************************************/
void USBHandler_ControlInCB( void )
{
}

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
 
/**@} EOF USBHandler_cfg.c */
