/******************************************************************************
 * @file MmcSdHandler.c
 *
 * @brief MMC/SD card handler implementation
 *
 * This file 
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
 *
 * \addtogroup MmcSdHandler
 * @{
 *****************************************************************************/

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "MmcSdHandler/MmcSdHandler.h"
#include "MmcSdHandler/MmcSdHandler_prm.h"

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------
// define the response length
#define	RESP_MSG_LEN	      ( 4 )

/// define the command buffer length
#define CMD_BUF_LEN         ( 6 )

// define the card type defs
#define	CARD_TYPE_MMC	      ( 0x01 )		// MMC card
#define	CARD_TYPE_SD1	      ( 0x02 )		// SD version 1
#define CARD_TYPE_SD2       ( 0x04 )    // SD version 2
#define	CARD_TYPE_BLK	      ( 0x08 )		// block mode
#define	CARD_TYPE_SDC	      ( CARD_TYPE_SD2 | CARD_TYPE_BLK )	// SD card

// define the commands
#define CMD0	              ( 0 )			  // GO_IDLE_STATE
#define CMD1	              ( 1 )			  // SEND_OP_COND ( MCC )
#define CMD8	              ( 8 )			  // SEND_IF_COND
#define CMD9	              ( 9 )			  // SEND_CSD
#define CMD10	              ( 10 )			// SEND_CID
#define CMD12	              ( 12 )			// STOP_TRANSMISSION
#define ACMD13              ( 0x80 + 13 )
#define CMD16	              ( 16 )			// SET_BLOCKLEN
#define CMD17	              ( 17 )			// READ_SINGLE_BLOCK
#define CMD18	              ( 18 )			// READ_MULTIPLE_BLOCK
#define CMD23	              ( 23 )			// SET_BLOCK_COUNT
#define ACMD23              ( 0x80 + 23 )
#define CMD24	              ( 24 )			// WRITE_BLOCK
#define CMD25	              ( 25 )			// WRITE_MULTIPLE_BLOCK
#define CMD32               ( 32 )      // ERASE_ER_BLK_START
#define CMD33               ( 33 )      // ERASE_ER_BLK_END
#define CMD38               ( 38 )      // ERASE
#define ACMD41              ( 0x80 + 41 )
#define CMD55	              ( 55 )			// APP_CMD
#define CMD58	              ( 58 )			// READ_OCR

/// define the number of error strings
#define NUM_ERROR_STRINGS   ( 20 )

/// define the number of dummy bytes for int
#define NUM_DUMMY_BYTES     ( 10 )      ///< 80 bits

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations ----------------------------------------------

// local parameter declarations -----------------------------------------------
static	VU8			nLclStatus;
static	VBOOL		bPowerFlag;
static	VU16		wTimer1;
static	VU16		wTimer2;
static	U8			nCardType;
static	U8			anResponse[ RESP_MSG_LEN ];
static  U8      anCmdBuffer[ CMD_BUF_LEN ];

// local function prototypes --------------------------------------------------
static	BOOL	RcvDataBlock( PU8 pnBUffer, U16 wCount );
static	BOOL	XmtDataBlock( PU8 pnBUffer, U8 nCmd );
static	U8		SendCmd( U8 nCmd, U32 uArg );
static	BOOL	PowerControl( BOOL bState );
static	U8		WaitReady( U16 wMilliseconds );
static	BOOL	Select( void );
static	void	Deselect( void );

