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

// local includes -------------------------------------------------------------
#include "ConfigManager/ConfigManager.h"

// library includes -----------------------------------------------------------
#include "CRC16/CRC16.h"
#include "EepromHandler/EepromHandler.h"
#if ( CONFIGMANAGER_ENABLE_LOGEVENTS == 1 )
#include "LogHandler/LogHandler.h"
#endif
#include "ManufInfo/ManufInfo.h"

// Macros and Defines ---------------------------------------------------------
/// define the address of the CRC
#define CFGBLK_CHCK_ADDR         ( EEPROMHANDLER_CFGBLOCK_BASE_ADDR )

/// define the address of the version block
#define CFGBLK_VERS_ADDR         ( CFGBLK_CHCK_ADDR + sizeof( U16 ))

/// define the address of the config block
#define CFGBLK_DATA_ADDR        ( CFGBLK_VERS_ADDR + sizeof( U16 ))

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations ----------------------------------------------

// local parameter declarations -----------------------------------------------

// local function prototypes --------------------------------------------------
/// command handlers
#if ( CONFIGMANAGER_ENABLE_DEBUG_COMMANDS == 1 )
static  ASCCMDSTS CmdRstCfg( U8 nCmdEnum );

// constant parameter initializations -----------------------------------------
/// declare the command strings
static  const CODE C8 szRstCfg[ ]   = { "RSTCFG" };

/// initialize the command table
const CODE ASCCMDENTRY g_atConfigManagerCmdHandlerTable[ ] =
{
  ASCCMD_ENTRY( szRstCfg, 4, 1, ASCFLAG_COMPARE_NONE, CONFIGMANAGER_RESET_SYSTEM_MODE, CmdRstCfg ),

  // the entry below must be here
  ASCCMD_ENDTBL( )
};
#endif  // CONFIGMANAGER_ENABLE_DEBUG_COMMANDS

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
  U16               wCalcCrc, wActCrc, wSize, wActVersion;
  CONFIGTYPE        eCfgBlkIdx;
  PCONFIGMGRBLKDEF  ptDef;
  PVGETPOINTER      pvGetPointerActual;
  PU8               pnActual;
  U16               wBase;
  U16UN             tExpVersion;
  
  // perform any local initialization
  ConfigManager_LocalInitialize( );

  // get the expectant version
  tExpVersion.anValue[ LE_U16_MSB_IDX ] = ManufInfo_GetSfwMajor( );
  tExpVersion.anValue[ LE_U16_LSB_IDX ] = ManufInfo_GetSfwMinor( );

  // get the stored CRC/version
  EepromHandler_RdWord( CFGBLK_CHCK_ADDR, &wActCrc );
  EepromHandler_RdWord( CFGBLK_VERS_ADDR, &wActVersion );
  
  // get the actual CRC
  wCalcCrc = ConfigManager_ComputeBlockCrc( CONFIG_SOURCE_EEPROM );

  // are they the same
  if (( wActCrc != wCalcCrc ) || ( wActVersion != tExpVersion.wValue ))
  {
    // reset to defaults
    ConfigManager_ResetDefaults( );
  }

  // now copy the EEPROM to the local storage
  wBase = CFGBLK_DATA_ADDR;
    
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
    EepromHandler_RdBlock( wBase, wSize, pnActual );
      
    // adjust the size
    wBase += wSize;
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
  U16               wBase;
  U16UN             tVersion;

  // get the expectant version
  tVersion.anValue[ LE_U16_MSB_IDX ] = ManufInfo_GetSfwMajor( );
  tVersion.anValue[ LE_U16_LSB_IDX ] = ManufInfo_GetSfwMinor( );
  
  // write it
  EepromHandler_WrWord( CFGBLK_VERS_ADDR, tVersion.wValue );

  // set the base address for the config blocks  
  wBase = CFGBLK_DATA_ADDR;

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
    if ( EepromHandler_WrBlock( wBase, wSize, pnActual ) != EEPROM_ERR_NONE )
    {
      // set the error flag/exit loop
      bStatus = TRUE;
      break;
    }
    
    // adjust the size
    wBase += wSize;
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
  EepromHandler_RdWord( CFGBLK_CHCK_ADDR, &wActCrc );

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
  EepromHandler_WrWord( CFGBLK_CHCK_ADDR, wCalcCrc );

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
  U16               wBase;

  // get the initial value
  wCrc = CRC16_GetInitialValue( );
  
  // set the base address for the config blocks
  wBase = CFGBLK_DATA_ADDR;
  
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
        EepromHandler_RdBlock( wBase, wSize, pnActData );
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
    
    // adjust the size
    wBase += wSize;
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
  U16               wBase;
  CONFIGTYPE        eCfgType;

  // get the block
  ptDef = ( PCONFIGMGRBLKDEF )&atConfigDefs[ eConfigType ];

  // get the pointer to the data
  pvGetPointer = ( PVOID )PGM_RDWORD( ptDef->pvGetActual );
  pnActData = ( PU8 )pvGetPointer( );

  // get the size
  wSize = PGM_RDWORD( ptDef->wSize );

  // calculate the address
  wBase = CFGBLK_DATA_ADDR;
  for ( eCfgType = 0; eCfgType < eConfigType; eCfgType++ )
  {
    // read the size
    ptDef = ( PCONFIGMGRBLKDEF )&atConfigDefs[ eCfgType ];

    // add the size
    wBase += PGM_RDWORD( ptDef->wSize );
  }

  // now write the EEPROM
  if ( EepromHandler_WrBlock( wBase, wSize, pnActData ) == EEPROM_ERR_NONE )
  {
    // set the write OK
    bWriteOk = TRUE;
  }
    
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

#if ( CONFIGMANAGER_ENABLE_DEBUG_COMMANDS == 1 )
/******************************************************************************
 * @function CmdRstCfg
 *
 * @brief reset to defaults
 *
 * This function will reset the parameters to their default state
 *
 * @param[in]   nCmdEnum      command handler enumeration
 *
 * @return      appropriate eerror
 *
 *****************************************************************************/
static ASCCMDSTS CmdRstCfg( U8 nCmdEnum )
{
  ASCCMDSTS eStatus = ASCCMD_STS_NONE;
  PC8       pszPassWord;

  // get the password
  AsciiCommandHandler_GetArg( nCmdEnum, 0, &pszPassWord );

  // now compare to password
  if ( STRCMP_P( CONFIGMANAGER_RESET_DEFAULTS_PASSWORD, pszPassWord ) == 0 )
  {
    // reset the parameters
    ConfigManager_ResetDefaults( );
  }
  else
  {
    // send the error
    eStatus = ASCCMD_STS_ILLPASSWORD;
  }

  // return the status
  return( eStatus );
}
#endif  // CONFIGMANAGER_ENABLE_DEBUG_COMMANDS

/**@} EOF ConfigManager.c */
