/******************************************************************************
 * @file Timers.c
 *
 * @brief timer implementation 
 *
 * This file provides the implementation for the timer module
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
 * \addtogroup Timers
 * @{
 *****************************************************************************/

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "Timers/Timers.h"

// library includes -----------------------------------------------------------
#include "Clock/Clock.h"

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------
/// define the control structure
typedef struct _LCLCTL
{
  TcChannel*    ptTimer;            /// pointer to the control structure
} LCLCTL, *PLCLCTL;
#define LCLCTL_SIZE             sizeof( LCLCTL )

// global parameter declarations ----------------------------------------------

// local parameter declarations -----------------------------------------------
static  LCLCTL    atLclCtls[ TIMER_ENUM_MAX ];
static  TIMERCHAN aePhyToDefMap[ TIMER_CHAN_MAX ];

// local function prototypes --------------------------------------------------
static  TcChannel*  GetTimerPointer( TIMERSEL eTimer, TIMERCHAN eChannel );
static  void        IrqCommonHandler( TIMERCHAN eChannel );

// constant parameter initializations -----------------------------------------
static  const U32   auClkDividers[ ] =
{
  2, 8, 32, 128
};

/******************************************************************************
 * @function Timers_Initialize
 *
 * @brief initialization
 *
 * This function provides the initialization for the timers
 *
 *****************************************************************************/
void Timers_Initialize( void )
{
  TIMERENUM eTimerEnum;
  PTIMERDEF ptDef;
  PLCLCTL   ptCtl;
  U32       uValue;
  
  // for each timer
  for( eTimerEnum = 0; eTimerEnum < TIMER_ENUM_MAX; eTimerEnum++ )
  {
    // get a pointer to the definition
    ptDef = ( PTIMERDEF )&atTimerDef[ eTimerEnum ];
    ptCtl = &atLclCtls[ eTimerEnum ];
    
    // get a pointer to the timer
    ptCtl->ptTimer =  GetTimerPointer( ptDef->eSelect );
    
    // now set the mode based on capture/wave
    switch( ptDef->eMode )
    {
      case TIMER_MODE_CAPTURE :
        // build the mode value
        uValue = ptDef->eClockSel;
        uValue |= BITS( ptDef->tEventEdge.aeCaptureEdges[ TIME_CMPCAP_CHAN_A ], TC_CMR_LDRA_Pos );
        uValue |= BITS( ptDef->tEventEdge.aeCaptureEdges[ TIME_CMPCAP_CHAN_B ], TC_CMD_LDRB_POS );
        uValue |= BITS( ptDef->eTrigEdge, TC_CMR_ETRGEDG_Pos );
        uValue |= ( ptDef->eExtTrig ) ? TC_CMR_ABETRG : 0;
        ptCtl->ptTimer->TC_CMR = uValue;
        
        // set the interrupt flags
        uValue = ( ptDef->tIntFlags.bCntOverFlow ) ? TC_IER_COVFS : 0;
        uValue |= ( ptDef->tIntFlags.bLoadRegA ) ? TC_IER_LDRAS : 0;
        uValue |= ( ptDef->tIntFlags.bLoadRegB ) ? TC_IER_LDRBS : 0;
        ptCtl->ptTimer->TC_IER = uValue;
        
        // if initial on, turn on timer
        if ( ptDef->bInitialOn )
        {
          // turn on timer
          ptCtl->ptTimer->TC_CCR = TC_CCR_CLKEN;
        }
        break;
        
      case TIMER_MODE_WAVEUP :
      case TIMER_MODE_WAVEUPDOWN :
      case TIMER_MODE_WAVEUPRC :
      case TIMER_MODE_WAVEUPDOWNRC :
        // build the mode
        uValue = ptDef->eClockSel;
        uValue |= BITS( ptDef->eMode, TC_CMR_WAVSEL_Pos );
        uValue |= TC_CMR_WAVE;
        uValue |= BITS( ptDef->tEventEdge.aeEvents[ TIME_CMPCAP_CHAN_A ], TC_CMR_ACPA_Pos );
        uValue |= BITS( ptDef->tEventEdge.aeEvents[ TIME_CMPCAP_CHAN_B ], TC_CMR_BCPB_Pos );
        ptCtl->ptTimer->TC_CMR = uValue;
        
        // set the interrupt flags
        uValue = ( ptDef->tIntFlags.bCntOverFlow ) ? TC_IER_COVFS : 0;
        uValue |= ( ptDef->tIntFlags.bLoadRegA ) ? TC_IER_LDRAS : 0;
        uValue |= ( ptDef->tIntFlags.bLoadRegB ) ? TC_IER_LDRBS : 0;
        ptCtl->ptTimer->TC_IER = uValue;
        
        // set the period ( RC )
        uValue = Clock_GetFreq( );
        if ( ptDef->eClockSel < TIMER_CLOCKSEL_TIMCLK5 )
        {
          uValue /= auClkDividers[ ptDef->eClockSel ];
        }
        uValue /= ptDef->uPeriod;
        ptCtl->ptTimer->TC_RC;

        // if initial on, turn on timer
        if ( ptDef->bInitialOn )
        {
          // turn on timer
          ptCtl->ptTimer->TC_CCR = TC_CCR_CLKEN;
        }
        break;

      default :
        break;
    }
  }
}

