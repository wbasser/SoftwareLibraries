/******************************************************************************
 * @file Clock.c
 *
 * @brief clock module implementation
 *
 * This file clock module initialization and up/down scaling
 *
 * @copyright Copyright (c) 2012 Cyber Integration
 * This document contains proprietary data and information of Cyber Integration 
 * LLC. It is the exclusive property of Cyber Integration, LLC and will not be 
 * disclosed in any form to any party without prior written permission of 
 * Cyber Integration, LLC. This document may not be reproduced or further used 
 * without the prior written permission of Cyber Integration, LLC.
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

// library includes -----------------------------------------------------------
#include "Interrupt/Interrupt.h"

// local includes -------------------------------------------------------------
#include "Clock/Clock.h"

// Macros and Defines ---------------------------------------------------------
/// define the internal oscillator frequencies
#define HSI_FREQ                            ( 16000000 )

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations ----------------------------------------------

// local parameter declarations -----------------------------------------------
static  U32   uSystemClockFreq;

// local function prototypes --------------------------------------------------
static  void  ConfigureMsi( PCLKSYSDEF ptClkDef );
static  void  ConfigurePll( PCLKSYSDEF ptClkDef );
static  void  ConfigureAhb( void );
static  void  ConfigureApb1( void );
static  void  ConfigureApb2( void );
static  void  ComputeSystemClockFreq( void );

// constant parameter initializations -----------------------------------------
// define the MSI clock rqnge
static  const U32 auMsiClockRange[ MSICLK_RANGE_MAX ] =
{
    100000,
    200000,
    400000,
    800000,
   1000000,
   2000000,
   4000000,
   8000000,
  16000000,
  24000000,
  32000000,
  48000000,
};

// define the AHB prescale table
static const U32 auAhbPrescale[ ] =
{
  0, 0, 0, 0, 0, 0, 0, 0, 
  1, 2, 3, 4, 6, 7, 8, 9  
};

/// define the APBX prescale table
static const U32 auApbxPrescale[ ] =
{
  0, 0, 0, 0, 1, 2, 3, 4
};

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
  PCLKSYSDEF  ptClkDef;
  U32         uTemp;
  
  // set the pointer
  ptClkDef = ( PCLKSYSDEF )&tClkSysDef;
  
  // determine the clock source
  switch( ptClkDef->eSysClkSrc )
  {
    case SYSCLK_SRC_MSI :
      // configure the MSI clock
      ConfigureMsi( ptClkDef );
      break;
      
    case SYSCLK_SRC_HSI16 :
      break;
      
    case SYSCLK_SRC_HSE :
      break;
      
    case SYSCLK_SRC_PLL :
      // determine the source of the 
      switch( ptClkDef->ePllClkSrc )
      {
        case PLLCLK_SRC_MSI :
          // configure the MSI clock
          ConfigureMsi( ptClkDef );
          break;
          
        case PLLCLK_SRC_HSI16 :
          break;
          
        case PLLCLK_SRC_HSE :
          break;
          
        default :
          //#error "Illegal PLL clock source!"
          break;
      }
      
      // now set upt the PLL
      ConfigurePll( ptClkDef );
      break;
      
    default :
      // this is an error - report it
      //#error "Illegal System Clock Source!"
      break;
  }
  
  // setup the prescales/MCO clock source/
  uTemp = BITS( ptClkDef->eAhbPrescale, RCC_CFGR_HPRE_Pos );
  uTemp |= BITS( ptClkDef->eApb1Prescale, RCC_CFGR_PPRE1_Pos );
  uTemp |= BITS( ptClkDef->eApb2Prescale, RCC_CFGR_PPRE2_Pos );
  uTemp |= BITS( ptClkDef->eMcoPrescale, RCC_CFGR_MCOPRE_Pos );
  uTemp |= BITS( ptClkDef->eMcoClkSrc, RCC_CFGR_MCOSEL_Pos );
  uTemp |= BITS( ptClkDef->eSysClkSrc, RCC_CFGR_SW_Pos );
  RCC->CFGR = uTemp;
  
  // now wait for clock source to be active
  while(( RCC->CFGR & RCC_CFGR_SWS ) != BITS( ptClkDef->eSysClkSrc, RCC_CFGR_SWS_Pos ));
  
  // set up the AHB/APB1/APB2 clock enables
  ConfigureAhb( );
  ConfigureApb1( );
  ConfigureApb2( );

  // compute clock frequency
  ComputeSystemClockFreq( );
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
 * @function Clock_GetFreq
 *
 * @brief get the current clock frequency
 *
 * This function will return the current clock frequency
 *
 *  @return     the frequency of the main clock
 *
 *****************************************************************************/
