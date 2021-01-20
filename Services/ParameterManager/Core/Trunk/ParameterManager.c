/******************************************************************************
 * @file ParameterManager.c
 *
 * @brief parameter manager implementation
 *
 * This file provides the implementation for the parameter manager
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
 * \addtogroup ParamaterManager
 * @{
 *****************************************************************************/

// local includes -------------------------------------------------------------
#include "ParameterManager/ParameterManager.h"

// library includes -----------------------------------------------------------
#if ( PARAM_USE_CRC == 1 )
  #include "CRC16/Crc16.h"
#endif // PARAM_USE_CRC

// Macros and Defines ---------------------------------------------------------
/// define the address of the CRC
#define PARAMETER_CHECK_ADDR        ( PARAM_BASE_ADDRESS )

/// define the address of the version block
#define PARAMETER_VERS_ADDR         ( PARAMETER_CHECK_ADDR + sizeof( U16 ))

/// define the address of the config block
#define PARAMETER_DATA_ADDR         ( PARAMETER_VERS_ADDR + sizeof( U16 ))

/// define the address computation macro
#define	PARAMADDR( eParam )		      ( PARAMETER_DATA_ADDR  + ( eParam * sizeof( PARAMARG )))

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations ----------------------------------------------

// local parameter declarations -----------------------------------------------
static  BOOL    bDefaultParameters;
static  BOOL    bErrorDetected;

// local function prototypes --------------------------------------------------
static  U16   ComputeParamCheck( BOOL bUpdateFlag );
static  void  SetDefaults( PARAMSELENUM eBegParam, PARAMSELENUM eEndParam );

/// command handlers
#if ( PARAM_ENABLE_DEBUG_COMMANDS == 1 )
  static  ASCCMDSTS CmdQryPrm( U8 nCmdEnum );
  static  ASCCMDSTS CmdSetPrm( U8 nCmdEnum );
  static  ASCCMDSTS CmdRstDfl( U8 nCmdEnum );

  // constant parameter initializations -----------------------------------------
  /// declare the command strings
  static  const CODE C8 szQryPrm[ ]   = { "QRYPRM" };
  static  const CODE C8 szSetPrm[ ]   = { "SETPRM" };
  static  const CODE C8 szRstDfl[ ]   = { "RSTDFL" };

  #if ( PARAM_SIZE_BYTES == 1 )
    static  const CODE C8 szRspQryPrm[ ]  = { "RPRM: %3d, %3d:%02X\n\r" };
  #elif ( PARAM_SIZE_BYTES == 2 )
    static  const CODE C8 szRspQryPrm[ ]  = { "RPRM: %3d, %5d:%04X\n\r" };
  #elif ( PARAM_SIZE_BYTES == 4 )
    static  const CODE C8 szRspQryPrm[ ]  = { "RPRM: %3d, %10u:%08X\n\r" };
  #else
    static  const CODE C8 szRspQryPrm[ ]  = { "RPRM: %3d, %3d:%02X\n\r" };
  #endif // PARAM_SIZE_BYTES

  /// initialize the command table
  const CODE ASCCMDENTRY g_atParamManagerCmdHandlerTable[ ] = 
  {
    ASCCMD_ENTRY( szQryPrm, 4, 1, ASCFLAG_COMPARE_NONE, 0,                             CmdQryPrm ),
    ASCCMD_ENTRY( szSetPrm, 4, 2, ASCFLAG_COMPARE_NONE, PARAM_WRITE_RESET_SYSTEM_MODE, CmdSetPrm ),
    ASCCMD_ENTRY( szRstDfl, 4, 1, ASCFLAG_COMPARE_NONE, PARAM_WRITE_RESET_SYSTEM_MODE, CmdRstDfl ),

    // the entry below must be here
    ASCCMD_ENDTBL( )
  };
#endif  // PARAM_ENABLE_DEBUG_COMMANDS

