/******************************************************************************
 * @file Event_cfg.h
 *
 * @brief Event generator configuration declarations
 *
 * This file provides the declarations for the configuration of the event generator
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
 * \addtogroup Event_cfg
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _EVENT_CFG_H
#define _EVENT_CFG_H

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "Event\Event_def.h"

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------
/// enumerate the user define events
typedef enum EVENTENUM
{
  // add user defined enumerations here
  
  // do not remove the below lines
  EVENT_ENUM_MAX,
  EVENT_ENUM_ILLEGAL = 0xFF
} EVENTENUM;

// structures -----------------------------------------------------------------

// global parameter declarations -----------------------------------------------
extern  const EVENTDEF  atEventDefs[ ];

/**@} EOF Event_cfg.h */

#endif  // _EVENT_CFG_H