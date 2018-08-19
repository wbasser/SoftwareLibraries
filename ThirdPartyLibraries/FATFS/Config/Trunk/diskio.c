/*-----------------------------------------------------------------------*/
/* Low level disk I/O module skeleton for FatFs     (C)ChaN, 2013        */
/*-----------------------------------------------------------------------*/
/* If a working storage control module is available, it should be        */
/* attached to the FatFs via a glue function rather than modifying it.   */
/* This is an example of glue functions to attach various exsisting      */
/* storage control module to the FatFs module with a defined API.        */
/*-----------------------------------------------------------------------*/

#include "diskio.h"		/* FatFs lower layer API */

#include "MmcSdHandler/MmcSdHandler.h"
/*-----------------------------------------------------------------------*/
/* Inidialize a Drive                                                    */
/*-----------------------------------------------------------------------*/

DSTATUS disk_initialize (
	BYTE pdrv				/* Physical drive nmuber (0..) */
)
{
	DSTATUS stat;

  // call the handler
  stat = MmcSdHandler_Initialize( pdrv );
  return( stat );
}



/*-----------------------------------------------------------------------*/
/* Get Disk Status                                                       */
/*-----------------------------------------------------------------------*/

DSTATUS disk_status (
	BYTE pdrv		/* Physical drive nmuber (0..) */
)
{
	DSTATUS stat;
  
  // call the handler
  stat = MmcSdHandler_Status( pdrv );
  return ( stat );
}



/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */
/*-----------------------------------------------------------------------*/

DRESULT disk_read (
	BYTE pdrv,		/* Physical drive nmuber (0..) */
	BYTE *buff,		/* Data buffer to store read data */
	DWORD sector,	/* Sector address (LBA) */
	UINT count		/* Number of sectors to read */
)
{
	DRESULT res;
  
  // call the handler
  res = MmcSdHandler_Read( pdrv, buff, sector, count );
  
	return ( res );
}



/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */
/*-----------------------------------------------------------------------*/

#if _USE_WRITE
DRESULT disk_write (
	BYTE pdrv,			/* Physical drive nmuber (0..) */
	const BYTE *buff,	/* Data to be written */
	DWORD sector,		/* Sector address (LBA) */
	UINT count			/* Number of sectors to write */
)
{
	DRESULT res;
  
  // call the handler
  res = MmcSdHandler_Write( pdrv, buff, sector, count );

	return ( res );
}
#endif


/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */
/*-----------------------------------------------------------------------*/

#if _USE_IOCTL
DRESULT disk_ioctl (
	BYTE pdrv,		/* Physical drive nmuber (0..) */
	BYTE cmd,		/* Control code */
	void *buff		/* Buffer to send/receive control data */
)
{
	DRESULT res;
  
  // call the handler
  res = MmcSdHandler_Ioctl( pdrv, cmd, buff );
  
	return ( res );
}
#endif

//DWORD get_fattime (void)
//{
//  return	  ((DWORD)(2012 - 1980) << 25)	/* Year = 2012 */
//  | ((DWORD)1 << 21)				/* Month = 1 */
//  | ((DWORD)1 << 16)				/* Day_m = 1*/
//  | ((DWORD)0 << 11)				/* Hour = 0 */
//  | ((DWORD)0 << 5)				/* Min = 0 */
//  | ((DWORD)0 >> 1);				/* Sec = 0 */
//}

