/******************************************************************************
 * @file Timers.c
 *
 * @brief timers implementation
 *
 * This file provides the implementation for the timers module
 *
 * @copyright Copyright (c) 2012 Nan Technologies
 * This document contains proprietary data and information of Nan Technologies
 * LLC. It is the exclusive property of Nan Technologies, LLC and will not be
 * disclosed in any form to any party without prior written permission of
 * Nan Technologies, LLC. This document may not be reproduced or further used
 * without the prior written permission of Nan Technologies, LLC.
 *
 * Version History
 * ======
 * $Rev: $
 * 
 *
 * \addtogroup Timers
 * @{
 *****************************************************************************/

// local includes -------------------------------------------------------------
#include "Timers/Timers.h"

// Macros and Defines ---------------------------------------------------------
/// define the delay count for clock gating
#define CLK_DELAY_COUNT     ( 0x3FFU )

/// define the macros to allow mapping the interrupts
#define CCU40_Irq_Handler  IRQ_Hdlr_21
#define CCU41_Irq_Handler  IRQ_Hdlr_22
#define CCU42_Irq_Handler  IRQ_Hdlr_23
#define CCU43_Irq_Handler  IRQ_Hdlr_24

/// define the interrupts
#define CCU40_IRQ_ENUM     CCU40_0_IRQn
#define CCU41_IRQ_ENUM     CCU40_1_IRQn
#define CCU42_IRQ_ENUM     CCU40_2_IRQn
#define CCU43_IRQ_ENUM     CCU40_3_IRQn

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------
typedef struct _LCLCTL
{
  CCU4_CC4_TypeDef* ptCcuRegs;
  U8                nIrqLevel;
} LCLCTL, *PLCLCTL;
#define LCLCTL_SIZE   sizeof( LCLCTL )

/// define the physical to control map
typedef struct _PHYMAP
{
  TIMERSENUM eTimer;             ///< enumerated device
} PHYMAP, *PPHYMAP;
#define PHYMAP_SIZE   sizseof( PHYMAP )

// global interrupt handlers
void  CCU40_Irq_Handler( void );
void  CCU41_Irq_Handler( void );
void  CCU42_Irq_Handler( void );
void  CCU43_Irq_Handler( void );

// global parameter declarations ----------------------------------------------

// local parameter declarations -----------------------------------------------
static  PHYMAP  atPhyMaps[ TIMER_CHAN_MAX ];

// local function prototypes --------------------------------------------------
static  void  GetLocalControl( TIMERCHAN eChan, PLCLCTL ptCtl );
static  void  ConfigureCompare( TIMERSENUM eTimer, PTIMERDEF ptDef, PLCLCTL ptCtl );
static  void  ConfigureCapture( TIMERSENUM eTimer, PTIMERDEF ptDef, PLCLCTL ptCtl );
static  void  CommonInterruptHandler( TIMERSENUM eTimer );

// constant parameter initializations -----------------------------------------

/******************************************************************************
 * @function Timers_Initialize
 *
 * @brief timer initialization
 *
 * This function initialization all configured timers
 *
 *****************************************************************************/
