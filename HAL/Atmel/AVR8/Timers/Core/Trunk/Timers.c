/******************************************************************************
 * @file Timers.c
 *
 * @brief timers implementation
 *
 * This file provides the implementation for the timers module
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
 * $Log: $
 * 
 *
 * \addtogroup Timers
 * @{
 *****************************************************************************/

// local includes -------------------------------------------------------------
#include "Timers/Timers.h"

// libary includes -------------------------------------------------------------
#include "Clock/Clock.h"

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------
/// define the timer control for 8 bit timers
typedef struct _TIMCTL8
{
  PU8   pnTCNT;                         ///< count register
  PU8   apnOCR[ MAX_COMPCHANS_8BIT ];   ///< output compare registers
} TIMCTL8, *PTIMCTL8;
#define TIMCTL8_SIZE    sizeof( TIMCTL8 )

/// define the timer control for 16 bit timers
typedef struct _TIMCTL16
{
  PU16  pwTCNT;                         ///< count register
  PU16  pwICR;                          ///< input capture register
  PU16  apwOCR[ MAX_COMPCHANS_16BIT ];  ///< output compare registers
} TIMCTL16, *PTIMCTL16;
#define TIMCTL16_SIZE   sizeof( TIMCTL16 )

/// define the timer definition 
typedef struct _TIMERCTL
{
  PU8         pnTCCRA;        ///< control register A
  PU8         pnTCCRB;        ///< control register B
  PU8         pnTIMSK;        ///< interrupt mask register
  PU8         pnTIFR;         ///< interrupt flag register
  U8          nPrescaleMask;  ///< prescale mask
  U8          nIntMask;       ///< interrupt mask
  U16         wPrescale;      ///< prescale select
  union
  {
    TIMCTL8   tTim8;          /// 8 bit timer specific regs
    TIMCTL16  tTim16;         /// 16 bit timer specific regs
  } tRegs;
} TIMERCTL, *PTIMERCTL;
#define TIMERCTL_SIZE   sizeof( TIMERCTL )

/// define the physical to control map
typedef struct _PHYMAP
{
  TIMERSENUM eTimer;             ///< enumerated device
} PHYMAP, *PPHYMAP;
#define PHYMAP_SIZE   sizseof( PHYMAP )

// global parameter declarations ----------------------------------------------

// local parameter declarations -----------------------------------------------
static  TIMERCTL  atTimerCtls[ TIMER_SEL_MAX ];
static  PHYMAP    atPhyMaps[ TIMER_SEL_MAX ];

