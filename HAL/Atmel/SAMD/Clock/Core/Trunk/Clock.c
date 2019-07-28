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
static  void  ConfigureXosc( PCLOCKOSCDEF ptDef );
static  void  ConfigureOsc32k( PCLOCKOSCDEF ptDef );
static  void  ConfigureOsc8Mhz( PCLOCKOSCDEF ptDef );
static  void  ConfigureOscDfll( PCLOCKOSCDEF ptDef );
static  void  ConfigureGenerator( PCLOCKGENDEF ptGen );
static  void  ConfigureMultiplexer( PCLOCKMUXDEF ptMux );
static  U32   GetSystemClockSource( CLOCKSRC eSrc );
static  U32   ComputeClockGenFreq( CLOCKGENID eClockGen );

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
  PCLOCKOSCDEF  ptClkDef;
  PCLOCKMUXDEF  ptMuxDef;
  PCLOCKGENDEF  ptGenDef;
  U8            nIdx;
  BOOL          bRunFlag;
  
  // now configure the flash wait states/clock divides
  NVMCTRL->CTRLB.bit.RWS = g_tClockMainDef.nFlashWaitStates;
  
  // configure the CPU/APBA/APBB dividers
  PM->CPUSEL.reg = g_tClockMainDef.eCpuDiv;
  PM->APBASEL.reg = g_tClockMainDef.eAPBADiv;
  PM->APBBSEL.reg = g_tClockMainDef.eAPBBDiv;
    
  // perform a software reset
  GCLK->CTRL.reg = GCLK_CTRL_SWRST;
  while( GCLK->CTRL.reg & GCLK_CTRL_SWRST );
  
  // for each oscillator in the list
  nIdx = 0;
  bRunFlag = TRUE;
  while( bRunFlag )
  {
    // get entry from table
    ptClkDef = ( PCLOCKOSCDEF )&g_atClockOscDefs[ nIdx++ ];
    
    // check for end of table
    if ( ptClkDef->eClockOsc != CLOCK_OSC_EOT )
    {
      // check for type
      switch( ptClkDef->eClockOsc )
      {
        case CLOCK_OSC_EXT :
          ConfigureXosc( ptClkDef );
          break;
        
        case CLOCK_OSC_INT32K :
          ConfigureOsc32k( ptClkDef );
          break;
          
        case CLOCK_OSC_EXT32K :
          break;
          
        case CLOCK_OSC_INTLP32K :
          break;
          
        case CLOCK_OSC_INT8MZ :
          ConfigureOsc8Mhz( ptClkDef );
          break;
          
        case CLOCK_OSC_DFLL :
          ConfigureOscDfll( ptClkDef );
          break;
        
        case CLOCK_OSC_DPLL :
          break;
          
        default :
          break;
      }
    }
    else
    {
      // exit
      bRunFlag = FALSE;
    }
  }
  
  // process the clock generators
  nIdx = 0;
  bRunFlag = TRUE;
  while( bRunFlag )
  {
    // get the definition
    ptGenDef = ( PCLOCKGENDEF )&g_atClockGenDefs[ nIdx++ ];
    
    // test for a valid entry
    if ( ptGenDef->nId != CLOCK_GENID_EOT )
    {
      // configure the generator
      ConfigureGenerator( ptGenDef );
    }
    else
    {
      // clear the run flag
      bRunFlag = FALSE;
    }
  }
  
  // process the clock multiplexers
  nIdx = 0;
  bRunFlag = TRUE;
  while( bRunFlag )
  {
    // get the definition
    ptMuxDef = ( PCLOCKMUXDEF )&g_atClockMuxDefs[ nIdx++ ];
    
    // test for a valid entry
    if ( ptMuxDef->eMuxId != CLOCK_MUXID_EOT )
    {
      // process it
      ConfigureMultiplexer( ptMuxDef );
    }
    else
    {
      // end of table - exit
      bRunFlag = FALSE;
    }
  }
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
  // perform a software reset
  GCLK->CTRL.reg = GCLK_CTRL_SWRST;
  while( GCLK->CTRL.reg & GCLK_CTRL_SWRST );
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
U32 Clock_GetFreq( )
{
  // GetSystemClockSource
  return( ComputeClockGenFreq( CLOCK_GENID_0 ));
}

