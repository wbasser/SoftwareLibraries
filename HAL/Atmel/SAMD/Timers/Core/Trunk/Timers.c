/******************************************************************************
 * @file Timers.c
 *
 * @brief timers implementation
 *
 * This file provides the implementation for the timers module
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
 * \addtogroup Timers
 * @{
 *****************************************************************************/

// local includes -------------------------------------------------------------
#include "Timers/Timers.h"

// library includes -----------------------------------------------------------
#include "Clock/Clock.h"

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------
/// define the physical to control map
typedef struct _PHYMAP
{
  TIMERENUM   eTimer;             ///< enumerated device
} PHYMAP, *PPHYMAP;
#define PHYMAP_SIZE   sizseof( PHYMAP )

/// define the struct for local control
typedef struct _LCLCTL
{
  CLOCKMUXID  eMuxId;           ///< clock mux id
  union
  {
    Tc*         ptTc;           ///< pointer to the control registers for TC3-7
    Tcc*        ptTcc;          ///< pointer to the control register for TCC0-2
  } tBasePtrs;
  
} LCLCTL, *PLCLCTL;
#define LCLCTL_SIZE   sizeof( LCLCTL )

// global parameter declarations ----------------------------------------------

// local parameter declarations -----------------------------------------------
static  PHYMAP  atPhyMaps[ TIMER_CHAN_MAX ];

// local function prototypes --------------------------------------------------
static  void        CommonTcInterruptHandler( TIMERENUM eTimer, Tc* ptTc );
static  void        CommonTccInterruptHandler( TIMERENUM eTimer, Tcc* ptTcc );
static  void        ConfigureTccPwmSing( PTIMERDEF ptDef );
static  void        ConfigureTccPwmDual( PTIMERDEF ptDef );
static  void        ConfigureTccCapture( PTIMERDEF ptDef );
static  void        ConfigureTcWave( PTIMERDEF ptDef );
static  void        ConfigureTcPwm( PTIMERDEF ptDef );
static  void        ConfigureTcCapture( PTIMERDEF ptDef );
static  void        ConfigureTcCapturePXX( PTIMERDEF ptDef );
static  void        GetTimerChannel( TIMERCHAN eChannel, PLCLCTL ptLclCtl );
static  U32         GetPrescaleValue( TIMERPRESCALE ePrescale );

// constant parameter initializations -----------------------------------------
const U16 awTimerPrescales[ TIMER_PRESCALE_MAX ] = 
{
  1, 2, 4, 8, 16, 64, 256, 1024
};

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
  TIMERENUM    eTimer;
  PTIMERDEF    ptDef;

  // for each timer in the config table
  for ( eTimer = 0; eTimer < TIMER_ENUM_MAX; eTimer++ )
  {
    // get the definition/register pointer
    ptDef = ( PTIMERDEF )&atTimerDefs[ eTimer ];

    // set the physical to level translation
    atPhyMaps[ ptDef->eChannel ].eTimer = eTimer;

    // now determine the type of timer
    switch( ptDef->eChannel )
    {
      #ifdef __SAM_D10_SUBFAMILY
      case TIMER_CHAN_0 :
        // determine the mode
        switch( ptDef->tModes.eTcc )
        {
          case TIMER_TCCMODE_WAVNF :
          case TIMER_TCCMODE_WAVMF :
            break;

          case TIMER_TCCMODE_PWMSING :
            // configure the PWM
            ConfigureTccPwmSing( ptDef );
            break;

          case TIMER_TCCMODE_PWMDUAL_TOP :
          case TIMER_TCCMODE_PWMDUAL_BOT :
          case TIMER_TCCMODE_PWMDUAL_TOPBOT :
          case TIMER_TCCMODE_PWMDUAL_CRITBOT :
            // configure the PWM
            ConfigureTccPwmDual( ptDef );
            break;

          case TIMER_TCCMODE_CAPTURE :
            // configure the capture
            ConfigureTccCapture( ptDef );
            break;
            
          default :
            break;
        }
        break;
        
      case TIMER_CHAN_1 :
      case TIMER_CHAN_2 :
        // determine the mode
        switch( ptDef->tModes.eTc )
        {
          case TIMER_TCMODE_WAVNF :
          case TIMER_TCMODE_WAVMF :
            // configure the wave output
            ConfigureTcWave( ptDef );
            break;

          case TIMER_TCMODE_PWM :
            // configure the PWM output
            ConfigureTcPwm( ptDef );
            break;

          case TIMER_TCMODE_CAPTURE :
            // configure the capture
            ConfigureTcCapture( ptDef );
            break;

          case TIMER_TCMODE_CAPPPW :
          case TIMER_TCMODE_CAPPWP :
            ConfigureTcCapturePXX( ptDef );
            break;
            
          default :
            break;
        }
        break;
      #else
      case TIMER_CHAN_0 :
      case TIMER_CHAN_1 :
      case TIMER_CHAN_2 :
        // determine the mode
        switch( ptDef->tModes.eTcc )
        {
          case TIMER_TCCMODE_WAVNF :
          case TIMER_TCCMODE_WAVMF :
            break;

          case TIMER_TCCMODE_PWMSING :
            // configure the PWM
            ConfigureTccPwmSing( ptDef );
            break;

          case TIMER_TCCMODE_PWMDUAL_TOP :
          case TIMER_TCCMODE_PWMDUAL_BOT :
          case TIMER_TCCMODE_PWMDUAL_TOPBOT :
          case TIMER_TCCMODE_PWMDUAL_CRITBOT :
            // configure the PWM
            ConfigureTccPwmDual( ptDef );
            break;

          case TIMER_TCCMODE_CAPTURE :
            // configure the capture
            ConfigureTccCapture( ptDef );
            break;
            
          default :
            break;
        }
        break;
        
      case TIMER_CHAN_3 :
      case TIMER_CHAN_4 :
      case TIMER_CHAN_5 :
      #ifdef TC6
      case TIMER_CHAN_6 :
      #endif  // TC6
      #ifdef TC7
      case TIMER_CHAN_7 :
      #endif  // TC7
        // determine the mode
        switch( ptDef->tModes.eTc )
        {
          case TIMER_TCMODE_WAVNF :
          case TIMER_TCMODE_WAVMF :
            // configure the wave output
            ConfigureTcWave( ptDef );
            break;

          case TIMER_TCMODE_PWM :
            // configure the PWM output
            ConfigureTcPwm( ptDef );
            break;

          case TIMER_TCMODE_CAPTURE :
            // configure the capture
            ConfigureTcCapture( ptDef );
            break;

          case TIMER_TCMODE_CAPPPW :
          case TIMER_TCMODE_CAPPWP :
            ConfigureTcCapturePXX( ptDef );
            break;
            
          default :
            break;
        }
        break;
      #endif
        
      default :
        break;
    }
  }
}

/******************************************************************************
 * @function Timers_Close
 *
 * @brief timer close
 *
 * This function closes all configured timers
 *
 *****************************************************************************/