/******************************************************************************
 * @function Timers_Close
 *
 * @brief close the timers
 *
 * This function will close all tiemrs
 *
 *****************************************************************************/
void Timers_Close( void )
{
  TIMERENUM eTimerEnum;
  PLCLCTL   ptCtl;
  
  // for each timer
  for( eTimerEnum = 0; eTimerEnum < TIMER_ENUM_MAX; eTimerEnum++ )
  {
    // get the pointer to the control
    ptCtl = &atLclCtls[ eTimerEnum ];
    
    // turn on timer
    ptCtl->ptTimer->TC_CCR = TC_CCR_CLKDIS;
    ptCtl->ptTimer->TC_CMR = 0;
  }
}

/******************************************************************************
 * @function Timers_Ioctl
 *
 * @brief control timer
 *
 * This function provides the miscellaneous control functions for the timer
 *
 * @param[in]   eTimerSel   timer select
 * @param[in]   eAction     timer action
 * @param[in]   pvParam     pointer to the parameter
 *
 * @return      appropriate error
 *
 *****************************************************************************/
TIMERERR Timers_Ioctl( TIMERENUM eTimerSel, TIMERIOCTL eAction, PVOID pvParam )
{
  TIMERERR  eError = TIMER_ERR_NONE;
  PTIMERDEF ptDef;
  PLCLCTL   ptCtl;

  // check for valid timer select
  if ( eTimerSel < TIMER_ENUM_MAX )
  {
    // get a pointer to the definition
    ptDef = ( PTIMERDEF )&atTimerDef[ eTimerSel ];
    ptCtl = &atLclCtls[ eTimerSel ];

    switch( eAction )
    {
      case TIMER_IOCTL_CHANGEDEF :
        break;
        
      case TIMER_IOCTL_STOPSTART :
        if ( PARAMBOOL( pvParam ) == ON )
        {
          // turn on the timer
          ptCtl->ptTimer->TC_CCR = TC_CCR_CLKEN;
        }
        else
        {
          // turn off the timer
          ptCtl->ptTImer->TC_CCR = TC_CCR_CLKDIS;
        }
        break;
        
      case TIMER_IOCTL_GETCOUNT :
        break;
        
      case TIMER_IOCTL_SETCOUNT :
        break;
        
      case TIMER_IOCTL_GETPERIOD : 
        break;
        
      case TIMER_IOCTL_SETPERIOD :
        break;
        
      case TIMER_IOCTL_GETCOMPAREVAL :
        break;
        
      case TIMER_IOCTL_SETCOMPAREVAL :
        break;
        
      case TIMER_IOCTL_SETCOMPAREPCT :
        break;
        
      case TIMER_IOCTL_GETCAPTURE :
        break;
        
      case TIMER_IOCTL_SETCAPTURE :
        break;
        
      case TIMER_IOCTL_GETPRESCALEDIV :
        break;

      default :
        eError = TIMER_ERR_ILLACTION;
        break;
    }
  }
  else
  {
    // rerport error
    eError = TIMER_ERR_ILLTIMER;
  }
  
  // return the error
  return( eError );
}

