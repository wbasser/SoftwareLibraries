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
static  U32   uMainOscFreq;

// local function prototypes --------------------------------------------------
static  void  SetupMasterOscillator( void );
static  void  SetupPlls( void );
static  void  SetupProgClocks( void );
static  U32   GetPllFreq( CLOCKPLLSEL ePllSel );

/******************************************************************************
 * @function Clock_Initialization
 *
 * @brief clock initialization
 *
 * This function will initialize the clock control module
 *
 *****************************************************************************/
void Clock_Initialize( void )
{
  U32 uValue;
  U8  nWaitStates;

  // clear 
  uValue = 0;
  
  // set the flash wait states
  #ifdef EFC0
  EFC0->EEFC_FMR = EEFC_FMR_FWS( 4 );
  #endif // EFCO
  #ifdef EFC1
  EFC1->EEFC_FMR = EEFC_FMR_FWS( 4 );
  #endif // EFC1
  #ifdef EFC
  EFC->EEFC_FMR = EEFC_FMR_FWS( 5 ) | EEFC_FMR_CLOE;
  #endif
  
  // set up the slow clock
  if (tClockSrcDef.eSlowClkSrc == CLOCK_SLOW_XOSC32K )
  {
    // turn on the external slow clock
    SUPC->SUPC_MR |= ( SUPC_MR_OSCBYPASS_BYPASS | SUPC_MR_KEY_PASSWD );
    SUPC->SUPC_CR = ( SUPC_CR_XTALSEL_CRYSTAL_SEL | SUPC_CR_KEY_PASSWD );
  }
  
  // set up the defined PLLS
  SetupPlls( );
  
  // based on clock selection
  switch( tClockMasterDef.eClockSource )
  {
    case CLOCK_MASTER_SCLK :
      // set the chip select
      uValue = PMC_MCKR_CSS_SLOW_CLK;
      
      // set the clock frequency
      uSystemClockFreq = CHIP_FREQ_XTAL_32K;
      break;
      
    case CLOCK_MASTER_MAINCK :
      // set up the master clock
      SetupMasterOscillator( );
      uSystemClockFreq = uMainOscFreq;
      uValue = PMC_MCKR_CSS_MAIN_CLK;
      break;
      
    case CLOCK_MASTER_PLLACK :
      // set up the master clock
      SetupMasterOscillator( );

      // set up PLLA
      uSystemClockFreq = GetPllFreq( CLOCK_PLL_SEL_A );
      uSystemClockFreq /= ( tClockMasterDef.bDiv2PLLA ) ? 2 : 1;
      uValue = PMC_MCKR_CSS_PLLA_CLK ;
      break;
      
    #ifdef REG_CKGR_PLLBR
    case CLOCK_MASTER_PLLBCK :
      // set up the master clock
      SetupMasterOscillator( );

      // set up PLLB
      uSystemClockFreq = GetPllFreq( CLOCK_PLL_SEL_B );
      uSystemClockFreq /= ( tClockMasterDef.bDiv2PLLB ) ? 2 : 1;
      uValue = PMC_MCKR_CSS_PLLB_CLK ;
      break;
    #endif // REG_CKGR_PLLBR
    
    default :
      uValue = 0;
      break;
  }
  
  // now set the prescale
  uValue |= BITS( tClockMasterDef.eClockPrescale, PMC_MCKR_PRES_Pos );
  uValue |= ( tClockMasterDef.bDiv2PLLA ) ? PMC_MCKR_PLLADIV2 : 0;
  #ifdef REG_CKGR_PLLBR
  uValue |= ( tClockMasterDef.bDiv2PLLB ) ? PMC_MCKR_PLLBDIV2 : 0;
  #endif // REG_CKGR_PLLBR
  
  // now write it/wait for ready
  PMC->PMC_MCKR = uValue;
  while( !( PMC->PMC_SR & PMC_SR_MCKRDY ));
  
  // configure the peogrammable clocks
  SetupProgClocks( );
  
  // now set up the flash wait states based on frequency
  if ( uSystemClockFreq < CHIP_FREQ_FWS_0 )
  {
    nWaitStates= 0;
  }
  else if ( uSystemClockFreq < CHIP_FREQ_FWS_1 )
  {
    nWaitStates= 1;
  }
  else if ( uSystemClockFreq < CHIP_FREQ_FWS_2 )
  {
    nWaitStates= 2;
  }
  else if ( uSystemClockFreq < CHIP_FREQ_FWS_3 )
  {
    nWaitStates= 3;
  }
  else if ( uSystemClockFreq < CHIP_FREQ_FWS_4 )
  {
    nWaitStates= 4;
  }
  else
  {
    nWaitStates= 5;
  }

  // now write the wait states
  #ifdef EFC0
  EFC0->EEFC_FMR = EEFC_FMR_FWS( nWaitStates );
  #endif // EFCO
  #ifdef EFC1
  EFC1->EEFC_FMR = EEFC_FMR_FWS( nWaitStates );
  #endif //EFC1
  #ifdef EFC
  EFC->EEFC_FMR = EEFC_FMR_FWS( nWaitStates ) | EEFC_FMR_CLOE;
  #endif // EFC
}