/******************************************************************************
 * @function ParameterMangaer_Initialize
 *
 * @brief parameter initialization
 *
 * This function will verify the parameters are valid and if not will reset
 * them to the default values
 *
 * @param[in]   bForceReset     force a reset to defaults
 *
*****************************************************************************/
void ParameterManager_Initialize( BOOL bForceReset )
{
  U16   wActCheckValue, wExpCheckValue, wActVersion;
  U16UN tExpVersion;

  // clear the error deteced flag
  bErrorDetected = FALSE;
  
  // get the expectant version
  tExpVersion.anValue[ LE_U16_MSB_IDX ] = ParameterManager_GetVerMajor( );
  tExpVersion.anValue[ LE_U16_LSB_IDX ] = ParameterManager_GetVerMinor( );
  
	// compute the checksum of the stored parameters
  ParameterManager_RdWord( PARAMETER_CHECK_ADDR, &wActCheckValue );
  ParameterManager_RdWord( PARAMETER_VERS_ADDR, &wActVersion );
  
  // now compare to see if equal
  wExpCheckValue = ComputeParamCheck( FALSE );
  if ( bErrorDetected == FALSE )
  {
    // now test 
    if (( bForceReset == TRUE ) || ( wExpCheckValue != wActCheckValue ) || ( wActVersion != tExpVersion.wValue ))
    {
      // corrupt or un-inintialized params/copy defaults
      SetDefaults( 0, PARAMSEL_MAX_NUM );
    }
  }
  
  // good config, post done
  #if ( PARAM_ENABLE_NOTIFICATIONS ==  ON )
    ParameterManager_PostDone( FALSE );
  #endif
}

/******************************************************************************
 * @function ParameterMangaer_SetDefaults
 *
 * @brief reset all parameters to their default values
 *
 * This function will copy the default values to the eeprom
 *
 *****************************************************************************/
void ParameterManager_SetDefaults( void )
{
	// set the defaults
  SetDefaults( 0, PARAMSEL_MAX_NUM );
}

/******************************************************************************
 * @function ParameterMangaer_SetDefaultsRange
 *
 * @brief reset all parameters to their default values
 *
 * This function will copy the default values to the eeprom
 *
 *****************************************************************************/
void ParameterManager_SetDefaultsRange( PARAMSELENUM eBegParam, PARAMSELENUM eEndParam )
{
	// set the defaults
	SetDefaults( eBegParam, eEndParam );
}

/******************************************************************************
 * @function ParameterMangaer_GetValue
 *
 * @brief get a parameter value
 *
 * This function will return the value of a given parameter
 *
 * @param[in]   eParam    parameter number
 * @param[in]   pxValue   pointer to the parameter value
 *
 * @return      appropriate eerror
 *  
 *****************************************************************************/
PARAMERRS	ParameterManager_GetValue( PARAMSELENUM eParam, PPARAMARG pxValue )
{
  PARAMERRS eError = PARAM_ERR_NONE;
  
  // clear the returned value
  *pxValue = 0;
  
  // valid parameter
  if ( eParam < PARAMSEL_MAX_NUM )
  {
    // get the value
    switch( sizeof( PARAMARG ))
    {
      case 1 :
        ParameterManager_RdByte( PARAMADDR( eParam ), ( PU8 )pxValue );
        break;
        
      case 2 :
        ParameterManager_RdWord( PARAMADDR( eParam ), ( PU16 )pxValue );
        break;
        
      case 4 :
        ParameterManager_RdLong( PARAMADDR( eParam ), ( PU32 )pxValue );
        break;
        
      default :
        break;
    }
  }
  else
  {
    // set the error
    eParam = PARAM_ERR_PNUM;
  }
  
  // return the error
  return( eError );
}

/******************************************************************************
 * @function ParameterMangaer_PutValue
 *
 * @brief store a parameter value
 *
 * This function will store the parameter value and update the current check
 * value
 *
 * @param[in]   eParam        parameter number
 * @param[in]   xValue        parameter value
 * @param[in]   bBypassLock   parameter value
 *
 * @return      appropriate eerror
 *
 *****************************************************************************/
PARAMERRS	ParameterManager_PutValue( PARAMSELENUM eParam, PARAMARG xValue, BOOL bBypassLock )
{
  PARAMERRS eError = PARAM_ERR_NONE;
  
  // valid parameter
  if ( eParam < PARAMSEL_MAX_NUM )
  {
    // check for locked
    if (( PGM_RDBYTE( atParamDefaults[ eParam ].bLocked ) == FALSE ) || ( bBypassLock == TRUE ))
    {
      // get the value
      switch( sizeof( PARAMARG ))
      {
        case 1 :
          ParameterManager_WrByte( PARAMADDR( eParam ), xValue );
          break;
        
        case 2 :
          ParameterManager_WrWord( PARAMADDR( eParam ), xValue );
          break;
        
        case 4 :
          ParameterManager_WrLong( PARAMADDR( eParam ), xValue );
          break;
        
        default :
          break;
      }
    
      // update the checksum
      ComputeParamCheck( TRUE );
    
      // clear the default parameters
      bDefaultParameters = FALSE;
    }
    else
    {
      // set the error
      eError = PARAM_ERR_LOCKED;
    }
  }
  else
  {
    // set the error
    eError = PARAM_ERR_PNUM;
  }

  // return the error
  return( eError );
}

