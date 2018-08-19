/******************************************************************************
 * @file CLock.c
 *
 * @brief Clock module implementation 
 *
 * This file provides the implementation for the clock module
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
 * \addtogroup CLock
 * @{
 *****************************************************************************/

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "Clock/Clock.h"

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------
/// define the slow internal clock
#define CPU_INT_SLOW_CLK_HZ             ( 32768 )
#define CPU_INT_FAST_CLK_HZ             ( 4000000 )

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations ----------------------------------------------

// local parameter declarations -----------------------------------------------

// local function prototypes --------------------------------------------------
static  void  SetupFeiMode( void );
static  void  SetupPeeMode( void );
static  void  SetupBpleMode( void );
static  void  SetupIhsMode( void );

// constant parameter initializations -----------------------------------------

/******************************************************************************
 * @function Clock_Initialize
 *
 * @brief initialization
 *
 * This function will initialize the clock based on the configuration structure
 *
 *****************************************************************************/
void Clock_Initialize( void )
{
  // determine the core clock selection
  switch( tClockDef.eClockMode )
  {
    case CLOCK_MODESEL_FEI :
      SetupFeiMode( );
      break;

    case CLOCK_MODESEL_PEE :
      SetupPeeMode( );
      break;

    case CLOCK_MODESEL_BPLE :
      SetupBpleMode( );
      break;

    case CLOCK_MODESEL_IHS :
      SetupIhsMode( );
      break;

    default :
      break;
  }
}

/******************************************************************************
 * @function Clock_GetFreq
 *
 * @brief get the core clock frequency
 *
 * This function will return the current clock frequency 
 *
 * @return    current clock frequency
 *
 *****************************************************************************/
U32 Clock_GetFreq( void )
{
  U32 uFreq;
  U8  nDivider;

  // check for PLL or FLL
  if (( MCG->C1 & MCG_C1_CLKS_MASK ) == 0x0u )
  {
    // output is either FLL/PLL
    if (( MCG->C6 & MCG_C6_PLLS_MASK ) == 0x0u )
    {
      // FLL is selected
      if (( MCG->C1 & MCG_C1_IREFS_MASK ) == 0x0u )
      {
        // External reference clock is selected
        uFreq = CLOCK_EXT_CRYSTAL_FREQ;   
        
        // System oscillator drives MCG clock
        nDivider = ( U8 )( 1u << (( MCG->C1 & MCG_C1_FRDIV_MASK ) >> MCG_C1_FRDIV_SHIFT ));
        uFreq /= nDivider;

        // check for high range
        if (( MCG->C2 & MCG_C2_RANGE0_MASK ) != 0x0u )
        {
          // divide by addtional 32
          uFreq /= 32u;
        }
      } 
      else 
      {
        // slow internal clock
        uFreq = CPU_INT_SLOW_CLK_HZ;                                     /* The slow internal reference clock is selected */
      }
      
      // Select correct multiplier to calculate the MCG output clock
      switch ( MCG->C4 & ( MCG_C4_DMX32_MASK | MCG_C4_DRST_DRS_MASK ))
      {
        case 0x0u:
          uFreq *= 640u;
          break;
        case 0x20u:
          uFreq *= 1280u;
          break;
        case 0x40u:
          uFreq *= 1920u;
          break;
        case 0x60u:
          uFreq *= 2560u;
          break;
        case 0x80u:
          uFreq *= 732u;
          break;
        case 0xA0u:
          uFreq *= 1464u;
          break;
        case 0xC0u:
          uFreq *= 2197u;
          break;
        case 0xE0u:
          uFreq *= 2929u;
          break;
        default:
          break;
      }
    } 
    else
    { 
      // PLL is selected
      nDivider = ( 1u + ( MCG->C5 & MCG_C5_PRDIV0_MASK ));
      uFreq = ( U32 )( CLOCK_EXT_CRYSTAL_FREQ / nDivider );
      nDivider = (( MCG->C6 & MCG_C6_VDIV0_MASK ) + 24u );
      uFreq *= nDivider;
    }
  }
  else if (( MCG->C1 & MCG_C1_CLKS_MASK ) == 0x40u )
  {
    // Internal reference clock is selected
    if (( MCG->C2 & MCG_C2_IRCS_MASK ) == 0x0u )
    {
      uFreq = CPU_INT_SLOW_CLK_HZ;
    } 
    else
    {
      uFreq = CPU_INT_FAST_CLK_HZ / ( 1 << (( MCG->SC & MCG_SC_FCRDIV_MASK ) >> MCG_SC_FCRDIV_SHIFT ));
    }
  } 
  else if (( MCG->C1 & MCG_C1_CLKS_MASK ) == 0x80u )
  {
    // External reference clock is selected
    uFreq = CLOCK_EXT_CRYSTAL_FREQ;
  } 
  else
  {
    // reserved value
    uFreq = 0;
  }

  // now apply the division
  uFreq /= ( 1u + (( SIM->CLKDIV1 & SIM_CLKDIV1_OUTDIV1_MASK ) >> SIM_CLKDIV1_OUTDIV1_SHIFT ));

  // return the frequency
  return( uFreq );
}

