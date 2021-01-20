/******************************************************************************
 * @file ManufInfo.c
 *
 * @brief Manufacturing Information impelementation
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
 * \addtogroup ManufInfo
 * @{
 *****************************************************************************/

// system includes ------------------------------------------------------------
#include "SystemDefines/SystemDefines_prm.h"

// local includes -------------------------------------------------------------
#include "ManufInfo/ManufInfo.h"

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations ----------------------------------------------

// local parameter declarations -----------------------------------------------

// local function prototypes --------------------------------------------------
#if ( MANUFINFO_ENABLE_DEBUGCOMMANDS  == 1 )
/// command handlers
  static  ASCCMDSTS CmdQryVer( U8 nCmdEnum );
#endif // MANUFINFO_ENABLE_DEBUGCOMMANDS

// constant parameter initializations -----------------------------------------
/// initialize the hardware version major
static  const CODE U8  nHdwVerMajor =
{
  MANUFINFO_HDW_MAJ
};

/// initialize the hardware version minor
static  const CODE U8  nHdwVerMinor =
{
  MANUFINFO_HDW_MIN
};

/// initialize the software version major
static  const CODE U8  nSfwVerMajor = 
{
  MANUFINFO_SFW_MAJ
};

/// initialize the software version minor
static  const CODE U8  nSfwVerMinor = 
{
  MANUFINFO_SFW_MIN
};

/// initialize the software version test
#if ( MANUFINFO_REV_SFW_TST_ENABLE == 1 )
  static  const CODE U8  nSfwVerTest =
  {
    MANUFINFO_SFW_TST
  };
#endif // MANUFINFO_REV_SFW_TST_ENABLE

/// initialize the title
static  const CODE C8  szTitle[ MANUFINFO_TITLE_LEN ] = 
{
  MANUFINFO_TITLE
};

/// initialize the serial number
static  const CODE C8  szSerNum[ MANUFINFO_SERNUM_LEN ] = 
{
  MANUFINFO_SERNUM
};

/// initialize the 1st part number
static  const CODE C8  szPartNum1[ MANUFINFO_PRTNUM1_LEN ] = 
{
  MANUFINFO_PRTNUM1
};

/// initialize the 2nd part number
#if (MANUFINFO_PRTNUM2_LEN != 0 )
  static  const CODE C8  szPartNum2[ MANUFINFO_PRTNUM2_LEN ] = 
  {
    MANUFINFO_PRTNUM2
  };
#endif  // MANUFINFO_PRTNUM2_LEN

#if ( MANUFINFO_ENABLE_DEBUGCOMMANDS  == 1 )
  // constant parameter initializations -----------------------------------------
  /// declare the command strings
  static  const CODE C8 szQryVer[ ]   = { "QVER" };
  static  const CODE C8 szQryVerX[ ]  = { "QRYVER" };

  /// initialize the command table
  const CODE ASCCMDENTRY g_atManufInfoCmdHandlerTable[ ] =
  {
    ASCCMD_ENTRY( szQryVer,  4, 0, ASCFLAG_COMPARE_NONE, 0, CmdQryVer ),
    ASCCMD_ENTRY( szQryVerX, 6, 0, ASCFLAG_COMPARE_NONE, 0, CmdQryVer ),

    // the entry below must be here
    ASCCMD_ENDTBL( )
  };

  /// define the response strings
  static  const CODE C8 szRspVr1[ ]   = { "RVER, " };
  #if (( MANUFINFO_REV_BLT_VER_ENABLE == 1 ) && ( SYSTEMDEFINE_BOOTLOADER_REV_ADDRESS != 0 ))
    static  const CODE C8 szRspBtl[ ] = { ", BTL:%02X_%02X" };
  #endif // SYSTEMDEFINE_BOOTLOADER_REV_ADDRESS
  static  const CODE C8 szRspVr2[ ]   = { ", HDW:%02X_%02X, " };
  #if ( MANUFINFO_REV_SFW_TST_ENABLE == 1 )
    static  const CODE C8 szRspVr3[ ]   = { "SFW:%02X_%02X_%02X, " };
  #else
    static  const CODE C8 szRspVr3[ ]   = { "SFW:%02X_%02X" };
  #endif // MANUFINFO_REV_SFW_TST_ENABLE
  static  const CODE C8 szNewLine[ ]  = { "\n\r" };
#endif // MANUFINFO_ENABLE_DEBUGCOMMANDS

/******************************************************************************
 * @function ManufInfo_GetSfwMajor
 *
 * @brief get the version major
 *
 * This function returns the major portion of the version
 *
 * @return      major revision
 *
 *****************************************************************************/
U8 ManufInfo_GetSfwMajor( void )
{
  // return the value
  return( PGM_RDBYTE( nSfwVerMajor ));
}

/******************************************************************************
 * @function ManufInfo_GetSfwMinor
 *
 * @brief get the version minor
 *
 * This function returns the minor portion of the version
 *
 * @return      minor revision
 *
 *****************************************************************************/
U8 ManufInfo_GetSfwMinor( void )
{
  // return the value
  return( PGM_RDBYTE( nSfwVerMinor ));
}

#if ( MANUFINFO_REV_SFW_TST_ENABLE == 1 )
  /******************************************************************************
   * @function ManufInfo_GetSfwTest
   *
   * @brief get the version test
   *
   * This function returns the test portion of the version
   *
   * @return      test revision
   *
   *****************************************************************************/
  U8 ManufInfo_GetSfwTest( void )
  {
    // return the value
    return( PGM_RDBYTE( nSfwVerTest ));
  }
#endif // MANUFINFO_REV_SFW_TST_ENABLE

