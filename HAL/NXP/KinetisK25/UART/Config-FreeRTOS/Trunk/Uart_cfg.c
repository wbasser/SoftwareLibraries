/******************************************************************************
 * @file Uart_cfg.c
 *
 * @brief UART configuraiton implementation
 *
 * This file pvoides the UART configuration implementation
 *  
 * @copyright Copyright (c) 2012 CyberIntegration
 * This document contains proprietary data and information of CyberIntegration
 * LLC. It is the exclusive property of CyberIntegration, LLC and will not be
 * disclosed in any form to any party without prior written permission of
 * CyberIntegration, LLC. This document may not be reproduced or further used
 * without the prior written permission of CyberIntegration, LLC.
 *
 * Version History
 * ======
 * $Rev: $
 * 
 *
 * \addtogroup UART
 * @{
 *****************************************************************************/

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "UART/Uart_cfg.h"

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations ----------------------------------------------
/// declare the buffers
// UART_BUFFER( name, rxsize, txsize );

/// device configuration table
const UARTDEF atUartDefs[ UART_DEV_MAX ] = 
{
  // UART_DEF( chan, baudrate, parity, name, rxsize, txsize, callback, rcvchr, rcverr, txemp, txcmp )
};  
  /**@} EOF Uart_cfg.c */
