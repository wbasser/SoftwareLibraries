/******************************************************************************
 * @file AscBinDirector.c
 *
 * @brief communications director implementation
 *
 * This file provides the implementation for the communications director
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
 * \addtogroup AscBinDirector
 * @{
 *****************************************************************************/

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "AscBinDirector/AscBinDirector.h"

// library includes -----------------------------------------------------------
#include "AsciiCommandHandler/AsciiCommandHandler.h"
#include "BinaryCommandHandler/BinaryCommandHandler.h"

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations ----------------------------------------------

// local parameter declarations -----------------------------------------------
static ASCBINSEL  eAscBinDirSel;

// local function prototypes --------------------------------------------------

// constant parameter initializations -----------------------------------------

/******************************************************************************
 * @function AscBInDIrector_Initialize
 *
 * @brief initialize the ascii/binary director functionality
 *
 * This function will initilaize the ascii/binary director functionality
 *
 *****************************************************************************/
void AscBinDirector_Initialize( void )
{
  // clear the ASCII message flag
  eAscBinDirSel = ASCBIN_SEL_ASC;
  
  // call the local iniitialization
  AscBinDirector_LocalInitialize( );
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
 * @return      TRUE
 *
 *****************************************************************************/
void AscBinDirector_ProcessChar( ASCBINDIRENUMS eEnum, U8 nChar, U8 nCompareValue )
{
  ASCCMDENUM  eAscProtEnum;
  BINCMDENUM  eBinProtEnum;

  // check for a valid enum
  if ( eEnum < ASCBINDIR_ENUM_MAX )
  {
    eAscProtEnum = PGM_RDBYTE( g_atAscBinDirectDefs[ eEnum ].eAscProtEnum );
    eBinProtEnum = PGM_RDBYTE( g_atAscBinDirectDefs[ eEnum ].eBinProtEnum );

    // call the binary handler
    if ( BinaryCommandHandler_ProcessChar( eBinProtEnum, nChar, nCompareValue ) == BINCMD_STS_IDLE )
    {
      // set the ascii selected
      eAscBinDirSel = ASCBIN_SEL_ASC;

      // not a valid start character, try the ASCII protocol
      AsciiCommandHandler_ProcessChar( eAscProtEnum, ( C8 )nChar, nCompareValue );
    }
    else
    {
      // set the binary selected
      eAscBinDirSel = ASCBIN_SEL_BIN;
    }
  }
}

/******************************************************************************
 * @function 
 *
 * @brief 
 *
 * This function 
 *
 * @return     the current state of the selection
 *
 *****************************************************************************/
ASCBINSEL AscBinDirector_GetSelection( void )
{
  // return the current selection
  return( eAscBinDirSel );
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
 * @return      TRUE
 *
 *****************************************************************************/
void AscBinDirector_SetSelection( ASCBINSEL eSel )
{
  // set the selectin
  if ( eSel < ASCBIN_SEL_MAX )
  {
    eAscBinDirSel = eSel;
  }
}

/**@} EOF CommDirector.c */
