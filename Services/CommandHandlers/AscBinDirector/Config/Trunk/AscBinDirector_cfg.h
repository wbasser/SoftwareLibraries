/******************************************************************************
 * @file AscBinDirector_cfg.h
 *
 * @brief Communications Director declarations 
 *
 * This file provides the declarations for the communications director
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
 * $Log: $
 * 
 *
 * \addtogroup AscBinDirector
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _ASCBINDIRECTOR_CFG_H
#define _ASCBINDIRECTOR_CFG_H

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "AscBinDirector/AscBinDirector_def.h"

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------
/// enumerate the directors
typedef enum _ASCBINDIRENUMS
{
  // add enumerations below
  
  // do not remove these entries
  ASCBINDIR_ENUM_MAX,
  ASCBINDIR_ENUM_ILLEGAL = 0xFF
} ASCBINDIRENUMS;

// structures -----------------------------------------------------------------

// global parameter declarations -----------------------------------------------
extern  const CODE ASCBINDEF  g_atAscBinDirectDefs[ ];

// global function prototypes --------------------------------------------------
extern  void  AscBinDirector_LocalInitialize( void );

/**@} EOF AscBinDirector_cfg.h */

#endif  // _ASCBINDIRECTOR_CFG_H