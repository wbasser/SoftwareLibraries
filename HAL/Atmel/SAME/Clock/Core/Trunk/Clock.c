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
U32 g_ClockMasterFrequency;

// local parameter declarations -----------------------------------------------
static  U32   uSystemClockFreq;         ///< HCLK/FCLK
static  U32   uMainOscFreq;             ///< MAINCK
static  U32   uSlowClockFreq;           ///< SLCK
static  U32   uPllAClkFreq;             ///< PLLA
static  U32   uUpllClkDivFreq;          ///< UPLLCKDIV

// local function prototypes --------------------------------------------------
static  void  SetupMainClock( void );
static  void  SetupPlls( void );
static  void  SetupProgClocks( void );
static  void  SetupPeriphClocks( void );
static  U32   GetPllFreq( void );

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
  U8  nWaitStates;
  U32 uValue;

  // clear the other frequencies
  uMainOscFreq = uPllAClkFreq = uUpllClkDivFreq = 0;
  uSlowClockFreq = 32000l;
  
  // set the flash wait states
  EFC->EEFC_FMR = EEFC_FMR_FWS( 7 );
  
  // set up the slow clock
  if (tClockSrcDef.eSlowClkSrc == CLOCK_SLOW_XOSC32K )
  {
    // turn on the external slow clock
    SUPC->SUPC_MR |= ( SUPC_MR_OSCBYPASS_BYPASS | SUPC_MR_KEY_PASSWD );
    SUPC->SUPC_CR = ( SUPC_CR_XTALSEL_CRYSTAL_SEL | SUPC_CR_KEY_PASSWD );
    uSlowClockFreq = 32768l;
  }

  // based on clock selection
  switch( tClockMasterDef.eClockSource )
  {
    case CLOCK_MASTER_SCLK :
      // now write it/wait for ready
      PMC->PMC_MCKR = PMC_MCKR_CSS_SLOW_CLK;
      while( !( PMC->PMC_SR & PMC_SR_MCKRDY ));
      
      // set the clock frequency
      uSystemClockFreq = CHIP_FREQ_XTAL_32K;
      break;
      
    case CLOCK_MASTER_MAINCK :
      // set up the master clock
      SetupMainClock( );
      uSystemClockFreq = uMainOscFreq;
      break;
      
    case CLOCK_MASTER_PLLACK :
      // set up the master clock
      SetupMainClock( );
      SetupPlls( );
      
      // switch to main clock with prescale set
      uValue = PMC_MCKR_CSS_MAIN_CLK;
      uValue |= PMC_MCKR_PRES( tClockMasterDef.eClockPrescale );
      uValue |= PMC_MCKR_MDIV( tClockMasterDef.eClockDivide );
      PMC->PMC_MCKR = uValue;
      while( !( PMC->PMC_SR & PMC_SR_MCKRDY ));
      
      // now switch to PLL
      uValue = PMC_MCKR_CSS_PLLA_CLK;
      uValue |= PMC_MCKR_PRES( tClockMasterDef.eClockPrescale );
      uValue |= PMC_MCKR_MDIV( tClockMasterDef.eClockDivide );
      PMC->PMC_MCKR = uValue;
      while( !( PMC->PMC_SR & PMC_SR_MCKRDY ));

      // set up PLLA
      uSystemClockFreq = uPllAClkFreq;
      break;
      
    default :
      uValue = 0;
      break;
  }

  // apply the prescale
  if (( PMC->PMC_MCKR & PMC_MCKR_PRES_Msk ) == PMC_MCKR_PRES_CLK_3 )
  {
    // divide it by 3
    uSystemClockFreq /= 3;
  }
  else
  {
    // apply the prescale
    uSystemClockFreq >>= (( PMC->PMC_MCKR & PMC_MCKR_PRES_Msk ) >> PMC_MCKR_PRES_Pos );
  }
  
  // set the external
  g_ClockMasterFrequency = uSystemClockFreq;
  
  // configure the peogrammable clocks
  SetupProgClocks( );
  
  // configure the peripheral clocks
  SetupPeriphClocks( );
  
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
  else if ( uSystemClockFreq < CHIP_FREQ_FWS_4 )
  {
    nWaitStates= 5;
  }
  else if ( uSystemClockFreq < CHIP_FREQ_FWS_4 )
  {
    nWaitStates= 6;
  }
  else
  {
    nWaitStates= 7;
  }

  // now write the wait states
  EFC->EEFC_FMR = EEFC_FMR_FWS( nWaitStates ) | EEFC_FMR_CLOE;
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
 * @function Clock_GetCpuFreq
 *
 * @brief get the current clock frequency
 *
 * This function will return the current clock frequency
 *
 * @return     the frequency of the main clock
 *
 *****************************************************************************/