/******************************************************************************
 * @function ParameterMangaer_GetLimits
 *
 * @brief get the limits for the parameter
 *
 * This function will return the limits for a given parameter
 *
 * @param[in]   eParam      parameter number
 * @param[in]   pxMinValue  pointer to the min value
 * @param[in]   pxMaxValue  pointer to the max value
 *
 * @return      appropriate eerror
 *
 *****************************************************************************/
PARAMERRS	ParameterManager_GetLimits( PARAMSELENUM eParam, PPARAMARG pxMinValue, PPARAMARG pxMaxValue )
{
  PARAMERRS eError = PARAM_ERR_NONE;

  // clear the returned value
  *pxMinValue = 0;
  *pxMaxValue = 0;
  
  // valid parameter
  if ( eParam < PARAMSEL_MAX_NUM )
  {
    // get the value
    switch( sizeof( PARAMARG ))
    {
      case 1 :
        *pxMinValue = PGM_RDBYTE( atParamDefaults[ eParam ].xMinVal );
        *pxMaxValue = PGM_RDBYTE( atParamDefaults[ eParam ].xMaxVal );
        break;
        
      case 2 :
        *pxMinValue = PGM_RDWORD( atParamDefaults[ eParam ].xMinVal );
        *pxMaxValue = PGM_RDWORD( atParamDefaults[ eParam ].xMaxVal );
        break;
        
      case 4 :
        *pxMinValue = PGM_RDDWRD( atParamDefaults[ eParam ].xMinVal );
        *pxMaxValue = PGM_RDDWRD( atParamDefaults[ eParam ].xMaxVal );
        break;
        
      default :
        break;
    }
  }
  
  // return the error
  return( eError );
}

#if ( PARAM_USE_NAMES == 1 )
  /******************************************************************************
   * @function ParameterMangaer_GetName
   *
   * @brief get a parameter name
   *
   * This function will return a pointer to the parameter name
   *
   * @param[in]   eParam    parameter number
   * @param[in]   pszName   pointer to the name
   *
   * @return      appropriate eerror
   *
   *****************************************************************************/
  PARAMERRS	ParameterManager_GetName( PARAMSELENUM eParam, PFC8 pszName )
  {
    PARAMERRS eError = PARAM_ERR_NONE;
  
    // reset the return value
    *pszName = NULL;
  
    // valid parameter
    if ( eParam < PARAMSEL_MAX_NUM )
    {
      // return the pointer
      return(( const PC8 )atParamDefaults[ eParam ].pszName );
    }
  
    // return the error
    return( eError );
  }
#endif

/******************************************************************************
 * @function ParameterMangaer_GetLimits
 *
 * @brief get the limits for the parameter
 *
 * This function will return the limits for a given parameter
 *
 * @param[in]   eParam      parameter number
 * @param[i0]   pxDfltValue pointer to the min value
 *
 * @return      appropriate eerror
 *
 *****************************************************************************/
PARAMERRS ParameterManager_GetDefaultValue( PARAMSELENUM eParam, PPARAMARG pxDfltValue )
{
  PARAMERRS eError = PARAM_ERR_NONE;

  // clear the returned value
  *pxDfltValue = 0;
  
  // valid parameter
  if ( eParam < PARAMSEL_MAX_NUM )
  {
    // get the value
    *pxDfltValue = PGM_RDBYTE( atParamDefaults[ eParam ].xDefVal );
  }
  
  // return the error
  return( eError );
}

/******************************************************************************
 * @function ParameterManager_GetDefaultStatus
 *
 * @brief gets the default status flag
 *
 * This function will the status of default state
 *
 * @return      TRUE if defaults have been set, FALSE otherwise
 *****************************************************************************/
BOOL ParameterManager_GetDefaultStatus( void )
{
  // return the state
  return( bDefaultParameters );
}

/******************************************************************************
 * @function SetDefaults
 *
 * @brief set all parameter so their default values
 *
 * This function will set all parameter to their default values
 *
 *****************************************************************************/
