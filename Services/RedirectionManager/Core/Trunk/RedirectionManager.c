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

// Macros and Defines ---------------------------------------------------------
/// define the exit character
#define EXIT_REDIRECT_CHAR                  ( 0x01 )    ///< CTRL-A

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations ----------------------------------------------

// local parameter declarations -----------------------------------------------
static  REDIRECTENUM  aeDestinations[ REDIRECT_ENUM_MAX ];

// local function prototypes --------------------------------------------------

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
  REDIRECTENUM    eDst;
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
      pvRedirectOut = ( PVREDIRECTOUT )PGM_RDWORD( &g_apvRedirectOuts[ eDst ] );
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

/**@} EOF RedirectionManager.c */