// local function prototypes --------------------------------------------------
static  void    Setup8BitTimer( TIMERSENUM eTimer, PTIMERDEF ptDef );
static  void    Setup16BitTimer( TIMERSENUM eTimer, PTIMERDEF ptDef );
static  void    CommonCmpIrq( TIMERSENUM eTimer );
static  void    CommonCapIrq( TIMERSENUM eTimer );
static  void    CommonOvfIrq( TIMERSENUM eTimer );

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
  TIMERSENUM  eTimer;
  
  // for each timer in the config table
  for ( eTimer = 0; eTimer < TIMERS_ENUM_MAX; eTimer++ )
  {
    // determine the size
    switch( PGM_RDBYTE( atTimerDefs[ eTimer ].eSize ))
    {
      case TIMER_SIZE_8BIT :
        Setup8BitTimer( eTimer, ( PTIMERDEF )&atTimerDefs[ eTimer ] );
        break;
        
      case TIMER_SIZE_16BIT :
        Setup16BitTimer( eTimer, ( PTIMERDEF )&atTimerDefs[ eTimer ] );
        break;
        
      default :
        break;
    }
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
TIMERSERR Timers_Ioctl( TIMERSENUM eTimerSel, TIMERIOCTL eAction, PVOID pvParam )
{
  TIMERSERR     eError = TIMERS_ERR_NONE;
  PTIMERDEF     ptDef;
  PTIMERCTL     ptCtl;
  TIMERSIZE     eSize;
  U16           wTemp;
  FLOAT         fPercent;
  PTIMERPWMSET  ptPwm;
  U8            nPrescale;
  U32           uDivider;
  
  // valid timer
  if ( eTimerSel < TIMERS_ENUM_MAX )
  {
    // get the pointer to the contro;
    ptDef = ( PTIMERDEF )&atTimerDefs[ eTimerSel ];
    ptCtl = &atTimerCtls[ eTimerSel ];
    
    // get the size
    eSize = ( TIMERSIZE )PGM_RDBYTE( ptDef->eSize );

    // determine the action
    switch( eAction )
    {
      case TIMER_IOCTL_CHANGEDEF :
        // change the def 
        ptDef = ( PTIMERDEF )pvParam;
        
        // get the size
        switch( eSize )
        {
          case TIMER_SIZE_8BIT :
            Setup8BitTimer( eTimerSel, ((PTIMERDEF)pvParam ));
            break;
            
          case TIMER_SIZE_16BIT :
            Setup16BitTimer( eTimerSel, ((PTIMERDEF)pvParam ));
            break;
            
          default :
            break;
        }
        break;
        
      case TIMER_IOCTL_STOPSTART :
        // get the prescale
        nPrescale = PGM_RDBYTE( ptDef->nPrescale );
        
        // determine action
        if ( PARAMBOOL( pvParam ))
        {
          // swith on the size
          switch( eSize )
          {
            case TIMER_SIZE_8BIT :
            *( ptCtl->tRegs.tTim8.pnTCNT ) = 0;
            break;
          
            case TIMER_SIZE_16BIT :
            *( ptCtl->tRegs.tTim16.pwTCNT ) = 0;
            break;
          
            default :
            break;
          }
          
          // clear pending/enable interrupts/timer
          *( ptCtl->pnTIFR ) = ptCtl->nIntMask;
          *( ptCtl->pnTIMSK ) = ptCtl->nIntMask;
          *( ptCtl->pnTCCRB ) |= nPrescale;
        }
        else
        {
          // disable interrupts/timer
          *( ptCtl->pnTIMSK ) = 0;
          *( ptCtl->pnTCCRB ) &= ~ptCtl->nPrescaleMask;
        }
        break;
        
      case TIMER_IOCTL_GETCOUNT :
        // swith on the size
        switch( eSize )
        {
          case TIMER_SIZE_8BIT :
            PARAMU32( pvParam )  = *( ptCtl->tRegs.tTim8.pnTCNT );
            break;
            
          case TIMER_SIZE_16BIT :
            PARAMU32( pvParam )  = *( ptCtl->tRegs.tTim16.pwTCNT );
            break;
            
          default :
            // set error
            eError = TIMERS_ERR_ILLTIMESIZE;
            break;
        }
        break;
        
      case TIMER_IOCTL_SETCOUNT :
        // swith on the size
        switch( eSize )
        {
          case TIMER_SIZE_8BIT :
            *( ptCtl->tRegs.tTim8.pnTCNT ) = PARAMU8( pvParam );
            break;
            
          case TIMER_SIZE_16BIT :
            *( ptCtl->tRegs.tTim16.pwTCNT ) = PARAMU16( pvParam );
            break;
            
          default :
            // set error
            eError = TIMERS_ERR_ILLTIMESIZE;
            break;
        }
        break;
        
      case TIMER_IOCTL_GETPERIOD :
        // swith on the size
        switch( eSize )
        {
          case TIMER_SIZE_8BIT :
            break;
            
          case TIMER_SIZE_16BIT :
            PARAMU32( pvParam )  = *( ptCtl->tRegs.tTim16.pwICR );
            break;
            
          default :
            // set error
            eError = TIMERS_ERR_ILLTIMESIZE;
            break;
        }
        break;
        
      case TIMER_IOCTL_SETPERIOD :
        // switch on the size
        switch( eSize )
        {
          case TIMER_SIZE_8BIT :
            break;
            
          case TIMER_SIZE_16BIT :
            // calculate the reload for the rate
            uDivider =  Clock_GetFreq( ) / ptCtl->wPrescale;
            uDivider /= PARAMU32( pvParam );
            *( ptCtl->tRegs.tTim16.pwICR ) = uDivider;
            break;
            
          default :
            // set error
            eError = TIMERS_ERR_ILLTIMESIZE;
            break;
        }        
        break;
        
      case TIMER_IOCTL_GETCOMPAREVAL :
        break;
        
      case TIMER_IOCTL_SETCOMPAREVAL :
        ptPwm = ( PTIMERPWMSET )pvParam;
        
        // swith on the size
        switch( eSize )
        {
          case TIMER_SIZE_8BIT :
            break;
          
          case TIMER_SIZE_16BIT :
            // calculate the new compare
            uDivider =  Clock_GetFreq( ) / ptCtl->wPrescale;
            uDivider /= ptPwm->wValue;
            *( ptCtl->tRegs.tTim16.apwOCR[ ptPwm->eChan ] ) = uDivider;
            break;
          
          default :
            // set error
            eError = TIMERS_ERR_ILLTIMESIZE;
            break;
        }
        break;
        
      case TIMER_IOCTL_SETCOMPAREPCT :
        ptPwm = ( PTIMERPWMSET )pvParam;
        
        // swith on the size
        switch( eSize )
        {
          case TIMER_SIZE_8BIT :
            break;
            
          case TIMER_SIZE_16BIT :
            // calculate the new compare
            wTemp = *( ptCtl->tRegs.tTim16.pwICR );
            fPercent = ptPwm->wValue / 1000.0;
            wTemp *= fPercent;
            *( ptCtl->tRegs.tTim16.apwOCR[ ptPwm->eChan ] ) = wTemp;
            break;
            
          default :
            // set error
            eError = TIMERS_ERR_ILLTIMESIZE;
            break;
        }
        break;
        
      case TIMER_IOCTL_GETCAPTURE :
        break;
        
      case TIMER_IOCTL_SETCAPTURE :
        break;

      case TIMER_IOCTL_GETPRESCALE :
        nPrescale = PGM_RDWORD( ptDef->nPrescale );

        // swith on the size
        switch( eSize )
        {
          case TIMER_SIZE_8BIT :
            // determine the timer
            switch( PGM_RDBYTE( ptDef->eSelect ))
            {
              case TIMER_SEL_0 :
                *((U32*)pvParam ) = PGM_RDWORD( awPreScales80[ nPrescale ] );
                break;
    
              case TIMER_SEL_2 :
                *((U32*)pvParam ) = PGM_RDWORD( awPreScales82[ nPrescale ] );
                break;
    
              default :
                *((U32*)pvParam ) = 1;
                break;
            }
            break;
          
          case TIMER_SIZE_16BIT :
            *((U32*)pvParam ) = PGM_RDWORD( awPreScales16[ nPrescale ] );
            break;
          
          default :
            // set error
            eError = TIMERS_ERR_ILLTIMESIZE;
            break;
        }
        break;
        
      default :
        // set the illegal timer error
        eError = TIMERS_ERR_ILLACTION;
        break;
    }
  }
  else
  {
    // set the illegal timer error
    eError = TIMERS_ERR_ILLTIMER;
  }
  
  return( eError );
}

#if defined( TIMER0_COMPA_vect )
/******************************************************************************
 * @function TIMER0_COMPA_vect
 *
 * @brief timer 0 compare A IRQ vector
 *
 * This function provides the timer 0 compare A IRQ vector
 *
 *****************************************************************************/
ISR( TIMER0_COMPA_vect )
{
  // call the common interrupt
  CommonCmpIrq( atPhyMaps[ 0 ].eTimer );
}
#endif

#if defined( TIMER0_COMPB_vect )
/******************************************************************************
 * @function TIMER0_COMPB_vect
 *
 * @brief timer 0 compare B IRQ vector
 *
 * This function provides the timer 0 compare B IRQ vector
 *
 *****************************************************************************/
ISR( TIMER0_COMPB_vect )
{
  // call the common interrupt
  CommonCmpIrq( atPhyMaps[ 0 ].eTimer );
}
#endif

#if defined( TIMER0_OVF_vect )
/******************************************************************************
 * @function TIMER0_OVF_vect
 *
 * @brief timer 0 overflow vector
 *
 * This function provides the timer 0 overflow vector
 *
 *****************************************************************************/
ISR( TIMER0_OVF_vect )
{
  // call the common interrupt
  CommonOvfIrq( atPhyMaps[ 0 ].eTimer );
}
#endif


#if defined( TIM0_COMPA_vect )
/******************************************************************************
 * @function TIM0_COMPA_vect
 *
 * @brief timer 0 compare A IRQ vector
 *
 * This function provides the timer 0 compare A IRQ vector
 *
 *****************************************************************************/
ISR( TIM0_COMPA_vect )
{
  // call the common interrupt
  CommonCmpIrq( atPhyMaps[ 0 ].eTimer );
}
#endif

#if defined( TIM0_COMPB_vect )
/******************************************************************************
 * @function TIM0_COMPB_vect
 *
 * @brief timer 0 compare B IRQ vector
 *
 * This function provides the timer 0 compare B IRQ vector
 *
 *****************************************************************************/
ISR( TIM0_COMPB_vect )
{
  // call the common interrupt
  CommonCmpIrq( atPhyMaps[ 0 ].eTimer );
}
#endif

#if defined( TIM0_OVF_vect )
/******************************************************************************
 * @function TIM0_OVF_vect
 *
 * @brief timer 0 overflow vector
 *
 * This function provides the timer 0 overflow vector
 *
 *****************************************************************************/
ISR( TIM0_OVF_vect )
{
  // call the common interrupt
  CommonOvfIrq( atPhyMaps[ 0 ].eTimer );
}
#endif

#if defined( TIMER1_CAPT_vect )
/******************************************************************************
 * @function TIMER1_CAPT_vect
 *
 * @brief timer 0 input capture IRQ vector
 *
 * This function provides the timer 1 input capture  IRQ vector
 *
 *****************************************************************************/
ISR( TIMER1_CAPT_vect )
{
  // call the common interrupt
  CommonCapIrq( atPhyMaps[ 1 ].eTimer );
}
#endif

#if defined( TIMER1_COMPA_vect )
/******************************************************************************
 * @function TIMER1_COMPA_vect
 *
 * @brief timer 1 compare A IRQ vector
 *
 * This function provides the timer 1 compare A IRQ vector
 *
 *****************************************************************************/
ISR( TIMER1_COMPA_vect )
{
  // call the common interrupt
  CommonCmpIrq( atPhyMaps[ 1 ].eTimer );
}
#endif

#if defined( TIMER1_COMPB_vect )
/******************************************************************************
 * @function TIMER1_COMPB_vect
 *
 * @brief timer 1 compare B IRQ vector
 *
 * This function provides the timer 1 compare B IRQ vector
 *
 *****************************************************************************/
ISR( TIMER1_COMPB_vect )
{
  // call the common interrupt
  CommonCmpIrq( atPhyMaps[ 1 ].eTimer );
}
#endif

#if defined( TIMER1_COMPC_vect )
/******************************************************************************
 * @function TIMER1_COMPC_vect
 *
 * @brief timer 1 compare C IRQ vector
 *
 * This function provides the timer 1 compare C IRQ vector
 *
 *****************************************************************************/
ISR( TIMER1_COMPC_vect )
{
  // call the common interrupt
  CommonCmpIrq( atPhyMaps[ 1 ].eTimer );
}
#endif

#if defined( TIMER1_OVF_vect )
/******************************************************************************
 * @function TIMER1_OVF_vect
 *
 * @brief timer 1 overflow IRQ vector
 *
 * This function provides the timer 1 overflow  IRQ vector
 *
 *****************************************************************************/
ISR( TIMER1_OVF_vect )
{
  // call the common interrupt
  CommonOvfIrq( atPhyMaps[ 1 ].eTimer );
}
#endif

#if defined( TIMER2_COMPA_vect )
/******************************************************************************
 * @function TIMER2_COMPA_vect
 *
 * @brief timer 22 compare A IRQ vector
 *
 * This function provides the timer 0 compare A IRQ vector
 *
 *****************************************************************************/
ISR( TIMER2_COMPA_vect )
{
  // call the common interrupt
  CommonCmpIrq( atPhyMaps[ 2 ].eTimer );
}
#endif

#if defined( TIMER2_COMPB_vect )
/******************************************************************************
 * @function TIMER2_COMPB_vect
 *
 * @brief timer 2 compare B IRQ vector
 *
 * This function provides the timer 0 compare B IRQ vector
 *
 *****************************************************************************/
ISR( TIMER2_COMPB_vect )
{
  // call the common interrupt
  CommonCmpIrq( atPhyMaps[ 2 ].eTimer );
}
#endif

#if defined( TIMER3_CAPT_vect )
/******************************************************************************
 * @function TIMER3_CAPT_vect
 *
 * @brief timer 3 input capture IRQ vector
 *
 * This function provides the timer 3 input capture  IRQ vector
 *
 *****************************************************************************/
ISR( TIMER3_CAPT_vect )
{
  // call the common interrupt
  CommonCapIrq( atPhyMaps[ 3 ].eTimer );
}
#endif

#if defined( TIMER3_COMPA_vect )
/******************************************************************************
 * @function TIMER3_COMPA_vect
 *
 * @brief timer 3 compare A IRQ vector
 *
 * This function provides the timer 3 compare A IRQ vector
 *
 *****************************************************************************/
ISR( TIMER3_COMPA_vect )
{
  // call the common interrupt
  CommonCmpIrq( atPhyMaps[ 3 ].eTimer );
}
#endif

#if defined( TIMER3_COMPB_vect )
/******************************************************************************
 * @function TIMER3_COMPB_vect
 *
 * @brief timer 3 compare B IRQ vector
 *
 * This function provides the timer 3 compare B IRQ vector
 *
 *****************************************************************************/
ISR( TIMER3_COMPB_vect )
{
  // call the common interrupt
  CommonCmpIrq( atPhyMaps[ 3 ].eTimer );
}
#endif

#if defined( TIMER3_COMPC_vect )
/******************************************************************************
 * @function TIMER3_COMPC_vect
 *
 * @brief timer 3 compare C IRQ vector
 *
 * This function provides the timer 3 compare C IRQ vector
 *
 *****************************************************************************/
ISR( TIMER3_COMPC_vect )
{
  // call the common interrupt
  CommonCmpIrq( atPhyMaps[ 3 ].eTimer );
}
#endif

#if defined( TIMER3_OVF_vect )
/******************************************************************************
 * @function TIMER3_OVF_vect
 *
 * @brief timer 3 overflow IRQ vector
 *
 * This function provides the timer 3 overflow  IRQ vector
 *
 *****************************************************************************/
ISR( TIMER3_OVF_vect )
{
  // call the common interrupt
  CommonOvfIrq( atPhyMaps[ 3 ].eTimer );
}
#endif

/******************************************************************************
 * @function Setup16BitTimer
 *
 * @brief 16 biit timer initialization
 *
 * This function configures a 16 bit timer to its selected mode of operation
 *
 * @param[in]   eTimer    timer selection
 *
 *****************************************************************************/
static void Setup8BitTimer( TIMERSENUM eTimer, PTIMERDEF ptDef )
{
  U32           uDivider;
  PTIMERCTL     ptCtl;
  U8            nPrescale;
  
  // set the pointers
  ptCtl = &atTimerCtls[ eTimer ];

  // get the prescale
  nPrescale = PGM_RDBYTE( ptDef->nPrescale );

  // set the registers
  switch( PGM_RDBYTE( ptDef->eSelect ))
  {
    case TIMER_SEL_0 :
      #ifdef TCCR0A
        ptCtl->pnTCCRA = ( PU8 )&TCCR0A;
        ptCtl->pnTCCRB = ( PU8 )&TCCR0B;
        ptCtl->pnTIMSK = ( PU8 )&TIMSK0;
        ptCtl->pnTIFR = ( PU8 )&TIFR0;
        ptCtl->tRegs.tTim8.pnTCNT = ( PU8 )&TCNT0;
        ptCtl->tRegs.tTim8.apnOCR[ 0 ] = ( PU8 )&OCR0A;
        ptCtl->tRegs.tTim8.apnOCR[ 1 ] = ( PU8 )&OCR0B;
        ptCtl->nPrescaleMask = TIMER_PRESCALE80_MASK;
        atPhyMaps[ 0 ].eTimer = eTimer;
        ptCtl->wPrescale = PGM_RDWORD( awPreScales80[ nPrescale ] );
      #endif
      break;
      
    case TIMER_SEL_2 :
      #ifdef  TCCR2A
        ptCtl->pnTCCRA = ( PU8 )&TCCR2A;
        ptCtl->pnTCCRB = ( PU8 )&TCCR2B;
        ptCtl->pnTIMSK = ( PU8 )&TIMSK2;
        ptCtl->pnTIFR = ( PU8 )&TIFR2;
        ptCtl->tRegs.tTim8.pnTCNT = ( PU8 )&TCNT2;
        ptCtl->tRegs.tTim8.apnOCR[ 0 ] = ( PU8 )&OCR2A;
        ptCtl->tRegs.tTim8.apnOCR[ 1 ] = ( PU8 )&OCR2B;
        ptCtl->nPrescaleMask = TIMER_PRESCALE82_MASK;
        atPhyMaps[ 2 ].eTimer = eTimer;
        ptCtl->wPrescale = PGM_RDWORD( awPreScales82[ nPrescale ] );
      #endif
      break;
      
    default :
      ptCtl->wPrescale = 1;
      break;
  }
  
  // configure the modes
  switch( PGM_RDBYTE( ptDef->tControl.tTim8.eMode ))
  {
    case TIMER_MODE8_NORM :
      break;
  
    case TIMER_MODE8_PWMPCM :
      break;
      
    case TIMER_MODE8_CTC :
      // calculate the reload for the rate
      uDivider = Clock_GetFreq( ) / ptCtl->wPrescale;
      uDivider /= PGM_RDDWRD( ptDef->uRateHz );
  
      // enable CTC mode
      *( ptCtl->pnTCCRA ) = BIT( WGM01 );
      
      // check for output
      if ( PGM_RDBYTE( ptDef->bOutputOn ))
      {
        // set toggle
        *( ptCtl->pnTCCRA ) |= BIT( COM0A0 );
      }

      // set the compare value
      *( ptCtl->tRegs.tTim8.apnOCR[ 0 ] ) = ( U8 )uDivider;
      
      // enable the output compare interrupt
      ptCtl->nIntMask |= BIT( OCIE0A );

      // check to see if time is on
      if ( PGM_RDBYTE( ptDef->bInitialOn ))
      {
        // enable the interrupts
        *( ptCtl->pnTIMSK ) = ptCtl->nIntMask;

        // turn on the timer
        *( ptCtl->pnTCCRB ) |= nPrescale; 
      }
      break;
      
    case TIMER_MODE8_PWMFASTM :
      break;
      
    case TIMER_MODE8_PWMPCO :
      break;
      
    case TIMER_MODE8_PWMFASTO :
      break;
      
    default :
      break;
  }
}

/******************************************************************************
 * @function Setup16BitTimer
 *
 * @brief 16 biit timer initialization
 *
 * This function configures a 16 bit timer to its selected mode of operation
 *
 * @param[in]   eTimer    timer selection
 *
 *****************************************************************************/
static void Setup16BitTimer( TIMERSENUM eTimer, PTIMERDEF ptDef )
{
  U32           uDivider;
  PTIMERCTL     ptCtl;
  U8            nTemp, nMode, nPrescale;
  
  // set the pointers
  ptCtl = &atTimerCtls[ eTimer ];

  // set the registers
  switch( PGM_RDBYTE( ptDef->eSelect ))
  {
    case TIMER_SEL_1 :
      #ifdef TCCR1A
        ptCtl->pnTCCRA = ( PU8 )&TCCR1A;
        ptCtl->pnTCCRB = ( PU8 )&TCCR1B;
        ptCtl->pnTIMSK = ( PU8 )&TIMSK1;
        ptCtl->pnTIFR = ( PU8 )&TIFR1;
        ptCtl->tRegs.tTim16.pwTCNT = ( PU16 )&TCNT1;
        ptCtl->tRegs.tTim16.apwOCR[ TIMER_CHAN_A ] = ( PU16 )&OCR1A;
        ptCtl->tRegs.tTim16.apwOCR[ TIMER_CHAN_B ] = ( PU16 )&OCR1B;
        #ifdef OC1RC 
        ptCtl->tRegs.tTim16.apwOCR[ TIMER_CHAN_C ] = ( PU16 )&OCR1C;
        #endif // OCR1C
        ptCtl->tRegs.tTim16.pwICR = ( PU16 )&ICR1;
        ptCtl->nPrescaleMask = TIMER_PRESCALE16_MASK;
        atPhyMaps[ 1 ].eTimer = eTimer;

      #endif // TCCR1A
      break;
      
    case TIMER_SEL_3 :
      #ifdef TCCR3A
        ptCtl->pnTCCRA = ( PU8 )&TCCR3A;
        ptCtl->pnTCCRB = ( PU8 )&TCCR3B;
        ptCtl->pnTIMSK = ( PU8 )&TIMSK3;
        ptCtl->pnTIFR = ( PU8 )&TIFR3;
        ptCtl->tRegs.tTim16.pwTCNT = ( PU16 )&TCNT3;
        ptCtl->tRegs.tTim16.apwOCR[ TIMER_CHAN_A ] = ( PU16 )&OCR3A;
        ptCtl->tRegs.tTim16.apwOCR[ TIMER_CHAN_B ] = ( PU16 )&OCR3B;
        ptCtl->tRegs.tTim16.apwOCR[ TIMER_CHAN_C ] = ( PU16 )&OCR3C;
        ptCtl->tRegs.tTim16.pwICR = ( PU16 )&ICR3;
        ptCtl->nPrescaleMask = TIMER_PRESCALE16_MASK;
        atPhyMaps[ 3 ].eTimer = eTimer;
      #endif  // TCCR1A
      break;
      
    default :
      break;
  }
  
   // get the prescale
  nPrescale = PGM_RDBYTE( ptDef->nPrescale );
  ptCtl->wPrescale = PGM_RDWORD( awPreScales16[ nPrescale ] );

  // configure the modes
  switch( PGM_RDBYTE( ptDef->tControl.tTim16.eMode ))
  {
    case TIMER_MODE16_CAPT :
      // set the initial edge/filter
      *( ptCtl->pnTCCRB ) = ( PGM_RDBYTE( ptDef->tControl.tTim16.eInitEdge )) ? BIT( ICES1 ) : 0;
      *( ptCtl->pnTCCRB ) |= ( PGM_RDBYTE( ptDef->tControl.tTim16.bIcFilterEnb )) ? BIT( ICNC1 ) : 0;
      
      // enable the output compare interrupt
      ptCtl->nIntMask |= BIT( ICIE1 );
      
      // check to see if overflow interrupt is on
      if ( PGM_RDBYTE( ptDef->bTovEnb ))
      {
        // enable the interrupt
        ptCtl->nIntMask |= BIT( TOIE1 );
      }
      break;
      
    case TIMER_MODE16_PWMPC8M :
      break;
      
    case TIMER_MODE16_PWMPC9M :
      break;
      
    case TIMER_MODE16_PWMPC10M :
      break;
      
    case TIMER_MODE16_CTCO :
      // calculate the reload for the rate
      uDivider = Clock_GetFreq( ) / ptCtl->wPrescale;
      uDivider /= PGM_RDDWRD( ptDef->uRateHz );
  
      // enable CTC mode
      *( ptCtl->pnTCCRB ) = BIT( WGM12 );
      
      // check for output
      if ( PGM_RDBYTE( ptDef->bOutputOn ))
      {
        // set toggle
        *( ptCtl->pnTCCRA ) |= BIT( COM1A0 );
      }

      // set the compare value
      *( ptCtl->tRegs.tTim16.apwOCR[ 0 ] ) = ( U16 )uDivider;
    
      // enable the output compare interrupt
      ptCtl->nIntMask |= BIT( OCIE1A );
      break;
      
    case TIMER_MODE16_PWMFAST8M :
      break;
      
    case TIMER_MODE16_PWMFAST9M :
      break;
      
    case TIMER_MODE16_PWMFAST10M :
      break;
      
    case TIMER_MODE16_PWMPFCI :
      break;
      
    case TIMER_MODE16_PWMPFCO :
      break;
      
    case TIMER_MODE16_PWMPCI :
      break;
      
    case TIMER_MODE16_PWMPCO :
      break;
      
    case TIMER_MODE16_CTCI :
      break;
      
    case TIMER_MODE16_PWMFASTI :
      // calculate the reload for the rate
      uDivider =  Clock_GetFreq( ) / ptCtl->wPrescale;
      uDivider /= PGM_RDDWRD( ptDef->uRateHz );
      
      // set up the actions
      nTemp = BITS( PGM_RDBYTE( ptDef->tControl.tTim16.aeOCRActions[ TIMER_CHAN_A ] ), COM1A0 );
      nTemp |= BITS( PGM_RDBYTE( ptDef->tControl.tTim16.aeOCRActions[ TIMER_CHAN_B ] ), COM1B0 );
      #ifdef COM1C0
      nTemp |= BITS( PGM_RDBYTE( ptDef->tControl.tTim16.aeOCRActions[ TIMER_CHAN_C ] ), COM1C0 );
      #endif // COM1C0

      // add the least significant bits of the mode
      nMode = PGM_RDBYTE( ptDef->tControl.tTim16.eMode );
      nTemp |=  nMode & 0x03;
      *( ptCtl->pnTCCRA ) = nTemp;
      
      // set up the lower bits of the mode
      nTemp = ( nMode >> 2 ) & 0x03;
      *( ptCtl->pnTCCRB ) = BITS( nTemp, WGM12 );

      // clear the percent
      *( ptCtl->tRegs.tTim16.apwOCR[ TIMER_CHAN_A ] ) = 0;
      *( ptCtl->tRegs.tTim16.apwOCR[ TIMER_CHAN_B ] ) = 0;
      *( ptCtl->tRegs.tTim16.apwOCR[ TIMER_CHAN_C ] ) = 0;

      // set up the period
      *( ptCtl->tRegs.tTim16.pwICR ) = uDivider;
      break;
      
    case TIMER_MODE16_PWMFASTO :
      break;
      
    default :
      break;
  }

  // check to see if time is on
  if ( PGM_RDBYTE( ptDef->bInitialOn ))
  {
    // enable the interrupts
    *( ptCtl->pnTIMSK ) = ptCtl->nIntMask;

    // turn on the timer
    *( ptCtl->pnTCCRB ) |= nPrescale;
  }
}

/******************************************************************************
 * @function CommonCmpIrq
 *
 * @brief common compare interrupt handler
 *
 * This function processes the compare interrupt
 *
 * @param[in]   eTimer    timer selection
 *
 *****************************************************************************/
static void CommonCmpIrq( TIMERSENUM eTimer )
{
  PTIMERDEF       ptDef;
  PVTIMERCALLBACK pvCallback;
  
  // set the pointers
  ptDef = ( PTIMERDEF )&atTimerDefs[ eTimer ];
  
  // determine if there is a callback function
  if (( pvCallback = ( PVTIMERCALLBACK )PGM_RDWORD( ptDef->pvCallback )) != NULL )
  {
    // call the function
    pvCallback( TIMER_CBEVENT_CMPUP, 0 );
  }
}

/******************************************************************************
 * @function CommonCapIrq
 *
 * @brief common capture interrupt handler
 *
 * This function processes the capture interrupt
 *
 * @param[in]   eTimer    timer selection
 *
 *****************************************************************************/
static void CommonCapIrq( TIMERSENUM eTimer )
{
  PTIMERDEF       ptDef;
  PTIMERCTL       ptCtl;
  PVTIMERCALLBACK pvCallback;
  TIMERCBEVENT    eEvent;
  U16             wCapture;
  
  // set the pointers
  ptDef = ( PTIMERDEF )&atTimerDefs[ eTimer ];
  ptCtl = &atTimerCtls[ eTimer ];
  
  // get the capture value
  wCapture = *( ptCtl->tRegs.tTim16.pwICR );
  
  // reset the counter
  *( ptCtl->tRegs.tTim16.pwTCNT ) = 0;
  *( ptCtl->pnTIFR ) |= TOV1;

  // set the edge
  eEvent = ( *( ptCtl->pnTCCRB ) & BIT( ICES1 )) ? TIMER_CBEVENT_CAPRE : TIMER_CBEVENT_CAPFE;
  
  // check for edge toggle
  if ( PGM_RDBYTE( ptDef->tControl.tTim16.eIcpEdge ) == TIMER_ICPEDGE_BOTH )
  {
    // toggle the edge
    *( ptCtl->pnTCCRB ) ^= BIT( ICES1 );
  }
  
  // determine if there is a callback function
  if (( pvCallback = ( PVTIMERCALLBACK )PGM_RDWORD( ptDef->pvCallback )) != NULL )
  {
    // call the function
    pvCallback( eEvent, wCapture );
  }
 }

/******************************************************************************
 * @function CommonOvfIrq
 *
 * @brief common overflow interrupt handler
 *
 * This function processes the overflow interrupt
 *
 * @param[in]   eTimer    timer selection
 *
 *****************************************************************************/
static void CommonOvfIrq( TIMERSENUM eTimer )
{
  PTIMERDEF       ptDef;
  PVTIMERCALLBACK pvCallback;
  
  // set the pointers
  ptDef = ( PTIMERDEF )&atTimerDefs[ eTimer ];
  
  // determine if there is a callback function
  if (( pvCallback = ( PVTIMERCALLBACK )PGM_RDWORD( ptDef->pvCallback )) != NULL )
  {
    // call the function
    pvCallback( TIMER_CBEVENT_TMO, 0 );
  }
}

/**@} EOF Timers.c */
