/******************************************************************************
 * @file Clock.c
 *
 * @brief clock module implementation
 *
 * This file clock module initialization and up/down scaling
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
 * \addtogroup Clock
 * @{
 *****************************************************************************/

// system includes ------------------------------------------------------------

// library includes -----------------------------------------------------------

// local includes -------------------------------------------------------------
#include "Clock/Clock.h"

// Macros and Defines ---------------------------------------------------------
#define   DELAY_CNT_VALUE       ( 0x3FF )

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations ----------------------------------------------

// local parameter declarations -----------------------------------------------

// local function prototypes --------------------------------------------------
static  void  UpScaleClock( U32 uCur, U32 uTgt );
static  void  DnScaleClock( U32 uCur, U32 uTgt );

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
  U32 uCurIdiv, uTgtIdiv, uClkCr;
  
  // Open the lock that protects privileged bits
  SCU_GENERAL->PASSWD = 0xC0U;
  while((( SCU_GENERAL->PASSWD ) & SCU_GENERAL_PASSWD_PROTS_Msk ));

  // Update PCLK selection mux
  SCU_CLK->CLKCR |= BITS( CLOCK_PCLKX2_ENABLE, SCU_CLK_CLKCR_PCLKSEL_Pos );

  // get the current IDIV value/set the target
  uCurIdiv = ((( SCU_CLK->CLKCR ) & SCU_CLK_CLKCR_IDIV_Msk ) >> SCU_CLK_CLKCR_IDIV_Pos);
  uTgtIdiv = CLOCK_IDIV_VALUE;

  // Update FDIV here
  uClkCr = SCU_CLK->CLKCR;
  WR_REG( uClkCr, SCU_CLK_CLKCR_FDIV_Msk, SCU_CLK_CLKCR_FDIV_Pos, CLOCK_FDIV_VALUE );
  SCU_CLK->CLKCR = uClkCr;

  // Delay, till frequency stable, appox. 15us.
  WR_REG( SCU_CLK->CLKCR, SCU_CLK_CLKCR_CNTADJ_Msk, SCU_CLK_CLKCR_CNTADJ_Pos, DELAY_CNT_VALUE );
  while ( SCU_CLK->CLKCR & SCU_CLK_CLKCR_VDDC2LOW_Msk );

  //determine up/down scaling of clock
  if ( uCurIdiv < uTgtIdiv )
  {
    // down scale the clock
    DnScaleClock( uCurIdiv, uTgtIdiv );
  }
  else
  {
    // up sclae the clock
    UpScaleClock( uCurIdiv, uTgtIdiv );
  }

  // disable any more changes
  SCU_GENERAL->PASSWD = 0xC3;
}
 
/******************************************************************************
 * @function AllowClkInitByStartup
 *
 * @brief test to allow startup to modify clock
 *
 * This function prevent the startup file from modifying the clock
 *
 * @return  0 to disallow clock init in the startup file.
 *
 *****************************************************************************/
U32 AllowClkInitByStartup( void )
{
  // always return 0
  return ( 0 );
}

/******************************************************************************
 * @function Clock_Initialization( void )
 *
 * @brief clock initialization
 *
 * This function will initialize the clock control module
 *
 *****************************************************************************/
static void UpScaleClock( U32 uCur, U32 uTgt )
{
  BOOL  bRunFlag = TRUE;
  U32   uClkCr;
  
  // loop
  while( bRunFlag )
  {
    if( uCur > (  uTgt * 4 ))
    {
      // add 4 and divide by 4
      uCur = ( uCur & 0xFFFFFFFCU ) + 4U;
      uCur = uCur >> 2;
        
      // now set the IDIV to current
      uClkCr = SCU_CLK->CLKCR;
      WR_REG( uClkCr, SCU_CLK_CLKCR_IDIV_Msk, SCU_CLK_CLKCR_IDIV_Pos, uCur );
      SCU_CLK->CLKCR = uClkCr;

      // wait for stable frequency
      WR_REG( SCU_CLK->CLKCR, SCU_CLK_CLKCR_CNTADJ_Msk, SCU_CLK_CLKCR_CNTADJ_Pos, DELAY_CNT_VALUE );
      while ( SCU_CLK->CLKCR & SCU_CLK_CLKCR_VDDC2LOW_Msk );
    }
    else
    {
      // now set the IDIV to target
      uClkCr = SCU_CLK->CLKCR;
      WR_REG( uClkCr, SCU_CLK_CLKCR_IDIV_Msk, SCU_CLK_CLKCR_IDIV_Pos, uTgt );
      SCU_CLK->CLKCR = uClkCr;
      
      // wait for stable frequency
      WR_REG( SCU_CLK->CLKCR, SCU_CLK_CLKCR_CNTADJ_Msk, SCU_CLK_CLKCR_CNTADJ_Pos, DELAY_CNT_VALUE );
      while ( SCU_CLK->CLKCR & SCU_CLK_CLKCR_VDDC2LOW_Msk );
      
      // clear run flag
      bRunFlag = FALSE;
    }
  }
}

/******************************************************************************
 * @function DnScaleClock
 *
 * @brief decrease the clock
 *
 * This function will decrease the clock
 *
 *****************************************************************************/
static void DnScaleClock(  U32 uCur, U32 uTgt )
{
  BOOL  bRunFlag = TRUE;
  U32   uClkCr;
  
  // loop
  while( bRunFlag )
  {
    if( uTgt > (  uCur * 4 ))
    {
      // add 4 and multiply by 4
      uCur = ( uCur & 0xFFFFFFFCU ) + 4U;
      uCur = uCur << 2;
        
      // now set the IDIV to current
      uClkCr = SCU_CLK->CLKCR;
      WR_REG( uClkCr, SCU_CLK_CLKCR_IDIV_Msk, SCU_CLK_CLKCR_IDIV_Pos, uCur );
      SCU_CLK->CLKCR = uClkCr;

      // wait for stable frequency
      WR_REG( SCU_CLK->CLKCR, SCU_CLK_CLKCR_CNTADJ_Msk, SCU_CLK_CLKCR_CNTADJ_Pos, DELAY_CNT_VALUE );
      while ( SCU_CLK->CLKCR & SCU_CLK_CLKCR_VDDC2LOW_Msk );
    }
    else
    {
      // now set the IDIV to target
      uClkCr = SCU_CLK->CLKCR;
      WR_REG( uClkCr, SCU_CLK_CLKCR_IDIV_Msk, SCU_CLK_CLKCR_IDIV_Pos, uTgt );
      SCU_CLK->CLKCR = uClkCr;
      
      // wait for stable frequency
      WR_REG( SCU_CLK->CLKCR, SCU_CLK_CLKCR_CNTADJ_Msk, SCU_CLK_CLKCR_CNTADJ_Pos, DELAY_CNT_VALUE );
      while ( SCU_CLK->CLKCR & SCU_CLK_CLKCR_VDDC2LOW_Msk );
      
      // clear run flag
      bRunFlag = FALSE;
    }
  }
}

/**@} EOF Clock.c */
