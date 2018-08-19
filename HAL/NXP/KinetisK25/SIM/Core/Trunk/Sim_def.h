/******************************************************************************
 * @file Sim_def.h
 *
 * @brief System Interface Modules (SIM) definitions 
 *
 * This file provides the definitions for the SIM module
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
 * \addtogroup Sim
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _SIM_DEF_H
#define _SIM_DEF_H

// system includes ------------------------------------------------------------
#include "Types/Types.h"

// local includes -------------------------------------------------------------

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------
/// helper macro to define the option control register #1
#define SIMDEFOPT1CFGM( usbvrenb, usbsbyenb, usbstopenb ) \
  .tFields = \
  { \
    .bUsbVREnb      = usbvrenb, \
    .bUsbVRStbyEnb  = usbsbyenb, \
    .bUsbStopEnb    = usbstopenb, \
  }

/// helper macro to define the option control register #2
#define SIMDEFOPT2M( rtcclkout, clkoutsel, pllfllsel, usbsource, tpmclksrc, uartclksrc ) \
  .tFields = \
  { \
    .bRTCClockOut  = rtcclkout, \
    .uClockOutSel  = clkoutsel, \
    .bPllFllSel    = pllfllsel, \
    .bUsbSrouce    = usbsource, \
    .uTpmClkSrcSel = tpmclksrc, \
    .uUarClkSrcSel = uartclksrc, \
 }

/// helper macro to define the clock geting control register 4
#define SIMDEFCR4M( i2c0, i2c1, uart0, uart1, uart2, usb, comp, spi0, spi1 ) \
  .tFields = \
  { \
    .bI2C0Enable    = i2c0, \
    .bI2C1Enable    = i2c1, \
    .bUart0Enable   = uart0, \
    .bUart1Enable   = uart1, \
    .bUart2Enable   = uart2, \
    .bUSBEnable     = usb, \
    .bCompEnable    = comp, \
    .bSPI0Enable    = spi0, \
    .bSPI1Enable    = spi1, \
  }

// helper macro to define the clock gating control register 5
#define SIMDEFCR5M( lptmr, tsi, porta, portb, portc, portd, porte ) \
  .tFields = \
  { \
    .bLPTmrEnable   = lptmr, \
    .bTSIEnable     = tsi, \
    .bPortAEnable   = porta, \
    .bPortBEnable   = portb, \
    .bPortCEnable   = portc, \
    .bPortDEnable   = portd, \
    .bPortEEnable   = porte, \
  }

/// helper macro to define the clock gating control register 6
#define SIMDEFCR6M( ftf, dmamux, pit, tpm0, tpm1, tpm2, adc0, rtc, dac0 ) \
  .tFields = \
  { \
    .bFTFEnable    = ftf, \
    .bDMAMuxEnable = dmamux, \
    .bPITEnable    = pit, \
    .bTPM0Enable   = tpm0, \
    .bTPM1Enable   = tpm1, \
    .bTPM2Enable   = tpm2, \
    .bADC0Enable   = adc0, \
    .bRTCEnable    = rtc, \
    .bDAC0Enable   = dac0, \
  }
  
/// helper macro to define the clock gating control register 7
#define SIMDEFCR7M( dma ) \
  .tFields = \
  { \
    .bDMAEnable    = dma, \
  }

// enumerations ---------------------------------------------------------------
/// enumerate the clock out select
typedef enum _SIMCLKOUTSEL
{
  SIM_CLKOUTSEL_RES1 = 0,             ///< reserved 1
  SIM_CLKOUTSEL_RES2,                 ///< reserved 2
  SIM_CLKOUTSEL_BUSCLK,               ///< bus clock
  SIM_CLKOUTSEL_LPOCLK,               ///< Low Power Clock out
  SIM_CLKOUTSEL_MCGIRCLK,             ///< MCGIRCLK
  SIM_CLKOUTSEL_RES3,                 ///< reserved 3
  SIM_CLKOUTSEL_OSCERCLK,             ///< OSCERCLK 
} SIMCLKOUTSEL;

/// enumerate the TPM/UART clock source
typedef enum _SIMPERCLKSEL
{
  SIM_PERSCLKSEL_DISABLED = 0,        ///< disabled
  SIM_PERCLKSEL_FLLPLLCLK,            ///< FLL/PLL clock
  SIM_PERCLKSEL_OSCERCLK,             ///< OSCER clock
  SIM_PERCLKSEL_MCGIRCLK,             ///< MCGIRLK
} SIMPERCLKSEL;

// structures -----------------------------------------------------------------
/// define the structure for the Option 1 control register
typedef union _SIMDEFOPT1CFG
{
  struct
  {
    U32   uReserved1    :  24;        ///< reserved 1
    U32   bUsbVREnb     :   1;        ///< USB Voltage Regulator Write Enable
    U32   bUsbVRStbyEnb :   1;        ///< USB voltage regulator standby write enable
    U32   bUsbStopEnb   :   1;        ///< USB stop standby
  } tFields;
  U32 uValue;
} SIMDEFOPT1CFG, *PSIMDEFOPT1CFG;
#define SIMDEFOPT1CFG_SIZE          sizeof( SIMDEFOPT1CFG )

/// define the structure for the Option 2 control register
typedef union _SIMDEFOPT2
{
  struct
  {
    U32           uReserved1    :   4;        ///< reserved 1
    U32           bRTCClockOut  :   1;        ///< RTC clock out select
    SIMCLKOUTSEL  uClockOutSel  :   3;        ///< clock out select
    U32           uReserved2    :   8;        ///< reserved 2
    U32           bPllFllSel    :   1;        ///< FLL/PLL2 select
    U32           bReserved3    :   1;        ///< reserved 3
    U32           bUsbSrouce    :   1;        ///< USB source
    U32           uReserved4    :   4;        ///< reserved 4
    SIMPERCLKSEL  uTpmClkSrcSel :   2;        ///< TPM clock source
    SIMPERCLKSEL  uUarClkSrcSel :   2;        ///< UART0 clock source
  } tFields;
  U32 uValue;
} SIMDEFOPT2, *PSIMDEFOPT2;
#define SIMDEFOPT2_SIZE          sizeof( SIMDEFOPT2 )

/// define the structure for the clock gating control register 4
typedef union _SIMDEFCR4
{
  struct
  {
    U32   uReserved1    :  6;         ///< reserved 1
    U32   bI2C0Enable   :  1;         ///< I2C0 clock enable
    U32   bI2C1Enable   :  1;         ///< I2C1 clock enable
    U32   uReserved2    :  2;         ///< reserved 2
    U32   bUart0Enable  :  1;         ///< UART0 clock enable
    U32   bUart1Enable  :  1;         ///< UART1 clock enable
    U32   bUart2Enable  :  1;         ///< UART2 clock enable
    U32   uReserved3    :  5;         ///< reserved 3
    U32   bUSBEnable    :  1;         ///< USB clock enable
    U32   bCompEnable   :  1;         ///< Comparator Clock Enable
    U32   uReserved4    :  2;         ///< reserved 4
    U32   bSPI0Enable   :  1;         ///< SPI0 clock enable
    U32   bSPI1Enable   :  1;         ///< SPI1 clock enable
  } tFields;
  U32 uValue;
} SIMDEFCR4, *PSIMDEFCR4;
#define SIMDEFCR4_SIZE                sizeof( SIMDEFCR4 )

/// define the structure for the clock gating control register 5
typedef union _SIMDEFCR5
{
  struct
  {
    U32   bLPTmrEnable  :  1;         ///< LPTimer
    U32   uReserved1    :  4;         ///< reserved 1
    U32   bTSIEnable    :  1;         ///< Touch Screen Interface clock enable
    U32   uReserved2    :  3;         ///< reserved 2
    U32   bPortAEnable  :  1;         ///< PortA clock enable
    U32   bPortBEnable  :  1;         ///< PortB clock enable
    U32   bPortCEnable  :  1;         ///< PortC clock enable
    U32   bPortDEnable  :  1;         ///< PortD clock enable
    U32   bPortEEnable  :  1;         ///< PortE clock enable
  } tFields;
  U32 uValue;
} SIMDEFCR5, *PSIMDEFCR5;
#define SIMDEFCR5_SIZE                sizeof( SIMDEFCR5 )

/// define the structure for the clock gating control register 6
typedef union _SIMDEFCR6
{
  struct
  {
    U32   bFTFEnable    :  1;         ///< Flash clock enable
    U32   bDMAMuxEnable :  1;         ///< DMA Mux clock enable
    U32   uReserved1    : 21;         ///< reserved 1
    U32   bPITEnable    :  1;         ///< Periodic Interval TImer clock enable
    U32   bTPM0Enable   :  1;         ///< Timer/PWM0 clock enable
    U32   bTPM1Enable   :  1;         ///< Timer/PWM1 clock enable
    U32   bTPM2Enable   :  1;         ///< Timer/PWM2 clock enable
    U32   bADC0Enable   :  1;         ///< ADC0 clock enable
    U32   uReserved2    :  1;         ///< reserved 2
    U32   bRTCEnable    :  1;         ///< RTC clock enable
    U32   uReserved3    :  1;         ///< reserved 3
    U32   bDAC0Enable   :  1;         ///< DAC0 clock enable
  } tFields;
  U32 uValue;
} SIMDEFCR6, *PSIMDEFCR6;
#define SIMDEFCR6_SIZE                sizeof( SIMDEFCR6 )

/// define the structure for the clock gating control register 7
typedef union _SIMDEFCR7
{
  struct
  {
    U32   uReserved1    :  8;         ///< reserved 1
    U32   bDMAEnable    :  1;         ///< DMA clock enable
  } tFields;
  U32 uValue;
} SIMDEFCR7, *PSIMDEFCR7;
#define SIMDEFCR7_SIZE                sizeof( SIMDEFCR7 )

/**@} EOF Sim_def.h */

#endif  // _SIM_DEF_H