void Timers_Initialize( void )
{
  TIMERSENUM    eTimer;
  PTIMERDEF     ptDef;
  LCLCTL        tLclCtl;

  // enable the clock
  SCU_GENERAL->PASSWD = 0x000000C0UL;
  WR_REG( SCU_CLK->CLKCR, SCU_CLK_CLKCR_CNTADJ_Msk, SCU_CLK_CLKCR_CNTADJ_Pos, CLK_DELAY_COUNT );
  SET_BIT( SCU_CLK->CGATCLR0, SCU_CLK_CGATCLR0_CCU40_Pos );
  while ( SCU_CLK->CLKCR & SCU_CLK_CLKCR_VDDC2LOW_Msk );
  SCU_GENERAL->PASSWD = 0x000000C3UL;

  // enable the global prescaler value
  CCU40->GIDLC = BIT( CCU4_GIDLC_SPRB_Pos );

  // for each timer in the config table
  for ( eTimer = 0; eTimer < TIMERS_ENUM_MAX; eTimer++ )
  {
    // get the definition/register pointer
    ptDef = ( PTIMERDEF )&atTimerDefs[ eTimer ];
    GetLocalControl( ptDef->eChan, &tLclCtl );

    // set the physical to level translation
    atPhyMaps[ ptDef->eChan ].eTimer = eTimer;

    // clear the counters
    tLclCtl.ptCcuRegs->TCCLR = BIT( CCU4_CC4_TCCLR_TRBC_Pos ) | BIT( CCU4_CC4_TCCLR_TCC_Pos ) | BIT( CCU4_CC4_TCCLR_DITC_Pos );

    // determine the mode
    switch( ptDef->eMode )
    {
      case TIMER_MODE_COUNTER :
        break;

      case TIMER_MODE_COMPARE :
        ConfigureCompare( eTimer, ptDef, &tLclCtl );
        break;

      case TIMER_MODE_CAPTURE :
        ConfigureCapture( eTimer, ptDef, &tLclCtl );
        break;

      default :
          break;
    }
  }
}

/******************************************************************************
 * @function Timers_StartStop
 *
 * @brief start/stop the timer
 *
 * This function will start/stop the timer
 *
 * @param[in]   eTimerSel   timer selection
 * @param[in]   bState      state of the timer
 *
 * @return      appropriate error
 *
 *****************************************************************************/
TIMERSERR Timers_StartStop( TIMERSENUM eTimerSel, BOOL bState )
{
  TIMERSERR  eError = TIMERS_ERR_NONE;
  PTIMERDEF  ptDef;
  LCLCTL     tLclCtl;
  U32        uTemp;

  // valid timer
  if ( eTimerSel < TIMERS_ENUM_MAX )
  {
    // get the def file/get the registers
    ptDef = ( PTIMERDEF )&atTimerDefs[ eTimerSel ];
    GetLocalControl( ptDef->eChan, &tLclCtl );

    // determine if this is a start or stop
    if ( bState )
    {
      // clear all interrupts
      uTemp = BIT( CCU4_CC4_SWR_RPM_Pos ) | BIT( CCU4_CC4_SWR_ROM_Pos ) | BIT( CCU4_CC4_SWR_RCMU_Pos ) | BIT( CCU4_CC4_SWR_RCMD_Pos );
      uTemp |= BIT( CCU4_CC4_SWR_RE0A_Pos ) | BIT( CCU4_CC4_SWR_RE1A_Pos ) | BIT( CCU4_CC4_SWR_RE2A_Pos ) | BIT( CCU4_CC4_SWR_RTRPF_Pos );
      tLclCtl.ptCcuRegs->SWR |= uTemp;

      // clear the global idle clear
      CCU40->GIDLC = BIT( ptDef->eChan );

      // turn on the timer
      tLclCtl.ptCcuRegs->TCSET |= 1;
    }
    else
    {
      // clear the counters/clear the global enables
      tLclCtl.ptCcuRegs->TCCLR = BIT( CCU4_CC4_TCCLR_TRBC_Pos ) | BIT( CCU4_CC4_TCCLR_TCC_Pos ) | BIT( CCU4_CC4_TCCLR_DITC_Pos );
      uTemp = BIT( ptDef->eChan ) | BIT( CCU4_GIDLS_PSIC_Pos );
      CCU40->GIDLS |= uTemp;
    }
  }
  else
  {
    // set the illegal timer error
    eError = TIMERS_ERR_ILLTIMER;
  }

  return( eError );
}

