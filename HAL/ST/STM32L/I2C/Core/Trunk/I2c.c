/******************************************************************************
 * @file I2C.c
 *
 * @brief I2C implementaion
 *
 * This file provides the implementation
 *
 * @copyright Copyright (c) 2014 Cyber Integration
 * This document contains proprietary data and information of Cyber Integration 
 * LLC. It is the exclusive property of Cyber Integration, LLC and will not be 
 * disclosed in any form to any party without prior written permission of 
 * Cyber Integration, LLC. This document may not be reproduced or further used 
 * without the prior written permission of Cyber Integration, LLC.
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

// Macros and Defines ---------------------------------------------------------
/// define the baud rate values for slow/fast
#define BAUD_RATE_SLOW          ( 100000l )
#define BAUD_RATE_FAST          ( 400000l )

/// define the clock calculation values
#define CLOCK_DIVIDER           ( 2 )
#define CLOCK_CALC_ARGU         ( 4 )
#define CLOCK_DIV_MAX           ( 0xFF )
#define CLOCK_DIV_MIN           ( 7 )

/// define the minimum receive byte count to use PDC
#define PDC_MIN_RCV_BYTE_COUNT  ( 3 )

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
  Twi*      ptTwi;        ///< pointer to the TWI
  Pdc*      ptPdc;        ///< pointer to the PDC
} LCLCTL, *PLCLCTL;
#define LCLCTLS_SIZE    sizeof( LCLCTLS )

// local parameter declarations -----------------------------------------------
static  LCLCTL      atLclCtls[ I2C_CHAN_MAX ];
static  I2CDEVENUM  aePhyToDefMap[ I2C_CHAN_MAX ];

// local function prototypes --------------------------------------------------
static  void  GetTwiChannel( I2CCHAN eChan, PLCLCTL ptCtl );
static  void  IrqCommonHandler( I2CDEVENUM eDev );
static  void  SetBaudrateClock( Twi* ptTwi, U32 uBaud );
static  void  ProcessComplete( I2CERR eError, PI2CDEF ptDef, PLCLCTL ptCtl );
static  BOOL  WaitStatusReady( Twi* ptTwi, U32 uMask, U32 uTimeout );

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

    // disable all interrupts/dummy read of status
    ptCtl->ptTwi->TWI_IDR = 0xFFFFFFFF;
    uTemp = ptCtl->ptTwi->TWI_SR;
    
    // reset the device
    ptCtl->ptTwi->TWI_CR = TWI_CR_SWRST;
    uTemp = ptCtl->ptTwi->TWI_RHR;
    
    // enable the master mode
    ptCtl->ptTwi->TWI_CR = TWI_CR_MSDIS;
    ptCtl->ptTwi->TWI_CR = TWI_CR_SVDIS | TWI_CR_MSEN;
    
        // now set the baudrate
    uTemp = ( ptDef->bFastSpeed ) ? BAUD_RATE_FAST : BAUD_RATE_SLOW;
    SetBaudrateClock( ptCtl->ptTwi, uTemp );

    // map physical to logical
    aePhyToDefMap[ ptDef->eChan ] = eDev;
    
    // enable the interrupt in the NVIC
    NVIC_EnableIRQ( TWI0_IRQn + ptDef->eChan );
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
    ptCtl->ptTwi->TWI_CR = TWI_CR_SWRST;

    // disable the interrupt in the NVIC
    NVIC_DisableIRQ( TWI0_IRQn + ptDef->eChan );
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
    
    // copy the values to the local controls
    ptCtl->pnData = ptXfrCtl->pnData;
    ptCtl->wCurLen = ptXfrCtl->wDataLen;
    ptCtl->wBufIdx = 0;

    // clear the error 
    ptCtl->eError = I2C_ERR_NONE;
    
    // set the address size/device address
    ptCtl->ptTwi->TWI_MMR = 0;
    ptCtl->ptTwi->TWI_MMR = TWI_MMR_DADR( ptXfrCtl->nDevAddr ) | BITS( ptXfrCtl->nAddrLen, TWI_MMR_IADRSZ_Pos ) | TWI_MMR_MREAD;
    ptCtl->ptTwi->TWI_MMR &= ~TWI_MMR_MREAD;

    #if ( I2C_ENABLE_DEBUG == 1 )
    DebugManager_AddElement( I2C_DEBUG_BASE_VALUE + 1, ( ptXfrCtl->nDevAddr << 8 ) | ( ptXfrCtl->wDataLen + ptXfrCtl->nAddrLen ));
    #endif // I2C_ENABLE_DEBUG

    // now write the address with read bit cleared
    if ( ptXfrCtl->nAddrLen > 0 )
    {
      // set the address
      ptCtl->ptTwi->TWI_IADR = 0;
      ptCtl->ptTwi->TWI_IADR = ptXfrCtl->tAddress.uValue;
    }

    // only use PDC if length is greater/equal to 2
    if ( ptCtl->wCurLen >= 2 )
    {
      // set up the transmit pointer/length
      ptCtl->ptPdc->PERIPH_TPR = ( U32 )ptCtl->pnData;
      ptCtl->ptPdc->PERIPH_TCR = ptCtl->wCurLen - 1;
      ptCtl->wBufIdx = ptCtl->wCurLen - 1;

      // enable the ENDTX interrupt
      ptCtl->ptTwi->TWI_IER = TWI_IER_ENDTX;

      // set running flag
      ptCtl->bRunning = TRUE;
      ptCtl->eError = I2C_ERR_NONE;

      // start the transfer
      ptCtl->ptPdc->PERIPH_PTCR = PERIPH_PTCR_TXTEN;

      // check for a blocking device
      if ( ptDef->pvCallBack == NULL )
      {
        // calculate the new time
        uTime = SystemTick_GetTimeMsec( ) + ptXfrCtl->uTimeout;

        // wait till done or timeout
        while( ptCtl->bRunning )
        {
          // check for timeout
          if ( SystemTick_GetTimeMsec( ) >= uTime )
          {
            // timeout occured - flag error
            ptCtl->eError = I2C_ERR_TIMEOUT;
            ptCtl->bRunning = FALSE;
          }
        }

        // disable interrupts/clear running flag
        ptCtl->bRunning = FALSE;
        ptCtl->ptTwi->TWI_IER = 0;

        // set the return error
        eError = ptCtl->eError;
      }
      else
      {
        // set the error to busy
        eError = I2C_ERR_BLKING;
      }
    }
    else
    {
      // write single byte/wait for ready
      ptCtl->ptTwi->TWI_THR = *( ptCtl->pnData + ptCtl->wBufIdx++ );
      if ( !WaitStatusReady( ptCtl->ptTwi, TWI_SR_TXRDY, 50000 ))
      {
        // send stop
        ptCtl->ptTwi->TWI_CR = TWI_CR_STOP;

        // wait for txcomp/ready
        if ( !WaitStatusReady( ptCtl->ptTwi, TWI_SR_TXRDY, 50000 ))
        {
          // now wait for TXCOMp
          if( WaitStatusReady( ptCtl->ptTwi, TWI_SR_TXCOMP, 50000 ))
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
      else
      {
        // set timeout
        ptCtl->eError = I2C_ERR_TIMEOUT;
      }
    }
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
    
    // copy the values to the local controls
    ptCtl->pnData = ptXfrCtl->pnData;
    ptCtl->wCurLen = ptXfrCtl->wDataLen;
    ptCtl->wBufIdx = 0;

    // set the running flag/direction/state
    ptCtl->bRunning = TRUE;
    ptCtl->eError = I2C_ERR_NONE;
    
    // set the address size/device address
    ptCtl->ptTwi->TWI_MMR = TWI_MMR_DADR( ptXfrCtl->nDevAddr ) | BITS( ptXfrCtl->nAddrLen, TWI_MMR_IADRSZ_Pos ) | TWI_MMR_MREAD;

    #if ( I2C_ENABLE_DEBUG == 1 )
    DebugManager_AddElement( I2C_DEBUG_BASE_VALUE + 3, ( ptXfrCtl->nDevAddr << 8 ) | ( ptXfrCtl->wDataLen + ptXfrCtl->nAddrLen ));
    #endif // I2C_ENABLE_DEBUG
    // now write the address with read bit cleared
    if ( ptXfrCtl->nAddrLen > 0 )
    {
      // set the address
      ptCtl->ptTwi->TWI_IADR = ptXfrCtl->tAddress.uValue;
    }

    // now determine PDB or direct
    if ( ptCtl->wCurLen < PDC_MIN_RCV_BYTE_COUNT )
    {
      // set the CR value
      uCrValue = TWI_CR_START;
      uCrValue |= ( ptCtl->wCurLen == 1 ) ? TWI_CR_STOP : 0;
      ptCtl->ptTwi->TWI_CR = uCrValue;

      // wait for RXRDY
      if ( !WaitStatusReady( ptCtl->ptTwi, TWI_SR_RXRDY, 50000 ))
      {
        // read and store the data
        *( ptCtl->pnData + ptCtl->wBufIdx++ ) = ptCtl->ptTwi->TWI_RHR;

        // decrement the count
        if ( --ptCtl->wCurLen == 1 )
        {
          // issue stop
          ptCtl->ptTwi->TWI_CR = TWI_CR_STOP;
      
          // wait for ready	  
          if ( !WaitStatusReady( ptCtl->ptTwi, TWI_SR_RXRDY, 50000 ))
          {
            // read and store the data
            *( ptCtl->pnData + ptCtl->wBufIdx++ ) = ptCtl->ptTwi->TWI_RHR;
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
        // set timeout
        ptCtl->eError = I2C_ERR_TIMEOUT;
      }
    }
    else
    {
      // set the PDC pointer/count/enable
      ptCtl->ptPdc->PERIPH_RPR = ( U32 )ptCtl->pnData;
      ptCtl->ptPdc->PERIPH_RCR = ptCtl->wCurLen - 2;
      ptCtl->ptPdc->PERIPH_PTCR = PERIPH_PTCR_RXTEN;
      ptCtl->wBufIdx = ptCtl->wCurLen - 2;

      // enable the interrupt
      ptCtl->ptTwi->TWI_IER = TWI_IER_ENDRX | TWI_IER_NACK;

      // start the transfer
      ptCtl->ptTwi->TWI_CR = TWI_CR_START;

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
        ptCtl->ptTwi->TWI_IER = 0;
      
        // set the return error
        eError = ptCtl->eError;
      }
      else
      {
        // set the error to busy
        eError = I2C_ERR_BLKING;
      }
    }
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
        ptCtl->ptTwi->TWI_IADR = 0;
        uValue = TWI_MMR_DADR( ptBusyParams->nDevAddr );
//        uValue |= ( ptBusyParams->bReadMode ) ? TWI_MMR_MREAD : 0;
        uValue |= TWI_MMR_MREAD;
        ptCtl->ptTwi->TWI_MMR = uValue;
        ptCtl->ptTwi->TWI_CR = TWI_CR_START | TWI_CR_STOP;

        // wait for ready	  
        if ( WaitStatusReady( ptCtl->ptTwi, TWI_SR_RXRDY, 10000 ))
        {
          // set the error to SLVNAK
          eError = I2C_ERR_SLVNAK;
        }
        else
        {
          // do a dummy read of the data/status
          uValue = ptCtl->ptTwi->TWI_RHR;
          uValue = ptCtl->ptTwi->TWI_SR;
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
    ptCtl->ptTwi->TWI_CR = TWI_CR_MSDIS;

    // enable the interrupt in the NVIC
    NVIC_DisableIRQ( TWI0_IRQn + eDev );
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

  // check for PDC_ENDTX
  if (( ptCtl->ptTwi->TWI_SR & TWI_SR_ENDTX ) && ( ptCtl->ptTwi->TWI_IMR & TWI_IMR_ENDTX ))
  {
    // disable the interrupt
    ptCtl->ptTwi->TWI_IDR = TWI_IDR_ENDTX;

    // disable the PDC
    ptCtl->ptPdc->PERIPH_PTCR = PERIPH_PTCR_TXTDIS;

    // wait for TXRDY
    if ( !WaitStatusReady( ptCtl->ptTwi, TWI_SR_TXRDY, 50000 ))
    {
      // now write the last character
      ptCtl->ptTwi->TWI_THR = *( ptCtl->pnData + ptCtl->wBufIdx );

      // set stop
      ptCtl->ptTwi->TWI_CR = TWI_CR_STOP;
    }
    else
    {
      // error timeout
      eError = I2C_ERR_TIMEOUT;
    }

    // set the error
    ProcessComplete( eError, ptDef, ptCtl );
  }

  // check for PDC_ENDRX
  if (( ptCtl->ptTwi->TWI_SR & TWI_SR_ENDRX ) && ( ptCtl->ptTwi->TWI_IMR & TWI_IMR_ENDRX ))
  {
    // disable the interrupt
    ptCtl->ptTwi->TWI_IDR = TWI_IDR_ENDRX;

    // disable the PDC
    ptCtl->ptPdc->PERIPH_PTCR =  PERIPH_PTCR_RXTDIS;

    // wait for RSRDY
    if ( !WaitStatusReady( ptCtl->ptTwi, TWI_SR_RXRDY, 50000 ))
    {
      // send a stop
      ptCtl->ptTwi->TWI_CR = TWI_CR_STOP;

      // store the data
      *( ptCtl->pnData + ptCtl->wBufIdx++ ) = ptCtl->ptTwi->TWI_RHR;

      // wait for ready
      if ( !WaitStatusReady( ptCtl->ptTwi, TWI_SR_RXRDY, 50000 ))
      {
        // get the last character
        *( ptCtl->pnData + ptCtl->wBufIdx ) = ptCtl->ptTwi->TWI_RHR;

        // clear the error
        eError = I2C_ERR_NONE;
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
  if (( ptCtl->ptTwi->TWI_SR & TWI_SR_NACK ) && ( ptCtl->ptTwi->TWI_IMR & TWI_IMR_NACK ))
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
static void SetBaudrateClock( Twi* ptTwi, U32 uBaud )
{
  U32   uPeripheralClock, uClkDiv, uClkLhDiv;
  
  // get the system clock value
  uPeripheralClock = Clock_GetFreq( );
  
  // calculate the divider
  uClkLhDiv = (uPeripheralClock / ( uBaud * CLOCK_DIVIDER )) - CLOCK_CALC_ARGU;
  while(( uClkLhDiv > CLOCK_DIV_MAX ) && ( uClkDiv < CLOCK_DIV_MIN ))
  {
    // adjust the divider 
    uClkDiv++;
    uClkLhDiv /= CLOCK_DIVIDER;
  }
  
  // now set it
  ptTwi->TWI_CWGR = TWI_CWGR_CLDIV( uClkLhDiv ) | TWI_CWGR_CHDIV( uClkLhDiv ) | TWI_CWGR_CKDIV( uClkDiv );
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
      ptCtl->ptTwi = TWI0;
      ptCtl->ptPdc = PDC_TWI0;
      break;
      
    case I2C_CHAN_1 :
      ptCtl->ptTwi = TWI1;
      ptCtl->ptPdc = PDC_TWI1;
      break;
    
    default :
      ptCtl->ptTwi = NULL;
      ptCtl->ptPdc = NULL;
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
static BOOL WaitStatusReady( Twi* ptTwi , U32 uMask, U32 uTimeOut )
{
  BOOL  bError = FALSE;
  BOOL  bRunFlag = TRUE;

  // now loop
  while( bRunFlag )
  {
    if ( ptTwi->TWI_SR & uMask )
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
