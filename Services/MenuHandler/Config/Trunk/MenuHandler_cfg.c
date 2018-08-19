/******************************************************************************
 * @file MenuHandler_cfg.c
 *
 * @brief menu handler configuration implementation 
 *
 * This file provides the implementaiton for the configuralbe  portion of the
 * menu handler
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
 * \addtogroup MenuHandler
 * @{
 *****************************************************************************/

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "MenuHandler/MenuHandler_cfg.h"

// library includes -----------------------------------------------------------
#if ( MENUHANDLER_USE_PARAMETERS == 1 )
#include "ParameterManager/ParameterManager.h"
#endif // MENUHANDLER_USE_PARAMETERS

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations ----------------------------------------------

// local parameter declarations -----------------------------------------------

// local function prototypes --------------------------------------------------

// constant parameter initializations -----------------------------------------
/// define the top level menu
const CODE MENUENTRY tTopLevelMenu =
{
};

/******************************************************************************
 * @function 
 *
 * @brief 
 *
 * This function 
 *
 * @param[in]   
 *
 *****************************************************************************/
void MenuHandler_DisplayLine( U8 nRow, U8 nCol, PC8 pszMessage )
{
}

/******************************************************************************
 * @function 
 *
 * @brief 
 *
 * This function 
 *
 *****************************************************************************/
void MenuHandler_DisplayClear( void )
{
}

#if ( MENUHANDLER_USE_PARAMETERS == 1 )
/******************************************************************************
 * @function 
 *
 * @brief 
 *
 * This function 
 *
 * @param[in]   
 *
 *****************************************************************************/
U32 MenuHandler_GetParameter( PARAMENUM eParam )
{
}

/******************************************************************************
 * @function 
 *
 * @brief 
 *
 * This function 
 *
 * @param[in]   
 *
 * @return      
 *
 *****************************************************************************/
void MenuHandler_PutParameter( U8 nParamNum, U32 uValue )
{
}

#endif // MENUHANDLER_USE_PARAMETERS

/**@} EOF MenuHandler_cfg.c */
