/******************************************************************************
 * @file IrCapture.h
 *
 * @brief 
 *
 * This file provides the declarations of the IR remote control handler
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
 * \addtogroup IrHandler
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _IRCAPTURE_H
#define _IRCAPTURE_H

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------

// library includes -----------------------------------------------------------
#include "Timers/Timers.h"

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations -----------------------------------------------

// global function prototypes --------------------------------------------------
extern  void	IRCapture_Control( BOOL bState );
extern  void  IRCapture_Process( TIMERCBEVENT eEvent, U16 wCapture );

/**@} EOF IrCapture.h */

#endif  // _IRCAPTURE_H