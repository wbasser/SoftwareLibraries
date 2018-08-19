/******************************************************************************
 * @file Event.h
 *
 * @brief Event generator declarations
 *
 * This file provides the declarations for the egnet generator
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
 * \addtogroup Event
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _EVENT_H
#define _EVENT_H

// local includes -------------------------------------------------------------
#include "Event/Event_cfg.h"

// Macros and Defines ---------------------------------------------------------

// global function prototypes --------------------------------------------------
extern  void  Event_Initialize( void );
extern  void  Event_Close( void );
//extern  Event IOCTL( EVENTENUM eEnum, EVENTIOCACTION eAction, PVOID pvArg );

/**@} EOF Event.h */

#endif  // _EVENT_H