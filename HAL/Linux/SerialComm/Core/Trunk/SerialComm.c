/******************************************************************************
 * @file SerialComm.c
 *
 * @brief SerialComm implementation 
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
 * \addtogroup SerialComm
 * @{
 *****************************************************************************/

// system includes ------------------------------------------------------------
#include <pthread.h>
#include <termios.h>

// local includes -------------------------------------------------------------
#include "SerialComm/SerialComm.h"

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------
/// define the local buffer size
#define	LCL_BUF_SIZE			( 255 )

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------
/// define the local control structure
typedef struct _LCLCTL
{
  BOOL                    bChannelOk;       ///< channel config OK
  PU8                     pnRcvBuffer;      ///< pointer to the receive buffer
  U16                     wRcvWrIndex;      ///< buffer write index
  U16                     wRcvRdIndex;      ///< buffer read index
  U16                     wBufCount;        ///< buffer count
  U16                     wBufSize;         ///< buffer size
  int                     iFileDescriptor;  ///< handle
  PVSERIALCOMMIRQCALLBACK pvCallback;       ///< event callback handler
  
} LCLCTL, *PLCLCTL;
#define LCLCTL_SIZE           sizeof( LCLCTL )

// global parameter declarations ----------------------------------------------

// local parameter declarations -----------------------------------------------
static  LCLCTL            atLclCtls[ SERIALCOMM_DEV_ENUM_MAX ];
static  int               iLclSignal;
static  struct sigaction  tCurSigAction;
static  struct sigaction  tPrvSigAction;

// local function prototypes --------------------------------------------------
static  void ReadHandler( int iSigNumber, siginfo_t *ptInfo, PVOID pvIgnored );

// constant parameter initializations -----------------------------------------
static const U32  auBaudConstants[ SERIALCOMM_BAUDRATE_MAX ] =
{
  B300,
  B600,
  B1200,
  B1800,
  B2400,
  B4800,
  B9600,
  B19200,
  B38400,
  B57600,
  B115200,
  B230400
};

static const U32  auCharSize[ SERIALCOMM_WLEN_MAX ] =
{
  CS5,
  CS6,
  CS7,
  CS8
};

/******************************************************************************
 * @function SerialComm_Initialize
 *
 * @brief initialization
 *
 * This function will initialize the SERIALCOMM to its defined operating
 * characteristics
 *
 *****************************************************************************/
void SerialComm_Initialize( void )
{
  SERIALCOMMDEVENUM eSerialComm;
  PSERIALCOMMDEF    ptDef;
  PLCLCTL           ptCtl;
  struct termios    tOptions;
  int               iCurSignal;
  BOOL              bFound;
  
  // first see if we can get a signal
  iLclSignal = 0;
  bFound = FALSE;
    
  // loop through all available signals
  for ( iCurSignal = SIGRTMIN; iCurSignal <= SIGRTMAX, bFound == FALSE; iCurSignal++ )
  do
  {
    // get previous sig action
    sigaction( iCurSignal, 0, &tPrvSigAction );
    
    // is this empty
    if ( tPrvSigAction .sa_handler == 0 )
    {
      // we found an empty slot
      iLclSignal = iCurSignal;
      tCurSigAction.sa_sigaction = ReadHandler;
      tCurSigAction.sa_flags = SA_SIGINFO;
      tCurSigAction.sa_restorer = NULL;
      sigemptyset( &tCurSigAction.sa_mask );
      sigaction( iLclSignal, &tCurSigAction, 0 );
      
      // set the found flag
      bFound = TRUE;
    }
    else
    {
      // not empty - restore original
      sigaction( iLclSignal, &tPrvSigAction, 0 );
    }
  };
  
  // if found - process rest
  if ( bFound )
  {
    // for each enumerated channel
    for ( eSerialComm = 0; eSerialComm < SERIALCOMM_DEV_ENUM_MAX; eSerialComm++ )
    {
      // get a pointer to the definition/control
      ptDef = ( PSERIALCOMMDEF )&atSerialCommDefs[ eSerialComm ];
      ptCtl = &atLclCtls[ eSerialComm ];
      
      // clear the read thread/pointer
      memset( ptCtl, 0, LCLCTL_SIZE );
      
      // clear the channel ok flag
      ptCtl->bChannelOk = FALSE;
      
      // open the port
      if (( ptCtl->iFileDescriptor = open( ptDef->pszChanName, O_RDWR | O_NOCTTY | O_NONBLOCK )) != -1 )
      {
        // set up for event driven
        fcntl( ptCtl->iFileDescriptor, F_SETSIG, iLclSignal );
        fcntl( ptCtl->iFileDescriptor, F_SETOWN, getpid( ));
        fcntl( ptCtl->iFileDescriptor, F_SETFL, O_ASYNC | ) | NONBLOCK );
        
        // get the default options
        tcgetattr( ptCtl->iFileDescriptor, &tOptions );
        
        // set the baudrate/wordlength/stop pits/parity
        tOptions.c_cflag = CLOCAL | CREAD;
        tOptions.c_cflag |= auBaudConstants[ ptDef->eBaudRate ];
        tOptions.c_cflag |= ( ptDef->eStopBits == SERIALCOMM_STOP_1 ) ? CSTOPB : 0;
        tOptions.c_cflag |= auCharSize[ ptDef->eWordLen ];
        switch( ptDef->eParity )
        {
          case SERIALCOMM_PARITY_EVEN :
            tOptions.c_cflag |= PARENB;
            break;
            
          case SERIALCOMM_PARITY_ODD :
            tOptions.c_cflag |= ( PARENB | PARODD );
            break;
            
          case SERIALCOMM_PARITY_NONE :
          default :
            break;
        }

        tOptions.c_iflag = IGNPAR;
        tOptions.c_oflag = 0;
        tOptions.c_lflag = 0;
        tOptions.c_cc[ VMIN ] = 1;
        tOptions.c_cc[ VTIME ] = 0;
        
        // flush it/set the attributes
        tcflush( ptCtl->iFileDescriptor, TCIFLUSH );
        tcsetattr( ptCtl->iFileDescriptor, TCSANOW, &tOptions );
        
        // now check for callback or local buffer
        if ( ptDef->pvCallback != NULL )
        {
          // store the callback in the control
          ptCtl->pvCallback = ptDef->pvCallback;
          
          // set the ok status
          ptCtl->bChannelOk = TRUE;
        }
        else
        {
          // create the buffer
          if (( ptCtl->pnRcvBuffer = malloc( ptDef->wRxBufSize )) != NULL )
          {
            // clear the indices
            ptCtl->wRcvWrIndex = ptCtl->wRcvRdIndex = ptCtl->wBufCount = 0;
            ptCtl->wBufSize = ptDef->wRxBufSize;
            
            // set the ok status
            ptCtl->bChannelOk = TRUE;
          }
        }
      }
    }
  }
}