void Timers_Close( void )
{
  TIMERENUM    eTimer;
  PTIMERDEF    ptDef;
  LCLCTL       tLclCtl;

  // for each timer in the config table
  for ( eTimer = 0; eTimer < TIMER_ENUM_MAX; eTimer++ )
  {
    // get the definition/register pointer
    ptDef = ( PTIMERDEF )&atTimerDefs[ eTimer ];

    // get the channel
    GetTimerChannel( ptDef->eChannel, &tLclCtl );

    // now determine the type of timer
    switch( ptDef->eChannel )
    {
      case TIMER_CHAN_0 :
      case TIMER_CHAN_1 :
      case TIMER_CHAN_2 :
        // reset TCC0
        tLclCtl.tBasePtrs.ptTcc->CTRLA.bit.SWRST = ON;
        while( tLclCtl.tBasePtrs.ptTcc->SYNCBUSY.bit.SWRST );
        break;
      
      case TIMER_CHAN_3 :
      case TIMER_CHAN_4 :
      case TIMER_CHAN_5 :
      #ifdef TC6
      case TIMER_CHAN_6 :
      #endif  // TC6
      #ifdef TC7
      case TIMER_CHAN_7 :
      #endif  // TC7
        // reset it/wait for sync
        tLclCtl.tBasePtrs.ptTc->COUNT16.CTRLA.bit.SWRST = ON;
        while(( tLclCtl.tBasePtrs.ptTc->COUNT16.STATUS.reg & TC_STATUS_SYNCBUSY ) != 0 );
        break;
      
      default :
        break;
    }

    // disable the interrupt
    NVIC_DisableIRQ( TCC0_IRQn + ptDef->eChannel );
  }
}

/******************************************************************************
 * @function Timers_Ioctl
 *
 * @brief perform an IOCTL action on a selected timer
 *
 * This function will check for a valid action and perform that action on
 * a given timer
 *
 * @param[in]   eTimerSel   timer selection
 * @param[in]   eAction     IOCTL action
 * @param[io]   pvParam     pointer to the parameter
 *
 * @return      appropriate error
 *
 *****************************************************************************/
TIMERERR Timers_Ioctl( TIMERENUM eTimerSel, TIMERIOCTL eAction, PVOID pvParam )
{
  TIMERERR          eError = TIMER_ERR_NONE;
  PTIMERDEF         ptDef;
  TIMERCHAN         eChannel;
  LCLCTL            tCtl;
  PTIMERIOCTLPARAM  ptParam;
  U32               uDivider, uTemp, uPrescale, uParam;
  CLOCKMUXID        eClockMux;
  FLOAT             fPercent;
  BOOL              bState;

  // valid timer
  if ( eTimerSel < TIMER_ENUM_MAX )
  {
    // get the pointer to the contro;
    ptDef = ( PTIMERDEF )&atTimerDefs[ eTimerSel ];

    // get the channel
    eChannel = ptDef->eChannel;

    // get the control pointer
    GetTimerChannel( eChannel, &tCtl );

    // compute the divider for the counter
    uDivider = Clock_GetMultiplexerFreq( tCtl.eMuxId ) / awTimerPrescales[ ptDef->ePrescale ];

    // determine the action
    switch( eAction )
    {
      case TIMER_IOCTL_CHANGEDEF :
        break;
        
      case TIMER_IOCTL_STOPSTART :
        // get the state
        bState = *( PBOOL )pvParam;

        // determine channel
        if ( eChannel < TIMER_CHAN_3 )
        {
          // clear all interrupt flags
          tCtl.tBasePtrs.ptTcc->INTFLAG.reg = TCC_INTFLAG_MASK;

          // enabe it
          tCtl.tBasePtrs.ptTcc->CTRLA.bit.ENABLE = bState;
          while( tCtl.tBasePtrs.ptTcc->SYNCBUSY.bit.ENABLE );

          // check for one shot
          if ( ptDef->tFlags.bOneShot )
          {
            // start a conversion
            tCtl.tBasePtrs.ptTcc->CTRLBSET.reg = TCC_CTRLBSET_ONESHOT;		  
            tCtl.tBasePtrs.ptTcc->CTRLBSET.reg = TCC_CTRLBSET_CMD_RETRIGGER;
            while( tCtl.tBasePtrs.ptTcc->SYNCBUSY.bit.CTRLB );
          }
        }
        else
        {
          // clear all interrupt flags
          tCtl.tBasePtrs.ptTc->COUNT16.INTFLAG.reg = TC_INTFLAG_MASK;

          // check for capture
          if ( ptDef->tModes.eTc == TIMER_TCMODE_CAPTURE )
          {
            // set the initial edge
            tCtl.tBasePtrs.ptTc->COUNT16.EVCTRL.bit.TCINV = ptDef->tFlags.bInvertEdge;
          }

          // enable the timer
          tCtl.tBasePtrs.ptTc->COUNT16.CTRLA.bit.ENABLE = bState;
          while ( tCtl.tBasePtrs.ptTc->COUNT16.STATUS.bit.SYNCBUSY );
        }
        break;
        
      case TIMER_IOCTL_GETCOUNT :
        break;
        
      case TIMER_IOCTL_SETCOUNT :
        break;
        
      case TIMER_IOCTL_GETPERIOD :
        break;
        
      case TIMER_IOCTL_SETPERIOD :
        if ( ptDef->eChannel < TIMER_CHAN_3 )
        {
          // get the value from the pvoid parameter
          uParam = PARAMU16( pvParam );

          // determine the waveform mode
          switch( ptDef->tModes.eTcc )
          {
            case TIMER_TCCMODE_PWMSING :
              // if times in usec
              if ( ptDef->tFlags.bTimesInUsec )
              {
                // convwerrt the divider into Nsecs
                uDivider = 1000000000ul / uDivider;

                // now compute the reload values for period and channels
                uTemp = ( uParam  * 1000 ) / uDivider; 
              }
              else
              {
                // compute the period
                uTemp = ( uDivider / uParam ) / GetPrescaleValue( ptDef->ePrescale );
              }

              // write it
              tCtl.tBasePtrs.ptTcc->PER.reg = uTemp;
              while( tCtl.tBasePtrs.ptTcc->SYNCBUSY.bit.PER );
              break;

            default :
              break;
          }
        }
        else
        {
          // determine the waveform mode
          switch( ptDef->tModes.eTc )
          {
            case TIMER_TCMODE_WAVNF :
              break;

            case TIMER_TCMODE_WAVMF :
              break;

            default :
              break;
          }
        }
        break;
        
      case TIMER_IOCTL_GETCOMPAREVAL :
        break;
        
      case TIMER_IOCTL_SETCOMPAREVAL :
        break;
        
      case TIMER_IOCTL_SETCOMPAREPCT :
        // get a pointer to the parameter
        ptParam = ( PTIMERIOCTLPARAM )pvParam;

        // check for which timer
        if ( ptDef->eChannel < TIMER_CHAN_3 )
        {
          // determine the waveform mode
          switch( ptDef->tModes.eTcc )
          {
            case TIMER_TCCMODE_PWMSING :
              uDivider = tCtl.tBasePtrs.ptTcc->PER.reg;
              fPercent = ( FLOAT )( ptParam->uValue / 1000.0 );
              uDivider *= fPercent;
              tCtl.tBasePtrs.ptTcc->CC[ ptParam->eCmpCapChan ].reg = uDivider;
              break;

            default :
              break;
          }
        }
        else
        {
          // compute the divider
          uDivider = 65535;
          fPercent = ( FLOAT )( ptParam->uValue / 1000.0 );
          uDivider *= fPercent;
          tCtl.tBasePtrs.ptTc->COUNT16.CC[ ptParam->eCmpCapChan ].reg = uDivider;
        }
        break;
        
      case TIMER_IOCTL_GETCAPTURE :
        break;
        
      case TIMER_IOCTL_SETCAPTURE :
        break;
        
      case TIMER_IOCTL_GETPRESCALEDIV :
       *(( PU32 )pvParam ) = awTimerPrescales[ ptDef->ePrescale ];
        break;

      default :
        // set the illegal timer error
        eError = TIMER_ERR_ILLACTION;
        break;
    }
  }
  else
  {
    // set the illegal timer error
    eError = TIMER_ERR_ILLTIMER;
  }
  
  return( eError );
}

