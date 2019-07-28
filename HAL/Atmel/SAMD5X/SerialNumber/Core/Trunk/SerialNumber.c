/******************************************************************************
 * @file SerialNumber.c
 *
 * @brief serial number implementation
 *
 * This file provides the implementation for the serial number
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
 * $Rev: $
 * 
 *
 * \addtogroup SerialNumber
 * @{
 *****************************************************************************/

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "SerialNumber/SerialNumber.h"

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------
/// define the serial number address address
#define SERNUM_WORD_ADDR0                             ( 0x0080A00C )
#define SERNUM_WORD_ADDR1                             ( 0x0080A040 )
#define SERNUM_WORD_ADDR2                             ( 0x0080A044 )
#define SERNUM_WORD_ADDR3                             ( 0x0080A048 )

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations ----------------------------------------------

// local parameter declarations -----------------------------------------------

// local function prototypes --------------------------------------------------

// constant parameter initializations -----------------------------------------

/******************************************************************************
 * @function SerialNumber_Get
 *
 * @brief get the serial number
 *
 * This function will get the serial number
 *
 * @param[io]   ptSerNum    pointer to the serial number
 *
 *****************************************************************************/
void SerialNumber_Get( PSERIALNUMBER ptSerNum )
{
  // get the words
  ptSerNum->tWords.tWord0.uValue = *(( PU32 )( U32 )( SERNUM_WORD_ADDR0 ));
  ptSerNum->tWords.tWord1.uValue = *(( PU32 )( U32 )( SERNUM_WORD_ADDR1 ));
  ptSerNum->tWords.tWord2.uValue = *(( PU32 )( U32 )( SERNUM_WORD_ADDR2 ));
  ptSerNum->tWords.tWord3.uValue = *(( PU32 )( U32 )( SERNUM_WORD_ADDR3 ));
};

/**@} EOF SerialNumber.c */