/******************************************************************************
 * @function SerialComm_Close
 *
 * @brief close a channel
 *
 * This function will kll the read thread if active and exit
 *
 * @param[in]   eDev        device
 *
 * @return      appropriate error
 *
 *****************************************************************************/
SERIALCOMMERR SerialComm_Close( SERIALCOMMDEVENUM eDev )
{
  PVOID         pvThreadResult;
  SERIALCOMMERR eStatus = SERIALCOMM_ERR_CLOSEFAIL;
  PLCLCTL       ptCtl;
  
  // check for a valid device
  if ( eDev < SERIALCOMM_DEV_ENUM_MAX )
  {
    // get a pointer
    ptCtl = &atLclCtls[ eDev ];
    
    // close the comm port
    close( ptCtl->iFileDescriptor );
  }
  else
  {
    // set the illegal device status
    eStatus = SERIALCOMM_ERR_ILLDEV;
  }
  
  // return the status
  return( eStatus );
}

/******************************************************************************
 * @function SerialComm_Read
 *
 * @brief read bytes from the SERIALCOMM
 *
 * This function will read bytes from the SERIALCOMM buffers
 *
 * @param[in]   eDev        device
 * @param[in]   pnData      pointer to the data storage
 * @param[in]   nLength     length of the data storage, number of bytes to read
 * @param[io]   pnBytesRead set to the number of actual bytes read
 *
 * @return      appropriate error
 *
 *****************************************************************************/
SERIALCOMMERR SerialComm_Read( SERIALCOMMDEVENUM eDev, PU8 pnData, U16 wLength, PU16 pwBytesRead )
{
  SERIALCOMMERR eError = SERIALCOMM_ERR_NONE;
  PLCLCTL       ptCtl;
  U16           wBufIdx, wBytesToCopy;
  
  // in all cases, clear the bytes read
  *( pwBytesRead ) = 0;

  // check for valid device enumerator
  if ( eDev < SERIALCOMM_DEV_ENUM_MAX )
  {
    // get the pointer to the control structure
    ptCtl = &atLclCtls[ eDev ];
    
    // are there any characters to read
    if ( ptCtl->wBufCount != 0 )
    {
      // compute the number of bytes to copy
      wBytesToCopy = MIN( wLength, ptCtl->wBufCount );
      *( pwBytesRead ) = wBytesToCopy;
      
      while( wBytesToCopy-- != 0 )
      {
        // copy the data
        *( pnData++ ) = *( ptCtl->pnRcvBuffer + ptCtl->wRcvRdIndex++ );
        
        // adjust the read pointer
        if ( ptCtl->wRcvRdIndex >= ptCtl->wBufSize )
        {
          // roll-over back to 0
          ptCtl->wRcvRdIndex = 0;
        }
        
        // adjust the count
        ptCtl->wBufCount--;
      }
    }
  }
  else
  {
    // set the error
    eError = SERIALCOMM_ERR_ILLDEV;
  }
  
  // return the error
  return( eError );
}

