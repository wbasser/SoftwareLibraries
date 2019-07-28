/******************************************************************************
 * @file Micro.h
 *
 * @brief Micro include files
 *
 * This file provides the declarations for the micro functions
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
 * \addtogroup Micro
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _MICRO_H
#define _MICRO_H

// local includes -------------------------------------------------------------
#include "Micro_cfg.h"

// library includes
#if ( MICRO_ENABLE_SYSTIMECMDS == 1 )
#include "AsciiCommandHandler/AsciiCommandHandler.h"
#endif // MICRO_ENABLE_SYSTIMECMDS

// global paramter declarations  ------------------------------------------------
#if ( MICRO_ENABLE_SYSTIMECMDS  == 1 )
extern  const CODE ASCCMDENTRY g_atMicroCmdHandlerTable[ ];
#endif // MICRO_ENABLE_SYSTIMECMDS

// global function prototypes --------------------------------------------------
extern  void  Micro_Shutdown( void );

/**@} EOF Micro.h */

#endif  // _MICRO_H