// constant parameter initializations -----------------------------------------
/// define the error strings
static  const CODE C8 szErrOK[ ]  = {	"OK" };
static  const CODE C8 szDskErr[ ] = { "DISK_ERR" };
static  const CODE C8 szIntErr[ ] = { "INT_ERR" };
static  const CODE C8 szNotRdy[ ] = { "NOT_READY" };
static  const CODE C8 szNoFile[ ] = { "NO_FILE" };
static  const CODE C8 szNoPath[ ] = { "NO_PATH" };
static  const CODE C8 szInvNam[ ] = { "INVALID_NAME" };
static  const CODE C8 szDenied[ ] = { "DENIED" };
static  const CODE C8 szExist[ ] =  { "EXIST" };
static  const CODE C8 szInvObj[ ] = { "INVALID_OBJECT" };
static  const CODE C8 szWrtPrt[ ] = { "WRITE_PROTECTED" };
static  const CODE C8 szInvDrv[ ] = { "INVALID_DRIVE" };
static  const CODE C8 szNotEnb[ ] = { "NOT_ENABLED" };
static  const CODE C8 szNofSys[ ] = { "NO_FILE_SYSTEM" };
static  const CODE C8 szMkfAbr[ ] = { "MKFS_ABORTED" };
static  const CODE C8 szTimeot[ ] = { "TIMEOUT" };
static  const CODE C8 szLocked[ ] = { "LOCKED" };
static  const CODE C8 szNotCor[ ] = { "NOT_ENOUGH_CORE" };
static  const CODE C8 szTooFil[ ] = { "TOO_MANY_OPEN_FILES" };
  
/// define the string array
static  const CODE PC8 aszErrors[ NUM_ERROR_STRINGS ] =
{
  ( PC8 )&szErrOK,
  ( PC8 )&szDskErr,
  ( PC8 )&szIntErr,
  ( PC8 )&szNotRdy,
  ( PC8 )&szNoFile,
  ( PC8 )&szNoPath,
  ( PC8 )&szInvNam,
  ( PC8 )&szDenied,
  ( PC8 )&szExist,
  ( PC8 )&szInvObj,
  ( PC8 )&szWrtPrt,
  ( PC8 )&szInvDrv,
  ( PC8 )&szNotEnb,
  ( PC8 )&szNofSys,
  ( PC8 )&szMkfAbr,
  ( PC8 )&szTimeot,
  ( PC8 )&szLocked,
  ( PC8 )&szNotCor,
  ( PC8 )&szTooFil,
};

/******************************************************************************
 * @function MmcSdHandler_Initialize
 *
 * @brief initialize the system
 *
 * This function will clear any flags and set the local status
 *
 *****************************************************************************/
void MmcSdHandler_Initialize( void )
{
	// clear the power flag
	bPowerFlag = OFF;
	
	// set the status equal to no card inserted
	nLclStatus |= MMCSDHANDLER_STS_NODISK;

  // determine if there is no card detect
  #if ( MCSDHANDLER_CARDDETECT_MODE != MMCSDHANDLER_CARDDETECT_MODE_NONE )
  BOOL  bCardPresent;

	// scan for card insertion/removal
  Gpio_Get( MMCSDHANDLER_CARD_DETECT_ENUM, &bCardPresent );
	if ( bCardPresent )
	{
		// clear no disk
		nLclStatus &= ~MMCSDHANDLER_STS_NODISK;
	}
	else
	{
		// set no disk
		nLclStatus |= MMCSDHANDLER_STS_NODISK | MMCSDHANDLER_STS_NOINIT;
	}
  #else
  // force a card inserted
  nLclStatus &= ~MMCSDHANDLER_STS_NODISK;
  #endif  // MCSDHANDLER_CARDDETECT_MODE != MMCSDHANDLER_CARDDETECT_MODE_NONE
}

/******************************************************************************
 * @function MmcSdHandler_ProcessTImer
 *
 * @brief process the background timer
 *
 * This function will process the background timers
 *
 * @param[in]   xArg        task aragument
 *
 * @return      TRUE        flush the queue
 *
 *****************************************************************************/
