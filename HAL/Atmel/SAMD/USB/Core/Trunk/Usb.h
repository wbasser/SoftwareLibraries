/******************************************************************************
 * @file Usb.h
 *
 * @brief USB declarations
 *
 * This file provides the declarations for the USB driver
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
 * \addtogroup USB
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _USB_H
#define _USB_H

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "USB/Usb_cfg.h"

// library includes -----------------------------------------------------------
#include "Types/Types.h"

// Macros and Defines ---------------------------------------------------------
/// define the size of the endpoint 0 buffer
#define USB_EP0_BUF_SIZE                    ( 64 )


// enumerations ---------------------------------------------------------------
/// enumerate the USB speeds
typedef enum _USBSPPED
{
	USB_SPEED_LOW = 0,
	USB_SPEED_FULL,
	USB_SPEED_HIGH
} USBSPEED;

// structures -----------------------------------------------------------------

// global parameter declarations -----------------------------------------------
extern  U8  anEp0BufIn[ ];
extern  U8  anEp0BufOut[ ];

// global function prototypes --------------------------------------------------
extern  void      Usb_Initialize( void );
extern  void      Usb_Reset( void );
extern  void      Usb_SetAddress( U8 nAddr );
extern  void      Usb_EnableEp( U8 nEndpoint, U8 nType, U16 wBufSize );
extern  void      Usb_DisableEp( U8 nEndpoint );
extern  void      Usb_ResetEp( U8 nEndpoint );
extern  U8        Usb_EpStartOut( U8 nEndpoint, PU8 pnData, U16 wSize );
extern  U8        Usb_EpStartIn( U8 nEndpoint, PU8 pnData, U16 wSize, BOOL bZlp );
extern  BOOL      Usb_EpEmpty( U8 nEndpoint );
extern  BOOL      Usb_EpReady( U8 nEndpoint );
extern  BOOL      Usb_EpPending( U8 nEndpoint );
extern  void      Usb_EpHandled( U8 nEndpoint );
extern  U16       Usb_EpOutLength( U8 nEndpoint );
extern  void      Usb_Detach( void );
extern  void      Usb_Attach( void );
extern  void      Usb_Ep0Out( void );
extern  void      Usb_Ep0In( U8 nSize );
extern  void      Usb_Ep0Stall( void );
extern  void      Usb_SetSpeed( USBSPEED eSpeed );
extern  USBSPEED  Usb_GetSpeed( void );
extern  PU8       Usb_GetSerialNumberSctringDescriptor( void );

/**@} EOF Usb.h */

#endif  // _USB_H