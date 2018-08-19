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
#include "I2C/I2c_prm.h"

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

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------
/// define the local control structure
typedef struct _LCLCTLS
{
  PU8     	pnData ;      ///< pointer to the data storage
  U16      	wCurLen;      ///< current receive/transmit length
  U16      	wBufIdx;      ///< buffer index
  I2CERR  	eError;       ///< current error
  BOOL   	  bRunning;     ///< running state
  Twihs*    ptTwi;        ///< pointer to the TWI
  U32       uIrqEnum;     ///< interrupt enumeration
  #if ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_FREERTOS )
  xSemaphoreHandle xI2cMutexSemaphore;
  #endif // SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_FREERTOS
} LCLCTL, *PLCLCTL;
#define LCLCTLS_SIZE    sizeof( LCLCTLS )

// local parameter declarations -----------------------------------------------
static  LCLCTL      atLclCtls[ I2C_CHAN_MAX ];
static  I2CDEVENUM  aePhyToDefMap[ I2C_CHAN_MAX ];

// local function prototypes --------------------------------------------------
static  void  GetTwiChannel( I2CCHAN eChan, PLCLCTL ptCtl );
static  void  IrqCommonHandler( I2CDEVENUM eDev );
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
    ptCtl->ptTwi->TWIHS_CR = TWIHS_CR_SVDIS | TWIHS_CR_MSEN;
    
        // now set the baudrate
    uTemp = ( ptDef->bFastSpeed ) ? BAUD_RATE_FAST : BAUD_RATE_SLOW;
    SetBaudrateClock( ptCtl->ptTwi, uTemp );

    // map physical to logical
    aePhyToDefMap[ ptDef->eChan ] = eDev;
    
    // enable the interrupt in the NVIC
    NVIC_EnableIRQ( ptCtl->uIrqEnum );
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

    // disable the interrupt in the NVIC
    NVIC_DisableIRQ( ptCtl->uIrqEnum );
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
  I2CERR 	    eError = I2C_ERR_NONE;
  PI2CDEF 	  ptDef;
  PLCLCTL 	  ptCtl;
  U32         uTime;  
  BOOL        bError;

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
    ptCtl->ptTwi->TWIHS_MMR = TWIHS_MMR_DADR( ptXfrCtl->nDevAddr ) | BITS( ptXfrCtl->nAddrLen, TWIHS_MMR_IADRSZ_Pos ) | TWIHS_MMR_MREAD;
    ptCtl->ptTwi->TWIHS_MMR &= ~TWIHS_MMR_MREAD;

    #if ( I2C_ENABLE_DEBUG == 1 )
    DebugManager_AddElement( I2C_DEBUG_BASE_VALUE + 1, ( ptXfrCtl->nDevAddr << 8 ) | ( ptXfrCtl->wDataLen + ptXfrCtl->nAddrLen ));
    #endif // I2C_ENABLE_DEBUG

    // now write the address with read bit cleared
    if ( ptXfrCtl->nAddrLen > 0 )
    {
      // set the address
      ptCtl->ptTwi->TWIHS_IADR = 0;
      ptCtl->ptTwi->TWIHS_IADR = ptXfrCtl->tAddress.uValue;
    }
    
    // is this a single data byte
    if ( ptXfrCtl->wDataLen < 2 )
    {
      // write single byte/wait for ready
      ptCtl->ptTwi->TWIHS_THR = *( ptCtl->pnData + ptCtl->wBufIdx++ );
      ptCtl->wCurLen--;
      ptCtl->ptTwi->TWIHS_CR = TWIHS_CR_STOP;        
      if ( !WaitStatusReady( ptCtl->ptTwi, TWIHS_SR_TXRDY, 50000 ))
      {
        // send stop
        ptCtl->ptTwi->TWIHS_CR = TWIHS_CR_STOP;

        // wait for txcomp/ready
        if ( !WaitStatusReady( ptCtl->ptTwi, TWIHS_SR_TXRDY, 50000 ))
        {
          // now wait for TXCOMp
          if( WaitStatusReady( ptCtl->ptTwi, TWIHS_SR_TXCOMP, 50000 ))
          {
            // set timeout
            ptCtl->eError = I2C_ERR_TIMEOUT;
          }
        }
        else
        {
          // set timeout
          ptCtl->eError = I2C_ERR_TIMEOUT;
        }
      }
    }
    else
    {
      // set the running flag/clear the error
      ptCtl->bRunning = TRUE;
      ptCtl->eError = I2C_ERR_TIMEOUT;
      
      // enable the interrupts/start the transfer
      ptCtl->ptTwi->TWIHS_IER = TWIHS_IER_TXRDY;
      ptCtl->ptTwi->TWIHS_THR = *( ptCtl->pnData + ptCtl->wBufIdx++ );

      // check for a blocking device
      if ( ptDef->pvCallBack == NULL )
      {
        uTime = SystemTick_GetTimeMsec( ) + ptXfrCtl->uTimeout;
        while( ptCtl->bRunning )
        {
          // check for timeout
          if ( SystemTick_GetTimeMsec( ) >= uTime )
          {
            // timeout occured - flag error
            ptCtl->bRunning = FALSE;
            ptCtl->eError = I2C_ERR_TIMEOUT;
          }
        }

        // disable interrupts
        ptCtl->ptTwi->TWIHS_IER = 0;
        
        // set the return error
        eError = ptCtl->eError;
      }
      else
      {
        // set the error to busy
        eError = I2C_ERR_BLKING;
      }
    }


    #if ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_FREERTOS )
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
  I2CERR      eError = I2C_ERR_NONE;
  PI2CDEF     ptDef;
  PLCLCTL     ptCtl;
  U32         uTime, uCrValue;  
  
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

    // set the running flag/direction/state
    ptCtl->bRunning = TRUE;
    ptCtl->eError = I2C_ERR_NONE;
    
    // set the address size/device address
    ptCtl->ptTwi->TWIHS_MMR = TWIHS_MMR_DADR( ptXfrCtl->nDevAddr ) | BITS( ptXfrCtl->nAddrLen, TWIHS_MMR_IADRSZ_Pos ) | TWIHS_MMR_MREAD;

    #if ( I2C_ENABLE_DEBUG == 1 )
    DebugManager_AddElement( I2C_DEBUG_BASE_VALUE + 3, ( ptXfrCtl->nDevAddr << 8 ) | ( ptXfrCtl->wDataLen + ptXfrCtl->nAddrLen ));
    #endif // I2C_ENABLE_DEBUG
    // now write the address with read bit cleared
    if ( ptXfrCtl->nAddrLen > 0 )
    {
      // set the address
      ptCtl->ptTwi->TWIHS_IADR = ptXfrCtl->tAddress.uValue;
    }

    // set the CR value
    uCrValue = TWIHS_CR_START;
    uCrValue |= ( ptCtl->wCurLen == 1 ) ? TWIHS_CR_STOP : 0;
    ptCtl->ptTwi->TWIHS_CR = uCrValue;

    // read and store the data
    *( ptCtl->pnData + ptCtl->wBufIdx++ ) = ptCtl->ptTwi->TWIHS_RHR;

    // decrement the count
    if ( --ptCtl->wCurLen == 1 )
    {
      // issue stop
      ptCtl->ptTwi->TWIHS_CR = TWIHS_CR_STOP;
    
      // wait for ready	  
      if ( !WaitStatusReady( ptCtl->ptTwi, TWIHS_SR_RXRDY, 50000 ))
      {
        // read and store the data
        *( ptCtl->pnData + ptCtl->wBufIdx++ ) = ptCtl->ptTwi->TWIHS_RHR;
      }
      else
      {
        // set timeout
        ptCtl->eError = I2C_ERR_TIMEOUT;
      }
    }

    #if ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_FREERTOS )
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
  return(( I2CERR )eError );
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

        // wait for ready	  
        if ( WaitStatusReady( ptCtl->ptTwi, TWIHS_SR_RXRDY, 10000 ))
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

    // enable the interrupt in the NVIC
    NVIC_DisableIRQ( ptCtl->uIrqEnum );
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
 * @function TWI0_Handler
 *
 * @brief interrupt TWI0 handler
 *
 * This function handles the interrupts from TWI0
 *
 *****************************************************************************/