BOOL MmcSdHandler_ProcessTimer( TASKARG xArg )
{
	// decrment timer 1 if time remaining
	if ( wTimer1 != 0 )
		wTimer1--;

	// decrment timer 2 if time remaining
	if ( wTimer2 != 0 )
		wTimer2--;
  
  #if ( MCSDHANDLER_CARDDETECT_MODE == MMCSDHANDLER_CARDDETECT_MODE_POLL )
  BOOL  bCardPresent;

	// scan for card insertion/removal
  Gpio_Get( MMCSDHANDLER_CARD_DETECT_ENUM, &bCardPresent );
	if ( bCardPresent )
	{
		// clear no disk
		nLclStatus &= ~MMCSDHANDLER_STS_NODISK;

    // post the task
    TaskManager_PostEvent( MCSDHANDLER_CARDDETECT_TASK_ENUM, MCDSDHANDLER_CARDDETECT_INSERTED_EVENT );
	}
	else
	{
		// set no disk
		nLclStatus |= MMCSDHANDLER_STS_NODISK | MMCSDHANDLER_STS_NOINIT;

    // post the task
    TaskManager_PostEvent( MCSDHANDLER_CARDDETECT_TASK_ENUM, MCDSDHANDLER_CARDDETECT_REMOVED_EVENT );
	}
  #endif // MCSDHANDLER_CARDDETECT_MODE == MMCSDHANDLER_CARDDETECT_POLL

  // always return true
  return( TRUE );
}

 
/******************************************************************************
 * @function MmcSdHandler_InitializeDrive
 *
 * @brief drive initialization
 *
 * This function will initialize the drive 
 *
 * @param[in]   nDrive        selected drive
 *
 * @return      appropriate status
 *
 *****************************************************************************/
U8 MmcSdHandler_InitializeDrive( U8 nDrive )
{
	U8	  nStatus = MMCSDHANDLER_STS_NOINIT;
	U8		nType = 0;
	U8		nCmd;
  U16   wSpeed;
  U8    anBuffer[ NUM_DUMMY_BYTES ];
  
	// only supports drive 0
	if ( nDrive == 0 )
	{
    // power off
    PowerControl( OFF );
    
		// check for card present
		if ( !( nLclStatus & MMCSDHANDLER_STS_NODISK ))
		{
			// turn on the power
			PowerControl( ON );
      
      // set the slow clock
      wSpeed = 64;
      Spi_Ioctl( MMCSDHANDLER_SPI_DEV_ENUM, SPI_IOCTLACTIONS_SETSPEED, ( PVOID ) &wSpeed );      

      // send the initial clock train
      memset( anBuffer, 0xFF, NUM_DUMMY_BYTES );
      Spi_WriteBlock( MMCSDHANDLER_SPI_DEV_ENUM, anBuffer, NUM_DUMMY_BYTES, FALSE );

      // enter idle state
      if ( SendCmd( CMD0, 0 ) == 1 )
      {
        // set the timer for 1 second
        wTimer1 = 1000 / MMCSDHANDLER_TIMER_MSECS;

        // check SDC Ver 2+
        if ( SendCmd( CMD8, 0x1AA ) == 1 )
        {
          // get the response
          Spi_ReadBlock( MMCSDHANDLER_SPI_DEV_ENUM, 0xFF, anResponse, RESP_MSG_LEN );

          // check for card voltage 2.7V - 3.6V
          if (( anResponse[ 2 ] == 0x01 ) && ( anResponse[ 3 ] == 0xAA ))
          {
            // send ACMD41 with HCS bit and check proper response
            while(( wTimer1 != 0 ) && SendCmd( ACMD41, 1UL << 30 ));
            
            // check for timer remaining and command 58
            if (( wTimer1 != 0 ) && ( SendCmd( CMD58, 0 ) == 0 ))
            {
              // get the response
              Spi_ReadBlock( MMCSDHANDLER_SPI_DEV_ENUM, 0xFF, anResponse, RESP_MSG_LEN );
  
              // set the type
              nType = ( anResponse[ 0 ] & 0x40 ) ? CARD_TYPE_SDC : CARD_TYPE_SD2;
            }
          }
        }
        else
        {
          if ( SendCmd( ACMD41, 0 ) <= 1 )
          {
            // SDC Ver1 or MMC
            nType = CARD_TYPE_SD1;
            nCmd = ACMD41;
          }
          else
          {
            // MMCV3
            nType = CARD_TYPE_MMC;
            nCmd = CMD1;
          }
          
          // exit idle loop
          while(( wTimer1 != 0 ) && ( SendCmd( nCmd, 0 )));
          
          // check for timeout
          if (( wTimer1 != 0 ) || ( SendCmd( CMD16, 512 ) != 0 ))
          {
            nType = 0;
          }
				}
	
				// copy the local card type
				nCardType = nType;
	
				// deselect
				Deselect( );
	
				// check for a valid type
				if ( nType != 0 )
				{
					// set valid status/set fast clock
					nLclStatus  &= ~MMCSDHANDLER_STS_NOINIT;
          wSpeed = 4;
          Spi_Ioctl( MMCSDHANDLER_SPI_DEV_ENUM, SPI_IOCTLACTIONS_SETSPEED, ( PVOID ) &wSpeed );      
				}
				else
				{
					// turn power off
					PowerControl( OFF );
				}
			}
		}
			
		// copy the status to local status
		nStatus = nLclStatus;
	}

	// return the current status
	return( nStatus );
}

