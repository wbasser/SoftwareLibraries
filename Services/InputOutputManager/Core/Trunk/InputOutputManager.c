/******************************************************************************
 * @file InputOutputManager.c
 *
 * @brief input/output manager implementation
 *
 * This file provides the implementation for the input/output manager
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
 * \addtogroup InputOutputManager
 * @{
 *****************************************************************************/

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "InputOutputManager/InputOutputManager.h"

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------
/// enumerate the input states
typedef enum _INPSTATE
{
  INP_STATE_IDLE = 0,       ///< idle state
  INP_STATE_DEBOUNCE,       ///< debounce
} INPSTATE;

/// enuemrate the latch states
typedef enum _RLYSTATE
{
  RLY_STATE_IDLE = 0,       ///< idle state
  RLY_STATE_DRIVING,        ///< driving
} RLYSTATE;

// structures -----------------------------------------------------------------
/// define the control structure
typedef struct _IOMNGRCTL
{
  union
  {
    INPSTATE    eInput;     ///< current input state
    RLYSTATE    eRelay;     ///< current relay state
  } tStates;;
  U16           wCurTime;   ///< current debounce/action time
  BOOL          bPrvIO;     ///< previous input/output state
  BOOL          bActIO;     ///< actual input/ouput state
} IOMNGRCTL, *PIOMNGRCTL;
#define IOMNGRCTL_SIZE      sizeof( IOMNGRCTL )

// global parameter declarations ----------------------------------------------

// local parameter declarations -----------------------------------------------
static  IOMNGRCTL   atCtrls[ IOMNGR_CHN_ENUM_MAX ];

// local function prototypes --------------------------------------------------
static  void  ProcessInput( PIOMNGRCHNDEF ptDef, PIOMNGRCTL ptCtl );
static  void  ProcessRelayDualCoil( PIOMNGRCHNDEF ptDef, PIOMNGRCTL ptCtl );
static  void  ProcessRelaySingCoil( PIOMNGRCHNDEF ptDef, PIOMNGRCTL ptCtl );
static  void  ProcessRelayMatrix( PIOMNGRCHNDEF ptDef, PIOMNGRCTL ptCtl );

// constant parameter initializations -----------------------------------------

/******************************************************************************
 * @function InputOutputManager_Initialize
 *
 * @brief initilization
 *
 * This function will perform any necessary initiliazation
 *
 *****************************************************************************/
void InputOutputManager_Initialize( void )
{
  // clear the control structure
  memset( atCtrls, 0, sizeof( atCtrls ));
}

/******************************************************************************
 * @function InputOutputManager_ProcessEvent
 *
 * @brief event processor
 *
 * This function will process the event and perform any associated action
 *
 * @param[in]   xArg        task argument
 *
 * @return        TRUE        returns true to flush the event    
 *
 *****************************************************************************/
BOOL InputOutputManager_ProcessEvent( TASKARG xArg )
{
  IOMNGRCHNENUM   eEnum;
  PIOMNGRCHNDEF   ptDef;
  PIOMNGRCTL      ptCtl;

  // for each relay
  for ( eEnum = 0; eEnum < IOMNGR_CHN_ENUM_MAX; eEnum++ )
  {
    // get the pointers
    ptDef = ( PIOMNGRCHNDEF )&atIOMngrChnDefs[ eEnum ];
    ptCtl = &atCtrls[ eEnum ];

    // determine the type
    switch( PGM_RDBYTE( ptDef->eType ))
    {
      case IOMNGR_TYPE_INP :
        ProcessInput( ptDef, ptCtl );
        break;

      case IOMNGR_TYPE_RLY :
        if ( ptCtl->bPrvIO != ptCtl->bActIO )
        {
          // set the relay state/set previous to actual
          Gpio_Set( PGM_RDBYTE( ptDef->aePins[ IOMNGRDEF_OUT_RLYCTL_PIN_IDX ] ), ptCtl->bActIO );
          ptCtl->bPrvIO = ptCtl->bActIO;
        }
        break;

      case IOMNGR_TYPE_RLYDCLATCH :
        ProcessRelayDualCoil( ptDef, ptCtl );
        break;

      case IOMNGR_TYPE_RLYSCLATCH :
        ProcessRelaySingCoil( ptDef, ptCtl );
        break;
        
      case IOMNGR_TYPE_RLYMATRIX :
        ProcessRelayMatrix( ptDef, ptCtl );
        break;
        
      default :
        break;
    }
  }

  // flush the event
  return( TRUE );
}

