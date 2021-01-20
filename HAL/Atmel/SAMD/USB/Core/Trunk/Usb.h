/******************************************************************************
 * @file Usb.h
 *
 * @brief USB driver declarations
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
 * \addtogroup USB
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _USB_H
#define _USB_H

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "USB/Usb_Cfg.h"

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------
/// define the in/out endponits/masks 
#define USB_ENDPOINT_IN                       ( 0x80 )
#define USB_ENDPOINT_OUT                      ( 0x00 )
#define USB_ENDPOINT_INDEX_MASK               ( 0x7f )
#define USB_ENDPOINT_DIRECTION_MASK           ( 0x80 )

// enumerations ---------------------------------------------------------------
/// enumerate the endpoint types
typedef enum _USBENDPNTTYPE
{
  USB_ENDPNTTYPE_CONTROL                    = 0,
  USB_ENDPNTTYPE_ISOCHRONOUS                = 1,
  USB_ENDPNTTYPE_BULK                       = 2,
  USB_ENDPNTTYPE_INTERRUPT                  = 3,
  USB_ENDPNTTYPE_OTHER                      = 4,
} USBENDPNTTYPE;

// structures -----------------------------------------------------------------

// global parameter declarations -----------------------------------------------

// global function prototypes --------------------------------------------------
extern  void Usb_Initialize( void );
extern  void Usb_AttachDetach( BOOL bDetach );
extern  void Usb_ResetEndpoint( U8 nEp, U8 nDir );
extern  void Usb_ConfigureEndpoint( U8 nEndpoint, U8 nType, U16 wSize );
extern  BOOL Usb_IsEndpointConfigured( U8 nEp, U8 nDir );
extern  BOOL Usb_EndpointGetStatus( U8 nEp, U8 nDir );
extern  void Usb_EndpointSetFeature( U8 nEp, U8 nDir );
extern  void Usb_EndpointClearFeature( U8 nEp, U8 nDir );
extern  void Usb_SetAddress( U8 nAddress );
extern  void Usb_Send( U8 nEp, PU8 pnData, U16 wLength );
extern  void Usb_Recv( U8 nEp, PU8 pnData, U16 wLength );
extern  void Usb_ControlSendZlp( void );
extern  void Usb_ControlStall( void );
extern  void Usb_ControlSend( PU8 pnData, U16 wLength );

/**@} EOF Usb.h */

#endif  // _USB_H