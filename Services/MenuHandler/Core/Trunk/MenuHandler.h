/******************************************************************************
 * @file MenuHandler.h
 *
 * @brief menu handler implementation declarations 
 *
 * This file provides the declarataions for the implementation
 *
 * @copyright Copyright (c) 2012 CyberIntegration
 * This document contains proprietary data and information of CyberIntegration 
 * LLC. It is the exclusive property of CyberIntegration, LLC and will not be 
 * disclosed in any form to any party without prior written permission of 
 * CyberIntegration, LLC. This document may not be reproduced or further used 
 * without the prior written permission of CyberIntegration, LLC.
 *
 * $Date: $
 *
 * Version History
 * ======
 * $Rev: $
 * 
 *
 * \addtogroup MenuHandler
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _MENUHANDLER_H
#define _MENUHANDLER_H

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "MenuHandler/MenuHandler_cfg.h"

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations -----------------------------------------------

// global function prototypes --------------------------------------------------
extern  void  MenuHandler_Initialize( void );
extern  BOOL  MenuHandler_ProcessKey( U8 nKey );


/**@} EOF MenuHandler.h */

#endif  // _MENUHANDLER_H