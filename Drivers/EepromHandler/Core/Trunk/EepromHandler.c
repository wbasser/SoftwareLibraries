/******************************************************************************
 * @file EepromHandler.c
 *
 * @brief EEPROM handler implementation
 *
 * This file implements the EEPROM handler
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
 * \addtogroup EepromHandler
 * @{
 *****************************************************************************/

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "EepromHandler/Eepromhandler.h"
#include "EepromHandler/EepromHandler_prm.h"

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------
/// define the read/write block timeouts in msec
#define WRBLOCK_MAX_TIMEOUT                 ( 2 )
#define RDBLOCK_MAX_TIMEOUT                 ( 2 )

/// define the byte time in USEC
#define USEC_PER_BYTE                       ( 500 )
#define BYTE_PER_MSEC                       ( 1000 / USEC_PER_BYTE )

/// define the macro to compute a block time
#define COMPUTE_BLOCK_TIME( len )           (( len / BYTE_PER_MSEC ) + 1 )

/// define the wait for busy time
#define WAIT_FOR_BUSY_DONE_TIME             ( 25 )

#if ( EEPROMHANDLER_ENABLE_BACKGROUND_WRITES == 1 )
#define the size of the write modified block 
#define BLOCK_WRITEMOD_SIZE                 (( EEPROMHANDLER_DEV_SIZE / EEPROMHANDLER_BLK_SIZE ) / 8 )
#define BLOCK_WRITEMOD_BYTE_OFFSET( addr )  ( addr / ( EEPROMHANDLER_BLK_SIZE * 8 ))
#define BLOCK_WRITEMOD_BIT_MASK( addr )     ( addr % ( EEPROMHANDLER_BLK_SIZE * 8 ))
#endif // EEPROMHANDLER_ENABLE_BACKGROUND_WRITES

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations ----------------------------------------------

// local parameter declarations -----------------------------------------------
static  BOOL      bCheckForBusyRequired;
#if (( EEPROMHANDLER_ENABLE_EMULATION == 1 ) || ( EEPROMHANDLER_ENABLE_BACKGROUND_WRITES == 1 ))
static  U8        anEeprom[ EEPROMHANDLER_DEV_SIZE ];
#endif // EEPROMHANDLER_ENABLE_EMULATION
#if (( EEPROMHANDLER_ENABLE_EMULATION == 0 ) || ( EEPROMHANDLER_ENABLE_BACKGROUND_WRITES == 1 ))
static  I2CXFRCTL tXfrCtl;
#endif // EEPROMHANDLER_ENABLE_EMULATION
#if ( EEPROMHANDLER_ENABLE_BACKGROUND_WRITES == 1 )
static  U8        anBlockModifiedStatus[ BLOCK_WRITEMOD_SIZE ];
#endif // EEPROMHANDLER_ENABLE_BACKGROUND_WRITES

// command handlers
#if ( EEPROMHANDLER_ENABLE_DEBUGCOMMANDS == 1 )
static  ASCCMDSTS CmdWrtEep( U8 nCmdEnum );
static  ASCCMDSTS CmdRedEep( U8 nCmdEnum );
static  ASCCMDSTS CmdDmpEep( U8 nCmdEnum );
static  ASCCMDSTS CmdFilEep( U8 nCmdEnum );
#endif // EEPROMHANDLER_ENABLE_DEBUGCOMMANDS

// local function prototypes --------------------------------------------------
#if ( EEPROMHANDLER_ENABLE_EMULATION == 0 )
static  EEPROMERR  WriteBlock( U16 wAddress, U8 nBlkLength, PU8 pnData );
#endif // EEPROMHANDLER_ENABLE_EMULATION

#if (( EEPROMHANDLER_ENABLE_BACKGROUND_WRITES == 1 ) || ( EEPROMHANDLER_ENABLE_EMULATION == 0 ))
static  BOOL       CheckForBusy( void );
#endif // EEPROMHANDLER_ENABLE_BACKGROUND_WRITES

