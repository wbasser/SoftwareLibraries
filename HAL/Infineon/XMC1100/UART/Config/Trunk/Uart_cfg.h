/******************************************************************************
 * @file Uart_cfg.h
 *
 * @brief UART configuration declarations
 *
 * This file contains the UARt configuration declarations
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
 
// ensure only one instatiation
#ifndef _UART_CFG_H
#define _UART_CFG_H

// system includes ------------------------------------------------------------

// library includes -----------------------------------------------------------

// local includes -------------------------------------------------------------
#include "UART/Uart_def.h"

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------
/// declare the UART enuemrations
typedef enum  _UARTDEVENUM
{
  // add user instantiations here

  // do not remove the below entries
  UART_DEV_MAX
} UARTDEVENUM;

// global parameter declarations -----------------------------------------------

// global function prototypes --------------------------------------------------
extern  const UARTDEF atUartDefs[ ];

/**@} EOF Uart_cfg.h */

#endif  // _UART_CFG_