/******************************************************************************
 * @function TCC0_Handler
 *
 * @brief TCC0 interrupt handler
 *
 * This function handles the interrupt for TCC0
 *
 *****************************************************************************/
void TCC0_Handler( void )
{
  // call the common interrupt handler
  CommonTccInterruptHandler( atPhyMaps[ TIMER_CHAN_0 ].eTimer, TCC0 );
}

#ifdef TCC1
/******************************************************************************
 * @function TCC1_Handler
 *
 * @brief TCC1 interrupt handler
 *
 * This function handles the interrupt for TCC1
 *
 *****************************************************************************/
void TCC1_Handler( void )
{
  // call the common interrupt handler
  CommonTccInterruptHandler( atPhyMaps[ TIMER_CHAN_1 ].eTimer, TCC1 );
}
#endif //TCC1

#ifdef TCC2
/******************************************************************************
 * @function TCC2_Handler
 *
 * @brief TCC2 interrupt handler
 *
 * This function handles the interrupt for TCC2
 *
 *****************************************************************************/
void TCC2_Handler( void )
{
  // call the common interrupt handler
  CommonTccInterruptHandler( atPhyMaps[ TIMER_CHAN_2 ].eTimer, TCC2 );
}
#endif // TCC2

#ifdef TCC3
/******************************************************************************
 * @function TC3_Handler
 *
 * @brief TCC0 interrupt handler
 *
 * This function handles the interrupt for TCC0
 *
 *****************************************************************************/
void TC3_Handler( void )
{
  // call the common interrupt handler
  CommonTcInterruptHandler( atPhyMaps[ TIMER_CHAN_3 ].eTimer, TC3 );
}
#endif // TCC3 

#ifdef TC4
/******************************************************************************
 * @function TC4_Handler
 *
 * @brief TC4 interrupt handler
 *
 * This function handles the interrupt for TC4
 *
 *****************************************************************************/
void TC4_Handler( void )
{
  // call the common interrupt handler
  CommonTcInterruptHandler( atPhyMaps[ TIMER_CHAN_4 ].eTimer, TC4 );
}
#endif // TC4

#ifdef TC5
/******************************************************************************
 * @function TC5_Handler
 *
 * @brief TC5 interrupt handler
 *
 * This function handles the interrupt for TC5
 *
 *****************************************************************************/
void TC5_Handler( void )
{
  // call the common interrupt handler
  CommonTcInterruptHandler( atPhyMaps[ TIMER_CHAN_5 ].eTimer, TC5 );
}
#endif //TC5

#ifdef TC6
/******************************************************************************
 * @function TC6_Handler
 *
 * @brief TC6 interrupt handler
 *
 * This function handles the interrupt for TC6
 *
 *****************************************************************************/
void TC6_Handler( void )
{
  // call the common interrupt handler
  CommonTcInterruptHandler( atPhyMaps[ TIMER_CHAN_6 ].eTimer, TC6 );
}
#endif  // TC6

#ifdef TC7
/******************************************************************************
 * @function TC7_Handler
 *
 * @brief TC7 interrupt handler
 *
 * This function handles the interrupt for TC7
 *
 *****************************************************************************/
void TC7_Handler( void )
{
  // call the common interrupt handler
  CommonTcInterruptHandler( atPhyMaps[ TIMER_CHAN_7 ].eTimer, TC7 );
}
#endif  // TC7

/******************************************************************************
 * @function CommonInterruptTccHandler
 *
 * @brief common interrupt handler for TCC
 *
 * This function handles all the interrupts for all  channels
 *
 * @param[in]   eTimer    timer enumeration
 *
 *****************************************************************************/
static void CommonTccInterruptHandler( TIMERENUM eTimer, Tcc* ptTcc )
{
  PTIMERDEF         ptDef;
  PVTIMERCALLBACK   pvCallback;
  TIMERCBEVENT      eEvent = TIMER_CBEVENT_NONE;
  TIMERCMPCAP       eCmpCap = TIMER_CMPCAP_MAX;
  U16               wCapValue = 0;
  TCC_INTFLAG_Type  tIntFlag;

  // get the interrupt status
  tIntFlag.reg = ptTcc->INTFLAG.reg;
  ptTcc->INTFLAG.reg = TCC_INTFLAG_MASK;

  // get pointer to the definition/control structures
  ptDef = ( PTIMERDEF )&atTimerDefs[ eTimer ];

  // get the callback
  pvCallback = ptDef->pvCallback;
  
  // check for overflow
  if ( tIntFlag.bit.OVF & ptTcc->INTENSET.bit.OVF )
  {
    eEvent = TIMER_CBEVENT_TMO;
  }
  else if ( tIntFlag.bit.MC0 & ptTcc->INTENSET.bit.MC0 )
  {
  }
  else if ( tIntFlag.bit.MC1 & ptTcc->INTENSET.bit.MC1 )
  {
  }
      
  // if callback not null and event
  if (( pvCallback != NULL ) && ( eEvent != TIMER_CBEVENT_NONE ))
  {
    // call the callback
    pvCallback( eEvent, eCmpCap, wCapValue );
  }
}

/******************************************************************************
 * @function CommonInterruptTcHandler
 *
 * @brief common interrupt handler for TC
 *
 * This function handles all the interrupts for all  channels
 *
 * @param[in]   eTimer    timer enumeration
 *
 *****************************************************************************/
