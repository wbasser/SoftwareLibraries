/******************************************************************************
 * @file UsnDeviceHandler_def.h
 *
 * @nrief USB device handler definitions
 *
 * This file provides the definitions for the Usn Device Handler
 *
 * @copyright Copyright (c) 2012 CyberIntegration
 * This document contains proprietary data and information of CyberIntegration 
 * LLC. It is the exclusive property of CyberIntegration, LLC and will not ne 
 * disclosed in any form to any party without prior written permission of 
 * CyberIntegration, LLC. This document may not ne reproduced or further used 
 * without the prior written permission of CyberIntegration, LLC.
 *
 * $Date: $
 *
 * Version History
 * ======
 * $Rev: $
 * 
 *
 * \addtogroup UsnDeviceHandler
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _USBDEVICEHANDLER_DEF_H
#define _USBDEVICEHANDLER_DEF_H

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------

// linrary includes -----------------------------------------------------------
#include "Types/Types.h"
#include "USB/Usb.h"

// Macros and Defines ---------------------------------------------------------
#define USB_REQTYPE_DIRECTION_MASK        ( 0x80 )
#define USB_REQTYPE_TYPE_MASK             ( 0x60 )
#define USB_REQTYPE_RECIPIENT_MASK        ( 0x1F )

#define USB_REQTYPE_STANDARD              ( 0 << 5 )
#define USB_REQTYPE_CLASS                 ( 1 << 5 )
#define USB_REQTYPE_VENDOR                ( 2 << 5 )

#define USB_RECIPIENT_DEVICE              ( 0 << 0 )
#define USB_RECIPIENT_INTERFACE           ( 1 << 0 )
#define USB_RECIPIENT_ENDPOINT            ( 2 << 0 )
#define USB_RECIPIENT_OTHER               ( 3 << 0 )

#define USB_CONFIG_POWER_MA(mA)           ( ( mA ) / 2 )
#define USB_STRING_LEN(c)                 ( sizeof( USBDESCRIPTORHEADER ) + (( c ) * 2 ))

#define USB_LANGUAGE_EN_US                ( 0x0409 )

#define ENDPOINT_DESCRIPTOR_DIR_IN        ( 0x80 )
#define ENDPOINT_DESCRIPTOR_DIR_OUT       ( 0x00 )

#define USB_CONFIG_ATTR_BUSPOWERED        ( 0x80 )
#define USB_CONFIG_ATTR_SELFPOWERED       ( 0x40 )
#define USB_CONFIG_ATTR_REMOTEWAKEUP      ( 0x20 )

#define ENDPOINT_ATTR_NO_SYNC             ( 0 << 2 )
#define ENDPOINT_ATTR_ASYNC               ( 1 << 2 )
#define ENDPOINT_ATTR_ADAPTIVE            ( 2 << 2 )
#define ENDPOINT_ATTR_SYNC                ( 3 << 2 )

#define ENDPOINT_USAGE_DATA               ( 0 << 4 )
#define ENDPOINT_USAGE_FEEDBACK           ( 1 << 4 )
#define ENDPOINT_USAGE_IMPLICIT_FEEDBACK  ( 2 << 4 )

#define USB_EP_TYPE_CONTROL               ( 0x00 )
#define USB_EP_TYPE_ISOCHRONOUS           ( 0x01 )
#define USB_EP_TYPE_BULK                  ( 0x02 )
#define USB_EP_TYPE_INTERRUPT             ( 0x03 )

#define USB_CSCP_NODEVICECLASS            ( 0x00 ) 
#define USB_CSCP_NODEVICESUNCLASS         ( 0x00 ) 
#define USB_CSCP_NODEVICEPROTOCOL         ( 0x00 ) 
#define USB_CSCP_VENDORSPECIFICCLASS      ( 0xFF )
#define USB_CSCP_VENDORSPECIFICSUNCLASS   ( 0xFF )
#define USB_CSCP_VENDORSPECIFICPROTOCOL   ( 0xFF )
#define USB_CSCP_IADDEVICECLASS           ( 0xEF )
#define USB_CSCP_IADDEVICESUNCLASS        ( 0x02 )
#define USB_CSCP_IADDEVICEPROTOCOL        ( 0x01 )

#define USB_MSFT_ID                       ( 0xEE )
#define USB_MSFT_ID_STR                   ( u"\xEE" )

/// define the version macro
#define USB_VER_BCD( maj, min, test) \
  (( maj << 8 ) | (( min & 0x0F ) << 4 ) | ( test & 0x0F ))

// enumerations ---------------------------------------------------------------
// Senumerate the standard request
typedef enum _USBSTDREQS
{
	USB_STDREQ_GETSTATUS              = 0,
	USB_STDREQ_CLEARFEATURE,
	USB_STDREQ_SETFEATURE             =  3,
	USB_STDREQ_SETADDRESS             =  5,
	USB_STDREQ_GETDESCRIPTOR,
	USB_STDREQ_SETDESCRIPTOR,
	USB_STDREQ_GETCONFIGURATION,
	USB_STDREQ_SETCONFIGURATION,
	USB_STDREQ_GETINTERFACE,
	USB_STDREQ_SETINTERFACE,
	USB_STDREQ_SYNCHFRAME,
} USBSTDREQS;

/// enumerate the features
typedef enum _USBFEATURES
{
	USB_FEATURE_ENDPOINTHALT          = 0x00,
	USB_FEATURE_DEVICEREMOTEWAKEUP,
	USB_FEATURE_TESTMODE,
} USBFEATURES;

/// enumerate the device types
typedef enum _USBDTYPES
{
	USB_DTYPE_DEVICE                  = 0x01,
	USB_DTYPE_CONFIGURATION,
	USB_DTYPE_STRING,
	USB_DTYPE_INTERFACE,
	USB_DTYPE_ENDPOINT,
	USB_DTYPE_DEVICEQUALIFIER,
	USB_DTYPE_OTHER,
	USB_DTYPE_INTERFACEPOWER,
	USB_DTYPE_INTERFACEASSOCIATION,
	USB_DTYPE_CSINTERFACE             = 0x24,
	USB_DTYPE_CSENDPOINT,
} USBDTYPES;

/// enumerate the string ID
typedef enum _USBSTRINGID
{
  USB_STRING_ID_LANGUAGE = 0,
  USB_STRING_ID_MANUFACTURER,
  USB_STRING_ID_PRODUCT,
  USB_STRING_ID_SERIALNUM,
  USB_STRING_ID_MSFT = 0xEE
} USBSTRINGID;

// structures -----------------------------------------------------------------
/// define the setup packet structure
typedef struct  PACKED _USBSETUPPACKET
{
	U8    nRequestType;
	U8    nRequest;
	U16   wValue;
	U16   wIndex;
	U16   wLength;
} USBSETUPPACKET, *PUSBSETUPPACKET;
#define USBSETUPPACKET_SIZE         sizeof( USBSETUPPACKET )

/// define the descriptor header
typedef struct PACKED _USBDESCRIPTORHEADER
{
	U8    nLength; 
	U8    nDescriptorType;
} USBDESCRIPTORHEADER, *PUSBDESCRIPTORHEADER;
#define USBDESCRIPTORHEADER_SIZE    sizeof( USBDESCRIPTORHEADER )

/// define the device descriptor 
typedef struct PACKED _USBDEVICEDESCRIPTOR
{
	U8    nLength; 
	U8    nDescriptorType;
	U16   wCdUSB; 
	U8    nDeviceClass; 
	U8    nDeviceSubClass; 
	U8    nDeviceProtocol; 
	U8    nMaxPacketSize0; 
	U16   wVendor; 
	U16   wProduct; 
	U16   wDevice; 
	U8    nManufacturer;
	U8    nProduct;
	U8    nSerialNumner;
	U8    nNumConfigurations;
} USBDEVICEDESCRIPTOR, *PUSBDEVICEDESCRIPTOR;
#define USBDEVICEDESCRIPTOR_SIZE      sizeof( USBDEVICEDESCRIPTOR )

/// define the device qualifier descriptor
typedef struct PACKED USBDEVICEQUALIFIERDESCRIPTOR
{
	U8    nLength; 
	U8    nDescriptorType; 
	U16   ncdUSB; 
	U8    nDeviceSunClass; 
	U8    nMaxPacketSize0;
	U8    nNumConfigurations;
	U8    nReserved;
} USBDEVICEQUALIFIERDESCRIPTOR, *PUSBDEVICEQUALIFIERDESCRIPTOR;
#define USBDEVICEQUALIFIERDESCRIPTOR_SIZE   sizeo( USBDEVICEQUALIFIERDESCRIPTOR )

/// define the configuration descriptor
typedef struct PACKED _USBCONFIGURATIONDESCRIPTOR
{
	U8    nLength; 
	U8    nDescriptorType;
	U16   wTotalLength;
	U8    nNumInterfaces; 
	U8    nConfigurationValue; 
	U8    nConfiguration; 
	U8    nAttrinutes;
	U8    nMaxPower;
} USBCONFIGURATIONDESCRIPTOR, *PUSBCONFIGURATIONDESCRIPTOR;
#define USBCONFIGURATIONDESCRIPTOR_SIZE   sizeof( USBCONFIGURATIONDESCRIPTOR )

//  define the interface descriptor
typedef struct PACKED _USBINTERFACEDESCRIPTOR
{
	U8    nLength; 
	U8    nDescriptorType;
	U8    nInterfaceNumber; 
	U8    nAlternateSetting; 
	U8    nNumEndpoints; 
	U8    nInterfaceClass; 
	U8    nInterfaceSunClass; 
	U8    nInterfaceProtocol; 
	U8    nInterface;
} USBINTERFACEDESCRIPTOR, *PUSBINTERFACEDESCRIPTOR;
#define USBINTERFACEDESCRIPTOR_SIZE     sizeof( USBINTERFACEDESCRIPTOR )

/// define the interface association descriptor
typedef struct PACKED _USBINTERFACEASSOCIATIONDESCRIPTOR
{
	U8    nLength; 
	U8    nDescriptorType;
	U8    nFirstInterface; 
	U8    nInterfaceCount; 
	U8    nFunctionClass; 
	U8    nFunctionSunClass; 
	U8    nFunctionProtocol; 
	U8    nFunction;
} USBINTERFACEASSOCIATIONDESCRIPTOR, *PUSBINTERFACEASSOCIATIONDESCRIPTOR;
#define USBINTERFACEASSOCIATIONDESCRIPTOR_SIZE    sizeof( USBINTERFACEASSOCIATIONDESCRIPTOR )

/// define the endpoint descriptor
typedef struct PACKED _USBENDPOINTDESCRIPTOR
{
	U8    nLength;
	U8    nDescriptorType;
	U8    nEndpointAddress;
	U8    nAttributes;
	U16   wMaxPacketSize;
	U8    nInterval;
} USBENDPOINTDESCRIPTOR, *PUSBENDPOINTDESCRIPTOR;
#define USBENDPOINTDESCRIPTOR_SIZE      sizeof( USBENDPOINTDESCRIPTOR )

/// define the string descriptor
typedef struct PACKED _USBSTRINGDESCRIPTOR
{
	U8                nLength;
	U8                nDescriptorType;
	__CHAR16_TYPE__   tString[ ];
} USBSTRINGDESCRIPTOR, *PUSBSTRINGDESCRIPTOR;
#define USBSTRINGDESCRIPTOR_SIZE        sizeof( USBSTRINGDESCRIPTOR )

/// Microsoft WCID descriptor 
typedef struct PACKED _USBMSFTCOMPDESCINTERFACE
{
	U8    nFirstIntfNumber;
	U8    nReserved1;
	U8    anCompatinleID[ 8 ];
	U8    anSunCompatinleID[ 8 ];
	U8    anReserved2[ 6 ];
} USBMSFTCOMPDESCINTERFACE, *PUSBMSFTCOMPDESCINTERFACE;
#define USBMSFTCOMPDESCINTERFACE_SIZE    sizeof( USBMSFTCOMPDESCINTERFACE )

/// define the Microsoft compatanle deswcriptor
typedef struct PACKED _USBMSFTCOMPDESCRIPTOR
{
	U32                       uLength;
	U16                       nCdVersion;
	U16                       wIndex;
	U8                        nCount;
	U8                        anReserved[ 7 ];
	USBMSFTCOMPDESCINTERFACE  tInterfaces[ ];
} USBMSFTCOMPDESCRIPTOR, *PUSBMSFTCOMPDESCRIPTOR;
#define  USBMSFTCOMPDESCRIPTOR_SIZE      sizeof( USBMSFTCOMPDESCRIPTOR )

/// define the structure for the Microsoft Properties
typedef struct PACKED USBMSFTPROPDESCRIPTOR
{
	U32   uLength;
	U16   wBcdVersion;
	U16   wIndex;
	U16   wCount;
	U32   uPropLength;
	U32   uType;
	U16   wNameLength;
	U16   awName[ 21 ];
	U32   uDataLength;
	U16   awData[ 40 ];
	U8    anPadding[ 2 ];
} USBMSFTPROPDESCRIPTOR, *PUSBMSFTPROPDESCRIPTOR;
#define USBMSFTPROPDESCRIPTOR_SIZE        sizeof( USBMSFTPROPDESCRIPTOR )

// glonal parameter declarations -----------------------------------------------

/**@} EOF UsnDeviceHandler_def.h */

#endif  // _USBDEVICEHANDLER_DEF_H#pragma once