// constant parameter initializations -----------------------------------------
/// define the command strings
#if ( EEPROMHANDLER_ENABLE_DEBUGCOMMANDS == 1 )
static  const CODE C8 szWrtEep[ ]   = { "WEEP" };
static  const CODE C8 szRedEep[ ]   = { "REEP" };
static  const CODE C8 szDmpEep[ ]   = { "DEEP" };
static  const CODE C8 szFilEep[ ]   = { "FEEP" };

/// string declarations
static  const CODE C8 szRspRep[ ]   = { "RREP, 0x%04X, 0x%02X\n\r" };

/// initialize the command table
const CODE ASCCMDENTRY g_atEepromDbgHandlerTable[ ] =
{
  ASCCMD_ENTRY( szWrtEep, 4, 2, ASCFLAG_COMPARE_EQ, EEPROMHANDLER_DIAGMODE_ENUM, CmdWrtEep ),
  ASCCMD_ENTRY( szRedEep, 4, 1, ASCFLAG_COMPARE_EQ, EEPROMHANDLER_DIAGMODE_ENUM, CmdRedEep ),
  ASCCMD_ENTRY( szDmpEep, 4, 2, ASCFLAG_COMPARE_EQ, EEPROMHANDLER_DIAGMODE_ENUM, CmdDmpEep ),
  ASCCMD_ENTRY( szFilEep, 4, 3, ASCFLAG_COMPARE_EQ, EEPROMHANDLER_DIAGMODE_ENUM, CmdFilEep ),

  // the entry below must be here
  ASCCMD_ENDTBL( )
};
#endif // EEPROMHANDLER_ENABLE_DEBUGCOMMANDS

/******************************************************************************
 * @function EepromHandler_INitialize
 *
 * @brief EEPROM handler initialization
 *
 * This function will initialize the EEPROM handler
 *
 *****************************************************************************/
void EepromHandler_Initialize( void )
{
  // clear the check for busy
  bCheckForBusyRequired = FALSE;
  
  #if (( EEPROMHANDLER_ENABLE_EMULATION == 1 ) || ( EEPROMHANDLER_ENABLE_BACKGROUND_WRITES == 1 ))
  memset( anEeprom, 0xFF, EEPROMHANDLER_DEV_SIZE );
  #endif // EEPROMHANDLER_ENABLE_EMULATION
  
  // call the local initialization
  #if (SYSTEMDEFINE_OS_SELECTION != SYSTEMDEFINE_OS_MINIMAL )
  EepromHandler_LclInitialize( );
  #endif // SYSTEMDEFINE_OS_SELECTON != SYSTEMDEFINE_OS_TASKMINIMAL
}

#if ( EEPROMHANDLER_ENABLE_BACKGROUND_WRITES == 1 )
/******************************************************************************
 * @function EepromHandler_BlockWriteEvent
 *
 * @brief block write update event
 *
 * This function will force the update of a modified block
 *
 * @param[in]     xArg      input argument
 * 
 * @return        TRUE      flush the event
 *
 *****************************************************************************/
