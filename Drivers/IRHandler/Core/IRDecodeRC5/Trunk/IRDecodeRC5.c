/******************************************************************************
 * @file IRDecodeRC5
 *
 * @brief decode the IR RC5 capture values
 *
 * This file 
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
 * \addtogroup IR
 * @{
 *****************************************************************************/

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "IRHandler\IRDecodeRC5\IRDecodeRC5.h"

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------
/// define the RC5 half bit time
#define RC5_HALFBIT_MEANTIME  ( 889 )

/// define the RC5 bit tolerance( 25% )
#define RC5_BIT_TOLERANCE     ( RC5_HALFBIT_MEANTIME / 4 )

/// define the minimum/maximum half bit times
#define HB_MIN_TIME           ( RC5_HALFBIT_MEANTIME - RC5_BIT_TOLERANCE )
#define HB_MAX_TIME           ( RC5_HALFBIT_MEANTIME + RC5_BIT_TOLERANCE )

/// define the minimum/maximum full bit times
#define FB_MIN_TIME           (( RC5_HALFBIT_MEANTIME * 2 ) - RC5_BIT_TOLERANCE )
#define FB_MAX_TIME           (( RC5_HALFBIT_MEANTIME * 2 ) + RC5_BIT_TOLERANCE )

/// define the number of RC5 command bits
#define RC5_NUM_CMD_BITS      ( 13 )

// enumerations ---------------------------------------------------------------
/// enumerate the decode states
typedef enum _DECPHASE
{
  DEC_PHASE_SYNC = 0,
  DEC_PHASE_DATA,
} DECPHASE;

/// enumerate the bit decodes
typedef enum _DECSTATE
{
  DEC_STATE_P0_DHAH   = 0,    ///< 0 
  DEC_STATE_P0_DHAF,          ///< 1
  DEC_STATE_P0_DFAH,          ///< 2
  DEC_STATE_P0_AFDF,          ///< 3
  DEC_STATE_P1_DHAH,          ///< 4
  DEC_STATE_P1_DHAF,          ///< 5
  DEC_STATE_P1_DFAH,          ///< 6
  DEC_STATE_P1_AFDF,          ///< 7
} DECSTATE;

/// enumerate the bit state
typedef enum _BITSTATE
{
  BIT_STATE_H = 0,
  BIT_STATE_F,
  BIT_STATE_ERR = 0x20
} BITSTATE;

/// enumerate the bit positions
typedef enum _BITPOS
{
  BIT_POS_A = 0,    /// active
  BIT_POS_D,        // deactive
  BIT_POS_P         // previous
} BITPOS;

// structures -----------------------------------------------------------------

// global parameter declarations ----------------------------------------------

// local parameter declarations -----------------------------------------------
static  DECPHASE  eDecPhase;
static  U8        nDecState;
static  U16       wCurData;
static  U8        nActRcvBits;
static  BOOL      bPrvBit;

// local function prototypes --------------------------------------------------
static  U16       ShiftRcvBit( BOOL bBit );
static  BITSTATE  DecodeBitTime( U16 wCapTime );

// constant parameter initializations -----------------------------------------

/******************************************************************************
 * @function 
 *
 * @brief 
 *
 * This function 
 *
 *****************************************************************************/
void IRDecodeRC5_Initialize( void )
{
  // reset the state/bit count/last decode
  eDecPhase = DEC_PHASE_SYNC;
}

/******************************************************************************
 * @function 
 *
 * @brief 
 *
 * This function 
 *
 * @param[in]   
 *
 *****************************************************************************/
U16 IRDecodeRC5_Process( PIRCAPVAL ptCapValue )
{
  U16 wReturn;
  
  // reset the value
  wReturn = 0;
  
  // if we are in sync bit phase
  if ( eDecPhase == DEC_PHASE_SYNC )
  {
    // force a valid off bit time
    ptCapValue->wOffTime = RC5_HALFBIT_MEANTIME;
  }
    
  // build the action
  nDecState = bPrvBit << BIT_POS_P;
  nDecState |= DecodeBitTime( ptCapValue->wOffTime ) << BIT_POS_D;
  nDecState |= DecodeBitTime( ptCapValue->wOnTime ) << BIT_POS_A;

  // now determine the bits to shift
  switch( nDecState )
  {
    case DEC_STATE_P0_DHAH :
    case DEC_STATE_P1_DFAH :
       // shift a 0
      wReturn = ShiftRcvBit( LO );
      break;

    case DEC_STATE_P1_DHAH :
      // shift a 1
      wReturn = ShiftRcvBit( HI );
      break;

    case DEC_STATE_P0_DHAF :
    case DEC_STATE_P1_AFDF :
      // shift a 0,1
      wReturn = ShiftRcvBit( LO );
      wReturn = ShiftRcvBit( HI );
      break;

    default :
      // reset the state
      eDecPhase = DEC_PHASE_SYNC;
      break;
  }
  
  // return the value
  return( wReturn );
}

/******************************************************************************
 * @function ShiftRcvBit
 *
 * @brief shift the bit into the data
 *
 * This function will shift a bit in the current data value, store the data
 * in the buffer and adjust the expected length
 *
 * @param[in]   bBit      current bit value
 *
 *****************************************************************************/
static U16 ShiftRcvBit( BOOL bBit )
{
  U16 wReturn = 0;
  
  // update previous bit
  bPrvBit = bBit;

  // determine the state
  switch( eDecPhase )
  {
    case DEC_PHASE_SYNC :
      // go to the data state/reset the bit count
      eDecPhase = DEC_PHASE_DATA;
      nActRcvBits = 0;
      break;
      
    case DEC_PHASE_DATA :
      // shift and add the bit
      wCurData <<= 1;
      wCurData |= bBit;
      
      // test for done
      if ( ++nActRcvBits == RC5_NUM_CMD_BITS )
      {
        // post the event
        wReturn = wCurData;
        
        // do nothing
        // reset the state
        eDecPhase = DEC_PHASE_SYNC;
      }
      break;

    default :
      // should never happen
      eDecPhase = DEC_PHASE_SYNC;
      break;
  }
  
  // return the result
  return( wReturn );
}

/******************************************************************************
 * @function DecodeBitTime
 *
 * @brief decode the bit time
 *
 * This function determine what time of bit was receiveid
 *
 * @param[in]   wCapTime      current bit value
 *
 *****************************************************************************/
static BITSTATE DecodeBitTime( U16 wCapTime )
{
  BITSTATE eBitState = BIT_STATE_ERR;

  // determine if this is a valid half bit time
  if (( wCapTime > HB_MIN_TIME ) && ( wCapTime < HB_MAX_TIME ))
  {
    eBitState = BIT_STATE_H;
  }
  else if (( wCapTime > FB_MIN_TIME ) && ( wCapTime < FB_MAX_TIME ))
  {
    eBitState = BIT_STATE_F;
  }
  
  // return the dec state
  return( eBitState );
}

/**@} EOF IRDecodeRC5.c */
