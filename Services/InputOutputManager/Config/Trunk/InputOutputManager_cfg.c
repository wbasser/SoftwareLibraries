/******************************************************************************
 * @file InputOutputManager_cfg.c
 *
 * @brief input output manager configuration implementation
 *
 * This file provides the implementation for the input/output manager configuration
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
 * \addtogroup InputOutputManager
 * @{
 *****************************************************************************/

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "InputOutputManager/InputOutputManager_cfg.h"

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations ----------------------------------------------

// local parameter declarations -----------------------------------------------

// local function prototypes --------------------------------------------------

// constant parameter initializations -----------------------------------------
const CODE IOMNGRCHNDEF  atIOMngrChnDefs[ IOMNGR_CHN_ENUM_MAX ] =
{
  // create entries using one of the below macros
  // IOMNGRDEFINP( pinenum, debounce, callback )
  // IOMNGRDEFRLY( pinenum )
  // IOMNGRDEFDCRLYLATCH( setpin, rstpin, drivetime )
  // IOMNGRDEFSCRLYLATCH( setpin, rstpin, drivetime )
  // IOMNGTRDEFMTXRELAY( mtxenum, setidx, rstidx, drivetime )
};

 const CODE IOMNGRMTXDEF  atIOMngrMtxDefs[ IOMNGR_MTX_ENUM_MAX ] =
 {
  // create entries using the below macro
  // IOMNGRDEFRLYMATRIX( mtxpin0, mtxpin1, mtxpin2, mtxpin3, mtxpin4, mtxpin5, mtxpin6, mtxpin7 )
 };
 
/**@} EOF InputOutputManager_cfg.c */
