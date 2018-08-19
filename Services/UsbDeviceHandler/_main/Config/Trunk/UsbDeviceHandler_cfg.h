/******************************************************************************
 * @file UsbDeviceHandler_cfg.h
 *
 * @brief USB device handler configuration declarations 
 *
 * This file provides the declarations for the USB device handler configuration
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
#ifndef _USBDEVICEHANDLER_CFG_H
#define _USBDEVICEHANDLER_CFG_H

// system includes ------------------------------------------------------------

// local includes -----------------------------------------------------------
#include "UsbDeviceHandler/UsbDeviceHandler_def.h"
#include "UsbDeviceHandler/UsbDeviceHandler_prm.h"
#include "UsbDeviceHandler/CDCClass/UsbDeviceHandlerCDC_def.h"

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------
// enumerate the interfaces
typedef enum _INTFACEENUM
{
  INTFACE_ENUM_VENDOR = 0,
  INTFACE_ENUM_CDC_CONTROL,
  INTFACE_ENUM_CDC_DATA
} INTFACEENUM;

// structures -----------------------------------------------------------------
/// define the configuration descriptor
typedef struct PACKED _USBCONFIGURATIONDESCR
{
  USBCONFIGURATIONDESCRIPTOR    tConfigDesc;
  USBINTERFACEDESCRIPTOR        tInterfaceDsesc;
  USBINTERFACEDESCRIPTOR        tCdcControlDsesc;
  USBDEVCDCFUNCHEADDESC         tCdcFuncHeader;
  USBDEVCDCFUNCACMDESC          tCdcFuncAcm;
  USBDEVCDCFUNCNIONDESC         tCdcFuncUnion;
  USBENDPOINTDESCRIPTOR         tCdcNotificationEndpoint;
  USBINTERFACEDESCRIPTOR        tCdcDatInterface;
  USBENDPOINTDESCRIPTOR         tCdcOutEndpoint;
  USBENDPOINTDESCRIPTOR         tCdcInENdpoint;
} USBCONFIGURATIONDESCR, *PUSBCONFIGURATIONDESCR;
#define USBCONFIGURATIONDESCR_SIZE           sizeof( USBCONFIGURATIONDESCR )

// global parameter declarations -----------------------------------------------
extern  const USBDEVICEDESCRIPTOR   tUsbDeviceDescriptor;
extern  const USBMSFTCOMPDESCRIPTOR tUsbMstComp;
extern  const USBMSFTPROPDESCRIPTOR tUsbMstProd;
extern  const USBSTRINGDESCRIPTOR   tUsbLangueString;
extern  const USBSTRINGDESCRIPTOR   tUsbMsftOSString;
extern  const C8                    szUsbManfString[ ];
extern  const C8                    szUsbProdString[ ];
extern  U8                          anUsbBufOut[ ];

// global function prototypes --------------------------------------------------
extern  void  UsbDeviceHandler_ControlSetupCallback( void );
extern  BOOL  UsbDeviceHandler_SetConfigurationCallback( U8 nConfig );
extern  BOOL  UsbDeviceHandler_SetInterfaceCallback( U8 nConfig );
extern  void  UsbDeviceHandler_HandleControlReceipients( U16 wType, U16 wValue );

/**@} EOF USBDEVICEHANDLER_CFG.h */

#endif  // _USBDEVICEHANDLER_CFG_H