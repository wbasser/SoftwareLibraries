/******************************************************************************
 * @file IRDecodeNEC
 *
 * @brief IR decode NEC declarations
 *
 * This file will decode the IR capture values
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
 * \addtogroup IR 
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _IRDECODENEC_H
#define _IRDECODENEC_H

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "IRHandler/IRHandler.h"

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations -----------------------------------------------

// global function prototypes --------------------------------------------------
extern  void  IRDecodeNEC_Initialize( void );
extern  U16   IRDecodeNEC_Process( PU32UN ptCapValue );

/**@} EOF IRDecodeNEC.h */

#endif  // _IRDECODENEC_H