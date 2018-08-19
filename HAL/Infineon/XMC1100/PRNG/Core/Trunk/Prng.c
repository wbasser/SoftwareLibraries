/******************************************************************************
 * @file Prng.c
 *
 * @brief 
 *
 * This file 
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
 * \addtogroup  Prng
 * @{
 *****************************************************************************/

// system includes ------------------------------------------------------------

// library includes -----------------------------------------------------------
#include "SystemTick/SystemTick.h"

// local includes -------------------------------------------------------------
#include "PRNG/Prng.h"

// Macros and Defines ---------------------------------------------------------
/// define the access values
#define PRNG_CTL_BYTE_ACCESS_MSK  BIT( 0x01 )
#define PRNG_CTL_WORD_ACCESS_MSK  BIT( 0x02 )

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations ----------------------------------------------

// local parameter declarations -----------------------------------------------

// local function prototypes --------------------------------------------------

// constant parameter initializations -----------------------------------------

/******************************************************************************
 * @function Prng_Initialize
 *
 * @brief initialize the psuedo random number generator
 *
 * This function will initialize any local variables for the PRNG
 *
 *****************************************************************************/
void Prng_Initialize( void )
{
}

/******************************************************************************
 * @function Prng_Seed
 *
 * @brief seed the random number generator
 *
 * This function will take the seed value(s) and store them into the 
 * random number generator
 *
 * @param[in]   tSeed     seed value
 *
 *****************************************************************************/
void Prng_Seed( U32UN tSeed )
{
  U64UN tCurrentTime;
  U8    nIdx;
  U32   uTemp;
  
  // get the current time
  tCurrentTime.hValue = SystemTick_GetTimeUsec( );
  
  // set up the PRNG for 16 bit access and key load
  PRNG->CTRL = PRNG_CTL_WORD_ACCESS_MSK | PRNG_CTRL_KLD_Msk;
  
  // store each value
  for ( nIdx = LE_U64_LSW_IDX; nIdx <= LE_U64_MSW_IDX; nIdx++ )
  {
    // load the word/wait for ready
    PRNG->WORD = tCurrentTime.awValue[ nIdx ];
    while( PRNG->CHK != PRNG_CHK_RDV_Msk );
  }
  
  // now store the seed
  for ( nIdx = LE_U32_LSW_IDX; nIdx <= LE_U32_MSW_IDX; nIdx++ )
  {
    // load the word/wait for ready
    PRNG->WORD = tSeed.awValue[ nIdx ];
    while( PRNG->CHK != PRNG_CHK_RDV_Msk );
  }
  
  // flush 64 bits
  uTemp = PRNG->WORD;
  uTemp ^= PRNG->WORD;
  uTemp ^= PRNG->WORD;
  uTemp ^= PRNG->WORD;
  
  // now set up for streaming mode/byte access
  PRNG->CTRL= PRNG_CTL_BYTE_ACCESS_MSK;
}

/******************************************************************************
 * @function Prng_Generate
 *
 * @brief generate a random number and return it
 *
 * This function will return the next random number in the psuedo sequence
 *
 * @return    next random number
 *
 *****************************************************************************/
U8 Prng_Generate( void )
{
  U32 uTemp;
  
  // get the value
  uTemp = PRNG->WORD;
  
  // return it
  return(( U8 )uTemp );
}
 
/**@} EOF Prng.c */