void EepromHandler_BlockWriteEvent( void )
{
  U8      nBlockOffset = 0;
  BOOL    bFoundFlag = FALSE;
  U16     wBlockBaseAddr;
  U8      nBitOffset;
  U32     uTime;
  I2CERR  eError;
  BOOL    bError = FALSE;
  
  // get the current time
	uTime = SystemTick_GetTimeMsec( ) + WAIT_FOR_BUSY_DONE_TIME;

  // ensure eeprom is not busy
  while(( bCheckForBusyRequired = CheckForBusy( )) == TRUE )
  {
    // check for timeout
    if ( SystemTick_GetTimeMsec( ) >= uTime )
    {
      // set the error
      bError = TRUE;
      break;
    }
  }
  
  // find a block
  if ( bError == FALSE )
  {
    // find a block
    while(( nBlockOffset < BLOCK_WRITEMOD_SIZE ) && ( bFoundFlag == FALSE ))
    {
      // test for modified
      if ( anBlockModifiedStatus[ nBlockOffset ] != 0 )
      {
        // set the found flag
        bFoundFlag = TRUE;
    
        // now determine which block 
        for ( nBitOffset = 0; nBitOffset < 8; nBitOffset++ )
        {
          if (( anBlockModifiedStatus[ nBlockOffset ] & BIT( nBitOffset )) != 0 )
          {
            // break out of loop
            break;
          }
        }
      }
      else
      {
        // increment the block offset
        nBlockOffset++;        
      }
    }
    
    // now if we have found a block that needs modified, write it
    if ( bFoundFlag )
    {
      // write the block
      wBlockBaseAddr = nBlockOffset * EEPROMHANDLER_BLK_SIZE * 8;
      wBlockBaseAddr += ( nBitOffset * EEPROMHANDLER_BLK_SIZE );
      
      // set the address
      tXfrCtl.tAddress.uValue = wBlockBaseAddr;
      tXfrCtl.nAddrLen = EEPROMHANDLER_ADR_SIZE;

      // set the the device address
      tXfrCtl.nDevAddr = EEPROMHANDLER_DEV_ADDR;

      // set the pointer to the data/length
      tXfrCtl.pnData = &anEeprom[ wBlockBaseAddr ];
      tXfrCtl.wDataLen = EEPROMHANDLER_BLK_SIZE;

      // calculate the read timeout
      tXfrCtl.uTimeout = WRBLOCK_MAX_TIMEOUT + COMPUTE_BLOCK_TIME( EEPROMHANDLER_BLK_SIZE + EEPROMHANDLER_ADR_SIZE );

      // perform write
      eError = ( EEPROMERR )I2c_Write( EEPROMHANDLER_I2C_ENUM, &tXfrCtl );
    }
    else
    {
      // disable this task
      TaskManager_EnableDisable( EEPROM_HANDLER_BACKGROUND_TASK_ENUM, OFF );
    }
  }
}
#endif // EEPROMHANDLER_ENABLE_BACKGROUND_WRITES

/******************************************************************************
 * @function EepromHandler_RdByte
 *
 * @brief read a byte from the eeprom
 *
 * This function will read a byte from the EEPROM
 *
 * @param[in]   wAddress    address to read from
 * @param[in]   pnData      data to read to
 *
 * @return      error enumeration
 *
 *****************************************************************************/
EEPROMERR EepromHandler_RdByte( U16 wAddress, PU8 pnData )
{
  EEPROMERR  eError = EEPROM_ERR_NONE;
  
  // call read block
  eError = EepromHandler_RdBlock( wAddress, sizeof( U8 ), pnData );
  
  // return the error
  return( eError );
}

/******************************************************************************
 * @function EepromHandler_RdWord
 *
 * @brief read a word from the EEPROM
 *
 * This function will read a word from the EEPROM
 *
 * @param[in]   wAddress    address to read from
 * @param[in]   pwData      data to read to
 *
 * @return      
 *
 *****************************************************************************/
EEPROMERR EepromHandler_RdWord( U16 wAddress, PU16 pwData )
{
  EEPROMERR  eError = EEPROM_ERR_NONE;
  
  // call read block
  eError = EepromHandler_RdBlock( wAddress, sizeof( U16 ), ( PU8 )pwData );
  
  // return the error
  return( eError );
}

/******************************************************************************
 * @function EepromHandler_RdLong
 *
 * @brief read a long from the EEPROM
 *
 * This function will read a long from the EEPROM
 *
 * @param[in]   wAddress    address to read from
 * @param[in]   puData      data to read to
 *
 * @return      
 *
 *****************************************************************************/
