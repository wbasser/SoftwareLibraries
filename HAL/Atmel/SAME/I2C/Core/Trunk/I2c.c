/******************************************************************************
 * @file I2C.c
 *
 * @brief I2C implementaion
 *
 * This file provides the implementation
 *
 * @copyright Copyright (c) 2014 CyberIntegration
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
 * \addtogroup I2C
 * @{
 *****************************************************************************/

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "I2C/I2c.h"

// library includes -----------------------------------------------------------
#include "Clock/Clock.h"
#include "SystemTick/SystemTick.h"
#if ( I2C_ENABLE_DEBUG == 1 )
#include "DebugManager/DebugManager.h"
#endif // I2C_ENABLE_DEBUG
#if ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_FREERTOS )
#include "FreeRtos/FreeRTOS.h"
#endif // SYSTEMDEFINE_OS_SELECTION

// Macros and Defines ---------------------------------------------------------
/// define the baud rate values for slow/fast
#define BAUD_RATE_SLOW          ( 100000l )
#define BAUD_RATE_FAST          ( 400000l )

/// define the clock calculation values
#define CLOCK_DIVIDER           ( 2 )
#define CLOCK_CALC_ARGU         ( 4 )
#define CLOCK_DIV_MAX           ( 0xFF )
#define CLOCK_DIV_MIN           ( 7 )

/// define the byte timeout
#define BYTE_TIMEOUT_VAL        ( 30000 )

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------
/// define the local control structure
typedef struct _LCLCTLS
{
  PU8     	pnData ;      ///< pointer to the data storage
  U16      	wCurLen;      ///< current receive/transmit length
  U16      	wBufIdx;      ///< buffer index
  I2CERR  	eError;       ///< current error
  Twihs*    ptTwi;        ///< pointer to the TWI
  #if ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_FREERTOS )
  xSemaphoreHandle xI2cMutexSemaphore;
  #endif // SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_FREERTOS
} LCLCTL, *PLCLCTL;
#define LCLCTLS_SIZE    sizeof( LCLCTLS )

// local parameter declarations -----------------------------------------------
static  LCLCTL      atLclCtls[ I2C_CHAN_MAX ];

// local function prototypes --------------------------------------------------
static  void  GetTwiChannel( I2CCHAN eChan, PLCLCTL ptCtl );
static  void  SetBaudrateClock( Twihs* ptTwi, U32 uBaud );
static  void  ProcessComplete( I2CERR eError, PI2CDEF ptDef, PLCLCTL ptCtl );
static  BOOL  WaitStatusReady( Twihs* ptTwi, U32 uMask, U32 uTimeout );

/******************************************************************************
 * @function I2c_Initialize
 *
 * @brief I2C initialization
 *
 * This function all devices in the table
 *
 *****************************************************************************/
void I2c_Initialize( void )
{
  PI2CDEF       ptDef;
  I2CDEVENUM    eDev;
  PLCLCTL       ptCtl;
  U32           uTemp;
  
  // for each device
  for ( eDev = 0; eDev < I2C_DEV_ENUM_MAX; eDev++ )
  {
    // get pointer to the definition structure
    ptDef = ( PI2CDEF )&atI2cDefs[ eDev ];
    ptCtl = &atLclCtls[ eDev ];
    GetTwiChannel( ptDef->eChan, ptCtl );

    #if ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_FREERTOS )
    ptCtl->xI2cMutexSemaphore = xSemaphoreCreateMutex( );
    #endif // SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_FREERTOS
 
    // disable all interrupts/dummy read of status
    ptCtl->ptTwi->TWIHS_IDR = 0xFFFFFFFF;
    uTemp = ptCtl->ptTwi->TWIHS_SR;
    
    // reset the device
    ptCtl->ptTwi->TWIHS_CR = TWIHS_CR_SWRST;
    uTemp = ptCtl->ptTwi->TWIHS_RHR;
    
    // enable the master mode
    ptCtl->ptTwi->TWIHS_CR = TWIHS_CR_MSDIS;
    ptCtl->ptTwi->TWIHS_CR = TWIHS_CR_SVDIS;
    ptCtl->ptTwi->TWIHS_CR = TWIHS_CR_MSEN;
    
        // now set the baudrate
    uTemp = ( ptDef->bFastSpeed ) ? BAUD_RATE_FAST : BAUD_RATE_SLOW;
    SetBaudrateClock( ptCtl->ptTwi, uTemp );
  }
}

