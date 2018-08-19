/******************************************************************************
 * @file UsiI2cSlave_def.h
 *
 * @brief Universal Serial Interface definitions declarations
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
#ifndef _USII2CSLAVE_DEF_H
#define _USII2CSLAVE_DEF_H

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "UsiI2cSlave/UsiI2cSlave_prm.h"

// library includes -----------------------------------------------------------
#include "Types/Types.h"

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------
/// enumerate the callbacks
typedef enum _USIIRQEVENTS
{
  USI_IRQ_EVENT_STPRCVD = 0,          ///< stop received
  USI_IRQ_EVENT_GETCHAR,              ///< get a character to trnasmit
  USI_IRQ_EVENT_PUTCHAR               ///< put a character upon receive
} USIIRQEVENTS;

// structures -----------------------------------------------------------------
/// define the callback
typedef BOOL  ( *PVUSIINTCALLBACK )( USIIRQEVENTS, PU8 );

// global parameter declarations -----------------------------------------------

// global function prototypes --------------------------------------------------

/**@} EOF UsiI2cSlave_def.h */

#endif  // _USII2CSLAVE_DEF_H