/******************************************************************************
 * @file LUFACDChandler_cfg.h
 *
 * @brief LUFA CDC Handler configuration declarations
 *
 * This file provides the LUFA CDC handler configuration declaration
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
 
// ensure only one instantiation
#ifndef _LUFACDCHANDLER_CFG_H
#define _LUFACDCHANDLER_CFG_H

// library includes -----------------------------------------------------------
#include "Micro/Types.h"
#include "LUFA/Drivers/USB/Usb.h"

// local includes -------------------------------------------------------------
#include "LUFACDCHandler/LUFACDCHandler_prm.h"

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------
/// enumerate the string ID
typedef enum _STRINGID
{
  STRING_ID_LANGUAGE = 0,
  STRING_ID_MANUFACTURER,
  STRING_ID_PRODUCT,
} STRINGID;

/// enumerate the interface IDS
typedef enum _INTERFACEID
{
  INTERFACE_ID_CDC_CCI  = 0,
  INTERFACE_ID_CDC_DCI,
  INTERFACE_ID_MSD
} INTERFACEID;

// structures -----------------------------------------------------------------
typedef struct _USB_Descriptor_Configuration_t
{
  USB_Descriptor_Configuration_Header_t    Config;

  // CDC Control Interface
  USB_Descriptor_Interface_Association_t    CDC_IAD;
  USB_Descriptor_Interface_t                CDC_CCI_Interface;
  USB_CDC_Descriptor_FunctionalHeader_t     CDC_Functional_Header;
  USB_CDC_Descriptor_FunctionalACM_t        CDC_Functional_ACM;
  USB_CDC_Descriptor_FunctionalUnion_t      CDC_Functional_Union;
  USB_Descriptor_Endpoint_t                 CDC_NotificationEndpoint;

  // CDC Data Interface
  USB_Descriptor_Interface_t                CDC_DCI_Interface;
  USB_Descriptor_Endpoint_t                 CDC_DataOutEndpoint;
  USB_Descriptor_Endpoint_t                 CDC_DataInEndpoint;
} USB_Descriptor_Configuration_t;

// global parameter declarations -----------------------------------------------
extern  const CODE USB_Descriptor_Device_t		    tDeviceDescriptor;
extern  const CODE USB_Descriptor_Configuration_t	tConfigurationDescriptor;
extern  const CODE USB_Descriptor_String_t			  tLanguageString;
extern  const CODE USB_Descriptor_String_t			  tManufacturerString;
extern  const CODE USB_Descriptor_String_t			  tProductString;

// global function prototypes --------------------------------------------------

/**@} EOF LUFACDCHandler_cfg.h */

#endif  // _LUFACDCHANDLER_CFG_H