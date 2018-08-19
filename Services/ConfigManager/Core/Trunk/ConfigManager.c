/******************************************************************************
 * @file ConfigManager.c
 *
 * @brief configuration manager implementation
 *
 * This file implements the configuration manager
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
 *
 * \addtogroup ConfigManager
 * @{
 *****************************************************************************/

// system includes ------------------------------------------------------------

// library includes -----------------------------------------------------------
#include "CRC16/CRC16.h"
#ifndef __ATMEL_AVR__
#include "EepromHandler/EepromHandler.h"
#endif // __ATMEL_AVR__

// local includes -------------------------------------------------------------
#include "ConfigManager/ConfigManager.h"
#include "ConfigManager/ConfigManager_prm.h"
#if ( CONFIGMANAGER_ENABLE_LOGEVENTS == 1 )
#include "LogHandler/LogHandler.h"
#endif

// Macros and Defines ---------------------------------------------------------
#ifndef __ATMEL_AVR__
/// define the address of the CRC
#define CFGBLK_CRC_ADDR         ( EEPROMHANDLER_CFGBLOCK_BASE_ADDR )

/// define the address of the config block
#define CFGBLK_DATA_ADDR        ( CFGBLK_CRC_ADDR + sizeof( U16 ))
#endif

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations ----------------------------------------------

// local parameter declarations -----------------------------------------------

// local function prototypes --------------------------------------------------

// constant parameter initializations -----------------------------------------

/******************************************************************************
 * @function ConfigManager_Initialize
 *
 * @brief initialize the configuration manager
 *
 * This function will check for a valid configuration, copy the defaults if
 * neccesary and update the CRC's
 *
 *****************************************************************************/
void ConfigManager_Initialize( void )
{
  U16               wCalcCrc, wActCrc, wSize;
  CONFIGTYPE        eCfgBlkIdx;
  PCONFIGMGRBLKDEF  ptDef;
  PVGETPOINTER      pvGetPointerActual;
  PU8               pnActual;
  #ifdef __ATMEL_AVR__
  PU16              puEepromAddress;
  #else
  U16               wBase;
  #endif //__ATMEL_AVR__
  
  // perform any local initialization
  ConfigManager_LocalInitialize( );

  // get the stored CRC
  #ifdef __ATMEL_AVR__ 
  wActCrc = EEP_RDWORD( wConfigCheck );
  #else
  EepromHandler_RdWord( CFGBLK_CRC_ADDR, &wActCrc );
  #endif //__ATMEL_AVR__

  // get the actual CRC
  wCalcCrc = ConfigManager_ComputeBlockCrc( CONFIG_SOURCE_EEPROM );

  // are they the same
  if ( wActCrc != wCalcCrc )
  {
    // reset to defaults
    ConfigManager_ResetDefaults( );
  }
  else
  {
    // now copy the EEPROM to the local storage
    #ifndef __ATMEL_AVR__
    wBase = EEPROMHANDLER_CFGBLOCK_BASE_ADDR + sizeof( U16 );
    #endif //__ATMEL_AVR__
    
    // for each block copy the data to EEPROM
    for ( eCfgBlkIdx = 0; eCfgBlkIdx < CONFIG_TYPE_MAX; eCfgBlkIdx++ )
    {
      // get the definition
      ptDef = ( PCONFIGMGRBLKDEF )&atConfigDefs[ eCfgBlkIdx ];
      
      // now get a pointer to the actual/size
      pvGetPointerActual = ( PVOID )PGM_RDWORD( ptDef->pvGetActual );
      pnActual = ( PU8 )pvGetPointerActual( );
      wSize = PGM_RDWORD( ptDef->wSize );
      
      // now read the data from the EEPROM
      #ifdef __ATMEL_AVR__ 
      puEepromAddress = ( PU16 )PGM_RDWORD( ptDef->pwEepromAddr );
      EEP_RDBLOCK_PTR( pnActual, puEepromAddress, wSize );
      #else
      EepromHandler_RdBlock( wBase, wSize, pnActual );
      #endif //__ATMEL_AVR__
      
      #ifndef __ATMEL_AVR__
      // adjust the size
      wBase += wSize;
      #endif //__ATMEL_AVR__
    }
  }

  // post the config done event
  #if ( CONFIGMANAGER_ENABLE_NOTIFICATIONS == 1)
  TaskManager_PostEvent( CONFIGMANGER_REPORTTING_TASK, SYSCTRLMNGR_EVENT_CONFIGDONE );
  #endif // CONFIGMANAGER_ENABLE_NOTIFICATIONS
}

/******************************************************************************
 * @function ConfigManager_ResetDefaults
 *
 * @brief reset defaults
 *
 * This function will reset all configuration blocks to their defaults
 *
 * @return    return FALSE if no errors, TRUE if errors found
 *
 *****************************************************************************/
