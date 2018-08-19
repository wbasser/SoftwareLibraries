/******************************************************************************
 * @file 
 *
 * @brief 
 *
 * This file 
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
 * \addtogroup 
 * @{
 *****************************************************************************/

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "RelayManager/RelayManager.h"

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------
#if ( RELAY_MANAGER_50HZ_SELECTION == 1 )
/// define the maximum slot number
#define MAX_SLOT_NUMBER     ( 20 / RELAY_MANAGER_SCAN_TIME )
#else
#define MAX_SLOT_NUMBER     ( 16 / RELAY_MANAGER_SCAN_TIME )
#endif // RELAY_MANAGER_50HZ_SELECTION

/// define the active on/off time for GPIO drive
#define ACTIVE_DRIVE_TIME   ( TASK_TIME_MSECS( RELAY_MANAGER_DRIVE_ACTIVE_TIME ) / TASK_TIME_MSECS( RELAY_MANAGER_SCAN_TIME ))

// structures -----------------------------------------------------------------
/// define the relay control
typedef struct RELAYCTL
{
  RELAYSTATE    eCurState;      ///< current relay state
  RELAYSTATE    eNxtState;      ///< next relay state
  RELAYSTATE    eReqState;      ///< requested state
  U8            nSlotNumber;    ///< slot number to acutate on/off
  U16           wDelayTime;     ///< active/minon delay time 
} RELAYCTL, *PRELAYCTL;
#define RELAYCTL_SIZE       sizeof( RELAYCTL )

// global parameter declarations ----------------------------------------------

// local parameter declarations -----------------------------------------------
static  U8        nSlotNumber;                                      ///< current slot number
static  RELAYCTL  atRelayCtls[ RELAY_ENUM_MAX ];                    ///< relay control
#if ( RELAY_MANAGER_ENABLE_CHARGEPUMP == 1 )
static  U8        anChargePumpCount[ RELAY_CHARGE_PUMP_ENUM_MAX ];  ///< charge pump count
#endif // RELAY_MANAGER_ENABLE_CHARGEPUMP
#if ( RELAY_MANAGER_ENABLE_KEEPALIVE == 1 )
static  U8        anKeepAliveCount[ RELAY_KEEP_ALIVE_ENUM_MAX ];    ///< timers control
#endif // RELAY_MANAGER_ENABLE_KEEPALIVE
#if (RELAY_MANAGER_ENABLE_DYNONOFTIMES == 1 )
static  RELAYDYMTIMDEF  atRelayOnOffTimes[ RELAY_ENUM_MAX ];
#endif // RELAY_MANAGER_ENABLE_DYNONOFTIMES

// local function prototypes --------------------------------------------------
static  U8        GenerateSlot( void );

// constant parameter initializations -----------------------------------------

/******************************************************************************
 * @function RelayManager_Initialize
 *
 * @brief initailization
 *
 * This function perform any initialization for the relay manager
 *
 *****************************************************************************/
void RelayManager_Initialize( void )
{
  RELAYENUM eRelay;
  
  // clear the charge pump and keep alive counts
#if ( RELAY_MANAGER_ENABLE_CHARGEPUMP == 1 )
  memset( anChargePumpCount, 0, sizeof( anChargePumpCount ));
#endif // RELAY_MANAGER_ENABLE_CHARGEPUMP
#if ( RELAY_MANAGER_ENABLE_KEEPALIVE == 1 )
  memset( anKeepAliveCount, 0, sizeof( anKeepAliveCount ));
#endif // RELAY_MANAGER_ENABLE_KEEPALIVE
  
  // clear the slot number/relay on count
  nSlotNumber = 0;
}

#if (RELAY_MANAGER_ENABLE_DYNONOFTIMES == 1 )
/******************************************************************************
 * @function RelayManager_GetConfigDflt
 *
 * @brief get default config block
 *
 * This function will return a pointer to the actual configuration block
 *
 * @return      pointer to the default config block
 *
 *****************************************************************************/