/******************************************************************************
 * @function InputOutputManager_SetRelay
 *
 * @brief set a relays state
 *
 * This function will set a relay to requested state
 *
 * @param[in]   eIOMngrEnum   relay enumeration
 * @param[in]   bState            state of the relay
 *
 * @return      approrpiate IOMNGRERR error
 *
 *****************************************************************************/
IOMNGRERR InputOutputManager_SetRelay( IOMNGRCHNENUM eIOMngrEnum, BOOL bState )
{
  IOMNGRERR   eError = IOMNGR_ERR_NONE;
  PIOMNGRCTL  ptCtl;

  // check for a valid enum
  if ( eIOMngrEnum < IOMNGR_CHN_ENUM_MAX )
  {
    // get the pointer to the control
    ptCtl = &atCtrls[ eIOMngrEnum ];

    // check for busy
    if (ptCtl->tStates.eRelay == RLY_STATE_IDLE )
    {
      // set the actual to the new stata
      ptCtl->bActIO = bState;
    }
    else
    {
      // return the busy error
      eError = IOMNGR_ERR_RLYBSY;
    }
  }
  else
  {
    // return illegal enum error
    eError = IOMNGR_ERR_ILLENUM;
  }

  // return the error
  return( eError );
}

/******************************************************************************
 * @function InputOutputManager_GetState
 *
 * @brief get the state of a device
 *
 * This function will return the state of an device
 *
 * @param[in]   eIOMngrEnum   device numeration
 *
 * @return      approrpiate IOMNGRERR error
 *
 *****************************************************************************/
IOMNGRERR InputOutputManager_GetState( IOMNGRCHNENUM eIOMngrEnum, PBOOL pbState )
{
  IOMNGRERR   eError = IOMNGR_ERR_NONE;
  PIOMNGRCTL  ptCtl;

  // check for a valid enum
  if ( eIOMngrEnum < IOMNGR_CHN_ENUM_MAX )
  {
    // get the pointer to the control
    ptCtl = &atCtrls[ eIOMngrEnum ];

    // return the current state
    *pbState = ptCtl->bActIO;
  }
  else
  {
    // return illegal enum error
    eError = IOMNGR_ERR_ILLENUM;
  }

  // return the error
  return( eError );
}

/******************************************************************************
 * @function ProcessInput
 *
 * @brief process an input channel
 *
 * This function will perform necessary processing on an input channel
 *
 * @param[in]   ptDef     pointer to the definitions structure
 * @param[in]   ptCtl     pointer to the controls structure
 *
 *****************************************************************************/
static void ProcessInput( PIOMNGRCHNDEF ptDef, PIOMNGRCTL ptCtl )
{
  BOOL              bCurState;
  PVIOMNGRCALLBACK  pvCallback;

  // determine if the state has changed
  Gpio_Get( PGM_RDBYTE( ptDef->aePins[ IOMNGRDEF_INP_PIN_IDX ] ), &bCurState );

  // determine the state
  switch( ptCtl->tStates.eInput )
  {
    case INP_STATE_IDLE :
      if ( bCurState != ptCtl->bPrvIO )
      {
        // there was a change - setup for debounce
        ptCtl->bPrvIO = bCurState;
        ptCtl->wCurTime = TASK_TIME_MSECS( PGM_RDWORD( ptDef->wTime )) / INPUTOUTPUTMANAGER_EXEC_RATE;

        // change states
        ptCtl->tStates.eInput = INP_STATE_DEBOUNCE;
      }
      break;

    case INP_STATE_DEBOUNCE :
      // check for no change in put
      if ( bCurState == ptCtl->bPrvIO )
      {
        // decrement the debounce time
        if ( --ptCtl->wCurTime == 0 )
        {
          //  change actual state/back to idle
          ptCtl->bActIO = ptCtl->bPrvIO;
          ptCtl->tStates.eInput = INP_STATE_IDLE;

          // check for a callback
          pvCallback = ( PVIOMNGRCALLBACK )PGM_RDWORD( ptDef->pvCallback );
          if ( pvCallback != NULL )
          {
            // do it
            pvCallback(( ptCtl->bActIO ) ? IOMNGRCB_EVENT_INPON : IOMNGRCB_EVENT_INPOFF );
          }
        }
      }
      else
      {
        // reset previous to actual/back to idle
        ptCtl->bPrvIO = ptCtl->bActIO;
        ptCtl->tStates.eInput = INP_STATE_IDLE;
      }
      break;

    default :
      // should never happen
      ptCtl->tStates.eInput = INP_STATE_IDLE;
      break;
  }  
}

/******************************************************************************
 * @function ProcessRelayDualCoil
 *
 * @brief process a dual coil relay
 *
 * This function will perform necessary processing on an relay channel
 *
 * @param[in]   ptDef     pointer to the definitions structure
 * @param[in]   ptCtl     pointer to the controls structure
 *
 *****************************************************************************/
