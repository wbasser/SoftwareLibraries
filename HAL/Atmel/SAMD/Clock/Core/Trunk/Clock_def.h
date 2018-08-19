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
/// define the helper macro to create an external osciallator
#define CLOCKOSCDEFXOSC( ondemand, standby, exttype, extfreq, startup, autogain ) \
  { \
    .eClockOsc = CLOCK_OSC_EXT, \
    .tFlags.bOnDemand = ondemand, \
    .tFlags.bRunInStandby = standby, \
    .eExtType = exttype, \
    .uExtFreq = extfreq, \
    .nStartupTime = startup, \
    .btFlags.AutoAmpGain = autogain \
  }
  
/// define the helper macro to create an internal 32Khz oscillator
#define CLOCKOSCDEF32KINT( ondemand, standby, out1kenb, out32kenb, id, outenab, outoffenb, impdtycyc, divselect, divisor ) \
  { \
    .eClockOsc = CLOCK_OSC_INT32K, \
    .tFlags = \
    { \
      .bOnDemand = ondemand, \
      .bRunInStandby = standby, \
      .bEnable1KOut = out1kenb, \
      .bEnable32KOut = out32kenb, \
    }, \
    .tOutGenDef = \
    { \
      .nId = id, \
      .eClockSrc = CLOCK_SRC_OSC32K, \
      .tFlags = \
      { \
        .bOutputEnabled = outenab, \
        .bOutOffEnabled = outoffenb, \
        .bImprovedDutyCycle = impdtycyc, \
      }, \
      .eDivideSelect = divselect, \
      .wDivisor = divisor, \
    }, \
  }
  
/// define the helper macro to create an external 32Khz oscillator
#define CLOCKOSCDEF32KEXT( ondemand, standby, out1kenb, out32kenb, exttype, extfreq, startup, autoamp ) \
  { \
    .eClockOsc = CLOCK_OSC_EXT32K, \
    .tFlags.bOnDemand = ondemand, \
    .tFlags.bRunInStandby = standby, \
    .tFlags.bEnable1KOut = out1kenb, \
    .tFlags.bEnable32KOut = out32kenb, \
    .eExtType = exttype, \
    .uExtFreq = extfreq, \
    .nStartupTime = startup, \
    .tFlags.bAutoAmpGain = autogamp, \
  }
  
/// define the helper macro to creeate an internal 8MHZ oscillator
#define CLOCKOSCDEF8MHZ( ondemand, standby, outdiv, id, outenab, outoffenb, impdtycyc, divselect, divisor ) \
  { \
    .eClockOsc =  CLOCK_OSC_INT8MZ, \
    .tFlags = \
    { \
      .bOnDemand = ondemand, \
      .bRunInStandby = standby, \
    }, \
    .e8MhzDiv = outdiv, \
    .tOutGenDef = \
    { \
      .nId = id, \
      .eClockSrc = CLOCK_SRC_OSC8M, \
      .tFlags = \
      { \
        .bOutputEnabled = outenab, \
        .bOutOffEnabled = outoffenb, \
        .bImprovedDutyCycle = impdtycyc, \
      }, \
      .eDivideSelect = divselect, \
      .wDivisor = divisor, \
    }, \
  }
  
/// define the helper macro to create an DFLL oscillator open loop mode
#define CLOCKOSCDEFDFLLOPEN( mult, coarse, fine, coarsestep, finestep, ondemand, usbrecover, quicklock, tracklock, lockonwakeup, chillcycle ) \
  { \
    .eClockOsc = CLOCK_OSC_DFLL, \
    .eDfpllLoopMode = CLOCK_DFPLLMODE_OPEN, \
    .wDfllMulitplier = mult, \
    .nDfllCoarse = coarse, \
    .wDfllFine = fine, \
    .nDfllCoarseStep = coarsestep, \
    .wDfllFineStep = finestep, \
    .tFlags = \
    { \
      .bClosedLoopMode = OFF, \
      .bOnDemand = ondemand, \
      .bEnableUsbRecovery = usbrecover, \
      .bQuickLock = quicklock, \
      .bTrackAfterFineLock = tracklock, \
      .bKeepLockWakeup = lockonwakeup, \
      .bEnableChillCycle = chillcycle, \
    }, \
  }