/******************************************************************************
 * @function MmcSdHandler_Status
 *
 * @brief returns the status
 *
 * This function willl determine if valid drive return stats
 *
 * @param[in]   nDrive
 *
 * @return      
 *
 *****************************************************************************/
U8 MmcSdHandler_Status( U8 nDrive )
{
	U8  nStatus = MMCSDHANDLER_STS_NOINIT;

	// check for only drive 0
	if ( nDrive == 0 )
	{
		// set the return
		nStatus = nLclStatus;
	}

	// return the status
	return( nStatus );
}

/******************************************************************************
 * @function MmcSdHandler_Read
 *
 * @brief read a sector
 *
 * This function will read a sector from the MMC/SD card
 *
 * @param[in]   nDrive      drive number
 * @param[in]   pnBuffer    pointer to the data buffer
 * @pamar[in]   uSector     sector to read
 * @param[in]   nCount      size of the read buffer
 *
 * @return      appropriate result
 *
 *****************************************************************************/
MMCSDHANDLERRESULT MmcSdHandler_Read( U8 nDrive, PU8 pnBuffer, U32 uSector, U8 nCount )
{
	MMCSDHANDLERRESULT eResult;

	// check for valid parameters
	if (( nDrive == 0 ) && ( nCount != 0 ))
	{
		// check for disk initialized
		if (( nLclStatus & MMCSDHANDLER_STS_NOINIT ) == 0 )
		{
      // convert to byte address if needed
      if ( !( nCardType & CARD_TYPE_BLK ))
      {
        // convert to sector address
        uSector *= MMCSDHANDLER_BLK_SIZE;
      }
      
      // check for single block
      if ( nCount == 1 )
      {
        // single block read
        if (( SendCmd( CMD17, uSector ) == 0 ) && ( RcvDataBlock( pnBuffer, MMCSDHANDLER_BLK_SIZE )))
        {
          // clear the count
          nCount = 0;
        }
      }
      else
      {
        // multiple block read
        if ( SendCmd( CMD18, uSector ) == 0 )
        {
          // loop
          do 
          {
            // if error - break
            if ( !RcvDataBlock( pnBuffer, MMCSDHANDLER_BLK_SIZE ))
            {
              // exit
              break;
            }
            else
            {
              // adjust buffer pointer
              pnBuffer += MMCSDHANDLER_BLK_SIZE;
            }
          }
          while ( --nCount );

          // terminate read
          SendCmd( CMD12, 0 );
        }
      }

      // deselect/idle
      Deselect( );

      // set the result based on count
      eResult = ( nCount != 0 ) ? MMCSDHANDLER_RES_ERROR : MMCSDHANDLER_RES_OK;
		}
		else
		{
			// return not ready
			eResult = MMCSDHANDLER_RES_NOTRDY;
		}
	}
	else
	{
		// set the parmaeter error
		eResult = MMCSDHANDLER_RES_PARERR;
	}

	// return the result
	return( eResult );
}

/******************************************************************************
 * @function MmcSdHandler_WRite
 *
 * @brief write a sector
 *
 * This function will write a sector to the MMC/SD card
 *
 * @param[in]   nDrive      drive number
 * @param[in]   pnBuffer    pointer to the data buffer
 * @pamar[in]   uSector     sector to write
 * @param[in]   nCount      size of the write buffer
 *
 * @return      appropriate result
 *
 *****************************************************************************/