PRELAYDYMTIMDEF RelayManager_GetConfigDflt( void )
{
  // return a pointer to the default config block
  return(( PRELAYDYMTIMDEF )&atRelayTimesDefs );
}

/******************************************************************************
 * @function RelayManager_GetConfigActl
 *
 * @brief get acutal config block
 *
 * This function will return a pointer to the actual configuration block
 *
 * @return      pointer to the actual config block
 *
 *****************************************************************************/
PRELAYDYMTIMDEF RelayManager_GetConfigActl( void )
{
  // return a pointer to the config block
  return(( PRELAYDYMTIMDEF )&atRelayOnOffTimes );
}
#endif // RELAY_MANAGER_ENABLE_DYNONOFTIMES

/******************************************************************************
 * @function RelayManager_ProcessScan
 *
 * @brief process the scan event
 *
 * This function will scan all relays
 *
 * @param[in]   xArg      task argument
 *
 * @return      TRUE      flush event
 *
 *****************************************************************************/
BOOL RelayManager_ProcessScan( TASKARG xArg )
{
  PRELAYCTLDEF        ptCtlDef;
  PRELAYCTL           ptRlyCtl;
  GPIOPINENUM         eGpioCtlEnum, eGpioDirEnum;
  PVRLYMNGRCALLBACK   pvCallback;
  #if ( RELAY_MANAGER_ENABLE_KEEPALIVE == 1 )
  RELAYKEEPALIVEENUM  eKeepAliveEnum;
  #endif // RELAY_MANAGER_ENABLE_KEEPALIVE
  RELAYENUM           eRelay;
  BOOL                bTimerState;
  U16                 wDelayTime;
  #if ( RELAY_MANAGER_ENABLE_DYNONOFTIMES == 1 )
  PRELAYDYMTIMDEF     ptRlyTim;
  #endif // RELAY_MANAGER_ENABLE_DYNONOFTIMES
  
  // incrment the current slot number
  nSlotNumber++;
  nSlotNumber %= MAX_SLOT_NUMBER;
  
  // for each relay in the table
  for ( eRelay = 0; eRelay < RELAY_ENUM_MAX; eRelay++ )
  {
    // set the pointers to the control/def structures
    ptRlyCtl = &atRelayCtls[ eRelay ];
    ptCtlDef = ( PRELAYCTLDEF )&atRelayCtlDefs[ eRelay ];
    #if ( RELAY_MANAGER_ENABLE_DYNONOFTIMES == 1 )
    ptRlyTim = &atRelayOnOffTimes[ eRelay ];
    #endif // RELAY_MANAGER_ENABLE_DYNONOFTIMES
    
    // determine the current state
    switch( ptRlyCtl->eCurState )
    {
      case RELAY_STATE_OFF :
        // check for a requested state to on
        if (( ptRlyCtl->eReqState == RELAY_STATE_ON ) && ( ptRlyCtl->nSlotNumber == nSlotNumber ))
        {
          // set the current state to request on
          ptRlyCtl->eCurState = RELAY_STATE_REQON;
          
          // generate a new random slot number
          ptRlyCtl->nSlotNumber = GenerateSlot( );
        }
        break;
        
      case RELAY_STATE_ON :
        // check for a requested state to on
        if (( ptRlyCtl->eReqState == RELAY_STATE_OFF ) && ( ptRlyCtl->nSlotNumber == nSlotNumber ))
        {
          // set the current state to request on
          ptRlyCtl->eCurState = RELAY_STATE_REQOFF;
          
          // generate a new random slot number
          ptRlyCtl->nSlotNumber = GenerateSlot( );
        }
        break;
        
      case RELAY_STATE_REQOFF :
        // get the GPIO control enumeration/turn it off
        eGpioCtlEnum = PGM_RDBYTE( ptCtlDef->eGpioCtlEnum );

        // turn on the GPIO pin/set dir
        #if ( RELAY_MANAGER_ENABLE_EXTDRIVER == 1 )
        // set the direction to off
        eGpioDirEnum = PGM_RDBYTE( ptCtlDef->eGpioDirEnum );
        Gpio_Set( eGpioDirEnum, OFF );
        #else
        // turn on the pin
        #if ( RELAY_MANAGER_ENABLE_KEEPALIVE == 1 )
        Gpio_Ioctl( eGpioCtlEnum, GPIO_IOCTL_CHDIR, GPIO_DIR_OUT );
        #endif // RELAY_MANAGER_ENABLE_KEEPALIVE
        Gpio_Set( eGpioCtlEnum, OFF );
        #endif //  RELAY_MANAGER_ENABLE_EXTDRIVER

        // decrement the keep alive timer
        #if ( RELAY_MANAGER_ENABLE_KEEPALIVE == 1 )
        eKeepAliveEnum = PGM_RDBYTE( ptCtlDef->nKeepAliveEnum );
        if ( --anKeepAliveCount[ eKeepAliveEnum ] == 0 )
        {
          // turn off the PWM timer
          bTimerState = OFF;
          Timers_Ioctl( PGM_RDBYTE( atRelayKeepAliveDefs[ eKeepAliveEnum ].eTimerEnum ), TIMER_IOCTL_STOPSTART, ( PVOID )&bTimerState );
        }
        #endif // RELAY_MANAGER_ENABLE_KEEPALIVE
        
        // set the delay
        ptRlyCtl->wDelayTime = ACTIVE_DRIVE_TIME;
        
        // set the next state
        ptRlyCtl->eNxtState = RELAY_STATE_ONDELAY;
        
        // go to active state
        ptRlyCtl->eCurState = RELAY_STATE_DRVACTIVE;
        break;
        
      case RELAY_STATE_REQON :
        // get the GPIO control enumeration/turn it off
        eGpioCtlEnum = PGM_RDBYTE( ptCtlDef->eGpioCtlEnum );

        // turn on the GPIO pin/set dir
        #if ( RELAY_MANAGER_ENABLE_EXTDRIVER == 1 )
        // set the direction to off
        eGpioDirEnum = PGM_RDBYTE( ptCtlDef->eGpioDirEnum );
        Gpio_Set( eGpioDirEnum, ON );
        #else
        // turn on the pin
        #if ( RELAY_MANAGER_ENABLE_KEEPALIVE == 1 )
        Gpio_Ioctl( eGpioDirEnum, GPIO_IOCTL_CHDIR, GPIO_DIR_OUT );
        #endif // RELAY_MANAGER_ENABLE_KEEPALIVE
        Gpio_Set( eGpioCtlEnum, ON );
        #endif // RELAY_MANAGER_ENABLE_EXTDRIVER

        #if ( RELAY_MANAGER_ENABLE_KEEPALIVE == 1 )
        // decrement the keep alive timer
        eKeepAliveEnum = PGM_RDBYTE( ptCtlDef->nKeepAliveEnum );
        if ( ++anKeepAliveCount[ eKeepAliveEnum ] != 0 )
        {
          // turn off the PWM timer
          bTimerState = OFF;
          Timers_Ioctl( PGM_RDBYTE( atRelayKeepAliveDefs[ eKeepAliveEnum ].eTimerEnum ), TIMER_IOCTL_STOPSTART, ( PVOID )&bTimerState );
        }
        #endif // RELAY_MANAGER_ENABLE_KEEPALIVE
        
        // set the delay
        ptRlyCtl->wDelayTime = ACTIVE_DRIVE_TIME;
        
        // set the next state
        ptRlyCtl->eNxtState = RELAY_STATE_ONDELAY;
        
        // go to active state
        ptRlyCtl->eCurState = RELAY_STATE_DRVACTIVE;
        break;
        
      case RELAY_STATE_DRVACTIVE :
        // get the GPIO control enumeration/turn it off
        eGpioCtlEnum = PGM_RDBYTE( ptCtlDef->eGpioCtlEnum );

        // decrement the time
        if ( ptRlyCtl->wDelayTime-- == 0 )
        {
          // check for keepalive
          #if ( RELAY_MANAGER_ENABLE_KEEPALIVE == 1 )
          // change the GPIO Dir
          #if ( RELAY_MANAGER_ENABLE_EXTDRIVER == 1 )
          Gpio_Set( eGpioCtlEnum, OFF );
          #else
          Gpio_Ioctl( eGpioCtlEnum, GPIO_IOCTL_CHDIR, GPIO_DIR_IN );
          #endif //  RELAY_MANAGER_ENABLE_EXTDRIVER
          #endif // RELAY_MANAGER_ENABLE_KEEPALIVE
          
          // goto on delay
          ptRlyCtl->eCurState = ptRlyCtl->eNxtState;
          ptRlyCtl->eNxtState = ( ptRlyCtl->eReqState == RELAY_STATE_ON ) ? RELAY_STATE_ON : RELAY_STATE_OFF;
          
          // set the time
          #if ( RELAY_MANAGER_ENABLE_DYNONOFTIMES == 0 )
          wDelayTime = ( ptRlyCtl->eCurState == RELAY_STATE_ON ) ? PGM_RDWORD( ptCtlDef->wMinOnTimeSecs ) : PGM_RDWORD( ptCtlDef->wMinOffTimeSecs );
          #else
          wDelayTime = ( ptRlyCtl->eCurState == RELAY_STATE_ON ) ? ptRlyTim->wMinOnTimeSecs : ptRlyTim->wMinOffTimeSecs;
          #endif // RELAY_MANAGER_ENABLE_DYNONOFTIMES
          ptRlyCtl->wDelayTime = TASK_TIME_SECS( wDelayTime ) / TASK_TIME_MSECS( RELAY_MANAGER_SCAN_TIME );
        }
        break;
        
      case RELAY_STATE_ONDELAY :
        if ( --ptRlyCtl->wDelayTime == 0 )
        {
          // now go to next state
          ptRlyCtl->eCurState = ptRlyCtl->eNxtState;

          // check for callback
          pvCallback = PGM_RDWORD( ptCtlDef->pvCallback );
          if ( pvCallback != NULL )
          {
            // call the function
            pvCallback(( ptRlyCtl->eCurState == RELAY_STATE_ON ) ? ON : OFF );
          }
        }
        break;
        
      default :
        // should never happen 
        ptRlyCtl->eCurState = RELAY_STATE_OFF;
        break;
    }
  }

  // always return true
  return( TRUE );
}

