/******************************************************************************
 * @file AscBinDirector_cfg.c
 *
 * @brief communications director implementation
 *
 * This file provides the implementation for the communications director
 *
 * @copyright Copyright (c) 2012 CyberIntegration
 * This document contains proprietary data and information of Cyber
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
 * \addtogroup AscBinDirector
 * @{
 *****************************************************************************/

// system includes -----------------------------------------------------------

// local includes -------------------------------------------------------------
#include "AscBinDirector/AscBinDirector.h"

// library includes -----------------------------------------------------------
#include "SystemDefines/SystemDefines_prm.h"

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations ----------------------------------------------

// local parameter declarations -----------------------------------------------

// local function prototypes --------------------------------------------------

// constant parameter initializations -----------------------------------------
const CODE ASCBINDEF  g_atAscBinDirectDefs[ ASCBINDIR_ENUM_MAX ] =
{
  // add entries to the table here
  // ASCBINENTRY( ascprot, binprot )
};

/******************************************************************************
 * @function AAscBinDirector_LocalInitialize
 *
 * @brief local initialization
 *
 * This function will perform any local dependent initialization
 *
 *****************************************************************************/
void AscBinDirector_LocalInitialize( void )
{
}


/**@} EOF CommDirector.c */
