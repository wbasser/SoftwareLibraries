/******************************************************************************
 * @file Sim.c
 *
 * @brief System Integration Module implementation
 *
 * This file provides the implementation for the System Integration Module (SIM)
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
 * \addtogroup Sim
 * @{
 *****************************************************************************/

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "SIM/Sim.h"

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations ----------------------------------------------

// local parameter declarations -----------------------------------------------

// local function prototypes --------------------------------------------------

// constant parameter initializations -----------------------------------------

/******************************************************************************
 * @function Sim_Initialize
 *
 * @brief SIM initialization
 *
 * This function will initialize the SIM module
 *
 *****************************************************************************/
void Sim_Initialize( void )
{
  // set up the clock gating registers
  SIM->SOPT1CFG = tSimDefOPT1Cfg.uValue;
  SIM->SOPT2    = tSimDefOPT2.uValue;
  SIM->SCGC4    = tSimDefCR4.uValue;
  SIM->SCGC5    = tSimDefCR5.uValue;
  SIM->SCGC6    = tSimDefCR6.uValue;
  SIM->SCGC7    = tSimDefCR7.uValue;
}
 
/**@} EOF Sim.c */