/******************************************************************************
 * @function SerialComm_Write
 *
 * @brief write data to the SERIALCOMM
 *
 * This function 
 *
 * @param[in]   eDev        device
 * @param[in]   pnData      pointer to the data storage
 * @param[in]   wLength     length of the data storage, number of bytes to write
 * @param[io]   pwBytesRead set to the number of actual bytes written
 *
 * @return      appropriate error
 *
 *****************************************************************************/
SERIALCOMMERR SerialComm_Write( SERIALCOMMDEVENUM eDev, PU8 pnData, U16 wLength, PU16 pwBytesWritten )
{
  SERIALCOMMERR eError = SERIALCOMM_ERR_NONE;
  PLCLCTL       ptCtl;
  
  // in all cases, clear the bytes read
  *( pwBytesWritten ) = 0;

  // check for valid device enumerator
  if ( eDev < SERIALCOMM_DEV_ENUM_MAX )
  {
    // get the pointer to the control structure
    ptCtl = &atLclCtls[ eDev ];
    
    // now write the data
    *( pwBytesWritten ) = write( ptCtl->iFileDescriptor, pnData, wLength );
  }
  else
  {
    // set the error
    eError = SERIALCOMM_ERR_ILLDEV;
  }
  
  // return the error
  return( eError );
}

/******************************************************************************
 * @function SerialComm_Ioctl
 *
 * @brief 
 *
 * This function 
 *
 * @param[in]   
 *
 * @return      
 *
 *****************************************************************************/
SERIALCOMMERR SerialComm_Ioctl( SERIALCOMMDEVENUM eDev, SERIALCOMMACTION eAction, PVOID pvData )
{
  SERIALCOMMERR eError = SERIALCOMM_ERR_NONE;
  
  // check for a valid SERIALCOMM
  if ( eDev < SERIALCOMM_DEV_ENUM_MAX )
  {
  }
  else
  {
    // set the error
    eError = SERIALCOMM_ERR_ILLDEV;
  }

  // return the error
  return( eError );
}

/******************************************************************************
 * @function ReadThread 
 *
 * @brief background read thread
 *
 * This function sits in a loop and attempts to read characters from the serial
 * port, decodes them and will set the receive message semaphore if a valid
 * message is received
 *
 * @param[in]   pvArg     pointer to an argument
 *
 * @return      pointer to a void
 *
 *****************************************************************************/
static  void ReadHandler( int iSigNumber, siginfo_t *ptInfo, PVOID pvIgnored )
{
  U8                anLclBuffer[ LCL_BUF_SIZE ];
  U8                nBufIndex;
  int               iBytesRead;
  PLCLCTL           ptCtl;
  SERIALCOMMDEVENUM eDev;
  
  // is this an incoming signal
  if ( ptInfo->si_code = POLL_IN )
  {
    // there is some data - find the device
    for ( eDev = 0; eDev < SERIALCOMM_DEV_ENUM_MAX; eDev++ )
    {
      // get the pointer to the control structure
      ptCtl = &atLclCtls[ eDev ];
      
      // this is our device
      if ( ptCtl->iFileDescriptor == ptInfo->si_fd )
      {
        // read characters
        if (( iBytesRead = read( ptCtl->iFileDescriptor, ( PVOID )anLclBuffer, LCL_BUF_SIZE )) > 0 )
        {
          // now for each byte process
          for( nBufIndex = 0; nBufIndex < iBytesRead; nBufIndex++ )
          {
            // check for callback or local buffer
            if ( ptCtl->pvCallback != NULL )
            {
              // process it
              ptCtl->pvCallback( anLclBuffer[ nBufIndex ] );
            }
            else
            {
              // add to queue if room
              if ( ptCtl->wBufCount < ptCtl->wBufSize )
              {
                // stuff it/increment pointer/count
                ptCtl->pnRcvBuffer[ ptCtl->wRcvWrIndex++ ] = anLclBuffer[ nBufIndex ];
                ptCtl->wBufCount++;
              }
            }
          }
        }
        
        // break out of loop
        break;
      }
    }
  }
}

/**@} EOF SerialComm.c */