/******************************************************************************
 * @function I2c_CLoseAll
 *
 * @brief I2C close all channels
 *
 * This function turns off all I2C channels
 *
 *****************************************************************************/
void I2c_CloseAll( void )
{
  PI2CDEF       ptDef;
  PLCLCTL       ptCtl;
  I2CDEVENUM    eDev;
  
  // for each device
  for ( eDev = 0; eDev < I2C_DEV_ENUM_MAX; eDev++ )
  {
    // get pointer to the definition structure
    ptDef = ( PI2CDEF )&atI2cDefs[ eDev ];
    ptCtl = &atLclCtls[ eDev ];

    // disable the control and wait for done
    ptCtl->ptTwi->TWIHS_CR = TWIHS_CR_SWRST;
    
    // delete the semaphore
    #if ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_FREERTOS )
    vSemaphoreDelete( ptCtl->xI2cMutexSemaphore );
    #endif // SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_FREERTOS
  }
}

/******************************************************************************
 * @function I2c_Write
 *
 * @brief write some characters to the I2C
 *
 * This function will write some characters to the I2C buffer if room
 *
 * @param[in]   eDev        Device enumeration
 * @param[in]   ptXfrCtl    pointero to the transfer control  
 *
 * @return      appropriate error value
 *
 *****************************************************************************/
I2CERR I2c_Write( I2CDEVENUM eDev, PI2CXFRCTL ptXfrCtl )
{
  I2CERR 	eError = I2C_ERR_NONE;
  PI2CDEF ptDef;
  PLCLCTL ptCtl;
  U32     uStatus;
  U32     uTimeout;

  // check for a valid I2C
  if ( eDev < I2C_DEV_ENUM_MAX )
  {
    // get pointer to the definition structure
    ptDef = ( PI2CDEF )&atI2cDefs[ eDev ];
    ptCtl = &atLclCtls[ eDev ];
    
    // take the sempahore
    #if ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_FREERTOS )
    xSemaphoreTake( ptCtl->xI2cMutexSemaphore, portMAX_DELAY );
    #endif // SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_FREERTOS
    
    // copy the values to the local controls
    ptCtl->pnData = ptXfrCtl->pnData;
    ptCtl->wCurLen = ptXfrCtl->wDataLen;
    ptCtl->wBufIdx = 0;

    // clear the error 
    ptCtl->eError = I2C_ERR_NONE;
    
    // set the address size/device address
    ptCtl->ptTwi->TWIHS_MMR = 0;
    ptCtl->ptTwi->TWIHS_MMR = TWIHS_MMR_DADR( ptXfrCtl->nDevAddr ) | BITS( ptXfrCtl->nAddrLen, TWIHS_MMR_IADRSZ_Pos );
    ptCtl->ptTwi->TWIHS_MMR &= ~TWIHS_MMR_MREAD;

    #if ( I2C_ENABLE_DEBUG == 1 )
    DebugManager_AddElement( I2C_DEBUG_BASE_VALUE + 1, ( ptXfrCtl->nDevAddr << 8 ) | ( ptXfrCtl->wDataLen + ptXfrCtl->nAddrLen ));
    #endif // I2C_ENABLE_DEBUG

    // test for zero address length
    if ( ptXfrCtl->nAddrLen != 0 )
    {
      // set the address
      ptCtl->ptTwi->TWIHS_IADR = 0;
      ptCtl->ptTwi->TWIHS_IADR = ptXfrCtl->tAddress.uValue;
    }

    // while there is data to send
    while(( ptCtl->wCurLen != 0 ) && ( eError == I2C_ERR_NONE ))
    {
      // reset the timeout
      uTimeout = BYTE_TIMEOUT_VAL;

      // get the status
      uStatus = ptCtl->ptTwi->TWIHS_SR;

      // now test for nack
      if ( uStatus & TWIHS_SR_NACK )
      {
        // set the error
        eError = I2C_ERR_SLVNAK;
      }
      else if ( uStatus & TWIHS_SR_TXRDY )
      {
        // write the data/decrement the length
        ptCtl->ptTwi->TWIHS_THR = *( ptCtl->pnData + ptCtl->wBufIdx++ );
        ptCtl->wCurLen--;
      }
      else if ( --uTimeout == 0 )
      {
        // set the error
        eError = I2C_ERR_TIMEOUT;
      }
    }

    // send a stop/wait for complete/clear status
    ptCtl->ptTwi->TWIHS_CR = TWIHS_CR_STOP;
    while( !( ptCtl->ptTwi->TWIHS_SR & TWIHS_SR_TXCOMP ));
    uStatus = ptCtl->ptTwi->TWIHS_SR;

    #if ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_FREERTOS )
    // release the semaphore
    xSemaphoreGive( ptCtl->xI2cMutexSemaphore );
    #endif // SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_FREERTOS
  }
  else
  {
    // illegal device
    eError = I2C_ERR_ILLDEV;
  }

  #if ( I2C_ENABLE_DEBUG == 1 )
  DebugManager_AddElement( I2C_DEBUG_BASE_VALUE + 2, eError );
  #endif // I2C_ENABLE_DEBUG

  // return the status
  return( eError );
}

