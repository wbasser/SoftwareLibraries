/****************************************7**************************************
 * @file  DALIMessageHandler_prm.h
 *
 * @brief  DALI message handler declarations
 *
 * This file provides the declarations for the DALI message handler
 *
 * @copyright Copyright (c) 2012 Nan Technologies
 * This document contains proprietary data and information of Nan Technologies
 * LLC. It is the exclusive property of Nan Technologies, LLC and will not be
 * disclosed in any form to any party without prior written permission of
 * Nan Technologies, LLC. This document may not be reproduced or further used
 * without the prior written permission of Nan Technologies, LLC.
 *
 * Version History
 * ======
 * $Rev: $
 *
 *
 * \addtogroup  DALIMessageHandler
 * @{
 *****************************************************************************/

// ensure only one instantiation
#ifndef _DALIMESSAGEHANDLER_PRM_H
#define _DALIMESSAGEHANDLER_PRM_H

// system includes ------------------------------------------------------------

// library includes -----------------------------------------------------------
#include "LEDManager/LedManager.h"

// local includes -------------------------------------------------------------

// Macros and Defines ---------------------------------------------------------
/// define the DALI version number
#define	DALI_VERSION_NUMBER               ( 1 )

/// define the physical minimum level
#define	DALI_PHYS_MINLEVEL                ( 25 )

/// define the enable debug macro
#define DALI_ENABLE_DEBUG                 ( 1 )

/// define the macro to disable default dim table
#define DALI_DISABLE_DFLT_DIMTABLE        ( 1 )

/// define the delay time between forward frame/backward frame
#define DALI_FORWARD_BACKWARD_DELAY_TIME  ( 4 )

/// define the GPIO pin to test for bus failure
#define DALI_BUS_STATUS                   ( GPIO_FUNC_ENUM_ILLEGAL )

/// define the muli-instantiation macro
#define DALI_MULTI_INSTANTIATION_ENABLE   ( 0 )
/**@} EOF DALIMessageHandler_prm.h */

#endif  // _DALIMESSAGEHANDLER_PRM_H
