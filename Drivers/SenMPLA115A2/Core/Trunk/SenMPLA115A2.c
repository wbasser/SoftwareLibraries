/******************************************************************************
 * @file SenMPLA115A2.c
 *
 * @brief MPLA115A2 barometric sensor implementation
 *
 * This file provides the implementation for the MPLA115A2
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
 * \addtogroup SenMPLA115A2
 * @{
 *****************************************************************************/

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "SenMPLA115A2/SenMPLA115A2.h"

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------
/// define the shifts for the coeeficients
#define COEEF_A0_SHIFT                    ( 8.0 )
#define COEEF_B1_SHIFT                    ( 8192.0 )
#define COEEF_B2_SHIFT                    ( 16384.0 )
#define COEEF_C12_SHIFT                   ( 4194304.0 )
#define COEEF_C12_LSBSHIFT                ( 4.0 )

/// define the device address
#define DEVICE_ADDRESS                    ( 0x60 )

/// define the commands
#define COMMAND_READ_PRESS                ( 0x00 )
#define COMMAND_READ_TEMP                 ( 0x02 )
#define COMMAND_READ_COEEF                ( 0x04 )

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------
/// define the result structure
typedef union _RESULT
{
  struct
  {
    U8  nPresMsb;
    U8  nPresLsb;
    U8  nTempMsb;
    U8  nTempLsb;
  } tFields;
  U8  anBuffer[ 1 ];
} RESULT *PRESULT;
#define RESULT_SIZE               sizeof( RESULT )

/// define the co-efficients structure
typedef struct _COEEF
{
  struct
  {
    U8  nA0Msb;
    U8  nA0Lsb;
    U8  nB1Msb;
    U8  nB1Lsb;
    U8  nB2Msb;
    U8  nB2Lsb;
    U8  nC12Msb;
    U8  nC12Lsb;
  } tFields;
  U8  anBuffer[ 1 ];
} COEEF, *PCOEEF;
#define COEEF_SIZE                sizeof( COEEF )

// global parameter declarations ----------------------------------------------

// local parameter declarations -----------------------------------------------
static  FLOAT fA0Coeef;
static  FLOAT fB1Coeef;
static  FLOAT fB2Coeef;
static  FLOAT fC12Coeef;

// local function prototypes --------------------------------------------------

// constant parameter initializations -----------------------------------------

/******************************************************************************
 * @function SenMPLA1152_Initialize
 *
 * @brief initialization
 *
 * This function performs any initialization necessary
 *
 *****************************************************************************/
void SenMPLA1152_Initialize( void )
{
  // read the coeeficients
}


/******************************************************************************
 * @function SenMPLA1152_GetValue
 *
 * @brief get a value
 *
 * This function will return the correct value ( humidity/temperature )
 *
 * @param[in]   eType     type of value
 *
 * @return      the current value for the appropriate channel
 *
 *****************************************************************************/
U16 SenMPLA1152_GetValue( SENMPLA115ATYPE eType )
{
}

/******************************************************************************
 * @function SenMPLA1152_Lineraize
 *
 * @brief value lineratization
 *
 * This function will taked the passed value ( A2D counts ) and convert them
 * to a valid sensor value
 *
 * @param[in]   wValue      current value in A2D counts
 * @param[in]   nOption     option
 *
 * @return      current real word value
 *
 *****************************************************************************/
FLOAT SenMPLA1152_Linearize( U16 wValue, U8 nOption )
{
}
 
/**@} EOF SenMPLA115A2.c */
