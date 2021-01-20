/******************************************************************************
 * @file ParameterManager_cfg.c
 *
 * @brief parameter manager configuration implementation
 *
 * This file provides the implementation for the configuraitn of the paramete
 * manager
 *
 * @copyright Copyright (c) 2012 CyberIntegration
 * This document contains proprietary data and information of CyberIntegration
 * LLC. It is the exclusive property of CyberIntegration, LLC and will not be
 * disclosed in any form to any party without prior written permission of 
 * CyberIntegration, LLC. This document may not be reproduced or further used
 * without the prior written permission of CyberIntegration, LLC.
 *
 * Version History
 * ======
 * $Rev: $
 * 
 *
 * \addtogroup 
 * @{
 *****************************************************************************/

// local includes -------------------------------------------------------------
#include "ParameterManager\ParameterManager_cfg.h"
#include "ParameterManager\ParameterManager.h"
#if ( PARAM_ENABLE_CONFIGVER == OFF )
  #include "ManufInfo/ManufInfo.h"
#endif // PARAM_ENABLE_CONFIGVER

// library includes -----------------------------------------------------------


// Macros and Defines ---------------------------------------------------------

/// define the memory block write enable code

// constant parameter initializations -----------------------------------------
const CODE  PARAMDFLTS  atParamDefaults[ PARAMSEL_MAX_NUM ] =
{
  // populate the table with one of the below macros
  // PARAM_ENTRY( defval, minval, maxval, lock )
  // PARAM_ENTRYNAM( defval, minval, maxval, lock, name )

};

/******************************************************************************
 * @function ParameterManager_GetVerMajor
 *
 * @brief get the software version major
 *
 * This function will return the software version major
 *
 * @return        version major
 *
 *****************************************************************************/
U8 ParameterManager_GetVerMajor( void )
{
  U8  nValue = 0;
  
  #if ( SYSTEMDEFINE_OS_SELECTION != SYSTEMDEFINE_OS_MINIMAL )
    #if ( PARAM_ENABLE_CONFIGVER == ON )
      nValue = PARAM_CONFIG_VERMAJ;
    #else
      nValue = ManufInfo_GetSfwMajor( );
    #endif //PARAM_ENABLE_CONFIGVER
  #endif // SYSTEMDEFINE_OS_SELECTION
  
  // return the software major
  return( nValue );
}

/******************************************************************************
 * @function ParameterManager_GetVerMajor
 *
 * @brief get the software version major
 *
 * This function will return the software version major
 *
 * @return        version major
 *
 *****************************************************************************/
U8  ParameterManager_GetVerMinor( void )
{
  U8  nValue = 0;
  
  #if ( SYSTEMDEFINE_OS_SELECTION != SYSTEMDEFINE_OS_MINIMAL )
    #if ( PARAM_ENABLE_CONFIGVER == ON )
      nValue = PARAM_CONFIG_VERMIN;
    #else
      nValue = ManufInfo_GetSfwMinor( );
    #endif //PARAM_ENABLE_CONFIGVER
  #endif // SYSTEMDEFINE_OS_SELECTION
  
  // return the software major
  return( nValue );
}

/******************************************************************************
 * @function ParameterManager_RdByte
 *
 * @brief read a byte from the eeprom
 *
 * This function will read a byte from the EEPROM
 *
 * @param[in]   wAddress    address to read from
 * @param[in]   pnData      data to read to
 *
 * @return        TRUE if errors detected, FALSE otherwise
 *
 *****************************************************************************/
BOOL ParameterManager_RdByte( U16 wAddress, PU8 pnData )
{
  BOOL  bStatus = TRUE;
  
  // return status
  return( bStatus );
}

/******************************************************************************
 * @function ParamaterManager_RdWord
 *
 * @brief read a word from the EEPROM
 *
 * This function will read a word from the EEPROM
 *
 * @param[in]   wAddress    address to read from
 * @param[in]   pwData      data to read to
 *
 * @return        TRUE if errors detected, FALSE otherwise
 *
 *****************************************************************************/
BOOL ParameterManager_RdWord( U16 wAddress, PU16 pwData )
{
  BOOL  bStatus = TRUE;
  
  // return status
  return( bStatus );
}

/******************************************************************************
 * @function ParamaterManager_RdLong
 *
 * @brief read a long from the EEPROM
 *
 * This function will read a long from the EEPROM
 *
 * @param[in]   wAddress    address to read from
 * @param[in]   puData      data to read to
 *
 * @return        TRUE if errors detected, FALSE otherwise
 *
 *****************************************************************************/
BOOL ParameterManager_RdLong( U16 wAddress, PU32 puData )
{
  BOOL  bStatus = TRUE;
  
  // return status
  return( bStatus );
}

/******************************************************************************
 * @function ParamaterManager_RdBlock
 *
 * @brief read a block of data from the eeprom
 *
 * This function will read a block of data from the eeprom
 *
 * @param[in]   wAddress    address to read from
 * @param[in]   wLength     length to read
 * @param[i0]   pnData      pointer to the data storage
 *
 * @return        TRUE if errors detected, FALSE otherwise
 *
 *****************************************************************************/
BOOL ParameterManager_RdBlock( U16 wAddress, U16 wLength, PU8 pnData )
{
  BOOL  bStatus = TRUE;
  
  // return status
  return( bStatus );
}

/******************************************************************************
 * @function ParamaterManager_WrByte
 *
 * @brief write a byte to the eeprom
 *
 * This function will write a byte to the eeprom
 *
 * @param[in]   wAddress    address to write to
 * @param[in]   nData       data to write
 *
 * @return        TRUE if errors detected, FALSE otherwise
 *
 *****************************************************************************/
BOOL ParameterManager_WrByte( U16 wAddress, U8 nData )
{
  BOOL  bStatus = TRUE;
  
  // return status
  return( bStatus );
}

/******************************************************************************
 * @function ParamaterManager_WrWord
 *
 * @brief write a word to the EEPROM
 *
 * This function will write a word to the EEPROM
 *
 * @param[in]   wAddress    address to write to
 * @param[in]   wData       data to write
 *
 * @return        TRUE if errors detected, FALSE otherwise
 *
 *****************************************************************************/
BOOL ParameterManager_WrWord( U16 wAddress, U16 wData )
{
  BOOL  bStatus = TRUE;
  
  // return status
  return( bStatus );
}

/******************************************************************************
 * @function ParamaterManager_WrLong
 *
 * @brief write a long to the EEPROM
 *
 * This function will write a long value to the EEPROM
 *
 * @param[in]   wAddress    address to write to
 * @param[in]   uData       data to write
 *
 * @return        TRUE if errors detected, FALSE otherwise
 *
 *****************************************************************************/
BOOL ParameterManager_WrLong( U16 wAddress, U32 uData )
{
  BOOL  bStatus = TRUE;
  
  // return status
  return( bStatus );
}

/******************************************************************************
 * @function ParamaterManager_WrBlock
 *
 * @brief write a block to the EEPROM
 *
 * This function will write a block of data to the EEPROM
 *
 * @param[in]   wAddress    address to write to
 * @param[in]   wLength     length of the data to write
 * @param[in]   uData       data to write
 *
 * @return        TRUE if errors detected, FALSE otherwise
 *
 *****************************************************************************/
BOOL ParameterManager_WrBlock( U16 wAddress, U16 wLength, PU8 pnData )
{
  BOOL  bStatus = TRUE;
  
  // return status
  return( bStatus );
}

/**@} EOF ParamaterManager_cfg.c */
