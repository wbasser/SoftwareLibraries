/******************************************************************************
 * @file Uart_cfg.h
 *
 * @brief UART configuration declarations
 *
 * This file contains the UARt configuration declarations
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
 
// ensure only one instatiation
#ifndef _UART_CFG_H
#define _UART_CFG_H

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "UART/Uart_def.h"

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------
/// declare the UART enuemrations
typedef enum  _UARTDEVENUM
{
  // enumerate devices below
  UART_DEV_ENUM_DBG = 0,
  
  // do not remove these entries
  UART_DEV_ENUM_MAX,
  UART_DEV_ENUM_ILLEGAL = 0xFF
} UARTDEVENUM;

// global function prototypes --------------------------------------------------
extern  const UARTDEF atUartDefs[ ];

/**@} EOF Uart_cfg.h */

#endif  // _UART_CFG_H