/******************************************************************************
 * @function I2c_Read
 *
 * @brief read some characters from the I2C
 *
 * This function will read bytes from the buffer
 *
 * @param[in]   eDev        Device enumeration
 * @param[in]   ptXfrCtl    pointero to the transfer control  
 *
 * @return      appropriate error value
 *
 *****************************************************************************/
I2CERR I2c_Read( I2CDEVENUM eDev, PI2CXFRCTL ptXfrCtl )
{
  I2CERR  eError = I2C_ERR_NONE;
  PI2CDEF ptDef;
  PLCLCTL ptCtl;
  U32     uStatus;
  U32     uTimeout;
  
  // check for a valid I2C
  if ( eDev < I2C_DEV_ENUM_MAX )
  {
    ptDef = ( PI2CDEF )&atI2cDefs[ eDev ];
    ptCtl = &atLclCtls[ eDev ];
    
    // take the sempahore
    #if ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_FREERTOS )
    xSemaphoreTake( ptCtl->xI2cMutexSemaphore, portMAX_DELAY );
    #endif // SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_FREERTOS
    
    // copy the values to the local controls
    ptCtl->pnData = ptXfrCtl->pnData;
    ptCtl->wCurLen = ptXfrCtl->wDataLen;
    ptCtl->wBufIdx = 0;

    // clear the error 
    ptCtl->eError = I2C_ERR_NONE;
    
    // set the address size/device address
    ptCtl->ptTwi->TWIHS_MMR = TWIHS_MMR_DADR( ptXfrCtl->nDevAddr ) | BITS( ptXfrCtl->nAddrLen, TWIHS_MMR_IADRSZ_Pos ) | TWIHS_MMR_MREAD;

    #if ( I2C_ENABLE_DEBUG == 1 )
    DebugManager_AddElement( I2C_DEBUG_BASE_VALUE + 3, ( ptXfrCtl->nDevAddr << 8 ) | ( ptXfrCtl->wDataLen + ptXfrCtl->nAddrLen ));
    #endif // I2C_ENABLE_DEBUG

    // check for zero address
    if ( ptXfrCtl->nAddrLen != 0 )
    {
      // set the address
      ptCtl->ptTwi->TWIHS_IADR = 0;
      ptCtl->ptTwi->TWIHS_IADR = ptXfrCtl->tAddress.uValue;
    }

    // set both start/stop
    ptCtl->ptTwi->TWIHS_CR = TWIHS_CR_START;

    // wait for RDY/NAK
    while(( eError == I2C_ERR_NONE ) && ( ptCtl->wCurLen != 0 ))
    {
      // if lenght is 1 - send stop
      if ( ptCtl->wCurLen == 1 )
      {
        // set stop
        ptCtl->ptTwi->TWIHS_CR = TWIHS_CR_STOP;
      }

      // reset timeout
      uTimeout = BYTE_TIMEOUT_VAL;
      while( --uTimeout != 0 )
      {
        // get the status
        uStatus = ptCtl->ptTwi->TWIHS_SR;
      
        // check for NAK
        if ( uStatus & TWIHS_SR_NACK )
        {
          // set the error
          eError = I2C_ERR_SLVNAK;
          break;
        }
        else if ( uStatus & TWIHS_SR_RXRDY )
        {
          // read the data
          *( ptCtl->pnData + ptCtl->wBufIdx++ ) = ptCtl->ptTwi->TWIHS_RHR;
          ptCtl->wCurLen--;
          break;
        }
      }

      // check for zero time
      if ( uTimeout == 0 )
      {
        // set timeout error
        eError = I2C_ERR_TIMEOUT;
      }
    }

    // wait for complete/clear the status
    while( !( ptCtl->ptTwi->TWIHS_SR & TWIHS_SR_TXCOMP ));
    uStatus = ptCtl->ptTwi->TWIHS_SR;

    #if ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_FREERTOS )
    // release the semaphore
    xSemaphoreGive( ptCtl->xI2cMutexSemaphore );
    #endif // SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_FREERTOS
  }
  else
  {
    // illegal device
    eError = I2C_ERR_ILLDEV;
  }
  
  #if ( I2C_ENABLE_DEBUG == 1 )
  DebugManager_AddElement( I2C_DEBUG_BASE_VALUE + 4, eError );
  #endif // I2C_ENABLE_DEBUG

  // return the error
  return( eError );
}