EEPROMERR EepromHandler_RdLong( U16 wAddress, PU32 puData )
{
  EEPROMERR  eError = EEPROM_ERR_NONE;
  
  // call read block
  eError = EepromHandler_RdBlock( wAddress, sizeof( U32 ), ( PU8 )puData );
  
  // return the error
  return( eError );
}

/******************************************************************************
 * @function EepromHandler_RdBlock
 *
 * @brief read a block of data from the eeprom
 *
 * This function will read a block of data from the eeprom
 *
 * @param[in]   wAddress    address to read from
 * @param[in]   wLength     length to read
 * @param[i0]   pnData      pointer to the data storage
 *
 * @return      error enumeration
 *
 *****************************************************************************/
EEPROMERR EepromHandler_RdBlock( U16 wAddress, U16 wLength, PU8 pnData )
{
  EEPROMERR   eError = EEPROM_ERR_NONE;
  #if (( EEPROMHANDLER_ENABLE_EMULATION == 0 ) && ( EEPROMHANDLER_ENABLE_BACKGROUND_WRITES == 0 ))
  U32         uTime;
  #endif // EEPROMHANDLER_ENABLE_EMULATION

  // check to see if valid address
  if(( wAddress + wLength ) < EEPROMHANDLER_DEV_SIZE )
  {
    #if (( EEPROMHANDLER_ENABLE_EMULATION == 0 ) && ( EEPROMHANDLER_ENABLE_BACKGROUND_WRITES == 0 ))
    // get the current time
    #if (SYSTEMDEFINE_OS_SELECTION != SYSTEMDEFINE_OS_MINIMAL )
    uTime = EEPROMHANDLER_GET_SYSTEMTIME( ) + WAIT_FOR_BUSY_DONE_TIME;
    #else
    uTime = 0x8000000;
    #endif // SYSTEMDEFINE_OS_SELECTON != SYSTEMDEFINE_OS_TASKMINIMAL

    // wait till done
    while(( bCheckForBusyRequired = EepromHandler_CheckDeviceBusy( )) == TRUE )
    {
      // check for timeout
     #if (SYSTEMDEFINE_OS_SELECTION != SYSTEMDEFINE_OS_MINIMAL )
      if ( EEPROMHANDLER_GET_SYSTEMTIME( ) >= uTime )
      #else
      if ( --uTime == 0 )
      #endif // SYSTEMDEFINE_OS_SELECTON != SYSTEMDEFINE_OS_TASKMINIMAL
      {
        // timeout occured - flag error
        eError = EEPROM_ERR_DEVBUSY;
        break;
      }
    }
    #endif // EEPROMHANDLER_ENABLE_EMULATION
  
    // do not execute if busy
    if ( eError == EEPROM_ERR_NONE )
    {
      #if (( EEPROMHANDLER_ENABLE_EMULATION == 0 ) && ( EEPROMHANDLER_ENABLE_BACKGROUND_WRITES == 0 ))
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
      eError = ( EEPROMERR )I2c_Read( EEPROMHANDLER_I2C_ENUM, &tXfrCtl );
      #else
      // copy from the local area
      memcpy( pnData, &anEeprom[ wAddress ], wLength ); 
      #endif // EEPROMHANDLER_ENABLE_EMULATION
    }
  }  
  else
  {
    // illegal address
    eError = EEPROM_ERR_ILLADDR;
  }

  // return the error
  return( eError );
}

/******************************************************************************
 * @function EepromHandler_WrByte
 *
 * @brief write a byte to the eeprom
 *
 * This function will write a byte to the eeprom
 *
 * @param[in]   wAddress    address to write to
 * @param[in]   nData       data to write
 *
 * @return      error enumeration
 *
 *****************************************************************************/
EEPROMERR EepromHandler_WrByte( U16 wAddress, U8 nData )
{
  EEPROMERR  eError = EEPROM_ERR_NONE;
  
  // call write block
  eError = EepromHandler_WrBlock( wAddress, sizeof( U8 ), &nData );
  
  // return the error
  return( eError );
}

