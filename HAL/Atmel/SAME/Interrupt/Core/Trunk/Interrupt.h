/******************************************************************************
 * @file Interrupt.h
 *
 * @brief Interrupt manager declarations
 *
 * This file provides the interrupt manager declarations
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
 * $Log: $
 * 
 *
 * \addtogroup Interrupt
 * @{
 *****************************************************************************/

// ensure only one instatiation
#ifndef _INTERRUPT_H
#define _INTERRUPT_H

// library includes -----------------------------------------------------------
#include "Types/Types.h"

// enumerations ---------------------------------------------------------------

// global function prototypes --------------------------------------------------
extern  void   Interrupt_Initialize( void );
extern  void   Interrupt_Disable( void );
extern  BOOL   Interrupt_Enable( void );

/**@} EOF Interrupt.h */

#endif  // _INTERRUPT_H