/// define the helper macro to create an DFLL oscillator open loop mode
#define CLOCKOSCDEFDFLLCLOSED( ondemand, usbrecover, quicklock, tracklock, lockonwakeup, chillcycle, refid, refsrc, refoutenab, refoutoffenb, refimpdtycyc, refdivselect, refdivisor, outid, outoutenab, outoutoffenb, outimpdtycyc, outdivselect, outdivisor ) \
  { \
    .eClockOsc = CLOCK_OSC_DFLL, \
    .eDfpllLoopMode = CLOCK_DFPLLMODE_CLOSED, \
    .tFlags = \
    { \
      .bClosedLoopMode = ON, \
      .bOnDemand = ondemand, \
      .bEnableUsbRecovery = usbrecover, \
      .bQuickLock = quicklock, \
      .bTrackAfterFineLock = tracklock, \
      .bKeepLockWakeup = lockonwakeup, \
      .bEnableChillCycle = chillcycle, \
    }, \
    .tRefGenDef = \
    { \
      .nId = refid, \
      .eClockSrc = refsrc, \
      .tFlags = \
      { \
        .bOutputEnabled = refoutenab, \
        .bOutOffEnabled = refoutoffenb, \
        .bImprovedDutyCycle = refimpdtycyc, \
      }, \
      .eDivideSelect = refdivselect, \
      .wDivisor = refdivisor, \
    }, \
    .tOutGenDef = \
    { \
      .nId = outid, \
      .eClockSrc = CLOCK_SRC_DFLL48M, \
      .tFlags = \
      { \
        .bOutputEnabled = outoutenab, \
        .bOutOffEnabled = outoutoffenb, \
        .bImprovedDutyCycle = outimpdtycyc, \
      }, \
      .eDivideSelect = outdivselect, \
      .wDivisor = outdivisor, \
    }, \
  }

/// define the helper macro to create an DPLL oscillator open loop mode
#define CLOCKOSCDEFDPLLOPEN( ondeamnd, standby, lock, wakeup, lpenb, filter, ref, div, outfreq ) \
  { \
    .eClockOsc = CLOCK_OSC_DFLL, \
    .eDpllLoopMode = CLOCK_DPLLMODE_OPEN, \
    .tFlags.bOnDeamnd = ondemand, \
    .tFlags.bRunInStandby = standby, \
    .tFlags.bLockBypass = lock, \
    .tFlags.bFastWakeUp = wakeup, \
    .tFlags.bLowPowerEnb = lpenb, \
    .eDpllFilter = filter, \
    .uExtFreq = ref, \
    .wReferenceDiv = div, \
    .uOutFreq = outfreq \
  }
  
/// define the helper macro to create the end of table
#define CLOCKOSCDEF_EOT( ) \
  { \
    .eClockOsc = CLOCK_OSC_EOT \
  }
  
/// define the helper macro to create the main clock definition
#define CLOCKDEFMAIN( cpu, apba, apbb, flash ) \
    .eCpuDiv = cpu, \
    .eAPBADiv = apba, \
    .eAPBBDiv = apbb, \
    .nFlashWaitStates = flash
      
/// define the helper macro to create a clock multiplexer definition
#define CLOCKDEFMUX( id, clkgenid, writelock ) \
  { \
    .eMuxId = id, \
    .eClockGenId = clkgenid, \
    .bWriteLock = writelock, \
  }
  
/// define the helper macro to create the end of table
#define CLOCKDEFMUX_EOT( ) \
 { \
   .eMuxId = CLOCK_MUXID_EOT, \
 }
 
/// define the helper macro to create a Clock generator
#define CLOCKGENDEF( clkgenid, clksrc, divselect, divisor, standby, outenb, outoffenb, dutycycle ) \
 { \
    .nId = clkgenid, \
    .eClockSrc = clksrc, \
    .wDivisor = divisor, \
    .tFlags = \
    { \
      .bRunInStandby = standby, \
      .bOutputEnabled = outenb, \
      .bOutOffEnabled = outoffenb, \
      .bImprovedDutyCycle = dutycycle, \
    }, \
    .eDivideSelect = divselect \
  }

/// define the help macro to create the end of tagle
#define CLOCKGENDEF_EOT( ) \
  { \
    .nId = CLOCK_GENID_EOT, \
  }
 
// enumerations ---------------------------------------------------------------
/// enumerate the clock oscillators
typedef enum _CLOCKOSC
{
  CLOCK_OSC_EXT= 0,               ///< external oscillator
  CLOCK_OSC_INT32K,               ///< internal 32KHz oscillator
  CLOCK_OSC_EXT32K,               ///< external 32KHz oscillator
  CLOCK_OSC_INTLP32K,             ///< internal 32Khz oscillator, low power
  CLOCK_OSC_INT8MZ,               ///< internal 8Mhz oscillator
  CLOCK_OSC_DFLL,                 ///< digital frequency locked loop
  CLOCK_OSC_DPLL,                 ///< digital phase locked loop, fractional
  CLOCK_OSC_MAX,
  CLOCK_OSC_EOT
} CLOCKOSC;