void TWI0_Handler( void )
{
  // call the common handler
  IrqCommonHandler( aePhyToDefMap[ I2C_CHAN_0 ] );
}

/******************************************************************************
 * @function TWI1_Handler
 *
 * @brief interrupt TWI1 handler
 *
 * This function handles the interrupts from TWI1
 *
 *****************************************************************************/
void TWI1_Handler( void )
{
  // call the common handler
  IrqCommonHandler( aePhyToDefMap[ I2C_CHAN_1 ] );
}

/******************************************************************************
 * @function TWI1_Handler
 *
 * @brief interrupt TWI2 handler
 *
 * This function handles the interrupts from TWI2
 *
 *****************************************************************************/
void TWI2_Handler( void )
{
  // call the common handler
  IrqCommonHandler( aePhyToDefMap[ I2C_CHAN_2 ] );
}

/******************************************************************************
 * @function IrqCommandHandler
 *
 * @brief common interrupt handler
 *
 * This function checks for the type of interrupts and processes them
 * appropriately
 *
 * @param[in]   eDev        physical device enumeration
 *
 *****************************************************************************/
static void IrqCommonHandler( I2CDEVENUM eDev )
{
  PI2CDEF   ptDef;
  PLCLCTL   ptCtl;
  U32       uData;
  I2CERR    eError = I2C_ERR_NONE;
  
  // get the definition/control structure pointers
  ptDef = ( PI2CDEF )&atI2cDefs[ eDev ];
  ptCtl = &atLclCtls[ eDev ];

  // check for 
  if ( ptCtl->ptTwi->TWIHS_SR & TWIHS_SR_TXRDY )
  {
    // disable the interrupt
    ptCtl->ptTwi->TWIHS_IDR = TWIHS_IDR_TXRDY;
    
    // is there more data to send
    if ( ptCtl->wCurLen != 0 )
    {
      // send the next byte
      ptCtl->ptTwi->TWIHS_THR = *( ptCtl->pnData + ptCtl->wBufIdx++ );
      ptCtl->wCurLen--;
    }
    else
    {
      // send stop
      ptCtl->ptTwi->TWIHS_CR = TWIHS_CR_STOP;

      // wait for TXCOMP
      if ( WaitStatusReady( ptCtl->ptTwi, TWIHS_SR_TXCOMP, 50000 ))
      {
        // error timeout
        eError = I2C_ERR_TIMEOUT;
      }
    }


    // set the error
    ProcessComplete( eError, ptDef, ptCtl );
  }

  // check for TWIHS_SR_RXRDY
  if ( ptCtl->ptTwi->TWIHS_SR & TWIHS_SR_RXRDY )
  {
    // disable the interrupt
    ptCtl->ptTwi->TWIHS_IDR = TWIHS_IDR_RXRDY;

    // read the data
    *( ptCtl->pnData + ptCtl->wBufIdx++ ) = ptCtl->ptTwi->TWIHS_RHR;
    ptCtl->wCurLen--;

    // check for count == 1
    if ( ptCtl->wCurLen == 1 )
    {
      // send a stop
      ptCtl->ptTwi->TWIHS_CR = TWIHS_CR_STOP;

      // wait for RSRDY
      if ( !WaitStatusReady( ptCtl->ptTwi, TWIHS_SR_RXRDY, 50000 ))
      {
        // get the last character
        *( ptCtl->pnData + ptCtl->wBufIdx ) = ptCtl->ptTwi->TWIHS_RHR;

        // wait for ready
        if ( !WaitStatusReady( ptCtl->ptTwi, TWIHS_SR_TXCOMP, 50000 ))
        {
          // clear the error
          eError = I2C_ERR_NONE;
        }
      }
    }
    else
    {
      // set the error
      eError = I2C_ERR_TIMEOUT;
    }

    // process complete
    ProcessComplete( eError, ptDef, ptCtl );
  }
  
  // check for a NACK
  if (( ptCtl->ptTwi->TWIHS_SR & TWIHS_SR_NACK ) && ( ptCtl->ptTwi->TWIHS_IMR & TWIHS_IMR_NACK ))
  {
    // set the error
    ProcessComplete( I2C_ERR_SLVNAK, ptDef, ptCtl );
  }
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
      ptCtl->uIrqEnum = ID_TWIHS0;
      break;
      
    case I2C_CHAN_1 :
      ptCtl->ptTwi = TWIHS1;
      ptCtl->uIrqEnum = ID_TWIHS1;
      break;
      
    case I2C_CHAN_2 :
      ptCtl->ptTwi = TWIHS2;
      ptCtl->uIrqEnum = ID_TWIHS2;
      break;
      
    default :
      ptCtl->ptTwi = NULL;
      ptCtl->uIrqEnum = 0;
      break;
  }
}

/******************************************************************************
 * @function ProcessComplete
 *
 * @brief process the read/write complete
 *
 * This function clears the funning flag, resets the state, and posts an event
 * appropriately
 *
 * @param[in]   eError    error
 * @param[in]   ptDef     pointer to the definition structure
 * @param[in]   ptCtl     pointer to the control structure
 *
 *****************************************************************************/
static void ProcessComplete( I2CERR eError, PI2CDEF ptDef, PLCLCTL ptCtl )
{
  // clear the funning flag/set the state to idle/set the error
  ptCtl->bRunning = FALSE;
  ptCtl->eError = eError;
  
  #if ( I2C_ENABLE_DEBUG == 1 )
  DebugManager_AddElement( I2C_DEBUG_BASE_VALUE + 6, eError );
  #endif // I2C_ENABLE_DEBUG
  // check to see if the callback is not null
  if ( ptDef->pvCallBack != NULL )
  {
    // call it
    ptDef->pvCallBack( eError );
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

  // now loop
  while( bRunFlag )
  {
    if ( ptTwi->TWIHS_SR & uMask )
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
