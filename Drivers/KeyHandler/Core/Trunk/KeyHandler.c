/******************************************************************************
 * @file KeyHandler.c
 *
 * @brief key handler implementation 
 *
 * This file provides the implementation for the key handler
 *
 * @copyright Copyright (c) 2012 CyberIntegration
 * This document contains proprietary data and information of CyberIntegration 
 * LLC. It is the exclusive property of CyberIntegration, LLC and will not be 
 * disclosed in any form to any party without prior written permission of 
 * CyberIntegration, LLC. This document may not be reproduced or further used 
 * without the prior written permission of CyberIntegration, LLC.
 *
 * $Date: $
 *
 * Version History
 * ======
 * $Rev: $
 * 
 *
 * \addtogroup KeyHandler
 * @{
 *****************************************************************************/

// system includes ------------------------------------------------------------
#ifdef __ATMELAVR__
#include "Util/delay.h"
#endif // __ATMELAVR__

// local includes -------------------------------------------------------------
#include "KeyHandler/KeyHandler.h"

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations ----------------------------------------------

// local parameter declarations -----------------------------------------------

// local function prototypes --------------------------------------------------

// constant parameter initializations -----------------------------------------

/******************************************************************************
 * @function KeyHandler_Initialize
 *
 * @brief key handler initialization
 *
 * This function provides the initialization of the key handler module
 *
 *****************************************************************************/
void KeyHandler_Initialize( void )
{
}

/******************************************************************************
 * @function KeyHandler_GetKeyState
 *
 * @brief Key Handler get key state
 *
 * This function will return the current key state for a given key
 *
 * @param[in]   eKeyEnum      key enumeration
 * @param[in]   pbSTate       pointer to return the key state
 *
 * @return     approproiate error
 *
 *****************************************************************************/
KEYHANDERR KeyHandler_GetKeyState( KEYHANDENUM eKeyEnum, PBOOL pbState )
{
  KEYHANDERR  eError = KEYHAND_ERR_NONE;
  PKEYHANDDEF ptDef;
  GPIOPINENUM eRowEnum;
  PVGETFUNC   pvGetFunc;
  
  // first determine if a valid key enum
  if ( eKeyEnum < KEYHAND_ENUM_MAX )
  {
    // set the pointer to the defintion
    ptDef = ( PKEYHANDDEF )&atKeyHandDefs[ eKeyEnum ];
    
    // determine the type of key
    switch( PGM_RDBYTE( ptDef->eType ))
    {
      case KEYHAND_TYPE_DIRECT :
        // get the state of the key
        Gpio_Get( PGM_RDBYTE( ptDef->eColEnum ), pbState );
        break;
        
      case KEYHAND_TYPE_ROWCOL :
        // get the row enum
        eRowEnum = PGM_RDBYTE( ptDef->eRowEnum );
        
        // set the row active
        Gpio_Set( eRowEnum, ON );
        #ifdef __ATMELAVR__
        _delay_us( 25 );
        #endif // __ATMELAVR__
        Gpio_Get( PGM_RDBYTE( ptDef->eColEnum ), pbState );
        
        // turn off the row
        Gpio_Set( eRowEnum, OFF );
        break;
        
      case KEYHAND_TYPE_SPECIAL :
        eRowEnum = PGM_RDBYTE( ptDef->eColEnum );
        pvGetFunc = ( PVGETFUNC )PGM_RDWORD( ptDef->pvGetFunc );
        *( pbState ) = pvGetFunc( eRowEnum );
        break;

      default :
        break;

    }
  }
  else
  {
    // set the error
    eError = KEYHAND_ERR_ILLENUM;
  }
  
  // return the key state
  return( eError );
}

/**@} EOF KeyHandler.c */