/// enumerate the clock source masks
typedef enum  _CLOCKSRC
{
  CLOCK_SRC_XOSC = 0,              ///< external oscilator
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

/// enumerate the external clock source type
typedef enum  _CLOCKEXTTYPE
{
  CLOCK_EXTTYPE_CLOCK = 0,         ///< clock
  CLOCK_EXTTYPE_CRYSTAL,          ///< crystal
  CLOCK_EXTTYPE_MAX
} CLOCKEXTTYPE;

/// enumerate the external 32K oscilaator strupt times
typedef enum  _CLOCKXOSC32KSTARTUP
{
  CLOCK_XOSC32KSTARTUP_0 = 0,     ///< no startup delay
  CLOCK_XOSC32KSTARTUP_32,        ///< 32 cycles
  CLOCK_XOSC32KSTARTUP_2048,      ///< 2048 cycles
  CLOCK_XOSC32KSTARTUP_4096,      ///< 4096 cycles
  CLOCK_XOSC32KSTARTUP_16384,     ///< 16384 cycles
	CLOCK_XOSC32KSTARTUP_32768,     ///< 32768 cycles
	CLOCK_XOSC32KSTARTUP_65536,     ///< 65536 cycles
	CLOCK_XOSC32KSTARTUP_131072,    ///< 131072 cycles
  CLOCK_XOSC32KSTARTUP_MAX
} CLOCKXOSC32KSTARTUP;

/// enumerate the external 32K oscilaator strupt times
typedef enum  _CLOCKXOSCSTARTUP
{
  CLOCK_XOSCSTARTUP_0 = 0,        ///< no startup delay
  CLOCK_XOSCSTARTUP_1,            ///< 1 cycle
  CLOCK_XOSCSTARTUP_2,            ///< 2 cycles
  CLOCK_XOSCSTARTUP_4,            ///< 4 cycles
  CLOCK_XOSCSTARTUP_8,            ///< 8 cycles
  CLOCK_XOSCSTARTUP_16,           ///< 16 clces
  CLOCK_XOSCSTARTUP_32,           ///< 32 cycles
  CLOCK_XOSCSTARTUP_64,           ///< 64 cycles
  CLOCK_XOSCSTARTUP_128,          ///< 128 cycles
  CLOCK_XOSCSTARTUP_256,          ///< 256 cycles
  CLOCK_XOSCSTARTUP_512,          ///< 512 cycles
  CLOCK_XOSCSTARTUP_1024,         ///< 1024 cycles
  CLOCK_XOSCSTARTUP_2048,         ///< 2048 cycles
  CLOCK_XOSCSTARTUP_4096,         ///< 4086 cycles
  CLOCK_XOSCSTARTUP_8192,         ///< 8192 cycles
  CLOCK_XOSCSTARTUP_16384,        ///< 16384 cycles
  CLOCK_XOSCSTARTUP_32768,        ///< 32768 cycles
  CLOCK_XOSCSTARTUP_MAX
} CLOCKXOSCSTARTUP;

/// enumerate the internal 32K oscilaator strupt times
typedef enum  _CLOCKOSC32KSTARTUP
{
  CLOCK_OSC32KSTARTUP_3 = 0,      ///< 3 cycles
  CLOCK_OSC32KSTARTUP_4,          ///< 4 cycles
  CLOCK_OSC32KSTARTUP_6,          ///< 6 cycles
  CLOCK_OSC32KSTARTUP_10,         ///< 10 clces
  CLOCK_OSC32KSTARTUP_18,         ///< 18 cycles
  CLOCK_OSC32KSTARTUP_34,         ///< 34 cycles
  CLOCK_OSC32KSTARTUP_66,         ///< 66 cycles
  CLOCK_OSC32KSTARTUP_130,        ///< 130 cycles
  CLOCK_OSC32KSTARTUP_MAX
} CLOCKOSC32KSTARTUP;

/// enumerate the internal 8Mhz oscillator divide output
typedef enum  _CLOCK8MHZOUTDIV
{
  CLOCK_8MHZ_OUTDIV_1 = 0,        ///< divide by 1
  CLOCK_8MHZ_OUTDIV_2,            ///< divide by 2
  CLOCK_8MHZ_OUTDIV_4,            ///< divide by 4
  CLOCK_8MHZ_MAX
} CLOCK8MHZOUTDIV;


/// enumerate the DPLL refernece clock source
typedef enum  _CLOCKDPLLREF
{
  CLOCK_DPLLREF_REF0 = 0,         ///< reference 0
  CLOCK_DPLLREF_REF1,             ///< reference 1
  CLOCK_DPLLREF_GCLK,             ///< GLOCK
  CLOCK_DPLLREF_MAX
} CLOCKDPLLREF;

/// enumerate the DPLL lock time
typedef enum  _CLOCKDPLLLKTIM
{
  CLOCK_DPLLLKTIM_DEFAULT = 0,    ///< default
  CLOCK_DPLLLKTIM_8MS = 0x04,     ///< 8 msec
  CLOCK_DPLLLKTIM_9MS,            ///< 9 msec
  CLOCK_DPLLLKTIM_10MS,           ///< 10 msec
  CLOCK_DPLLLKTIM_11MS,           ///< 11 msec
  CLOCK_DPLLLKTIM_MAX
} CLOCKDPLLLKTIM;

/// enumerate the DPLL filter type
typedef enum  _CLOCKDPLLFILT
{
  CLOCK_DPLLFILTER_DEFAULT = 0,   ///< default
  CLOCK_DPLLFILTER_LOBAND,        ///< low bandwidth
  CLOCK_DPLLFILTER_HIBAND,        ///< high bandwidth
  CLOCK_DPLLFILTER_HIDAMP,        ///< high damping
  CLOCK_DPLLFILTER_MAX
} CLOCKDPLLFILT;

/// enumerate the DFPLL mode
typedef enum _CLOCKDFPLLMODE
{
  CLOCK_DFPLLMODE_OPEN = 0,        ///< open loop mode
  CLOCK_DFPLLMODE_CLOSED,          ///< close loop mode
} CLOCKDFPLLMODE;

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

/// enumerate the clock gnerator id
typedef enum _CLOCKGENID
{
  CLOCK_GENID_0 = 0,            ///< glock generator 0
  CLOCK_GENID_1,
  CLOCK_GENID_2,
  CLOCK_GENID_3,
  CLOCK_GENID_4,
  CLOCK_GENID_5,
  CLOCK_GENID_6,
  CLOCK_GENID_7,
  CLOCK_GENID_EOT = 0xFF,       /// end of table
} CLOCKGENID;

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
  CLOCK_MUXID_EVSYSCH6,         /// event system channel 6
  CLOCK_MUXID_EVSYSCH7,         /// event system channel 7
  CLOCK_MUXID_EVSYSCH8,         /// event system channel 8
  CLOCK_MUXID_EVSYSCH9,         /// event system channel 9
  CLOCK_MUXID_EVSYSCH10,        /// event system channel 10
  CLOCK_MUXID_EVSYSCH11,        /// event system channel 11
  CLOCK_MUXID_SERCOM_SLOW,      /// SERCOMx slow
  CLOCK_MUXID_SERCOM_0,         /// SERCOM 0 
  CLOCK_MUXID_SERCOM_1,         /// SERCOM 1 
  CLOCK_MUXID_SERCOM_2,         /// SERCOM 2 
  CLOCK_MUXID_SERCOM_3,         /// SERCOM 3 
  CLOCK_MUXID_SERCOM_4,         /// SERCOM 4 
  CLOCK_MUXID_SERCOM_5,         /// SERCOM 5 
  CLOCK_MUXID_TCC01,            /// TCC 0 and 1
  CLOCK_MUXID_TCC2TC3,          /// TCC 2 and TC3
  CLOCK_MUXID_TC45,             /// TC 4 and 5
  CLOCK_MUXID_TC67,             /// TC 6 and 7
  CLOCK_MUXID_ADC,              /// ADC
  CLOCK_MUXID_ACDIG,            /// AC - DIG
  CLOCK_MUXID_ACANA,            /// AC - ANA
  CLOCK_MUXID_DAC,              /// DAC
  CLOCK_MUXID_PTC,              /// PTC
  CLOCK_MUXID_IS20,             /// I2S 0
  CLOCK_MUXID_IS21,             /// I2S 1
  CLOCK_MUXID_EOT = 0xFF,       /// end of table
} CLOCKMUXID;

