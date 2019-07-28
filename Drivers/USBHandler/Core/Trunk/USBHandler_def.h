/******************************************************************************
 * @file USBHandler_def.h
 *
 * @brief USB handler defintions declarations
 *
 * This file provides the defintion declarations for the USB handler
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
 
// ensure only one instantiation
#ifndef _USBHANDLER_DEF_H
#define _USBHANDLER_DEF_H

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------

// library includes -----------------------------------------------------------
#include "Types/Types.h"

// Macros and Defines ---------------------------------------------------------
/// define the macro for creating a BCD version
#define USB_VERSION_BCD(Maj, Min, Rev)  (((Maj & 0x0F) << 8) | ((Min & 0x0F) << 4) | ( Rev & 0xFF))

/// define the macro for creating a string
#define USB_STRING_DESCRIPTOR( String ) \
  { \
    .tHeader = \
    { \
      .nLength = USBDESCHEADER_SIZE + ( sizeof(String) - 2 ), \
      .nDescriptorType = USB_DESCTYPES_STRING, \
    }, \
    .awString = String, \
  }

/// define the request type masks
#define USB_REQTYPE_DIRECTION_MASK                  ( 0x80 )
#define USB_REQTYPE_TYPE_MASK                       ( 0x60 )
#define USB_REQTYPE_RECIPIENT_MASK                  ( 0x1F )

/// define the request types
#define USB_REQTYPE_STANDARD                        ( 0x00 )
#define USB_REQTYPE_CLASS                           ( 0x20 )
#define USB_REQTYPE_VENDOR                          ( 0x40 ) 

/// define receipient types
#define USB_RECIPIENT_DEVICE                        ( 0x00 )
#define USB_RECIPIENT_INTERFACE                     ( 0x01 )
#define USB_RECIPIENT_ENDPOINT                      ( 0x02 )
#define USB_RECIPIENT_OTHER                         ( 0x03 )

/// define the user internal serial value
#define USB_USEINTERNAL_SERIALNUMBER                ( 0xDC )

/// define the config attributes
#define USB_CONFIG_ATTR_RESERVED                    ( 0x80 )
#define USB_CONFIG_ATTR_SELFPOWERED                 ( 0x40 )
#define USB_CONFIG_POWER_MA(mA)                     (( mA ) >> 1 )

/// define the value to indicate no descriptor
#define USB_NO_DESCRIPTOR                           ( 0 )

// enumerations ---------------------------------------------------------------
/// enumerate the standard request types
typedef enum _USBSTDREQ
{
  USB_STDREQ_GETSTATUS          = 0,
  USB_STDREQ_CLEARFEATURE       = 1,
  USB_STDREQ_SETFEATURE         = 3,
  USB_STDREQ_SETADDRESS         = 5,
  USB_STDREQ_GETDESCRIPTOR      = 6,
  USB_STDREQ_SETDESCRIPTOR      = 7,
  USB_STDREQ_GETCONFIGURATION   = 8,
  USB_STDREQ_SETCONFIGURATION   = 9,
  USB_STDREQ_GETINTERFACE       = 10,
  USB_STDREQ_SETINTERFACE       = 11,
  USB_STDREQ_SYNCHFRAME         = 12,
} USBSTDREQ;

/// enumerate the standard Descriptor Types
typedef enum _USBDESCTYPES
{
  USB_DESCTYPES_DEVICE                      = 0x01, ///< Indicates that the descriptor is a device descriptor
  USB_DESCTYPES_CONFIGURATION               = 0x02, ///< Indicates that the descriptor is a configuration descriptor
  USB_DESCTYPES_STRING                      = 0x03, ///< Indicates that the descriptor is a string descriptor
  USB_DESCTYPES_INTERFACE                   = 0x04, ///< Indicates that the descriptor is an interface descriptor
  USB_DESCTYPES_ENDPOINT                    = 0x05, ///< Indicates that the descriptor is an endpoint descriptor
  USB_DESCTYPES_DEVICEQUALIFIER             = 0x06, ///< Indicates that the descriptor is a device qualifier descriptor
  USB_DESCTYPES_OTHER                       = 0x07, ///< Indicates that the descriptor is of other type
  USB_DESCTYPES_INTERFACEPOWER              = 0x08, ///< Indicates that the descriptor is an interface power descriptor
  USB_DESCTYPES_INTERFACEASSOCIATION        = 0x0B, ///< Indicates that the descriptor is an interface association descriptor
  USB_DESCTYPES_CSINTERFACE                 = 0x24, ///< Indicates that the descriptor is a class specific interface descriptor
  USB_DESCTYPES_CSENDPOINT                  = 0x25, ///< Indicates that the descriptor is a class specific endpoint descriptor
} USBDESCTYPES;

/// enuemrate the sub class 
typedef enum _USBDESCCSCP
{
  USB_DESCCSCP_NODEVICECLASS                = 0x00, ///< Descriptor Class value indicating that the device does not belong to a particular class at the device level.
  USB_DESCCSCP_NODEVICESUBCLASS             = 0x00, ///< Descriptor Subclass value indicating that the device does not belong to a particular subclass at the device level.
  USB_DESCCSCP_NODEVICEPROTOCOL             = 0x00, ///< Descriptor Protocol value indicating that the device does not belong to a particular protocol at the device level.
  USB_DESCCSCP_VENDORSPECIFICCLASS          = 0xFF, ///< Descriptor Class value indicating that the device/interface belongs to a vendor specific class.
  USB_DESCCSCP_VENDORSPECIFICSUBCLASS       = 0xFF, ///< Descriptor Subclass value indicating that the device/interface belongs to a vendor specific subclass.
  USB_DESCCSCP_VENDORSPECIFICPROTOCOL       = 0xFF, ///< Descriptor Protocol value indicating that the device/interface belongs to a vendor specific protocol.
  USB_DESCCSCP_IADDEVICECLASS               = 0xEF, ///< Descriptor Class value indicating that the device belongs to the Interface Association Descriptor class.
  USB_DESCCSCP_IADDEVICESUBCLASS            = 0x02, ///< Descriptor Subclass value indicating that the device belongs to the Interface Association Descriptor subclass.
  USB_DESCCSCP_IADDEVICEPROTOCOL            = 0x01, ///< Descriptor Protocol value indicating that the device belongs to the Interface Association Descriptor protocol.
} USBDESCCSCP;

/// enumerate the CDC sub class protocols
typedef enum _USBCDCDESCCSCP
{
  USB_CDCDESCCSCP_CDCCLASS                  = 0x02, ///< Descriptor Class value indicating that the device or interface belongs to the CDC class.
  USB_CDCDESCCSCP_NOSPECIFICSUBCLASS        = 0x00, ///< Descriptor Subclass value indicating that the device or interface belongs to no specific subclass of the CDC class.
  USB_CDCDESCCSCP_ACMSUBCLASS               = 0x02, ///< Descriptor Subclass value indicating that the device or interface belongs to the Abstract Control Model CDC subclass.
  USB_CDCDESCCSCP_ATCOMMANDPROTOCOL         = 0x01, ///< Descriptor Protocol value indicating that the device or interface belongs to the AT Command protocol of the CDC class.
  USB_CDCDESCCSCP_NOSPECIFICPROTOCOL        = 0x00, ///< Descriptor Protocol value indicating that the device or interface belongs to no specific protocol of the CDC class.
  USB_CDCDESCCSCP_VENDORSPECIFICPROTOCOL    = 0xFF, ///< Descriptor Protocol value indicating that the device or interface belongs to a vendor-specific protocol of the CDC class.
  USB_CDCDESCCSCP_CDCDATACLASS              = 0x0A, ///< Descriptor Class value indicating that the device or interface belongs to the CDC Data class.
  USB_CDCDESCCSCP_NODATASUBCLASS            = 0x00, ///< Descriptor Subclass value indicating that the device or interface belongs to no specific subclass of the CDC data class.
  USB_CDCDESCCSCP_NODATAPROTOCOL            = 0x00, ///< Descriptor Protocol value indicating that the device or interface belongs to no specific protocol of the CDC data class.
} USBCDCDESCCSCP;

// enumerate the CDC descriptor sub types
typedef enum _USBCDCDSUBTYPES
{
  USB_CDCDSUBTYPES_CSINTF_HEADER           = 0x00, ///< CDC class-specific Header functional descriptor
  USB_CDCDSUBTYPES_CSINTF_CALLMANAGEMENT   = 0x01, ///< CDC class-specific Call Management functional descriptor
  USB_CDCDSUBTYPES_CSINTF_ACM              = 0x02, ///< CDC class-specific Abstract Control Model functional descriptor
  USB_CDCDSUBTYPES_CSINTF_DIRECTLINE       = 0x03, ///< CDC class-specific Direct Line functional descriptor
  USB_CDCDSUBTYPES_CSINTF_TELEPHONERINGER  = 0x04, ///< CDC class-specific Telephone Ringer functional descriptor
  USB_CDCDSUBTYPES_CSINTF_TELEPHONECALL    = 0x05, ///< CDC class-specific Telephone Call functional descriptor
  USB_CDCDSUBTYPES_CSINTF_UNION            = 0x06, ///< CDC class-specific Union functional descriptor
  USB_CDCDSUBTYPES_CSINTF_COUNTRYSELECTION = 0x07, ///< CDC class-specific Country Selection functional descriptor
  USB_CDCDSUBTYPES_CSINTF_TELEPHONEOPMODES = 0x08, ///< CDC class-specific Telephone Operation Modes functional descriptor
  USB_CDCDSUBTYPES_CSINTF_USBTERMINAL      = 0x09, ///< CDC class-specific USB Terminal functional descriptor
  USB_CDCDSUBTYPES_CSINTF_NETWORKCHANNEL   = 0x0A, ///< CDC class-specific Network Channel functional descriptor
  USB_CDCDSUBTYPES_CSINTF_PROTOCOLUNIT     = 0x0B, ///< CDC class-specific Protocol Unit functional descriptor
  USB_CDCDSUBTYPES_CSINTF_EXTENSIONUNIT    = 0x0C, ///< CDC class-specific Extension Unit functional descriptor
  USB_CDCDSUBTYPES_CSINTF_MULTICHANNEL     = 0x0D, ///< CDC class-specific Multi-Channel Management functional descriptor
  USB_CDCDSUBTYPES_CSINTF_CAPI             = 0x0E, ///< CDC class-specific Common ISDN API functional descriptor
  USB_CDCDSUBTYPES_CSINTF_ETHERNET         = 0x0F, ///< CDC class-specific Ethernet functional descriptor
  USB_CDCDSUBTYPES_CSINTF_ATM              = 0x10, ///< CDC class-specific Asynchronous Transfer Mode functional descriptor
} USBCDCDSUBTYPES;

/// enumerate the control requests
typedef enum _USBCNTLREQ
{
  USB_CNTLREQ_GETSTATUS           = 0,
  USB_CNTLREQ_CLEARFEATURE        = 1,
  USB_CNTLREQ_SETFEATURE          = 3,
  USB_CNTLREQ_SETADDRESS          = 5,
  USB_CNTLREQ_GETDESCRIPTOR       = 6,
  USB_CNTLREQ_SETDESCRIPTOR       = 7,
  USB_CNTLREQ_GETCONFIGURATION    = 8,
  USB_CNTLREQ_SETCONFIGURATION    = 9,
  USB_CNTLREQ_GETINTERFACE        = 10,
  USB_CNTLREQ_SETINTERFACE        = 11,
  USB_CNTLREQ_SYNCHFRAME          = 12,
} USBCNTLREQ;

//// enumerate the feature selectors
typedef enum _USBFEATURESEL
{
  USB_FEATURESEL_ENDPOINTHALT       = 0x00,
  USB_FEATURESEL_DEVICEREMOTEWAKEUP = 0x01,
  USB_FEATURESEL_TESTMODE           = 0x02,
} USBFEATURESEL;

// structures -----------------------------------------------------------------
/// define the request header 
typedef struct _USBREQHEADER
{
  U8            nRequestType;   ///< Type of the request.
  U8            nRequest;       ///< Request command code.
  U16           wValue;         ///< wValue parameter of the request.
  U16           wIndex;         ///< wIndex parameter of the request.
  U16           wLength;        ///< Length of the data to transfer in bytes.
} PACKED USBREQHEADER, *PUSBREQHEADER;
#define USBREQHEADER_SIZE             sizeof( USBREQHEADER )

/// define the descriptor header
typedef struct _USBDESCHEADER
{
  U8            nLength; 
  U8            nDescriptorType;
} PACKED USBDESCHEADER, *PUSBDESCHEADER;
#define USBDESCHEADER_SIZE            sizeof( USBDESCHEADER )

/// define the device descriptor
typedef struct _USBDEVDESC
{
  USBDESCHEADER tHeader;
  U16           wBcdUSB; 
  U8            nDeviceClass; 
  U8            nDeviceSubClass; 
  U8            nDeviceProtocol; 
  U8            nMaxPacketSize0; 
  U16           wVendor; 
  U16           wProduct; 
  U16           wBcdDevice; 
  U8            nManufacturer;
  U8            nProduct;
  U8            nSerialNumber;
  U8            nNumConfigurations;
} PACKED USBDEVDESC, *PUSBDEVDESC;
#define USBDEVDESC_SIZE               sizeof( USBDEVDESC )

/// define the device qualifier descriptor
typedef struct _USBDEVQQUALDESC
{
  USBDESCHEADER tHeader;
  U16           wBcdUSB; 
  U8            nDeviceSubClass; 
  U8            nMaxPacketSize0;
  U8            nNumConfigurations;
  U8            nReserved;
} PACKED USBDEVQQUALDESC, *PUSBDEVQQUALDESC;
#define USBDEVQUALDESC_SIZE           sizeof( USBDEVQUALDESC )

/// define the configuration descriptor
typedef struct _USBCONFDESC
{
  USBDESCHEADER tHeader;
  U16           wTotalLength;
  U8            nNumInterfaces; 
  U8            nConfigurationValue; 
  U8            nConfiguration; 
  U8            nAttributes;
  U8            nMaxPower;
} PACKED USBCONFDESC, *PUSBCONFDESC;
#define USBCONFDESC_SIZE              sizeof( USBCONFDESC )

/// define the interface descriptor
typedef struct _USBINTFDESC
{
  USBDESCHEADER tHeader;
  U8            nInterfaceNumber; 
  U8            nAlternateSetting; 
  U8            nNumEndpoints; 
  U8            nInterfaceClass; 
  U8            nInterfaceSubClass; 
  U8            nInterfaceProtocol; 
  U8            nInterface;
} PACKED USBINTFDESC, *PUSBINTFDESC;
#define USBINTFDESC_SIZE              sizeof( USBINTFDESC )

/// define the interface associatione descriptor
typedef struct _USBINTFASSCDESC
{
  USBDESCHEADER tHeader;
  U8            nFirstInterface; 
  U8            nInterfaceCount; 
  U8            nFunctionClass; 
  U8            nFunctionSubClass; 
  U8            nFunctionProtocol; 
  U8            nFunction;
} PACKED USBINTFASSCDESC, *PUSBINTFASSCDESC;
#define USBINTFASSCDESC_SIZE          sizeof( USBINTFASSCDESC )

/// define the CDC functional header descriptor
typedef struct _USBCDFUNCCDESC
{
  USBDESCHEADER tHeader;
  U8            nSubType;
  U16           wCdcSpecification;
} PACKED USBCDFUNCCDESC, *PUSBCDFUNCCDESC;
#define USBCDFUNCCDESC_SIZE           sizeof( USBCDFUNCCDESC )

/// define the CDC functional ACM descriptor
typedef struct _USBCDCFUNCACMDESC
{
  USBDESCHEADER tHeader;
  U8            nSubType;
  U8            nCapabilities;
} PACKED USBCDCFUNCACMDESC, *PUSBCDCFUNCACMDESC;
#define USBCDCFUNCACMDESC_SIZE        sizeof( USBCDCFUNCACMDESC )

/// define the CDC call management functionsal descriptor
typedef struct _USBCDCCALLMGMTDESC
{
  USBDESCHEADER tHeader;
  U8            nSubType;
  U8            nCapabilities;
  U8            nDataInterface;
} PACKED USBCDCCALLMGMTDESC, *PUSBCDCCALLMGMTDESC;
#define  USBCDCCALLMGMTDESC_SIZE      sizeof( USBCDCCALLMGMTDESC )

/// define the CDC functional union descriptor
typedef struct _USBCDCFUNCUNIONDESC
{
  USBDESCHEADER tHeader;
  U8            nSubType;
  U8            nMasterInterfaceNumber;
  U8            nSlaveInterfaceNumber;
} PACKED USBCDCFUNCUNIONDESC, *PUSBCDCFUNCUNIONDESC;
#define USBCDCFUNCUNIONDESC_SIZE      sizeof( USBCDCFUNCUNIONDESC )

/// define the endpoint descriptor
typedef struct _USBENDDESCR
{
  USBDESCHEADER tHeader;
  U8            nEndpointAddress;
  U8            nAttributes;
  U16           wMaxPacketSize;
  U8            nInterval;
} PACKED USBENDDESCR, *PUSBENDDESCR;
#define USBENDDESCR_SIZE              sizeof( USBENDDESCR )

/// define the string descriptor
typedef struct _USBSTRDESC
{
  USBDESCHEADER tHeader;
  U16           awString[];
} PACKED USBSTRDESC, *PUSBSTRDESC;
#define USBSTRDESC_SIZE               sizeof( USBSTRDESC )

/**@} EOF USBHandler_def.h */

#endif  // _USBHANDLER_DEF_H