/******************************************************************************
 * @file IRHandler.c
 *
 * @brief IR handler implementation
 *
 * This file provides the IR handler implementation
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
 * \addtogroup IRHandler
 * @{
 *****************************************************************************/

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "IRHandler/IRHandler.h"
#if ( IRHANDLER_CUR_MODE == IRHANDLER_MODE_NEC )
#include "IRHandler/IRDecodeNEC/IRDecodeNEC.h"
#elif ( IRHANDLER_CUR_MODE == IRHANDLER_MODE_RC5 )
#include "IRHandler/IRDecodeRc5/IRDecodeRc5.h" 
#endif

// library includes -----------------------------------------------------------
#include "QueueManager/QueueManager.h"
#if (( IRHANDLER_ENABLE_RAW_DEBUG == 1 ) || ( IRHANDLER_ENABLE_CODE_DEBUG == 1 ))
#include "DebugManager/DebugManager.h"
#endif

// Macros and Defines ---------------------------------------------------------
/// define the queue put event 
#define IRCAPTURE_QUEUE_EVENT     ( QUEUEPUT_EVENT( IRHANDLER_CAPTURE_QUEUE ))

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations ----------------------------------------------

// local parameter declarations -----------------------------------------------

// local function prototypes --------------------------------------------------

// constant parameter initializations -----------------------------------------

/******************************************************************************
 * @function IRHandler_Initialize
 *
 * @brief IR handler initialization
 *
 * This function will intialize each of the sub functions 
 *
 *****************************************************************************/
void IRHandler_Initialize( void )
{
  // initalize the decoder
  #if ( IRHANDLER_CUR_MODE == IRHANDLER_MODE_NEC )
  IRDecodeNEC_Initialize( );
  #elif ( IRHANDLER_CUR_MODE == IRHANDLER_MODE_RC5 )
  IRDeocdeRc5_Initialize( );
  #endif

  // turn on the capture control
  IRCapture_Control( ON );
}

/******************************************************************************
 * @function IRHandler_ProcessEvent
 *
 * @brief process output events
 *
 * This function process's all events for the management of the output function
 *
 * @param[in]   xArg      event argument
 *
 * @return      TRUE
 *
 *****************************************************************************/
BOOL IRHandler_ProcessEvent( TASKARG xArg )
{
  U32UN             tCapValue;
  U16               wCode;
  PIRHANDCMDDEF     ptDef;
  IRHANDCMDENUM     eIdx;
  PVIRHANDCALLBACK  pvCallback;
  
  // get the argument from the queue
  if ( xArg == IRCAPTURE_QUEUE_EVENT )
  {
    // get the queue event
    QueueManager_Get( IRHANDLER_CAPTURE_QUEUE, ( PU8 )&tCapValue );

    // stuff the raw data if enabled
    #if ( IRHANDLER_ENABLE_RAW_DEBUG == 1 )
    DebugManager_AddElement( tCapValue.awValue[ IRCAP_INDEX_ONTIME ], tCapValue.awValue[ IRCAP_INDEX_OFFTIME ] );
    #endif // IRHANDLER_ENABLE_RAW_DEBUG
  
    #if ( IRHANDLER_CUR_MODE == IRHANDLER_MODE_NEC )
    if (( wCode = IRDecodeNEC_Process( &tCapValue )) != 0 )
    #elif ( IRHANDLER_CUR_MODE == IRHANDLER_MODE_RC5 )
    if (( wCode == IRDeocdeRc5_Process( &tCapValue )) != 0 )
    #endif
    {
      // find code
      for ( eIdx = 0; eIdx < IRHAND_CMDENUM_MAX; eIdx++ )
      {
        // set the pointer
        ptDef = ( PIRHANDCMDDEF )&atIrHandCmdDefs[ eIdx ];
      
        // compare codes
        if ( PGM_RDWORD( ptDef->wCode ) == wCode )
        {
          // now determine the reporting method
          switch( PGM_RDBYTE( ptDef->eRptMethod ))
          {
            case IRHAND_RPTMETHOD_CB :
            // check for valid callback
            if (( pvCallback = ( PVIRHANDCALLBACK )PGM_RDWORD( ptDef->pvCallback )) != NULL )
            {
              // do it
              pvCallback( eIdx );
            }
            break;
          
            case IRHAND_RPTMETHOD_EVENT :
            // post an event
            TaskManager_PostEvent( PGM_RDBYTE( ptDef->eTaskEnum ), eIdx );
            break;
          
            default :
            break;
          }
        
          // exit loop
          break;
        }
      }
    
      // check for debug
      #if ( IRHANDLER_ENABLE_CODE_DEBUG == 1 )
      DebugManager_AddElement( 0xC0DE, wCode );
      #endif // IRHANDLER_ENABLE_CODE_DEBUG
    }
  }
  
  // return the status
  return( TRUE );
}

/**@} EOF IRHandler.c */
