/******************************************************************************
 * @file Uart_cfg.h
 *
 * @brief UART configuration declarations
 *
 * This file contains the UARt configuration declarations
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
  UART_DEV_MAX
} UARTDEVENUM;

// global parameter declarations -----------------------------------------------

// global function prototypes --------------------------------------------------
extern  const UARTDEF atUartDefs[ ];

/**@} EOF Uart_cfg.h */

#endif  // _UART_CFG_