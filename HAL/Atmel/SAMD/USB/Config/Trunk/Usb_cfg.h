/******************************************************************************
 * @file Usb_cfg.h
 *
 * @brief USB configuration declarations
 *
 * This file provides the configuration delcartions
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
 * \addtogroup Usb_cfg
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _USB_CFG_H
#define _USB_CFG_H

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------

// library includes -----------------------------------------------------------
#include "Types/Types.h"

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations -----------------------------------------------

// global function prototypes --------------------------------------------------
extern  void  Usb_HandleSetUp( PU8 pnBuffer );
extern  void  Usb_HandlerControlOutComplete( void );
extern  void  Usb_HandlerControlInComplete( void );
extern  void  Usb_CbCompletion( void );

/**@} EOF Usb_cfg.h */

#endif  // _USB_CFG_H