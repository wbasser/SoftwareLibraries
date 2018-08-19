/******************************************************************************
 * @file AfeLMP9100X.h
 *
 * @brief Analog Front End LMP9100X declarations
 *
 * This file provides the declaration for the Analog Front End LMP9100X
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
 * \addtogroup AfeLMP9100X
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _AFELMP9100X_H
#define _AFELMP9100X_H

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "AfeLMP9100X/AfeLMP9100X_cfg.h"

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------
/// enumerate the error messages
typedef enum _AFELMP9100XERR
{
  AFELMP9100X_ERR_NONE = 0,           ///< no error detected
  AFELMP9100X_ERR_ILLDEV,             ///< illegal device enumeration
  AFLEMP9100X_ERR_INITFAIL,           ///< failure on initialization
  AFLEMP9100X_ERR_MODEFAIL,           ///< setting mode failed
  AFLEMP9100X_ERR_ILLMODE,            ///< illegal mode
} AFELMP9100XERR;

// structures -----------------------------------------------------------------

// global parameter declarations -----------------------------------------------

// global function prototypes --------------------------------------------------
extern  void            AfeLMP9100X_Initialize( void );
extern  AFELMP9100XERR  AfeLMP9100X_SetConfig( AFELMP9100ENUM eDevice, PAFELMP9100XCFGDEF ptCfgDef );
extern  AFELMP9100XERR  AfeLMP9100X_SetOperationalMode( AFELMP9100ENUM eDevice, AFELMP9100XOPMODE eMode );

/**@} EOF AfeLMP9100X.h */

#endif  // _AFELMP9100X_H