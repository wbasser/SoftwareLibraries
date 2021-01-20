/******************************************************************************
 * @file Usb_cfg.c
 *
 * @brief USB configuration implementation
 *
 * This file provides the configuration implementation
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
 * \addtogroup Usb_cfg
 * @{
 *****************************************************************************/

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "Usb/Usb_cfg.h"

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations ----------------------------------------------

// local parameter declarations -----------------------------------------------

// local function prototypes --------------------------------------------------

// constant parameter initializations -----------------------------------------

/******************************************************************************
 * @function Usb_HandleStandardRequest
 *
 * @brief handle a standard request
 *
 * This function will call the desired standard request handler
 *
 * @param[in]   pnBuffer    pointer to the data buffer
 *
 *****************************************************************************/
void Usb_HandleStandardRequest( PU8 pnBuffer )
{
}

/******************************************************************************
 * @function Usb_ProcessRecvCallback
 *
 * @brief process the receive callback
 *
 * This function will process the receive callback
 *
 * @param[in]   nEp   endoint that requires servicing
 *
 *****************************************************************************/
void Usb_ProcessRecvCallback( U8 nEp )
{
}

/******************************************************************************
 * @function Usb_ProcessSendCallback
 *
 * @brief process the send callback
 *
 * This function will process the send callback
 *
 * @param[in]   nEp   endoint that requires servicing
 *
 *****************************************************************************/
void Usb_ProcessSendCallback( U8 nEp )
{
}

/**@} EOF Usb_cfg.c */
