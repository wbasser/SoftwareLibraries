/******************************************************************************
 * @file  Clock_def.h
 *
 * @brief 
 *
 * This file 
 *
 * @copyright Copyright (c) 2012 Cyber Integration
 * This document contains proprietary data and information of Cyber Integration 
 * LLC. It is the exclusive property of Cyber Integration, LLC and will not be 
 * disclosed in any form to any party without prior written permission of 
 * Cyber Integration, LLC. This document may not be reproduced or further used 
 * without the prior written permission of Cyber Integration, LLC.
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

// library includes -----------------------------------------------------------
#include "Types/Types.h"

// local includes -------------------------------------------------------------

// Macros and Defines ---------------------------------------------------------
/// define the helper macro to define the system clock using the MSI
#define CLKSYSMSIDEFM( msiclkrange, ahbpre, apb1pre, apb2pre, mcosrc, mcopre ) \
    .eSysClkSrc     = SYSLCK_SRC_MSI, \
    .eMsiClkRange   = msiclkrange, \
    .eMcoClkSrc     = mcosrc, \
    .eMcoPrescale   = mcopre, \
    .eAhbPrescale   = ahbpre, \
    .eApb1Prescale  = apb1pre, \
    .eApb2Prescale  = apb2pre \

/// define the helper macro to define the system clock using the PLL
#define CLKSYSMSIPLLDEFM( msiclkrange, ahbpre, apb1pre, apb2pre, mcosrc, mcopre, pllrpre, plldiv, pllmult ) \
    .eSysClkSrc     = SYSCLK_SRC_PLL, \
    .ePllClkSrc     = PLLCLK_SRC_MSI, \
    .eMsiClkRange   = msiclkrange, \
    .eMcoClkSrc     = mcosrc, \
    .eMcoPrescale   = mcopre, \
    .eAhbPrescale   = ahbpre, \
    .eApb1Prescale  = apb1pre, \
    .eApb2Prescale  = apb2pre, \
    .ePllPrescale   = pllrpre, \
    .nPllDiv        = plldiv, \
    .nPllMult       = pllmult \

/// define the helper macro to define the AHB clock enable
#define CLKAHBENBDEFM( dma1, dma2, flash, crc, touch, gpioa, gpiob, gpioc, gpiod, gpioe, gpiof, gpiog, gpioh, adc, rng, fmc, qpsi ) \
    .bDMA1          = dma1, \
    .bDMA2          = dma2, \
    .bFlash         = flash, \
    .bCrc           = crc, \
    .bTouch         = touch, \
    .bGPIOA         = gpioa, \
    .bGPIOB         = gpiob, \
    .bGPIOC         = gpioc, \
    .bGPIOD         = gpiod, \
    .bGPIOE         = gpioe, \
    .bGPIOF         = gpiof, \
    .bGPIOG         = gpiog, \
    .bGPIOH         = gpioh, \
    .bADC           = adc, \
    .bRNG           = rng, \
    .bFMC           = fmc, \
    .bQSPI          = qpsi \

/// define the helper macro to define the APB1 clock enables
#define CLKAPB1ENBDEFM( tim2, tim3, tim4, tim5, tim6, tim7, wwdgen, spi2, spi3, usart2, usart3, uart4, uart5, i2c1, i2c2, i2c3, can, pwr, dac, opamp, lptim1, lpuart, swp, lptim2 ) \
    .bTim2          = tim2, \
    .bTim3          = tim3, \
    .bTim4          = tim4, \
    .bTim5          = tim5, \
    .bTim6          = tim6, \
    .bTim7          = tim7, \
    .bWwdGen        = wwdgen, \
    .bSpi2          = spi2, \
    .bSpi3          = spi3, \
    .bUsart2        = usart2, \
    .bUsart3        = usart3, \
    .bUart4         = uart4, \
    .bUart5         = uart5, \
    .bI2c1          = i2c1, \
    .bI2c2          = i2c2, \
    .bI2c3          = i2c3, \
    .bCan           = can, \
    .bPwr           = pwr, \
    .bDAC1          = dac, \
    .bOpAmp         = opamp, \
    .bLpTim1        = lptim1, \
    .bLpUart1       = lpuart, \
    .bSwp1          = swp, \
    .bLpTim2        = lptim2 \

/// dfine the helper macro to define the APB2 clock enables
#define CLKAPB2ENBDEFM( syscfg, fw, sdmmc, tim1, spi1, tim8, usart1, tim15, tim16, tim17, sai1, sai2, dfsdm ) \
    .bSysCfg        = syscfg, \
    .bFw            = fw, \
    .bSdmMc         = sdmmc, \
    .bTim1          = tim1, \
    .bSpi1          = spi1, \
    .bTim8          = tim8, \
    .bUsart1        = usart1, \
    .bTim15         = tim15, \
    .bTim16         = tim16, \
    .bTim17         = tim17, \
    .bSai1          = sai1, \
    .bSai2          = sai2, \
    .bDfSdm         = dfsdm \
  
// enumerations ---------------------------------------------------------------
/// enumerate the sources for the system clock
typedef enum _SYSCLKSRC
{
  SYSCLK_SRC_MSI = 0,             ///< MSI 
  SYSCLK_SRC_HSI16,               ///< High Speed Clock Internal
  SYSCLK_SRC_HSE,                 ///< High Speed Clock External
  SYSCLK_SRC_PLL,                  ///< PLL clock
  SYSCLK_SRC_MAX
} SYSCLKSRC;

/// enumerate the MSI clock range
typedef enum _MSICLKRANGE
{
  MSICLK_RANGE_100KHZ = 0,        ///< 100KHZ
  MSICLK_RANGE_200KHZ,            ///< 200KHZ
  MSICLK_RANGE_400KHZ,            ///< 400KHZ
  MSICLK_RANGE_800KHZ,            ///< 800KHZ
  MSICLK_RANGE_1MHZ,              ///< 1MHZ
  MSICLK_RANGE_2MHZ,              ///< 2MHZ
  MSICLK_RANGE_4MHZ,              ///< 4MHZ
  MSICLK_RANGE_8MHZ,              ///< 8MHZ
  MSICLK_RANGE_16MHZ,             ///< 16MHZ
  MSICLK_RANGE_24MHZ,             ///< 24MHZ
  MSICLK_RANGE_32MHZ,             ///< 32MHZ
  MSICLK_RANGE_48MHZ,             ///< 48MHZ
  MSICLK_RANGE_MAX
} MSICLKRANGE;

/// enumarate the PLL clock source
typedef enum _PLLCLKSRC
{
  PLLCLK_SRC_NONE = 0,            ///< no 
  PLLCLK_SRC_MSI,                 ///< MSI 
  PLLCLK_SRC_HSI16,               ///< High Speed Clock Internal
  PLLCLK_SRC_HSE,                 ///< High Speed Clock External
  PLLCLK_SRC_MAX,
} PLLCLKSRC;

/// enumerate the APB1/2 prescale
typedef enum _ABPXPRESCALE
{
  APBX_PRESCALE_1 = 0,            ///< prescale of 1
  APBX_PRESCALE_2 = 4,            ///< prescale of 2
  APBX_PRESCALE_4,                ///< prescale of 4
  APBX_PRESCALE_8,                ///< prescale of 8
  APBX_PRESCALE_16,               ///< prescale of 16
  APBX_PRESCALE_MAX
} ABPXPRESCALE;

/// enumerate the AHB prescale
typedef enum _AHBPRESCALE
{
  AHB_PRESCALE_1 = 0,             ///< prescale of 1
  AHB_PRESCALE_2 = 8,             ///< prescale of 2
  AHB_PRESCALE_4,                 ///< prescale of 4
  AHB_PRESCALE_8,                 ///< prescale of 8
  AHB_PRESCALE_16,                ///< prescale of 16
  AHB_PRESCALE_64,                ///< prescale of 64
  AHB_PRESCALE_128,               ///< prescale of 128
  AHB_PRESCALE_256,               ///< prescale of 256
  AHB_PRESCALE_512,               ///< prescale of 512
  AHB_PRESCALE_MAX
} AHBPRESCALE;

/// enumerate the MCO clock source
typedef enum _MCOCLKSRC
{
  MCO_CLKSRC_NONE = 0,            ///< no MCO clock source
  MCO_CLKSRC_SYSCLK,              ///< system clock
  MCO_CLKSRC_MSI,                 ///< MSI clock
  MCO_CLKSRC_HSI16,               ///< HSI*16 clock
  MCO_CLKSRC_HSE,                 ///< HSE clock
  MCO_CLKSRC_PLL,                 ///< PLL
  MCO_CLKSRC_LSI,                 ///< LSI
  MCO_CLKSRC_LSE,                 ///< LSE
} MCOCLKSRC;

/// enumerate the MCO clock prescale
typedef enum _MCOPRESCALE
{
  MCO_PRESCALE_1 = 0,             ///< prescale of 1
  MCO_PRESCALE_2,                 ///< prescale of 2
  MCO_PRESCALE_4,                 ///< prescale of 4
  MCO_PRESCALE_8,                 ///< prescale of 8
  MCO_PRESCALE_16,                ///< prescale fo 16
  MCO_PRESCALE_MAX
} MCOPRESCALE;

/// enumerate the PLL main clock prescale
typedef enum _PLLRPRESCALE
{
  PLLR_PRESCALE_2 = 0,            ///< prescale of 2
  PLLR_PRESCALE_4,                ///< prescale of 4
  PLLR_PRESCALE_6,                ///< prescale of 6
  PLLR_PRESCALE_8,                ///< prescale of 8
  PLLR_PRESCALE_MAX
} PLLRPRESCALE;

// structures -----------------------------------------------------------------
/// define the system clock definition structure
typedef struct _CLKSYSDEF
{
  SYSCLKSRC     eSysClkSrc;       ///< clock source
  MSICLKRANGE   eMsiClkRange;     ///< MSI clock range
  PLLCLKSRC     ePllClkSrc;       ///< PLL clock source
  PLLRPRESCALE  ePllPrescale;     ///< PLL R output prescale
  MCOCLKSRC     eMcoClkSrc;       ///< MCO clock source
  MCOPRESCALE   eMcoPrescale;     ///< MCO clock prescale
  U8            nPllDiv;          ///< PLL divider
  U8            nPllMult;         ///< PLL multiplier
  U8            nMCODiv;          ///< MCO divider
  AHBPRESCALE   eAhbPrescale;     ///< AHB prescale
  ABPXPRESCALE  eApb1Prescale;    ///< APB1 prescale
  ABPXPRESCALE  eApb2Prescale;    ///< APB2 prescale
} CLKSYSDEF, *PCLKSYSDEF;
#define CLKSYSDEF_SIZE            sizeof( CLKSYSDEF )

/// define the AHB clock enable structure
typedef struct _CLKAHBDEF
{
	BOOL          bDMA1   : 1;      ///< DMA1
  BOOL          bDMA2   : 1;      ///< DMA2
  BOOL          bFlash  : 1;      ///< flash
  BOOL          bCrc    : 1;      ///< CRC
  BOOL          bTouch  : 1;      ///< touch
  BOOL          bGPIOA  : 1;      ///< GPIOA
  BOOL          bGPIOB  : 1;      ///< GPIOB
  BOOL          bGPIOC  : 1;      ///< GPIOC
  BOOL          bGPIOD  : 1;      ///< GPIOD
  BOOL          bGPIOE  : 1;      ///< GPIOE
  BOOL          bGPIOF  : 1;      ///< GPIOF
  BOOL          bGPIOG  : 1;      ///< GPIOG
  BOOL          bGPIOH  : 1;      ///< GPIOH
  BOOL          bADC    : 1;      ///< ADC
  BOOL          bRNG    : 1;      ///< RNG
  BOOL          bFMC    : 1;      ///< FMC
  BOOL          bQSPI   : 1;      ///< QSPI
} CLKAHBDEF, *PCLKAHBDEF;
#define CLKAHBENB_SIZE            sizeof( CLKAHBENB )

/// define the APB1 clock enable structure
typedef struct _CLKAPB1DEF
{
  BOOL          bTim2   : 1;      ///< timer #2
  BOOL          bTim3   : 1;      ///< timer #3
  BOOL          bTim4   : 1;      ///< timer #4
  BOOL          bTim5   : 1;      ///< timer #5
  BOOL          bTim6   : 1;      ///< timer #6
  BOOL          bTim7   : 1;      ///< timer #7
  BOOL          bWwdGen : 1;      ///< WWDGen
  BOOL          bSpi2   : 1;      ///< SPI2
  BOOL          bSpi3   : 1;      ///< SPI3
  BOOL          bUsart2 : 1;      ///< USART2
  BOOL          bUsart3 : 1;      ///< USART3
  BOOL          bUart4  : 1;      ///< UART4
  BOOL          bUart5  : 1;      ///< UART5
  BOOL          bI2c1   : 1;      ///< IC2C1
  BOOL          bI2c2   : 1;      ///< IC2C2
  BOOL          bI2c3   : 1;      ///< IC2C3
  BOOL          bCan    : 1;      ///< CAN
  BOOL          bPwr    : 1;      ///< Pwr
  BOOL          bDAC1   : 1;      ///< DAC 1
  BOOL          bOpAmp  : 1;      ///< opamp
  BOOL          bLpTim1 : 1;      ///< low power timer #1
  BOOL          bLpUart1: 1;      ///< low power UART #1
  BOOL          bSwp1   : 1;      ///< single wire protocol
  BOOL          bLpTim2 : 1;      ///< low power timer #2
} CLKAPB1DEF, *PCLKAPB1DEF;
#define CLKAPB1ENB_SIZE           sizeof( CLKAPB1ENB )

/// define the APB2 clock enable structure
typedef struct _CLKAPB2DEF
{
  BOOL          bSysCfg : 1;      ///< system config
  BOOL          bFw     : 1;      ///< firewall
  BOOL          bSdmMc  : 1;      ///< SCM/MC
  BOOL          bTim1   : 1;      ///< timer #1
  BOOL          bSpi1   : 1;      ///< SPI1
  BOOL          bTim8   : 1;      ///< timer #8
  BOOL          bUsart1 : 1;      ///< USART #1
  BOOL          bTim15  : 1;      ///< timer #15
  BOOL          bTim16  : 1;      ///< timer #16
  BOOL          bTim17  : 1;      ///< timer #17
  BOOL          bSai1   : 1;      ///< SAI1
  BOOL          bSai2   : 1;      ///< SAI2
  BOOL          bDfSdm  : 1;      ///< DFSDM
} CLKAPB2DEF, *PCLKAPB2DEF;
#define CLKAPB2DEF_SIZE           sizeof( CLKAPB2DEF )

/**@} EOF Clock_def.h */

#endif  // _CLOCK_DEF_H

