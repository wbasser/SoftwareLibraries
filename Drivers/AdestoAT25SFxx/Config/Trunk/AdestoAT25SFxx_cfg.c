/******************************************************************************
 * @file AdestoAT25SFxx_cfg.c
 *
 * @brief Adesto AT25SFxx configuration file implementation
 *
 * This file provides the implementation for the Adesto AT25SFxx serial
 * memories
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

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "AdestoAT25SFxx/AdestoAT25SFxx_cfg.h"

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations ----------------------------------------------

// local parameter declarations -----------------------------------------------

// local function prototypes --------------------------------------------------

// constant parameter initializations -----------------------------------------

/******************************************************************************
 * @function AdestoAT25SFxx_LocalInitialize
 *
 * @brief local initilaization
 *
 * This function will perform any needed local initialization
 *
 *****************************************************************************/
void AdestoAT25SFxx_LocalInitialize( void )
{
}

/******************************************************************************
 * @function AdestoAT25SFxx_ChipSelect
 *
 * @brief chip select
 *
 * This function will enable/disable the chip select based on state
 *
 * @param[in]   bState			desired state
 *
 *****************************************************************************/
void AdestoAT25SFxx_ChipSelect( BOOL bState )
{
}

/******************************************************************************
 * @function AdestoAT25SFxx_SpiRead
 *
 * @brief SPI read
 *
 * This function will read from the desired SPI device
 *
 * @param[in]   pnBuffer		pointer to the data to write
 * @param[in]		wLength			length of the data to write
 *
 *****************************************************************************/
void AdestoAT25SFxx_SpiRead( PU8 pnBuffer, U16 wLength )
{
}


/******************************************************************************
 * @function AdestoAT25SFxx_SpiWrite
 *
 * @brief SPI write
 *
 * This function will write to the desired SPI device
 *
 * @param[in]   pnBuffer		pointer to the data to write
 * @param[in]		wLength			length of the data to write
 *
 *****************************************************************************/
void AdestoAT25SFxx_SpiWrite( PU8 pnBuffer, U16 wLength )
{
}

/**@} EOF AdestoAT25SFxx_cfg.c */
