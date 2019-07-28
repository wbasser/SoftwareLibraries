/******************************************************************************
 * @file I2c.c
 *
 * @brief I2C Sercom implementation
 *
 * This file provides the implementation of the I2C
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

// library includes -----------------------------------------------------------
#include "Clock/Clock.h"
#include "SystemTick/SystemTick.h"

// local includes -------------------------------------------------------------
#include "I2C/I2c.h"

// Macros and Defines ---------------------------------------------------------
/// define the baud rate values for slow/fast
#define BAUD_RATE_SLOW      ( 100000l )
#define BAUD_RATE_FAST      ( 400000l )

/// define the stop command value 
#define CMD_SEND_STOP       ( 3 )

// enumerations ---------------------------------------------------------------
/// enuemrate the bus direction
typedef enum _BUSDIR
{
  BUS_DIR_WRITE = 0,    ///< bus direction is a write
  BUS_DIR_READ,         ///< bus direction is a read
  BUS_DIR_RDWRITE,      ///< bus direction is write/followed by a read
} BUSDIR;

/// enumerate the process write actions
typedef enum _PRCWRTACTION
{
  PRC_WR_ACTION_DONE = 0,   ///< done
  PRC_WR_ACTION_CONT,       ///< continue
  PRC_WR_ACTION_RESTART,    ///< restart
} PRCWRACTION;

// structures -----------------------------------------------------------------
/// define the local control structure
typedef struct _LCLCTL
{
  PI2CXFRCTL  ptXfrCtl;     ///< transfer control ointer
  U16         wBufIdx;      ///< buffer index
  I2CERR      eError;       ///< current error
  BOOL        bRunning;     ///< running state
  BUSDIR      eBusDir;      ///< bus direction
} LCLCTL, *PLCLCTL;
#define LCLCTL_SIZE         sizeof( LCLCTL )

// local parameter declarations -----------------------------------------------
static  LCLCTL      atLclCtls[ I2C_CHAN_MAX ];
static  I2CDEVENUM  aePhyToDefMap[ I2C_CHAN_MAX ];

// local function prototypes --------------------------------------------------
static  SercomI2cm*   GetSercomChannel( I2CCHAN eChan );
static  void          IrqCommonHandler( I2CDEVENUM eDev );
static  PRCWRACTION   ProcessMasterWrite( PLCLCTL ptCtl, SercomI2cm* ptI2c );
static  BOOL          ProcessMasterRead( PLCLCTL ptCtl, SercomI2cm* ptI2c );
static  void          ProcessComplete( I2CERR eError, PI2CDEF ptDef, PLCLCTL ptCtl, SercomI2cm* ptI2c );
static  U32           ComputeBaudRate( I2CCHAN eChan, U32 uBaud );

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
  PI2CDEF                 ptDef;
  I2CDEVENUM              eDev;
  SercomI2cm*             ptI2c;
  U32                     uTemp;
  SERCOM_I2CM_CTRLA_Type  tCtrlA;
  
  // for each device
  for ( eDev = 0; eDev < I2C_DEV_ENUM_MAX; eDev++ )
  {
    // get pointer to the definition structure
    ptDef = ( PI2CDEF )&g_atI2cDefs[ eDev ];
    ptI2c = GetSercomChannel( ptDef->eChan );

    // disable the control and wait for done
    ptI2c->CTRLA.bit.SWRST = 1;
    #ifdef __SAM_D21_SUBFAMILY
    while( ptI2c->SYNCBUSY.bit.SWRST );
    #endif //  __SAM_D21_SUBFAMILY
    
    // now set the baudrate
    uTemp = ( ptDef->bFastSpeed ) ? BAUD_RATE_FAST : BAUD_RATE_SLOW;
    ptI2c->BAUD.reg = ComputeBaudRate( ptDef->eChan, uTemp );
    
    // now set up control B
    ptI2c->CTRLB.reg = SERCOM_I2CM_CTRLB_SMEN;

    // enable the interrupts
    aePhyToDefMap[ ptDef->eChan ] = eDev;
    
    // enable the interrupt in the NVIC
    NVIC_EnableIRQ( SERCOM0_IRQn + ptDef->eChan );
    NVIC_SetPriority( SERCOM0_IRQn + ptDef->eChan, 3 );
      
    // now build control A
    tCtrlA.reg = SERCOM_I2CM_CTRLA_MODE_I2C_MASTER;
    tCtrlA.reg |= ( ptDef->bRunStandby ) ? SERCOM_I2CM_CTRLA_RUNSTDBY : 0;
    tCtrlA.reg |= SERCOM_I2CM_CTRLA_ENABLE;
    tCtrlA.reg |= SERCOM_I2CM_CTRLA_INACTOUT( 1 );
    tCtrlA.reg |= SERCOM_I2CM_CTRLA_SDAHOLD( 2 );
    ptI2c->CTRLA = tCtrlA;
    
    // now wait for done
    #ifdef __SAM_D21_SUBFAMILY
    while( ptI2c->SYNCBUSY.bit.SWRST );
    #endif //  __SAM_D21_SUBFAMILY
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
  PI2CDEF                 ptDef;
  I2CDEVENUM              eDev;
  SercomI2cm*             ptI2c;
  
  // for each device
  for ( eDev = 0; eDev < I2C_DEV_ENUM_MAX; eDev++ )
  {
    // get pointer to the definition structure
    ptDef = ( PI2CDEF )&g_atI2cDefs[ eDev ];
    ptI2c = GetSercomChannel( ptDef->eChan );

    // disable the control and wait for done
    ptI2c->CTRLA.bit.SWRST = 1;
    #ifdef __SAM_D21_SUBFAMILY
    while( ptI2c->SYNCBUSY.bit.SWRST );
    #endif //  __SAM_D21_SUBFAMILY

    // enable the interrupt in the NVIC
    NVIC_DisableIRQ( SERCOM0_IRQn + ptDef->eChan );
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
 * @param[in]   ptXftCtl    pointer to the transfer control
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
  SercomI2cm* ptI2c;

  // check for a valid I2C
  if ( eDev < I2C_DEV_ENUM_MAX )
  {
    // get pointer to the definition structure
    ptDef = ( PI2CDEF )&g_atI2cDefs[ eDev ];
    ptCtl = &atLclCtls[ eDev ];
    ptI2c = GetSercomChannel( ptDef->eChan );
    
    // copy the values to the local controls
    ptCtl->ptXfrCtl = ptXfrCtl;
    ptCtl->wBufIdx = 0;

    // clear any error bits in the status/flag
    ptI2c->STATUS.reg = SERCOM_I2CM_STATUS_MASK;

    // set the running flag/direction/state
    ptCtl->bRunning = TRUE;
    ptCtl->eBusDir = BUS_DIR_WRITE;
    ptCtl->eError = I2C_ERR_NONE;

    // enable the interrupts
    ptI2c->INTFLAG.reg = SERCOM_I2CM_INTFLAG_MASK;
    ptI2c->INTENSET.reg = SERCOM_I2CM_INTENSET_MB | SERCOM_I2CM_INTENSET_ERROR;
    
    // now write the address with read bit cleared
    #ifdef __SAM_D21_SUBFAMILY
    while( ptI2c->SYNCBUSY.bit.SWRST );
    #endif //  __SAM_D21_SUBFAMILY
    ptI2c->ADDR.bit.ADDR = ( ptXfrCtl->nDevAddr << 1 ) | BUS_DIR_WRITE;

    // check for a blocking device
    if ( ptDef->pvCallBack == NULL )
    {
      // wait till done or timeout
      ptCtl->bRunning = TRUE;
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

      // copy the error
      eError = ptCtl->eError;
    
      // disable the interrupts
      ptI2c->INTENCLR.reg = SERCOM_I2CM_INTENCLR_MB | SERCOM_I2CM_INTENCLR_ERROR;
    }
    else
    {
      // set the error to busy
      eError = I2C_ERR_BLKING;
    }
  }
  else
  {
    // illegal device
    eError = I2C_ERR_ILLDEV;
  }

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
 * @param[in]   ptXftCtl    pointer to the transfer control
 *
 * @return      appropriate error value
 *
 *****************************************************************************/
