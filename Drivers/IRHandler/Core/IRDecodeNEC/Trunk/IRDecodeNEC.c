/******************************************************************************
 * @file IRDecodeNEC
 *
 * @brief decode the IR NEC capture values
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
#include "IRHandler\IRDecodeNEC\IRDecodeNEC.h"

// library includes -----------------------------------------------------------
#include "Clock/Clock.h"

// Macros and Defines ---------------------------------------------------------
/// define the header HI/LO times
#define HDR_ON_TIME_NSEC        ( 9000000 )
#define HDRS_OFF_TIME_NSEC      ( 4500000 )
#define HDRR_OFF_TIME_NSEC      ( 2250000 )

/// define the heder hi/lo tolerances
#define HDR_ON_TIME_TOL_NSEC    ( HDR_ON_TIME_NSEC / 4 )
#define HDRS_OFF_TIME_TOL_NSEC  ( HDRS_OFF_TIME_NSEC / 4 )
#define HDRR_OFF_TIME_TOL_NSEC  ( HDRR_OFF_TIME_NSEC / 4 )

/// define the bit HI/LO times
#define BIT_ON_TIME_NSEC        ( 562000 )
#define BIT0_OFF_TIME_NSEC      ( 562000 )
#define BIT1_OFF_TIME_NSEC      ( 1687000 )

/// define the bit HI/LO times tolerances
#define BIT_ON_TIME_TOL_NSEC    ( BIT_ON_TIME_NSEC / 4 )
#define BIT0_OFF_TIME_TOL_NSEC  ( BIT0_OFF_TIME_NSEC / 4 )
#define BIT1_OFF_TIME_TOL_NSEC  ( BIT1_OFF_TIME_NSEC / 4 )

/// define the toleranced header times
#define HDR_ON_MIN_TIME_NSEC    ( HDR_ON_TIME_NSEC - HDR_ON_TIME_TOL_NSEC )
#define HDR_ON_MAX_TIME_NSEC    ( HDR_ON_TIME_NSEC + HDR_ON_TIME_TOL_NSEC )
#define HDRS_OFF_MIN_TIME_NSEC  ( HDRS_OFF_TIME_NSEC - HDRS_OFF_TIME_TOL_NSEC )
#define HDRS_OFF_MAX_TIME_NSEC  ( HDRS_OFF_TIME_NSEC + HDRS_OFF_TIME_TOL_NSEC )
#define HDRR_OFF_MIN_TIME_NSEC  ( HDRR_OFF_TIME_NSEC - HDRR_OFF_TIME_TOL_NSEC )
#define HDRR_OFF_MAX_TIME_NSEC  ( HDRR_OFF_TIME_NSEC + HDRR_OFF_TIME_TOL_NSEC )

/// define the toleranced bit times
#define BIT_ON_MIN_TIME_NSEC    ( BIT_ON_TIME_NSEC - BIT_ON_TIME_TOL_NSEC )
#define BIT_ON_MAX_TIME_NSEC    ( BIT_ON_TIME_NSEC + BIT_ON_TIME_TOL_NSEC )
#define BIT0_OFF_MIN_TIME_NSEC  ( BIT0_OFF_TIME_NSEC - BIT0_OFF_TIME_TOL_NSEC )
#define BIT0_OFF_MAX_TIME_NSEC  ( BIT0_OFF_TIME_NSEC + BIT0_OFF_TIME_TOL_NSEC )
#define BIT1_OFF_MIN_TIME_NSEC  ( BIT1_OFF_TIME_NSEC - BIT1_OFF_TIME_TOL_NSEC )
#define BIT1_OFF_MAX_TIME_NSEC  ( BIT1_OFF_TIME_NSEC + BIT1_OFF_TIME_TOL_NSEC )

// enumerations ---------------------------------------------------------------
/// enumerate the decode states
typedef enum _DECSTATE
{
  DEC_STATE_START = 0,
  DEC_STATE_DATA,
  DEC_STATE_STOP
} DECSTATE;

// structures -----------------------------------------------------------------

// global parameter declarations ----------------------------------------------

// local parameter declarations -----------------------------------------------
static  DECSTATE  eDecState;
static  U32UN     tData;
static  U8        nBitCount;
static  U16       wLastDecode;
static  BOOL      bLastRepeat;
static  U16       wBitXOnMinCounts;
static  U16       wBitXOnMaxCounts;
static  U16       wBit0OffMinCounts;
static  U16       wBit0OffMaxCounts;
static  U16       wBit1OffMinCounts;
static  U16       wBit1OffMaxCounts;
static  U16       wHdrXOnMinCounts;
static  U16       wHdrXOnMaxCounts;
static  U16       wHdrSOffMinCounts;
static  U16       wHdrSOffMaxCounts;
static  U16       wHdrROffMinCounts;
static  U16       wHdrROffMaxCounts;

// local function prototypes --------------------------------------------------
static  BOOL  CheckValidTimes( U16 wTime, U16 wMinTime, U16 wMaxTime );

// constant parameter initializations -----------------------------------------

/******************************************************************************
 * @function 
 *
 * @brief 
 *
 * This function 
 *
 *****************************************************************************/