MMCSDHANDLERRESULT MmcSdHandler_Write( U8 nDrive, const U8 *pnBuffer, U32 uSector, U8 nCount )
{
	MMCSDHANDLERRESULT eResult;

	// check for valid parameters
	if (( nDrive == 0 ) && ( nCount != 0 ))
	{
		// check for disk initialized
		if ( !( nLclStatus & MMCSDHANDLER_STS_NOINIT ))
		{
			// check for protected
			if ( nLclStatus & MMCSDHANDLER_STS_PROTECT )
			{
				// convert to byte address if needed
				if ( !( nCardType & CARD_TYPE_BLK ))
				{
					// convert to sector address
					uSector *= MMCSDHANDLER_BLK_SIZE;
				}

				// check for single block
				if ( nCount == 1 )
				{
					// single block write
					if (( SendCmd( CMD24, uSector ) == 0 ) && ( XmtDataBlock(( PU8 )pnBuffer, 0xFE )))
					{
						// clear the count
						nCount = 0;
					}
				}
				else
				{
					// multiple block write
					if ( nCardType & CARD_TYPE_SDC )
					{
						// send the commands for multiple blocks
            SendCmd( ACMD23, nCount );
					}
					
					// start write
					if ( SendCmd( CMD25, uSector ) == 0 )
					{
						// loop
						do 
						{
							// if error - break
							if ( !XmtDataBlock(( PU8 )pnBuffer, 0xFC ))
								break;

							// adjust buffer
							pnBuffer += MMCSDHANDLER_BLK_SIZE;

						}
						while ( --nCount );

						// send stop token
						if ( !( XmtDataBlock( NULL, 0xFD ))) 
						{
							// set count to 1 to indicate error
							nCount = 1;
						}
					}
				}

				// deselect/ idle 
				Deselect( );

				// set the result based on count
				eResult = ( nCount != 0 ) ? MMCSDHANDLER_RES_ERROR : MMCSDHANDLER_RES_OK;
			}
			else
			{
				// set protected status
				eResult = MMCSDHANDLER_RES_WRPRT;
			}
		}
		else
		{
			// return not ready
			eResult = MMCSDHANDLER_RES_NOTRDY;
		}
	}
	else
	{
		// set the parmaeter error
		eResult = MMCSDHANDLER_RES_PARERR;
	}

	// return the result
	return( eResult );
}

/******************************************************************************
 * @function MmcSdHandler_Ioctl
 *
 * @brief perform an control funciton on the card
 *
 * This function will perform an control function on the card
 *
 * @param[in]   nDrive      drive number
 * @param[in]   nCmd        command
 * @param[io]   pvBuffer    pointer to the data value
 *
 * @return      appropriate result
 *
 *****************************************************************************/