/******************************************************************************
 * @function Clock_GetMultiplexerFreq
 *
 * @brief get the current clock of a given peripheral frequency
 *
 * This function will return the current clock frequency
 *
 * @param[in]   eMuxId      multiplexer ID
 * 
 *  @return     the frequency of the clock
 *
 *****************************************************************************/
U32 Clock_GetMultiplexerFreq( CLOCKMUXID eMuxId )
{
  CLOCKGENID  eClockGen;
  
  //  set the channel into the control register/read it
  *(( PU8 )&GCLK->CLKCTRL.reg ) = eMuxId;
  eClockGen = GCLK->CLKCTRL.bit.GEN;
  
  return ( ComputeClockGenFreq( eClockGen ));
}

/******************************************************************************
 * @function ConfigureXosc
 *
 * @brief configure the external oscillator
 *
 * This function will configure the external oscillator
 *
 * @param[in]   ptDef     pointer to a clock definition structure
 *
 *****************************************************************************/
static void ConfigureXosc( PCLOCKOSCDEF ptDef )
{
  // create a pointer
  SYSCTRL_XOSC_Type tOsc;
    
  // set the control bits
  tOsc.reg = 0;
  tOsc.bit.STARTUP = ptDef->nStartupTime;
  tOsc.bit.XTALEN = ptDef->eExtType;
  tOsc.bit.AMPGC = ptDef->tFlags.bAutoAmpGain;
  
  // set gain based on frequency if not auto gain
  if ( !ptDef->tFlags.bAutoAmpGain )
  {
    if( ptDef->uExtFreq <= 2000000 )
    {
      // set the gain to 0
      tOsc.reg |= SYSCTRL_XOSC_GAIN( SYSCTRL_XOSC_GAIN_0_Val );
    }
    else if ( ptDef->uExtFreq <= 4000000 )
    {
      // set the gain to 1
      tOsc.reg |= SYSCTRL_XOSC_GAIN( SYSCTRL_XOSC_GAIN_1_Val );
    }
    else if ( ptDef->uExtFreq <= 8000000 )
    {
      // set the gain to 2
      tOsc.reg |= SYSCTRL_XOSC_GAIN( SYSCTRL_XOSC_GAIN_2_Val );
    }
    else if ( ptDef->uExtFreq <= 16000000 )
    {
      // set the gain to 3
      tOsc.reg |= SYSCTRL_XOSC_GAIN( SYSCTRL_XOSC_GAIN_3_Val );
    }
    else if ( ptDef->uExtFreq <= 30000000 )
    {
      // set the gain to 4
      tOsc.reg |= SYSCTRL_XOSC_GAIN( SYSCTRL_XOSC_GAIN_4_Val );
    }
  }
  
  // set the on demand/runin standby
  tOsc.bit.ONDEMAND = ptDef->tFlags.bOnDemand;
  tOsc.bit.RUNSTDBY = ptDef->tFlags.bRunInStandby;
  
  // enable it
  tOsc.bit.ENABLE = TRUE;
  
  // store it
  SYSCTRL->XOSC.reg = tOsc.reg;
}

/******************************************************************************
 * @function ConfigureOsc32k
 *
 * @brief configure the 32K oscillator
 *
 * This function will configure the 32K oscillator
 *
 * @param[in]   ptDef     pointer to a clock definition structure
 *
 *****************************************************************************/
