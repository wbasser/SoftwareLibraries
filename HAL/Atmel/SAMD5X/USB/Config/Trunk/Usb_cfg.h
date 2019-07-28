/******************************************************************************
 * @file Usb_cfg.h  
 *
 * @brief USB Driver configuration declarations 
 *
 * This file provides the configuration declarations for the USB driver
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

// local includes -------------------------------------------------------------

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------
/// define the maximum number of endpoints
#define USB_NUMBER_ENDPOINTS                    ( 5 )

/// define the endpoint size
#define`USB_ENDPOINT0_SIZE                      ( 64 )

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations -----------------------------------------------

// global function prototypes --------------------------------------------------
extern  void  Usb_HandleSsetup( void );
extern  void  Usb_HandleControlOutCallback( void );
extern  void  Usb_HandleCcontrolInCallback( void );
extern  void  Usb_HandleEndpointsCallback( void );

/**@} EOF Usb_cfg.h */

#endif  // _USB_CFG_H