/******************************************************************************
 * @function Clock_Close
 *
 * @brief close the clock
 *
 * This function will close the clock
 *
 *****************************************************************************/
void Clock_Close( void )
{
}

/******************************************************************************
 * @function Clock_GetFreq
 *
 * @brief get the current clock frequency
 *
 * This function will return the current clock frequency
 *
 *  @return     the frequency of the main clock
 *
 *****************************************************************************/
U32 Clock_GetFreq( void )
{
  // GetSystemClockSource
  return( uSystemClockFreq );
}

/******************************************************************************
 * @function SetupMasterOscilator
 *
 * @brief setup the master oscillator
 *
 * This function will setup the master oscillator
 *
 * @return    freqency of the osccillator
 *
 *****************************************************************************/
static void SetupMasterOscillator( void )
{
  U32   uValue, uOscFreq;
  
  // get the value of the current register/set the key
  uValue = REG_CKGR_MOR;
  uValue |= CKGR_MOR_KEY_PASSWD;
  
  // determine source
  switch( tClockSrcDef.eMainCLkSrc )
  {
    case CLOCK_MAIN_FASTRC :
      // set up the divider
      uValue &= ~CKGR_MOR_MOSCRCF_Msk;
      uValue |= BITS( tClockSrcDef.eFastOscSel, CKGR_MOR_MOSCRCF_Pos );
      
      // write it
      PMC->CKGR_MOR = uValue;

      // set the clock frequency
      uMainOscFreq = CHIP_FREQ_MAINCK_RC_4MHZ * tClockSrcDef.eFastOscSel;
      break;
      
    case CLOCK_MAIN_EXTOSC :
      // set the oscillator bypass
      uValue |= CKGR_MOR_MOSCXTBY;
      
      // fall into external crystal

    case CLOCK_MAIN_EXTXTAL :
      // set the startup time/enable it
      uValue |= CKGR_MOR_MOSCXTST( tClockSrcDef.nMainStart) ;
      uValue |= CKGR_MOR_MOSCXTEN;
      uValue |= CKGR_MOR_MOSCSEL;
      
      // write it/wait for sync
      PMC->CKGR_MOR = uValue;
      while( !( PMC->PMC_SR & PMC_SR_MOSCXTS ));

      // set the clock frequency
      uMainOscFreq = CLOCK_EXTXTAL_FREQ;
      break;
      
    default :
      break;
  }
}

/******************************************************************************
 * @function SetupPll
 *
 * @brief setup the PLL
 *
 * This function will setup the defined PLL
 *
 *****************************************************************************/
