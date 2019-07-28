/******************************************************************************
 * @file Usb_cfg.c
 *
 * @brief USB configuration implementation 
 *
 * This file provides the implementation for the USB configuration
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

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "Usb/Usb_cfg.h"

// library includes -----------------------------------------------------------
#include "USBHandler/USBHandler.h"

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations ----------------------------------------------

// local parameter declarations -----------------------------------------------

// local function prototypes --------------------------------------------------

// constant parameter initializations -----------------------------------------

/******************************************************************************
 * @function Usb_HandleSetup
 *
 * @brief handle the setup callback
 *
 * This function provides the callback to handle the setup
 *
 *****************************************************************************/
void Usb_HandleSetup( void )
{
  // call the USB handler setup
  USBHandler_ProcessSetupRequest( );
}

/******************************************************************************
 * @function Usb_HandleControlOutCallback
 *
 * @brief handle the control out complele callback
 *
 * This function provides the callback to handle control out completion
 *
 *****************************************************************************/
void Usb_HandleControlOutCallback( void )
{
  // call the USB handler control out process
  USBHandler_ProcessControlOut( );
}

/******************************************************************************
 * @function Usb_HandleCcontrolInCallback
 *
 * @brief handle the control in completion 
 *
 * This function provides the callback to handle control in completion
 *
 *****************************************************************************/
void Usb_HandleCcontrolInCallback( void )
{
  // call the USB handler control in process
  USBHandler_ProcessControlIn( );
}
 

/******************************************************************************
 * @function Usb_HandleEndpointsCallback
 *
 * @brief handle the endpoint completion 
 *
 * This function provides the callback to handle control in completion
 *
 *****************************************************************************/
void Usb_HandleEndpointsCallback( void )
{
}

/**@} EOF Usb_cfg.c */