///******************************************************************************
// * @function Timers_GetCounter
// *
// * @brief gets the current value of the counter
// *
// * This function will return the current value of the counter
// *
// * @param[in]   eTimerSel   timer selection
// * @param[io]   pwCounter   storage for the counter value
// *
// * @return      appropriate error
// *
// *****************************************************************************/
//TIMERSERR Timers_GetCounter( TIMERSENUM eTimerSel, PU16 pwCounter )
//{
//  TIMERSERR eError = TIMERS_ERR_NONE;
//
//  // valid timer
//  if ( eTimerSel < TIMERS_ENUM_MAX )
//  {
//  }
//  else
//  {
//    // set the illegal timer error
//    eError = TIMERS_ERR_ILLTIMER;
//  }
//
//  return( eError );
//}
//
///******************************************************************************
// * @function Timers_SetCounter
// *
// * @brief set the counter value
// *
// * This function will set a counter value into the count register
// *
// * @param[in]   eTimerSel   timer selection
// * @param[in]   wCounter    new counter value
// *
// * @return
// *
// *****************************************************************************/
//TIMERSERR Timers_SetCounter( TIMERSENUM eTimerSel, U16 wCounter )
//{
//  TIMERSERR eError = TIMERS_ERR_NONE;
//
//  // valid timer
//  if ( eTimerSel < TIMERS_ENUM_MAX )
//  {
//  }
//  else
//  {
//    // set the illegal timer error
//    eError = TIMERS_ERR_ILLTIMER;
//  }
//
//  return( eError );
//}
//
/******************************************************************************
 * @function Timers_GetPeriod
 *
 * @brief gets the current value of the period
 *
 * This function will return the current value of the counter
 *
 * @param[in]   eTimerSel   timer selection
 * @param[io]   pwPeriod    storage for the counter value
 *
 * @return      appropriate error
 *
 *****************************************************************************/
TIMERSERR Timers_GetPeriod( TIMERSENUM eTimerSel, PU16 pwPeriod )
{
  TIMERSERR  eError = TIMERS_ERR_NONE;
  PTIMERDEF  ptDef;
  LCLCTL     tLclCtl;

  // valid timer
  if ( eTimerSel < TIMERS_ENUM_MAX )
  {
    // get the def file/get the registers
    ptDef = ( PTIMERDEF )&atTimerDefs[ eTimerSel ];
    GetLocalControl( ptDef->eChan, &tLclCtl );

    // read the period
    *pwPeriod = tLclCtl.ptCcuRegs->PR;
  }
  else
  {
    // set the illegal timer error
    eError = TIMERS_ERR_ILLTIMER;
  }

  return( eError );
}

/******************************************************************************
 * @function Timers_SetPeriod
 *
 * @brief set the PWM period
 *
 * This function will set the desired period into the period register
 *
 * @param[in]   eTimerSel   timer selection
 * @param[in]   wPeriod     new period value
 *
 * @return
 *
 *****************************************************************************/
TIMERSERR Timers_SetPeriod( TIMERSENUM eTimerSel, U16 wPeriod )
{
  TIMERSERR  eError = TIMERS_ERR_NONE;
  PTIMERDEF  ptDef;
  LCLCTL     tLclCtl;

  // valid timer
  if ( eTimerSel < TIMERS_ENUM_MAX )
  {
    // get the def file/get the registers
    ptDef = ( PTIMERDEF )&atTimerDefs[ eTimerSel ];
    GetLocalControl( ptDef->eChan, &tLclCtl );

    // read the period
    tLclCtl.ptCcuRegs->PRS = wPeriod;

    // set up the shadow registers
    CCU40->GCSS = BITS( CCU4_GCSS_S0SE_Msk , ( ptDef->eChan * 4 ));
  }
  else
  {
    // set the illegal timer error
    eError = TIMERS_ERR_ILLTIMER;
  }

  return( eError );
}

/******************************************************************************
 * @function Timers_GetCompareVal
 *
 * @brief gets the current value of a timers compare channel
 *
 * This function will return the current value of a timers compare channel
 *
 * @param[in]   eTimerSel   timer selection
 * @param[io]   pwCounter   storage for the counter value
 *
 * @return      appropriate error
 *
 *****************************************************************************/
TIMERSERR Timers_GetCompareVal( TIMERSENUM eTimerSel, PU16 pwCompare )
{
  TIMERSERR  eError = TIMERS_ERR_NONE;
  PTIMERDEF  ptDef;
  LCLCTL     tLclCtl;

  // valid timer
  if ( eTimerSel < TIMERS_ENUM_MAX )
  {
    // get the def file/get the registers
    ptDef = ( PTIMERDEF )&atTimerDefs[ eTimerSel ];
    GetLocalControl( ptDef->eChan, &tLclCtl );

    // read the period
    *pwCompare = tLclCtl.ptCcuRegs->CR;
  }
  else
  {
    // set the illegal timer error
    eError = TIMERS_ERR_ILLTIMER;
  }

  return( eError );
}

