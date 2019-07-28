/******************************************************************************
 * @file AdestoAT25SFxx.c
 *
 * @brief Adesto AT25SFxx  implemenation
 *
 * This file provides the implementation for the Adesto ATSG2Fxx series of 
 * serial memory devices
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
 * \addtogroup Adesto AT25SFxx 
 * @{
 *****************************************************************************/

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "AdestoAT25SFxx/AdestoAT25SFxx.h"

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------
/// define the commands
#define	CMD_READ_ARRAY                      ( 0x03 )
#define	CMD_ERASE_BLK4K                     ( 0x20 )
#define	CMD_ERASE_BLK32K                    ( 0x52 )
#define	CMD_ERASE_BLK64K                    ( 0xD8 )
#define	CMD_ERASE_CHIP                      ( 0x60 )
#define	CMD_WRITE_PAGE                      ( 0x02 )
#define	CMD_READ_STSREG1                    ( 0x05 )
#define CMD_READ_MANUDEVID                  ( 0x9F )

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------
/// define the command structure
typedef struct _LCLCMD
{
  U8  nCommand;         ///< command
  U8  nAddrMSB;		///< address[23:16]
  U8  nAddrMS1;		///< address[15:8]
  U8  nAddrLSB;		///< address[7:0]
} LCLCMD, *PLCLCMD;
#define	LCLCMD_SIZE                     sizeof( LCLCMD )

// global parameter declarations ----------------------------------------------

// local parameter declarations -----------------------------------------------

// local function prototypes --------------------------------------------------
static	ADESTOAT25SERR	WaitTillDone( U32 uTimeoutMilliseoncds );

// constant parameter initializations -----------------------------------------

/******************************************************************************
 * @function AdestoAT25SFxx_Initialize
 *
 * @brief initialization
 *
 * This function will perform any needed initialization
 *
 *****************************************************************************/
void AdestoAT25SFxx_Initialize( void )
{
  // call the local initialize
  AdestoAT25SFxx_LocalInitialize( );
}

/******************************************************************************
 * @function AdestoAT25SFxx_ReadDevId
 *
 * @brief read a device ID
 *
 * This function will read a device ID
 *
 * @param[io]   pnManufId     pointer to the manufacture ID
 * @param[io]	pnDevID1      pointer to the device ID 1
 * @param[io]	pnDevID1      pointer to the device ID 1
 *
 * @return      approrpiate error
 *
 *****************************************************************************/
ADESTOAT25SERR AdestoAT25SFxx_ReadDevId( PU8 pnManufId, PU8 pnDevID1, PU8 pnDevID2 )
{
  U8  anLclBuffer[ 3 ];

  // set the command
  anLclBuffer[ 0 ] = CMD_READ_MANUDEVID;

  // chip select it
  AdestoAT25SFxx_ChipSelect( ON );
  AdestoAT25SFxx_SpiWrite( anLclBuffer, 1 );
  AdestoAT25SFxx_SpiRead( anLclBuffer, 3 );
  AdestoAT25SFxx_ChipSelect( OFF );

  // copy the vaoues
  *( pnManufId ) = anLclBuffer[ 0 ];
  *( pnDevID1 ) = anLclBuffer[ 1 ];
  *( pnDevID2 ) = anLclBuffer[ 2 ];

  // return no error
  return( ADESTOAT25S_ERR_NONE );
}

/******************************************************************************
 * @function AdestoAT25SFxx_Read
 *
 * @brief read a block
 *
 * This function will read a block of data from the given address
 *
 * @param[in]   uAddress      address of the block of data
 * @param[in]	pnData        pointer to the data
 * @param[in]	wLength       length of the data read
 *
 * @return      approrpiate error
 *
 *****************************************************************************/
ADESTOAT25SERR AdestoAT25SFxx_Read( U32 uAddress, PU8 pnData, U16 wLength )
{
  ADESTOAT25SERR  eError = ADESTOAT25S_ERR_NONE;
  LCLCMD          tLclCmd;
  U8              nDummy = 0;

  // check for valid address
  if (( uAddress + wLength ) < ADESTOAT25SF_MAX_ADDRESS )
  {
    tLclCmd.nCommand = CMD_READ_ARRAY;
    tLclCmd.nAddrMSB = ( uAddress >> 16 ) & 0xFF;
    tLclCmd.nAddrMS1 = ( uAddress >> 8 ) & 0xFF;
    tLclCmd.nAddrLSB = uAddress & 0xFF;

    // chip select it
    AdestoAT25SFxx_ChipSelect( ON );

    // write the data
    AdestoAT25SFxx_SpiWrite(( PU8 )&tLclCmd, LCLCMD_SIZE );

    // write a dummy byte
    AdestoAT25SFxx_SpiWrite(( PU8 )&nDummy, 1 );

    // now read the data
    AdestoAT25SFxx_SpiRead( pnData, wLength );

    // deselect it
    AdestoAT25SFxx_ChipSelect( OFF );
  }
  else
  {
    // report the error
    eError = ADESTOAT25S_ERR_ILLADDR;
  }

  // return the error
  return( eError );
}

/******************************************************************************
 * @function AdestoAT25SFxx_Write
 *
 * @brief read a block
 *
 * This function will write a block of data from the given address
 *
 * @param[in]   uAddress          address of the block of data
 * @param[in]   pnData            pointer to the data
 * @param[in]	wLength           length of the data read
 *
 * @return      approrpiate error
 *
 *****************************************************************************/