MMCSDHANDLERRESULT MmcSdHandler_Ioctl( U8 nDrive, U8 nCmd, PVOID pvBuffer )
{
	MMCSDHANDLERRESULT	eResult = MMCSDHANDLER_RES_ERROR;
	U8		              nShift, anCsd[ 16 ];
	U16		              wSize;

	// check for valid drive
	if ( nDrive != 0 )
	{
		// check for a power command
		if ( nCmd == MMCSDHANDLER_CTRL_POWER )
		{
			// check for what we need to do
			switch( *(( PU8 )pvBuffer ))
			{
				case 0 :	// power off
					// if power on
					if ( bPowerFlag )
					{
						// turn it off
						PowerControl( OFF );
						eResult = MMCSDHANDLER_RES_OK;
					}
					break;

				case 1 :	// power on
					// turn it on
					PowerControl( ON );
					eResult = MMCSDHANDLER_RES_OK;
					break;

				case 2 :	// power get
					*(( PU8  )pvBuffer + 1 ) = bPowerFlag;
					eResult = MMCSDHANDLER_RES_OK;
					break;

				default :
					eResult = MMCSDHANDLER_RES_PARERR;
					break;
			}
		}
		else
		{
			// check for not initialized
			if ( nLclStatus & MMCSDHANDLER_STS_NOINIT )
			{
				// set the result
				eResult = MMCSDHANDLER_RES_NOTRDY;
			}
			else
			{
				// select the device
				Select( );

				// process the sub command
				switch( nCmd )
				{
					case MMCSDHANDLER_GET_SECTOR_COUNT :
						// get the sector count
						if (( SendCmd( CMD9, 0 ) == 0 ) && ( RcvDataBlock( &anCsd[ 0 ], 16 )))
						{
							// check for SDC Ver 2.00
							if (( anCsd[ 0 ] >> 6 ) == 1 )	
							{
								// compute the size
								wSize = anCsd[ 9 ] + (( U16 )anCsd[ 8 ] << 8 ) + 1;
								*( PU32 )pvBuffer = ( U32 )( wSize << 10 );
							}
							else
							{
								// MMC or SDC ver 1.xx
								nShift = ( anCsd[ 5 ] & 0x0F ) + (( anCsd[ 10 ] & 0x80 ) >> 7 ) + (( anCsd[ 9 ] & 0x03 ) << 1 ) + 2;
								wSize = ( anCsd[ 8 ] >> 6 ) + (( U16 )anCsd[ 7 ] << 2 ) + (( U16 )anCsd[ 6 ] & 0x03 << 10 ) + 1;
								*( PU32 )pvBuffer = ( U32 )( wSize << ( nShift - 9 ));
							}
							
							// set ok result
							eResult = MMCSDHANDLER_RES_OK;
						}
						break;

					case MMCSDHANDLER_GET_SECTOR_SIZE :
						// set the sector size
						*( PU16 )pvBuffer = MMCSDHANDLER_BLK_SIZE;
						eResult = MMCSDHANDLER_RES_OK;
						break;

					case MMCSDHANDLER_CTRL_SYNC :
						// check for ready
						if ( Select( ))
						{
							// set good response
							eResult = MMCSDHANDLER_RES_OK;
						}
						break;

					case MMCSDHANDLER_GET_CSD :
						// get the current CSD
						if (( SendCmd( CMD9, 0 ) == 0 ) && ( RcvDataBlock( pvBuffer, 16 )))
							eResult = MMCSDHANDLER_RES_OK;
						break;

					case MMCSDHANDLER_GET_CID :
						// get the current CID
						if (( SendCmd( CMD10, 0 ) == 0 ) && ( RcvDataBlock( pvBuffer, 16 )))
							eResult = MMCSDHANDLER_RES_OK;
						break;

					case MMCSDHANDLER_GET_OCR :
						// get the current OCR
						if ( SendCmd( CMD58, 0 ) == 0 )
						{
							// get the response
              Spi_ReadBlock( MMCSDHANDLER_SPI_DEV_ENUM, 0xFF, anResponse, RESP_MSG_LEN );

							// for each byte
							for ( nShift = 0; nShift < RESP_MSG_LEN; nShift++ )
							{
								// copy the data
								*( PU8 )( pvBuffer + nShift ) = anResponse[ nShift ];
							}
							eResult = MMCSDHANDLER_RES_OK;
						}
						break;
						
					case MMCSDHANDLER_GET_TYPE :
						// return the type
						*( PU8 )pvBuffer = nCardType;
						break;
						
					default :
						eResult = MMCSDHANDLER_RES_PARERR;
						break;
				}

				// deselect/idle
				Deselect( );
			}
		}
	}
	else
	{
		// return parameter error
		eResult = MMCSDHANDLER_RES_PARERR;
	}

	// return the result
	return( eResult );
}

/******************************************************************************
 * @function MmcSdHandler_GetErrorString
 *
 * @brief returns a string
 *
 * This function will return the string for a given error
 *
 * @param[in]   nError
 *
 * @return      the pointer to the stirng
 *
 *****************************************************************************/
PC8 MmcSdHandler_GetErrorString( U8 nError )
{
  PC8  pvString = NULL;
  
  // check for valid
  if ( nError < NUM_ERROR_STRINGS )
  {
    // get the pointer to the string
    pvString =  ( PC8 )PGM_RDWORD( aszErrors[ nError ] );  
  }
  
  // return the string
  return( pvString );  
}

/******************************************************************************
 * @function RcvDataBlock
 *
 * @brief get a data block
 *
 * This function will read a block of data
 *
 * @param[io]   pnBuffer    pointer to the block
 * @param[in]   wCount      count
 *
 * @return      TRUE if no errors, FALSE otherwise
 *
 *****************************************************************************/