void IRDecodeNEC_Initialize( void )
{
  U32   uClockFreq, uPrescale, uCountNsecs;

  // reset the state/bit count/last decode
  eDecState = DEC_STATE_START;
  nBitCount = 0;
  bLastRepeat = FALSE;

  // get the clock frequency/timer prescale and compute the count in USEC
  uClockFreq = Clock_GetFreq( );
  Timers_Ioctl( IRHANDLER_CAPTURE_TIMER, TIMER_IOCTL_GETPRESCALE, ( PVOID )&uPrescale );
  uCountNsecs = uClockFreq / uPrescale;
  uCountNsecs = 1000000000 / uCountNsecs;

  // now compute the header counts
  wHdrXOnMinCounts = HDR_ON_MIN_TIME_NSEC / uCountNsecs;
  wHdrXOnMaxCounts = HDR_ON_MAX_TIME_NSEC / uCountNsecs;
  wHdrSOffMinCounts = HDRS_OFF_MIN_TIME_NSEC / uCountNsecs;
  wHdrSOffMaxCounts = HDRS_OFF_MAX_TIME_NSEC / uCountNsecs;
  wHdrROffMinCounts = HDRR_OFF_MIN_TIME_NSEC / uCountNsecs;
  wHdrROffMaxCounts = HDRR_OFF_MAX_TIME_NSEC / uCountNsecs;

  // compute the data bit counts
  wBitXOnMinCounts = BIT_ON_MIN_TIME_NSEC / uCountNsecs;
  wBitXOnMaxCounts = BIT_ON_MAX_TIME_NSEC / uCountNsecs;
  wBit0OffMinCounts = BIT0_OFF_MIN_TIME_NSEC / uCountNsecs;
  wBit0OffMaxCounts = BIT0_OFF_MAX_TIME_NSEC / uCountNsecs;
  wBit1OffMinCounts = BIT1_OFF_MIN_TIME_NSEC / uCountNsecs;
  wBit1OffMaxCounts = BIT1_OFF_MAX_TIME_NSEC / uCountNsecs;
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
U16 IRDecodeNEC_Process( PU32UN ptCapValue )
{
  U8        nBit = 0;
  U16       wReturn;
  U16UN     tDecode;
  
  // reset the value
  wReturn = 0;

  // determine the state
  switch( eDecState )
  {
    case DEC_STATE_START :
      // is this a valid start bit
      if ( CheckValidTimes( ptCapValue->awValue[ IRCAP_INDEX_ONTIME ], wHdrXOnMinCounts, wHdrXOnMaxCounts ))
      {
        // now check for a start bit
        if ( CheckValidTimes( ptCapValue->awValue[ IRCAP_INDEX_OFFTIME ], wHdrSOffMinCounts, wHdrSOffMaxCounts ))
        {
          // valid start bit/clear bit count/clear data/goto data
          nBitCount = 0;
          tData.uValue = 0;
          eDecState = DEC_STATE_DATA;
          wLastDecode = 0;
        }
        else if ( CheckValidTimes( ptCapValue->awValue[ IRCAP_INDEX_OFFTIME ], wHdrROffMinCounts, wHdrROffMaxCounts ))
        {
          // check to see if repeat is enabled
          if ( bLastRepeat == TRUE )
          {
            // return the last key
            wReturn = wLastDecode;
          }
        }
      }
      break;

    case DEC_STATE_DATA :
      // check for a valid bit
      if( CheckValidTimes( ptCapValue->awValue[ IRCAP_INDEX_ONTIME ], wBitXOnMinCounts, wBitXOnMaxCounts ))
      {
        // increment the bit count/shift data
        tData.uValue <<= 1;
        nBitCount++;

        // now determine if this is a zero
        if ( CheckValidTimes( ptCapValue->awValue[ IRCAP_INDEX_OFFTIME ], wBit0OffMinCounts, wBit0OffMaxCounts ))
        {
          // set bit to 0
          nBit = 0;
        }
        else if ( CheckValidTimes( ptCapValue->awValue[ IRCAP_INDEX_OFFTIME ], wBit1OffMinCounts, wBit1OffMaxCounts ))
        {
          // set bit to 1
          nBit = 1;
        }
        else
        {
          // error
          eDecState = DEC_STATE_START;
        }

        // add the bit
        tData.uValue |= nBit;

        // check for done
        if ( nBitCount == IRHANDLER_NUM_BITS )
        {
          // determine if the stop bit disable is set
          #if ( IRHANDLER_STOPBIT_DISABLE == 1 )
          // set the decode
          tDecode.anValue[ LE_U16_MSB_IDX ] = tData.anValue[ LE_U32_MSB_IDX ];
          tDecode.anValue[ LE_U16_LSB_IDX ] = tData.anValue[ LE_U32_MS1_IDX ];
          
          // set the decoded value
          wLastDecode = tDecode.wValue;
          wReturn = wLastDecode;

          // back to start
          eDecState = DEC_STATE_START;
          #else
          // set state to stop
          eDecState = DEC_STATE_STOP;
          #endif
        }
      }
      else
      {
        // error - back to start bit
        eDecState = DEC_STATE_START;
      }
      break;

    case DEC_STATE_STOP :
      // check for a valid stop bit
      if ( CheckValidTimes( ptCapValue->awValue[ IRCAP_INDEX_ONTIME ], BIT_ON_MIN_TIME_NSEC, BIT_ON_MAX_TIME_NSEC ))
      {
        // now validate the data
        if ((( tData.anValue[ LE_U32_MSB_IDX ] | tData.anValue[ LE_U32_MS2_IDX ] ) == 0xFF ) &&
            (( tData.anValue[ LE_U32_LSB_IDX ] | tData.anValue[ LE_U32_MS1_IDX ] ) == 0xFF ))
        {
          // set the decode
          tDecode.anValue[ LE_U16_MSB_IDX ] = tData.anValue[ LE_U32_MSB_IDX ];
          tDecode.anValue[ LE_U16_LSB_IDX ] = tData.anValue[ LE_U32_MS1_IDX ];
          
          // set the decoded value
          wLastDecode = tDecode.wValue;
          wReturn = wLastDecode;
        }
      }

      // reset back to start
      eDecState = DEC_STATE_START;
      break;

    default :
      break;
  }

  // return the value
  return( wReturn );
}

/******************************************************************************
 * @function 
 *
 * @brief 
 *
 * This function 
 *
 * @param[in]   wTime     time to compare
 * @param[in]   wMinTime  minimum time
 * @param[in]   wMaxTime  maximum time
 *
 * @return      TRUE if valid time, FALSE otherwise
 *
 *****************************************************************************/
static BOOL CheckValidTimes( U16 wTime, U16 wMinTime, U16 wMaxTime )
{
  BOOL bResult = FALSE;

  // check for valid min time
  if (( wTime >= wMinTime ) && ( wTime <= wMaxTime ))
  {
    // set good result
    bResult = TRUE;
  }

  // return the result
  return( bResult );
}

/**@} EOF IRDecodeNEC.c */
