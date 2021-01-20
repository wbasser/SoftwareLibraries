/******************************************************************************
 * @file QueueManager_cfg.c
 *
 * @brief queue manager configuration implementation
 *
 * This file contains the list of queue configurations
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
 * \addtogroup 
 * @{
 *****************************************************************************/

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "QueueManager/QueueManager_cfg.h"

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations ----------------------------------------------
/// allocate the buffer storage for each queue
// QUEUE_BUFFER( name, entry_size, num_entries );


/// create entry for each queue
const CODE QUEUEDEF  g_atQueueDefs[ QUEUE_ENUM_MAX ] =
{
  // QUEUE_ENTRY( task, entry_size, num_entries, name, empflg, putflg, getflg, fullflg ),
};



/**@} EOF QueueManager_cfg.c */
