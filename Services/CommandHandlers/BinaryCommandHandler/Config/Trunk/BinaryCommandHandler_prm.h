/******************************************************************************
 * @file BinaryCommandHandler_prm.h
 *
 * @brief binary command handler parameter declarations
 *
 * This file provides the parameter declarations for the binary command handler
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
 * $Log: $
 * 
 *
 * \addtogroup BinaryCommandHandler
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _BINARYCOMMANDHANDLER_PRM_H
#define _BINARYCOMMANDHANDLER_PRM_H

// system includes ------------------------------------------------------------

// Macros and Defines ---------------------------------------------------------
/// define the macro to enable master mode
#define BINCMDHAND_ENABLE_MASTERMODE            ( 0 )

/// define the enable debug macro
#define BINARYCOMMANDHANDLER_ENABLE_DEBUG       ( 0 )

/// define the debug base value
#define BINARYCOMMANDHANDLER_DEBUG_BASE         ( 0x9900 )

/// define the number of nested command tables
#define BINARYCOMMANDHANDLER_TABLE_STACK_DEPTH  ( 4 )

/**@} EOF BinaryCommandHandler_prm.h */

#endif  // _BINARYCOMMANDHANDLER_PRM_H