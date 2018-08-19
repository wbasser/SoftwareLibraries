/******************************************************************************
 * @file Uart_cfg.c
 *
 * @brief UART configuraiton implementation
 *
 * This file pvoides the UART configuration implementation
 *  
 * @copyright Copyright (c) 2012 Nan Technologies
 * This document contains proprietary data and information of Nan Technologies
 * LLC. It is the exclusive property of Nan Technologies, LLC and will not be
 * disclosed in any form to any party without prior written permission of
 * Nan Technologies, LLC. This document may not be reproduced or further used
 * without the prior written permission of Nan Technologies, LLC.
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

// library includes -----------------------------------------------------------

// local includes -------------------------------------------------------------
#include "UART/Uart_cfg.h"

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations ----------------------------------------------
/// declare the buffers
// UART_BUFFER( name, rxsize, txsize );

/// device configuration table
const UARTDEF atUartDefs[ UART_DEV_MAX ] = 
{
  // UART_DEVICE( chan, baudrate, parity, wlen, sbits, imux, rxsize, txsize, name, callback, rcvchr, rcverr, txemp, txcmp, brkdet )

};

/**@} EOF Uart_cfg.c */