static BOOL RcvDataBlock( PU8 pnBuffer, U16 wCount )
{
	BOOL	bStatus = FALSE;
	U8		nResponse;

	// wait till ready 200
	wTimer1 = 200 / MMCSDHANDLER_TIMER_MSECS;
	do
	{
    // get the response
    Spi_Read( MMCSDHANDLER_SPI_DEV_ENUM, 0xFF, &nResponse );
	}
	while(( nResponse == 0xFF ) && ( wTimer1 != 0 ));

	// check for okay
	if ( nResponse == 0xFE )
	{
		// read a buffer
    Spi_ReadBlock( MMCSDHANDLER_SPI_DEV_ENUM, 0xFF, pnBuffer, wCount );

		// discard the CRC
    Spi_Read( MMCSDHANDLER_SPI_DEV_ENUM, 0xFF, &nResponse );
    Spi_Read( MMCSDHANDLER_SPI_DEV_ENUM, 0xFF, &nResponse );

		// set good status
		bStatus = TRUE;
	}

	// return the status
	return( bStatus );
}

/******************************************************************************
 * @function XmtDataBlock
 *
 * @brief write a data block
 *
 * This function will write a block of data
 *
 * @param[io]   pnBuffer    pointer to the block
 * @param[in]   wCount      count
 *
 * @return      TRUE if no errors, FALSE otherwise
 *
 *****************************************************************************/
static BOOL XmtDataBlock( PU8 pnBuffer, U8 nCmd )
{
	BOOL	bStatus = FALSE;
  U8    nResponse;

	// wait for ready
	if ( WaitReady( 500 ) == TRUE )
	{
		// transmit the token
    Spi_Write( MMCSDHANDLER_SPI_DEV_ENUM, nCmd );

		// is this a data toekn
		if ( nCmd != 0xFD )
		{
			// write the buffer
      Spi_WriteBlock( MMCSDHANDLER_SPI_DEV_ENUM, pnBuffer, MMCSDHANDLER_BLK_SIZE, FALSE );

			// send CRC
      Spi_Write( MMCSDHANDLER_SPI_DEV_ENUM, 0xFF );
      Spi_Write( MMCSDHANDLER_SPI_DEV_ENUM, 0xFF );

			// get the response
      Spi_Read( MMCSDHANDLER_SPI_DEV_ENUM, 0xFF, &nResponse );
			if (( nResponse & 0x1F ) == 0x05 )
			{
				// set good status
				bStatus = TRUE;
			}
		}
	}

	// return the status
	return( bStatus );
}

/******************************************************************************
 * @function SendCmd
 *
 * @brief send a command to the device
 *
 * This function will send a command to the device and wait for a response
 *
 * @param[in]   nCmd      command to send
 * @param[in]   uArg      arugment
 *
 * @return      the read response
 *
 *****************************************************************************/
static U8 SendCmd( U8 nCmd, U32 uArg )
{
  U8  nResponse, nTemp;
  
  // check for accelerated comand
  if ( nCmd & 0x80 )
  {
    nCmd &= 0x7F;
    if (( nResponse = SendCmd( CMD55, 0 )) > 1 )
    {
      return( nResponse );
    }
  }

  // ensure de-selected
  if ( nCmd != CMD12 )
  {
    // deselect it
    Deselect( );

    // now-reselect it
    if ( Select( ) == 0 )
    {
      // return 0xFF
      return( 0xFF );
    }
  }

  // send the command packet
  anCmdBuffer[ 0 ] = 0x40 | nCmd;
  anCmdBuffer[ 1 ] = uArg >> 24;
  anCmdBuffer[ 2 ] = uArg >> 16;
  anCmdBuffer[ 3 ] = uArg >> 8;
  anCmdBuffer[ 4 ] = uArg & 0xFF;

  // determine the CRC
  switch( nCmd )
  {
    case CMD0 :
      anCmdBuffer[ 5 ] = 0x95;
      break;
        
    case CMD8 :
      anCmdBuffer[ 5 ] = 0x87;
      break;
        
    default :
      anCmdBuffer[ 5 ] = 1;
  }
    
  // now output it
  Spi_WriteBlock( MMCSDHANDLER_SPI_DEV_ENUM, anCmdBuffer, CMD_BUF_LEN, FALSE );
    
  // check for stop read command
  if ( nCmd == CMD12 )
  {
    // dummy read
    Spi_Read( MMCSDHANDLER_SPI_DEV_ENUM, 0xFF, &nResponse );
  }
    
  // retry ten times
  nTemp = 10;
  do
  {
    Spi_Read( MMCSDHANDLER_SPI_DEV_ENUM, 0xFF, &nResponse );
  } while((( nResponse & 0x80 ) == 0x80 ) && ( --nTemp != 0 ));
    
  // return the response
  return( nResponse );
}