/******************************************************************************
 * @function Timers_SetCompareVal
 *
 * @brief set a timers compare channel value
 *
 * This function will store the value into the selected timers compare channel
 *
 * @param[in]   eTimerSel   timer selection
 * @param[in]   wCompare    new compare value
 *
 * @return
 *
 *****************************************************************************/
TIMERSERR Timers_SetCompareVal( TIMERSENUM eTimerSel, U16 wCompare )
{
  TIMERSERR  eError = TIMERS_ERR_NONE;
  PTIMERDEF  ptDef;
  LCLCTL     tLclCtl;

  // valid timer
  if ( eTimerSel < TIMERS_ENUM_MAX )
  {
    // get the def file/get the registers
    ptDef = ( PTIMERDEF )&atTimerDefs[ eTimerSel ];
    GetLocalControl( ptDef->eChan, &tLclCtl );

    // read the period
    tLclCtl.ptCcuRegs->CRS = wCompare;

    // set up the shadow registers
    CCU40->GCSS = BITS( CCU4_GCSS_S0SE_Msk , ( ptDef->eChan * 4 ));
  }
  else
  {
    // set the illegal timer error
    eError = TIMERS_ERR_ILLTIMER;
  }

  return( eError );
}

/******************************************************************************
 * @function Timers_SetComparePct
 *
 * @brief set a timers compare channel value
 *
 * This function will store the value into the selected timers compare channel
 *
 * @param[in]   eTimerSel   timer selection
 * @param[in]   wPercent    new compare value in percent tenths
 *
 * @return
 *
 *****************************************************************************/
TIMERSERR Timers_SetComparePct( TIMERSENUM eTimerSel, U16 wPercent )
{
  TIMERSERR  eError = TIMERS_ERR_NONE;
  PTIMERDEF  ptDef;
  LCLCTL     tLclCtl;
  U32				 uTemp;
  FLOAT			 fPercent;

  // valid timer
  if ( eTimerSel < TIMERS_ENUM_MAX )
  {
    // get the def file/get the registers
    ptDef = ( PTIMERDEF )&atTimerDefs[ eTimerSel ];
    GetLocalControl( ptDef->eChan, &tLclCtl );

    // calculate the new compare
    uTemp = tLclCtl.ptCcuRegs->PR;
    fPercent = wPercent / 1000.0;
    uTemp *= fPercent;
    tLclCtl.ptCcuRegs->CRS = uTemp;

    // set up the shadow registers
    CCU40->GCSS = BITS( CCU4_GCSS_S0SE_Msk , ( ptDef->eChan * 4 ));
  }
  else
  {
    // set the illegal timer error
    eError = TIMERS_ERR_ILLTIMER;
  }

  // return the error
  return( eError );
}

///******************************************************************************
// * @function Timers_GetCapture
// *
// * @brief gets the current value of the input capture register
// *
// * This function will return the current value of the input capture register
// *
// * @param[in]   eTimerSel   timer selection
// * @param[io]   pwCapture   storage for the capture value
// *
// * @return      appropriate error
// *
// *****************************************************************************/
//TIMERSERR Timers_GetCapture( TIMERSENUM eTimerSel, TIMERCHAN eChanSel, PU16 pwCapture )
//{
//  TIMERSERR eError = TIMERS_ERR_NONE;
//
//  // valid timer
//  if ( eTimerSel < TIMERS_ENUM_MAX )
//  {
//  }
//  else
//  {
//    // set the illegal timer error
//    eError = TIMERS_ERR_ILLTIMER;
//  }
//
//  return( eError );
//}

/******************************************************************************
 * @function GetLocalControl
 *
 * @brief sets up the local control based on channel
 *
 * This function will store the value into the selected timers compare channel
 *
 * @param[in]   eChan       channel number
 * @param[io]   ptCtl       pointer to the control structure
 *
 *****************************************************************************/
