/******************************************************************************
 * @file USBDevciceHandlerCDC_def.h
 *
 * @brief USB device handler CDC class definitions 
 *
 * This file provides the structure used by USB device handler when 
 * implementing a CDC classs
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
 * \addtogroup USBDevciceHandler
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _USBDEVCICEHANDLERCDC_DEF_H
#define _USBDEVCICEHANDLERCDC_DEF_H

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------

// library includes -----------------------------------------------------------
#include "Types/Types.h"

// Macros and Defines ---------------------------------------------------------
#define USB_CDC_INTERFACE_CLASS             ( 0x02 )
#define USB_CDC_INTERFACE_SUBCLASS_ACM      ( 0x02 )
#define USB_CDC_INTERFACE_CLASS_DATA        ( 0x0A )

#define USB_CDC_SUBTYPE_HEADER              ( 0x00 )
#define USB_CDC_SUBTYPE_ACM                 ( 0x02 )
#define USB_CDC_SUBTYPE_UNION               ( 0x06 )

#define USB_CDC_SEND_ENCAPSULATED_COMMAND   ( 0x00 )
#define USB_CDC_GET_ENCAPSULATED_RESPONSE   ( 0x01 )
#define USB_CDC_SET_LINE_ENCODING           ( 0x20 )
#define USB_CDC_GET_LINE_ENCODING           ( 0x21 )
#define USB_CDC_SET_CONTROL_LINE_STATE      ( 0x22 )
#define USB_CDC_SEND_BREAK                  ( 0x23 )

#define USB_CDC_NOTIFICATION_BUF_SIZE       ( 8 )
#define USB_CDC_IN_OUT_BUF_SIZE             ( 64 )

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------
/// define the function header descriptor
typedef struct PACKED _USBDEVCDCFUNCHEADDESC
{
    U8  nLength;
    U8  nDescriptorType;
    U8  nDescriptorSubtype;
    U16 wCdCDC;
} USBDEVCDCFUNCHEADDESC, *PUSBDEVCDCFUNCHEADDESC;
#define USBDEVCDCFUNCHEADDESC_SIZE      sizeof( USBDEVCDCFUNCHEADDESC )

/// define the DCD function descriptor
typedef struct PACKED _USBDEVCDCFUNCACMDESC
{
    U8  nLength;
    U8  nDescriptorType;
    U8  nDescriptorSubtype;
    U8  nCapabilities;
} USBDEVCDCFUNCACMDESC, *PUSBDEVCDCFUNCACMDESC;
#define USBDEVCDCFUNCACMDESC_SIZE       sizeof( USBDEVCDCFUNCACMDESC )

/// define the functional union descriptor
typedef struct PACKED _USBDEVCDCFUNCNIONDESC
{
    U8  nLength;
    U8  nDescriptorType;
    U8  nDescriptorSubtype;
    U8  nMasterInterface;
    U8  nSlaveInterface;
} USBDEVCDCFUNCNIONDESC, *PUSBDEVCDCFUNCNIONDESC;
#define USBDEVCDCFUNCNIONDESC_SIZE    sizeof( USBDEVCDCFUNCNIONDESC )

/// define the line encoding structure
typedef struct PACKED _USBDEVICECDCLINEENCODING
{
  U32   uBaudRate;
  U8    nCharFormat;
  U8    nParityType;
  U8    nDataBits;
} USBDEVICECDCLINEENCODING, *PUSBDEVICECDCLINEENCODING;
#define USBDEVICECDCLINEENCODING_SIZE         sizeof( USBDEVICECDCLINEENCODING )

// global parameter declarations -----------------------------------------------

// global function prototypes --------------------------------------------------

/**@} EOF USBDevciceHandlerCDC_def.h */

#endif  // _USBDEVCICEHANDLERCDC_DEF_H