static void ConfigureOsc32k( PCLOCKOSCDEF ptDef )
{
  // create a pointer
  SYSCTRL_OSC32K_Type tOsc;
  
  // set the calibration bits
  tOsc.reg = 0;
  #ifdef __SAM_D10_SUBFAMILY
  PU32  puCalAddr = ( PU32 )( U32 )FUSES_OSC32K_ADDR;
  tOsc.bit.CALIB =  *( puCalAddr ) >> FUSES_OSC32K_Pos;
  #elif __SAM_D20_SUBFAMILY
  PU32  puCalAddr = ( PU32 )( U32 )FUSES_OSC32KCAL_ADDR;
  tOsc.bit.CALIB =  *( puCalAddr ) >> FUSES_OSC32KCAL_Pos;
  #else
  PU32  puCalAddr = ( PU32 )( U32 )FUSES_OSC32K_CAL_ADDR;
  tOsc.bit.CALIB =  *( puCalAddr ) >> FUSES_OSC32K_CAL_Pos;
  #endif // _SAMD10_
  
  // set the options
  tOsc.bit.EN1K = ptDef->tFlags.bEnable1KOut;
  tOsc.bit.EN32K = ptDef->tFlags.bEnable32KOut;
  tOsc.bit.STARTUP = ptDef->nStartupTime;
  tOsc.bit.ONDEMAND = ptDef->tFlags.bOnDemand;
  tOsc.bit.RUNSTDBY = ptDef->tFlags.bRunInStandby;
  tOsc.bit.WRTLOCK = ptDef->tFlags.bLockBypass;
  
  // enable it
  tOsc.bit.ENABLE = TRUE;
  
  // store the settings
  SYSCTRL->OSC32K.reg = tOsc.reg;
}

/******************************************************************************
 * @function ConfigureOsc8Mhz
 *
 * @brief configure the 8MHZ oscillator
 *
 * This function will configure the 8MHZZ oscillator
 *
 * @param[in]   ptDef     pointer to a clock definition structure
 *
 *****************************************************************************/
static void ConfigureOsc8Mhz( PCLOCKOSCDEF ptDef )
{
  // create a pointer
  SYSCTRL_OSC8M_Type tOsc;
  
  // get the oscillator calibration value
  tOsc = SYSCTRL->OSC8M;
  
  // set the options/do not alter calibation/frequency range
  tOsc.bit.PRESC = ptDef->e8MhzDiv;
  tOsc.bit.ONDEMAND = ptDef->tFlags.bOnDemand;
  tOsc.bit.RUNSTDBY = ptDef->tFlags.bRunInStandby;
  
  // enable it
  tOsc.bit.ENABLE = TRUE;
  
  // store the settings
  SYSCTRL->OSC8M.reg = tOsc.reg;

  // set up the associated clock generator
  ConfigureGenerator( &ptDef->tOutGenDef );
}

/******************************************************************************
 * @function ConfigureOscDfll
 *
 * @brief configure the DFLL oscillator
 *
 * This function will configure the DFLL oscillator
 *
 * @param[in]   ptDef     pointer to a clock definition structure
 *
 *****************************************************************************/
