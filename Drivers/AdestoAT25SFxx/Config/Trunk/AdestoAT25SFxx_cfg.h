/******************************************************************************
 * @file AdestoAT25SFxx_cfg.h
 *
 * @brief Adesto AT25SFxx serial memory configuration declarations 
 *
 * This file provides the configuration declarations for the AT25SFxx serial 
 * memory
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
#ifndef _ADESTOAT25SFXX_CFG_H
#define _ADESTOAT25SFXX_CFG_H

// system includes ------------------------------------------------------------
#include "Types/Types.h"

// local includes -------------------------------------------------------------
#include "AdestoAT25SFxx/AdestoAT25SFxx_prm.h"

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations -----------------------------------------------

// global function prototypes --------------------------------------------------
extern	void	AdestoAT25SFxx_LocalInitialize( void );
extern	void	AdestoAT25SFxx_ChipSelect( BOOL bState );
extern	void	AdestoAT25SFxx_SpiRead( PU8 pnBuffer, U16 wLength );
extern	void	AdestoAT25SFxx_SpiWrite( PU8 pnBuffer, U16 wLength );

/**@} EOF AdestoAT25SFxx_cfg.h */

#endif  // _ADESTOAT25SFXX_CFG_H