ADESTOAT25SERR AdestoAT25SFxx_Write( U32 uAddress, PU8 pnData, U16 wLength )
{
  ADESTOAT25SERR  eError = ADESTOAT25S_ERR_NONE;
  LCLCMD          tLclCmd;

  // check for valid address
  if (( uAddress + wLength ) < ADESTOAT25SF_MAX_ADDRESS )
  {
    // check for a valid page length
    if ( wLength <= ADESTOAT25SF_PAGE_SIZE )
    {
      tLclCmd.nCommand = CMD_WRITE_PAGE;
      tLclCmd.nAddrMSB = ( uAddress >> 16 ) & 0xFF;
      tLclCmd.nAddrMS1 = ( uAddress >> 8 ) & 0xFF;
      tLclCmd.nAddrLSB = uAddress & 0xFF;

      // chip select it
      AdestoAT25SFxx_ChipSelect( ON );

      // write the command
      AdestoAT25SFxx_SpiWrite(( PU8 )&tLclCmd, LCLCMD_SIZE );
      AdestoAT25SFxx_SpiWrite( pnData, wLength );

      // deselect it
      AdestoAT25SFxx_ChipSelect( OFF );

      // now wait till done
      eError = WaitTillDone( 5 );
    }
    else
    {
      // set the error
      eError = ADESTOAT25S_ERR_ILLPAGESIZE;
    }
  }
  else
  {
    // report the error
    eError = ADESTOAT25S_ERR_ILLADDR;
  }

  // return the error
  return( eError );
}

/******************************************************************************
 * @function AdestoAT25SFxx_Erase
 *
 * @brief erase data
 *
 * This function will erase either a page/block4/block32/chip
 *
 * @param[in]   eEraseType          erase type
 * @param[in]   uAddress            address of the block of data
 *
 * @return      appropriate error
 *
 *****************************************************************************/
ADESTOAT25SERR AdestoAT25SFxx_Erase( ADESTOAT24SERASE eEraseType, U32 uAddress )
{
  ADESTOAT25SERR  eError = ADESTOAT25S_ERR_NONE;
  LCLCMD          tLclCmd;
  U32             uTimeoutMsecs;
  U8              nDummy = 0;

  // check for an appropriate erase type
  if ( eEraseType < ADESTOAT24S_ERASE_MAX )
  {
    // determine the command
    switch( eEraseType )
    {
      case ADESTOAT24S_ERASE_BLOCK4K :
        tLclCmd.nCommand = CMD_ERASE_BLK4K;
        uTimeoutMsecs = 300;
        break;

      case ADESTOAT24S_ERASE_BLOCK32K :
        tLclCmd.nCommand = CMD_ERASE_BLK32K;
        uTimeoutMsecs = 1300;
        break;

      case ADESTOAT24S_ERASE_BLOCK64K :
        tLclCmd.nCommand = CMD_ERASE_BLK64K;
        uTimeoutMsecs = 3000;
        break;

      case ADESTOAT24S_ERASE_CHIP :
        tLclCmd.nCommand = CMD_ERASE_CHIP;
        uTimeoutMsecs = 20000;
        break;

      default :
        break;
    }

    // now set the address
    tLclCmd.nAddrMSB = ( uAddress >> 16 ) & 0xFF;
    tLclCmd.nAddrMS1 = ( uAddress >> 8 ) & 0xFF;
    tLclCmd.nAddrLSB = uAddress & 0xFF;

    // chip select it
    AdestoAT25SFxx_ChipSelect( ON );

    // write the data
    AdestoAT25SFxx_SpiWrite(( PU8 )&tLclCmd, LCLCMD_SIZE );

    // write a dummy byte
    AdestoAT25SFxx_SpiWrite(( PU8 )&nDummy, 1 );

    // deselect it
    AdestoAT25SFxx_ChipSelect( OFF );

    // now wait till done
    eError = WaitTillDone( uTimeoutMsecs );
  }
  else
  {
    // report error
    eError = ADESTOAT25S_ERR_ILLERASETYPE;
  }

  // return the error
  return( eError );
}

/******************************************************************************
 * @function WaitTillDone
 *
 * @brief poll for operation complete
 *
 * This function will poll the status register for completion
 *
 * @param[in]   uTimeoutMsecs		timeout in milliseconds
 *
 * @return      appropriate error
 *
 *****************************************************************************/
static ADESTOAT25SERR WaitTillDone( U32 uTimeoutMsecs )
{
  ADESTOAT25SERR  eError = ADESTOAT25S_ERR_NONE;
  BOOL            bRunFlag = TRUE;
  U32             uTime;  
  U8              nStatus, nCommand;
 
  // get the running time
  uTime = AdestoAT25SFxx_GetSystemTimeMsecs( ) + uTimeoutMsecs;

  // while loop
  while( bRunFlag == TRUE )
  {
    // read the status register
    nCommand = CMD_READ_STSREG1;

    // chip select it
    AdestoAT25SFxx_ChipSelect( ON );
    AdestoAT25SFxx_SpiWrite( &nCommand, 1 );
    AdestoAT25SFxx_SpiRead( &nStatus, 1 );
    AdestoAT25SFxx_ChipSelect( OFF );

    // are we still busy
    if (( nStatus & 0x01 ) == FALSE )
    {
      // clear the flag
      bRunFlag = FALSE;
    }
    else
    {
      // check for timeout
      if ( AdestoAT25SFxx_GetSystemTimeMsecs( ) >= uTime )
      {
        // timeout occured - flag error
        bRunFlag = FALSE;
        eError = ADESTOATF25S_ERR_TIMEOUT;
      }
    }
  }

  // return the results
  return( eError );
}

/**@} EOF Adesto AT25SFxx .c */