/******************************************************************************
 * @function EepromHandler_WrWord
 *
 * @brief write a word to the EEPROM
 *
 * This function will write a word to the EEPROM
 *
 * @param[in]   wAddress    address to write to
 * @param[in]   wData       data to write
 *
 * @return      error enumeration
 *
 *****************************************************************************/
EEPROMERR EepromHandler_WrWord( U16 wAddress, U16 wData )
{
  EEPROMERR  eError = EEPROM_ERR_NONE;
  
  // call write block
  eError = EepromHandler_WrBlock( wAddress, sizeof( U16 ), ( PU8 )&wData );
  
  // return the error
  return( eError );
}

/******************************************************************************
 * @function EepromHandler_WrLong
 *
 * @brief write a long to the EEPROM
 *
 * This function will write a long value to the EEPROM
 *
 * @param[in]   wAddress    address to write to
 * @param[in]   uData       data to write
 *
 * @return      error enumeration
 *
 *****************************************************************************/
EEPROMERR EepromHandler_WrLong( U16 wAddress, U32 uData )
{
  EEPROMERR  eError = EEPROM_ERR_NONE;
  
  // call write block
  eError = EepromHandler_WrBlock( wAddress, sizeof( U32 ), ( PU8 )&uData );
  
  // return the error
  return( eError );
}

/******************************************************************************
 * @function EepromHandler_WrBlock
 *
 * @brief write a block to the EEPROM
 *
 * This function will write a block of data to the EEPROM
 *
 * @param[in]   wAddress    address to write to
 * @param[in]   wLength     length of the data to write
 * @param[in]   uData       data to write
 *
 * @return      error enumeration
 *
 *****************************************************************************/
EEPROMERR EepromHandler_WrBlock( U16 wAddress, U16 wLength, PU8 pnData )
{
  EEPROMERR eError = EEPROM_ERR_NONE;
  #if (( EEPROMHANDLER_ENABLE_EMULATION == 0 ) && ( EEPROMHANDLER_ENABLE_BACKGROUND_WRITES == 0 ))
  U8        nBlkLength;
  #endif // EEPROMHANDLER_ENABLE_EMULATION
  
  // check to see if valid address
  if(( wAddress + wLength ) < EEPROMHANDLER_DEV_SIZE )
  {
    #if (( EEPROMHANDLER_ENABLE_EMULATION == 0 ) && ( EEPROMHANDLER_ENABLE_BACKGROUND_WRITES == 0 ))
      // while we still have bytes to write
      while( wLength != 0 )
      {
        // compute the availabe space in the block
        nBlkLength = ( EEPROMHANDLER_BLK_SIZE - ( wAddress & ( EEPROMHANDLER_BLK_SIZE - 1 )));

        // set the block length
        nBlkLength = MIN( nBlkLength, wLength );

        // write this block
        if (( eError = WriteBlock( wAddress, nBlkLength, pnData )) == EEPROM_ERR_NONE )
        {
          // adjust the address/adjust the length
          wLength -= nBlkLength;
          wAddress += nBlkLength;
        
          // adjust the data pointer
          pnData += nBlkLength;
        }
        else
        {
          // exit loop
          break;
        }
      }
    #else
      // copy to local
      memcpy( &anEeprom[ wAddress ], pnData, wLength );
    #endif // EEPROMHANDLER_ENABLE_EMULATION
    
    #if ( EEPROMHANDLER_ENABLE_BACKGROUND_WRITES == 1 )
      // mark block as modified
      anBlockModifiedStatus[ BLOCK_WRITEMOD_BYTE_OFFSET( wAddress )] |= BLOCK_WRITEMOD_BIT_MASK( wAddress );
    
      // enable this task
      TaskManager_EnableDisable( EEPROM_HANDLER_BACKGROUND_TASK_ENUM, ON );
    #endif // EEPROMHANDLER_ENABLE_BACKGROUND_WRITES
  }
  else
  {
    // illegal address
    eError = EEPROM_ERR_ILLADDR;
  }

  // return the error
  return( eError );
}