static void SetDefaults( PARAMSELENUM eBegParam, PARAMSELENUM eEndParam )
{
  U16UN       tVersion;
  U16         wWriteLength, wAddress;
  PPARAMARG   ptValues;
  PARAMARG    tValue;
  U8          nNumParams, nParamCount, nParamIdx;
  PPARAMDFLTS ptDefaults;

  // update the version
  tVersion.anValue[ LE_U16_MSB_IDX ] = ParameterManager_GetVerMajor( );
  tVersion.anValue[ LE_U16_LSB_IDX ] = ParameterManager_GetVerMinor( );
  
  // write it
  ParameterManager_WrWord( PARAMETER_VERS_ADDR, tVersion.wValue );

  // set the parameter count/set the beginning address
  nParamCount = PARAMSEL_MAX_NUM;
  wAddress = PARAMETER_DATA_ADDR;
  
  // create a local buffer
  if (( ptValues = ( PPARAMARG )malloc( PARAM_UPDATE_BLOCK_SIZE )) != NULL )
  {
    // set the block length
    nNumParams = PARAM_UPDATE_BLOCK_SIZE / sizeof( PARAMARG );
  }
  else
  {
    // force length to 1
    ptValues = &tValue;
    nNumParams = 1;
  }

  // set the addresses
  wAddress = PARAMETER_DATA_ADDR;
  ptDefaults = ( PPARAMDFLTS )&atParamDefaults;

  // while there is data
  while( nParamCount != 0 )
  {
    // clear the write length
    wWriteLength = 0;
    
    // set the number of parameters to iterate this loop
    nNumParams = MIN( nNumParams, nParamCount );
    
    // get the default values for this block
    for ( nParamIdx = 0; nParamIdx < nNumParams; nParamIdx++ )
    {
      // copy the defaults
      *( ptValues + nParamIdx ) = ptDefaults++->xDefVal;
      wWriteLength += sizeof( PARAMARG );
    }

    // now write it
    if (( bErrorDetected = ParameterManager_WrBlock( wAddress, wWriteLength, ( PU8 )ptValues )) == TRUE )
    {
      // exit
      break;
    }

    // adjust address
    wAddress += wWriteLength;
    
    // adjust the parameter count
    nParamCount -= nNumParams;
  }
  
  // if no error detected
  if ( !bErrorDetected )
  {
    // force an update of the Check value
    bDefaultParameters = TRUE;
    ComputeParamCheck( TRUE );
  }
}

/******************************************************************************
 * @function ComputeParamCheck
 *
 * @brief compute the check value for the parameter block
 *
 * This function compute the check value of the parameter block
 * and update the check value if requested
 *
 * @param[in]   bUpdateFlag   request and udpate of the Check value
 *
 * @return      current check value of the parameter block
 *
 *****************************************************************************/
static U16 ComputeParamCheck( BOOL bUpdateFlag )
{
  U16         wCheckValue, wIdx, wTotalLength, wBlockLength, wReadLength, wAddress;
  PU8         pnValues;
  U8          nValue;
  BOOL        bErrorDetected = FALSE;

  // initialize the value
  #if ( PARAM_USE_CRC == 1 )
    wCheckValue = CRC16_GetInitialValue( );
  #else
    wCheckValue = 0;
  #endif // PARAM_USE_CRC

  // compute the total length
  wTotalLength = PARAMSEL_MAX_NUM * sizeof( PARAMARG );

  // create a local buffer
  if (( pnValues = malloc( PARAM_UPDATE_BLOCK_SIZE )) != NULL )
  {
    // set the block length
    wBlockLength = PARAM_UPDATE_BLOCK_SIZE;
  }
  else
  {
    // force length to 1
    pnValues = &nValue;
    wBlockLength = 1;
  }

  // set the addresses
  wAddress = PARAMETER_DATA_ADDR;

  // for each byte in the parameter block
  while( wTotalLength != 0 )
  {
    // set the current block length
    wReadLength = MIN( wBlockLength, wTotalLength );

    // read the block
    bErrorDetected = ParameterManager_RdBlock( wAddress, wReadLength, pnValues );

    // now for each byte in block
    for( wIdx = 0; wIdx < wReadLength; wIdx++ )
    {
      // now add it to the local check value
      #if ( PARAM_USE_CRC == 1 )
      // update with Crc
      wCheckValue = CRC16_CalculateByte( wCheckValue, *( pnValues + wIdx ));
      #else
      // just add if this is a checksum
      wCheckValue += *( pnValues + wIdx );
      #endif // PARAM_USE_CRC
    }

    // adjust address
    wAddress += wReadLength;

    // adjust the total length
    wTotalLength -= wReadLength;
  }

  // check for error
  if ( !bErrorDetected )
  {
    // if this is a checksum - 2's complement it
    #if ( PARAM_USE_CRC == 0 )
    {
      wCheckValue ^= wCheckValue;
      wCheckValue++;
    }
    #endif // PARAM_USE_CRC == 0

    // determine if we are to write this value
    if ( bUpdateFlag )
    {
      ParameterManager_WrWord( PARAMETER_CHECK_ADDR, wCheckValue );
    }
  }

  // return the calculated value
  return( wCheckValue );
}

