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
 
/// define the helper macro to create the main clock definition
#define CLOCKDEFMAINM( cpu, hs, rtcsrc ) \
    .nCpuDiv = cpu, \
    .nHsDiv = hs, \
    .eRtcClkSrc = rtcsrc 
      
/// define the helper macro to create a clock multiplexer definition
#define CLOCKDEFMUXM( id, clkgenid, writelock ) \
  { \
    .eMuxId = id, \
    .eClockGenId = clkgenid, \
    .bWriteLock = writelock, \
  }
  
/// define the helper macro to create the end of table
#define CLOCKDEFMUX_EOT( ) \
 { \
   .eMuxId = CLOCK_MUXID_MAX, \
 }
 
/// define the helper macro to create a Clock generator
#define CLOCKGENDEFM( clkgenid, clksrc, divselect, divisor, standby, outenb, outoffenb, dutycycle ) \
 { \
    .eId = clkgenid, \
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

/// define the helper macro to create the end of tagle
#define CLOCKGENDEF_EOT( ) \
  { \
    .eId = CLOCK_GENID_MAX, \
  }

/// define the helper for defining the external 32K oscillator
#define CLOCKXOSC32DEFM( ondemand, runstby, wrtlock, enb1kout, enb32kout, xtalenb, cfdpre, cfdswb, cfdenb, cfdevn, startup, gain ) \
  { \
    .eOscSel = CLOCK_OSC_OSC32K, \
    .tOscs = \
    { \
      .tOsc32Def = \
      { \
        .tFlags = \
        { \
          .bOnDemand = ondemand, \
          .bRunInStandby = runstby, \
          .bWriteLock = wrtlock, \
          .bEnable1KOut = enb1kout, \
          .bEnable32KOut = enb32kout, \
          .bXtalEnable = xtalenb, \
          .bClkFailPrescDiv2 = cfdpre, \
          .bClkFailSwitchBack = cfdswb, \
          .bClkFailDetEnable = CfdEnb, \
          .bClkFailDetEvent = cfdevn, \
        }, \
        .eClkStartup = startup, \
        .eGainMode = gain, \
      }, \
    }, \
  }

/// define the helper macro for defining an external oscillator
#define CLOCKXOSCDEFM( oscsel, ondemand, runstby, xtalenb, cfdswb, cfdenb, loopenb, lbgenb, cfdpre, startdly, imult, iref ) \
  { \
    .eOscSel = oscsel, \
    .tOscs = \
    { \
      .tXOscDef = \
      { \
        .tFlags = \
        { \
          .bOnDemand = ondemand, \
          .bRunInStandby = runstby, \
          .bXtalEnable = xtalenb, \
          .bClkFailSwitchBack = cfdswb, \
          .bClkFailDetEnable = cfdenb, \
          .bEnableLoopControl = loopenb, \
          .bLowBufGainEnable = lbgenb, \
        }, \
        .eFailPrescale = cfdpre, \
        .eStartup = startdly, \
        .nOscIMultiplier, \
        .nOscIReference, \
      }, \
    }, \
  }
  
/// define the helper macro for defining the DPLL oscillator
#define CLOCKDPLLDEFM( oscsel, ondemand, runstby, filtenb, fastwkenb, lckbypassenb, loopfrac, loopratio, divider, filtsel, lcktim, refclk ) \
  { \
    .eOscSel = oscsel, \
    .tOscs = \
    { \
      .tDpllDef = \
      { \
        .tFlags = \
        { \
          .bOnDemand = ondemand, \
          .bRunInStandby = runstby, \
          .bDCOFiltEnable = filtenb, \
          .bFastWakeEnable = fastwkenb, \
          .bLockByPassEnable = lkbypassenb, \
        }, \
        .nLoopDivRatioFrac = loopfrac, \
        .wLoopDivRatio = loopratio, \
        .wDivider = divider, \
        .eDcoFilerSel = filtsel, \
        .eLockTimeSel = lcktim, \
        .eRefClockSel = refclk, \
      }, \
    }, \
  }

/// define the helper macro for defining the DFLL oscillator
#define CLOCKDFLLDEFM( ondemand, runstby, waitlock, crslckbyp, quicklkdsb, chillcycdsb, usbrecv, loselock, calaftlock, clsenb, multratio, coarse, fine, crsstep, finestep, multi ) \
  { \
    .eOscSel = CLOCK_OSC_DFLL48M, \
    .tOscs = \
    { \
      .tDpllDef = \
      { \
        .tFlags = \
        { \
          .bOnDemand = ondemand, \
          .bRunInStandby = runstby, \
          .bWaitLock = waitlock, \
          .bCoarseLockBypass = crslckbyp, \
          .bQuickLockDisable = quicklkdsb, \
          .bChillCycleDisable = chillcycdsb, \
          .bUsbClockRecoverEnb = usbrecv, \
          .bLoseLockAfterWake = loselock, \
          .bLockCalAfetrLock = calaftlock, \
          .bCloseLoopEnable = cslenb, \
        }, \
        .wMultipierDiffRatio = multratio, \
        .nCoarseValue = coarse, \
        .nFineValue = fine, \
        .nCoarseMaxStep = crsstep, \
        .nFineMaxStep = finestep, \
        .wMultipliser = multi, \
      }, \
    }, \
  }
  
/// define the helper macro for defining the end of the table
#define CLOCKOSCDEF_EOT( ) \
  { \
    .eOscSel = CLOCK_OSC_MAX \
  }


/// define the helper macro for defining peripherals
#define CLOCKPERIPHDEFM( enum, state ) \
  { \
    .ePeriph = enum, \
    .bState = state, \
  } 

/// define the helper macro for defining the end of the table
#define CLOCKPERIPHDEF_EOT( ) \
  { \
    CLOCK_PERIPH_MAX \
  } 
  
// enumerations ---------------------------------------------------------------
/// enumerate the oscillators
typedef enum _CLOCKOSC
{
  CLOCK_OSC_XOSC0 = 0,
  CLOCK_OSC_XOSC1,
  CLOCK_OSC_OSC32K,
  CLOCK_OSC_XOSC32K,
  CLOCK_OSC_DFLL48M,
  CLOCK_OSC_DPLL0,
  CLOCK_OSC_DPLL1,
  CLOCK_OSC_MAX
} CLOCKOSC;

/// enumerate the clock source masks
typedef enum  _CLOCKSRC
{
  CLOCK_SRC_XOSC0= 0,             ///< external oscilator 0
  CLOCK_SRC_XOSC1,                ///< external oscilator 1
  CLOCK_SRC_GCLKIN,               ///< generator input pad
  CLOCK_SRC_GCLKGEN1,             ///< generic clock generator 1 output
  CLOCK_SRC_OSCULP32K,            ///< low power 32Khz oscillator
  CLOCK_SRC_XOSC32K,              ///< external 32Khz oscillator
  CLOCK_SRC_DFLL48M,              ///< DFPLL 48Mhz
  CLOCK_SRC_DPLL0,                ///< DPLL 0
  CLOCK_SRC_DPLL1,                ///< DPLL 1
  CLOCK_SRC_MAX
} CLOCKSRC;

/// enumerate the external 32K oscilaator strupt times
typedef enum  _CLOCKXOSC32KSTARTUP
{
  CLOCK_XOSC32KSTARTUP_2048 = 0,  ///< 2048 cycles
  CLOCK_XOSC32KSTARTUP_4096,      ///< 4096 cycles
  CLOCK_XOSC32KSTARTUP_16384,     ///< 16384 cycles
  CLOCK_XOSC32KSTARTUP_32768,     ///< 32768 cycles
  CLOCK_XOSC32KSTARTUP_65536,     ///< 65536 cycles
  CLOCK_XOSC32KSTARTUP_131072,    ///< 131072 cycles
  CLOCK_XOSC32KSTARTUP_262144,    ///< 262144 cycles
  CLOCK_XOSC32KSTARTUP_MAX
} CLOCKXOSC32KSTARTUP;

/// enumerate the external 32K gain mode
typedef enum  _CLOCKXOSC32KGAINMODE
{
  CLOCK_XOSC32KGAIMMODE_XT = 1,   ///< Standard mode
  CLOCK_XOSC32KGAIMMODE_HS,       ///< High Speed mode
  CLOCK_XOSC32KGAINMODE_MAX
} CLOCKXOSC32KGAINMODE;

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

/// enumerate the DPLL refernece clock source
typedef enum  _CLOCKDPLLREF
{
  CLOCK_DPLLREF_GCLK = 0,         ///< GCLK
  CLOCK_DPLLREF_XOSC32,           ///< external oscilator 32
  CLOCK_DPLLREF_XOSC0,            ///< external oscillator #0
  CLOCK_DPLLREF_XOSC1,            ///< external oscillator #0
  CLOCK_DPLLREF_MAX
} CLOCKDPLLREF;

/// enumerate the DPLL lock time
typedef enum  _CLOCKDPLLLKTIM
{
  CLOCK_DPLLLKTIM_DEFAULT = 0,    ///< default
  CLOCK_DPLLLKTIM_800US = 0x04,   ///< 800 usec
  CLOCK_DPLLLKTIM_900US,          ///< 900 usec
  CLOCK_DPLLLKTIM_1000US,         ///< 1000 usec
  CLOCK_DPLLLKTIM_1100US,         ///< 1100 usec
  CLOCK_DPLLLKTIM_MAX
} CLOCKDPLLLKTIM;


/// enumerate the DFPLL mode
typedef enum _CLOCKDFPLLMODE
{
  CLOCK_DFPLLMODE_OPEN = 0,        ///< open loop mode
  CLOCK_DFPLLMODE_CLOSED,          ///< close loop mode
} CLOCKDFPLLMODE;

/// enumerate the DPLL DCO filter
typedef enum _CLOCKDPLLDCOFILT
{
  CLOCK_DPLLDCOFILT_3210KHZ = 0,  /// 3.21MHZ (0.5pf)
  CLOCK_DPLLDCOFILT_1600KHZ,      /// 1.6MHZ (1pf)
  CLOCK_DPLLDCOFILT_1100KHZ,      /// 1.1MHZ (1.5pf)
  CLOCK_DPLLDCOFILT_800KHZ,       /// 0.8MHZ (2pf)
  CLOCK_DPLLDCOFILT_640KHZ,       /// 0.64MHZ (2.5pf)
  CLOCK_DPLLDCOFILT_550KHZ,       /// 0.55MHZ (3pf)
  CLOCK_DPLLDCOFILT_450KHZ,       /// 0.45MHZ (3.5pf)
  CLOCK_DPLLDCOFILT_400KHZ,       /// 0.4MHZ (4pf)
  CLOCK_DPLLDCOFILT_MAX,
} CLOCKDPLLDCOFILT;

/// enumerate the DPLL PI filter
typedef enum _CLOCKDPLLPIFILT
{
  CLOCK_DPLLPIFILT_0927KHZDF76 = 0,   /// 9.27KHZ (.76)
  CLOCK_DPLLPIFILT_1310KHZ1DF108,     /// 131KHZ (1.08)
  CLOCK_DPLLPIFILT_0464KHZDF038,
  CLOCK_DPLLPIFILT_0656KHZDF054,
  CLOCK_DPLLPIFILT_1310KHZDF056,
  CLOCK_DPLLPIFILT_1850KHZDF079,
  CLOCK_DPLLPIFILT_0656KHZDF028,
  CLOCK_DPLLPIFILT_0927KHZDF039,
  CLOCK_DPLLPIFILT_0464KHZDF149,
  CLOCK_DPLLPIFILT_0656KHZDF211,
  CLOCK_DPLLPIFILT_0232KHZDF075,
  CLOCK_DPLLPIFILT_0328KHZDF106,
  CLOCK_DPLLPIFILT_0656KHZDF107,
  CLOCK_DPLLPIFILT_0927KHZDF151,  
  CLOCK_DPLLPIFILT_0328KHZDF053,
  CLOCK_DPLLPIFILT_0464KHZDF075,
  CLOCK_DPLLPIFILT_MAX,
} CLOCKDPLLPIFILT;

/// enumerate the clock gnerator id
typedef enum _CLOCKGENID
{
  CLOCK_GENID_00 = 0,           ///< glock generator 0
  CLOCK_GENID_01,               ///< glock generator 1
  CLOCK_GENID_02,               ///< glock generator 2
  CLOCK_GENID_03,               ///< glock generator 3
  CLOCK_GENID_04,               ///< glock generator 4
  CLOCK_GENID_05,               ///< glock generator 5
  CLOCK_GENID_06,               ///< glock generator 6
  CLOCK_GENID_07,               ///< glock generator 7
  CLOCK_GENID_08,               ///< glock generator 8
  CLOCK_GENID_09,               ///< glock generator 9
  CLOCK_GENID_10,               ///< glock generator 10
  CLOCK_GENID_11,               ///< glock generator 11
  CLOCK_GENID_MAX               /// end of table
} CLOCKGENID;

/// enumerate the clock multiplexer id
typedef enum  _CLOCKMUXID
{
  CLOCK_MUXID_DFLL48M_REF = 0,  ///< DFLL48M reference
  CLOCK_MUXID_FDPLL0,           ///< DFLL0 reference
  CLOCK_MUXID_FDPLL1,           ///< DFLL1 reference
  CLOCK_MUXID_DPLL32K,          ///< DPLL32K reference
  CLOCK_MUXID_EIC,              ///< external interrupt controller
  CLOCK_MUXID_FREQMSR,          ///< freuqncy measure
  CLOCK_MUXID_FREQREF,          ///< frequency reference
  CLOCK_MUXID_SERCOM_0,         /// SERCOM 0 
  CLOCK_MUXID_SERCOM_1,         /// SERCOM 1 
  CLOCK_MUXID_TC0,              /// TC 0
  CLOCK_MUXID_TC1,              /// TC 1
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
  CLOCK_MUXID_SERCOM_2,         /// SERCOM 2 
  CLOCK_MUXID_SERCOM_3,         /// SERCOM 3 
  CLOCK_MUXID_TCC01,            /// TCC 0 and 1
  CLOCK_MUXID_TC23,             /// TC 2 and 3
  CLOCK_MUXID_CAN0,             /// CAN 0
  CLOCK_MUXID_CAN1,             /// CAN 1
  CLOCK_MUXID_TCC23,            /// TCC 2 and 3
  CLOCK_MUXID_TC45,             /// TC 4 and 5  
  CLOCK_MUXID_PDEC,             /// PDEC
  CLOCK_MUXID_AC,               /// AC
  CLOCK_MUXID_CCL,              /// CCL
  CLOCK_MUXID_SERCOM_4,         /// SERCOM 4 
  CLOCK_MUXID_SERCOM_5,         /// SERCOM 5 
  CLOCK_MUXID_SERCOM_6,         /// SERCOM 6 
  CLOCK_MUXID_SERCOM_7,         /// SERCOM 7 
  CLOCK_MUXID_TCC4,             /// TCC 4
  CLOCK_MUXID_TC67,             /// TC 6 and 7  
  CLOCK_MUXID_ADC0,             /// ADC0
  CLOCK_MUXID_ADC1,             /// ADC1
  CLOCK_MUXID_DAC,              /// DAC
  CLOCK_MUXID_IS20,             /// I2S 0
  CLOCK_MUXID_IS21,             /// I2S 1
  CLOCK_MUXID_SDHC0,            /// SCHC0
  CLOCK_MUXID_SDHC1,            /// SCHC`
  CLOCK_MUXID_CM4TRACE,         /// CM4 Trace
  CLOCK_MUXID_MAX,              /// end of table
} CLOCKMUXID;

/// enumerate the general clock divisor 
typedef enum _CLOCKDIVSEL 
{
  CLOCK_DIVSEL_NUMERIC = 0,   /// general clock divide select - numeric ( CLKSRC / GENDIV.DIV )
  CLOCK_DIVSEL_POWER2         /// general clock divide select - power2 (CLKSRC / 2^(GENDIV.DIV+1))
} CLOCKDIVSEL;

/// enumerate the RTC clock selection
typedef enum _CLOCKRTCSRCSEL
{
  CLOCK_RTCSRCSEL_ULP1K = 0,    ///< 1.024KHZ from 32KHZ internal ULP oscillator
  CLOCK_RTCSRCSEL_ULP32K,       ///< 32.768KHZ from 32KHZ internal ULP oscillator
  CLOCK_RTCSRCSEL_XOSC1K = 4,   ///< 1.024KHZ from 32KHZ extern oscillator
  CLOCK_RTCSRCSEL_XOSC32K,      ///< 32.768KHZ from 32KHZ extern oscillator
  CLOCK_RTCSRCSEL_MAX
} CLOCKRTCSRCSEL;

/// enumerate the clock fail detect prescale
typedef enum _CLOCKFAILPRESCALE
{
  CLOCK_FAILPRESCALE_1 = 0,     ///< clock prescale of 1
  CLOCK_FAILPRESCALE_2,         ///< clock prescale of 2
  CLOCK_FAILPRESCALE_4,         ///< clock prescale of 4
  CLOCK_FAILPRESCALE_8,         ///< clock prescale of 8
  CLOCK_FAILPRESCALE_16,        ///< clock prescale of 16
  CLOCK_FAILPRESCALE_32,        ///< clock prescale of 32
  CLOCK_FAILPRESCALE_64,        ///< clock prescale of 64
  CLOCK_FAILPRESCALE_128,       ///< clock prescale of 128
  CLOCK_FAILPRESCALE_256,       ///< clock prescale of 256
  CLOCK_FAILPRESCALE_512,       ///< clock prescale of 512
  CLOCK_FAILPRESCALE_1024,      ///< clock prescale of 1024
  CLOCK_FAILPRESCALE_2048,      ///< clock prescale of 2048
  CLOCK_FAILPRESCALE_4096,      ///< clock prescale of 4096
  CLOCK_FAILPRESCALE_8192,      ///< clock prescale of 8192
  CLOCK_FAILPRESCALE_16384,     ///< clock prescale of 16384
  CLOCK_FAILPRESCALE_32768,     ///< clock prescale of 32768
  CLOCK_FAILPRESCALE_MAX
} CLOCKFAILPRESCALE;

/// enumerate the peripheral clocks
typedef enum _CLOCKPERIPH
{
  CLOCK_PERIPH_AC = 0,
  CLOCK_PERIPH_ADC0,
  CLOCK_PERIPH_ADC1,
  CLOCK_PERIPH_AES,
  CLOCK_PERIPH_BRIDGEA,
  CLOCK_PERIPH_BRIDGEB,
  CLOCK_PERIPH_BRIDGEC,
  CLOCK_PERIPH_BRIDGED,
  CLOCK_PERIPH_CCL,
  CLOCK_PERIPH_CMCC,
  CLOCK_PERIPH_DAC,
  CLOCK_PERIPH_DMAC,
  CLOCK_PERIPH_DSU,
  CLOCK_PERIPH_EIC,
  CLOCK_PERIPH_EVSYS,
  CLOCK_PERIPH_FREQM,
  CLOCK_PERIPH_GCLK,
  CLOCK_PERIPH_ICM,
  CLOCK_PERIPH_I2S,
  CLOCK_PERIPH_MCLK,
  CLOCK_PERIPH_NVMCTRL,
  CLOCK_PERIPH_OSCCTRL,
  CLOCK_PERIPH_OSC32KCTRL,
  CLOCK_PERIPH_PAC,
  CLOCK_PERIPH_PCC,
  CLOCK_PERIPH_PDEC,
  CLOCK_PERIPH_PM,
  CLOCK_PERIPH_PORT,
  CLOCK_PERIPH_PUKCC,
  CLOCK_PERIPH_QSPI,
  CLOCK_PERIPH_QSPI2X,
  CLOCK_PERIPH_RAMECC,
  CLOCK_PERIPH_RSTC,
  CLOCK_PERIPH_RTC,
  CLOCK_PERIPH_SDHC0,
  CLOCK_PERIPH_SDHC1,
  CLOCK_PERIPH_SERCOM0,
  CLOCK_PERIPH_SERCOM1,
  CLOCK_PERIPH_SERCOM2,
  CLOCK_PERIPH_SERCOM3,
  CLOCK_PERIPH_SERCOM4,
  CLOCK_PERIPH_SERCOM5,
  CLOCK_PERIPH_SERCOM6,
  CLOCK_PERIPH_SERCOM7,
  CLOCK_PERIPH_SUPC,
  CLOCK_PERIPH_TC0,
  CLOCK_PERIPH_TC1,
  CLOCK_PERIPH_TC2,
  CLOCK_PERIPH_TC3,
  CLOCK_PERIPH_TC4,
  CLOCK_PERIPH_TC5,
  CLOCK_PERIPH_TC6,
  CLOCK_PERIPH_TC7,
  CLOCK_PERIPH_TCC0,
  CLOCK_PERIPH_TCC1,
  CLOCK_PERIPH_TCC2,
  CLOCK_PERIPH_TCC3,
  CLOCK_PERIPH_TCC4,
  CLOCK_PERIPH_TRNG,
  CLOCK_PERIPH_USB,
  CLOCK_PERIPH_WDT,
  CLOCK_PERIPH_MAX
} CLOCKPERIPH;

// structures -----------------------------------------------------------------
/// define the main clock structure
typedef struct _CLOCKMAINDEF
{
  U8                nHsDiv;             /// < HS clock divide
  U8                nCpuDiv;            ///< CPU clock divide
  CLOCKRTCSRCSEL    eRtcClkSrc;         ///< RTC clock selection
} CLOCKMAINDEF, *PCLOCKMAINDEF;
#define CLOCKMAINDEF_SIZE           sizeof( CLOCKMAINDEF )

/// define the clock multiplixer definition structure
typedef struct _CLOCKMUXDEF
{
  CLOCKMUXID        eMuxId;             ///< ID
  CLOCKGENID        eClockGenId;        ///< clock generator ID
  BOOL              bWriteLock;         ///< write locked
} CLOCKMUXDEF, *PCLOCKMUXDEF;
#define CLOCKMUXDEF_SIZE            sizeof( CLOCKMUXDEF )

/// define the clock generator definition structure
typedef struct _CLOCKGENDEF
{
  CLOCKGENID        eId;                      ///< ID
  CLOCKSRC          eClockSrc;                ///< clock source
  U16               wDivisor;                 ///< divisor
  struct
  {
    BOOL            bRunInStandby       : 1;  ///< enable run in standby
    BOOL            bOutputEnabled      : 1;  ///< output enabled
    BOOL            bOutOffEnabled      : 1;  ///< output off enabled
    BOOL            bImprovedDutyCycle  : 1;  ///< improved duty cycle
  } tFlags;
  CLOCKDIVSEL       eDivideSelect;            ///< divide select
} CLOCKGENDEF, *PCLOCKGENDEF;
#define CLOCKGENDEF_SIZE            sizeof( CLOCKGENDEF )

/// define the external 32K oscillator definition structure
typedef struct _CLOCKOSC32KDEF
{
  struct  
  {
    BOOL                bOnDemand           : 1;  ///< enable on demand operation
    BOOL                bRunInStandby       : 1;  ///< enable run in standby
    BOOL                bWriteLock          : 1;  ///< enable write lock
    BOOL                bEnable1KOut        : 1;  ///< enable the 1024KHZ output
    BOOL                bEnable32KOut       : 1;  ///< enable the 32KHZ output
    BOOL                bXtalEnable         : 1;  ///< enable the crystal input
    BOOL                bClkFailPrescDiv2   : 1;  ///< enable clock fail prescale divice x 2
    BOOL                bClkFailSwitchBack  : 1;  ///< enable the clock fail switch back
    BOOL                bClkFailDetEnable   : 1;  ///< enable the clock fail detector
    BOOL                bClkFailDetEvent    : 1;  ///< enable the clock fail det event
  } tFlags;
  CLOCKXOSC32KSTARTUP   eClkStartup;      ///< clock startup
  CLOCKXOSC32KGAINMODE  eGainMode;        ///< gain mode  
} CLOCKOSC32KDEF, *PCLOCKOSC32KDEF;
#define CLOCKOSC32KDEF_SIZE         sizeof( CLOCKOSC32KDEF )

/// define the external  oscillator definition structure
typedef struct _CLOCKXOSCKDEF
{
  struct  
  {
    BOOL                bOnDemand           : 1;  ///< enable on demand operation
    BOOL                bRunInStandby       : 1;  ///< enable run in standby
    BOOL                bXtalEnable         : 1;  ///< enable the crystal input
    BOOL                bClkFailSwitchBack  : 1;  ///< enable the clock fail switch back
    BOOL                bClkFailDetEnable   : 1;  ///< enable the clock fail detector
    BOOL                bEnableLoopControl  : 1;  ///< enable auto loop control
    BOOL                bLowBufGainEnable   : 1;  ///< low buffer gain
  } tFlags;
  CLOCKFAILPRESCALE     eFailPrescale;            ///< clock fail prescale
  CLOCKXOSCSTARTUP      eStartup;                 ///< startup up delay
  U8                    nOscIMultiplier;          ///< oscillator I multiplier
  U8                    nOscIReference;           ///< oscillator I reference
} CLOCKXOSCKDEF, *PCLOCKXOSCKDEF;
#define CLOCKXOSCKDEF_SIZE        sizeof( CLOCKXOSCKDEF )

/// define the DPLL definition structure
typedef struct _CLOCKDPLLDEF
{
  struct  
  {
    BOOL                bOnDemand           : 1;  ///< enable on demand operation
    BOOL                bRunInStandby       : 1;  ///< enable run in standby
    BOOL                bDCOFiltEnable      : 1;  ///< enable DCO filter
    BOOL                bFastWakeEnable     : 1;  ///< enable fast wakeup
    BOOL                bLockByPassEnable   : 1;  ///< enable lock bypass
  } tFlags;
  U8                    nLoopDivRatioFrac;        ///< loop divider ratio fractional
  U16                   wLoopDivRatio;            ///< loop divider ratio
  U16                   wDivider;                 ///< clock divider
  CLOCKDPLLDCOFILT      eDcoFilerSel;             ///< DCO filter select
  CLOCKDPLLLKTIM        eLockTimeSel;             ///< lock time select
  CLOCKDPLLREF          eRefClockSel;             ///< reference clock select
} CLOCKDPLLDEF, *PCLOCKDPLLDEF;
#define CLOCKDPLLDEF_SIZE         sizeof( CLOCKDPLLDEF )
  
/// define the DFLL48 definition structure
typedef struct _CLOCKDFLLDEF
{
  struct  
  {
    BOOL                bOnDemand           : 1;  ///< enable on demand operation
    BOOL                bRunInStandby       : 1;  ///< enable run in standby
    BOOL                bWaitLock           : 1;  ///< wait on lock
    BOOL                bCoarseLockBypass   : 1;  ///< bypass coarse lock
    BOOL                bQuickLockDisable   : 1;  ///< disable quick lock
    BOOL                bChillCycleDisable  : 1;  ///< chill cycle disable
    BOOL                bUsbClockRecoverEnb : 1;  ///< USB clock reocver enb
    BOOL                bLoseLockAfterWake  : 1;  ///< lose lock after wake
    BOOL                bLockCalAfetrLock   : 1;  ///< lock cal register after lock
    BOOL                bCloseLoopEnable    : 1;  ///< enable close loop
  } tFlags;
  U16                   wMultipierDiffRatio;      ///< multiplier diff ratio
  U8                    nCoarseValue;             ///< coarse value
  U8                    nFineValue;               ///< fine value
  U8                    nCoarseMaxStep;           ///< coarse max step
  U8                    nFineMaxStep;             ///< fine max step
  U16                   wMultipliser;             ///< multipliser
} CLOCKDFLLDEF, *PCLOCKDFLLDEF;
#define CLOCKDFLLDEF_SIZE         sizeof( CLOCKDFLLDEF )
  
/// combined clock definition structure
typedef struct _CLOCKOSCDEF
{
  CLOCKOSC              eOscSel;
  union
  {
    CLOCKOSC32KDEF      tOsc32Def;
    CLOCKXOSCKDEF       tXOscDef;
    CLOCKDPLLDEF        tDpllDef;
    CLOCKDFLLDEF        tDfllDef;  
  } tOscs;
} CLOCKOSCDEF, *PCLOCKOSCDEF;
#define CLOCKOSCDEF_SIZE          sizeof( CLOCKOSCDEF )

/// define the peripheral clock definition structure
typedef struct _CLOCKPERIPHDEF
{
  CLOCKPERIPH           ePeriph;
  BOOL                  bState;
} CLOCKPERIPHDEF, *PCLOCKPERIPHDEF;
#define CLOCKPERIPHDEF_SIZE       sizeof( CLOCKPERIPHDEF )

/**@} EOF Clock_def.h */

#endif  // _CLOCK_DEF_H
