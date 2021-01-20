/******************************************************************************
 * @file EepromHandler_cfg.c
 *
 * @brief EEPROM handler configuration implementation
 *
 * This file provoides the implementation for the EEPROM handler
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
 * \addtogroup EepromHandler
 * @{
 *****************************************************************************/

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "EepromHandler/EepromHandler.h"

// library includes -----------------------------------------------------------
#include "SystemTick/SystemTick.h"
#if ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_FREERTOS )
  // OS includes
  #include "FreeRtos.h"
#endif // SYSTEMDEFINE_OS_SELECTION

// Macros and Defines ---------------------------------------------------------
#if (( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_FREERTOS ) && ( EEPROMHANDLER_ENABLE_BACKGROUND_WRITES == 1 ))
  #define EEPROM_BKGNDWRITE_TASK_PRIORITY       ( tskIDLE_PRIORITY + 4 )
#endif // SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_FREERTOS

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations ----------------------------------------------

// local parameter declarations -----------------------------------------------

// local function prototypes --------------------------------------------------
#if (( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_FREERTOS ) && ( EEPROMHANDLER_ENABLE_BACKGROUND_WRITES == 1 ))
  static  void  BackGroundWriteTask( PVOID pvParameters );
#endif // SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_FREERTOS

// constant parameter initializations -----------------------------------------

/******************************************************************************
 * @function EepromHandler_LclInitialize
 *
 * @brief EEPROM handler initialization
 *
 * This function will initialize the EEPROM handler
 *
 *****************************************************************************/
void EepromHandler_LclInitialize( void )
{
  #if ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_FREERTOS )
    // create the background write task
    xTaskCreate( BackGroundWriteTask, "EEPROM BKGND Write", configMINIMAL_STACK_SIZE, NULL, EEPROM_BKGNDWRITE_TASK_PRIORITY, NULL );
  #endif // SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_FREERTOS
}

#if ( EEPROMHANDLER_ENABLE_BACKGROUND_WRITES == 1 )
#if ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_TASKMANAGER )
  /******************************************************************************
   * @function EepromHandler_ProcessBackgroundWrite
   *
   * @brief process the background  writ etask
   *
   * This function will call the base background write task
   *
   * @param[in]   xArg          task argument
   *
   * @return      TRUE to flush event
   *
   *****************************************************************************/
  BOOL EepromHandler_ProcessBackgroundWrite( TASKARG xArg )
  {
    // call the function
    EepromHandler_BlockWriteEvent( );  
  
    // return true
    return( TRUE );
  }
#elif ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_FREERTOS )
  /******************************************************************************
   * @function BackGroundWriteTask
   *
   * @brief background write task
   *
   * This function calls the background write function
   *
   * @param[in]   pvParameters  not used
   *
   *****************************************************************************/
  static void BackGroundWriteTask( PVOID pvParameters )
  {
    // main loop
    FOREVER
    {
      // call the default handler
      EepromHandler_BlockWriteEvent( );

      // sleep for the animate rate
      vTaskDelay( EEPROMHANDLER_PAGE_WRITE_MSECS / portTICK_RATE_MS );
    }
  }
#endif // SYSTEMDEFINE_OS_SELECTION
#endif // EEPROMHANDLER_ENABLE_BACKGROUND_WRITES

/******************************************************************************
 * @function EepromHandler_GetSystemTime
 *
 * @brief get system time
 *
 * This function will initialize the return the system time
 *
 *****************************************************************************/
U32 EepromHandler_GetSystemTime( void )
{
  U32 uTime = 0;

  // get the system time
  uTime = SystemTick_GetTimeMsec( );

  // return the time
  return( uTime );
}

/******************************************************************************
 * @function EepromHandler_LclRdBlock
 *
 * @brief read a block of data from the eeprom
 *
 * This function will read a block of data from the eeprom
 *
 * @param[in]   wAddress    address to read from
 * @param[in]   wLength     length to read
 * @param[i0]   pnData      pointer to the data storage
 *
 * @return      TRUE if errors, FALSE if OK
 *
 *****************************************************************************/
BOOL EepromHandler_LclRdBlock( U16 wAddress, U16 wLength, PU8 pnData )
{
  BOOL      bStatus;
  I2CXFRCTL tXfrCtl;

  // set the address
  tXfrCtl.tAddress.uValue = wAddress;
  tXfrCtl.nAddrLen = EEPROMHANDLER_ADR_SIZE;

  // set the the device address
  tXfrCtl.nDevAddr = EEPROMHANDLER_DEV_ADDR;

  // calculate the read timeout
  tXfrCtl.uTimeout = RDBLOCK_MAX_TIMEOUT + COMPUTE_BLOCK_TIME( wLength );

  // set the pointer to the data/length
  tXfrCtl.pnData = pnData;
  tXfrCtl.wDataLen = wLength;

  // perform read/write
  bStatus = ( I2c_Read( EEPROMHANDLER_I2C_ENUM, &tXfrCtl ) == I2C_ERR_NONE ) ? FALSE : TRUE;

  // return the status
  return( bStatus );
}

/******************************************************************************
 * @function EepromHandler_LclRdBlock
 *
 * @brief read a block of data from the eeprom
 *
 * This function will read a block of data from the eeprom
 *
 * @param[in]   wAddress    address to read from
 * @param[in]   wLength     length to read
 * @param[i0]   pnData      pointer to the data storage
 *
 * @return      TRUE if errors, FALSE if OK
 *
 *****************************************************************************/
BOOL EepromHandler_LclWrBlock( U16 wAddress, U16 wLength, PU8 pnData )
{
  BOOL      bStatus;
  I2CXFRCTL tXfrCtl;

  // set the address
  tXfrCtl.tAddress.uValue = wAddress;
  tXfrCtl.nAddrLen = EEPROMHANDLER_ADR_SIZE;

  // set the the device address
  tXfrCtl.nDevAddr = EEPROMHANDLER_DEV_ADDR;

  // set the pointer to the data/length
  tXfrCtl.pnData = pnData;
  tXfrCtl.wDataLen = wLength;

  // calculate the read timeout
  tXfrCtl.uTimeout = WRBLOCK_MAX_TIMEOUT + COMPUTE_BLOCK_TIME( wLength + EEPROMHANDLER_ADR_SIZE );

  // perform write
  bStatus = ( I2c_Write( EEPROMHANDLER_I2C_ENUM, &tXfrCtl ) == I2C_ERR_NONE ) ? FALSE : TRUE;

  // return the status
  return( bStatus );
}

/******************************************************************************
 * @function EepromHandler_LclCheckBusy
 *
 * @brief checks to see if the EEPROM is busy executing the last requested function
 *
 * This function will check for busy
 *
 * @return      TRUE if busy, FALSE if not
 *
 *****************************************************************************/
BOOL EepromHandler_LclCheckBusy( void )
{
  BOOL bBusy;
  I2CCHKBSY tChkBusy;

  // check for device presence
  tChkBusy.nDevAddr = EEPROMHANDLER_DEV_ADDR;
  tChkBusy.bReadMode = EEPROMHANDLER_I2C_POLL_MODE;
  if ( I2c_Ioctl( EEPROMHANDLER_I2C_ENUM, I2C_ACTION_POLL_DEVICE, ( PVOID )&tChkBusy ) == I2C_ERR_NONE )
  {
    // clear busy
    bBusy = FALSE;
  }
  else
  {
    // set busy
    bBusy = TRUE;
  }

  // return the busy status
  return( bBusy );
}

/**@} EOF EepromHandler_cfg.c */