/******************************************************************************
 * @function ManufInfo_GetHdwMajor
 *
 * @brief get the version major
 *
 * This function returns the major portion of the version
 *
 * @return      major revision
 *
 *****************************************************************************/
U8 ManufInfo_GetHdwMajor( void )
{
  // return the value
  return( PGM_RDBYTE( nHdwVerMajor ));
}

/******************************************************************************
 * @function ManufInfo_GetHdwMinor
 *
 * @brief get the version minor
 *
 * This function returns the minor portion of the version
 *
 * @return      minor revision
 *
 *****************************************************************************/
U8 ManufInfo_GetHdwMinor( void )
{
  // return the value
  return( PGM_RDBYTE( nHdwVerMinor ));
}

/******************************************************************************
 * @function ManufInfo_GetTitle
 *
 * @brief get the title
 *
 * This function returns the title
 *
 * @return      pointer to the title
 *
 *****************************************************************************/
const PC8 ManufInfo_GetTitle( void )
{
  // return the pointer to the title
  return(( const PC8 )&szTitle );
}

/******************************************************************************
 * @function ManufInfo_GetSerNum
 *
 * @brief gets the serial number
 *
 * This function returns the serial number
 *
 * @return      pointer to the serial number
 *
 *****************************************************************************/
const PC8 ManufInfo_GetSerNum( void )
{
  // return the pointer to the serial number
  return(( const PC8 )&szSerNum );
}

/******************************************************************************
 * @function ManufInfo_GetPartNum1
 *
 * @brief gets the 1st part number
 *
 * This function returns the 1st part number
 *
 * @return      pointer to the 1st part number
 *
 *****************************************************************************/
const PC8 ManufInfo_GetPartNum1( void )
{
  // return the pointer to the 1st part number
  return(( const PC8 )&szPartNum1 );
}

#if ( MANUFINFO_PRTNUM2_LEN != 0 )
  /******************************************************************************
   * @function ManufInfo_GetPartNum2
   *
   * @brief gets the 2nd part number
   *
   * This function returns the 2nd part number
   *
   * @return      pointer to the 2nd part number
   *
   *****************************************************************************/
  const PC8 ManufInfo_GetPartNum2( void )
  {
    // return the pointer to the 2nd part number
    return(( const PC8 )&szPartNum2 );
  }
#endif // MANUFINFO_PRTNUM2_LEN 

#if ( MANUFINFO_ENABLE_DEBUGCOMMANDS  == 1 )
  /******************************************************************************
   * @function CmdQryVer
   *
   * @brief query version command handler
   *
   * This function outputs the current board title and version
   *
   * @return  Aappropriate statue
   *****************************************************************************/
  static ASCCMDSTS CmdQryVer( U8 nCmdEnum )
  {
    PC8   pcBuffer;
    U16   wBufferIndex = 0;
    #if (( MANUFINFO_REV_BLT_VER_ENABLE == 1 ) && ( SYSTEMDEFINE_BOOTLOADER_REV_ADDRESS != 0 ))
      U32   uBootLoadRevAddress;
      U32UN tBootLoadRevision;
    #endif // SYSTEMDEFINE_BOOTLOADER_REV_ADDRESS 
  
    #if (( MANUFINFO_REV_BLT_VER_ENABLE == 1 ) && ( SYSTEMDEFINE_BOOTLOADER_REV_ADDRESS != 0 ))
      // set the address of the boot loader
      uBootLoadRevAddress = SYSTEMDEFINE_BOOTLOADER_REV_ADDRESS;
      tBootLoadRevision.uValue = *( PU32 )( uBootLoadRevAddress );
    #endif

    // get a pointer to the buffer
    AsciiCommandHandler_GetBuffer( nCmdEnum, &pcBuffer );

    // output the version=
    AsciiCommandHandler_OutputString( nCmdEnum, ( PC8 )szRspVr1 );
    AsciiCommandHandler_OutputString( nCmdEnum, ( PC8 )szTitle );
    #if (( MANUFINFO_REV_BLT_VER_ENABLE == 1 ) && ( SYSTEMDEFINE_BOOTLOADER_REV_ADDRESS != 0 ))
      wBufferIndex = SPRINTF_P( pcBuffer + wBufferIndex, ( PCC8 )szRspBtl, tBootLoadRevision.anValue[ LE_U32_LSB_IDX ], tBootLoadRevision.anValue[ LE_U32_MS1_IDX ] );
    #endif // SYSTEMDEFINE_BOOTLOADER_REV_ADDRESS 
    wBufferIndex += SPRINTF_P( pcBuffer + wBufferIndex, ( PCC8 )szRspVr2, nHdwVerMajor, nHdwVerMinor );
    #if ( MANUFINFO_REV_SFW_TST_ENABLE == 1 )
      SPRINTF_P( ( pcBuffer + wBufferIndex ), ( PCC8 )szRspVr3, nSfwVerMajor, nSfwVerMinor, nSfwVerTest );
    #else
      SPRINTF_P( ( pcBuffer + wBufferIndex ), ( PCC8 )szRspVr3, nSfwVerMajor, nSfwVerMinor );
    #endif // MANUFINFO_REV_SFW_TST_ENABLE
    AsciiCommandHandler_OutputBuffer( nCmdEnum );
    AsciiCommandHandler_OutputString( nCmdEnum, ( PC8 )szPartNum2 );
    AsciiCommandHandler_OutputString( nCmdEnum, ( PC8 )szNewLine );

    // return the error
    return( ASCCMD_STS_NONE );
  }
#endif // MANUFINFO_ENABLE_DEBUGCOMMANDS

/**@} EOF ManufInfo.c */