static void GetLocalControl( TIMERCHAN eChan, PLCLCTL ptCtl )
{
  switch( eChan )
  {
    case TIMER_CHAN_CCU0 :
      ptCtl->ptCcuRegs = CCU40_CC40;
      ptCtl->nIrqLevel = CCU40_IRQ_ENUM;
      break;

    case TIMER_CHAN_CCU1 :
      ptCtl->ptCcuRegs = CCU40_CC41;
      ptCtl->nIrqLevel = CCU41_IRQ_ENUM;
      break;

    case TIMER_CHAN_CCU2 :
      ptCtl->ptCcuRegs = CCU40_CC42;
      ptCtl->nIrqLevel = CCU42_IRQ_ENUM;
      break;

    case TIMER_CHAN_CCU3 :
      ptCtl->ptCcuRegs = CCU40_CC43;
      ptCtl->nIrqLevel = CCU43_IRQ_ENUM;
      break;

    default :
      ptCtl->ptCcuRegs = NULL;
      ptCtl->nIrqLevel = 0;
      break;
  }
}

/******************************************************************************
 * @function ComfigureCompare
 *
 * @brief configures the slice for a compare function
 *
 * This function will configure the selected channel for use as a compare
 * function
 *
 * @param[in]   eTimer      timer enumeration
 * @param[in]   ptDef       pointer to the definition structure
 * @param[in]   ptCtl       pointer to the control structure
 *
 *****************************************************************************/
static void ConfigureCompare( TIMERSENUM eTimer, PTIMERDEF ptDef, PLCLCTL ptCtl )
{
  U32   uTemp;
  FLOAT	fPercent;

  // clear the SRS register
  ptCtl->ptCcuRegs->SRS = 0;

  // set up prescale
  uTemp = BITS( ptDef->ePreScale, CCU4_CC4_PSC_PSIV_Pos );
  ptCtl->ptCcuRegs->PSC = uTemp;

  // set trap sync/edge in control
  uTemp = BIT( CCU4_CC4_TC_TRPSE_Pos ) | (( ptDef->eAlign == TIMER_CMPALIGN_CENTER ) ? BIT( CCU4_CC4_TC_TCM_Pos ) : 0 );
  ptCtl->ptCcuRegs->TC |= uTemp;

  // set the passive level
  ptCtl->ptCcuRegs->PSL = ptDef->bInvert;

  // set the period/compare register settings
  uTemp = SystemCoreClock / ptDef->uPeriodHz;
  ptCtl->ptCcuRegs->PRS = uTemp;
  fPercent = ptDef->wComparePct / 1000.0;
  uTemp *= fPercent;
  ptCtl->ptCcuRegs->CRS = 0;

  // clear all interrupts
  uTemp = BIT( CCU4_CC4_SWR_RPM_Pos ) | BIT( CCU4_CC4_SWR_ROM_Pos ) | BIT( CCU4_CC4_SWR_RCMU_Pos ) | BIT( CCU4_CC4_SWR_RCMD_Pos );
  uTemp |= BIT( CCU4_CC4_SWR_RE0A_Pos ) | BIT( CCU4_CC4_SWR_RE1A_Pos ) | BIT( CCU4_CC4_SWR_RE2A_Pos ) | BIT( CCU4_CC4_SWR_RTRPF_Pos );
  ptCtl->ptCcuRegs->SWR |= uTemp;

  // callback must be set if any interrupts are enabled
  if ( ptDef->pvCallback != NULL )
  {
    // set up the interrupts
    switch( ptDef->tIrq.eCmpIrq )
    {
      case TIMER_CMPIRQ_CMPUP :
        ptCtl->ptCcuRegs->INTE |= BIT( CCU4_CC4_INTE_CMUE_Pos );
        ptCtl->ptCcuRegs->SRS |= BITS( ptDef->eChan, CCU4_CC4_SRS_CMSR_Pos );
        break;

      case TIMER_CMPIRQ_CMPDN :
        ptCtl->ptCcuRegs->INTE |= BIT( CCU4_CC4_INTE_CMDE_Pos );
        ptCtl->ptCcuRegs->SRS |= BITS( ptDef->eChan, CCU4_CC4_SRS_CMSR_Pos );
        break;

      case TIMER_CMPIRQ_CMPBOTH :
        ptCtl->ptCcuRegs->INTE |= BIT( CCU4_CC4_INTE_CMUE_Pos ) | BIT( CCU4_CC4_INTE_CMDE_Pos );
        ptCtl->ptCcuRegs->SRS |= BITS( ptDef->eChan, CCU4_CC4_SRS_CMSR_Pos );
        break;

      case TIMER_CMPIRQ_PERUP :
        ptCtl->ptCcuRegs->INTE |= BIT( CCU4_CC4_INTE_PME_Pos );
        ptCtl->ptCcuRegs->SRS |= BITS( ptDef->eChan, CCU4_CC4_SRS_POSR_Pos );
        break;

      default :
        ptCtl->ptCcuRegs->INTE = 0;
        break;
    }

    // now enable the interrupt
    if ( ptCtl->ptCcuRegs->INTE != 0 )
    {
      // set the interrupt priority/enable
      NVIC_SetPriority( ptCtl->nIrqLevel, ptDef->nIrqPri );
      NVIC_EnableIRQ( ptCtl->nIrqLevel );
    }
  }
  else
  {
    // disable all interrupts
    ptCtl->ptCcuRegs->INTE = 0;
  }

  // set up the shadow registers
  uTemp = BITS(( CCU4_GCSS_S0SE_Pos | CCU4_GCSS_S0DSE_Pos | CCU4_GCSS_S0PSE_Pos | CCU4_GCSS_S1SE_Pos ), ( ptDef->eChan * 4 ));
  CCU40->GCSS = uTemp;

  // if initially on
  if ( ptDef->bInitialOn )
  {
    // start it
    Timers_StartStop( eTimer, ON );
  }
}