#if ( PARAM_ENABLE_DEBUG_COMMANDS == 1 )
  /******************************************************************************
   * @function CmdQryPrm
   *
   * @brief query parameter value
   *
   * This function will display a parameter value if valid parameter
   *
   * @param[in]   nCmdEnum      command handler enumeration
   *
   * @return      appropriate eerror
   *
   *****************************************************************************/
  static ASCCMDSTS CmdQryPrm( U8 nCmdEnum )
  {
    U32UN       tTemp;
    PC8         pcBuffer;
    PARAMERRS   eError;
    PARAMARG    xValue;
  
    // fetch the pointer to the buffer
    AsciiCommandHandler_GetBuffer( nCmdEnum, &pcBuffer );

    // get the parameter number
    AsciiCommandHandler_GetValue( nCmdEnum, 0, &tTemp.uValue );

    // get the parameter
    if (( eError = ParameterManager_GetValue( tTemp.anValue[ LE_U32_LSB_IDX ], &xValue )) != PARAM_ERR_NONE )
    {
      // output the error
      SPRINTF_P( pcBuffer, ( char const * )g_szAsciiErrStrn, eError, eError );
    }
    else
    {
      // output the value
      SPRINTF_P( pcBuffer, ( char const * )szRspQryPrm, tTemp.anValue[ LE_U32_LSB_IDX ], ( unsigned int )xValue, ( unsigned int )xValue );
    }
  
    // return status
    return( ASCCMD_STS_OUTPUTBUFFER );
  }

  /******************************************************************************
   * @function CmdSetPrm
   *
   * @brief set a parameter value
   *
   * This function will set a parameter value if valid parameter
   *
   * @param[in]   nCmdEnum      command handler enumeration
   *
   * @return      appropriate eerror
   *
   *****************************************************************************/
  static ASCCMDSTS CmdSetPrm( U8 nCmdEnum )
  {
    ASCCMDSTS     eStatus = ASCCMD_STS_NONE;
    U32UN         tTemp;
    PC8           pcBuffer;
    PARAMERRS     eError;
    PARAMARG      xValue;
    PARAMSELENUM  eParamEnum;

    // fetch the pointer to the buffer
    AsciiCommandHandler_GetBuffer( nCmdEnum, &pcBuffer );

    // get the parameter number/value
    AsciiCommandHandler_GetValue( nCmdEnum, 0, &tTemp.uValue );
    eParamEnum = tTemp.anValue[ LE_U32_LSB_IDX ];
    AsciiCommandHandler_GetValue( nCmdEnum, 1, &tTemp.uValue );


    // now get the appropriate size value
    switch( sizeof( PARAMARG ))
    {
      case 1 :
        xValue = tTemp.anValue[ LE_U32_LSB_IDX ];
        break;
      
      case 2 :
        xValue = tTemp.awValue[ LE_U32_LSW_IDX ];
        break;
      
      case 4 :
        xValue = tTemp.uValue;
        break;
      
      default :
        xValue = tTemp.anValue[ LE_U32_LSB_IDX ];
        break;
    }

    // set the parameter
    if (( eError = ParameterManager_PutValue( eParamEnum, xValue, TRUE )) != PARAM_ERR_NONE )
    {
      // output the error
      SPRINTF_P( pcBuffer, ( char const * )g_szAsciiErrStrn, eError, eError );
      eStatus = ASCCMD_STS_OUTPUTBUFFER;
    }
  
    // return status
    return( eStatus );
  }

  /******************************************************************************
   * @function CmdRstDfl
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
  static ASCCMDSTS CmdRstDfl( U8 nCmdEnum )
  {
    ASCCMDSTS eStatus = ASCCMD_STS_NONE;
    PC8       pszPassWord;

    // get the password
    AsciiCommandHandler_GetArg( nCmdEnum, 0, &pszPassWord );

    // now compare to password
    if ( STRCMP_P(( PCC8 )PARAM_RESET_DEFAULTS_PASSWORD, pszPassWord ) == 0 )
    {
      // reset the parameters
      ParameterManager_SetDefaults( );
    }
    else
    {
      // send the error
      eStatus = ASCCMD_STS_ILLPASSWORD;
    }

    // return the status
    return( eStatus );
  }
#endif  // PARAM_ENABLE_DEBUG_COMMANDS

/**@} EOF ParamaterManager.c */