#if ( EEPROMHANDLER_ENABLE_EMULATION == 0 )
/******************************************************************************
 * @function WriteBlock
 *
 * @brief write a block to the EEPROM
 *
 * This function will write a block of data to the EEPROM
 *
 * @param[in]   wAddress    address to write to
 * @param[in]   wBlkLength  length of the data to write
 * @param[in]   pnData      pointer to the data to write
 *
 * @return      error enumeration
 *
 *****************************************************************************/
static EEPROMERR WriteBlock( U16 wAddress, U8 nBlkLength, PU8 pnData )
{
  EEPROMERR eError = EEPROM_ERR_NONE;
  U32       uTime;

  // get the current time
  #if (SYSTEMDEFINE_OS_SELECTION != SYSTEMDEFINE_OS_MINIMAL )
  EepromHandler_LclInitialize( );
  #endif // SYSTEMDEFINE_OS_SELECTON != SYSTEMDEFINE_OS_TASKMINIMAL
  UTime = EEPROMHANDLER_GET_SYSTEMTIME( ) + WAIT_FOR_BUSY_DONE_TIME;

  // wait till done
  while(( bCheckForBusyRequired = CheckForBusy( )) == TRUE )
  {
    // check for timeout
  #if (SYSTEMDEFINE_OS_SELECTION != SYSTEMDEFINE_OS_MINIMAL )
  EepromHandler_LclInitialize( );
  #endif // SYSTEMDEFINE_OS_SELECTON != SYSTEMDEFINE_OS_TASKMINIMAL
    if ( EEPROMHANDLER_GET_SYSTEMTIME( ) >= uTime )
    {
      // timeout occured - flag error
      eError = EEPROM_ERR_DEVBUSY;
      break;
    }
  }

  // if error - do not execute
  if ( eError == EEPROM_ERR_NONE )
  {
    // set the address
    tXfrCtl.tAddress.uValue = wAddress;
    tXfrCtl.nAddrLen = EEPROMHANDLER_ADR_SIZE;

    // set the the device address
    tXfrCtl.nDevAddr = EEPROMHANDLER_DEV_ADDR;

    // set the pointer to the data/length
    tXfrCtl.pnData = pnData;
    tXfrCtl.wDataLen = nBlkLength;

    // calculate the read timeout
    tXfrCtl.uTimeout = WRBLOCK_MAX_TIMEOUT + COMPUTE_BLOCK_TIME( nBlkLength + EEPROMHANDLER_ADR_SIZE );

    // perform write
    #if (SYSTEMDEFINE_OS_SELECTION != SYSTEMDEFINE_OS_MINIMAL )
    #endif // SYSTEMDEFINE_OS_SELECTON != SYSTEMDEFINE_OS_TASKMINIMAL
    eError = ( EEPROMERR )I2c_Write( EEPROMHANDLER_I2C_ENUM, &tXfrCtl );

    // set the check for busy
    bCheckForBusyRequired = TRUE;
  }
  
  // return the error
  return( eError );
}
#endif // EEPROMHANDLER_ENABLE_EMULATION

#if (( EEPROMHANDLER_ENABLE_EMULATION == 0 ) || ( EEPROMHANDLER_ENABLE_BACKGROUND_WRITES == 1 ))
/******************************************************************************
 * @function CheckForBusy
 *
 * @brief checks to see if the EEPROM is busy executing the last requested function
 *
 * This function will check for busy
 *
 * @return      TRUE if busy, FALSE if not
 *
 *****************************************************************************/