U32 Clock_GetFreq( void )
{
  // GetSystemClockSource
  return( uSystemClockFreq );
}

/******************************************************************************
 * @function Clock_GetAPB1Frequency
 *
 * @brief get the current APB1 clock frequency
 *
 * This function will return the current APB1 clock frequency
 *
 *  @return     the frequency of the main clock
 *
 *****************************************************************************/
U32 Clock_GetAPB1Frequency( void )
{
  U32 uApbFreq;

  // get the base frequency
  uApbFreq = Clock_GetFreq( );

  // compute the divisor
  uApbFreq >>= auApbxPrescale[ ( RCC->CFGR & RCC_CFGR_PPRE1 ) >> RCC_CFGR_PPRE1_Pos ];

  // return the freq
  return( uApbFreq );
}

/******************************************************************************
 * @function Clock_GetAPB2Frequency
 *
 * @brief get the current APB2 clock frequency
 *
 * This function will return the current APB2 clock frequency
 *
 *  @return     the frequency of the main clock
 *
 *****************************************************************************/
U32 Clock_GetAPB2Frequency( void )
{
  U32 uApbFreq;

  // get the base frequency
  uApbFreq = Clock_GetFreq( );

  // compute the divisor
  uApbFreq >>= auApbxPrescale[ ( RCC->CFGR & RCC_CFGR_PPRE2 ) >> RCC_CFGR_PPRE2_Pos ];

  // return the freq
  return( uApbFreq );
}


/******************************************************************************
 * @function ConfigureMsi
 *
 * @brief MSI configuration
 *
 * This function will configure the MSI clock
 *
 *  @param[in]      ptClkDef    pointer to the clock defitinion
 *
 *****************************************************************************/
static void ConfigureMsi( PCLKSYSDEF ptClkDef )
{
  // turn on MSI/set the range
  RCC->CR = RCC_CR_MSION | BITS( ptClkDef->eMsiClkRange, RCC_CR_MSIRANGE_Pos ) | RCC_CR_MSIRGSEL;
}

/******************************************************************************
 * @function ConfigurePll
 *
 * @brief PLL configuration
 *
 * This function will configure the PLL
 *
 *  @param[in]      ptClkDef    pointer to the clock defitinion
 *
 *****************************************************************************/
static void ConfigurePll( PCLKSYSDEF ptClkDef )
{
  U32 uTemp;
  
  // set up the PLL
  uTemp = RCC_PLLCFGR_PLLREN;
  uTemp |= BITS( ptClkDef->ePllClkSrc, RCC_PLLCFGR_PLLSRC_Pos );
  uTemp |= BITS( ptClkDef->nPllDiv, RCC_PLLCFGR_PLLM_Pos );
  uTemp |= BITS( ptClkDef->nPllMult, RCC_PLLCFGR_PLLN_Pos );
  uTemp |= BITS( ptClkDef->ePllPrescale, RCC_PLLCFGR_PLLR_Pos );
  uTemp |= RCC_PLLCFGR_PLLREN;
  RCC->PLLCFGR = uTemp;
  
  // turn it on/wait till it is ready
  RCC->CR |= RCC_CR_PLLON;
  while( !( RCC->CR & RCC_CR_PLLRDY ));
}