static void CommonTcInterruptHandler( TIMERENUM eTimer, Tc* ptTc )
{
  PTIMERDEF         ptDef;
  PVTIMERCALLBACK   pvCallback;
  TIMERCBEVENT      eEvent = TIMER_CBEVENT_NONE;
  TIMERCMPCAP       eCmpCap = TIMER_CMPCAP_MAX;
  U16               wCapValue = 0;
  TC_INTFLAG_Type   tIntFlag;

  // get the interrupt flags
  tIntFlag.reg = ptTc->COUNT16.INTFLAG.reg;
  ptTc->COUNT16.INTFLAG.reg = TC_INTFLAG_MASK;

  // get pointer to the definition/control structures
  ptDef = ( PTIMERDEF )&atTimerDefs[ eTimer ];

  // get the callback
  pvCallback = ptDef->pvCallback;

  // check for capture mode
  if ( ptDef->tModes.eTc == TIMER_TCMODE_CAPTURE )
  {
    // reset the counter
    if ( ptDef->tFlags.bResetToZero )
    {
      ptTc->COUNT16.COUNT.reg = 0;
    }

    // set the event/channel/value
    eEvent = ( ptTc->COUNT16.EVCTRL.bit.TCINV ) ? TIMER_CBEVENT_CAPFE : TIMER_CBEVENT_CAPRE;
    wCapValue = ptTc->COUNT16.CC[ TIMER_CMPCAP_CHAN0 ].reg;

    // toggle the edge
    ptTc->COUNT16.EVCTRL.bit.TCINV ^= 1;
  }
  
  // check for overflow
  if ( tIntFlag.bit.OVF && ptTc->COUNT16.INTENSET.bit.OVF )
  {
    eEvent = TIMER_CBEVENT_TMO;
  }
  else if ( tIntFlag.bit.MC0 && ptTc->COUNT16.INTENSET.bit.MC0 )
  {
    // set the channel
    eCmpCap = TIMER_CMPCAP_CHAN0;

    // determine the mode
    switch( ptDef->tModes.eTc )
    {
      case TIMER_TCMODE_WAVNF :
      case TIMER_TCMODE_WAVMF :
      case TIMER_TCMODE_PWM :
        // set the event/channel/value
        eEvent = ( ptDef->eDirection == TIMER_DIRECTION_UP ) ? TIMER_CBEVENT_CMPUP : TIMER_CBEVENT_CMPDN;
        break;

      case TIMER_TCMODE_CAPPPW :
        // set the event/channel/value
        eEvent = ( ptTc->COUNT16.EVCTRL.bit.TCINV ) ? TIMER_CBEVENT_CAPFE : TIMER_CBEVENT_CAPRE;
        wCapValue = ptTc->COUNT16.CC[ TIMER_CMPCAP_CHAN0 ].reg;
        break;

      case TIMER_TCMODE_CAPPWP :
        // set the event/channel/value
        eEvent = ( ptTc->COUNT16.EVCTRL.bit.TCINV ) ? TIMER_CBEVENT_CAPFE : TIMER_CBEVENT_CAPRE;
        wCapValue = ptTc->COUNT16.CC[ TIMER_CMPCAP_CHAN1 ].reg;
        break;

      default :
        break;
    }
  }
  else if ( tIntFlag.bit.MC1 && ptTc->COUNT16.INTENSET.bit.MC1 )
  {
    // set the channel
    eCmpCap = TIMER_CMPCAP_CHAN1;

    // determine the mode
    switch( ptDef->tModes.eTc )
    {
      case TIMER_TCMODE_WAVNF :
      case TIMER_TCMODE_WAVMF :
      case TIMER_TCMODE_PWM :
        // set the event/channel/value
        eEvent = ( ptDef->eDirection == TIMER_DIRECTION_UP ) ? TIMER_CBEVENT_CMPUP : TIMER_CBEVENT_CMPDN;
        break;

      case TIMER_TCMODE_CAPPPW :
        // set the event/channel/value
        eEvent = ( ptTc->COUNT16.EVCTRL.bit.TCINV ) ? TIMER_CBEVENT_CAPRE : TIMER_CBEVENT_CAPFE;
        wCapValue = ptTc->COUNT16.CC[ TIMER_CMPCAP_CHAN1 ].reg;
        break;

      case TIMER_TCMODE_CAPPWP :
        // set the event/channel/value
        eEvent = ( ptTc->COUNT16.EVCTRL.bit.TCINV ) ? TIMER_CBEVENT_CAPRE : TIMER_CBEVENT_CAPFE;
        wCapValue = ptTc->COUNT16.CC[ TIMER_CMPCAP_CHAN0 ].reg;
        break;

      default :
        break;
    }
  }

  // if callback not null and event
  if (( pvCallback != NULL ) && ( eEvent != TIMER_CBEVENT_NONE ))
  {
    // call the callback
    pvCallback( eEvent, eCmpCap, wCapValue );
  }
}

/******************************************************************************
 * @function Congiure02PwmDual
 *
 * @brief TImer 0-2 PWM configuration
 *
 * This function configures a timer 0-2 for PWM operation
 *
 * @param[in]   ptDef     pointer to a DEF function
 *
 *****************************************************************************/
static void ConfigureTccPwmDual( PTIMERDEF ptDef )
{
  LCLCTL            tLclCtl;
  U32               uDivider, uPeriod, uTemp;
  U32               auChans[ TIMER_CMPCAP_MAX ];
  TIMERCMPCAP       eCmpCap;

  // get the channel
  GetTimerChannel( ptDef->eChannel, &tLclCtl );

  // compute the divider for the counter
  uDivider = Clock_GetMultiplexerFreq( tLclCtl.eMuxId ) / awTimerPrescales[ ptDef->ePrescale ];

  // if times in usec
  if ( ptDef->tFlags.bTimesInUsec )
  {
    // convwerrt the divider into Nsecs
    uDivider = 1000000000ul / uDivider;

    // now compute the reload values for period and channels
    uPeriod = (( ptDef->uPwmPeriod * 1000 ) / uDivider ) >> 1; 

    // for ecah compare/capture channel
    for ( eCmpCap = TIMER_CMPCAP_CHAN0; eCmpCap < TIMER_CMPCAP_MAX; eCmpCap++ )
    {
      // compute each channel reload
      uTemp = (( ptDef->auChanValues[ eCmpCap ] * 1000 ) / uDivider ) >> 1;
      auChans[ eCmpCap ] = uPeriod - uTemp;
    }
  }
  else
  {
    uPeriod = 0;
  }

  // reset TCC0
  tLclCtl.tBasePtrs.ptTcc->CTRLA.bit.SWRST = ON;
  while( tLclCtl.tBasePtrs.ptTcc->SYNCBUSY.bit.SWRST );
    
  // set up the TCC0 for dual slope operation
  tLclCtl.tBasePtrs.ptTcc->WAVE.reg = ptDef->tModes.eTcc;
  while(tLclCtl.tBasePtrs.ptTcc->SYNCBUSY.bit.WAVE );
    
  // set up the period
  tLclCtl.tBasePtrs.ptTcc->PER.reg = uPeriod;
  while( tLclCtl.tBasePtrs.ptTcc->SYNCBUSY.bit.PER );

  // set up CC0s
  for ( eCmpCap = TIMER_CMPCAP_CHAN0; eCmpCap < TIMER_CMPCAP_MAX; eCmpCap++ )
  {
    // is this channel active
    if ( ptDef->abChanOpsEnables[ eCmpCap ] == TRUE )
    {
      // set the compare value
      tLclCtl.tBasePtrs.ptTcc->CC[ eCmpCap ].reg = auChans[ eCmpCap ];
      while( tLclCtl.tBasePtrs.ptTcc->SYNCBUSY.bit.CC0 );

      // set the event
      switch( eCmpCap )
      {
        case TIMER_CMPCAP_CHAN0 :
          tLclCtl.tBasePtrs.ptTcc->EVCTRL.bit.MCEO0 = ON;
          break;

        case TIMER_CMPCAP_CHAN1 :
          tLclCtl.tBasePtrs.ptTcc->EVCTRL.bit.MCEO1 = ON;
          break;

        case TIMER_CMPCAP_CHAN2 :
          tLclCtl.tBasePtrs.ptTcc->EVCTRL.bit.MCEO2 = ON;
          break;

        case TIMER_CMPCAP_CHAN3 :
          tLclCtl.tBasePtrs.ptTcc->EVCTRL.bit.MCEO3 = ON;
          break;

        default :
          break;
      }
    }
  }

  // check for a valid callback
  if ( ptDef->pvCallback != NULL )
  {
    //  enable the overflow interrupt
//    tLclCtl.tBasePtrs.ptTcc->INTFLAG.bit.OVF = ON;
    tLclCtl.tBasePtrs.ptTcc->INTENSET.bit.OVF = ON;

    // enable the interrupt controller
    NVIC_EnableIRQ( TCC0_IRQn + ptDef->eChannel );
  }
    
  // enable it
  if ( ptDef->tFlags.bInitialOn )
  {
    tLclCtl.tBasePtrs.ptTcc->CTRLA.bit.ENABLE = ON;
    while( tLclCtl.tBasePtrs.ptTcc->SYNCBUSY.bit.ENABLE );

    // if not oneshot, trigger the timer
    if ( !ptDef->tFlags.bOneShot )
    {
      // send a retrigger
      tLclCtl.tBasePtrs.ptTcc->CTRLBSET.reg = TCC_CTRLBSET_CMD_RETRIGGER;
      while( tLclCtl.tBasePtrs.ptTcc->SYNCBUSY.bit.CTRLB );
    } 
  }
}

