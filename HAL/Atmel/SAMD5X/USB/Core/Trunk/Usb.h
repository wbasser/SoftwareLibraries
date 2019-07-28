/******************************************************************************
 * @file Usb.h  
 *
 * @brief USB Driver declrations
 *
 * This file provides the declarations for the USB driver
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
 * \addtogroup Usb
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _Usb_H
#define _USB_H

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "USb/Usb_cfg.h"

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------
/// define the speed enumeration
typedef enum _USPSPEED
{
  USB_SPEED_LOW = 0,
  USB_SPEED_FULL,
  USB_SPEED_HIGH,
} USBSPEED;

/// enmerate the endpoint type
typedef enum _USBEPTYPE
{
  USB_EPTYPE_CONTROL = 0,
  USB_EPTYPE_ISOCHRONOUS,
  USB_EPTYPE_BULK,
  USB_EPTYPE_INTERRUPT,
  USB_EPTYPE_DUAL_BANK,
} USBEPTYPE;

/// enumerate the endpoint attributes
typedef enum _USBEPATTRIB
{
  USB_EPATTRIB_NOSYNC   = 0x00,
  USB_EPATTRIB_ASYNC    = 0x04,
  USB_EPATTRIB_ADAPTIVE = 0x08,
  USB_EPATTRIB_SYNC     = 0x0C
} USBEPATTRIB;

/// enumerate the endoint usages
typedef enum _USBEPUSAGE
{
  USB_EPUSAGE_DATA      = 0x00,
  USB_EPUSAGE_FEEDBACK  = 0x10,
  USB_EPUSAGE_IMPFDBK   = 0x20
} USBEPUSAGE;

/// enumerate the direction
typedef enum _USBDIRECTION
{
  USB_DIRECTION_OUT = 0,
  USB_DIRECTION_IN = 0x80,
} USBDIRECTION;

// structures -----------------------------------------------------------------

// global parameter declarations -----------------------------------------------
extern  U8  ALIGNED4 g_anUsbEp0BufIn[ ];
extern  U8  ALIGNED4 g_anUsbEp0BufOut[ ];

// global function prototypes --------------------------------------------------
extern  void      Usb_Initialize( void );
extern  void      Usb_Attach( void );
extern  void      Usb_Detach( void );
extern  void      Usb_Reset( void );
extern  void      Usb_SetAddress( U8 nAddr);
extern  void      Usb_EnableEp( U8 nEp, USBEPTYPE eType, U8 nPktSize );
extern  void      Usb_DisableEp( U8 nEp );
extern  void      Usb_ResetEp( U8 nEp );
extern  BOOL      Usb_EpReady( U8 nEp );
extern  BOOL      Usb_EpPending( U8 nEp );
extern  void      Usb_WaitForReady( U8 nEp );
extern  void      Usb_WaitForPending( U8 nEp );
extern  U8        Usb_StartOut( U8 nEp, PU8 pnData, U8 nLen );
extern  U8        Usb_EpStartIn( U8 nEp, PU8 pnData, U8 nSize, BOOL bZlp );
extern  U8        Usb_EpGetOutLength( U8 nEp );
extern  void      Usb_EpSetHandled(  U8 nEp );
extern  void      Usb_Ep0In( U8 nSize );
extern  void      Usb_Ep0Out( void );
extern  void      Usb_Ep0Stall( void );
extern  void      Usb_SetSpeed( USBSPEED eSpeed );
extern  USBSPEED  Usb_GetSpeed( void );

/**@} EOF Usb.h */

#endif  // _USB_H