/******************************************************************************
 * @function ConfigureAhb
 *
 * @brief AHB configuration
 *
 * This function will configure the AHB
 *
 *****************************************************************************/
static void ConfigureAhb( void )
{
  PCLKAHBDEF  ptClkAhbDef;
  U32         uTemp;
  
  // set the pointer
  ptClkAhbDef = ( PCLKAHBDEF )&tClkAhbDef;
  
  // build the AHB1 value/write it
  uTemp =  ( ptClkAhbDef->bDMA1 )  ? RCC_AHB1ENR_DMA1EN  : 0;
  uTemp |= ( ptClkAhbDef->bDMA2 )  ? RCC_AHB1ENR_DMA2EN  : 0;
  uTemp |= ( ptClkAhbDef->bFlash ) ? RCC_AHB1ENR_FLASHEN : 0;
  uTemp |= ( ptClkAhbDef->bCrc )   ? RCC_AHB1ENR_CRCEN   : 0;
  uTemp |= ( ptClkAhbDef->bTouch ) ? RCC_AHB1ENR_TSCEN   : 0;
  RCC->AHB1ENR = uTemp;

  // build the AHB2 value/write it
  uTemp =  ( ptClkAhbDef->bGPIOA ) ? RCC_AHB2ENR_GPIOAEN : 0;
  uTemp |= ( ptClkAhbDef->bGPIOB ) ? RCC_AHB2ENR_GPIOBEN : 0;
  uTemp |= ( ptClkAhbDef->bGPIOC ) ? RCC_AHB2ENR_GPIOCEN : 0;
  uTemp |= ( ptClkAhbDef->bGPIOD ) ? RCC_AHB2ENR_GPIODEN : 0;
  uTemp |= ( ptClkAhbDef->bGPIOE ) ? RCC_AHB2ENR_GPIOEEN : 0;
  uTemp |= ( ptClkAhbDef->bGPIOF ) ? RCC_AHB2ENR_GPIOFEN : 0;
  uTemp |= ( ptClkAhbDef->bGPIOG ) ? RCC_AHB2ENR_GPIOGEN : 0;
  uTemp |= ( ptClkAhbDef->bGPIOH ) ? RCC_AHB2ENR_GPIOHEN : 0;
  uTemp |= ( ptClkAhbDef->bADC )   ? RCC_AHB2ENR_ADCEN   : 0;
  uTemp |= ( ptClkAhbDef->bRNG )   ? RCC_AHB2ENR_RNGEN   : 0;
  RCC->AHB2ENR = uTemp;

  // build the AHB2 value/write it
  uTemp =  ( ptClkAhbDef->bFMC )   ? RCC_AHB3ENR_FMCEN   : 0;
  uTemp |= ( ptClkAhbDef->bQSPI )  ? RCC_AHB3ENR_QSPIEN  : 0;
  RCC->AHB3ENR = uTemp;
}

/******************************************************************************
 * @function ConfigureApb1
 *
 * @brief Apb1 configuration
 *
 * This function will configure the APB1
 *
 *****************************************************************************/