/******************************************************************************
 * @function ConfigureTccPwmSing
 *
 * @brief TImer 0-2 PWM configuration
 *
 * This function configures a timer 0-2 for PWM operation
 *
 * @param[in]   ptDef     pointer to a DEF function
 *
 *****************************************************************************/
static void ConfigureTccPwmSing( PTIMERDEF ptDef )
{
  LCLCTL            tLclCtl;
  U32               uDivider, uPeriod, uTemp;
  U32               auChans[ TIMER_CMPCAP_MAX ];
  TIMERCMPCAP       eCmpCap;
  FLOAT             fPercent;

  // get the channel
  GetTimerChannel( ptDef->eChannel, &tLclCtl );

  // compute the divider for the counter
  uDivider = Clock_GetMultiplexerFreq( tLclCtl.eMuxId ) / awTimerPrescales[ ptDef->ePrescale ];

  // if times in usec
  if ( ptDef->tFlags.bTimesInUsec )
  {
    // convwerrt the divider into Nsecs
    uDivider = 1000000000ul / uDivider;

    // now compute the reload values for period and channels
    uPeriod = ( ptDef->uPwmPeriod * 1000 ) / uDivider; 

    // for ecah compare/capture channel
    for ( eCmpCap = TIMER_CMPCAP_CHAN0; eCmpCap < TIMER_CMPCAP_MAX; eCmpCap++ )
    {
      // compute each channel reload
      uTemp = ( ptDef->auChanValues[ eCmpCap ] * 1000 ) / uDivider;
      auChans[ eCmpCap ] = uPeriod - uTemp;
    }
  }
  else
  {
    // compute the period
    uPeriod = ( uDivider / ptDef->uPwmPeriod ) / GetPrescaleValue( ptDef->ePrescale );
    
    // for ecah compare/capture channel
    for ( eCmpCap = TIMER_CMPCAP_CHAN0; eCmpCap < TIMER_CMPCAP_MAX; eCmpCap++ )
    {
      // compute each channel reload
      fPercent = ptDef->auChanValues[ eCmpCap ] / 1000.0;
      uTemp = uPeriod * fPercent;
      auChans[ eCmpCap ] = uTemp;
    }
  }

  // reset TCC0
  tLclCtl.tBasePtrs.ptTcc->CTRLA.bit.SWRST = ON;
  while( tLclCtl.tBasePtrs.ptTcc->SYNCBUSY.bit.SWRST );
    
  // set up the TCC0 for dual slope operation
  tLclCtl.tBasePtrs.ptTcc->WAVE.reg = ptDef->tModes.eTcc;
  while(tLclCtl.tBasePtrs.ptTcc->SYNCBUSY.bit.WAVE );
    
  // set up the period
  tLclCtl.tBasePtrs.ptTcc->PER.reg = uPeriod;
  while( tLclCtl.tBasePtrs.ptTcc->SYNCBUSY.bit.PER );

  // set up CC0s
  for ( eCmpCap = TIMER_CMPCAP_CHAN0; eCmpCap < TIMER_CMPCAP_MAX; eCmpCap++ )
  {
    // is this channel active
    if ( ptDef->abChanOpsEnables[ eCmpCap ] == TRUE )
    {
      // set the compare value
      tLclCtl.tBasePtrs.ptTcc->CC[ eCmpCap ].reg = auChans[ eCmpCap ];
      while( tLclCtl.tBasePtrs.ptTcc->SYNCBUSY.bit.CC0 );

      // set the event
      switch( eCmpCap )
      {
        case TIMER_CMPCAP_CHAN0 :
          tLclCtl.tBasePtrs.ptTcc->EVCTRL.bit.MCEO0 = OFF;
          break;

        case TIMER_CMPCAP_CHAN1 :
          tLclCtl.tBasePtrs.ptTcc->EVCTRL.bit.MCEO1 = OFF;
          break;

        case TIMER_CMPCAP_CHAN2 :
          tLclCtl.tBasePtrs.ptTcc->EVCTRL.bit.MCEO2 = OFF;
          break;

        case TIMER_CMPCAP_CHAN3 :
          tLclCtl.tBasePtrs.ptTcc->EVCTRL.bit.MCEO3 = OFF;
          break;

        default :
          break;
      }
    }
  }

  // check for a valid callback
  if ( ptDef->pvCallback != NULL )
  {
    //  enable the overflow interrupt
    // tLclCtl.tBasePtrs.ptTcc->INTFLAG.bit.OVF = ON;
    tLclCtl.tBasePtrs.ptTcc->INTENSET.bit.OVF = ON;

    // enable the interrupt controller
    NVIC_EnableIRQ( TCC0_IRQn + ptDef->eChannel );
  }
    
  // enable it
  if ( ptDef->tFlags.bInitialOn )
  {
    tLclCtl.tBasePtrs.ptTcc->CTRLA.bit.ENABLE = ON;
    while( tLclCtl.tBasePtrs.ptTcc->SYNCBUSY.bit.ENABLE );

    // if not oneshot, trigger the timer
    if ( !ptDef->tFlags.bOneShot )
    {
      // send a retrigger
      tLclCtl.tBasePtrs.ptTcc->CTRLBSET.reg = TCC_CTRLBSET_CMD_RETRIGGER;
      while( tLclCtl.tBasePtrs.ptTcc->SYNCBUSY.bit.CTRLB );
    } 
  }
}

/******************************************************************************
 * @function Congiure02Capture
 *
 * @brief TImer 0-2 simple capture configuration
 *
 * This function configures a timer 0-2 for simple capture operation
 *
 * @param[in]   ptDef     pointer to a DEF function
 *
 *****************************************************************************/
static void ConfigureTccCapture( PTIMERDEF ptDef )
{
  LCLCTL            tLclCtl;
  TCC_EVCTRL_Type   tEvctrl;
  TCC_CTRLA_Type    tCtrlA;
  TCC_INTENSET_Type tIntSet;

  // get the channel
  GetTimerChannel( ptDef->eChannel, &tLclCtl );

  // reset TCC0
  tLclCtl.tBasePtrs.ptTcc->CTRLA.bit.SWRST = ON;
  while( tLclCtl.tBasePtrs.ptTcc->SYNCBUSY.bit.SWRST );
  
  // set up the events
  tEvctrl.reg = 0;
  tEvctrl.bit.MCEI0 = ON;
  tEvctrl.bit.TCEI0 = ON;
  tLclCtl.tBasePtrs.ptTcc->EVCTRL.reg = tEvctrl.reg;

  // set up the interrupts
  tIntSet.bit.OVF = ptDef->tFlags.bOvfIrqEnable;
  tIntSet.bit.MC0 = ON;
  tLclCtl.tBasePtrs.ptTcc->INTENSET.reg = tIntSet.reg;

  // enable it
  tCtrlA.reg = ptDef->ePrescale;
  tCtrlA.bit.RUNSTDBY = ptDef->tFlags.bRunStandby;
  tCtrlA.bit.CPTEN0 = ON;
  tLclCtl.tBasePtrs.ptTcc->CTRLA.reg = tCtrlA.reg;

  if ( ptDef->tFlags.bInitialOn )
  {
    // set the control B
    tLclCtl.tBasePtrs.ptTcc->CTRLBSET.reg = TCC_CTRLBSET_CMD_RETRIGGER;

    // set the enable
    tLclCtl.tBasePtrs.ptTcc->CTRLA.bit.ENABLE = ON;
    while( tLclCtl.tBasePtrs.ptTcc->SYNCBUSY.bit.ENABLE );
  }
}

