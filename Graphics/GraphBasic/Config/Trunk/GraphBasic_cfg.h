/******************************************************************************
 * @file GraphBasic_cfg.h
 *
 * @brief	graphics basic configuraiton declarations 
 *
 * This file provides the declarations for the graph basic
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
 * \addtogroup GraphBasic_cfg
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _GRAPHBASIC_CFG_H
#define _GRAPHBASIC_CFG_H

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------

// library includes -----------------------------------------------------------
#include "Types/Types.h"

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations -----------------------------------------------

// global function prototypes --------------------------------------------------
extern	void	GraphBasic_LocalInitialize( void );
extern  void  GraphBasic_RefreshScreen( void );
extern  U16   GraphBasic_GetMaxX( void );
extern  U16   GraphBasic_GetFontX( void );

/**@} EOF GraphBasic_cfg.h */

#endif  // _GRAPHBASIC_CFG_H