static void ConfigureApb1( void )
{
  PCLKAPB1DEF  ptClkApb1Def;
  U32         uTemp;

  // set the pointer
  ptClkApb1Def = ( PCLKAPB1DEF )&tClkApb1Def;
  
  // build the APB1 reg 1 value/write it
  uTemp =  ( ptClkApb1Def->bTim2 )   ? RCC_APB1ENR1_TIM2EN   : 0;
  uTemp |= ( ptClkApb1Def->bTim3 )   ? RCC_APB1ENR1_TIM3EN   : 0;
  uTemp |= ( ptClkApb1Def->bTim4 )   ? RCC_APB1ENR1_TIM4EN   : 0;
  uTemp |= ( ptClkApb1Def->bTim5 )   ? RCC_APB1ENR1_TIM5EN   : 0;
  uTemp |= ( ptClkApb1Def->bTim6 )   ? RCC_APB1ENR1_TIM6EN   : 0;
  uTemp |= ( ptClkApb1Def->bTim7 )   ? RCC_APB1ENR1_TIM7EN   : 0;
  uTemp |= ( ptClkApb1Def->bWwdGen ) ? RCC_APB1ENR1_WWDGEN   : 0;
  uTemp |= ( ptClkApb1Def->bSpi2 )   ? RCC_APB1ENR1_SPI2EN   : 0;
  uTemp |= ( ptClkApb1Def->bSpi3 )   ? RCC_APB1ENR1_SPI3EN   : 0;
  uTemp |= ( ptClkApb1Def->bUsart2 ) ? RCC_APB1ENR1_USART2EN : 0;
  uTemp |= ( ptClkApb1Def->bUsart3 ) ? RCC_APB1ENR1_USART3EN : 0;
  uTemp |= ( ptClkApb1Def->bUart4 )  ? RCC_APB1ENR1_UART4EN  : 0;
  uTemp |= ( ptClkApb1Def->bUart5 )  ? RCC_APB1ENR1_UART5EN  : 0;
  uTemp |= ( ptClkApb1Def->bI2c1 )   ? RCC_APB1ENR1_I2C1EN   : 0;
  uTemp |= ( ptClkApb1Def->bI2c2 )   ? RCC_APB1ENR1_I2C2EN   : 0;
  uTemp |= ( ptClkApb1Def->bI2c3 )   ? RCC_APB1ENR1_I2C3EN   : 0;
  uTemp |= ( ptClkApb1Def->bCan )    ? RCC_APB1ENR1_CAN1EN   : 0;
  uTemp |= ( ptClkApb1Def->bPwr )    ? RCC_APB1ENR1_PWREN    : 0;
  uTemp |= ( ptClkApb1Def->bDAC1 )   ? RCC_APB1ENR1_DAC1EN   : 0;
  uTemp |= ( ptClkApb1Def->bOpAmp )  ? RCC_APB1ENR1_OPAMPEN  : 0;
  uTemp |= ( ptClkApb1Def->bLpTim1 ) ? RCC_APB1ENR1_LPTIM1EN : 0;
  RCC->APB1ENR1 = uTemp;
  
  // build the APB1 reg 2 value/write it
  uTemp =  ( ptClkApb1Def->bLpUart1 ) ? RCC_APB1ENR2_LPUART1EN : 0;
  uTemp |= ( ptClkApb1Def->bSwp1 )    ? RCC_APB1ENR2_SWPMI1EN  : 0;
  uTemp |= ( ptClkApb1Def->bLpTim2 )  ? RCC_APB1ENR2_LPTIM2EN  : 0;
  RCC->APB1ENR2 = uTemp;
}

/******************************************************************************
 * @function ConfigureApb2
 *
 * @brief Apb2 configuration
 *
 * This function will configure the APB2
 *
 *****************************************************************************/
static void ConfigureApb2( void )
{
  PCLKAPB2DEF  ptClkApb2Def;
  U32         uTemp;
  
  // set the pointer
  ptClkApb2Def = ( PCLKAPB2DEF )&tClkApb2Def;
  
  // build the APB1 reg 1 value/write it
  uTemp =  ( ptClkApb2Def->bSysCfg ) ? RCC_APB2ENR_SYSCFGEN : 0;
  uTemp |= ( ptClkApb2Def->bFw )     ? RCC_APB2ENR_FWEN     : 0;
  uTemp |= ( ptClkApb2Def->bSdmMc )  ? RCC_APB2ENR_SDMMC1EN : 0;
  uTemp |= ( ptClkApb2Def->bTim1 )   ? RCC_APB2ENR_TIM1EN   : 0;
  uTemp |= ( ptClkApb2Def->bSpi1 )   ? RCC_APB2ENR_SPI1EN   : 0;
  uTemp |= ( ptClkApb2Def->bTim8 )   ? RCC_APB2ENR_TIM8EN   : 0;
  uTemp |= ( ptClkApb2Def->bUsart1 ) ? RCC_APB2ENR_USART1EN : 0;
  uTemp |= ( ptClkApb2Def->bTim15 )  ? RCC_APB2ENR_TIM15EN  : 0;
  uTemp |= ( ptClkApb2Def->bTim16 )  ? RCC_APB2ENR_TIM16EN  : 0;
  uTemp |= ( ptClkApb2Def->bTim17 )  ? RCC_APB2ENR_TIM17EN  : 0;
  uTemp |= ( ptClkApb2Def->bSai1 )   ? RCC_APB2ENR_SAI1EN   : 0;
  uTemp |= ( ptClkApb2Def->bSai2 )   ? RCC_APB2ENR_SAI2EN   : 0;
  uTemp |= ( ptClkApb2Def->bDfSdm )  ? RCC_APB2ENR_DFSDM1EN : 0;
  RCC->APB2ENR = uTemp;
}