BOOL ConfigManager_ResetDefaults( void )
{
  U16               wSize;
  CONFIGTYPE        eCfgBlkIdx;
  PCONFIGMGRBLKDEF  ptDef;
  PVGETPOINTER      pvGetPointer;
  PU8               pnActual, pnDefault;
  BOOL              bStatus = FALSE;
  #ifdef __ATMEL_AVR__
  PU16              puEepromAddress;
  #else
  U16               wBase;
  #endif //__ATMEL_AVR__

  // set the base address for the config blocks  
  #ifndef __ATMEL_AVR__
  wBase = CFGBLK_DATA_ADDR;
  #endif //__ATMEL_AVR__
  
  // for each block copy the data to EEPROM
  for ( eCfgBlkIdx = 0; eCfgBlkIdx < CONFIG_TYPE_MAX; eCfgBlkIdx++ )
  {
    // get the definition
    ptDef = ( PCONFIGMGRBLKDEF )&atConfigDefs[ eCfgBlkIdx ];
    
    // now get a pointer to the actual/defaults/size
    pvGetPointer = ( PVOID )PGM_RDWORD( ptDef->pvGetActual );
    pnActual = ( PU8 )pvGetPointer( );
    pvGetPointer = ( PVOID )PGM_RDWORD( ptDef->pvGetDefault );
    pnDefault = ( PU8 )pvGetPointer( );
    wSize = PGM_RDWORD( ptDef->wSize );
    
    // now copy the default to the actual
    MEMCPY_P( pnActual, pnDefault, wSize );

    // now write the data to the EEPROM
    #ifdef __ATMEL_AVR__ 
    puEepromAddress = ( PU16 )PGM_RDWORD( ptDef->pwEepromAddr );
    EEP_WRBLOCK_PTR( pnActual, puEepromAddress, wSize );
    #else
    if ( EepromHandler_WrBlock( wBase, wSize, pnActual ) != EEPROM_ERR_NONE )
    {
      // set the error flag/exit loop
      bStatus = TRUE;
      break;
    }
    #endif //__ATMEL_AVR__
    
    #ifndef __ATMEL_AVR__
    // adjust the size
    wBase += wSize;
    #endif //__ATMEL_AVR__
  }
  
  // if no error
  if ( bStatus == FALSE )
  {
    // update the CRC
    ConfigManager_UpdateCRC( CONFIG_SOURCE_EEPROM );
  }

  // test for log handler
  #if ( CONFIGMANAGER_ENABLE_LOGEVENTS == 1 )
  LogHandler_AddEntry( LOG_TYPE_CFGRST, wCrc, bStatus );
  #endif

  // return the status
  return( bStatus );
}

/******************************************************************************
 * @function ConfigManager_GetSignature
 *
 * @brief get the config blocks signature
 *
 * This function will read the config blocks signature
 *
 * @return    the current signature of the config blok  
 *
 *****************************************************************************/
U16 ConfigManager_GetSignature( void )
{
  U16         wActCrc;
  
  // get the stored CRC
  #ifdef __ATMEL_AVR__ 
  wActCrc = EEP_RDWORD( wConfigCheck );
  #else
  EepromHandler_RdWord( CFGBLK_CRC_ADDR, &wActCrc );
  #endif //__ATMEL_AVR__

  // return the actual CRC
  return( wActCrc );
}

/******************************************************************************
 * @function ConfigManager_UpdateCRC
 *
 * @brief update the CRC of the entire block
 *
 * This function calculates the CRC of the block and returns the CRC
 *
 * @param[in]   eSource   source to calculate the CRC on
 *
 * @return      the current CRC
 *
 *****************************************************************************/
U16 ConfigManager_UpdateCRC( CONFIGSOURCE eSource )
{
  U16 wCalcCrc;

  // calculate the CRC block
  wCalcCrc = ConfigManager_ComputeBlockCrc( eSource );
  #ifdef __ATMEL_AVR__ 
  EEP_WRWORD( wConfigCheck, wCalcCrc );
  #else
  EepromHandler_WrWord( CFGBLK_CRC_ADDR, wCalcCrc );
  #endif //__ATMEL_AVR__

  // return the CRC
  return( wCalcCrc );
}

/******************************************************************************
 * @function ConfigManager_ComputeBlockCrc
 *
 * @brief compute the CRC of the block
 *
 * This function computes the CRC of the defined configuration block (EEPROM/RAM)
 *
 * @param[in]   eSource   source to compute the CRC on
 *
 * @return      the CRC of the block
 *
 *****************************************************************************/