U32 Clock_GetCpuFreq( void )
{
  // GetSystemClockSource
  return( uSystemClockFreq );
}

/******************************************************************************
 * @function Clock_GetMastClockFreq
 *
 * @brief get the current master clock frequency
 *
 * This function will return the current master clock frequency
 *
 * @return     the frequency of the main clock
 *
 *****************************************************************************/
U32 Clock_GetMastClockFreq( void )
{
  U32             uFreq = 0;
  U32             uDivider;
  CLOCKMASTERDIV  eMasterDiv;
  
  // get the divisor
  eMasterDiv = ( PMC->PMC_MCKR & PMC_MCKR_MDIV_Msk ) >> PMC_MCKR_MDIV_Pos;
  
  // set the divider
  switch( eMasterDiv )
  {
    case CLOCK_MASTER_DIV_2 :
      uDivider = 2;
      break;
      
    case CLOCK_MASTER_DIV_4 :
      uDivider = 4;
      break;
      
    case CLOCK_MASTER_DIV_3 :
      uDivider = 3;
      break;

    case CLOCK_MASTER_DIV_1 :
    default :
      uDivider = 1;
      break;
  }
  
  // set to base
  uFreq = uSystemClockFreq / uDivider;
  
  // return the frequency
  return( uFreq );
}

/******************************************************************************
 * @function Clock_GetProgClockFreq
 *
 * @brief get the current programmable clock frequency
 *
 * This function will return the current programmable clock frequency
 *
 * @param[in]   eClkEnum      clock enumerator
 *
 * @return     the frequency of the programmable clock
 *
 *****************************************************************************/
U32 Clock_GetProgClockFreq( CLOCKPRGCLKENUM eClkEnum )
{
  U32             uFreq = 0;
  CLOCKPRGCLKSRC  eClockSrc;
  U32             uPrescale;
  
  // check for a valid programmable clock
  if ( eClkEnum < CLOCK_PRGCLK_ENUM_MAX )
  {
    // determine the clock source
    eClockSrc = ( CLOCKPRGCLKSRC )( PMC->PMC_PCK[ eClkEnum ] & PMC_PCK_CSS_Msk );
    switch ( eClockSrc )
    {
      case CLOCK_PRGCLKSRC_SCLK :
        uFreq = uSlowClockFreq;
        break;
        
      case CLOCK_PRGCLKSRC_MAINCK :
        uFreq = uMainOscFreq;
        break;
        
      case CLOCK_PRGCLKSRC_PLLACK :
        uFreq = uPllAClkFreq;
        break;
        
      case CLOCK_PRGCLKSRC_UPLLCKDIV :
        uFreq = uUpllClkDivFreq;
        break;
        
      case CLOCK_PRGCLKSRC_MCK :
        uFreq = uSystemClockFreq;
        break;
        
      default :
        break;
    }
    
    // now apply the divider
    uPrescale = ( PMC->PMC_PCK[ eClkEnum ] & PMC_PCK_PRES_Msk ) / PMC_PCK_PRES_Pos;
    uFreq /= uPrescale;
  }
  
  // return the frequency
  return( uFreq );
}

/******************************************************************************
 * @function SetupMainClock
 *
 * @brief setup the main clock
 *
 * This function will setup the main clock
 *
 *****************************************************************************/
