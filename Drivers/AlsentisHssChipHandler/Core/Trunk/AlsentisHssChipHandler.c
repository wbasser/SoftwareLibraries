/******************************************************************************
 * @file AlsentisChipHandler.c
 *
 * @brief Alsentis HSS chip handler implementation 
 *
 * This file provides the implementation for the driver that interfaces
 * with an Alsentis HSS touch controller
 *
 * @copyright Copyright (c) 2012 Nan Technologies
 * This document contains proprietary data and information of Nan Technologies 
 * LLC. It is the exclusive property of Nan Technologies, LLC and will not be 
 * disclosed in any form to any party without prior written permission of 
 * Nan Technologies, LLC. This document may not be reproduced or further used 
 * without the prior written permission of Nan Technologies, LLC.
 *
 * $Date: $
 *
 * Version History
 * ======
 * $Rev: $
 * 
 *
 * \addtogroup AlsentisChipHandler
 * @{
 *****************************************************************************/

// system includes ------------------------------------------------------------

// library includes -----------------------------------------------------------

// local includes -------------------------------------------------------------
#include "AlsentisHssChipHandler/AlsentisHssChipHandler.h"
#include "AlsentisHssChipHandler/AlsentisHssChipHandler_prm.h"

// Macros and Defines ---------------------------------------------------------
#define ALSENTIS_HSS_TOUCH_CMD_RDSENSORS          ( 0x9F )

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------
/// define the local control structure
typedef struct _LCLCTL
{
  BOOL  bValidSensors;          ///< valid sensors
  U32   uSensorStates;          ///< sensor states
} LCLCTL, *PLCLCTL;
#define LCLCTL_SIZE           sizeof( LCLCTL )

// global parameter declarations ----------------------------------------------

// local parameter declarations -----------------------------------------------
static  LCLCTL atLclCtl[ ALSENTISHSSCHIP_DEV_ENUM_NAX ];

// local function prototypes --------------------------------------------------
static  void  ReadSensorStates( ALENTISHSSDCHIPEVENUM eChipEnum );

// constant parameter initializations -----------------------------------------

/******************************************************************************
 * @function AlsentisHssChipHandler_Initialize
 *
 * @brief intialization
 *
 * This function will initialize the chip handler
 *
 *****************************************************************************/
void AlsentisHssChipHandler_Initialize( void )
{
  // clear the current sensor value
  memset( &atLclCtl, 0, ( ALSENTISHSSCHIP_DEV_ENUM_NAX * LCLCTL_SIZE ));
}

/******************************************************************************
 * @function AlsentisHssChipHandler_ProcessIrq
 *
 * @brief process an interrupt request
 *
 * This function will process an interrupt request
 *
 * @param[in]   eIrqEnum    GPIO pin enum
 * @param[in]   eEvent      GPIO interrupt event
 *
 *****************************************************************************/
void AlsentisHssChipHandler_ProcessIrq( GPIOPINENUM eIrqEnum, GPIOIRQCBEVENT eEvent )
{
  TASKARG xArg;
  
  // build the argument
  xArg = eIrqEnum << 8;
  xArg |= eEvent;
  
  // post the event
  TaskManager_PostEvent( ALSENTISHSSCHIPHANDLER_SCHD_ENUM, xArg );
}

/******************************************************************************
 * @function AlsentisHssChipHandler_ProcessEvent
 *
 * @brief process an event
 *
 * This function will process the task event
 *
 * @param[in]   xArg      event argument
 *
 * @return      TRUE      flush the event 
 *
 *****************************************************************************/
BOOL AlsentisHssChipHandler_ProcessEvent( TASKARG xArg )
{
  GPIOPINENUM           eGpioIrq;
  GPIOIRQCBEVENT        eEvent;
  PALSENTISHSSCHIPDEF   ptDef;
  ALENTISHSSDCHIPEVENUM eChipEnum;
  
  // process the event
  eGpioIrq = HI16( xArg );
  eEvent = LO16( xArg );
  
  // only process rising edge
  if ( eEvent == GPIO_IRQCB_EVENT_RISE )
  {
    // for each enumeration
    for ( eChipEnum = 0; eChipEnum < ALSENTISHSSCHIP_DEV_ENUM_NAX; eChipEnum++ )
    {
      // set the chip definition
      ptDef = ( PALSENTISHSSCHIPDEF )&atAlsentisHssChipDevs[ eChipEnum ];

      // is this our channel
      if ( PGM_RDBYTE( ptDef->eGpioIrq ) == eGpioIrq )
      {
        // read the sensors
        ReadSensorStates( eChipEnum );
      }
    }
  }

  // always return true
  return( TRUE );
}

/******************************************************************************
 * @function AlsentisHssChipHandler_GetSensors
 *
 * @brief get the sensors state
 *
 * This function will return the state of all sensors
 *
 * @param[in]   eChipEnum     chip enumeration
 * @param[io]   puSensors      pointer to the storage for the sensor status         
 *
 * @return      an appropriate error
 *
 *****************************************************************************/