/******************************************************************************
 * @function ComfigureCapture
 *
 * @brief configures the slice for a capture function
 *
 * This function will configure the selected channel for use as a capture
 * function
 *
 * @param[in]   eTimer      timer enumeration
 * @param[in]   ptDef       pointer to the definition structure
 * @param[in]   ptCtl       pointer to the control structure
 *
 *****************************************************************************/
static void ConfigureCapture( TIMERSENUM eTimer, PTIMERDEF ptDef, PLCLCTL ptCtl )
{
  U32   uTemp;

  // set aligned edge
  ptCtl->ptCcuRegs->TC = 0;
  ptCtl->ptCcuRegs->SRS = 0;

  // determine if this is a single edge or dual edge
  switch( ptDef->eEdgeSel )
  {
    case TIMER_EDGESEL_RISING :
      uTemp = BITS( ptDef->eEventSel, CCU4_CC4_INS_EV0IS_Pos );
      uTemp |= BITS( ptDef->eEdgeSel, CCU4_CC4_INS_EV0EM_Pos );
      ptCtl->ptCcuRegs->CMC = BIT( CCU4_CC4_CMC_CAP0S_Pos );
      ptCtl->ptCcuRegs->INTE |= BIT( CCU4_CC4_INTE_E0AE_Pos );
      ptCtl->ptCcuRegs->SRS |= BITS( ptDef->eChan, CCU4_CC4_SRS_E0SR_Pos );
      break;

    case TIMER_EDGESEL_FALLING :
      uTemp = BITS( ptDef->eEventSel, CCU4_CC4_INS_EV1IS_Pos );
      uTemp |= BITS( ptDef->eEdgeSel, CCU4_CC4_INS_EV1EM_Pos );
      ptCtl->ptCcuRegs->CMC = BIT( CCU4_CC4_CMC_CAP1S_Pos );
      ptCtl->ptCcuRegs->INTE |= BIT( CCU4_CC4_INTE_E1AE_Pos );
      ptCtl->ptCcuRegs->SRS |= BITS( ptDef->eChan, CCU4_CC4_SRS_E1SR_Pos );
      break;

    case TIMER_EDGESEL_BOTH :
      uTemp = ( BITS( ptDef->eEventSel, CCU4_CC4_INS_EV0IS_Pos ) | BITS( ptDef->eEventSel, CCU4_CC4_INS_EV1IS_Pos ));
      uTemp |= ( BITS( TIMER_EDGESEL_RISING, CCU4_CC4_INS_EV0EM_Pos ) | BITS( TIMER_EDGESEL_FALLING, CCU4_CC4_INS_EV1EM_Pos ) );
      ptCtl->ptCcuRegs->CMC = ( BIT( CCU4_CC4_CMC_CAP0S_Pos ) | BITS( 2, CCU4_CC4_CMC_CAP1S_Pos ));
      ptCtl->ptCcuRegs->INTE |= ( BIT( CCU4_CC4_INTE_E0AE_Pos ) | BIT( CCU4_CC4_INTE_E1AE_Pos ));
      ptCtl->ptCcuRegs->SRS |= ( BITS( ptDef->eChan, CCU4_CC4_SRS_E0SR_Pos ) | BITS( ptDef->eChan, CCU4_CC4_SRS_E1SR_Pos ));
      break;

    default :
      break;
  }

  // set the input selection
  ptCtl->ptCcuRegs->INS = uTemp;

  // set capture mode and clear on capture/set continuous capture
  uTemp = BIT( CCU4_CC4_TC_CMOD_Pos ) | BITS( 3, CCU4_CC4_TC_CAPC_Pos );
  uTemp |= BIT( CCU4_CC4_TC_CCS_Pos );
  ptCtl->ptCcuRegs->TC |= uTemp;

  // set up the prescale
  uTemp = BITS( ptDef->ePreScale, CCU4_CC4_PSC_PSIV_Pos );
  ptCtl->ptCcuRegs->PSC = uTemp;

  // clear all interrupts
  uTemp = BIT( CCU4_CC4_SWR_RPM_Pos ) | BIT( CCU4_CC4_SWR_ROM_Pos ) | BIT( CCU4_CC4_SWR_RCMU_Pos ) | BIT( CCU4_CC4_SWR_RCMD_Pos );
  uTemp |= BIT( CCU4_CC4_SWR_RE0A_Pos ) | BIT( CCU4_CC4_SWR_RE1A_Pos ) | BIT( CCU4_CC4_SWR_RE2A_Pos ) | BIT( CCU4_CC4_SWR_RTRPF_Pos );
  ptCtl->ptCcuRegs->SWR |= uTemp;

  // callback must be set if any interrupts are enabled
  if ( ptDef->pvCallback != NULL )
  {
    // now enable the interrupt
    if ( ptCtl->ptCcuRegs->INTE != 0 )
    {
      // set the interrupt priority/enable
      NVIC_SetPriority( ptCtl->nIrqLevel, ptDef->nIrqPri );
      NVIC_EnableIRQ( ptCtl->nIrqLevel );
    }
  }
  else
  {
    // disable all interrupts
    ptCtl->ptCcuRegs->INTE = 0;
  }

  // if initially on
  if ( ptDef->bInitialOn )
  {
    // start it
    Timers_StartStop( eTimer, ON );
  }
}

