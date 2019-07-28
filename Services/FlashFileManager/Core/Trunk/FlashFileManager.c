/******************************************************************************
 * @file FlashFileManager.c
 *
 * @brief Flash File Manager implementation
 *
 * This file provides the implementation of the Flash File Manager
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
 * \addtogroup FlashFileManager
 * @{
 *****************************************************************************/

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "FlashFileManager/FlashFileManager.h"

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------

/// define the maximum number of open files
#define	MAX_FILE_HANDLES                                            ( 4 )

/// define the size of the h

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------
/// define the flash file entry structure
typedef struct _FLASHFILEENTRY
{
  U32 uEntrySize;
  U32 uFileSizeSize;
  U32 uEntryOffset;
  U16 wNameLength;
} FLASHFILEENTRY, *PFLASHFILEENTRY;
#define	FLASHFILEENTRY_SIZE                         sizeof( FLASHFILEENTRY )

/// define the local control structure
typedef struct _LCLCTL
{
  BOOL  bInUse;                       ///< in use
  U32   uFileLength;                  ///< file length
  U32   uBaseAddress;                 ///< base address of file
  U32   uCurOffset;                   ///< current offset
} LCLCTL, *PLCLCTL;
#define	LCLCTL_SIZE                                 sizeof( LCLCTL )

// global parameter declarations ----------------------------------------------

// local parameter declarations -----------------------------------------------
static  LCLCTL  atLclCtls[ MAX_FILE_HANDLES ];
static  C8      acCurFileName[ FLASHFILE_MAX_FILE_NAME ];

// local function prototypes --------------------------------------------------
static  S16 FindEmptyControl( void );

// constant parameter initializations -----------------------------------------

/******************************************************************************
 * @function FlashFileManager_Initialize
 *
 * @brief file manager initialization
 *
 * This function will perform any needed initialization
 *
 *****************************************************************************/
void FlashFileManager_Initialize( void )
{
  // clear all controls
  memset( atLclCtls, 0, ( LCLCTL_SIZE * MAX_FILE_HANDLES ));
}

/******************************************************************************
 * @function FlashFileManager_Find
 *
 * @brief find a file
 *
 * This function will search for a file and return the approprite file handle
 *
 * @param[in]   pszFileName pointer to the file name to open
 *
 * @return      file handle or -1 if not found
 *
 *****************************************************************************/
FLASHFILEHANDLE	FlashFileManager_Find( PC8 pszFileName )
{
  FLASHFILEHANDLE   tHandle = -1;
  BOOL              bRunFlag = TRUE;
  U32               uCurrentAddress = 0;
  FLASHFILEENTRY    tFileEntry;
  PLCLCTL           ptLclCtl;	

  // loop
  while( bRunFlag )
  {
    // read a block
    FlashFileManager_LocalReadBlock( uCurrentAddress, ( PU8 )&tFileEntry, FLASHFILEENTRY_SIZE );

    // check for done
    if ( tFileEntry.uEntrySize != 0 )
    {
      // now read the file name
      FlashFileManager_LocalReadBlock( uCurrentAddress + FLASHFILEENTRY_SIZE, ( PU8 )&acCurFileName, tFileEntry.wNameLength );

      // compare the file name
      if ( strncmp( pszFileName, acCurFileName, tFileEntry.wNameLength ) == 0 )
      {
        // file name found-search for an empty file handle
        if (( tHandle = FindEmptyControl( )) != -1 )
        {
          // get a pointer to the control block
          ptLclCtl = &atLclCtls[ tHandle ];

          // copy the information from the file entry to the control structure
          ptLclCtl->uBaseAddress = tFileEntry.uEntryOffset;
          ptLclCtl->uCurOffset = 0;
          ptLclCtl->uFileLength = tFileEntry.uEntrySize;
        }

        // clear the run flag
        bRunFlag = FALSE;
      }
      else
      {
        // move to the next entry
        uCurrentAddress += tFileEntry.uEntrySize;
      }
    }
    else
    {
      // not found - clear run flag
      bRunFlag = 0;
    }
  }

  // return the handle
  return( tHandle );
}

/******************************************************************************
 * @function FlashFileManager_Read
 *
 * @brief read data from a file
 *
 * This function will read data from a file
 *
 * @param[in]   tHandle       file handle
 * @param[in]   pnBuffer      pointer to the data buffer
 * @param[in]   wLength       length of the data to read
 * @param[in]   puBytesRead   pointer to store the number of bytes read
 *
 * @return      appropriate error
 *
 *****************************************************************************/
FLASHFILEERROR FlashFileManager_Read( FLASHFILEHANDLE tHandle, PU8 pnBuffer, U16 wLength, PU16	puBytesRead )
{
  FLASHFILEERROR  eError = FLASHFILE_ERROR_NONE;
  PLCLCTL         ptLclCtl;	
  U32             uBytesRead;

  // ensure valid handle
  if ( tHandle != -1 )
  {
    // get a pointer to the control block
    ptLclCtl = &atLclCtls[ tHandle ];

    // check for end of file
    if ( ptLclCtl->uCurOffset < ptLclCtl->uFileLength )
    {
      // now set the number bytes to read
      uBytesRead = MIN(( ptLclCtl->uFileLength - ptLclCtl->uCurOffset ), wLength );

      // now read the bytes
      FlashFileManager_LocalReadBlock( ptLclCtl->uBaseAddress + ptLclCtl->uCurOffset, pnBuffer, uBytesRead );

      // adjust the current address
      ptLclCtl->uCurOffset += uBytesRead;
      *( puBytesRead ) = uBytesRead;
    }
    else
    {
      // return end of file
      eError = FLASHFILE_ERROR_ENDOFFILE;
    }
  }
  else
  {
    // return the illegal handle
    eError = FLASHFILE_ERROR_ILLHANDLE;
  }

  // return the error
  return( eError );
}

/******************************************************************************
 * @function FindEmptyControl
 *
 * @brief find an empty control block
 *
 * This function will search for an empty control block
 *
 * @return  file index or -1 if not found
 *
 *****************************************************************************/
static S16 FindEmptyControl( void )
{
  U8    nCtlIdx;
  S16   iHandle = -1;

  // for each control block
  for ( nCtlIdx = 0; nCtlIdx < MAX_FILE_HANDLES; nCtlIdx++ )
  {
    // set the pointer
    // check for non valid block
    if ( atLclCtls[ nCtlIdx ].bInUse == FALSE )
    {
      // found an empty one-mark it in use
      atLclCtls[ nCtlIdx ].bInUse = TRUE;
      iHandle = nCtlIdx;

      // exit
      break;
    }
  }

  // return the handle
  return( iHandle );
}

/**@} EOF FlashFileManager.c */