/******************************************************************************
 * @function PowerControl
 *
 * @brief power control
 *
 * This function will turn power off/on to the MMC/SD card
 *
 * @param[in]       bState    on/off state
 *
 * @return          status
 *
 *****************************************************************************/
static BOOL PowerControl( BOOL bState )
{
	BOOL	bStatus = FALSE; 
	
  #if ( MMCSDHANDLER_POWERCONTROL_ENABLE == 1 )
	// determine state
	if ( bState )
	{
		// check for card present
		if ( !( nLclStatus & MMCSDHANDLER_STS_NODISK ))
		{
			// turn on the power control/delay 30MSEC/initialize the SPI/send clock train/set the power flag on
			Gpio_Set( MMCSDHANDLER_POWER_CONTROL_ENUM, ON );
      wTimer2 = 30 / MMCSDHANDLER_TIMER_MSECS;
      while( wTimer2 != 0 );
			bPowerFlag = ON;
		}
		else
		{
			bStatus = TRUE;
		}
	}
	else
	{
		// tun off ofwer/turn off the SPI/
		Gpio_Set( MMCSDHANDLER_POWER_CONTROL_ENUM, OFF );
		bPowerFlag = OFF;
	}
	#endif
  
	// return status
	return( bStatus );
}

/******************************************************************************
 * @function Select
 *
 * @brief select the device
 *
 * This function will enable the chip select and wait for ready
 *
 * @return      TRUE if successful, FALSE if not
 *
 *****************************************************************************/
static BOOL Select( void )
{
  BOOL bStatus;
  
  // chip select
  Gpio_Set( MMCSDHANLDER_CHIP_SELECT_ENUM, ON );

  // write a dummy byte
  Spi_Write( MMCSDHANDLER_SPI_DEV_ENUM, 0XFF );
  
  // wait for ready 
  if (( bStatus = WaitReady( 500 )) == FALSE )
  {
    // deselct it
    Deselect( );
  }
  
  // return the status
  return( bStatus );
}

/******************************************************************************
 * @function Deselect
 *
 * @brief disable the chip select
 *
 * This function will clear the chip select and reset the SPI
 *
 *****************************************************************************/
static void Deselect( void )
{
	U8  nTemp;

	// deselect the device/idle the line
  Gpio_Set( MMCSDHANLDER_CHIP_SELECT_ENUM, OFF );
	Spi_Read( MMCSDHANDLER_SPI_DEV_ENUM, 0xFF, &nTemp );
}

/******************************************************************************
 * @function WaitReady
 *
 * @brief wait for ready
 *
 * This function will set up a timer, read the device and check for ready
 *
 * @return      TRUE if OK, FALSE if error
 *
 *****************************************************************************/
static BOOL WaitReady( U16 wMilliseconds )
{
	U8  nTemp;

	// set the timer for the desired time
	wTimer2 = wMilliseconds / MMCSDHANDLER_TIMER_MSECS;

	// loop
	do
	{
		// get the result
		Spi_Read( MMCSDHANDLER_SPI_DEV_ENUM, 0xFF, &nTemp );
	} 
	while(( nTemp != 0xFF ) && ( wTimer2 != 0 ));

	// return the read value
	return(( nTemp == 0xFF ) ? TRUE : FALSE );
}

/**@} EOF MmcSdHandler.c */