/******************************************************************************
 * @function CCU40_Irq_Handler
 *
 * @brief CCU40 IRQ handler
 *
 * This function handles the interrupts from CCU40
 *
 *****************************************************************************/
void CCU40_Irq_Handler( void )
{
  // call the common interrupt handler
  CommonInterruptHandler( atPhyMaps[ TIMER_CHAN_CCU0 ].eTimer );
}

/******************************************************************************
 * @function CCU41_Irq_Handler
 *
 * @brief CCU41 IRQ handler
 *
 * This function handles the interrupts from CCU41
 *
 *****************************************************************************/
void CCU41_Irq_Handler( void )
{
  // call the common interrupt handler
  CommonInterruptHandler( atPhyMaps[ TIMER_CHAN_CCU1 ].eTimer );
}

/******************************************************************************
 * @function CCU42_Irq_Handler
 *
 * @brief CCU42 IRQ handler
 *
 * This function handles the interrupts from CCU42
 *
 *****************************************************************************/
void CCU42_Irq_Handler( void )
{
  // call the common interrupt handler
  CommonInterruptHandler( atPhyMaps[ TIMER_CHAN_CCU2 ].eTimer );
}

/******************************************************************************
 * @function CCU43_Irq_Handler
 *
 * @brief CCU43 IRQ handler
 *
 * This function handles the interrupts from CCU43
 *
 *****************************************************************************/
