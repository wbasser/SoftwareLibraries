/******************************************************************************
 * @file MMcSdHandler.h
 *
 * @brief MMC/SD card handler declarations 
 *
 * This file provides the declarations for the MMC/SD card handler
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
 * \addtogroup MMcSdHandler
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _MMCSDHANDLER_H
#define _MMCSDHANDLER_H

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "MMcSdHandler/MmcSdHandler_prm.h"

// library includes -----------------------------------------------------------
#include "TaskManager/TaskManager.h"

// Macros and Defines ---------------------------------------------------------
/// define the task time
#define MMCSDHANDLER_EXEC_RATE              ( TASK_TIME_MSECS( MMCSDHANDLER_TIMER_MSECS ))

/// define the number of events
#define MMCSDHANDLER_NUM_EVENTS             ( 2 )

/// define the block size
#define	MMCSDHANDLER_BLK_SIZE	              ( 512 )

/// define the status masks
#define MMCSDHANDLER_STS_NOINIT		          ( 0x01 )	///< Drive not initialized
#define MMCSDHANDLER_STS_NODISK		          ( 0x02 )	///< No medium in the drive
#define MMCSDHANDLER_STS_PROTECT		        ( 0x04 )	///< Write protected

/// Generic commands
#define MMCSDHANDLER_CTRL_SYNC			        ( 0 )		
#define MMCSDHANDLER_GET_SECTOR_COUNT	      ( 1 )		
#define MMCSDHANDLER_GET_SECTOR_SIZE	      ( 2 )
#define MMCSDHANDLER_GET_BLOCK_SIZE	        ( 3 )		
#define MMCSDHANDLER_CTRL_POWER		          ( 4 )
#define MMCSDHANDLER_CTRL_LOCK			        ( 5 )
#define MMCSDHANDLER_CTRL_EJECT		          ( 6 )

/// MMC/SDC command
#define MMCSDHANDLER_GET_TYPE		            ( 10 )
#define MMCSDHANDLER_GET_CSD			          ( 11 )
#define MMCSDHANDLER_GET_CID			          ( 12 )
#define MMCSDHANDLER_GET_OCR			          ( 13 )
#define MMCSDHANDLER_GET_SDSTAT	            ( 14 )

// enumerations ---------------------------------------------------------------
// enumerate the result
typedef enum 	_MMCSDHANDLERRESULT
{
  MMCSDHANDLER_RES_OK = 0,			      ///< 0: Successful
  MMCSDHANDLER_RES_ERROR,			        ///< 1: R/W Error
  MMCSDHANDLER_RES_WRPRT,			        ///< 2: Write Protected
  MMCSDHANDLER_RES_NOTRDY,			      ///< 3: Not Ready
  MMCSDHANDLER_RES_PARERR			        ///< 4: Invalid Parameter
} MMCSDHANDLERRESULT;


// structures -----------------------------------------------------------------

// global parameter declarations -----------------------------------------------

// global function prototypes --------------------------------------------------
extern	void 			          MmcSdHandler_Initialize( void );
extern	U8				          MmcSdHandler_InitializeDrive( U8 nDrive );
extern	BOOL			          MmcSdHandler_ProcessTimer( TASKARG xArg );
extern	U8				          MmcSdHandler_Status( U8 nDrive );
extern	MMCSDHANDLERRESULT	MmcSdHandler_Read( U8 nDrive, PU8 pnBuffer, U32 uSector, U8 nCount );
extern	MMCSDHANDLERRESULT	MmcSdHandler_Write( U8 nDrive, const U8 *pnBuffer, U32 uSector, U8 nCount );
extern	MMCSDHANDLERRESULT	MmcSdHandler_Ioctl( U8 nDrive, U8 nCmd, PVOID pvBUffer );
extern  PC8                 MmcSdHandler_GetErrorString( U8 nError );

/**@} EOF MMcSdHandler.h */

#endif  // _MMCSDHANDLER_H