static void ConfigureOscDfll( PCLOCKOSCDEF ptDef )
{
  SYSCTRL_DFLLCTRL_Type tCtrl;
  SYSCTRL_DFLLVAL_Type  tVal;
  SYSCTRL_DFLLMUL_Type  tMult;
  U32                   uCoarse, uFine;
//  CLOCKMUXDEF           tClockMux;

  // enable the multiplexer for the reference 
//  tClockMux.bWriteLock = OFF;
//  tClockMux.eClockGenId = CLOCK_GENID_1;
//  tClockMux.eMuxId = CLOCK_MUXID_DFLL48M_REF;
//  ConfigureMultiplexer( &tClockMux );

  // workaround for errate 9905
  SYSCTRL->DFLLCTRL.bit.ONDEMAND = OFF;
  while( !SYSCTRL->PCLKSR.bit.DFLLRDY );

  // fill the control structure
  tCtrl.reg = 0;
//  #ifdef __SAM_D21_SUBFAMILY
//  tCtrl.bit.WAITLOCK = OFF;
//  tCtrl.bit.BPLCKC = OFF;
//  tCtrl.bit.USBCRM = ptDef->tFlags.bEnableUsbRecovery;
//  #endif
//  tCtrl.bit.QLDIS = ON;
//  tCtrl.bit.CCDIS = ( ptDef->tFlags.bEnableChillCycle ) ? FALSE : TRUE;
//  tCtrl.bit.ONDEMAND = OFF;
//  tCtrl.bit.RUNSTDBY = OFF;
//  tCtrl.bit.LLAW = ptDef->tFlags.bKeepLockWakeup;
//  tCtrl.bit.STABLE = OFF;
//  tCtrl.bit.MODE = ptDef->tFlags.bClosedLoopMode;
//  tCtrl.bit.ENABLE = OFF;
    tCtrl.reg = SYSCTRL_DFLLCTRL_ENABLE | SYSCTRL_DFLLCTRL_USBCRM | SYSCTRL_DFLLCTRL_BPLCKC | SYSCTRL_DFLLCTRL_CCDIS | SYSCTRL_DFLLCTRL_STABLE;

  // clear the multipler/value registers
  tMult.reg = 0;
  tVal.reg = 0;

  // determine the mode for coarse/fine/step/mul
  if ( ptDef->tFlags.bClosedLoopMode )
  {
    // fill the coarse/fine/write it
    uCoarse =  *(( PU32 )( FUSES_DFLL48M_COARSE_CAL_ADDR )) >> FUSES_DFLL48M_COARSE_CAL_Pos;
    uFine = *(( PU32 )( FUSES_DFLL48M_FINE_CAL_ADDR )) & FUSES_DFLL48M_FINE_CAL_Msk;
    tVal.reg = SYSCTRL_DFLLVAL_COARSE( uCoarse );
    tVal.reg |= SYSCTRL_DFLLVAL_FINE( uFine );

    // set the coarse/fine step/multipler
    tMult.reg = SYSCTRL_DFLLMUL_MUL( 48000 );
  }
  else
  {
    // open loop mode values
  }

  // write the VALUE/MULT registers
  SYSCTRL->DFLLVAL.reg = tVal.reg;
  SYSCTRL->DFLLMUL.reg = tMult.reg;

  // write it/wait for sync
  SYSCTRL->DFLLCTRL.reg = tCtrl.reg;
  while( !SYSCTRL->PCLKSR.bit.DFLLRDY );

  // now configure the output generator
  ConfigureGenerator( &ptDef->tOutGenDef );
}

/******************************************************************************
 * @function ConfigureGenerator
 *
 * @brief configure a clock generator
 *
 * This function will configure a clock generator
 *
 * @param[in]   ptGen     pointer to a generator definition structure
 *
 *****************************************************************************/
static void ConfigureGenerator( PCLOCKGENDEF ptGen )
{
  GCLK_GENDIV_Type  tGenDiv;
  GCLK_GENCTRL_Type tGenCtl;
  
  // set the divisor/wait for sync
  tGenDiv.reg = 0;
  tGenDiv.bit.ID = ptGen->nId;
  tGenDiv.bit.DIV = ptGen->wDivisor;
  GCLK->GENDIV.reg = tGenDiv.reg;
  while( GCLK->STATUS.reg != 0 );
  
  // set the genererator control
  tGenCtl.reg = 0;
  tGenCtl.bit.ID = ptGen->nId;
  tGenCtl.bit.GENEN = ON;
  tGenCtl.bit.IDC = ptGen->tFlags.bImprovedDutyCycle;
  tGenCtl.bit.OOV = ptGen->tFlags.bOutOffEnabled;
  tGenCtl.bit.OE = ptGen->tFlags.bOutputEnabled;
  tGenCtl.bit.DIVSEL = ptGen->eDivideSelect;
  tGenCtl.bit.RUNSTDBY = ptGen->tFlags.bRunInStandby;
  tGenCtl.bit.SRC = ptGen->eClockSrc;
  GCLK->GENCTRL.reg = tGenCtl.reg;
  while( GCLK->STATUS.bit.SYNCBUSY );
  

  // if this is generator 0, set the clock frequency
  if ( ptGen->nId == CLOCK_GENID_0 )
  {
    // determine the source
    switch( ptGen->eClockSrc )
    {
      case CLOCK_SRC_OSC8M :
        uSystemClockFreq = 8000000l;
        break;

      case CLOCK_SRC_DFLL48M :
        uSystemClockFreq = 48000000l;
        break;

      default :
        break;
    }
  }
}

