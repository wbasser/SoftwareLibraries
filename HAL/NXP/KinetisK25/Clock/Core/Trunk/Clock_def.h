/******************************************************************************
 * @file Clock_def.c
 *
 * @brief Clock control definitions 
 *
 * This file provides the definitions for the clock control
 *
 * @copyright Copyright (c) 2012 Guardhat
 * This document contains proprietary data and information of Guardhat 
 * LLC. It is the exclusive property of Guardhat, LLC and will not be 
 * disclosed in any form to any party without prior written permission of 
 * Guardhat, LLC. This document may not be reproduced or further used 
 * without the prior written permission of Guardhat, LLC.
 *
 * Version History
 * ======
 * $Rev: $
 * 
 *
 * \addtogroup Clock
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _CLOCKDEF_H
#define _CLOCKDEF_H

// system includes ------------------------------------------------------------
#include "Types/Types.h"

// local includes -------------------------------------------------------------

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------
#define CLOCKFEIDEFM( flldiv, outdiv1, outdiv4, intrunonstop ) \
  .eClockMode         = CLOCK_MODESEL_FEIK, \
  .eFllPllSel         = CLOCK_FLLPLLSEL_FLL, \
  .eIntClkSel         = CLOCK_INTSEL_SLOW, \
  .eCoreClkSel        = CLOCK_CORESEL_FLLPLL, \
  .nFllDivider        = flldiv, \
  .nOutDivider1       = outdiv1, \
  .nOutDivider4       = outdiv4, \
  .bEnableIntRefClk   = TRUE, \
  .bIntRefRunOnStop   = intrunonstop
  
#define CLOCKPEEDEFM( oscsel, osccap, flldiv, outdiv1, outdiv4 ) \
  .eClockMode         = CLOCK_MODESEL_PEE, \
  .eClockSelect       = oscsel, \
  .eOscCap            = osccap, \
  .eFllPllSel         = CLOCK_FLLPLLSEL_PLL, \
  .eCoreClkSel        = CLOCK_CORESEL_FLLPLL, \
  .nFllDivider        = flldiv, \
  .nOutDivider1       = outdiv1, \
  .nOutDivider4       = outdiv4, \

#define CLOCKBPLEDEFM( oscsel, osccap, outdiv1, outdiv4 ) \
  .eClockMode         = CLOCK_MODESEL_BPLE, \
  .eClockSelect       = oscsel, \
  .eOscCap            = osccap, \
  .eCoreClkSel        = CLOCK_CORESEL_EXTREF, \
  .nOutDivider1       = outdiv1, \
  .nOutDivider4       = outdiv4, \

#define CLOCKIHSDEFM( outdiv1, outdiv4, intrunonstop ) \
  .eClockMode         = CLOCK_MODESEL_IHS, \
  .eIntClkSel         = CLOCK_INTSEL_FAST, \
  .eCoreClkSel        = CLOCK_CORESEL_INTREF, \
  .nOutDivider1       = outdiv1, \
  .nOutDivider4       = outdiv4, \
  .bEnableIntRefClk   = TRUE, \
  .bIntRefRunOnStop   = intrunonstop

// enumerations ---------------------------------------------------------------
/// enumerate the modes
typedef enum _CLOCKMODESEL
{
  CLOCK_MODESEL_FEI = 0,                ///< FLL enaged internal
  CLOCK_MODESEL_PEE,                    ///< PLL engaged exernal
  CLOCK_MODESEL_BPLE,                   ///< Bypassed Low Power External
  CLOCK_MODESEL_IHS,                    ///< internal high speed
} CLOCKMODESEL;

/// enumerate the oscilator selection
typedef enum _CLOCKOSCSEL
{
  CLOCK_OSCSEL_EXT = 0,                 ///< external clock select.
  CLOCK_OSCSEL_LFLP,                    ///< low frequency( 32KHZ ), low power
  CLOCK_OSCSEL_LFHG,                    ///< low frequency( 32KHZ ), high gain
  CLOCK_OSCSEL_HFLP,                    ///< high frequency( 3-32MHZ ), low power
  CLOCK_OSCSEL_HFHG,                    ///< high frequency( 3-32MHZ ), high gain
  CLOCK_OSCSEL_MAX
} CLOCKOSCSEL;

/// enumerate the oscillator internal capacitance
typedef enum _CLOCKOSCCAP
{
  CLOCK_OSCCAP_NONE = 0,                ///< no capacitance
  CLOCK_OSCCAP_2PF,                     ///< 2PF capacitance
  CLOCK_OSCCAP_4PF,                     ///< 4PF capacitance
  CLOCK_OSCCAP_8PF,                     ///< 8PF capacitance
  CLOCK_OSCCAP_16PF,                    ///< 16PF capacitance
  CLOCK_OSCCAP_MAX
} CLOCKOSCCAP;

/// enumerate the internal clock select
typedef enum _CLOCKINTSEL
{
  CLOCK_INTSEL_SLOW = 0,                ///< slow clock select
  CLOCK_INTSEL_FAST,                    ///< fast clock select
  CLOCK_INTSEL_MAX,
} CLOCKINTSEL;

/// enumerate the core clock select
typedef enum _CLOCKCORESEL
{
  CLOCK_CORESEL_FLLPLL = 0,             ///< FLL/PLL
  CLOCK_CORESEL_INTREF,                 ///< internal reference
  CLOCK_CORESEL_EXTREF,                 ///< external reference
  CLOCK_CORESEL_MAX
} CLOCKCORESEL;

/// enumerate the FLL/PLL select
typedef enum _CLOCKFLLPLLSEL
{
  CLOCK_FLLPLLSEL_FLL = 0,              ///< FLL select
  CLOCK_FLLPLLSEL_PLL,                  ///< PLL select
  CLOCK_FLLPLLSEL_MAX
} CLOCKFLLPLLSEL;

/// enumerate the FLL divide low range
typedef enum _CLOCKFLLDIVLR
{
  CLOCK_FLLDIVLR_1 = 0,                 ///< divide by 1
  CLOCK_FLLDIVLR_2,                     ///< divide by 2
  CLOCK_FLLDIVLR_4,                     ///< divide by 4
  CLOCK_FLLDIVLR_8,                     ///< divide by 8
  CLOCK_FLLDIVLR_16,                    ///< divide by 16
  CLOCK_FLLDIVLR_32,                    ///< divide by 32
  CLOCK_FLLDIVLR_64,                    ///< divide by 64
  CLOCK_FLLDIVLR_128,                   ///< divide by 128
  CLOCK_FLLDIVLR_MAX
} CLOCKFLLDIVLR;

/// enumerate the FLL divide high range
typedef enum _CLOCKFLLDIVHR
{
  CLOCK_FLLDIVHR_32 = 0,                ///< divide by 32
  CLOCK_FLLDIVHR_64,                    ///< divide by 64
  CLOCK_FLLDIVHR_128,                   ///< divide by 128
  CLOCK_FLLDIVHR_256,                   ///< divide by 256
  CLOCK_FLLDIVHR_512,                   ///< divide by 512
  CLOCK_FLLDIVHR_1024,                  ///< divide by 1024
  CLOCK_FLLDIVHR_1280,                  ///< divide by 1280
  CLOCK_FLLDIVHR_1536,                  ///< divide by 1536
  CLOCK_FLLDIVHR_MAX
} CLOCKFLLDIVHR;

// structures -----------------------------------------------------------------
typedef struct _CLOCKDEF
{
  CLOCKMODESEL    eClockMode;           ///< clock mode
  CLOCKOSCSEL     eClockSelect;         ///< clock select
  CLOCKOSCCAP     eOscCap;              ///< capacitance select
  CLOCKFLLPLLSEL  eFllPllSel;           ///< FLL/Pll select
  CLOCKINTSEL     eIntClkSel;           ///< internal clock select
  CLOCKCORESEL    eCoreClkSel;          ///< core clock select
  U8              nFllDivider;          ///< FLL divider
  U8              nOutDivider1;         ///< output 1 divider
  U8              nOutDivider4;         ///< output 4 divider
  BOOL            bEnableIntRefClk;     ///< enable internal reference clock
  BOOL            bIntRefRunOnStop;     ///< enable internal reference run on stop
} CLOCKDEF, *PCLOCKDEF;
#define CLOCKDEF_SIZE               sizeof( CLOCKDEF )

/**@} EOF Clock_def.h */

#endif  // _CLOCKDEF_H