static BOOL CheckForBusy( void )
{
  BOOL      bBusy;
  I2CCHKBSY tChkBusy;

  // only perform if required
  if ( bCheckForBusyRequired == TRUE )
  {
    // check for device presence
    tChkBusy.nDevAddr = EEPROMHANDLER_DEV_ADDR;
    tChkBusy.bReadMode = EEPROMHANDLER_I2C_POLL_MODE;
  #if (SYSTEMDEFINE_OS_SELECTION != SYSTEMDEFINE_OS_MINIMAL )
  EepromHandler_LclInitialize( );
  #endif // SYSTEMDEFINE_OS_SELECTON != SYSTEMDEFINE_OS_TASKMINIMAL
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
  }
  else
  {
    // clear busy
    bBusy = FALSE;
  }

  // return the busy state
  return( bBusy );
}
#endif // EEPROMHANDLER_ENABLE_EMULATION

#if ( EEPROMHANDLER_ENABLE_DEBUGCOMMANDS == 1 )
/******************************************************************************
 * @function CmdWrtEep
 *
 * @brief write a byte to the eeprom
 *
 * This function wites a byte to the eeprom
 *
 * @return  A
 *****************************************************************************/
static ASCCMDSTS CmdWrtEep( U8 nCmdEnum )
{
  ASCCMDSTS   eStatus = ASCCMD_STS_NONE;
  U32UN       tTemp;
  U16         wAddress;
  EEPROMERR   eError;
  PC8         pcBuffer;

  // get the buffer
  AsciiCommandHandler_GetBuffer( nCmdEnum, &pcBuffer );

  // get the address/data
  AsciiCommandHandler_GetValue( nCmdEnum, 0, &tTemp.uValue );
  wAddress = tTemp.awValue[ LE_U32_LSB_IDX ];
  AsciiCommandHandler_GetValue( nCmdEnum, 1, &tTemp.uValue );

  // now write the value
  // write the normal eeprom
  eError = EepromHandler_WrByte( wAddress, tTemp.anValue[ LE_U32_LSB_IDX ] );
  
  if ( eError != EEPROM_ERR_NONE )
  {
    // output the result
    SPRINTF_P( pcBuffer, ( char const * )g_szAsciiErrStrn, eError, eError );
    eStatus = ASCCMD_STS_OUTPUTBUFFER;
  }

  // return status
  return( eStatus );
}

/******************************************************************************
 * @function CmdRedEep
 *
 * @brief write a byte to the eeprom
 *
 * This function reads a byte from the eeprom
 *
 * @return  A
 *****************************************************************************/
static ASCCMDSTS CmdRedEep( U8 nCmdEnum )
{
  U32UN     tTemp;
  U16       wAddress;
  EEPROMERR eError;
  PC8       pcBuffer;

  // get the buffer
  AsciiCommandHandler_GetBuffer( nCmdEnum, &pcBuffer );

  // get the address/data
  AsciiCommandHandler_GetValue( nCmdEnum, 0, &tTemp.uValue );
  wAddress = tTemp.awValue[ LE_U32_LSW_IDX ];
 
  // read the normal eeprom
  eError = EepromHandler_RdByte( wAddress, &tTemp.anValue[ LE_U32_LSB_IDX ] );   
  
  // test for error
  if ( eError == EEPROM_ERR_NONE )
  {
    // output the result
    SPRINTF_P( pcBuffer, ( char const * )szRspRep, wAddress, tTemp.anValue[ LE_U32_LSB_IDX ] );
  }
  else
  {
    // output the error
    SPRINTF_P( pcBuffer, ( char const * )g_szAsciiErrStrn, eError, eError );
  }

  // return status
  return( ASCCMD_STS_OUTPUTBUFFER );
}

/******************************************************************************
 * @function CmdDmpEep
 *
 * @brief dumps the EEPROM to the display
 *
 * This function dumps the EEPROM to the display
 *
 * @return  A
 *****************************************************************************/