static void ProcessRelayDualCoil( PIOMNGRCHNDEF ptDef, PIOMNGRCTL ptCtl )
{
  // determine the state
  switch( ptCtl->tStates.eRelay )
  {
    case RLY_STATE_IDLE :
      // check for a requested change
      if ( ptCtl->bActIO != ptCtl->bPrvIO )
      {
        // set up the delay
        ptCtl->wCurTime = TASK_TIME_MSECS( PGM_RDWORD( ptDef->wTime )) / INPUTOUTPUTMANAGER_EXEC_RATE;

        // turn on the set/reset pins approrpiate
        Gpio_Set((( ptCtl->bActIO ) ? PGM_RDBYTE( ptDef->aePins[ IOMNGRDEF_OUT_RLYSET_PIN_IDX ] ) : PGM_RDBYTE( ptDef->aePins[ IOMNGRDEF_OUT_RLYRST_PIN_IDX ] )), ON );

        // go to driving state
        ptCtl->tStates.eRelay = RLY_STATE_DRIVING;
      }
      break;

    case RLY_STATE_DRIVING :
      // check for driving done
      if ( --ptCtl->wCurTime == 0 )
      {
        // set the previous to actual
        ptCtl->bPrvIO = ptCtl->bActIO;

        // turn off both drives
        Gpio_Set( PGM_RDBYTE( ptDef->aePins[ IOMNGRDEF_OUT_RLYSET_PIN_IDX ] ), OFF );
        Gpio_Set( PGM_RDBYTE( ptDef->aePins[ IOMNGRDEF_OUT_RLYRST_PIN_IDX ] ), OFF );

        // back to idle
        ptCtl->tStates.eRelay = RLY_STATE_IDLE;
      }
      break;

    default :
      // should never happen
      ptCtl->tStates.eRelay = RLY_STATE_IDLE;
      break;
  }  
}

/******************************************************************************
 * @function ProcessRelaySingCoil
 *
 * @brief process a single coil relay
 *
 * This function will perform necessary processing on an relay channel
 *
 * @param[in]   ptDef     pointer to the definitions structure
 * @param[in]   ptCtl     pointer to the controls structure
 *
 *****************************************************************************/
static void ProcessRelaySingCoil( PIOMNGRCHNDEF ptDef, PIOMNGRCTL ptCtl )
{
  GPIODIR eDir;

  // determine the state
  switch( ptCtl->tStates.eRelay )
  {
    case RLY_STATE_IDLE :
      // check for a requested change
      if ( ptCtl->bActIO != ptCtl->bPrvIO )
      {
        // set up the delay
        ptCtl->wCurTime = TASK_TIME_MSECS( PGM_RDWORD( ptDef->wTime )) / INPUTOUTPUTMANAGER_EXEC_RATE;

        // make the pins output
        eDir = GPIO_DIR_OUT;
        Gpio_Ioctl( PGM_RDBYTE( ptDef->aePins[ IOMNGRDEF_OUT_RLYRST_PIN_IDX ] ), GPIO_ACT_SETDIR, &eDir );
        Gpio_Ioctl( PGM_RDBYTE( ptDef->aePins[ IOMNGRDEF_OUT_RLYSET_PIN_IDX ] ), GPIO_ACT_SETDIR, &eDir );

        // turn on the set/reset pins approrpiate
        Gpio_Set((( ptCtl->bActIO ) ? PGM_RDBYTE( ptDef->aePins[ IOMNGRDEF_OUT_RLYSET_PIN_IDX ] ) : PGM_RDBYTE( ptDef->aePins[ IOMNGRDEF_OUT_RLYRST_PIN_IDX ] )), ON );
        Gpio_Set((( ptCtl->bActIO ) ? PGM_RDBYTE( ptDef->aePins[ IOMNGRDEF_OUT_RLYRST_PIN_IDX ] ) : PGM_RDBYTE( ptDef->aePins[ IOMNGRDEF_OUT_RLYSET_PIN_IDX ] )), OFF );

        // go to driving state
        ptCtl->tStates.eRelay = RLY_STATE_DRIVING;
      }
      break;

    case RLY_STATE_DRIVING :
      // check for driving done
      if ( --ptCtl->wCurTime == 0 )
      {
        // set the previous to actual
        ptCtl->bPrvIO = ptCtl->bActIO;

        // make the pins output
        eDir = GPIO_DIR_IN;
        Gpio_Ioctl( PGM_RDBYTE( ptDef->aePins[ IOMNGRDEF_OUT_RLYRST_PIN_IDX ] ), GPIO_ACT_SETDIR, &eDir );
        Gpio_Ioctl( PGM_RDBYTE( ptDef->aePins[ IOMNGRDEF_OUT_RLYSET_PIN_IDX ] ), GPIO_ACT_SETDIR, &eDir );

        // turn off both drives
        Gpio_Set( PGM_RDBYTE( ptDef->aePins[ IOMNGRDEF_OUT_RLYSET_PIN_IDX ] ), OFF );
        Gpio_Set( PGM_RDBYTE( ptDef->aePins[ IOMNGRDEF_OUT_RLYRST_PIN_IDX ] ), OFF );

        // back to idle
        ptCtl->tStates.eRelay = RLY_STATE_IDLE;
      }
      break;

    default :
      // should never happen
      ptCtl->tStates.eRelay = RLY_STATE_IDLE;
      break;
  }  
}

