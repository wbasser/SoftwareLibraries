/******************************************************************************
 * @file Clock.h
 *
 * @brief clock module definitions
 *
 * This file provides the clock initializatin and up/down scaling
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
 * $Rev: $
 * 
 *
 * \addtogroup Clock
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _CLOCK_H
#define _CLOCK_H

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------

// library includes -----------------------------------------------------------
#include "Types/Types.h"

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------
/// enumerate the main clock divides
typedef enum  _CLOCKMAINDIV
{
  CLOCK_MAINDIV_1 = 0,          ///< divide main clock by 1
  CLOCK_MAINDIV_2,              ///< divide main clock by 2
  CLOCK_MAINDIV_4,              ///< divide main clock by 4
  CLOCK_MAINDIV_8,              ///< divide main clock by 8
  CLOCK_MAINDIV_16,             ///< divide main clock by 16
  CLOCK_MAINDIV_32,             ///< divide main clock by 32
  CLOCK_MAINDIV_64,             ///< divide main clock by 64
  CLOCK_MAINDIV_128,            ///< divide main clock by 128
  CLOCK_MAINDIV_MAX
} CLOCKMAINDIV;

/// enumerate the clock multiplexer id
typedef enum  _CLOCKMUXID
{
  CLOCK_MUXID_DFLL48M_REF = 0,  ///< DFLL48M reference
  CLOCK_MUXID_DPLL96M,          ///< DFLL96M reference
  CLOCK_MUXID_DPLL32K,          ///< DPLL32K reference
  CLOCK_MUXID_WDT,              ///< watchdog
  CLOCK_MUXID_RTC,              ///< real time clock
  CLOCK_MUXID_EIC,              ///< external interrupt controller
  CLOCK_MUXID_USB,              ///< USB
  CLOCK_MUXID_EVSYSCH0,         /// event system channel 0
  CLOCK_MUXID_EVSYSCH1,         /// event system channel 1
  CLOCK_MUXID_EVSYSCH2,         /// event system channel 2
  CLOCK_MUXID_EVSYSCH3,         /// event system channel 3
  CLOCK_MUXID_EVSYSCH4,         /// event system channel 4
  CLOCK_MUXID_EVSYSCH5,         /// event system channel 5
  CLOCK_MUXID_SERCOM_SLOW,      /// SERCOMx slow
  CLOCK_MUXID_SERCOM_0,         /// SERCOM 0 
  CLOCK_MUXID_SERCOM_1,         /// SERCOM 1 
  CLOCK_MUXID_SERCOM_2,         /// SERCOM 2 
  CLOCK_MUXID_SERCOM_3,         /// SERCOM 3
  CLOCK_MUXID_TCC0,             /// TCC 0
  CLOCK_MUXID_TC1TC2,           /// TC 1 and T2
  CLOCK_MUXID_ADC,              /// ADC
  CLOCK_MUXID_ACDIG,            /// AC - DIG
  CLOCK_MUXID_ACANA,            /// AC - ANA
  CLOCK_MUXID_DAC,              /// DAC
  CLOCK_MUXID_PTC,              /// PTC
} CLOCKMUXID;

/// enumerate the clock gnerator id
typedef enum _CLOCKGENID
{
  CLOCK_GENID_0 = 0,            ///< glock generator 0
  CLOCK_GENID_1,
  CLOCK_GENID_2,
  CLOCK_GENID_3,
  CLOCK_GENID_4,
  CLOCK_GENID_5,
} CLOCKGENID;

/// enumerate the general clock divisor 
typedef enum _CLOCKDIVSEL 
{
  CLOCK_DIVSEL_NUMERIC = 0,   /// general clock divide select - numeric ( CLKSRC / GENDIV.DIV )
  CLOCK_DIVSEL_POWER2         /// general clock divide select - power2 (CLKSRC / 2^(GENDIV.DIV+1))
} CLOCKDIVSEL;

/// enumerate the clock source masks
typedef enum  _CLOCKSRC
{
  CLOCK_SRC_XOSC = 0,             ///< external oscilator
  CLOCK_SRC_GCLKIN,               ///< generator input pad
  CLOCK_SRC_GCLKGEN1,             ///< generic clock generator 1 output
  CLOCK_SRC_OSCULP32K,            ///< low power 32Khz oscillator
  CLOCK_SRC_OSC32K,               ///< 32Khz oscillator
  CLOCK_SRC_XOSC32K,              ///< external 32Khz oscillator
  CLOCK_SRC_OSC8M,                ///< 8Mhz oscillator
  CLOCK_SRC_DFLL48M,              ///< DFPLL 48Mhz
  CLOCK_SRC_DPLL96M,              ///< FDPLL 96Mhz
  CLOCK_SRC_MAX
} CLOCKSRC;

// structures -----------------------------------------------------------------

// global parameter declarations -----------------------------------------------

// global function prototypes --------------------------------------------------
extern  void  Clock_Initialize( CLOCKSRC eClkSrc, U8 nFlashStates, CLOCKMAINDIV eCpuDiv, CLOCKMAINDIV eApbADiv, CLOCKMAINDIV eApbBDiv );
extern  U32   Clock_GetSysFreq( void );
extern  U32   Clock_GetGenClock( CLOCKGENID eGenId );
extern  void  Clock_PeriphEnable( CLOCKMUXID eMuxId, CLOCKGENID eGenId ); 
extern  void  Clock_SetGenClock( CLOCKGENID eGenId, CLOCKSRC eClockSrc, U16 wDivisor, CLOCKDIVSEL eDivSel );

/**@} EOF Clock.h */

#endif  // _CLOCK_H
