/******************************************************************************
 * @file  Clock_def.h
 *
 * @brief 
 *
 * This file 
 *
 * @copyright Copyright (c) 2012 CyberIntegration
 * This document contains proprietary data and information of CyberIntegration 
 * LLC. It is the exclusive property of CyberIntegration, LLC and will not be 
 * disclosed in any form to any party without prior written permission of 
 * CyberIntegration, LLC. This document may not be reproduced or further used 
 * without the prior written permission of CyberIntegration, LLC.
 *
 * $Date: $
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
#ifndef _CLOCK_DEF_H
#define _CLOCK_DEF_H

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------

// library includes -----------------------------------------------------------
#include "Types/Types.h"

// Macros and Defines ---------------------------------------------------------
/// define the helper macro to define the clock source
#define CLOCKSRCDEFM( slowclksrc, mainclksrc, mainstart, fastsel, xtalfreq ) \
  .eSlowClkSrc          = slowclksrc, \
  .eMainCLkSrc          = mainclksrc, \
  .nMainStart           = mainstart, \
  .eFastOscSel          = fastsel, \
  .uXtalFreq            = xtalfreq
  
/// define the helpmer macro for master clock definition
#define CLOCKMASTERCLKDEFM( clksrc, prescale, divide, usbplldiv2 ) \
  .eClockSource         = clksrc, \
  .eClockPrescale       = prescale, \
  .eClockDivide         = divide, \
  .bDiv2USBPLL          = usbplldiv2

#define CLOCKPLLDEFM( pllfreq, lockcnt ) \
  .uPllFreq             = pllfreq, \
  .nSlowClockLockCnt    = lockcnt

/// define the helper macro for defning programmable clock generator
#define CLOCKPROGCLKDEFM( clksel, clksrc, prescale ) \
  { \
    .eProgClkEnum     = clksel, \
    .eClockSource     = clksrc, \
    .eClockPrescale   = prescale \
  }
 
/// define the helper macro for defning peripheral clock generator
#define CLOCKPERIPHCLKDEFM( periphid, clksrc, prescale ) \
  { \
    .nPeriphID   = periphid, \
    .eClockSource   = clksrc, \
    .eClockPrescale = prescale \
  }

// enumerations ---------------------------------------------------------------
/// enumerate the slow clock source 
typedef enum  _CLOCKSLOWSRC
{
  CLOCK_SLOW_OSC32K = 0,           ///< 32Khz oscillator
  CLOCK_SLOW_XOSC32K,              ///< external 32Khz oscillator
  CLOCK_SLOW_MAX
} CLOCKSLOWSRC;

/// enumerate the main clock source 
typedef enum  _CLOCKMAINSRC
{
  CLOCK_MAIN_FASTRC = 0,          ///< fast RC
  CLOCK_MAIN_EXTXTAL,             ///< external crystal
  CLOCK_MAIN_MAX
} CLOCKSMAINRC;

/// enumerate the master clock select
typedef enum _CLOCKMASTERSRC
{
  CLOCK_MASTER_SCLK = 0,          ///< select mlow cock
  CLOCK_MASTER_MAINCK,            ///< main clock
  CLOCK_MASTER_UPLLCKDIV,         ///< UPLL clock divided
  CLOCK_MASTER_PLLACK,            ///< PLL A clock
  CLOCK_MASTER_USBPLL,            ///< USB PLL clock
  CLOCK_MASTER_MAX
} CLOCKMASTERSRC;

/// enumerate the main on chipp oscilator freq
typedef enum _CLOCKONCHIPFRQSEL
{
  CLOCK_ONCHIP_FRQ_4MHZ = 0,      ///< 4 MHZ
  CLOCK_ONCHIP_FRQ_8MHZ,          ///< 8 MHZ
  CLOCK_ONCHIP_FRQ_12MHZ,         ///< 12 MHZ
  CLOCK_ONCHIP_FRQ_MAX
} CLOCKONCHIPFRQSEL;

/// enumerate the master clock prescale
typedef enum _CLOCKMASTERPRE
{
  CLOCK_MASTER_PRE_1 = 0,         ///< prescale 1
  CLOCK_MASTER_PRE_2,             ///< prescale 2
  CLOCK_MASTER_PRE_4,             ///< prescale 4
  CLOCK_MASTER_PRE_8,             ///< prescale 8
  CLOCK_MASTER_PRE_16,            ///< prescale 16
  CLOCK_MASTER_PRE_32,            ///< prescale 32
  CLOCK_MASTER_PRE_64,            ///< prescale 64
  CLOCK_MASTER_PRE_3,             ///< prescale 3
  CLOCK_MASTER_PRE_MAX
} CLOCKMASTERPRE;

/// enumerate the master clock divide
typedef enum _CLOCKMASTERDIV
{
  CLOCK_MASTER_DIV_1 = 0,         /// divide by 1
  CLOCK_MASTER_DIV_2,             /// divide by 2
  CLOCK_MASTER_DIV_4,             /// divide by 4
  CLOCK_MASTER_DIV_3,             /// divide by 3
  CLOCK_MASTER_DIV_MAX,
} CLOCKMASTERDIV;

/// enumerate the programmable clocks
typedef enum _CLOCKPRGCLKENUM
{
  CLOCK_PRGCLK_ENUM_0 = 0,        ///< programmable clock 0
  CLOCK_PRGCLK_ENUM_1,            ///< programmable clock 1
  CLOCK_PRGCLK_ENUM_2,            ///< programmable clock 2
  CLOCK_PRGCLK_ENUM_3,            ///< programmable clock 3
  CLOCK_PRGCLK_ENUM_4,            ///< programmable clock 4
  CLOCK_PRGCLK_ENUM_5,            ///< programmable clock 5
  CLOCK_PRGCLK_ENUM_6,            ///< programmable clock 6
  CLOCK_PRGCLK_ENUM_MAX
} CLOCKPRGCLKENUM;

/// enumerate the programmable clock source
typedef enum _CLOCKPRGCLKSRC
{
  CLOCK_PRGCLKSRC_SCLK = 0,          ///< select mlow cock
  CLOCK_PRGCLKSRC_MAINCK,            ///< main clock
  CLOCK_PRGCLKSRC_PLLACK,            ///< PLL A clock
  CLOCK_PRGCLKSRC_UPLLCKDIV,         ///< USB PLL clock
  CLOCK_PRGCLKSRC_MCK,               ///< master clock is selected
  CLOCK_PRGCLKSRC_MAX
} CLOCKPRGCLKSRC;

/// enumerate the programmable clock prescale
typedef enum _CLOCKPRGCLKPRE
{
  CLOCK_PRGCLK_PRE_1 = 0,         ///< prescale 1
  CLOCK_PRGCLK_PRE_2,             ///< prescale 2
  CLOCK_PRGCLK_PRE_4,             ///< prescale 2
  CLOCK_PRGCLK_PRE_8,             ///< prescale 2
  CLOCK_PRGCLK_PRE_16,            ///< prescale 2
  CLOCK_PRGCLK_PRE_32,            ///< prescale 2
  CLOCK_PRGCLK_PRE_64,            ///< prescale 2
  CLOCK_PRGCLK_PRE_MAX
} CLOCKPRGCLKPRE;

// structures -----------------------------------------------------------------
/// define the clock source selection
typedef struct _CLOCKSRCDEF
{
  CLOCKSLOWSRC      eSlowClkSrc;    ///< slow clock source
  CLOCKSMAINRC      eMainCLkSrc;    ///< main clock source
  U8                nMainStart;     ///< main startup time 
  CLOCKONCHIPFRQSEL eFastOscSel;    ///< fast oscillator select
  U32               uXtalFreq;      ///< crystal frequency
} CLOCKSRCDEF, *PCLOCKSRCDEF;
#define CLOCKSRCDEF_SIZE          sizeof( CLOCKSRCDEF )

/// define the master clock definition
typedef struct _CLOCKMASTERCLKDEF
{
	CLOCKMASTERSRC    eClockSource;   ///< clock source
  CLOCKMASTERPRE    eClockPrescale; ///< clock prescale
  CLOCKMASTERDIV    eClockDivide;   ///< clock divide
  BOOL              bDiv2USBPLL;    ///< divide the USB PLL clock by 2
} CLOCKMASTERCLKDEF, *PCLOCKMASTERCLKDEF;
#define CLOCKMASTERCLKDEF_SIZE    sizeof( CLOCKMASTERCLKDEF )

/// define the clock PLL defintion
typedef struct _CLOCKPLLDEF
{
  U32               uPllFreq;           /// PLL frequency
  U8                nSlowClockLockCnt;  ///< slow clock lock count
} CLOCKPLLDEF, *PCLOCKPLLDEF;
#define CLOCKPLLDEF_SIZE          sizeof( CLOCKPLLDEF )

/// define the programmble clock register
typedef struct _CLOCKPROGCLKDEF
{
  CLOCKPRGCLKENUM   eProgClkEnum;     ///< program clock enumeration
	CLOCKPRGCLKSRC    eClockSource;     ///< clock source
  CLOCKPRGCLKPRE    eClockPrescale;   ///< clock prescale
} CLOCKPROGCLKDEF, *PCLOCKPROGCLKDEF;
#define CLOCKPROGCLKDEF_SIZE      sizeof( CLOCKPROGCLKDEF )

/// define the pephrieral clock register
typedef struct _CLOCKPERIPHCLKDEF
{
  U8                nPeriphID;          ///< peripheral ID
	CLOCKPRGCLKSRC    eClockSource;       ///< clock source
  CLOCKPRGCLKPRE    eClockPrescale;     ///< clock prescale
} CLOCKPERIPHCLKDEF, *PCLOCKPERIPHCLKDEF;
#define CLOCKPERIPHCLKDEF_SIZE      sizeof( CLOCKPERIPHCLKDEF )

/**@} EOF Clock_def.h */

#endif  // _CLOCK_DEF_H