/// enumerate the general clock divisor 
typedef enum _CLOCKDIVSEL 
{
  CLOCK_DIVSEL_NUMERIC = 0,   /// general clock divide select - numeric ( CLKSRC / GENDIV.DIV )
  CLOCK_DIVSEL_POWER2         /// general clock divide select - power2 (CLKSRC / 2^(GENDIV.DIV+1))
} CLOCKDIVSEL;

// structures -----------------------------------------------------------------
/// define the clock generator definition structure
typedef struct _CLOCKGENDEF
{
  U8                nId;                      ///< ID
  CLOCKSRC          eClockSrc;                ///< clock source
  U16               wDivisor;                 //< divisor
  struct
  {
    BOOL            bRunInStandby       : 1;  ///< enable run in standby
    BOOL            bOutputEnabled      : 1;  ///< output enabled
    BOOL            bOutOffEnabled      : 1;  ///< output off enabled
    BOOL            bImprovedDutyCycle  : 1;  ///< improved duty cycle
  } tFlags;
  CLOCKDIVSEL       eDivideSelect;            ///< divide select
} CLOCKGENDEF, *PCLOCKGENDEF;
#define CLOCKGENDEF_SIZE      sizeof( CLOCKGENDEF )

/// define the clock definition structure
typedef struct _CLOCKOSCDEF
{
  CLOCKOSC          eClockOsc;          ///< oscillator type
  struct  
  {
    BOOL            bOnDemand           : 1;  ///< enable on demand operation
    BOOL            bRunInStandby       : 1;  ///< enable run in standby
    BOOL            bLockBypass         : 1;  ///< enable lock bypass
    BOOL            bFastWakeUp         : 1;  ///< enable fast wakeup
    BOOL            bLowPowerEnb        : 1;  ///< enabe low power operation
    BOOL            bAutoAmpGain        : 1;  ///< enable automatic gain/amplitude
    BOOL            bEnable1KOut        : 1;  ///< enable the 1024KHZ output
    BOOL            bEnable32KOut       : 1;  ///< enable the 32KHZ output
    BOOL            bQuickLock          : 1;  ///< quick lock enable
    BOOL            bTrackAfterFineLock : 1;  ///< track after fine lock
    BOOL            bKeepLockWakeup     : 1;  ///< keep lock on after wakeup
    BOOL            bEnableChillCycle   : 1;  ///< enable chill cycle
    BOOL            bEnableUsbRecovery  : 1;  ///< enable USB recovery
    BOOL            bClosedLoopMode     : 1;  ///< closed loop mode
  } tFlags;
  U32               uExtFreq;           ///< external/reference frequency
  U32               uOutFreq;           ///< output frequency
  U8                nStartupTime;       ///< startup time
  CLOCKEXTTYPE      eExtType;           ///< external clock type
  CLOCK8MHZOUTDIV   e8MhzDiv;           ///< 8MHZ divide
  U16               wReferenceDiv;      ///< reference divider
  U16               wDfllMulitplier;    ///< DFLL multiplier
  U8                nDfllCoarse;        ///< DFLL coarse
  U16               wDfllFine;          ///< DFLL fine
  U8                nDfllCoarseStep;    ///< DFLL coarse step size
  U16               wDfllFineStep;      ///< DLFF fine step size
  CLOCKDFPLLMODE    eDfpllLoopMode;     ///< DFLL/DPLL loop mode
  CLOCKDPLLREF      eDpllRefClk;        ///< DPLL reference clock
  CLOCKDPLLLKTIM    eDpllLockTime;      ///< DPLL lock time
  CLOCKDPLLFILT     eDpllFilter;        ///< DPLL filter type
  CLOCKGENDEF       tRefGenDef;         ///< refenence clock generator definition
  CLOCKGENDEF       tOutGenDef;         ///< clock output generator definition
} CLOCKOSCDEF, *PCLOCKOSCDEF;
#define CLOCKOSCDEF_SIZE     sizeof( CLOCKOSCDEF )

/// define the main clock structure
typedef struct _CLOCKMAINDEF
{
  CLOCKMAINDIV      eCpuDiv;          ///< CPU divider
  CLOCKMAINDIV      eAPBADiv;         ///< APBA divider
  CLOCKMAINDIV      eAPBBDiv;         ///< APBB divider
  U8                nFlashWaitStates; ///< number of flash wait states
} CLOCKMAINDEF, *PCLOCKMAINDEF;
#define CLOCKMAINDEF_SIZE     sizeof( CLOCKMAINDEF )

/// define the clock multiplixer definition structure
typedef struct _CLOCKMUXDEF
{
  CLOCKMUXID        eMuxId;             ///< ID
  CLOCKGENID        eClockGenId;        ///< clock generator ID
  BOOL              bWriteLock;         ///< write locked
} CLOCKMUXDEF, *PCLOCKMUXDEF;
#define CLOCKMUXDEF_SIZE      sizeof( CLOCKMUXDEF )

/**@} EOF Clock_def.h */

#endif  // _CLOCK_DEF_H

