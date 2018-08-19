/******************************************************************************
 * @file Usb_cfg.c
 *
 * @brief USB configuration implementation file 
 *
 * This file provides the implementaion for the abstraction layer of the
 * USB
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
 * \addtogroup Usb
 * @{
 *****************************************************************************/

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "USB/Usb_cfg.h"

// library includes -----------------------------------------------------------

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
 * @brief handle the setup 
 *
 * This function will call the setup handler
 *
 * @param[in]   pnBuffer
 *
 *****************************************************************************/
void Usb_HandleSetUp( PU8 pnBuffer )
{
}

/******************************************************************************
 * @function Usb_HandlerControlOutComplete
 *
 * @brief handle control out completion
 *
 * This function will call the control out completion handler
 *
 *****************************************************************************/
void Usb_HandlerControlOutComplete( void )
{
}

/******************************************************************************
 * @function Usb_HandlerControlInComplete
 *
 * @brief handle control in completion
 *
 * This function will call the control in completion handler
 *
 *****************************************************************************/
void Usb_HandlerControlInComplete( void )
{
}

/******************************************************************************
 * @function USB_CbCompletion
 *
 * @brief USB all callback completions
 *
 * This function will call the callback to mark that all interrupts have been
 * handler
 *
 *****************************************************************************/
void  Usb_CbCompletion( void )
{
}

 /**@} EOF Usb_Cfg.c */
