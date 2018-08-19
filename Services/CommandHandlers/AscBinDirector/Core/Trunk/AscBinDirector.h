/******************************************************************************
 * @file AscBinDirector.h
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
#ifndef _ASCBINDIRECTOR_H
#define _ASCBINDIRECTOR_H

// system includes ------------------------------------------------------------

// library includes -----------------------------------------------------------

// local includes -------------------------------------------------------------
#include "CommandHandlers/AscBinDirector/AscBinDirector_cfg.h"

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations -----------------------------------------------

// global function prototypes --------------------------------------------------
extern  void      AscBinDirector_Initialize( void );
extern  void      AscBinDirector_ProcessChar( ASCBINDIRENUMS eEnum, U8 nChar, U8 nCompareValue );
extern  ASCBINSEL AscBinDirector_GetSelection( void );
extern  void      AscBinDirector_SetSelection( ASCBINSEL eSel );

/**@} EOF AscBinDirector.h */

#endif  // _ASCBINDIRECTOR_H