#if ( RELAY_MANAGER_ENABLE_CHARGEPUMP == 1 )
/******************************************************************************
 * @function RelayManager_ProcessChargePump
 *
 * @brief process the charge pump
 *
 * This function will process all charge pumps
 *
 * @param[in]   xArg      task argument
 *
 * @return      TRUE      flush event
 *
 *****************************************************************************/
BOOL RelayManager_ProcessChargePump( TASKARG xArg )
{
  RELAYCHARGEPUMPENUM eChargePump;
  GPIOPINENUM         eGpioEnum;

  // for each charge pump
  for ( eChargePump = 0; eChargePump < RELAY_CHARGE_PUMP_ENUM_MAX; eChargePump++ )
  {
    // get GPIO
    eGpioEnum = PGM_RDBYTE( atRelayChargePumpDefs[ eChargePump ].eGpioEnum );

    // check for active
    if ( anChargePumpCount[ eChargePump ] != 0 )
    {
      // toggle the charge pump
      Gpio_Toggle( eGpioEnum );
    }
    else
    {
      // force it low
      Gpio_Set( eGpioEnum, OFF );
    }
  }

  // always return true
  return( TRUE );
}
#endif // RELAY_MANAGER_ENABLE_CHARGEPUMP

/******************************************************************************
 * @function RelayManager_SetState
 *
 * @brief set a relay state
 *
 * This function will set the new relay state
 *
 * @param[in]   eRelay      relay enumeration
 * @param[in]   bState      relay state
 *
 * @return      appropriate error
 *
 *****************************************************************************/
