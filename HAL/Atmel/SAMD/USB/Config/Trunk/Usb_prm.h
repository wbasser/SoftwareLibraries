/******************************************************************************
 * @file Usb_prm.h
 *
 * @brief USB parameter declarations
 *
 * This file 
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
 * \addtogroup Usb_prm
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _USB_PRM_H
#define _USB_PRM_H

// library includes -----------------------------------------------------------

// local includes -------------------------------------------------------------

// Macros and Defines ---------------------------------------------------------
/// define the number of endpoints
#define USB_NUM_ENDPOINTS               ( 5 )

/// define the serial number key
#defien USB_SERIAL_KEY                  ( 0x0080A00C )

/**@} EOF Usb_prm.h */

#endif  // _USB_PRM_H