/******************************************************************************
 * @function CongiureTcWave
 *
 * @brief TImers 3-7 wave configuration
 *
 * This function configures a timer 3-7 for wave operation
 *
 * @param[in]   ptDef     pointer to a DEF function
 *
 *****************************************************************************/
static void ConfigureTcWave( PTIMERDEF ptDef )
{
  LCLCTL            tLclCtl;
  TC_CTRLA_Type     tCtrlA;
  TC_CTRLBSET_Type  tCtrlB;
  TC_CTRLC_Type     tCtrlC;
  TC_EVCTRL_Type    tEvent;
  U32               uDivider;
  
  // get the channel
  GetTimerChannel( ptDef->eChannel, &tLclCtl );
  
  // compute the divider for the counter
  uDivider = Clock_GetMultiplexerFreq( tLclCtl.eMuxId ) / awTimerPrescales[ ptDef->ePrescale ];
  uDivider /= ptDef->auChanValues[ TIMER_CMPCAP_CHAN0 ];
  
  // reset it/wait for sync
  tLclCtl.tBasePtrs.ptTc->COUNT16.CTRLA.bit.SWRST = ON;
  while(( tLclCtl.tBasePtrs.ptTc->COUNT16.STATUS.reg & TC_STATUS_SYNCBUSY ) != 0 );

  // store the top value in CC0
  tLclCtl.tBasePtrs.ptTc->COUNT16.CC[ 0 ].reg = uDivider;
  
  // set up CTRLA
  tCtrlA.reg = TC_CTRLA_MODE_COUNT16;
  tCtrlA.reg |= TC_CTRLA_WAVEGEN( ptDef->tModes.eTc );
  tCtrlA.reg |= TC_CTRLA_PRESCALER( ptDef->ePrescale );
  tCtrlA.reg |= ( ptDef->tFlags.bRunStandby ) ? TC_CTRLA_RUNSTDBY : 0;
  tLclCtl.tBasePtrs.ptTc->COUNT16.CTRLA.reg = tCtrlA.reg;
  while(( tLclCtl.tBasePtrs.ptTc->COUNT16.STATUS.reg & TC_STATUS_SYNCBUSY ) != 0 );
  
  // set up CTLRB
  tCtrlB.reg = ( ptDef->eDirection == TIMER_DIRECTION_DN ) ? TC_CTRLBSET_DIR : 0;
  tCtrlB.reg |= ( ptDef->tFlags.bOneShot ) ? TC_CTRLBSET_ONESHOT : 0;
  tLclCtl.tBasePtrs.ptTc->COUNT16.CTRLBSET.reg = tCtrlB.reg;
  while(( tLclCtl.tBasePtrs.ptTc->COUNT16.STATUS.reg & TC_STATUS_SYNCBUSY ) != 0 );
  
  // set up CTRLC
  tCtrlC.reg = ( ptDef->abInvertOutputs[ TIMER_CMPCAP_CHAN0 ] ) ? TC_CTRLC_INVEN0 : 0;
  tCtrlC.reg |= ( ptDef->abInvertOutputs[ TIMER_CMPCAP_CHAN1 ] ) ? TC_CTRLC_INVEN1 : 0;
  tLclCtl.tBasePtrs.ptTc->COUNT16.CTRLC.reg = tCtrlC.reg;
  while(( tLclCtl.tBasePtrs.ptTc->COUNT16.STATUS.reg & TC_STATUS_SYNCBUSY ) != 0 );
  
  // set up the event registers
  tEvent.reg = ( ptDef->tFlags.bOvfOutEnable ) ? TC_EVCTRL_OVFEO : 0;
  tEvent.reg |= ( ptDef->abChanOpsEnables[ TIMER_CMPCAP_CHAN0 ] ) ? TC_EVCTRL_MCEO0  : 0;
  tEvent.reg |= ( ptDef->abChanOpsEnables[ TIMER_CMPCAP_CHAN1 ] ) ? TC_EVCTRL_MCEO1  : 0;
  tLclCtl.tBasePtrs.ptTc->COUNT16.EVCTRL.reg = tEvent.reg;
  
  // determine if there is an inverrupt
  if ( ptDef->pvCallback != NULL )
  {
    // set the IRQ bits
    tLclCtl.tBasePtrs.ptTc->COUNT16.INTENSET.bit.OVF = ptDef->tFlags.bOvfIrqEnable;
    tLclCtl.tBasePtrs.ptTc->COUNT16.INTENSET.bit.MC0 = ptDef->abChanOpsEnables[ TIMER_CMPCAP_CHAN0 ];
    tLclCtl.tBasePtrs.ptTc->COUNT16.INTENSET.bit.MC1 = ptDef->abChanOpsEnables[ TIMER_CMPCAP_CHAN1 ];
    
    // check for a high priority
    if ( ptDef->tFlags.bHighPriority )
    {
      // set it to a higher priority
      NVIC_SetPriority( TCC0_IRQn + ptDef->eChannel, 1 );
    }

    // enable the interrupt
    NVIC_EnableIRQ( TCC0_IRQn + ptDef->eChannel );
  }
  
  // now enable the control register
  tLclCtl.tBasePtrs.ptTc->COUNT16.CTRLA.reg |= TC_CTRLA_ENABLE;
  while(( tLclCtl.tBasePtrs.ptTc->COUNT16.STATUS.reg & TC_STATUS_SYNCBUSY ) != 0 );
  
  // check for initial on
  if ( ptDef->tFlags.bInitialOn )
  {
    // force on
    tLclCtl.tBasePtrs.ptTc->COUNT16.CTRLBSET.reg = TC_CTRLBCLR_CMD( TC_CTRLBCLR_CMD_RETRIGGER_Val );
  }
}

/******************************************************************************
 * @function CongiureTcPWM
 *
 * @brief TImers 3-7 wave configuration
 *
 * This function configures a timer 3-7 for wave operation
 *
 * @param[in]   ptDef     pointer to a DEF function
 *
 *****************************************************************************/