static void SetupPlls( void )
{
  CLOCKPLLSEL ePllSel;
  U8          nIdx;
  U32         uValue;
  
  // for each defined PLL
  nIdx = 0;
  while(( ePllSel = atClockPllDef[ nIdx ].ePllSel ) != CLOCK_PLL_MAX )
  {
    switch( ePllSel )
    {
      case CLOCK_PLL_SEL_A :
        // set the multiplier/divier/lock count
        uValue = CKGR_PLLAR_DIVA( atClockPllDef[ nIdx ].nDivider );
        uValue |= CKGR_PLLAR_MULA( atClockPllDef[ nIdx ].wMultiplier );
        uValue |= CKGR_PLLAR_PLLACOUNT( atClockPllDef[ nIdx ].nSlowClockLockCnt );
        uValue |= CKGR_PLLAR_ONE;
        PMC->CKGR_PLLAR = uValue;
        while( !( PMC->PMC_SR & PMC_SR_LOCKA ));
        break;
        
      #ifdef REG_CKGR_PLLBR
      case CLOCK_PLL_SEL_B :
        // set the multiplier/divier/lock count
        uValue = CKGR_PLLBR_DIVB( atClockPllDef[ nIdx ].nDivider );
        uValue |= CKGR_PLLBR_MULB( atClockPllDef[ nIdx ].wMultiplier );
        uValue |= CKGR_PLLBR_PLLBCOUNT( atClockPllDef[ nIdx ].nSlowClockLockCnt );
        uValue |= CKGR_PLLAR_ONE;
        PMC->CKGR_PLLBR = uValue;
        while( !( PMC->PMC_SR & PMC_SR_LOCKB ));
        break;
      #endif // REG_CKGR_PLLBR
        
      default :
        break;
    }
  
    // increment the index
    nIdx++;
  }
}

/******************************************************************************
 * @function SetupProgClocks
 *
 * @brief setup the programmable clocks
 *
 * This function will setup the programmable clocks
 *
 *****************************************************************************/
static  void  SetupProgClocks( void )
{
  CLOCKPRGCLKENUM eClkEnum, eCurrent;
  U32             uValue;
  
  // for each defined clock enum
  eCurrent = 0;
  while(( eClkEnum = atClockProgDef[ eCurrent ].eProgClkEnum ) != CLOCK_PRGCLK_ENUM_MAX )
  {
    // set the values
    uValue = atClockProgDef[ eCurrent ].eClockSource;
    uValue |= BITS( atClockProgDef[ eCurrent ].eClockPrescale, PMC_PCK_PRES_Pos );
    
    // get the appropriate control register
    PMC->PMC_PCK[ eClkEnum ] = uValue;
    
    // enable it
    PMC->PMC_SCER = BIT( eClkEnum + 8 );
    
    // increment the current index
    eCurrent++;
  }
}

/******************************************************************************
 * @function GetPllFreq
 *
 * @brief get the PLL frequency
 *
 * This function will return the PLL frequency
 *
 * @param[in]   ePllSel   PLL select
 *
 * @return      frequency of the PLL
 *
 *****************************************************************************/
static U32 GetPllFreq( CLOCKPLLSEL ePllSel )
{
  U32 uValue, uMultiplier, uDivider;
  U32 uPllFreq;
  
  switch( ePllSel )
  {
    case CLOCK_PLL_SEL_A :
      // get the current settings
      uValue = PMC->CKGR_PLLAR;
      break;
      
    #ifdef REG_CKGR_PLLBR
    case CLOCK_PLL_SEL_B :
      // get the current settings
      uValue = PMC->CKGR_PLLBR;
      break;
    #endif // REG_CKGR_PLLBR
      
    default :
      uValue = 0;
      break;
  }
  
  // get the multipler/divider
  uMultiplier = ( uValue & CKGR_PLLAR_MULA_Msk ) >> CKGR_PLLAR_MULA_Pos;
  uDivider = ( uValue & CKGR_PLLAR_DIVA_Msk ) >> CKGR_PLLAR_DIVA_Pos;
  
  // now compute the frequency
  uPllFreq = ( uMainOscFreq / uDivider ) * ( uMultiplier + 1 );
  
  // return the frequency
  return( uPllFreq );
}

/**@} EOF Clock.c */