/******************************************************************************
 * @function ConfigureMultiplexer
 *
 * @brief configure a multipler
 *
 * This function will configure a designated multiplexor
 *
 * @param[in]   ptMux   pointero to a multipler def bllock
 *
 *****************************************************************************/
static void ConfigureMultiplexer( PCLOCKMUXDEF ptMux )
{
  GCLK_CLKCTRL_Type tCtl;
  
  // set the bits
  tCtl.reg = 0;
  tCtl.bit.ID = ptMux->eMuxId;
  tCtl.bit.CLKEN = TRUE;
  tCtl.bit.GEN = ptMux->eClockGenId;
  tCtl.bit.WRTLOCK = FALSE;
  GCLK->CLKCTRL.reg = tCtl.reg;
}

/******************************************************************************
 * @function GetSystemCLockSourceFreq
 *
 * @brief Gets the frequency of a system clock source
 *
 * This function will  return the frequency of a given clock source
 *
 * @param[in]   eSrc    appropriate enum for a system clock source
 *
 * @return        frequency of a given clock source
 *****************************************************************************/
static U32 GetSystemClockSource( CLOCKSRC eSrc )
{
  U32 uFreqHz = 0;
    
  // determine the clock source
  switch( eSrc )
  {
    case CLOCK_SRC_XOSC :
      break;
        
    case CLOCK_SRC_GCLKIN :
      break;
        
    case CLOCK_SRC_GCLKGEN1 :
      break;
        
    case CLOCK_SRC_OSCULP32K :
      break;

    case CLOCK_SRC_OSC32K :
      uFreqHz = 32768l;
      break;
      
    case CLOCK_SRC_XOSC32K :
      break;
        
    case CLOCK_SRC_OSC8M :
      uFreqHz = 8000000l >> SYSCTRL->OSC8M.bit.PRESC;
      break;
        
    case CLOCK_SRC_DFLL48M :
      uFreqHz = 48000000l;
      break;
        
    case CLOCK_SRC_DPLL96M :
      break;
        
    default :
      uFreqHz = 0;
      break;
    
  }  
  
  // return the frequency
  return( uFreqHz );
}

/******************************************************************************
 * @function ComputeClockGenFreq
 *
 * @brief Gets the frequency of a clock generator
 *
 * This function will return the frequency of a given clock generrator
 *
 * @param[in]   eClockGen   clock generator
 *
 * @return        frequency of a given clock generator
 *
 *****************************************************************************/
static U32 ComputeClockGenFreq( CLOCKGENID eClockGen )
{
  U32         uGeneratorFreq, uDivider;
  BOOL        bDivisorSelect;
  CLOCKSRC    eClockSource;

  //  disable interrupts
  Interrupt_Disable( );
  
  //  wait for sync
  while( GCLK->STATUS.reg & GCLK_STATUS_SYNCBUSY );
  
  //  select the generator/sync
  *(( PU8 )&GCLK->GENCTRL.reg ) = eClockGen;
  while( GCLK->STATUS.reg & GCLK_STATUS_SYNCBUSY );
  
  // get the souce/divisor
  bDivisorSelect = GCLK->GENCTRL.bit.DIVSEL;
  eClockSource = GCLK->GENCTRL.bit.SRC;
  
  // get the frequency for that source
  uGeneratorFreq = GetSystemClockSource( eClockSource );
  
  //  select the appropatie generator
  *(( PU8 )&GCLK->GENDIV.reg ) = eClockGen;
  while( GCLK->STATUS.reg & GCLK_STATUS_SYNCBUSY );

  // get te dividor
  uDivider = GCLK->GENDIV.bit.DIV;
  
  // re-enable interrupts
  Interrupt_Enable( );
  
  // compute the frequency
  if ( !bDivisorSelect && uDivider > 1 )
  {
    // device the freq by the divider
    uGeneratorFreq /= uDivider;
  }
  else if ( bDivisorSelect )
  {
    // shift the frequency by the diver + 1
    uGeneratorFreq >>= ( uDivider + 1 );
  }
  
  // return the frequency
  return( uGeneratorFreq );
}

/**@} EOF Clock.c */