static ASCCMDSTS CmdDmpEep( U8 nCmdEnum )
{
  ASCCMDSTS eStatus = ASCCMD_STS_NONE;
  U32UN     tTemp;
  U16       wAddress;
  PU8       pnLclBuffer;
  EEPROMERR eError;
  PC8       pcBuffer;

  // get the buffer
  AsciiCommandHandler_GetBuffer( nCmdEnum, &pcBuffer );

  // get the base address, length
  AsciiCommandHandler_GetValue( nCmdEnum, 0, &tTemp.uValue );
  wAddress = tTemp.awValue[ LE_U32_LSW_IDX ];
  AsciiCommandHandler_GetValue( nCmdEnum, 1, &tTemp.uValue );

  // now allocate the local space for the buffer
  if (( pnLclBuffer = ( PU8 )malloc( tTemp.awValue[ LE_U32_LSW_IDX ] )) != NULL )
  {
    // now read the eeprom
    if (( eError = EepromHandler_RdBlock( wAddress, tTemp.awValue[ LE_U32_LSW_IDX ], pnLclBuffer )) == EEPROM_ERR_NONE )
    {
      // output the block
      AsciiCommandHandler_DisplayBlock( nCmdEnum, wAddress, pnLclBuffer, tTemp.awValue[ LE_U32_LSW_IDX ] );
    }
    else
    {
      // output the error
      SPRINTF_P( pcBuffer, ( char const * )g_szAsciiErrStrn, eError, eError );
      eStatus = ASCCMD_STS_OUTPUTBUFFER;
    }

    // now free the block of memory
    free( pnLclBuffer );
  }

  // return no error
  return( eStatus );
}

/******************************************************************************
 * @function CmdFilEep
 *
 * @brief fill the EEPROM to the display
 *
 * This function fills the EEPROM to the a given value
 *
 * @return  A
 *****************************************************************************/
static ASCCMDSTS CmdFilEep( U8 nCmdEnum )
{
  ASCCMDSTS eStatus = ASCCMD_STS_NONE;
  U32UN     tTemp;
  U16       wAddress;
  U16       wLength;
  PU8       pnLclBuffer;
  EEPROMERR eError;
  PC8       pcBuffer;

  // get the buffer
  AsciiCommandHandler_GetBuffer( nCmdEnum, &pcBuffer );

  // get the base address, length
  AsciiCommandHandler_GetValue( nCmdEnum, 0, &tTemp.uValue );
  wAddress = tTemp.awValue[ LE_U32_LSW_IDX ];
  AsciiCommandHandler_GetValue( nCmdEnum, 1, &tTemp.uValue );
  wLength = tTemp.awValue[ LE_U32_LSW_IDX ];
  AsciiCommandHandler_GetValue( nCmdEnum, 2, &tTemp.uValue );

  // now allocate the local space for the buffer
  if (( pnLclBuffer = ( PU8 )malloc( wLength )) != NULL )
  {
    // fill the buffer with the new value
    memset( pnLclBuffer, tTemp.anValue[ LE_U32_LSB_IDX ], wLength );
    
    // now read the eeprom
    if (( eError = EepromHandler_WrBlock( wAddress, wLength, pnLclBuffer )) != EEPROM_ERR_NONE )
    {
      // output the error
      SPRINTF_P( pcBuffer, ( char const * )g_szAsciiErrStrn, eError, eError );
      eStatus = ASCCMD_STS_OUTPUTBUFFER;
    }

    // now free the block of memory
    free( pnLclBuffer );
  }
  else
  {
    // set the error
    SPRINTF_P( pcBuffer, ( char const * )g_szAsciiErrStrn, 0xFF, 0xFF );
    eStatus = ASCCMD_STS_OUTPUTBUFFER;
  }

  // return no error
  return( eStatus );
}
#endif // EEPROMHANDLER_ENABLE_DEBUGCOMMANDS

/**@} EOF EepromHandler.c */
