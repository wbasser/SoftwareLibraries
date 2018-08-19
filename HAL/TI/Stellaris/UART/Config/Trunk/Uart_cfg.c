/******************************************************************************
 * @file Uart_cfg.c
 *
 * @brief UART configuraiton implementation
 *
 * This file pvoides the UART configuration implementation
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
 * $Log: $
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
  // UART_DEVICE( devper, gpiopper, devbase, gpiopbase, rxpin, txpin, rxfunc, txfunc, rxsize, txsize, rxbuf, txbuf, callback )
};

/**@} EOF Uart_cfg.c */
