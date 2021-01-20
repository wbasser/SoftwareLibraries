/******************************************************************************
 * @file Usb_Cfg.h
 *
 * @brief USB configuration declarations 
 *
 * This file provides the declarations for the USB configuration
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
#ifndef _USB_CFG_H
#define _USB_CFG_H

// system includes ------------------------------------------------------------
#include "Types/Types.h"

// local includes -------------------------------------------------------------

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------
/// define the number of endpoints
#define USB_NUM_ENDPOINTS                     ( 8 )

/// define the maximum packet size
#define USB_MAK_PACKET_SIZE                   ( 64 )

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations -----------------------------------------------

// global function prototypes --------------------------------------------------
extern  void  Usb_HandleStandardRequest( PU8 pnBuffer );
extern  void  Usb_ProcessRecvCallback( U8 nEp, U16 wCount );
extern  void  Usb_ProcessSendCallback( U8 nEp );

/**@} EOF Usb_Cfg.h */

#endif  // _USB_CFG_H