/******************************************************************************
 * @function TC0_Handler
 *
 * @brief interrupt TC0 handler
 *
 * This function handles the interrupts from TC0
 *
 *****************************************************************************/
void TC0_Handler( void )
{
  // call the common handler
  IrqCommonHandler( aePhyToDefMap[ TIMER_CHAN_0 ] );
}

#ifdef TC1
/******************************************************************************
 * @function TC1_Handler
 *
 * @brief interrupt TC0 handler
 *
 * This function handles the interrupts from TC0
 *
 *****************************************************************************/
void TC1_Handler( void )
{
  // call the common handler
  IrqCommonHandler( aePhyToDefMap[ TIMER_CHAN_1 ] );
}
#endif // TC1

#ifdef TC2
/******************************************************************************
 * @function TC2_Handler
 *
 * @brief interrupt TC0 handler
 *
 * This function handles the interrupts from TC0
 *
 *****************************************************************************/
void TC2_Handler( void )
{
  // call the common handler
  IrqCommonHandler( aePhyToDefMap[ TIMER_CHAN_2 ] );
}
#endif // TC2

/******************************************************************************
 * @function IrqCommonHandler
 *
 * @brief common interrupt handler
 *
 * This function will process the interrupts 
 *
 * @param[in]   eChannel    timer channel
 *
 *****************************************************************************/
static void IrqCommonHandler( TIMERCHAN eChannel )
{
  PTIMERDEF       ptDef;
  PLCLCTL         ptCtl;
  U32             uStatus;
  PVTIMERCALLBACK pvCallback;
  
  // get a pointer to the definition
  ptDef = ( PTIMERDEF )&atTimerDef[ eChannel ];
  ptCtl = &atLclCtls[ eChannel ];
  
  // get the status
  uStatus = ptCtl->ptTimer->TC_SR;
  
  // check for a valid callback
  if (( pvCallback = ptDef->pvCallback ) != NULL )
  {
    // process the counter overflow
    if (( uStatus & TC_SR_COVFS ) == TC_SR_COVFS ) && ( ptDef->tIntFlags.bCntOverflow ))
    {
      // call the handler
      pvCallback( TIMER_CBEVENT_TMO, eChannel, 0 );
    }
    
    // process capture callback
    if (( uStatus & TC_SR_LDRAS ) == TC_SR_LDRAS ) && ( ptDef->tIntFlags.bLoadRegA ))
    {
      // call the handler
      pvCallback(TIMER_CBEVENT_CAPTUREA, eChannel, ptCtl->TC_RA );
    }

    // process capture callback
    if (( uStatus & TC_SR_LDRBS ) == TC_SR_LDRBS ) && ( ptDef->tIntFlags.bLoadRegB )))
    {
      // call the handler
      pvCallback(TIMER_CBEVENT_CAPTUREB, eChannel, ptCtl->TC_RB );
    }
  }
}

/******************************************************************************
 * @function GetTimerPointer
 *
 * @brief get a pointer to the timer registers
 *
 * This function will return the pointer to the correct channel resgisters
 *
 * @param[in]   eTimer    timer channel
 * @param[in]   eChannel    timer channel
 *
 * @return      pointer, or NULL if illegal
 *
 *****************************************************************************/
static TcChannel* GetTimerPointer( TIMERSEL eTimer, TIMERCHAN eChannel )
{
  TcChannel* ptTimer = NULL;
  
  // check for valid timer/channel
  if (( eTimer < TIMER_SEL_MAX ) && ( eChannel < TIMER_CHAN_MAX ))
  {
    switch( eTimer )
    {
      case TIMER_SEL_0 :
        ptTimer = TC0.TC_CHANNEL[ eChannel ];
        break;

      case TIMER_SEL_1 :
        ptTimer = TC1.TC_CHANNEL[ eChannel ];
        break;
        
      default :
        break;
    }
  }
  
  // return the pointer
  return( ptTimer );
}

/**@} EOF Timers.c */
