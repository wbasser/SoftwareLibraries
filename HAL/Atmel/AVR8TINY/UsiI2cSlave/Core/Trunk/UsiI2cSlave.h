/******************************************************************************
 * @file UsiI2cSlave.h
 *
 * @brief Universal Serial Interface declarations
 *
 * This file provides the declarations for the universal serial interface module
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
 * \addtogroup USI
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _USII2CSLAVE_H
#define _USII2CSLAVE_H

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "UsiI2cSlave/UsiI2cSlave_cfg.h"

// library includes -----------------------------------------------------------
#include "Types/Types.h"

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------
/// define the interrupt callback type
typedef   BOOL ( *PVUSIINTCALLBACK )( USIIRQEVENTS eEvent, PU8 pnData );

// global parameter declarations -----------------------------------------------

// global function prototypes --------------------------------------------------
extern  void  UsiI2cSlave_Initialize( void );
extern  void  UsiI2cSlave_SetAddress( U8 nAddress );

/**@} EOF UsiI2cSlave.h */

#endif  // _USII2CSLAVE_H