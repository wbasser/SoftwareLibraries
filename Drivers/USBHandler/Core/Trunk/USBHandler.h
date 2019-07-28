/******************************************************************************
 * @file USBHandler.h
 *
 * @brief USB handler declarations
 *
 * This file provides the declarations for the USB handler
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
#ifndef _USBHANDLER_H
#define _USBHANDLER_H

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "USBHandler/USBHandler_cfg.h"

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations -----------------------------------------------

// global function prototypes --------------------------------------------------
extern  void  USBHandler_ProcessSetupRequest( void );
extern  void  USBHandler_ProcessControlOut( void );
extern  void  USBHandler_ProcessControlIn( void );

/**@} EOF USBHandler.h */

#endif  // _USBHANDLER_H