RLYMGRERR RelayManager_SetState( RELAYENUM eRelay, BOOL bState )
{
  RLYMGRERR eError = RLYMGR_ERR_NONE;
  PRELAYCTL ptRlyCtl;
  
  if ( eRelay < RELAY_ENUM_MAX )
  {
    // get the control structure
    ptRlyCtl = &atRelayCtls[ eRelay ];
    
    // set the requested state
    ptRlyCtl->eReqState = ( bState ) ? RELAY_STATE_ON : RELAY_STATE_OFF; 
  }
  else
  {
    // set error
    eError = RLYMGR_ERR_ILLENUM;
  }

  // return the error
  return( eError );
}

/******************************************************************************
 * @function RelayManager_GetState
 *
 * @brief get relay state
 *
 * This function will return the relay state
 *
 * @param[in]   eRelay      relay enumeration
 * @param[in]   bState      relay state
 *
 * @return      appropriate error
 *
 *****************************************************************************/
RLYMGRERR RelayManager_GetState( RELAYENUM eRelay, PBOOL pbState )
{
  RLYMGRERR   eError = RLYMGR_ERR_NONE;
  PRELAYCTL   ptCtl;
  
  if ( eRelay < RELAY_ENUM_MAX )
  {
    // get the control structure
    ptCtl = &atRelayCtls[ eRelay ];
    
    // get the requested state
    *( pbState ) = ( ptCtl->eCurState == RELAY_STATE_OFF ) ? OFF : ON; 
  }
  else
  {
    // set error
    eError = RLYMGR_ERR_ILLENUM;
  }
  
  // return the error
  return( eError );
}