/******************************************************************************
 * @function ComputeSystemClockFreq
 *
 * @brief compute the system clock frequency
 *
 * This function will compute the system clock frequency
 *
 *****************************************************************************/
static void ComputeSystemClockFreq( void )
{
  U32 uMsiClockFreq, uMsiClockIndex, uPllVcoFreq, uPllDivider;

  // determine the MSI clock freq
  if ( !( RCC->CR & RCC_CR_MSIRGSEL ))
  {
    // get the index from CSR
    uMsiClockIndex = ( RCC->CSR & RCC_CSR_MSISRANGE ) >> RCC_CSR_MSISRANGE_Pos;
  }
  else
  {
    // get the index from CR
    uMsiClockIndex = ( RCC->CR & RCC_CR_MSIRANGE ) >> RCC_CR_MSIRANGE_Pos;
  }

  // get the MSI clock freq
  uMsiClockFreq = auMsiClockRange[ uMsiClockIndex ];

  // now determine the clock source
  switch( RCC->CFGR & RCC_CFGR_SWS )
  {
    case RCC_CFGR_SWS_HSI :
      // set the freq to the HSI value
      uSystemClockFreq = HSI_FREQ;
      break;

    case RCC_CFGR_SWS_HSE :
      // set the freq to the HSE value
      uSystemClockFreq = CLK_HSE_FREQ;
      break;

    case RCC_CFGR_SWS_PLL :
      // get the divider
      uPllDivider = (( RCC->PLLCFGR & RCC_PLLCFGR_PLLM ) >> RCC_PLLCFGR_PLLM_Pos ) + 1;

      // determine the source
      switch( RCC->PLLCFGR & RCC_PLLCFGR_PLLSRC )
      {
        case RCC_PLLCFGR_PLLSRC_HSI :
          uPllVcoFreq = HSI_FREQ / uPllDivider;
          break;

        case RCC_PLLCFGR_PLLSRC_HSE :
          uPllVcoFreq = CLK_HSE_FREQ / uPllDivider;
          break;

        case RCC_PLLCFGR_PLLSRC_MSI :
        default :
          uPllVcoFreq = uMsiClockFreq / uPllDivider;
          break;
      }

      // now apply the divider
      uPllVcoFreq *= (( RCC->PLLCFGR & RCC_PLLCFGR_PLLN ) >> RCC_PLLCFGR_PLLN_Pos );
      uSystemClockFreq = uPllVcoFreq / (((( RCC->PLLCFGR & RCC_PLLCFGR_PLLR ) >> RCC_PLLCFGR_PLLR_Pos ) + 1 ) * 2 );
      break;

    case RCC_CFGR_SWS_MSI :
    default :
      // set to MSI
      uSystemClockFreq = uMsiClockFreq;
      break;
  }

  // apply the prescaler
  uSystemClockFreq >>= auAhbPrescale[ ( RCC->CFGR & RCC_CFGR_HPRE ) >> RCC_CFGR_HPRE_Pos ];
}

/**@} EOF Clock.c */
