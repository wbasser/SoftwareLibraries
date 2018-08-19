/******************************************************************************
 * @file Watchdog.c
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
 * \addtogroup Watchdog
 * @{
 *****************************************************************************/

// system includes ------------------------------------------------------------

// library includes -----------------------------------------------------------
#include "Micro/Types.h"

// local includes -------------------------------------------------------------
#include "Watchdog/Watchdog.h"
#include "Watchdog/Watchdog_prm.h"

// Macros and Defines ---------------------------------------------------------
/// define the watchdog frequency in hz
#define WATCHDOG_FREQ     ( 32768 )

/// define the  watchdog magic word
#define WATCHDOG_MAGIC    ( 0xABADCAFE );

/// define the clock delay count
#define CLK002_DELAYCNT                (0x3FFU)

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations ----------------------------------------------

// local parameter declarations -----------------------------------------------
static  U32 uLoCounts, uHiCounts;

// local function prototypes --------------------------------------------------

// constant parameter initializations -----------------------------------------

/******************************************************************************
 * @function Watchdog_Initialize
 *
 * @brief initialize the watchdog
 *
 * This function will enable the clock to the watchdog, set the boundaries and
 * enable the watchdog
 *
 *****************************************************************************/
void Watchdog_Initialize( void )
{
  if ( !(( U32 )SCU_CLK->CGATCLR0 & ( U32 )SCU_CLK_CGATCLR0_WDT_Pos ))
  { 
    SCU_GENERAL->PASSWD = 0x000000C0UL;
    WR_REG( SCU_CLK->CLKCR, SCU_CLK_CLKCR_CNTADJ_Msk, SCU_CLK_CLKCR_CNTADJ_Pos, CLK002_DELAYCNT );
    SET_BIT( SCU_CLK->CGATCLR0, SCU_CLK_CGATCLR0_WDT_Pos );
    while ((uint32_t)SCU_CLK->CLKCR & (uint32_t)SCU_CLK_CLKCR_VDDC2LOW_Msk);
    SCU_GENERAL->PASSWD = 0x000000C3UL;
   }

  // calculate the boundary values
  uLoCounts = ( WATCHDOG_LO_BOUNDARY * 1000 ) / ( 1000000 / WATCHDOG_FREQ );
  uHiCounts = ( WATCHDOG_HI_BOUNDARY * 1000 ) / ( 1000000 / WATCHDOG_FREQ );
  
  // now store the lo/hi boundary
  WDT->WLB = uLoCounts;
  WDT->WUB = uHiCounts;

  // now enable the watchdog
  #if ( WATCHDOG_ENABLE == 1 )
  WDT->CTR = WDT_CTR_ENB_Msk | WDT_CTR_DSP_Msk;
  #endif
}

/******************************************************************************
 * @function Watchdog_Update
 *
 * @brief wathdog update
 *
 * This function writes the magic word to the watchdog tiemr
 *
 *****************************************************************************/
void Watchdog_Update( void )
{
  // now write the magic word to the watchdog
  WDT->SRV = WATCHDOG_MAGIC;
}

/**@} EOF Watchdog.c */