ALSENTISCHIPERR AlsentisHssChipHandler_GetSensors( ALENTISHSSDCHIPEVENUM eChipEnum, PU32 puSensors )
{
  ALSENTISCHIPERR eError = ALSENTIS_CHIPERR_NONE;
  PLCLCTL               ptCtl;
  
  // valid chip
  if ( eChipEnum < ALSENTISHSSCHIP_DEV_ENUM_NAX )
  {
    // set the chip definition/control pointers
    ptCtl = &atLclCtl[ eChipEnum ];

    // check for valid
    if ( ptCtl->bValidSensors )
    {
      // set the return value
      *( puSensors ) = ptCtl->uSensorStates;
    }
    else
    {
      // set the error
      eError = ALSENTIS_CHIPERR_NOTVALID; 
    }
  }
  else
  {
    // return the error
    eError = ALSENTIS_CHIPERR_ILLENUM;
  }
  
  // return the error
  return( eError );
}

/******************************************************************************
 * @function AlsentisHssChipHandler_GetSensorStatus
 *
 * @brief gets a single sensor status
 *
 * This function will return the status of a single sensor
 *
 * @param[in]   eChipEnum     chip enumeration
 * @param[in]   nSensorIdx    sensor index
 * @param[io]   pbState       pointer to store the current state of the sensor
 *
 * @return      an appropriate error
 *
 *****************************************************************************/
ALSENTISCHIPERR AlsentisHssChipHandler_GetSensorStatus( ALENTISHSSDCHIPEVENUM eChipEnum, U8 nSensorIdx, PBOOL pbState )
{
  ALSENTISCHIPERR       eError = ALSENTIS_CHIPERR_NONE;
  PALSENTISHSSCHIPDEF   ptDef;
  PLCLCTL               ptCtl;
  
  // valid chip
  if ( eChipEnum < ALSENTISHSSCHIP_DEV_ENUM_NAX )
  {
    // set the chip definition/control pointers
    ptDef = ( PALSENTISHSSCHIPDEF )&atAlsentisHssChipDevs[ eChipEnum ];
    ptCtl = &atLclCtl[ eChipEnum ];
    
    // test for valid
    if ( ptCtl->bValidSensors )
    {
      // now test for valid sensor index
      if ( nSensorIdx < PGM_RDBYTE( ptDef->nNumSensors ))
      {
        // now get the sensor value
        *( pbState ) = ( ptCtl->uSensorStates & BIT( nSensorIdx )) ? TRUE : FALSE;
      }
      else
      {
        // illegal sensor index
        eError = ALSENTIS_CHIPERR_ILLSENSOR;
      }
    }
    else
    {
      // set the error
      eError = ALSENTIS_CHIPERR_NOTVALID; 
    }
  }
  else
  {
    // return the error
    eError = ALSENTIS_CHIPERR_ILLENUM;
  }
  
  // return the error
  return( eError );
}

/******************************************************************************
 * @function ReadSensorStates
 *
 * @brief reads the sensor states
 *
 * This function will return the sensor states
 *
 * @param[in]   eChipEnum     chip enumeration
 *
 *****************************************************************************/
static void ReadSensorStates( ALENTISHSSDCHIPEVENUM eChipEnum )
{
  PALSENTISHSSCHIPDEF   ptDef;
  PLCLCTL               ptCtl;
  I2CXFRCTL             tXfrCtl;
  U8                    nNumberSensors;

  // get the definition/control pointers
  ptDef = ( PALSENTISHSSCHIPDEF )&atAlsentisHssChipDevs[ eChipEnum ];
  ptCtl = &atLclCtl[ eChipEnum ];
  
  // set the address/command in the address
  tXfrCtl.nAddrLen = 2;
  tXfrCtl.tAddress.anValue[ LE_U32_LSB_IDX ] = ALSENTIS_HSS_TOUCH_CMD_RDSENSORS;
  tXfrCtl.tAddress.anValue[ LE_U32_MS2_IDX ] = ALSENTIS_HSS_TOUCH_CMD_RDSENSORS;

  // set the device address 
  tXfrCtl.nDevAddr = PGM_RDBYTE( ptDef->nAddress );
  
  // set the transfer length/data pointer
  nNumberSensors = PGM_RDBYTE( ptDef->nNumSensors );
  tXfrCtl.wDataLen = ( nNumberSensors / 8 ) + 1;
  tXfrCtl.pnData = ( PU8 )&ptCtl->uSensorStates;
  
  // set the timeout
  tXfrCtl.uTimeout = 100;
  
  // now issue the read
  if ( I2c_Read( PGM_RDBYTE( ptDef->eI2cDevEnum ), &tXfrCtl ) != I2C_ERROR_NONE )
  {
    // clear the sensor value
    ptCtl->uSensorStates = 0;
    ptCtl->bValidSensors = FALSE;
  }
  else
  {
    // set the valid sensor flag
    ptCtl->bValidSensors = TRUE;
  }
}

/**@} EOF AlsentisChipHandler.c */
