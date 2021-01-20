/******************************************************************************
 * @file RedirectionManager.c
 *
 * @brief redirection manager implementation
 *
 * This file provides the implementation for the redirection manager
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
 * \addtogroup RedirectionManager
 * @{
 *****************************************************************************/

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "RedirectionManager/RedirectionManager.h"

// library includes -----------------------------------------------------------
#if ( REDIRECTIONMANAGER_ENABLE_DEBUG_COMMANDS == 1 )
  #include "SystemControlManager/SystemControlManager.h"
#endif  // REDIRECTIONMANAGER_ENABLE_DEBUG_COMMANDS

// Macros and Defines ---------------------------------------------------------
/// define the exit character
#define EXIT_REDIRECT_CHAR                  ( 0x01 )    ///< CTRL-A

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations ----------------------------------------------

// local parameter declarations -----------------------------------------------
static  REDIRECTENUM  aeDestinations[ REDIRECT_ENUM_MAX ];

// local function prototypes --------------------------------------------------
/// command handlers
#if ( REDIRECTIONMANAGER_ENABLE_DEBUG_COMMANDS == 1 )
  static  ASCCMDSTS CmdRedirect( U8 nCmdEnum );
#endif  // REDIRECTIONMANAGER_ENABLE_DEBUG_COMMANDS

// constant parameter initializations -----------------------------------------
// commands
#if ( REDIRECTIONMANAGER_ENABLE_DEBUG_COMMANDS == 1 )
  static  const CODE C8 szRedirect[ ]   = { "REDIRECT" };

  // declare the table
  const CODE ASCCMDENTRY g_atRedirectionManagerDbgCmdTable[ ] = 
  {
    ASCCMD_ENTRY( szRedirect, 8, 2, ASCFLAG_COMPARE_NONE, 0, CmdRedirect ),

    // the entry below must be here
    ASCCMD_ENDTBL( )
  };
#endif  // REDIRECTIONMANAGER_ENABLE_DEBUG_COMMANDS

// constant parameter initializations -----------------------------------------

/******************************************************************************
 * @function RedirectionManager_Initialize
 *
 * @brief initialization
 *
 * This function will perform any needed initialization
 *
 *****************************************************************************/
void RedirectionManager_Initialize( void )
{
  REDIRECTENUM  eDevIdx;

  // set all redirects to zero
  for ( eDevIdx = 0; eDevIdx < REDIRECT_ENUM_MAX; eDevIdx++ )
  {
    // set to none
    aeDestinations[ eDevIdx ] = REDIRECT_ENUM_ILLEGAL;
  }
}

/******************************************************************************
 * @function RedirectionManager_SetRedirect
 *
 * @brief set the redirection destinaltion for a given source
 *
 * This function will test to ensure that this is a valid redirection and that 
 * one does not exist for this interface
 *
 * @param[in]   eSrc      source device
 * @param[in]   eDst      destination device
 *
 * @return      FALSE if no errors, TRUE otherwise
 *
 *****************************************************************************/
BOOL RedirectionManager_SetRedirect( REDIRECTENUM eSrc, REDIRECTENUM eDst )
{
  BOOL  bError = TRUE;

  // check for valid source/destination
  if (( eSrc < REDIRECT_ENUM_MAX ) && ( eDst < REDIRECT_ENUM_MAX ) && ( eSrc < eDst ) && ( aeDestinations[ eSrc ] == REDIRECT_ENUM_ILLEGAL ))
  {
    // set the re-direction
    aeDestinations[ eSrc ] = eDst;
    aeDestinations[ eDst ] = eSrc;

    // clear the error
    bError = FALSE;
  }

  // return the error
  return( bError );
}

/******************************************************************************
 * @function RedirectionManagerChar_Redirect
 *
 * @brief redirect
 *
 * This function will send the character to the redirect port
 *
 * @param[in]   eSrc      source of the character
 * @param[in]   nchar     character to be redirected
 *
 * @return      TRUE if character redirected, FALSE if not
 *
 *****************************************************************************/
BOOL RedirectionManager_RedirectChar( REDIRECTENUM eSrc, U8 nChar )
{
  BOOL          bRedirected = FALSE;
  REDIRECTENUM  eDst;
  PVREDIRECTOUT pvRedirectOut;

  // check for valid redirection
  if ( aeDestinations[ eSrc ] != REDIRECT_ENUM_ILLEGAL )
  {
    // get the destination
    eDst = aeDestinations[ eSrc ];

    // check for exit
    if ( nChar == REDIRECT_EXIT_CHAR )
    {
      // cancel re-direction
      aeDestinations[ eSrc ] = REDIRECT_ENUM_ILLEGAL;
      aeDestinations[ eDst ] = REDIRECT_ENUM_ILLEGAL;
    }
    else
    {
      // set redirected status
      bRedirected = TRUE;

      // get the re-direction handler
      pvRedirectOut = ( PVREDIRECTOUT )PGM_RDWORD( g_apvRedirectOuts[ eDst ] );
      if ( pvRedirectOut != NULL )
      {
        // call it
        pvRedirectOut( nChar );
      }
    }
  }

  // return the redirection staus
  return( bRedirected );
}

#if ( REDIRECTIONMANAGER_ENABLE_DEBUG_COMMANDS == 1 )
static  ASCCMDSTS CmdRedirect( U8 nCmdEnum )
{
  ASCCMDSTS     eStatus = ASCCMD_STS_NONE;
  U32UN         tValue;
  REDIRECTENUM  eSrc, eDst;
  PC8           pcBuffer;

  // get the buffer
  AsciiCommandHandler_GetBuffer( nCmdEnum, &pcBuffer );

  // get the source,destination
  AsciiCommandHandler_GetValue( nCmdEnum, 0, &tValue.uValue );
  eSrc = tValue.anValue[ LE_U32_LSB_IDX ];
  AsciiCommandHandler_GetValue( nCmdEnum, 1, &tValue.uValue );
  eDst = tValue.anValue[ LE_U32_LSB_IDX ];
  if ( RedirectionManager_SetRedirect( eSrc, eDst ))
  {
    // report error
    sprintf( pcBuffer, ( PCC8 )g_szAsciiErrStrn, 0xFF, 0xFF );
    AsciiCommandHandler_OutputBuffer( nCmdEnum );
  }

  // return the status
  return( eStatus );
}
#endif  // REDIRECTIONMANAGER_ENABLE_DEBUG_COMMANDS

/**@} EOF RedirectionManager.c */