/******************************************************************************
 * @function Clock_GetPeriphFreq
 *
 * @brief get the peripheral clock frequency
 *
 * This function will return the current clock frequency 
 *
 * @return    current clock frequency
 *
 *****************************************************************************/
U32 Clock_GetPeriphFreq( void )
{
  U32 uFreq;

  // get the core frequency
  uFreq = Clock_GetFreq( );

  // apply the divisor
  uFreq /= ( 1u + (( SIM->CLKDIV1 & SIM_CLKDIV1_OUTDIV4_MASK ) >> SIM_CLKDIV1_OUTDIV4_SHIFT ));

  // return the frequency
  return( uFreq );
}

/******************************************************************************
 * @function SetupFeiMode
 *
 * @brief setup FLL Enaged Internal Mode
 *
 * This function will initialize the clock based on the configuration structure
 *
 *****************************************************************************/
static void SetupFeiMode( void )
{
  // set up output divide mode
  SIM->CLKDIV1 = SIM_CLKDIV1_OUTDIV1( tClockDef.nOutDivider1 - 1 ) | SIM_CLKDIV1_OUTDIV4( tClockDef.nOutDivider4 - 1 );

  // switch to FEI mode
  MCG->C1 = MCG_C1_IREFS_MASK | MCG_C1_IRCLKEN_MASK | MCG_C1_FRDIV( tClockDef.nFllDivider );
  MCG->C2 = 0;

  // set up for external mid range
  MCG->C4 &= ~MCG_C4_DRST_DRS_MASK;
  MCG->C4 |= MCG_C4_DRST_DRS( 1 );

  // enable the oscillator
  OSC0->CR = OSC_CR_ERCLKEN_MASK;

  // disable PLL
  MCG->C5 = 0;
  MCG->C6 = 0;

  // wait for FLL is running on internal
  while(( MCG->S & MCG_S_IREFST_MASK ) == 0 );

  // now wait for the output of the FLL to be enabled
  while(( MCG->S & ( MCG_S_LOLS0_MASK | MCG_S_LOCK0_MASK )) != 0 );
}

/******************************************************************************
 * @function SetupPeeMode
 *
 * @brief setup PLL engaged Exernal Mode
 *
 * This function will initialize the clock based on the configuration structure
 *
 *****************************************************************************/
static void SetupPeeMode( void )
{
  // set up output divide mode
  SIM->CLKDIV1 = SIM_CLKDIV1_OUTDIV1( tClockDef.nOutDivider1 - 1 ) | SIM_CLKDIV1_OUTDIV4( tClockDef.nOutDivider4 - 1 );

  // switch to FBE mode
  MCG->C2 = MCG_C2_RANGE0( 2 ) | MCG_C2_EREFS0_MASK;
  
  // turn on the oscillator/enable 2PF capacitance
  OSC0->CR = OSC_CR_ERCLKEN_MASK | OSC_CR_SC2P_MASK;

  // enable the clock source//divider/internal reference clock
  MCG->C1 = MCG_C1_CLKS( 2 ) | MCG_C1_FRDIV( tClockDef.nFllDivider ) | MCG_C1_IRCLKEN_MASK;

  // diable the DCO
  MCG->C4 = ( U8 )~( MCG_C4_DMX32_MASK | MCG_C4_DRST_DRS_MASK );

  // set the PLL divider
  MCG->C5 =  MCG_C5_PRDIV0( 1 );
  MCG->C6 = 0;

  // wait for FLL ref to be the external ref clock
  while(( MCG->S & MCG_S_IREFST_MASK ) != 0 );

  // wait for external ref clock to be selected 
  while(( MCG->S & ( MCG_S_LOLS0_MASK | MCG_S_LOCK0_MASK )) != MCG_S_LOLS0_MASK );

  // switch to PBE mode
  MCG->C6 = MCG_C6_PLLS_MASK;

  // wait for external ref clock to be selected 
  while(( MCG->S & ( MCG_S_LOLS0_MASK | MCG_S_LOCK0_MASK )) != MCG_S_LOLS0_MASK );

  // switch to PEE mode
  MCG->C1 = MCG_C1_FRDIV( tClockDef.nFllDivider ) | MCG_C1_IRCLKEN_MASK;

  // wait for external ref clock to be selected 
  while(( MCG->S & ( MCG_S_LOLS0_MASK | MCG_S_LOCK0_MASK )) != MCG_S_LOLS0_MASK );
}

/******************************************************************************
 * @function SetupFeiMode
 *
 * @brief setup FLL Enaged Internal Mode
 *
 * This function will initialize the clock based on the configuration structure
 *
 *****************************************************************************/
static void SetupBpleMode( void )
{
}

/******************************************************************************
 * @function SetupFeiMode
 *
 * @brief setup FLL Enaged Internal Mode
 *
 * This function will initialize the clock based on the configuration structure
 *
 *****************************************************************************/
static void SetupIhsMode( void )
{
}
 
/**@} EOF CLock.c */
