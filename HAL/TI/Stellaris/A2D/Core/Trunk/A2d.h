/******************************************************************************
 * @file A2D.h
 *
 * @brief A2D declarations
 *
 * This file provides the declarations for the A2D devices
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
 * \addtogroup A2D
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _A2D_H
#define _A2D_H

// system includes ------------------------------------------------------------

// library includes -----------------------------------------------------------

// local includes -------------------------------------------------------------
#include "A2D/A2d_cfg.h"

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------
/// enumerate the error values
typedef enum _A2DERR
{
  A2D_ERR_NONE = 0,         ///< no error
  A2D_ERR_ILLDEV,           ///< illegal device
  A2D_ERR_ILLPER,           ///< illegal peripheral
  A2D_ERR_ILLACT,           ///< illegal IOCTL action
} A2DERR;

/// enumerate the IOCTL actions
typedef enum _A2DACTION
{
  A2D_ACTION_NONE = 0,      ///< no action
  A2D_ACTION_MAX            ///< maximum action selection
} A2DACTION;

// structures -----------------------------------------------------------------

// global parameter declarations -----------------------------------------------

// global function prototypes --------------------------------------------------
extern  void    A2d_Initialize( void  );
extern  A2DERR  A2d_Convert( A2DDEVENUM eDev, PU16 pwData );
extern  A2DERR  A2d_Ioctl( A2DDEVENUM eDev, A2DACTION eAction, PVOID pvData );
extern  void    A2d_Close( void );

/**@} EOF A2d.h */

#endif  // _A2D_H