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
#include "Clock/Clock.h"
#include "I2C/I2c.h"
#include "Interrupt/Interrupt.h"
#include "PowerManager/PowerManager.h"

// Macros and Defines ---------------------------------------------------------
/// define the baud rate values for slow/fast
#define BAUD_RATE_SLOW      ( 100000l )
#define BAUD_RATE_FAST      ( 400000l )

/// define the stop command value 
#define CMD_SEND_STOP       ( 3 )

/// define the macro to map the control pointer
#define MAP_HANDLE_TO_POINTER( handle )   (( PLCLCTL )handle )

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
typedef struct _LCLCTLS
{
  PI2CXFRCTL  ptXfrCtl;     ///< transfer control ointer
  U16         wBufIdx;      ///< buffer index
  I2CERR      eError;       ///< current error
  BOOL        bRunning;     ///< running state
  BUSDIR      eBusDir;      ///< bus direction
  SercomI2cm* ptI2c;        ///< pointer to the SERCOM
  CLOCKMUXID  eClockId;     ///< clock ID
} LCLCTL, *PLCLCTL;
#define LCLCTL_SIZE           sizeof( LCLCTL )

// local parameter declarations -----------------------------------------------

// local function prototypes --------------------------------------------------
static  void          GetSercomChannel( I2CCHAN eChan, PLCLCTL ptLclCtl );
static  void          ProcessInterrupts( PLCLCTL ptLclCtl );
static  PRCWRACTION   ProcessMasterWrite( PLCLCTL ptLclCtl );
static  BOOL          ProcessMasterRead( PLCLCTL ptLclCtl );
static  void          ProcessComplete( I2CERR eError, PLCLCTL ptLclCtl );
static  U32           ComputeBaudRate( U32 uBaud, CLOCKMUXID eClockId );

/******************************************************************************
 * @function I2c_Initialize
 *
 * @brief I2C initialization
 *
 * This function all devices in the table
 *
 *****************************************************************************/
PVI2CHANDLE I2c_Configure( PI2CDEF ptI2cDef )
{
  U32                     uTemp;
  SERCOM_I2CM_CTRLA_Type  tCtrlA;
  PLCLCTL                 ptLclCtl;

  // malloc space
  if (( ptLclCtl = malloc( LCLCTL_SIZE )) != NULL )
  {
    // configure the GPIO pins
    Gpio_Configure( ptI2cDef->eDevPort, ptI2cDef->nSdaPin, GPIO_MODE_INPUT,         OFF, ptI2cDef->eDevMux, OFF );
    Gpio_Configure( ptI2cDef->eDevPort, ptI2cDef->nSclPin, GPIO_MODE_OUTPUT_INPDSB, OFF, ptI2cDef->eDevMux, OFF );

    // get pointer to the definition structure
    GetSercomChannel( ptI2cDef->eChan, ptLclCtl );

    // disable the control and wait for done
    ptLclCtl->ptI2c->CTRLA.bit.SWRST = 1;
    #ifdef __SAM_D21_SUBFAMILY
    while( ptI2c->SYNCBUSY.bit.SWRST );
    #endif //  __SAM_D21_SUBFAMILY
  
    // now set the baudrate
    uTemp = ( ptI2cDef->bFastSpeed ) ? BAUD_RATE_FAST : BAUD_RATE_SLOW;
    ptLclCtl->ptI2c->BAUD.reg = ComputeBaudRate( uTemp, ptLclCtl->eClockId );
  
    // now set up control B
    ptLclCtl->ptI2c->CTRLB.reg = SERCOM_I2CM_CTRLB_SMEN;

    // now build control A
    tCtrlA.reg = SERCOM_I2CM_CTRLA_MODE_I2C_MASTER;
    tCtrlA.reg |= SERCOM_I2CM_CTRLA_SPEED( ptI2cDef->bFastSpeed );
    tCtrlA.reg |= SERCOM_I2CM_CTRLA_ENABLE;
    tCtrlA.reg |= SERCOM_I2CM_CTRLA_INACTOUT( 1 );
    tCtrlA.reg |= SERCOM_I2CM_CTRLA_SDAHOLD( 2 );
    ptLclCtl->ptI2c->CTRLA = tCtrlA;
  
    // now wait for done
    #ifdef __SAM_D21_SUBFAMILY
    while( ptI2c->SYNCBUSY.bit.SWRST );
    #endif //  __SAM_D21_SUBFAMILY
    }

  // return the handle
  return(( PVI2CHANDLE )ptLclCtl );
}