/******************************************************************************
 * @function RelayManager_GetDriveState
 *
 * @brief get relay state
 *
 * This function will return the relay state
 *
 * @param[in]   eRelay      relay enumeration
 * @param[in]   peState     pointer to the drive state
 *
 * @return      appropriate error
 *
 *****************************************************************************/
RLYMGRERR RelayManager_GetDriveState( RELAYENUM eRelay, PRELAYSTATE peState )
{
  RLYMGRERR   eError = RLYMGR_ERR_NONE;
  PRELAYCTL   ptCtl;
  
  if ( eRelay < RELAY_ENUM_MAX )
  {
    // get the control structure
    ptCtl = &atRelayCtls[ eRelay ];
    
    // get the requested state
    *( peState ) = ptCtl->eCurState; 
  }
  else
  {
    // set error
    eError = RLYMGR_ERR_ILLENUM;
  }
  
  // return the error
  return( eError );
}

#if ( RELAY_MANAGER_ENABLE_ZEROCROSS == 1 )
/******************************************************************************
 * @function RelayManager_ProcessZeroCross
 *
 * @brief process the zero corss event
 *
 * This function will clear the slot number to allow a restart of the zero 
 * cross slot number
 *
 *****************************************************************************/
void RelayManager_ProcessZeroCross( void )
{
  // reset the slot
  nSlotNumber = 0;
}
#endif // RELAY_MANAGER_ENABLE_ZEROCROSS 

/******************************************************************************
 * @function GenerateSlot
 *
 * @brief generate a slot
 *
 * This function will generate the next slot
 *
 * @return      slot number
 *
 *****************************************************************************/
static U8 GenerateSlot( void )
{
  U8  nSlot;
  
  // generate a random number
  nSlot = rand( ) % MAX_SLOT_NUMBER;
  return( nSlot );
}

/**@} EOF .c */