static void ConfigureTcPwm( PTIMERDEF ptDef )
{
  LCLCTL            tLclCtl;
  TC_CTRLA_Type     tCtrlA;
  TC_CTRLBSET_Type  tCtrlB;
  TC_CTRLC_Type     tCtrlC;
  TC_EVCTRL_Type    tEvent;
  U32               uDivider;
  
  // get the channel
  GetTimerChannel( ptDef->eChannel, &tLclCtl );
  
  // compute the divider for the counter
  uDivider = Clock_GetMultiplexerFreq( tLclCtl.eMuxId ) / awTimerPrescales[ ptDef->ePrescale ];
  
  // reset it/wait for sync
  tLclCtl.tBasePtrs.ptTc->COUNT16.CTRLA.bit.SWRST = ON;
  while(( tLclCtl.tBasePtrs.ptTc->COUNT16.STATUS.reg & TC_STATUS_SYNCBUSY ) != 0 );

  // store the top value in CC0
  tLclCtl.tBasePtrs.ptTc->COUNT16.CC[ 0 ].reg = 0;
  tLclCtl.tBasePtrs.ptTc->COUNT16.CC[ 1 ].reg = 0;
  
  // set up CTRLA
  tCtrlA.reg = TC_CTRLA_MODE_COUNT16;
  tCtrlA.reg |= TC_CTRLA_WAVEGEN( ptDef->tModes.eTc );
  tCtrlA.reg |= TC_CTRLA_PRESCALER( ptDef->ePrescale );
  tCtrlA.reg |= ( ptDef->tFlags.bRunStandby ) ? TC_CTRLA_RUNSTDBY : 0;
  tLclCtl.tBasePtrs.ptTc->COUNT16.CTRLA.reg = tCtrlA.reg;
  while(( tLclCtl.tBasePtrs.ptTc->COUNT16.STATUS.reg & TC_STATUS_SYNCBUSY ) != 0 );
  
  // set up CTLRB
  tCtrlB.reg = ( ptDef->eDirection == TIMER_DIRECTION_DN ) ? TC_CTRLBSET_DIR : 0;
  tLclCtl.tBasePtrs.ptTc->COUNT16.CTRLBSET.reg = tCtrlB.reg;
  while(( tLclCtl.tBasePtrs.ptTc->COUNT16.STATUS.reg & TC_STATUS_SYNCBUSY ) != 0 );
  
  // set up CTRLC
  tCtrlC.reg = ( ptDef->abInvertOutputs[ TIMER_CMPCAP_CHAN0 ] ) ? TC_CTRLC_INVEN0 : 0;
  tCtrlC.reg |= ( ptDef->abInvertOutputs[ TIMER_CMPCAP_CHAN1 ] ) ? TC_CTRLC_INVEN1 : 0;
  tLclCtl.tBasePtrs.ptTc->COUNT16.CTRLC.reg = tCtrlC.reg;
  while(( tLclCtl.tBasePtrs.ptTc->COUNT16.STATUS.reg & TC_STATUS_SYNCBUSY ) != 0 );
  
  // set up the event registers
  tEvent.reg = ( ptDef->tFlags.bOvfOutEnable ) ? TC_EVCTRL_OVFEO : 0;
  tEvent.reg |= ( ptDef->abChanOpsEnables[ TIMER_CMPCAP_CHAN0 ] ) ? TC_EVCTRL_MCEO0  : 0;
  tEvent.reg |= ( ptDef->abChanOpsEnables[ TIMER_CMPCAP_CHAN1 ] ) ? TC_EVCTRL_MCEO1  : 0;
  tLclCtl.tBasePtrs.ptTc->COUNT16.EVCTRL.reg = tEvent.reg;
  
  // determine if there is an inverrupt
  if ( ptDef->pvCallback != NULL )
  {
    // set the IRQ bits
    tLclCtl.tBasePtrs.ptTc->COUNT16.INTENSET.bit.OVF = ptDef->tFlags.bOvfIrqEnable;
    tLclCtl.tBasePtrs.ptTc->COUNT16.INTENSET.bit.MC0 = ptDef->abChanOpsEnables[ TIMER_CMPCAP_CHAN0 ];
    tLclCtl.tBasePtrs.ptTc->COUNT16.INTENSET.bit.MC1 = ptDef->abChanOpsEnables[ TIMER_CMPCAP_CHAN1 ];
    
    // check for a high priority
    if ( ptDef->tFlags.bHighPriority )
    {
      // set it to a higher priority
      NVIC_SetPriority( TCC0_IRQn + ptDef->eChannel, 1 );
    }

    // enable the interrupt
    NVIC_EnableIRQ( TCC0_IRQn + ptDef->eChannel );
  }
  
  // now enable the control register
  tLclCtl.tBasePtrs.ptTc->COUNT16.CTRLA.reg |= TC_CTRLA_ENABLE;
  while(( tLclCtl.tBasePtrs.ptTc->COUNT16.STATUS.reg & TC_STATUS_SYNCBUSY ) != 0 );
  
  // check for initial on
  if ( ptDef->tFlags.bInitialOn )
  {
    // force on
    tLclCtl.tBasePtrs.ptTc->COUNT16.CTRLBSET.reg = TC_CTRLBCLR_CMD( TC_CTRLBCLR_CMD_RETRIGGER_Val );
  }
}

/******************************************************************************
 * @function CongiureTcCapture
 *
 * @brief TImers 3-7 capture configuration
 *
 * This function configures a timer 3-7 for capture operation
 *
 * @param[in]   ptDef     pointer to a DEF function
 *
 *****************************************************************************/
static void ConfigureTcCapture( PTIMERDEF ptDef )
{
  LCLCTL            tLclCtl;
  TC_CTRLA_Type     tCtrlA;
  TC_CTRLC_Type     tCtrlC;
  TC_EVCTRL_Type    tEvent;
  TC_INTENSET_Type  tIntenSet;
  
  // get the channel
  GetTimerChannel( ptDef->eChannel, &tLclCtl );
  
  // reset it/wait for sync
  tLclCtl.tBasePtrs.ptTc->COUNT16.CTRLA.bit.SWRST = ON;
  while(( tLclCtl.tBasePtrs.ptTc->COUNT16.STATUS.reg & TC_STATUS_SYNCBUSY ) != 0 );
  
  // clear the control C registes
  tCtrlC.reg = 0;
  tCtrlC.reg |= TC_CTRLC_CPTEN0;
  tLclCtl.tBasePtrs.ptTc->COUNT16.CTRLC.reg = tCtrlC.reg;
  while(( tLclCtl.tBasePtrs.ptTc->COUNT16.STATUS.reg & TC_STATUS_SYNCBUSY ) != 0 );

  // set up the event registers
  tEvent.reg = ( ptDef->tFlags.bOvfOutEnable ) ? TC_EVCTRL_OVFEO : 0;
  tEvent.reg |= TC_EVCTRL_TCEI;
  tEvent.reg |= ( ptDef->tFlags.bInvertEdge ) ? TC_EVCTRL_TCINV : 0;
  tLclCtl.tBasePtrs.ptTc->COUNT16.EVCTRL.reg = tEvent.reg;
  
  // determine if there is an inverrupt
  if ( ptDef->pvCallback != NULL )
  {
    // set the IRQ bits
    tIntenSet.bit.OVF = ptDef->tFlags.bOvfIrqEnable;
    tIntenSet.bit.MC0 = ON;
    tLclCtl.tBasePtrs.ptTc->COUNT16.INTENSET.reg = tIntenSet.reg;
    
    // clear all pending interrupts
    tLclCtl.tBasePtrs.ptTc->COUNT16.INTFLAG.reg = TC_INTFLAG_MASK;

    // enable the interrupt
    NVIC_EnableIRQ( TCC0_IRQn + ptDef->eChannel );
  }
  
  // set up CTRLA
  tCtrlA.reg = TC_CTRLA_MODE_COUNT16;
  tCtrlA.reg |= TC_CTRLA_PRESCALER( ptDef->ePrescale );
  tCtrlA.reg |= ( ptDef->tFlags.bRunStandby ) ? TC_CTRLA_RUNSTDBY : 0;
  tCtrlA.reg |= ( ptDef->tFlags.bInitialOn ) ? TC_CTRLA_ENABLE : 0;
  tLclCtl.tBasePtrs.ptTc->COUNT16.CTRLA.reg = tCtrlA.reg;
  while(( tLclCtl.tBasePtrs.ptTc->COUNT16.STATUS.reg & TC_STATUS_SYNCBUSY ) != 0 );
}

