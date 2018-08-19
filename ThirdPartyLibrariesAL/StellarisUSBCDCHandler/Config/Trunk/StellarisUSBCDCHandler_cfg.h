/******************************************************************************
 * @file USBCDCHandler_cfg.h
 *
 * @brief USB CDC handler configuration declarations
 *
 * This file provides the USB CDC handler configuration declarations
 *
 * @copyright Copyright (c) 2012 Cyber Intergration
 * This document contains proprietary data and information of Endurance Products 
 * LLC. It is the eUSBCDCHANDER_CFGclusive property of Endurance Products, LLC and will not be 
 * disclosed in any form to any party without prior written permission of 
 * Endurance Products, LLC. This document may not be reproduced or further used 
 * without the prior written permission of Endurance Products, LLC.
 *
 * Version History
 * ======
 * $Log: $
 * 
 *
 * \addtogroup USBCDCHandler
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _USBCDCHANDER_CFG_H
#define _USBCDCHANDER_CFG_H

// library includes -----------------------------------------------------------

// local includes -------------------------------------------------------------
#include "Micro/Types.h"

// Macros and Defines ---------------------------------------------------------

// global parameter declarations -----------------------------------------------
extern const PU8 const apUSBCDCStringDescriptors[ ];

// global function prototypes --------------------------------------------------
extern  U8  USBCDCHandler_GetNumberOfStrings( void );

/**@} EOF USBCDCHandler_cfg.h */

#endif  // _USBCDCHANDER_CFG_H