I2CERR I2c_Read( I2CDEVENUM eDev, PI2CXFRCTL ptXfrCtl )
{
  I2CERR      eError = I2C_ERR_NONE;
  PI2CDEF     ptDef;
  PLCLCTL     ptCtl;
  U32         uTime;  
  SercomI2cm* ptI2c;
  U8          nAddr;
  
  // check for a valid I2C
  if ( eDev < I2C_DEV_ENUM_MAX )
  {
    // get pointers to the control/def structure
    ptDef = ( PI2CDEF )&g_atI2cDefs[ eDev ];
    ptCtl = &atLclCtls[ eDev ];
    ptI2c = GetSercomChannel( ptDef->eChan );

    // copy the values to the local controls
    ptCtl->ptXfrCtl = ptXfrCtl;
    ptCtl->wBufIdx = 0;

    // clear any error bits in the status
    ptI2c->STATUS.reg = SERCOM_I2CM_STATUS_MASK;

    // clear the interrupts
    ptI2c->INTFLAG.reg = SERCOM_I2CM_INTFLAG_MASK;
    
    // clear the ack flag
    ptI2c->CTRLB.reg &= ~SERCOM_I2CM_CTRLB_ACKACT;

    // set the running flag/direction/state
    ptCtl->bRunning = TRUE;

    // clear the error
    ptCtl->eError = I2C_ERR_NONE;

    // compute the bus address
    nAddr = ( ptXfrCtl->nDevAddr << 1 );

    // process the address 
    if ( ptXfrCtl->nAddrLen == 0 )
    {
      // enable all the interrupts/set the bus direction/adjust the address
      ptI2c->INTENSET.reg = SERCOM_I2CM_INTENSET_MB | SERCOM_I2CM_INTENSET_SB | SERCOM_I2CM_INTENSET_ERROR;
      ptCtl->eBusDir = BUS_DIR_READ;
      nAddr |= BUS_DIR_READ;
    }
    else
    {
      // enable only the MB and Error interrupts
      ptI2c->INTENSET.reg = SERCOM_I2CM_INTENSET_MB | SERCOM_I2CM_INTENSET_ERROR;
      ptCtl->eBusDir = BUS_DIR_RDWRITE;
    }

    // now write the address with read bit set
    #ifdef __SAM_D21_SUBFAMILY
    while( ptI2c->SYNCBUSY.bit.SWRST );
    #endif //  __SAM_D21_SUBFAMILY
    ptI2c->ADDR.bit.ADDR = nAddr;

    // check for a blocking device
    if ( ptDef->pvCallBack == NULL )
    {
      ptCtl->bRunning = TRUE;
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

      // copy the error
      eError = ptCtl->eError;
      
      // disable the interrupts
      ptI2c->INTENCLR.reg = SERCOM_I2CM_INTENCLR_MB | SERCOM_I2CM_INTENCLR_SB | SERCOM_I2CM_INTENCLR_ERROR;
    }
    else
    {
      // set the error to busy
      eError = I2C_ERR_BLKING;
    }
  }
  else
  {
    // illegal device
    eError = I2C_ERR_ILLDEV;
  }
  
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
  PI2CDEF     ptDef;
  PU32        puData;
  SercomI2cm* ptI2c;
  U32         uTime;
  PI2CCHKBSY  ptBusyParams;

  // set the pointer to avoid compiler errors
  puData = ( PU32 )pvData;
  
  // check for a valid I2C
  if ( eDev < I2C_DEV_ENUM_MAX )
  {
    // get pointers to the control/def structure
    ptDef = ( PI2CDEF )&g_atI2cDefs[ eDev ];
    ptI2c = GetSercomChannel( ptDef->eChan );

    // process the action
    switch( eAction )
    {
      case I2C_ACTION_POLL_DEVICE :
        // get the parameters
        ptBusyParams = ( PI2CCHKBSY )pvData;

        // clear any error bits in the status/clear ACK flag
        ptI2c->STATUS.reg = SERCOM_I2CM_STATUS_MASK;
        ptI2c->CTRLB.reg &= ~SERCOM_I2CM_CTRLB_ACKACT;

        // now write the address with read bit set
        #ifdef __SAM_D21_SUBFAMILY
        while( ptI2c->SYNCBUSY.bit.SWRST );
        #endif //  __SAM_D21_SUBFAMILY
        ptI2c->ADDR.bit.ADDR = ( ptBusyParams->nDevAddr << 1 ) | ptBusyParams->bReadMode;

        // get the status - check for flag
        uTime = SystemTick_GetTimeMsec( ) + 10;
        while( SystemTick_GetTimeMsec( ) < uTime  )
        {
          // check for MB flag
          if ( ptI2c->INTFLAG.bit.MB )
          {
            // clear the flag
            ptI2c->INTFLAG.reg = SERCOM_I2CM_INTFLAG_MB;

            // break;
            break;
          }
        }

        // return the status based on RXNACK
        if ( ptI2c->STATUS.reg & SERCOM_I2CM_STATUS_RXNACK )
        {
          // set error 
          eError = I2C_ERR_SLVNAK;
        }

        // issue stop
        #ifdef __SAM_D21_SUBFAMILY
        while( ptI2c->SYNCBUSY.reg & SERCOM_I2CM_SYNCBUSY_MASK );
        #endif //  __SAM_D21_SUBFAMILY
        ptI2c->CTRLB.reg |= SERCOM_I2CM_CTRLB_CMD( CMD_SEND_STOP );

        // clear status bits
        ptI2c->STATUS.reg = SERCOM_I2CM_STATUS_MASK;
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
  I2CERR       eError = I2C_ERR_NONE;
  SercomI2cm*  ptI2c;
  
  // check for a valid UART
  if ( eDev < I2C_DEV_ENUM_MAX )
  {
    // get the pointer to the USART channel
    ptI2c = GetSercomChannel( g_atI2cDefs[ eDev ].eChan );

    // disable the USART
    while( ptI2c->SYNCBUSY.reg & SERCOM_I2CM_SYNCBUSY_MASK );
    ptI2c->CTRLA.reg &= ~SERCOM_I2CM_CTRLA_ENABLE;

    // enable the interrupt in the NVIC
    NVIC_DisableIRQ( SERCOM0_IRQn + g_atI2cDefs[ eDev ].eChan );
  }
  else
  {
    // illegal device
    eError = I2C_ERR_ILLDEV;
  }
  
  // return the error
  return( eError );
}

#ifdef SERCOM0
#if ( I2C_CHAN0_IN_USE == 1)
/******************************************************************************
 * @function SERCOM0_Handler
 *
 * @brief interrupt SERCOM 0 handler
 *
 * This function handles the interrupts from SERCOM 0
 *
 *****************************************************************************/
void SERCOM0_Handler( void )
{
  // call the common handler
  IrqCommonHandler( aePhyToDefMap[ I2C_CHAN_0 ] );
}
#endif // I2C_CHAN0_IN_USE
#endif // SERCOM0

#ifdef SERCOM1 
#if ( I2C_CHAN1_IN_USE == 1)
/******************************************************************************
 * @function SERCOM1_Handler
 *
 * @brief interrupt SERCOM 1 handler
 *
 * This function handles the interrupts from SERCOM 1
 *
 *****************************************************************************/
void SERCOM1_Handler( void )
{
  // call the common handler
  IrqCommonHandler( aePhyToDefMap[ I2C_CHAN_1 ] );
}
#endif // I2C_CHAN1_IN_USE
#endif // SERCOM1

#ifdef SERCOM2
#if ( I2C_CHAN2_IN_USE == 1 )
/******************************************************************************
 * @function SERCOM2_Handler
 *
 * @brief interrupt SERCOM 2 handler
 *
 * This function handles the interrupts from SERCOM 2
 *
 *****************************************************************************/
void SERCOM2_Handler( void )
{
  // call the common handler
  IrqCommonHandler( aePhyToDefMap[ I2C_CHAN_2 ] );
}
#endif // I2C_CHAN2_IN_USE
#endif // SERCOM2

#ifdef SERCOM3
#if ( I2C_CHAN3_IN_USE == 1 )
/******************************************************************************
 * @function SERCOM3_Handler
 *
 * @brief interrupt SERCOM 3 handler
 *
 * This function handles the interrupts from SERCOM 3
 *
 *****************************************************************************/
void SERCOM3_Handler( void )
{
  // call the common handler
  IrqCommonHandler( aePhyToDefMap[ I2C_CHAN_3 ] );
}
#endif // I2C_CHAN3_IN_USE
#endif // SERCOM3

#ifdef SERCOM4
#if ( I2C_CHAN4_IN_USE == 1 )
/******************************************************************************
 * @function SERCOM4_Handler
 *
 * @brief interrupt SERCOM 4 handler
 *
 * This function handles the interrupts from SERCOM 4
 *
 *****************************************************************************/
void SERCOM4_Handler( void )
{
  // call the common handler
  IrqCommonHandler( aePhyToDefMap[ I2C_CHAN_4 ] );
}
#endif // I2C_CHAN4_IN_USE
#endif // SERCOM4

#ifdef SERCOM5
#if ( I2C_CHAN5_IN_USE == 1 )
/******************************************************************************
 * @function SERCOM5_Handler
 *
 * @brief interrupt SERCOM 5 handler
 *
 * This function handles the interrupts from SERCOM 5
 *
 *****************************************************************************/
void SERCOM5_Handler( void )
{
  // call the common handler
  IrqCommonHandler( aePhyToDefMap[ I2C_CHAN_5 ] );
}
#endif // I2C_CHAN5_IN_USE
#endif // SERCOM5

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
  PI2CDEF       ptDef;
  PLCLCTL       ptCtl;
  SercomI2cm*   ptI2c;
  U8            nAddr;
  
  // get the definition/control structure pointers
  ptDef = ( PI2CDEF )&g_atI2cDefs[ eDev ];
  ptCtl = &atLclCtls[ eDev ];

  // get the pointer to the register
  ptI2c = GetSercomChannel( ptDef->eChan );

  // check for error first
  if ( ptI2c->INTFLAG.bit.ERROR )
  {
    // clear the flag
    ptI2c->INTFLAG.reg |= SERCOM_I2CM_INTFLAG_ERROR;
    
    // set the error and exit
    ProcessComplete( I2C_ERR_BUSFAULT, ptDef, ptCtl, ptI2c );
  }

  // check master on bus
  if ( ptI2c->INTFLAG.bit.MB )
  {
    // clear the flag
    ptI2c->INTFLAG.reg = SERCOM_I2CM_INTFLAG_MB;

    // now determine the action
    if ( ptI2c->STATUS.bit.ARBLOST )
    {
      // error - arbitration lost
      ProcessComplete( I2C_ERR_ARBLOST, ptDef, ptCtl, ptI2c );
    }
    else if ( ptI2c->STATUS.bit.RXNACK )
    {
      // error - NAK
      ProcessComplete( I2C_ERR_SLVNAK, ptDef, ptCtl, ptI2c );
    }
    else
    {
      // process the bus write
      switch( ProcessMasterWrite( ptCtl, ptI2c ))
      {
        case PRC_WR_ACTION_DONE :
          //  call process complete
          ProcessComplete( I2C_ERR_NONE, ptDef, ptCtl, ptI2c );
          break;

        case PRC_WR_ACTION_RESTART :
          // allow all interrupts
          ptI2c->INTENSET.reg = SERCOM_I2CM_INTENSET_MB | SERCOM_I2CM_INTENSET_SB | SERCOM_I2CM_INTENSET_ERROR;

          // compute the bus address
          nAddr = ( ptCtl->ptXfrCtl->nDevAddr << 1 ) | BUS_DIR_READ;

          // now write the address with read bit set
          while( ptI2c->SYNCBUSY.reg != 0 );
          ptI2c->ADDR.bit.ADDR = nAddr;
          break;

        default :
          break;
      }
    }
  }

  // check slave on bus
  if ( ptI2c->INTFLAG.bit.SB )
  {
    // clear the flag
    ptI2c->INTFLAG.reg = SERCOM_I2CM_INTFLAG_SB;

    // check for read
    if ( ProcessMasterRead( ptCtl, ptI2c ) == TRUE )
    {
      //  call process complete
      ProcessComplete( I2C_ERR_NONE, ptDef, ptCtl, ptI2c );
    }
  }
}

/******************************************************************************
 * @function ProcessMasterWrite
 *
 * @brief process the master write 
 *
 * This function determines if data is to be written and processes it
 *
 * @param[in]   eError    error
 * @param[in]   ptDef     pointer to the definition structure
 * @param[in]   ptCtl     pointer to the control structure
 * @param[in]   ptI2c     pointer to the I2C module
 *
 * @return      appropriate action
 *
 *****************************************************************************/
static PRCWRACTION ProcessMasterWrite( PLCLCTL ptCtl, SercomI2cm* ptI2c )
{
  U8            nData;
  PRCWRACTION   eAction = PRC_WR_ACTION_CONT;

  // is there any address that needs to be sent
  if ( ptCtl->ptXfrCtl->nAddrLen != 0 )
  {
    // get the address byte/decrement the length/set send data
    nData = ptCtl->ptXfrCtl->tAddress.anValue[ ptCtl->ptXfrCtl->nAddrLen - 1 ];
    ptCtl->ptXfrCtl->nAddrLen--;
  }
  else if ( ptCtl->ptXfrCtl->wDataLen != 0 )
  {
    // determine if this a write
    if ( ptCtl->eBusDir == BUS_DIR_WRITE )
    {
      // get data/set data send
      nData = *( ptCtl->ptXfrCtl->pnData + ptCtl->wBufIdx++ );
      ptCtl->ptXfrCtl->wDataLen--;
    }
    else
    {
      // this must be a read/force a restart
      eAction = PRC_WR_ACTION_RESTART;
    }
  }
  else
  {
    // set the action to end
    eAction = PRC_WR_ACTION_DONE;
  }

  // check for data to write
  if ( eAction == PRC_WR_ACTION_CONT )
  {
    // send the data
    while( ptI2c->SYNCBUSY.reg != 0 );
    ptI2c->DATA.reg = nData;
  }  

  // return the action
  return( eAction );
}

/******************************************************************************
 * @function ProcessMasterRead
 *
 * @brief process the master read 
 *
 * This function determines if data is to be read and processes it
 *
 * @param[in]   eError    error
 * @param[in]   ptDef     pointer to the definition structure
 * @param[in]   ptCtl     pointer to the control structure
 * @param[in]   ptI2c     pointer to the I2C module
 *
 * @return      TRUE if done, FALSE if more data
 *
 *****************************************************************************/
static BOOL ProcessMasterRead( PLCLCTL ptCtl, SercomI2cm* ptI2c )
{
  BOOL  bStatus = FALSE;
  U8    nData;

  // decrement the length
  ptCtl->ptXfrCtl->wDataLen--;

  // determine if we are done
  if ( ptCtl->ptXfrCtl->wDataLen == 0 )
  {
    // set NACK
    ptI2c->CTRLB.reg |= SERCOM_I2CM_CTRLB_ACKACT;

    // send a stop
    while( ptI2c->SYNCBUSY.reg != 0 );
    ptI2c->CTRLB.reg |= SERCOM_I2CM_CTRLB_CMD( CMD_SEND_STOP );

    // set status to true
    bStatus = TRUE;
  }

  // read the current byte
  nData = ptI2c->DATA.reg;
  *( ptCtl->ptXfrCtl->pnData + ptCtl->wBufIdx++ ) = nData;

  // return the state
  return( bStatus );
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
 * @param[in]   ptI2c     pointer to the I2C module
 *
 *****************************************************************************/
static void ProcessComplete( I2CERR eError, PI2CDEF ptDef, PLCLCTL ptCtl, SercomI2cm* ptI2c )
{
  // clear the funning flag/set the state to idle/set the error
  ptCtl->bRunning = FALSE;
  ptCtl->eError = eError;

  // issue stop
  while( ptI2c->SYNCBUSY.reg & SERCOM_I2CM_SYNCBUSY_MASK );
  ptI2c->CTRLB.reg |= SERCOM_I2CM_CTRLB_CMD( CMD_SEND_STOP );

  // check to see if the callback is not null
  if ( ptDef->pvCallBack != NULL )
  {
    // call it
    ptDef->pvCallBack( 0 );
  }
}

/******************************************************************************
 * @function ComputeBaudRate
 *
 * @brief compute baud rate
 *
 * This function computes the value for the baud rate register
 *
 * @param[in]   uBaud     desired baud rate
 *
 * @return      computer baud rate
 *
 *****************************************************************************/
static U32 ComputeBaudRate( I2CCHAN eChan, U32 uBaud )
{
  U32   uPeripheralClock, uBaudRate;
  
  // get the system clock value
  uPeripheralClock = Clock_GetMultiplexerFreq( CLOCK_MUXID_SERCOM_0 + eChan );
  
  // calculate the baud rate
  //uBaudRate = divceil( uPeripheralClock, ( 2000*( uBaud ))) - 5;
  uBaudRate = (( uPeripheralClock / uBaud ) / 2 ) - 5;

  // return it
  return( uBaudRate & 0xFF );
}

/******************************************************************************
 * @function GetSercomChannel
 *
 * @brief gets a pointer to the control structure
 *
 * This function will return a pointer to the correct control structure
 * for a given channel
 *
 * @param[in]   eChan			SERCOM channel
 *
 * @return      pointer to a SercomI2c or NULL if illegal channel
 *
 *****************************************************************************/
static SercomI2cm* GetSercomChannel( I2CCHAN eChan )
{
  Sercom* ptSercom = NULL;
  
  switch( eChan )
  {
    case I2C_CHAN_0 :
      ptSercom = SERCOM0;
      break;
      
    case I2C_CHAN_1 :
      ptSercom = SERCOM1;
      break;
    
    #ifdef SERCOM2
    case I2C_CHAN_2:
      ptSercom = SERCOM2;
      break;
    #endif // SERCOM2
    
    #ifdef SERCOM3
    case I2C_CHAN_3 :
      ptSercom = SERCOM3;
      break;
    #endif // SERCOM3
    
    #ifdef SERCOM4
    case I2C_CHAN_4 :
      ptSercom = SERCOM4;
      break;
    #endif // SERCOM4
    
    #ifdef SERCOM5
    case I2C_CHAN_5 :
      ptSercom = SERCOM5;
      break;
    #endif // SERCOM5
    
    default :
      ptSercom = NULL;
      break;
  }
  
  // return the pointer to the channlel
  return( &ptSercom->I2CM );
}

/**@} EOF I2c.c */
