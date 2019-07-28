/******************************************************************************
 * @file AdestoAT25SFxx.h
 *
 * @brief	Adesto AT25SFxx declarations 
 *
 * This file provides the declarations for the Adesto AT25SFxx series of
 * serial flash memory devices
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
 * \addtogroup AdestoAT25SFxx
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _ADESTOAT25SFXX_H
#define _ADESTOAT25SFXX_H

// system includes ------------------------------------------------------------
#include "Types/Types.h"

// local includes -------------------------------------------------------------
#include "AdestoAT25SFxx/AdestoAT25SFxx_cfg.h"

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------
/// enumerate the error codes
typedef enum _ADESTOAT25SERR
{
  ADESTOAT25S_ERR_NONE = 0,
  ADESTOAT25S_ERR_ILLADDR,
  ADESTOAT25S_ERR_ILLPAGESIZE,
  ADESTOAT25S_ERR_ILLERASETYPE,
  ADESTOATF25S_ERR_TIMEOUT,
} ADESTOAT25SERR;

/// enumerat the erase type
typedef enum _ADESTOAT24SERASE
{
  ADESTOAT24S_ERASE_BLOCK4K = 0,
  ADESTOAT24S_ERASE_BLOCK32K,
  ADESTOAT24S_ERASE_BLOCK64K,
  ADESTOAT24S_ERASE_CHIP,
  ADESTOAT24S_ERASE_MAX
} ADESTOAT24SERASE;

// structures -----------------------------------------------------------------

// global parameter declarations -----------------------------------------------

// global function prototypes --------------------------------------------------
extern	void            AdestoAT25SFxx_Initialize( void );
extern  ADESTOAT25SERR  AdestoAT25SFxx_ReadDevId( PU8 pnManufId, PU8 pnDevID1, PU8 pnDevID2 );
extern	ADESTOAT25SERR	AdestoAT25SFxx_Read( U32 uAddress, PU8 pnData, U16 wLength );
extern	ADESTOAT25SERR	AdestoAT25SFxx_Write( U32 uAddress, PU8 pnData, U16 wLength );
extern	ADESTOAT25SERR	AdestoAT25SFxx_Erase( ADESTOAT24SERASE eEraseType, U32 uAddress );

/**@} EOF AdestoAT25SFxx.h */

#endif  // _ADESTOAT25SFXX_H