/******************************************************************************
 * @function I2c_Write
 *
 * @brief write some characters to the I2C
 *
 * This function will write some characters to the I2C buffer if room
 *
 * @param[in]   pvI2cHandle handle
 * @param[in]   ptXftCtl    pointer to the transfer control
 *
 * @return      appropriate error value
 *
 *****************************************************************************/
I2CERR I2c_Write( PVI2CHANDLE pvI2cHandle, PI2CXFRCTL ptXfrCtl )
{
  I2CERR      eError = I2C_ERR_NONE;
  U32         uTime;  
  PLCLCTL     ptLclCtl;

  // map handle to pointer
  ptLclCtl = ( PLCLCTL  )pvI2cHandle;

  // check for a valid I2C
  if ( ptLclCtl != NULL )
  {
    // copy the values to the local controls
    ptLclCtl->ptXfrCtl = ptXfrCtl;
    ptLclCtl->wBufIdx = 0;

    // clear any error bits in the status/flag
    ptLclCtl->ptI2c->STATUS.reg = SERCOM_I2CM_STATUS_MASK;

    // set the running flag/direction/state
    ptLclCtl->bRunning = TRUE;
    ptLclCtl->eBusDir = BUS_DIR_WRITE;
    ptLclCtl->eError = I2C_ERR_NONE;

    // now write the address with read bit cleared
    #ifdef __SAM_D21_SUBFAMILY
    while( ptLclCtl->ptI2c->SYNCBUSY.bit.SWRST );
    #endif //  __SAM_D21_SUBFAMILY
    ptLclCtl->ptI2c->ADDR.bit.ADDR = ( ptXfrCtl->nDevAddr << 1 ) | BUS_DIR_WRITE;

    // wait till done or timeout
    ptLclCtl->bRunning = TRUE;
    uTime = 0x80000000;
    while( ptLclCtl->bRunning )
    {
      ProcessInterrupts( ptLclCtl );

      // check for timeout
      if ( --uTime == 0 )
      {
        // timeout occured - flag error
        ptLclCtl->bRunning = FALSE;
        ptLclCtl->eError = I2C_ERR_TIMEOUT;
      }
    }

    // copy the error
    eError = ptLclCtl->eError;
  
    // disable the interrupts
    ptLclCtl->ptI2c->INTENCLR.reg = SERCOM_I2CM_INTENCLR_MB | SERCOM_I2CM_INTENCLR_ERROR;
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
 * @param[in]   pvI2cHandle handle
 * @param[in]   ptXftCtl    pointer to the transfer control
 *
 * @return      appropriate error value
 *
 *****************************************************************************/
I2CERR I2c_Read( PVI2CHANDLE pvI2cHandle, PI2CXFRCTL ptXfrCtl )
{
  I2CERR      eError = I2C_ERR_NONE;
  U32         uTime;  
  PLCLCTL     ptLclCtl;
  U8          nAddr;

  // map handle to pointer
  ptLclCtl = ( PLCLCTL  )pvI2cHandle;

  // check for a valid I2C
  if ( ptLclCtl != NULL )
  {
    // copy the values to the local controls
    ptLclCtl->ptXfrCtl = ptXfrCtl;
    ptLclCtl->wBufIdx = 0;

    // clear any error bits in the status
    ptLclCtl->ptI2c->STATUS.reg = SERCOM_I2CM_STATUS_MASK;

    // clear the interrupts
    ptLclCtl->ptI2c->INTFLAG.reg = SERCOM_I2CM_INTFLAG_MASK;
    
    // clear the ack flag
    ptLclCtl->ptI2c->CTRLB.reg &= ~SERCOM_I2CM_CTRLB_ACKACT;

    // set the running flag/direction/state
    ptLclCtl->bRunning = TRUE;

    // clear the error
    ptLclCtl->eError = I2C_ERR_NONE;

    // compute the bus address
    nAddr = ( ptXfrCtl->nDevAddr << 1 );

    // process the address 
    if ( ptXfrCtl->nAddrLen == 0 )
    {
      // enable all the interrupts/set the bus direction/adjust the address
      ptLclCtl->ptI2c->INTENSET.reg = SERCOM_I2CM_INTENSET_MB | SERCOM_I2CM_INTENSET_SB | SERCOM_I2CM_INTENSET_ERROR;
      ptLclCtl->eBusDir = BUS_DIR_READ;
      nAddr |= BUS_DIR_READ;
    }
    else
    {
      // enable only the MB and Error interrupts
      ptLclCtl->ptI2c->INTENSET.reg = SERCOM_I2CM_INTENSET_MB | SERCOM_I2CM_INTENSET_ERROR;
      ptLclCtl->eBusDir = BUS_DIR_RDWRITE;
    }

    // now write the address with read bit set
    #ifdef __SAM_D21_SUBFAMILY
    while( ptI2c->SYNCBUSY.bit.SWRST );
    #endif //  __SAM_D21_SUBFAMILY
    ptLclCtl->ptI2c->ADDR.bit.ADDR = nAddr;

    // check for a blocking device
    ptLclCtl->bRunning = TRUE;
    uTime = 0x80000000;
    while( ptLclCtl->bRunning )
    {
      // process the interrupts
      ProcessInterrupts( ptLclCtl );

      // check for timeout
      if ( --uTime == 0  )
      {
        // timeout occured - flag error
        ptLclCtl->bRunning = FALSE;
        ptLclCtl->eError = I2C_ERR_TIMEOUT;
      }

      // copy the error
      eError = ptLclCtl->eError;
      
      // disable the interrupts
      ptLclCtl->ptI2c->INTENCLR.reg = SERCOM_I2CM_INTENCLR_MB | SERCOM_I2CM_INTENCLR_SB | SERCOM_I2CM_INTENCLR_ERROR;
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
I2CERR I2c_Ioctl( PVI2CHANDLE pvI2cHandle, I2CACTION eAction, PVOID pvData )
{
  I2CERR      eError = I2C_ERR_NONE;
  PLCLCTL     ptLclCtl;
  PU32        puData;
  PI2CCHKBSY  ptBusyParams;
  U32         uTime;

  // map handle to pointer
  ptLclCtl = ( PLCLCTL  )pvI2cHandle;

  // check for a valid I2C
  if ( ptLclCtl != NULL )
  {
    // set the pointer to avoid compiler errors
    puData = ( PU32 )pvData;
  
    // process the action
    switch( eAction )
    {
      case I2C_ACTION_POLL_DEVICE :
        // get the parameters
        ptBusyParams = ( PI2CCHKBSY )pvData;

        // clear any error bits in the status/clear ACK flag
        ptLclCtl->ptI2c->STATUS.reg = SERCOM_I2CM_STATUS_MASK;
        ptLclCtl->ptI2c->CTRLB.reg &= ~SERCOM_I2CM_CTRLB_ACKACT;

        // now write the address with read bit set
        #ifdef __SAM_D21_SUBFAMILY
        while( ptI2c->SYNCBUSY.bit.SWRST );
        #endif //  __SAM_D21_SUBFAMILY
        ptLclCtl->ptI2c->ADDR.bit.ADDR = ( ptBusyParams->nDevAddr << 1 ) | ptBusyParams->bReadMode;

        // get the status - check for flag
        uTime = 0x40000000;
        while( --uTime != 0 )
        {
          // check for MB flag
          if ( ptLclCtl->ptI2c->INTFLAG.bit.MB )
          {
            // clear the flag
            ptLclCtl->ptI2c->INTFLAG.reg = SERCOM_I2CM_INTFLAG_MB;

            // break;
            break;
          }
        }

        // return the status based on RXNACK
        if ( ptLclCtl->ptI2c->STATUS.reg & SERCOM_I2CM_STATUS_RXNACK )
        {
          // set error 
          eError = I2C_ERR_SLVNAK;
        }

        // issue stop
        #ifdef __SAM_D21_SUBFAMILY
        while( ptLclCtl->ptI2c->SYNCBUSY.reg & SERCOM_I2CM_SYNCBUSY_MASK );
        #endif //  __SAM_D21_SUBFAMILY
        ptLclCtl->ptI2c->CTRLB.reg |= SERCOM_I2CM_CTRLB_CMD( CMD_SEND_STOP );

        // clear status bits
        ptLclCtl->ptI2c->STATUS.reg = SERCOM_I2CM_STATUS_MASK;
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
 * @function ProcessInterrupts
 *
 * @brief common interrupt handler
 *
 * This function checks for the type of interrupts and processes them
 * appropriately
 *
 * @param[in]   ptLclCtl    pointer to the local control
 *
 *****************************************************************************/
static void ProcessInterrupts( PLCLCTL ptLclCtl )
{
  U8            nAddr;
  
  // check for error first
  if ( ptLclCtl->ptI2c->INTFLAG.bit.ERROR )
  {
    // clear the flag
    ptLclCtl->ptI2c->INTFLAG.reg |= SERCOM_I2CM_INTFLAG_ERROR;
    
    // set the error and exit
    ProcessComplete( I2C_ERR_BUSFAULT, ptLclCtl );
  }

  // check master on bus
  if (ptLclCtl-> ptI2c->INTFLAG.bit.MB )
  {
    // clear the flag
    ptLclCtl->ptI2c->INTFLAG.reg = SERCOM_I2CM_INTFLAG_MB;

    // now determine the action
    if ( ptLclCtl->ptI2c->STATUS.bit.ARBLOST )
    {
      // error - arbitration lost
      ProcessComplete( I2C_ERR_ARBLOST, ptLclCtl );
    }
    else if ( ptLclCtl->ptI2c->STATUS.bit.RXNACK )
    {
      // error - NAK
      ProcessComplete( I2C_ERR_SLVNAK, ptLclCtl );
    }
    else
    {
      // process the bus write
      switch( ProcessMasterWrite( ptLclCtl ))
      {
        case PRC_WR_ACTION_DONE :
          //  call process complete
          ProcessComplete( I2C_ERR_NONE, ptLclCtl );
          break;

        case PRC_WR_ACTION_RESTART :
          // allow all interrupts
          ptLclCtl->ptI2c->INTENSET.reg = SERCOM_I2CM_INTENSET_MB | SERCOM_I2CM_INTENSET_SB | SERCOM_I2CM_INTENSET_ERROR;

          // compute the bus address
          nAddr = ( ptLclCtl->ptXfrCtl->nDevAddr << 1 ) | BUS_DIR_READ;

          // now write the address with read bit set
          while( ptLclCtl->ptI2c->SYNCBUSY.reg != 0 );
          ptLclCtl->ptI2c->ADDR.bit.ADDR = nAddr;
          break;

        default :
          break;
      }
    }
  }

  // check slave on bus
  if ( ptLclCtl->ptI2c->INTFLAG.bit.SB )
  {
    // clear the flag
    ptLclCtl->ptI2c->INTFLAG.reg = SERCOM_I2CM_INTFLAG_SB;

    // check for read
    if ( ProcessMasterRead( ptLclCtl ) == TRUE )
    {
      //  call process complete
      ProcessComplete( I2C_ERR_NONE, ptLclCtl );
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
 * @param[in]   ptLclCtl     pointer to the control structure
 *
 * @return      appropriate action
 *
 *****************************************************************************/
static PRCWRACTION ProcessMasterWrite( PLCLCTL ptLclCtl )
{
  U8            nData;
  PRCWRACTION   eAction = PRC_WR_ACTION_CONT;

  // is there any address that needs to be sent
  if ( ptLclCtl->ptXfrCtl->nAddrLen != 0 )
  {
    // get the address byte/decrement the length/set send data
    nData = ptLclCtl->ptXfrCtl->tAddress.anValue[ ptLclCtl->ptXfrCtl->nAddrLen - 1 ];
    ptLclCtl->ptXfrCtl->nAddrLen--;
  }
  else if ( ptLclCtl->ptXfrCtl->wDataLen != 0 )
  {
    // determine if this a write
    if ( ptLclCtl->eBusDir == BUS_DIR_WRITE )
    {
      // get data/set data send
      nData = *( ptLclCtl->ptXfrCtl->pnData + ptLclCtl->wBufIdx++ );
      ptLclCtl->ptXfrCtl->wDataLen--;
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
    while( ptLclCtl->ptI2c->SYNCBUSY.reg != 0 );
    ptLclCtl->ptI2c->DATA.reg = nData;
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
 * @param[in]   ptLclCtl     pointer to the control structure
 *
 * @return      TRUE if done, FALSE if more data
 *
 *****************************************************************************/
static BOOL ProcessMasterRead( PLCLCTL ptLclCtl )
{
  BOOL  bStatus = FALSE;
  U8    nData;

  // decrement the length
  ptLclCtl->ptXfrCtl->wDataLen--;

  // determine if we are done
  if ( ptLclCtl->ptXfrCtl->wDataLen == 0 )
  {
    // set NACK
    ptLclCtl->ptI2c->CTRLB.reg |= SERCOM_I2CM_CTRLB_ACKACT;

    // send a stop
    while( ptLclCtl->ptI2c->SYNCBUSY.reg != 0 );
    ptLclCtl->ptI2c->CTRLB.reg |= SERCOM_I2CM_CTRLB_CMD( CMD_SEND_STOP );

    // set status to true
    bStatus = TRUE;
  }

  // read the current byte
  nData = ptLclCtl->ptI2c->DATA.reg;
  *( ptLclCtl->ptXfrCtl->pnData + ptLclCtl->wBufIdx++ ) = nData;

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
 * @param[in]   ptLclCtl     pointer to the control structure
 *
 *****************************************************************************/
static void ProcessComplete( I2CERR eError, PLCLCTL ptLclCtl )
{
  // clear the funning flag/set the state to idle/set the error
  ptLclCtl->bRunning = FALSE;
  ptLclCtl->eError = eError;

  // issue stop
  while( ptLclCtl->ptI2c->SYNCBUSY.reg & SERCOM_I2CM_SYNCBUSY_MASK );
  ptLclCtl->ptI2c->CTRLB.reg |= SERCOM_I2CM_CTRLB_CMD( CMD_SEND_STOP );
}

/******************************************************************************
 * @function ComputeBaudRate
 *
 * @brief compute baud rate
 *
 * This function computes the value for the baud rate register
 *
 * @param[in]   uBaud     desired baud rate
 * @param[in]   eClockId  clock mux ID
 *
 * @return      computer baud rate
 *
 *****************************************************************************/
static U32 ComputeBaudRate( U32 uBaud, CLOCKMUXID eClockId )
{
  U32   uPeripheralClock, uBaudRate;
  
  // get the system clock value
  uPeripheralClock = Clock_GetPeriphClock( eClockId );
  
  // calculate the baud rate
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
 * @param[in]   ptLclCtl  pointer to the local control structure
 *
 *****************************************************************************/
static void GetSercomChannel( I2CCHAN eChan, PLCLCTL ptLclCtl )
{
  U32         uPeriphId;
  
  switch( eChan )
  {
    case I2C_CHAN_0 :
      ptLclCtl->ptI2c = &SERCOM0->I2CM;
      ptLclCtl->eClockId = CLOCK_MUXID_SERCOM_0;
      uPeriphId = PM_APBCMASK_SERCOM0;
      break;
      
    case I2C_CHAN_1 :
      ptLclCtl->ptI2c = &SERCOM1->I2CM;
      ptLclCtl->eClockId = CLOCK_MUXID_SERCOM_1;
      uPeriphId = PM_APBCMASK_SERCOM1;
      break;
    
    #ifdef SERCOM2
    case I2C_CHAN_2:
      ptLclCtl->ptI2c = &SERCOM2->I2CM;
      ptLclCtl->eClockId = CLOCK_MUXID_SERCOM_2;
      uPeriphId = PM_APBCMASK_SERCOM2;
      break;
    #endif // SERCOM2
    
    #ifdef SERCOM3
    case I2C_CHAN_3 :
      ptLclCtl->ptI2c = &SERCOM3->I2CM;
      ptLclCtl->eClockId = CLOCK_MUXID_SERCOM_3;
      uPeriphId = PM_APBCMASK_SERCOM3;
      break;
    #endif // SERCOM3
    
    #ifdef SERCOM4
    case I2C_CHAN_4 :
      ptLclCtl->ptI2c = &SERCOM4->I2CM;
      ptLclCtl->eClockId = CLOCK_MUXID_SERCOM_4;
      uPeriphId = PM_APBCMASK_SERCOM4;
      break;
    #endif // SERCOM4
    
    #ifdef SERCOM5
    case I2C_CHAN_5 :
      ptLclCtl->ptI2c = &SERCOM5->I2CM;
      ptLclCtl->eClockId = CLOCK_MUXID_SERCOM_5;
      uPeriphId = PM_APBCMASK_SERCOM5;
      break;
    #endif // SERCOM5
    
    default :
      ptLclCtl->ptI2c = NULL;
      break;
  }
  
  // now enable the clock and power mask
  Clock_PeriphEnable( ptLclCtl->eClockId, CLOCK_GENID_0 );
  PowerManager_DisableEnablePeriphC( uPeriphId, ON );
}

/**@} EOF I2c.c */