/******************************************************************************
 * @function CongiureTcCapturePXX
 *
 * @brief TImers 3-7 capture configuration
 *
 * This function configures a timer 3-7 for capture operation
 *
 * @param[in]   ptDef     pointer to a DEF function
 *
 *****************************************************************************/
static void ConfigureTcCapturePXX( PTIMERDEF ptDef )
{
  LCLCTL            tLclCtl;
  TC_CTRLA_Type     tCtrlA;
  TC_CTRLC_Type     tCtrlC;
  TC_EVCTRL_Type    tEvent;
  TC_INTENSET_Type  tIntenSet;
  
  // get the channel
  GetTimerChannel( ptDef->eChannel, &tLclCtl );
  
  // reset it/wait for sync
  tLclCtl.tBasePtrs.ptTc->COUNT16.CTRLA.bit.SWRST = ON;
  while(( tLclCtl.tBasePtrs.ptTc->COUNT16.STATUS.reg & TC_STATUS_SYNCBUSY ) != 0 );
  
  // clear the control C registes
  tCtrlC.reg = 0;
  tCtrlC.reg |= ( ptDef->abChanIrqEnables[ TIMER_CMPCAP_CHAN0 ] ) ? TC_CTRLC_CPTEN0 : 0;
  tCtrlC.reg |= ( ptDef->abChanIrqEnables[ TIMER_CMPCAP_CHAN1 ] ) ? TC_CTRLC_CPTEN1 : 0;
  tLclCtl.tBasePtrs.ptTc->COUNT16.CTRLC.reg = tCtrlC.reg;
  while(( tLclCtl.tBasePtrs.ptTc->COUNT16.STATUS.reg & TC_STATUS_SYNCBUSY ) != 0 );

  // set up the event registers
  tEvent.reg = ( ptDef->tFlags.bOvfOutEnable ) ? TC_EVCTRL_OVFEO : 0;
  tEvent.reg |= TC_EVCTRL_TCEI;
  tEvent.reg |= ( ptDef->tFlags.bInvertEdge ) ? TC_EVCTRL_TCINV : 0;
  tEvent.reg |= TC_EVCTRL_EVACT( ptDef->eEventAction );
  tLclCtl.tBasePtrs.ptTc->COUNT16.EVCTRL.reg = tEvent.reg;
  
  // determine if there is an inverrupt
  if ( ptDef->pvCallback != NULL )
  {
    // set the IRQ bits
    tIntenSet.bit.OVF = ptDef->tFlags.bOvfIrqEnable;
    tIntenSet.bit.MC0 = ptDef->abChanIrqEnables[ TIMER_CMPCAP_CHAN0 ];
    tIntenSet.bit.MC1 = ptDef->abChanIrqEnables[ TIMER_CMPCAP_CHAN1 ];
    tLclCtl.tBasePtrs.ptTc->COUNT16.INTENSET.reg = tIntenSet.reg;
    
    // clear all pending interrupts
    tLclCtl.tBasePtrs.ptTc->COUNT16.INTFLAG.reg = TC_INTFLAG_MASK;

    // enable the interrupt
    NVIC_EnableIRQ( TCC0_IRQn + ptDef->eChannel );
  }
  
  // set up CTRLA
  tCtrlA.reg = TC_CTRLA_MODE_COUNT16;
  tCtrlA.reg |= TC_CTRLA_PRESCALER( ptDef->ePrescale );
  tCtrlA.reg |= ( ptDef->tFlags.bRunStandby ) ? TC_CTRLA_RUNSTDBY : 0;
  tCtrlA.reg |= ( ptDef->tFlags.bInitialOn ) ? TC_CTRLA_ENABLE : 0;
  tLclCtl.tBasePtrs.ptTc->COUNT16.CTRLA.reg = tCtrlA.reg;
  while(( tLclCtl.tBasePtrs.ptTc->COUNT16.STATUS.reg & TC_STATUS_SYNCBUSY ) != 0 );
}

/******************************************************************************
 * @function GetTimer37channel
 *
 * @brief gets a pointer to the timer channel
 *
 * This function returns a pointer to the control structuer for a given channel
 *
 * @param[in]   eChannel    desired channel
 * @param[io]   ptLclCtl    pointer for the storage of the lcocal control
 *
 *****************************************************************************/
static void GetTimerChannel( TIMERCHAN eChannel, PLCLCTL ptLclCtl )
{
  // determine the channel
  switch( eChannel )
  {
    case TIMER_CHAN_0 :
      ptLclCtl->tBasePtrs.ptTcc = TCC0;
      ptLclCtl->eMuxId = CLOCK_MUXID_TCC01;
      break;

    #ifdef TCC1
    case TIMER_CHAN_1 :
      ptLclCtl->tBasePtrs.ptTcc = TCC1;
      ptLclCtl->eMuxId = CLOCK_MUXID_TCC01;
      break;
    #endif  // TCC2

    #ifdef TCC2
    case TIMER_CHAN_2 :
      ptLclCtl->tBasePtrs.ptTcc = TCC2;
      ptLclCtl->eMuxId = CLOCK_MUXID_TCC2TC3;
      break;
    #endif  // TCC2

    #ifdef TC3
    case TIMER_CHAN_3 :
      ptLclCtl->tBasePtrs.ptTc = TC3;
      ptLclCtl->eMuxId = CLOCK_MUXID_TCC2TC3;
      break;
    #endif  // TC3

    #ifdef TC4
    case TIMER_CHAN_4 :
      ptLclCtl->tBasePtrs.ptTc = TC4;
      ptLclCtl->eMuxId = CLOCK_MUXID_TC45;
      break;
    #endif  // TC4

    #ifdef TC5
    case TIMER_CHAN_5 :
      ptLclCtl->tBasePtrs.ptTc = TC5;
      ptLclCtl->eMuxId = CLOCK_MUXID_TC45;
      break;
    #endif  // TC5

    #ifdef TC6
    case TIMER_CHAN_6 :
      ptLclCtl->tBasePtrs.ptTc = TC6;
      ptLclCtl->eMuxId = CLOCK_MUXID_TC67;
      break;
    #endif  // TC6

    #ifdef TC7
    case TIMER_CHAN_7 :
      ptLclCtl->tBasePtrs.ptTc = TC7;
      ptLclCtl->eMuxId = CLOCK_MUXID_TC67;
      break;
    #endif  // TC7

    default :
      ptLclCtl->tBasePtrs.ptTc = NULL;
      ptLclCtl->eMuxId = CLOCK_MUXID_EOT;
      break;
  }
}

static U32 GetPrescaleValue( TIMERPRESCALE ePrescale )
{
  U32 uPrescale;

  switch ( ePrescale )
  {
    case TIMER_PRESCALE_1 :
      uPrescale = 1;
      break;

    case TIMER_PRESCALE_2 :
      uPrescale = 2;
      break;

    case TIMER_PRESCALE_4 :
      uPrescale = 4;
      break;

    case TIMER_PRESCALE_8 :
      uPrescale = 8;
      break;

    case TIMER_PRESCALE_16 :
      uPrescale = 16;
      break;

    case TIMER_PRESCALE_64 :
      uPrescale = 64;
      break;

    case TIMER_PRESCALE_256 :
      uPrescale = 256;
      break;

    case TIMER_PRESCALE_1024 :
      uPrescale = 1024;
      break;

    default :
      uPrescale = 1;
      break;

  }

  // return the value
  return( uPrescale );
}

/**@} EOF Timers.c */
