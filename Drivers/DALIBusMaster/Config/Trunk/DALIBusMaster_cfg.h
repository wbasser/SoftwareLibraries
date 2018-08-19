/******************************************************************************
 * @file DALIBusMaster_cfg.h  
 *
 * @brief DALIBus master configuration declarations
 *
 * This file provides the declarations for the configurable portion of the
 * DALI Bus Master
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
 * $Rev: $
 * 
 *
 * \addtogroup DALIBusMaster
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _DALIBUSMASTER_CFG_H
#define _DALIBUSMASTER_CFG_H

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------

// library includes -----------------------------------------------------------
#include "Types/Types.h"

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations -----------------------------------------------

// global function prototypes --------------------------------------------------
extern  void  DALIBusMaster_Transmit( PU8 pnBuffer, U8 nLength );
extern  void  DALIBusMaster_Receive( PU8 pnBuffer, U8 nLength );
extern  void  DALIBusMaster_StopReceive( void );
extern  void  DALIBusMaster_PutDeviceStatus( PDALIBUSMASTERDEVSTATUS ptStatus );
extern  void  DALIBusMaster_GetDeviceStatus( PDALIBUSMASTERDEVSTATUS ptStatus );

/**@} EOF DALIBusMaster_cfg.h */

#endif  // _DALIBUSMASTER_CFG_H