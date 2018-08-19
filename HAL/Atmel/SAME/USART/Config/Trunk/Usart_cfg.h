/******************************************************************************
 * @file Usart_cfg.h
 *
 * @brief USART configuration declarations
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
 * \addtogroup USART
 * @{
 *****************************************************************************/
 
// ensure only one instatiation
#ifndef _USART_CFG_H
#define _USART_CFG_H

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "USART/Usart_def.h"

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------
/// declare the USART enuemrations
typedef enum  _USARTDEVENUM
{
  // enumerate devices below
  USART_DEV_ENUM_DBG = 0,
  
  // do not remove these entries
  USART_DEV_ENUM_MAX,
  USART_DEV_ENUM_ILLEGAL = 0xFF
} USARTDEVENUM;

// global function prototypes --------------------------------------------------
extern  const USARTDEF atUsartDefs[ ];

/**@} EOF Usart_cfg.h */

#endif  // _USART_CFG_H