U16 ConfigManager_ComputeBlockCrc( CONFIGSOURCE eSource )
{
  U16               wCrc, wBlkDataIdx, wSize;
  CONFIGTYPE        eCfgBlkIdx;
  PCONFIGMGRBLKDEF  ptDef;
  PVGETPOINTER      pvGetPointer;
  PU8               pnActData = NULL;
  #ifdef __ATMEL_AVR__
  PU16              puEepromAddress;
  #else
  U16               wBase;
  #endif //__ATMEL_AVR__

  // get the initial value
  wCrc = CRC16_GetInitialValue( );
  
  // set the base address for the config blocks
  #ifndef __ATMEL_AVR__
  wBase = CFGBLK_DATA_ADDR;
  #endif //__ATMEL_AVR__
  
  // for each config block
  for( eCfgBlkIdx = 0; eCfgBlkIdx < CONFIG_TYPE_MAX; eCfgBlkIdx++ )
  {
    // get the definition
    ptDef = ( PCONFIGMGRBLKDEF )&atConfigDefs[ eCfgBlkIdx ];
    pvGetPointer = ( PVOID )PGM_RDWORD( ptDef->pvGetActual );
    
    // now get the size
    wSize = PGM_RDWORD( ptDef->wSize );

    // determine source
    switch( eSource )
    {
      case CONFIG_SOURCE_EEPROM :
        // allocate temporary space for the config block
        pnActData = malloc( wSize );

        // read a byte from EEPROM
        #ifdef __ATMEL_AVR__ 
        puEepromAddress = ( PU16 )PGM_RDWORD( ptDef->pwEepromAddr );
        EEP_RDBLOCK_PTR( pnActData, puEepromAddress, wSize );
        #else
        EepromHandler_RdBlock( wBase, wSize, pnActData );
        #endif //__ATMEL_AVR__
        break;
        
      case CONFIG_SOURCE_RAM :
        // get a pointer to the actual data
        pnActData = ( PU8 )pvGetPointer( );
        break;
        
      default :
        break;
    }

    // now for each byte 
    for ( wBlkDataIdx = 0; wBlkDataIdx < wSize; wBlkDataIdx++ )
    {
      // now add to crc
      wCrc = CRC16_CalculateByte( wCrc, *( pnActData + wBlkDataIdx ));
    }

    // if data block was malloced (source == EEPROM )
    if (( eSource == CONFIG_SOURCE_EEPROM ) && ( pnActData != NULL ))
    {
      // free the pointer
      free( pnActData );
    }
    
    #ifndef __ATMEL_AVR__
    // adjust the size
    wBase += wSize;
    #endif //__ATMEL_AVR__
  }
  
  // return the CRC
  return( wCrc );
}

/******************************************************************************
 * @function ConfigManager_UpdateConfig
 *
 * @brief update the configuration block
 *
 * This function writes the updated block to EEPROM
 *
 * @param[in]   eConfigType   configuration type
 *
 *****************************************************************************/
void ConfigManager_UpdateConfig( CONFIGTYPE eConfigType )
{
  PU8               pnActData;
  PCONFIGMGRBLKDEF  ptDef;
  PVGETPOINTER      pvGetPointer;
  U16               wSize;
  BOOL              bWriteOk = FALSE;
  #ifdef __ATMEL_AVR__
  PU16              puEepromAddress;
  #else
  U16               wBase;
  CONFIGTYPE        eCfgType;
  #endif //__ATMEL_AVR__

  // get the block
  ptDef = ( PCONFIGMGRBLKDEF )&atConfigDefs[ eConfigType ];

  // get the pointer to the data
  pvGetPointer = ( PVOID )PGM_RDWORD( ptDef->pvGetActual );
  pnActData = ( PU8 )pvGetPointer( );

  // get the size
  wSize = PGM_RDWORD( ptDef->wSize );

  // calculate the address
  #ifndef __ATMEL_AVR__
  wBase = CFGBLK_DATA_ADDR;
  for ( eCfgType = 0; eCfgType < eConfigType; eCfgType++ )
  {
    // read the size
    ptDef = ( PCONFIGMGRBLKDEF )&atConfigDefs[ eCfgType ];

    // add the size
    wBase += PGM_RDWORD( ptDef->wSize );
  }
  #endif //__ATMEL_AVR__

  // now write the EEPROM
  #ifdef __ATMEL_AVR__ 
  puEepromAddress = ( PU16 )PGM_RDWORD( ptDef->pwEepromAddr );
  EEP_WRBLOCK_PTR( pnActData, puEepromAddress, wSize );
  bWriteOk = TRUE;
  #else
  if ( EepromHandler_WrBlock( wBase, wSize, pnActData ) == EEPROM_ERR_NONE )
  {
    // set the write OK
    bWriteOk = TRUE;
  }
  #endif //__ATMEL_AVR__
    
  // update the CRC
  if ( bWriteOk )
  {
    // now update the CRC
    ConfigManager_UpdateCRC( CONFIG_SOURCE_EEPROM );
  }

  // log event
  #if ( CONFIGMANAGER_ENABLE_LOGEVENTS == 1 )
  LogHandler_AddEntry( LOG_TYPE_CFGUPD, xArg, 0 );
  #endif
}

/**@} EOF ConfigManager.c */
