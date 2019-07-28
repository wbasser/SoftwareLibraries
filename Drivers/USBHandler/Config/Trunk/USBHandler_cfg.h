/******************************************************************************
 * @file USBHandler_cfg.h
 *
 * @brief USB handler configuration declarations
 *
 * This file provides the declara
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
#ifndef _USBHANDLER_CFG_H
#define _USBHANDLER_CFG_H

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------

// library includes -----------------------------------------------------------
#include "USBHandler/USBHandler_def.h"

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------
/// enumerate the string indices
typedef enum _STRINGID
{
  STRING_ID_LANGUAGE = 0,
  STRING_ID_MANUFACUTRER,
  STRING_ID_PRODUCT,
} STRINGID;

/// enumerate the interface indices
typedef enum _INTERFACEID
{
  INTERFACE_ID_CDCCCI = 0,
  INTERFACE_ID_CDCDCI,
} INTERFACEID;

// structures -----------------------------------------------------------------
/// declare the configuration descriptro
typedef struct  _USBDESCCONF
{
  // configuration descriptor
  USBCONFDESC         tConfig;
  
  // CDC Control interface
  USBINTFASSCDESC     tCdcIAd;
  USBINTFDESC         tCdcCcdInterface;
  USBCDFUNCCDESC      tCdcFuncionHeader;
  USBCDCFUNCACMDESC   tCdcFunctionAcm;
  USBCDCFUNCUNIONDESC tCdcFunctionUnion;
  USBENDDESCR         tCdcNotificationEndpoint;
  
  // DCD Data Interface
  USBINTFDESC         tCdcDciInterface;
  USBENDDESCR         tCdcDataOutEndpoint;
  USBENDDESCR         tCdcDataInEndpoint;
} USBDESCCONF;
 
// global parameter declarations -----------------------------------------------
extern  const USBDEVDESC  g_tDeviceDescriptor;
extern  const USBCONFDESC g_tConfigDescriptor;
extern  const USBSTRDESC  g_tLanguageString;
extern  const USBSTRDESC  g_tManufacuterString;
extern  const USBSTRDESC  g_tProductString;

// global function prototypes --------------------------------------------------
extern  BOOL  USBHandler_SetConfigurtionCB( U16 wConfig );
extern  BOOL  USBHandler_SetInterfaceCB( U16 wInterface );
extern  void  USBHandler_ControlOutCB( void );
extern  void  USBHandler_ControlInCB( void );

/**@} EOF USBHandler_cfg.h */

#endif  // _USBHANDLER_CFG_H