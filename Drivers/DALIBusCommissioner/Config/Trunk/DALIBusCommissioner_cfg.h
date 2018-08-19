/******************************************************************************
 * @file DALIBusCommissioner_cfg.h
 *
 * @brief DALI Bus commissioner configuration declarations 
 *
 * This file provides the declarations for the bus commissioner
 *
 * @copyright Copyright (c) 2012CyberIntegration
 * This document contains proprietary data and information ofCyberIntegration 
 * LLC. It is the exclusive property ofCyberIntegration, LLC and will not be 
 * disclosed in any form to any party without prior written permission of 
 *CyberIntegration, LLC. This document may not be reproduced or further used 
 * without the prior written permission ofCyberIntegration, LLC.
 *
 * $Date: $
 *
 * Version Histor
 * ======
 * $Rev: $
 * 
 *
 * \addtogroup DALIBusCommissioner
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _DALIBUSCOMMISSIONER_CFG_H
#define _DALIBUSCOMMISSIONER_CFG_H

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "DALIBusCommissioner/DALIBusCommissioner_prm.h"

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations -----------------------------------------------

// global function prototypes --------------------------------------------------
extern  U8    DALIBusCommissioner_GetDeviceTableEntry( U8 nIndex );
extern  void  DALIBusCommissioner_SetDeviceTableEntry( U8 nIndex, U8 nStatus );
extern  U8    DALIBusCommissioner_GetDeviceCount( void );
extern  void  DALIBusCommissioner_PutMessage( U8 nAddress, U8 nData );
extern  void  DALIBusCommissioner_GetMessage( PU8 pnStatus, PU8 pnData );
extern  void  DALIBusCommissioner_CommissionStartStop( BOOL bState, U8 nOption );

/**@} EOF DALIBusCommissioner_cfg.h */

#endif  // _DALIBUSCOMMISSIONER_CFG_H
