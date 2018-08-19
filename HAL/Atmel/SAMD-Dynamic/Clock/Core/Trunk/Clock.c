/******************************************************************************
 * @file Clock.c
 *
 * @brief clock module implementation
 *
 * This file clock module initialization and up/down scaling
 *
 * @copyright Copyright (c) 2012 CyberIntegration
 * This document contains proprietary data and information of CyberIntegration 
 * LLC. It is the exclusive property of CyberIntegration, LLC and will not be 
 * disclosed in any form to any party without prior written permission of 
 * CyberIntegration, LLC. This document may not be reproduced or further used 
 * without the prior written permission of CyberIntegration, LLC.
 *
 * Version History
 * ======
 * Rev: $
 * 
 *
 * \addtogroup Clock
 * @{
 *****************************************************************************/

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "Clock/Clock.h"

// library includes -----------------------------------------------------------
#include "Interrupt/Interrupt.h"

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations ----------------------------------------------

// local parameter declarations -----------------------------------------------
static  U32   uSystemClockFreq;

// local function prototypes --------------------------------------------------

/******************************************************************************
 * @function Clock_Initialization
 *
 * @brief clock initialization
 *
 * This function will initialize the clock control module
 *
 * @param[in]     nFlashWaitStates  flash states
 * @param[in]     eCpuDiv           CPU cllock divisor
 * @parma[in]     eApbADiv          APB A divisor
 * @param[in]     eAbbBDiv          APB B divisor
 *
 *****************************************************************************/
void Clock_Initialize( CLOCKSRC eClkSrc, U8 nFlashWaitStates, CLOCKMAINDIV eCpuDiv, CLOCKMAINDIV eApbADiv, CLOCKMAINDIV eApbBDiv )
{
  U32 uCoarse, uFine;

  // now configure the flash wait states/clock divides
  NVMCTRL->CTRLB.bit.RWS = nFlashWaitStates;
  
  // configure the CPU/APBA/APBB dividers
  PM->CPUSEL.reg = eCpuDiv;
  PM->APBASEL.reg = eApbADiv;
  PM->APBBSEL.reg = eApbBDiv;
    
  // perform a software reset
  GCLK->CTRL.reg = GCLK_CTRL_SWRST;
  while( GCLK->CTRL.reg & GCLK_CTRL_SWRST );

  // determine clock source
  switch( eClkSrc )
  {
    case CLOCK_SRC_OSC8M :
      // clear the oscilator prescale
      SYSCTRL->OSC8M.bit.PRESC = 0;

      // set the system clock
      uSystemClockFreq = 8000000l;
      break;

    case CLOCK_SRC_DFLL48M :
      // get the course and fine DLL calibaration values
      uCoarse =  *(( PU32 )( FUSES_DFLL48M_COARSE_CAL_ADDR )) >> FUSES_DFLL48M_COARSE_CAL_Pos;
      uFine = *(( PU32 )( FUSES_DFLL48M_FINE_CAL_ADDR )) & FUSES_DFLL48M_FINE_CAL_Msk;

      // clear the DFLL control
      SYSCTRL->DFLLCTRL.reg = 0; // See Errata 9905
      while ( !( SYSCTRL->PCLKSR.reg & SYSCTRL_PCLKSR_DFLLRDY ));

      // now set the muliiplier and cal values
      SYSCTRL->DFLLMUL.reg = SYSCTRL_DFLLMUL_MUL( 48000 );
      SYSCTRL->DFLLVAL.reg = SYSCTRL_DFLLVAL_COARSE( uCoarse) | SYSCTRL_DFLLVAL_FINE( uFine );

      // enable it and wait till ready
      SYSCTRL->DFLLCTRL.reg = SYSCTRL_DFLLCTRL_ENABLE | SYSCTRL_DFLLCTRL_USBCRM | SYSCTRL_DFLLCTRL_BPLCKC | SYSCTRL_DFLLCTRL_CCDIS | SYSCTRL_DFLLCTRL_STABLE;
      while ( !(SYSCTRL->PCLKSR.reg & SYSCTRL_PCLKSR_DFLLRDY ));

      // set the system clock
      uSystemClockFreq = 48000000l;
      break;

    default :
      break;
  }

  // set the GenCLK 0 to have the DFLL/wait till synced
  GCLK->GENCTRL.reg = GCLK_GENCTRL_ID( 0 ) | GCLK_GENCTRL_SRC( eClkSrc ) | GCLK_GENCTRL_RUNSTDBY | GCLK_GENCTRL_GENEN;
  while ( GCLK->STATUS.reg & GCLK_STATUS_SYNCBUSY );
}

/******************************************************************************
 * @function Clock_GetFreq
 *
 * @brief get the current clock frequency
 *
 * This function will return the current clock frequency
 *
 * @return     the frequency of the main clock
 *
 *****************************************************************************/
U32 Clock_GetFreq( )
{
  // GetSystemClockSource
  return( uSystemClockFreq );
}

/******************************************************************************
 * @function Clock_PereiphEnable
 *
 * @brief peripheral clock enable
 *
 * This function will enable and select the clock generator for a clock mux
 *
 * @param[in]     eMuxId            multiplexer ID
 * @parma[in]     eGenId            generator ID
 *
 *****************************************************************************/
void Clock_PeriphEnable( CLOCKMUXID eMuxId, CLOCKGENID eGenId )
{
  GCLK_CLKCTRL_Type tCtl;
  
  // set the bits
  tCtl.reg = 0;
  tCtl.bit.ID = eMuxId;
  tCtl.bit.CLKEN = TRUE;
  tCtl.bit.GEN = eGenId;
  tCtl.bit.WRTLOCK = FALSE;
  GCLK->CLKCTRL.reg = tCtl.reg;
}

/******************************************************************************
 * @function Clock_SetGenClock
 *
 * @brief set a genral clock generator
 *
 * This function will enable and select the clock generator for a clock mux
 *
 * @parma[in]     eGenId            generator ID
 * @param[in]     eClockSrc         clock source
 * @param[in]     wDivisor          divisor
 * @param[in]     eDivSel           divisor select
 *
 *****************************************************************************/
void Clock_SetGenClock( CLOCKGENID eGenId, CLOCKSRC eClockSrc, U16 wDivisor, CLOCKDIVSEL eDivSel )
{
  U32 uTemp;

  // set up the divide
  uTemp = GCLK_GENDIV_ID( eGenId );
  uTemp |= GCLK_GENDIV_DIV( wDivisor );
  GCLK->GENDIV.reg = uTemp;

  // set the bits
  uTemp = GCLK_GENCTRL_ID( eGenId );
  uTemp |= GCLK_GENCTRL_SRC( eClockSrc );
  uTemp |= GCLK_GENCTRL_GENEN;
  uTemp |= eDivSel;
  GCLK->GENCTRL.reg = uTemp;
  while( GCLK->STATUS.bit.SYNCBUSY );
}

/**@} EOF Clock.c */