static void SetupMainClock( void )
{
  U32   uValue;
  
  // get the value of the current register/set the key
  uValue = CKGR_MOR_KEY_PASSWD;
  
  // determine source
  switch( tClockSrcDef.eMainCLkSrc )
  {
    case CLOCK_MAIN_FASTRC :
      // set up the divider
      uValue |= BITS( tClockSrcDef.eFastOscSel, CKGR_MOR_MOSCRCF_Pos );
      
      // write it
      PMC->CKGR_MOR = uValue;
      while( !( PMC->PMC_SR & PMC_SR_MOSCRCS ));

      // set the clock frequency
      uMainOscFreq = CHIP_FREQ_MAINCK_RC_4MHZ * tClockSrcDef.eFastOscSel;
      break;
      
    case CLOCK_MAIN_EXTXTAL :
      // initialize main oscillator
      uValue = CKGR_MOR_KEY_PASSWD;
      uValue |= CKGR_MOR_MOSCXTST( tClockSrcDef.nMainStart) ;
      uValue |= CKGR_MOR_MOSCRCEN;
      uValue |= CKGR_MOR_MOSCXTEN;
    
      // write it/wait for sync
      PMC->CKGR_MOR = uValue;
      while( !( PMC->PMC_SR & PMC_SR_MOSCXTS ));

      // set the startup time/enable it
      uValue |= CKGR_MOR_MOSCSEL;
      
      // write it/wait for sync
      PMC->CKGR_MOR = uValue;
      while( !( PMC->PMC_SR & PMC_SR_MOSCSELS ));

      // set the clock frequency
      uMainOscFreq = tClockSrcDef.uXtalFreq;
      break;
      
    default :
      break;
  }
      
  // switch to main clock
  PMC->PMC_MCKR = ( PMC->PMC_MCKR & PMC_MCKR_CSS_Msk ) | PMC_MCKR_CSS_MAIN_CLK;
  while( !( PMC->PMC_SR & PMC_SR_MCKRDY ));
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
  U32 uValue;
  U8  nMultiplier;
  
  // calculate the multiplier
  nMultiplier = tClockPllDef.uPllFreq / uMainOscFreq;
  
  // check to see if multiplier is within valid
  if ( nMultiplier < 62 )
  {
    // we're okay, now set the divider/multiplier
    uValue = CKGR_PLLAR_DIVA( 1 );
    uValue |= CKGR_PLLAR_MULA( nMultiplier );
    uValue |= CKGR_PLLAR_PLLACOUNT( tClockPllDef.nSlowClockLockCnt );
    uValue |= CKGR_PLLAR_ONE;
    PMC->CKGR_PLLAR = uValue;
    
    // wait for sync
    while( !( PMC->PMC_SR & PMC_SR_LOCKA ));

    // set the PLL clock
    uPllAClkFreq = GetPllFreq( );
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
    PMC->PMC_SCER = BIT(( eClkEnum + 8 ));
    
    // increment the current index
    eCurrent++;
  }
}

/******************************************************************************
 * @function SetupPeriphClocks
 *
 * @brief setup the peripheral clocks
 *
 * This function will setup the peripheral clocks
 *
 *****************************************************************************/
static  void  SetupPeriphClocks( void )
{
  U8    nClkEnum, nCurrent;
  U32   uValue;
  
  // for each defined clock enum
  nCurrent = 0;

  while(( nClkEnum = atClockPeriphDef[ nCurrent ].nPeriphID ) != 255 )
  {
    // set the PID/clock source/write mode/divider/enable
    uValue = atClockPeriphDef[ nCurrent ].nPeriphID;
    uValue |= BITS( atClockPeriphDef[ nCurrent ].eClockSource, 8 );
    uValue |= BIT( 12 );
    uValue |= PMC_PCR_DIV( atClockPeriphDef[ nCurrent ].eClockPrescale );
    uValue |= PMC_PCR_EN;

    // now write it
    PMC->PMC_PCR = uValue;

    // increment the current index
    nCurrent++;
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
static U32 GetPllFreq( void )
{
  U32 uValue, uMultiplier, uDivider;
  U32 uPllFreq;
  
  // get the current settings
  uValue = PMC->CKGR_PLLAR;
  
  // get the multipler/divider
  uMultiplier = ( uValue & CKGR_PLLAR_MULA_Msk ) >> CKGR_PLLAR_MULA_Pos;
  uDivider = ( uValue & CKGR_PLLAR_DIVA_Msk ) >> CKGR_PLLAR_DIVA_Pos;
  
  // now compute the frequency
  uPllFreq = ( uMainOscFreq / uDivider ) * ( uMultiplier );
  
  // return the frequency
  return( uPllFreq );
}

/**@} EOF Clock.c */