/******************************************************************************
 * @function ProcessRelayMatrix
 *
 * @brief process a relay matrix
 *
 * This function will perform necessary processing on an relay channel
 *
 * @param[in]   ptDef     pointer to the definitions structure
 * @param[in]   ptCtl     pointer to the controls structure
 *
 *****************************************************************************/
static void ProcessRelayMatrix( PIOMNGRCHNDEF ptDef, PIOMNGRCTL ptCtl )
{
  IOMNGRMTXENUM eMtxEnum;
  GPIOPINENUM   ePinEnum;
  U8            nIndex;
  PIOMNGRMTXDEF ptMatrix;

  // get the matrix enum
  eMtxEnum = PGM_RDBYTE( ptDef->nMtxEnum );
  ptMatrix = ( PIOMNGRMTXDEF )&atIOMngrMtxDefs[ eMtxEnum ];

  // determine the state
  switch( ptCtl->tStates.eRelay )
  {
    case RLY_STATE_IDLE :
      // check for a requested change
      if ( ptCtl->bActIO != ptCtl->bPrvIO )
      {
        // set up the delay
        ptCtl->wCurTime = TASK_TIME_MSECS( PGM_RDWORD( ptDef->wTime )) / INPUTOUTPUTMANAGER_EXEC_RATE;
        
        // set the GPIO pins
        for ( nIndex = 0; nIndex < IOMNGR_RLYMTXIDX_MAX; nIndex++ )
        {
          if (( ePinEnum = PGM_RDBYTE( ptMatrix->aePins[ nIndex ] )) != GPIO_PIN_ENUM_ILLEGAL )
          {
            // set to the requested state
            Gpio_Set( ePinEnum, ( ptCtl->bActIO ) ? LO : HI );
          }
          else
          {
            // exit loop
            break;
          }
        }

        // now drive the set pin to the appropriate state
        ePinEnum = PGM_RDBYTE( ptDef->aePins[ ( ptCtl->bActIO ) ? IOMNGRDEF_OUT_MTXSET_PIN_IDX : IOMNGRDEF_OUT_MTXRST_PIN_IDX ] );
        ePinEnum = PGM_RDBYTE( ptMatrix->aePins[ ePinEnum ] );
        Gpio_Set( ePinEnum, ( ptCtl->bActIO ) ? HI : LO );

        // go to driving state
        ptCtl->tStates.eRelay = RLY_STATE_DRIVING;
      }
      break;

    case RLY_STATE_DRIVING :
      // check for driving done
      if ( --ptCtl->wCurTime == 0 )
      {
        // set the previous to actual
        ptCtl->bPrvIO = ptCtl->bActIO;

        // reset the GPIO pins
        ePinEnum = PGM_RDBYTE( ptDef->aePins[ ( ptCtl->bActIO ) ? IOMNGRDEF_OUT_MTXSET_PIN_IDX : IOMNGRDEF_OUT_MTXRST_PIN_IDX ] );
        ePinEnum = PGM_RDBYTE( ptMatrix->aePins[ ePinEnum ] );
        Gpio_Set( ePinEnum, ( ptCtl->bActIO ) ? LO : HI );

        // set the GPIO pins
        for ( nIndex = 0; nIndex < IOMNGR_RLYMTXIDX_MAX; nIndex++ )
        {
          if (( ePinEnum = PGM_RDBYTE( ptMatrix->aePins[ nIndex ] )) != GPIO_PIN_ENUM_ILLEGAL )
          {
            // set to the requested state
            Gpio_Set( ePinEnum, LO );
          }
          else
          {
            // exit loop
            break;
          }
        }
        
        // back to idle
        ptCtl->tStates.eRelay = RLY_STATE_IDLE;
      }
      break;

    default :
      // should never happen
      ptCtl->tStates.eRelay = RLY_STATE_IDLE;
      break;
  }  
}

/**@} EOF InputOutputManager.c */