void CCU43_Irq_Handler( void )
{
  // call the common interrupt handler
  CommonInterruptHandler( atPhyMaps[ TIMER_CHAN_CCU3 ].eTimer );
}

/******************************************************************************
 * @function CommonInterruptHandler
 *
 * @brief common interrupt handler
 *
 * This function handles all the interrupts for all  channels
 *
 *****************************************************************************/
static void CommonInterruptHandler( TIMERSENUM eTimer )
{
  PTIMERDEF       ptDef;
  LCLCTL          tCtl;
  PVTIMERCALLBACK pvCallback;
  TIMERCBEVENT    eEvent = TIMER_CBEVENT_NONE;
  U16             wValue = 0;

  // get pointer to the definition/control structures
  ptDef = ( PTIMERDEF )&atTimerDefs[ eTimer ];
  GetLocalControl( ptDef->eChan, &tCtl );

  // get the callback
  pvCallback = ptDef->pvCallback;

  // determine the event
  switch( ptDef->eMode )
  {
    case TIMER_MODE_COMPARE :
      switch( ptDef->tIrq.eCmpIrq )
      {
        case TIMER_CMPIRQ_CMPUP :
          eEvent = TIMER_CBEVENT_CMPUP;
          tCtl.ptCcuRegs->SWR = CCU4_CC4_SWR_RCMU_Msk;
          break;

        case TIMER_CMPIRQ_CMPDN :
          eEvent = TIMER_CBEVENT_CMPDN;
          tCtl.ptCcuRegs->SWR = CCU4_CC4_SWR_RCMD_Msk;
          break;

        case TIMER_CMPIRQ_CMPBOTH :
          eEvent = ( tCtl.ptCcuRegs->INTS & CCU4_CC4_INTS_CMUS_Msk ) ? TIMER_CBEVENT_CMPUP : TIMER_CBEVENT_CMPDN;
          tCtl.ptCcuRegs->SWR = CCU4_CC4_SWR_RCMU_Msk | CCU4_CC4_SWR_RCMD_Msk;
          break;

        case TIMER_CMPIRQ_PERUP :
          eEvent = TIMER_CBEVENT_PERUP;
          tCtl.ptCcuRegs->SWR = CCU4_CC4_SWR_RPM_Msk;
          break;

       default :
          break;
      }

      // now perform the callback
      pvCallback( eEvent, wValue );
      break;

    case TIMER_MODE_CAPTURE :
      // check for rising edge
      if ( tCtl.ptCcuRegs->INTS & CCU4_CC4_INTS_E0AS_Msk )
      {
        // set the rising edge event/get the captured value
        eEvent = TIMER_CBEVENT_CAPRE;
        wValue = tCtl.ptCcuRegs->CV[ 1 ];

        // now perform the callback
        pvCallback( eEvent, wValue );

        // clear the interrupt
        tCtl.ptCcuRegs->SWR = CCU4_CC4_SWR_RE0A_Msk;
      }
      
      // check for falling edge
      if ( tCtl.ptCcuRegs->INTS & CCU4_CC4_INTS_E1AS_Msk )
      {
        // set the falling edge event/get the captured value
        eEvent = TIMER_CBEVENT_CAPFE;
        wValue = tCtl.ptCcuRegs->CV[ 3 ];

        // now perform the callback
        pvCallback( eEvent, wValue );

        // clear the interrupt
        tCtl.ptCcuRegs->SWR = CCU4_CC4_SWR_RE1A_Msk;
      }
      break;

    default :
      break;
  }

  // now perform the callback
  // pvCallback( eEvent, wValue );
}

/**@} EOF Timers.c */