/******************************************************************************
 * @function I2c_Ioctl
 *
 * @brief I2C IOCTL functions
 *
 * This function provides functionality to modify the I2Cs parameters
 *
 * @param[in]   eDev        Device enumeration
 * @param[in]   eAction     action to take
 * @param[io]   pvData      pointer to data storage/retrieval
 *
 * @return      appropriate error value
 *
 *****************************************************************************/
I2CERR I2c_Ioctl( I2CDEVENUM eDev, I2CACTION eAction, PVOID pvData )
{
  I2CERR      eError = I2C_ERR_NONE;
  PLCLCTL     ptCtl;
  PU32        puData;
  U32         uValue;
  PI2CCHKBSY  ptBusyParams;

  // set the pointer to avoid compiler errors
  puData = ( PU32 )pvData;
  
  // check for a valid I2C
  if ( eDev < I2C_DEV_ENUM_MAX )
  {
    // get pointers to the control/def structure
    ptCtl = &atLclCtls[ eDev ];

    // process the action
    switch( eAction )
    {
      case I2C_ACTION_POLL_DEVICE :
        // get the parameters
        ptBusyParams = ( PI2CCHKBSY )pvData;

        // read status/set the address/issue a start/check for NACK
        ptCtl->ptTwi->TWIHS_IADR = 0;
        uValue = TWIHS_MMR_DADR( ptBusyParams->nDevAddr );
        uValue |= TWIHS_MMR_MREAD;
        ptCtl->ptTwi->TWIHS_MMR = uValue;
        ptCtl->ptTwi->TWIHS_CR = TWIHS_CR_START | TWIHS_CR_STOP;

        // test for NAK
        uValue = ptCtl->ptTwi->TWIHS_SR;
        if ( uValue & TWIHS_SR_NACK )
        {
          // set the error to SLVNAK
          eError = I2C_ERR_SLVNAK;
        }
        else
        {
          // do a dummy read of the data/status
          uValue = ptCtl->ptTwi->TWIHS_RHR;
          uValue = ptCtl->ptTwi->TWIHS_SR;
        }
        
        #if ( I2C_ENABLE_DEBUG == 1 )
        DebugManager_AddElement( I2C_DEBUG_BASE_VALUE + 5, eError );
        #endif // I2C_ENABLE_DEBUG
      break;

      case I2C_ACTION_SET_DEVADDR :
        break;
        
      case I2C_ACTION_GET_DEVADDR :
        break;
        
      default :
        // illegal action
        eError = I2C_ERR_ILLACT;
        break;
    }
  }
  else
  {
    // illegal device
    eError = I2C_ERR_ILLDEV;
  }
  
  // return the error
  return( eError );
}

