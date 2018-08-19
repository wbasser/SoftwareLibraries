/******************************************************************************
 * @file Prng.h
 *
 * @brief 
 *
 * This file 
 *
 * @copyright Copyright (c) 2012 Nan Technologies
 * This document contains proprietary data and information of Nan Technologies 
 * LLC. It is the exclusive property of Nan Technologies, LLC and will not be 
 * disclosed in any form to any party without prior written permission of 
 * Nan Technologies, LLC. This document may not be reproduced or further used 
 * without the prior written permission of Nan Technologies, LLC.
 *
 * Version History
 * ======
 * $Rev: $
 * 
 *
 * \addtogroup Prng
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _PRNG_H
#define _PRNG_H

// system includes ------------------------------------------------------------

// library includes -----------------------------------------------------------
#include "Micro/Types.h"

// local includes -------------------------------------------------------------

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations -----------------------------------------------

// global function prototypes --------------------------------------------------
extern  void  Prng_Initialize( void );
extern  void  Prng_Seed( U32UN tSeed );
extern  U8    Prng_Generate( void );

/**@} EOF Prng.h */

#endif  //_PRNG_H