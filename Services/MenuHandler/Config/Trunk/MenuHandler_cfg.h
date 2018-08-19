/******************************************************************************
 * @file MenuHandler_cfg.h
 *
 * @brief Menu handler configuration declarations 
 *
 * This file provides the declarations for the configuration of the menu handler
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
 * \addtogroup MenuHandler_cfg
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _MENUHANDLER_CFG_H
#define _MENUHANDLER_CFG_H

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "MenuHandler/MenuHandler_prm.h"
#include "MenuHandler/MenuHandler_def.h"

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations -----------------------------------------------
extern  const CODE MENUENTRY    tTopLevelMenu;

// global function prototypes --------------------------------------------------
extern  void  MenuHandler_DisplayLine( U8 nRow, U8 nCol, PC8 pszMessage );
extern  void  MenuHandler_DisplayClear( void );
#if ( MENUHANDLER_USE_PARAMETERS == 1 )
extern  U32   MenuHandler_GetParameter( U8 nParamNum );
extern  void  MenuHandler_PutParameter( U8 nParamNum, U32 uValue );
#endif // MENUHANDLER_USE_PARAMETERS

/**@} EOF MenuHandler_cfg.h */

#endif  // _MENUHANDLER_CFG_H