/******************************************************************************
 * @function I2c_Close
 *
 * @brief Close the I2C
 *
 * This function will disable any interrupt, unregister the interrupt handler
 *
 * @param[in]   eDev        Device enumeration
 *
 * @return      appropriate error value
 *
 *****************************************************************************/
I2CERR I2c_Close( I2CDEVENUM eDev )
{
  I2CERR  eError = I2C_ERR_NONE;
  PLCLCTL ptCtl;
  
  // check for a valid UART
  if ( eDev < I2C_DEV_ENUM_MAX )
  {
    // get the pointer to the USART channel
    ptCtl = &atLclCtls[ eDev ];

    // turn off the TWI
    ptCtl->ptTwi->TWIHS_CR = TWIHS_CR_MSDIS;
  }
  else
  {
    // illegal device
    eError = I2C_ERR_ILLDEV;
  }
  
  // return the error
  return( eError );
}

/******************************************************************************
 * @function SetBaudrateClock
 *
 * @brief compute and set the baudrate clock
 *
 * This function computes the value for the baud rate register
 *
 * @param[in]   ptTwi     poitner to the TWI
 * @param[in]   uBaud     desired baud rate
 *
 *****************************************************************************/
static void SetBaudrateClock( Twihs* ptTwi, U32 uBaud )
{
  U32   uPeripheralClock, uClkDiv, uClkLhDiv;
  
  // get the system clock value
  uClkDiv = 0;
  uPeripheralClock = Clock_GetMastClockFreq( );
  
  // calculate the divider
  uClkLhDiv = (uPeripheralClock / ( uBaud * CLOCK_DIVIDER )) - CLOCK_CALC_ARGU;
  while(( uClkLhDiv > CLOCK_DIV_MAX ) && ( uClkDiv < CLOCK_DIV_MIN ))
  {
    // adjust the divider 
    uClkDiv++;
    uClkLhDiv /= CLOCK_DIVIDER;
  }
  
  // now set it
  ptTwi->TWIHS_CWGR = TWIHS_CWGR_CLDIV( uClkLhDiv ) | TWIHS_CWGR_CHDIV( uClkLhDiv ) | TWIHS_CWGR_CKDIV( uClkDiv );
}

/******************************************************************************
 * @function GetTwiChannel
 *
 * @brief gets a pointer to the peripheral
 *
 * This function will return a pointer to the peripheral
 * for a given channel
 *
 * @param[in]   eChan			TWI channel
 * @param[in]   ptCtl    pointer to the control structure
 *
 *
 *****************************************************************************/
static void GetTwiChannel( I2CCHAN eChan, PLCLCTL ptCtl )
{
  switch( eChan )
  {
    case I2C_CHAN_0 :
      ptCtl->ptTwi = TWIHS0;
      break;
      
    case I2C_CHAN_1 :
      ptCtl->ptTwi = TWIHS1;
      break;

    #ifdef TWIHS2
    case I2C_CHAN_2 :
      ptCtl->ptTwi = TWIHS2;
      break;
    #endif // TWIHS2
    
      
    default :
      ptCtl->ptTwi = NULL;
      break;
  }
}


/******************************************************************************
 * @function WaitStatusReady
 *
 * @brief wait for a status value to be set in the status register
 *
 * This function tests for the status bit to be set 
 *
 * @param[in]   ptTwi     pointer to the TWI
 * @param[in]   uMask     status bit mask
 * @param[in]   uTimeout  timeout in counts
 * 
 * @return      TRUE if error, FALSE if not
 *
 *****************************************************************************/
static BOOL WaitStatusReady( Twihs* ptTwi , U32 uMask, U32 uTimeOut )
{
  BOOL  bError = FALSE;
  BOOL  bRunFlag = TRUE;
  U32   uStatus;

  // now loop
  while( bRunFlag )
  {
    uStatus = ptTwi->TWIHS_SR;
    if ( uStatus & uMask )
    {
      // clear the run flag
      bRunFlag = FALSE;
    }
    else
    {
      // check for timeout
      if ( --uTimeOut == 0 )
      {
        // timeout occured-set error/exit loop
        bError = TRUE;
        bRunFlag = FALSE;
      }
    }
  }

  // return the error detected flag
  return( bError );
}

/**@} EOF I2c.c */
