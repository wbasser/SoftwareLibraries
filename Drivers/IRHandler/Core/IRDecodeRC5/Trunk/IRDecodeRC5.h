/******************************************************************************
 * @file IRDecodeRC5
 *
 * @brief IR decode RC5 format declarations
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
#ifndef _IRDECODERC5_H
#define _IRDECODERC5_H

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "IRHandler/IRHandler.h"

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations -----------------------------------------------

// global function prototypes --------------------------------------------------
extern  void  IRDecodeRC5_Initialize( void );
extern  U16   IRDecodeRC5_Process( PIRCAPVAL ptCapValue );

/**@} EOF IRDecodeRC5.h */

#endif  // _IRDECODERC5_H