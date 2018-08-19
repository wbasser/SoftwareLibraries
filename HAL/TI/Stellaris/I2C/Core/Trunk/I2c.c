/******************************************************************************
 * @file I2C.c
 *
 * @brief I2C implementation
 *
 * This file provides the implemntation for the I2C peripherals
 *
 * @copyright Copyright (c) 2012 Cyber Intergration
 * This document contains proprietary data and information of Endurance Products 
 * LLC. It is the exclusive property of Endurance Products, LLC and will not be 
 * disclosed in any form to any party without prior written permission of 
 * Endurance Products, LLC. This document may not be reproduced or further used 
 * without the prior written permission of Endurance Products, LLC.
 *
 * Version History
 * ======
 * $Log: $
 * 
 *
 * \addtogroup I2C
 * @{
 *****************************************************************************/

// system includes ------------------------------------------------------------

// library includes -----------------------------------------------------------

// local includes -------------------------------------------------------------
#include "I2C/I2c.h"
#include "Interrupt/Interrupt.h"
#include "SystemTick/SystemTick.h"

// debug include
#if ( I2C_DEBUG_ENABLED == 1 )
#include "DebugManager/DebugManager.h"
#endif  // I2C_DEBUG_ENABLED

// Macros and Defines ---------------------------------------------------------
#define MAX_NUM_I2CS 2

// enumerations ---------------------------------------------------------------
/// enumerate the I2C physical devices
typedef enum _PHYDEV
{
  PHY_I2C_DEV0 = 0,      ///< physical I2C 0
  PHY_I2C_DEV1,          ///< physical I2C 1
} PHYDEV;

/// enumerate the states
typedef enum _STATES
{
  STATE_IDLE = 0,       ///< idle 
  STATE_ADDR_NEXT,      ///< transmit the next address byte
  STATE_DATA_NEXT,      ///< transmit the next byte
  STATE_WAIT_ACK,       ///< wait for ack
  STATE_RCV_NEXT,       ///< receive next byte
  STATE_RCV_LAST,       ///< receive last byte
} STATES;

// structures -----------------------------------------------------------------
/// define the local control structure
typedef struct _LCLCTLS
{
  I2CXFRCTL  tI2cCtl;      ///< I2C control 
  U8      nBufIdx;      ///< buffer index
  STATES  eState;       ///< current state
  I2CERR  eError;       ///< current error
  BOOL    bRunning;     ///< running state
  BOOL    bReadMode;    ///< read mode
} LCLCTL, *PLCLCTL;
#define LCLCTLS_SIZE    sizeof( LCLCTLS )

/// define the physical to logical mapping structure
typedef struct _PHYLOGMAP
{
  I2CDEVENUM eDev;   ///< logical enumeration
} PHYLOGMAP, *PPHYLOGMAP;
#define PHYLOGMAP_SIZE  sizeof( PHYLOGMAP )

typedef struct
{
  U8  nStatus;
  U8  nState;
} DBGDATA;

// local parameter declarations -----------------------------------------------
static  LCLCTL    atLclCtls[ I2C_DEV_MAX ];
static  PHYLOGMAP atMaps[ MAX_NUM_I2CS ];

// local function prototypes --------------------------------------------------
static  I2CERR  CheckBusBusy( U32 uDevBase );
static  void    Irq0Handler( void );
static  void    Irq1Handler( void );
static  void    IrqCommonHandler( PHYDEV ePhy );
static  void    ProcessComplete( I2CERR eError, PI2CDEF ptDef, PLCLCTL ptCtl );
static  U8      GetAddressByte( PI2CXFRCTL ptI2cCtl );
static  U8      GetDataByte( PLCLCTL ptLclCtl );
static  void    PutDataByte( PLCLCTL ptLclCtl, U8 nData );

/******************************************************************************
 * @function I2c_Initialize
 *
 * @brief I2C initialization
 *
 * This function all devices in the table
 *
 * @return      appropriate error value
 *
 *****************************************************************************/
void I2c_Initialize( void )
{
  PI2CDEF     ptDef;
  PVOID       pvIrqHandler;
  I2CDEVENUM  eDev;
  
  // for each device
  for ( eDev = 0; eDev < I2C_DEV_MAX; eDev++ )
  {
    // get pointer to the definition structure
    ptDef = ( PI2CDEF )&atI2cDefs[ eDev ];
    
    // check for peripheral present
    if ( MAP_SysCtlPeripheralPresent( ptDef->uDevPeriph ))
    {
      // initialize the I2C/GPIO peripherals
      MAP_SysCtlPeripheralEnable( ptDef->uDevPeriph );
      MAP_SysCtlPeripheralEnable( ptDef->uGpioPeriph );
      
      // initialize the pins
      if ( CLASS_IS_TEMPEST || CLASS_IS_FIRESTORM )
      {
        MAP_GPIOPinConfigure( ptDef->uSdaPinFunc );
        MAP_GPIOPinConfigure( ptDef->uSclPinFunc );
      }
      
      // initialize the normal GPIO pins as I2CS
      MAP_GPIOPinTypeI2C( ptDef->uGpioBase, ( ptDef->nSdaPin | ptDef->nSclPin ));
      
      // enable the controller
      MAP_I2CMasterInitExpClk( ptDef->uDevBase, MAP_SysCtlClockGet( ), ptDef->bFastMode );
      MAP_I2CMasterEnable( ptDef->uDevBase );
  
      // set the appropriate interrupt handler/IRQ
      switch( ptDef->uDevBase )
      {
        case I2C0_MASTER_BASE :
          pvIrqHandler = Irq0Handler;
          atMaps[ PHY_I2C_DEV0 ].eDev = eDev;
          break;
          
        case I2C1_MASTER_BASE :
          pvIrqHandler = Irq1Handler;
          atMaps[ PHY_I2C_DEV1 ].eDev = eDev;
          break;
          
        default :
          break;
      }
  
      // register/enable the interrupt/enable the master interrupt
      I2CIntRegister( ptDef->uDevBase, pvIrqHandler );
    }
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
 * @param[in]   ptI2Cctl    pointer to the control structure
 *
 * @return      appropriate error value
 *
 *****************************************************************************/
I2CERR I2c_Write( I2CDEVENUM eDev, PI2CXFRCTL ptI2cCtl )
{
  I2CERR 	  eError = I2C_ERR_NONE;
  PI2CDEF 	ptDef;
  PLCLCTL 	ptCtl;
  U32       uTime;  

  // check for a valid I2C
  if ( eDev < I2C_DEV_MAX )
  {
    // get pointer to the definition structure
    ptDef = ( PI2CDEF )&atI2cDefs[ eDev ];
    ptCtl = &atLclCtls[ eDev ];
    
    // wait till the bus is not busy
    if (( eError = CheckBusBusy( ptDef->uDevBase )) == I2C_ERR_NONE )
    {
      // copy the values to the local controls
      memcpy( &ptCtl->tI2cCtl, ptI2cCtl, I2CXFRCTL_SIZE );
      ptCtl->nBufIdx = 0;
      ptCtl->bReadMode = FALSE;

      // set the running flag
      ptCtl->bRunning = TRUE;

#if ( I2C_DEBUG_ENABLED == 1 )
DebugManager_AddElement( I2C_DEBUG_BASE_VALUE, 0 );
#endif  // I2C_DEBUG_ENABLED
      // set the slave address/ enable interrupts
      MAP_I2CMasterSlaveAddrSet( ptDef->uDevBase, ptI2cCtl->nDevAddr, FALSE );
      MAP_I2CMasterIntEnable( ptDef->uDevBase );
  
      // check for a command address length
      if ( ptCtl->tI2cCtl.nAddrLen != 0 )
      {
        // output an a command/address byte
        MAP_I2CMasterDataPut( ptDef->uDevBase, GetAddressByte( &ptCtl->tI2cCtl ));

        // determine the state/command
        if (( ptCtl->tI2cCtl.nAddrLen == 1 ) && ( ptCtl->tI2cCtl.wDataLen == 0 ))
        {
          // set the state to wait for an ack/send single send
          ptCtl->eState = STATE_WAIT_ACK;
          MAP_I2CMasterControl( ptDef->uDevBase, I2C_MASTER_CMD_SINGLE_SEND );
        }
        else
        {
          // set the state to next address/send burst send
          ptCtl->eState = STATE_ADDR_NEXT;
          MAP_I2CMasterControl( ptDef->uDevBase, I2C_MASTER_CMD_BURST_SEND_START );
        }
      }
      else if ( ptCtl->tI2cCtl.wDataLen != 0 )
      {
        // output a transmit byte
        MAP_I2CMasterDataPut( ptDef->uDevBase, GetDataByte( ptCtl ));

        // determine state/command
        if ( ptCtl->tI2cCtl.wDataLen > 1 )
        {
          // set the state to transmit next/send a burst send
          ptCtl->eState = STATE_DATA_NEXT;
          MAP_I2CMasterControl( ptDef->uDevBase, I2C_MASTER_CMD_BURST_SEND_START );
        }
        else
        {
          // set the state to wait for an ack/send single send
          ptCtl->eState = STATE_WAIT_ACK;
          MAP_I2CMasterControl( ptDef->uDevBase, I2C_MASTER_CMD_SINGLE_SEND );
        }
      }
      else
      {
        // set the illegal parameter
        eError = I2C_ERR_ILLPRM;
      }

      // if no error set, continue
      if ( eError == I2C_ERR_NONE )
      {
        // check for a blocking device
        if ( ptDef->pvCallBack == NULL )
        {
          // wait till done or timeout
          SystemTick_DelayMsec( ptCtl->tI2cCtl.uTimeout );
          while( ptCtl->bRunning )
          {
            if ( ptCtl->tI2cCtl.uTimeout != 0 )
            {
              // check for timeout
              if ( SystemTick_IsDelayExpired( ))
              {
                // timeout occured - flag error
                ptCtl->bRunning = FALSE;
                ptCtl->eError = I2C_ERR_TIMEOUT;
              }
            }
          }

          // set the error
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
      // set the error busy
      eError = I2C_ERR_BUSBUSY;
    }
  }
  else
  {
    // illegal device
    eError = I2C_ERR_ILLDEV;
  }

  // disable interrupts
  MAP_I2CMasterIntDisable( ptDef->uDevBase );

  // return the status
  return(( I2CERR )eError );
}

/******************************************************************************
 * @function I2c_Read
 *
 * @brief read some characters from the I2C
 *
 * This function will read bytes from the buffer
 *
 * @param[in]   eDev        Device enumeration
 * @param[in]   ptI2Cctl    pointer to the control structure
 *
 * @return      appropriate error value
 *
 *****************************************************************************/
I2CERR I2c_Read( I2CDEVENUM eDev, PI2CXFRCTL ptI2cCtl )
{
  I2CERR  eError = I2C_ERR_NONE;
  PI2CDEF ptDef;
  PLCLCTL ptCtl;
  
  // check for a valid I2C
  if ( eDev < I2C_DEV_MAX )
  {
    // get pointers to the control/def structure
    ptDef = ( PI2CDEF )&atI2cDefs[ eDev ];
    ptCtl = &atLclCtls[ eDev ];

    // wait till the bus is not busy
    if (( eError = CheckBusBusy( ptDef->uDevBase )) == I2C_ERR_NONE )
    {
      // copy the values to the local controls
      memcpy( &ptCtl->tI2cCtl, ptI2cCtl, I2CXFRCTL_SIZE );
      ptCtl->nBufIdx = 0;
      ptCtl->bReadMode = TRUE;

#if ( I2C_DEBUG_ENABLED == 1 )
DebugManager_AddElement( I2C_DEBUG_BASE_VALUE, 1 );
#endif  // I2C_DEBUG_ENABLED
      // set the slave address/enable interrupts
      MAP_I2CMasterSlaveAddrSet( ptDef->uDevBase, ptCtl->tI2cCtl.nDevAddr, FALSE );
      MAP_I2CMasterIntEnable( ptDef->uDevBase );
      
      // check for a valid read length
      if ( ptCtl->tI2cCtl.wDataLen != 0 )
      {
        // set the running flag
        ptCtl->bRunning = TRUE;

        // check for a command address length
        if ( ptCtl->tI2cCtl.nAddrLen != 0 )
        {
          // output an a command/address byte
          MAP_I2CMasterDataPut( ptDef->uDevBase, GetAddressByte( &ptCtl->tI2cCtl ));

          // determine the state
          ptCtl->eState = ( ptCtl->tI2cCtl.nAddrLen == 0 ) ? STATE_WAIT_ACK : STATE_ADDR_NEXT;
          
          // always send a continue
          MAP_I2CMasterControl( ptDef->uDevBase, I2C_MASTER_CMD_BURST_SEND_START );
        }
        else 
        {
          // set the slave address with a read
          MAP_I2CMasterSlaveAddrSet( ptDef->uDevBase, ptCtl->tI2cCtl.nDevAddr, TRUE );
          
          // determine our control/state
          if ( ptCtl->tI2cCtl.wDataLen > 1 )
          {
            // send read cont/goto read next
            MAP_I2CMasterControl( ptDef->uDevBase, I2C_MASTER_CMD_BURST_RECEIVE_START );
            ptCtl->eState = STATE_RCV_NEXT;
          }
          else
          {
            // send read single/goto read last
            MAP_I2CMasterControl( ptDef->uDevBase, I2C_MASTER_CMD_SINGLE_RECEIVE );
            ptCtl->eState = STATE_RCV_LAST;
          }
        }

        // check for a blocking device
        if ( ptDef->pvCallBack == NULL )
        {
          // wait till done or timeout
          SystemTick_DelayMsec( ptCtl->tI2cCtl.uTimeout );
          while( ptCtl->bRunning )
          {
            if ( ptCtl->tI2cCtl.uTimeout != 0 )
            {
              // check for timeout
              if ( SystemTick_IsDelayExpired( ))
              {
                // timeout occured - flag error
                ptCtl->bRunning = FALSE;
                ptCtl->eError = I2C_ERR_TIMEOUT;
              }
            }
          }
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
        // set the illegal parameter
        eError = I2C_ERR_ILLPRM;
      }
    }
    else
    {
      // set the error busy
      eError = I2C_ERR_BUSBUSY;
    }
  }
  else
  {
    // illegal device
    eError = I2C_ERR_ILLDEV;
  }
  
  // disable interrupts
  MAP_I2CMasterIntDisable( ptDef->uDevBase );

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
  PLCLCTL     ptCtl;
  PU32        puData;
  U32         uStatus;
  PI2CCHKBSY  ptBusyParams;

  // set the pointer to avoid compiler errors
  puData = ( PU32 )pvData;
  
  // check for a valid I2C
  if ( eDev < I2C_DEV_MAX )
  {
    // get pointers to the control/def structure
    ptDef = ( PI2CDEF )&atI2cDefs[ eDev ];
    ptCtl = &atLclCtls[ eDev ];

    // process the action
    switch( eAction )
    {
      case I2C_ACTION_POLL_DEVICE :
        // get the parameters
        ptBusyParams = ( PI2CCHKBSY )pvData;

        // set the slave address/single read
        MAP_I2CMasterSlaveAddrSet( ptDef->uDevBase, ptBusyParams->nDevAddr, TRUE );
        MAP_I2CMasterControl( ptDef->uDevBase, ( ptBusyParams->bReadMode ) ? I2C_MASTER_CMD_SINGLE_RECEIVE : I2C_MASTER_CMD_SINGLE_SEND );

        // wait till not busy
        while( MAP_I2CMasterBusy( ptDef->uDevBase ));
        
        // get tje status
        uStatus = MAP_I2CMasterErr( ptDef->uDevBase );
        if (( uStatus == I2C_MASTER_ERR_ARB_LOST ) || ( uStatus == I2C_MASTER_ERR_ADDR_ACK ))
        {
          // poll device failed
          eError = I2C_ERR_SLA_NAK;

          // clear the error
          MAP_I2CMasterControl( ptDef->uDevBase, I2C_MASTER_CMD_BURST_RECEIVE_ERROR_STOP );
        }
        else
        {
          // get a dummy data byte
          uStatus = MAP_I2CMasterDataGet( ptDef->uDevBase );
        }

#if ( I2C_DEBUG_ENABLED == 1 )
DebugManager_AddElement( I2C_DEBUG_BASE_VALUE | 0x18, eError );
#endif  // I2C_DEBUG_ENABLED
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
  I2CERR eError = I2C_ERR_NONE;
  
  // check for a valid I2C
  if ( eDev < I2C_DEV_MAX )
  {
    // disable the I2C
    I2CMasterIntDisable( atI2cDefs[ eDev ].uDevBase );
    I2CMasterDisable( atI2cDefs[ eDev ].uDevBase );
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
 * @function CheckBusBusy 
 *
 * @brief check if busy is busy
 *
 * This function handles the interrupts from I2C 0
 *
 * @param[in]   uDevBase  device base
 *
 *****************************************************************************/
static I2CERR CheckBusBusy( U32 uDevBase )
{
  I2CERR	eError = I2C_ERR_NONE;

  // if bus busy, set error
  if ( MAP_I2CMasterBusBusy( uDevBase ))
  {
    // set the error
    eError = I2C_ERR_BUSBUSY;
  }

  // return status
  return( eError );
}

/******************************************************************************
 * @function Irq0Handler
 *
 * @brief interrupt device 0 handler
 *
 * This function handles the interrupts from I2C 0
 *
 *****************************************************************************/
static void Irq0Handler( void )
{
  // call the common handler
  IrqCommonHandler( PHY_I2C_DEV0 );
}

/******************************************************************************
 * @function Irq1Handler
 *
 * @brief interrupt device 1 handler
 *
 * This function handles the interrupts from I2C 1
 *
 *****************************************************************************/
static void Irq1Handler( void )
{
  // call the common handler
  IrqCommonHandler( PHY_I2C_DEV1 );
}

/******************************************************************************
 * @function IrqCommandHandler
 *
 * @brief common interrupt handler
 *
 * This function checks for the type of interrupts and processes them
 * appropriately
 *
 * @param[in]   ePhy        physical device enumeration
 *
 *****************************************************************************/
static void IrqCommonHandler( PHYDEV ePhy )
{
  U32         uStatus;
  PI2CDEF     ptDef;
  PLCLCTL     ptCtl;
  I2CDEVENUM  eDev;
  
  // get the definition/control structure pointers
  eDev = atMaps[ ePhy ].eDev;
  ptDef = ( PI2CDEF )&atI2cDefs[ eDev ];
  ptCtl = &atLclCtls[ eDev ];
  
  // clear the interrupt
  MAP_I2CMasterIntClear( ptDef->uDevBase );
  
  // get the status
  uStatus = MAP_I2CMasterErr( ptDef->uDevBase );

#if ( I2C_DEBUG_ENABLED == 1 )
DebugManager_AddElement( I2C_DEBUG_BASE_VALUE | 0x10, (( uStatus & 0x0F ) << 8 ) | ( ptCtl->eState & 0x0F ));
#endif  // I2C_DEBUG_ENABLED
  // now process it
  switch( uStatus )
  {
    // bus error
    case I2C_MASTER_ERR_ARB_LOST :
      ProcessComplete( I2C_ERR_IF_BUS, ptDef, ptCtl );
      break;
      
    // slave address ACK error
    case I2C_MASTER_ERR_ADDR_ACK :
      // send an error stop/back to idle
      MAP_I2CMasterControl( ptDef->uDevBase, I2C_MASTER_CMD_BURST_RECEIVE_ERROR_STOP );
      ProcessComplete( I2C_ERR_SLA_NAK, ptDef, ptCtl );
      break;
      
    // slave data ACK error
    case I2C_MASTER_ERR_DATA_ACK :
      // send an error stop/back to idle
      MAP_I2CMasterControl( ptDef->uDevBase, I2C_MASTER_CMD_BURST_RECEIVE_ERROR_STOP );
      ProcessComplete( I2C_ERR_DAT_NAK, ptDef, ptCtl );
      break;

    // no error
    case I2C_MASTER_ERR_NONE :
      // process the state
      switch( ptCtl->eState )
      {
        // transmit the next address byte
        case STATE_ADDR_NEXT :
          // are there more address bytes
          MAP_I2CMasterDataPut( ptDef->uDevBase, GetAddressByte( &ptCtl->tI2cCtl ));
          
          // determine next state/command
          if ( ptCtl->tI2cCtl.nAddrLen == 0 )
          {
            // is there any transmit data to send
            if ( ptCtl->tI2cCtl.wDataLen != 0 )
            {
              // send continue/goto state based on read/write mode
              MAP_I2CMasterControl( ptDef->uDevBase, I2C_MASTER_CMD_BURST_SEND_CONT );
              ptCtl->eState = ( ptCtl->bReadMode ) ? STATE_WAIT_ACK : STATE_DATA_NEXT;
            }
            else
            {
              // send finish/go to wait ack
              MAP_I2CMasterControl( ptDef->uDevBase, I2C_MASTER_CMD_BURST_SEND_FINISH );
              ptCtl->eState = STATE_WAIT_ACK;
            }
          }
          else
          {
              // send continue/stay in this state
              MAP_I2CMasterControl( ptDef->uDevBase, I2C_MASTER_CMD_BURST_SEND_CONT );
          }
          break;
          
        // transmit the next byte
        case STATE_DATA_NEXT :
          // output a data byte
          MAP_I2CMasterDataPut( ptDef->uDevBase, GetDataByte( ptCtl ));
          
          // decrement the length/test for done
          if ( ptCtl->tI2cCtl.wDataLen == 0 )
          {
              // send finish/go to wait ack
              MAP_I2CMasterControl( ptDef->uDevBase, I2C_MASTER_CMD_BURST_SEND_FINISH );
              ptCtl->eState = STATE_WAIT_ACK;
          }
          else
          {
            // set the command/stay in state
            MAP_I2CMasterControl( ptDef->uDevBase, I2C_MASTER_CMD_BURST_SEND_CONT );
          }
          break;
          
        // queue wait for ack
        case STATE_WAIT_ACK :
          // determine if we have to issue a restart for a read
          if ( ptCtl->bReadMode )
          {
            // store the address/perform a single read
            MAP_I2CMasterSlaveAddrSet( ptDef->uDevBase, ptCtl->tI2cCtl.nDevAddr, TRUE );

            // clear the buffer index
            ptCtl->nBufIdx = 0;
          
            // one or more characters
            if ( ptCtl->tI2cCtl.wDataLen > 1 )
            {
              MAP_I2CMasterControl( ptDef->uDevBase, I2C_MASTER_CMD_BURST_RECEIVE_START );
              ptCtl->eState = STATE_RCV_NEXT;
            }
            else
            {
              MAP_I2CMasterControl( ptDef->uDevBase, I2C_MASTER_CMD_SINGLE_RECEIVE );
              ptCtl->eState = STATE_RCV_LAST;
            }
          }
          else
          {
            // post complete
            ProcessComplete( I2C_ERR_NONE, ptDef, ptCtl );
          }
          break;
        
        // receive next byte  
        case STATE_RCV_NEXT :
          // get the data/increment the index/decrement the buffer
          PutDataByte( ptCtl, MAP_I2CMasterDataGet( ptDef->uDevBase ));
                    
          // check for done
          if ( ptCtl->tI2cCtl.wDataLen == 1 )
          {
            // receive finish/goto last state
            ptCtl->eState = STATE_RCV_LAST;
            MAP_I2CMasterControl( ptDef->uDevBase, I2C_MASTER_CMD_BURST_RECEIVE_FINISH );
          }
          else
          {
            // stay in this state/send the command
            MAP_I2CMasterControl( ptDef->uDevBase, I2C_MASTER_CMD_BURST_RECEIVE_CONT );
          }
          break;
          
        // receive last byte  
        case STATE_RCV_LAST :
          // get the data/exit
          PutDataByte( ptCtl, MAP_I2CMasterDataGet( ptDef->uDevBase ));
          ProcessComplete( I2C_ERR_NONE, ptDef, ptCtl );
          break;
          
        // idle/default
        case STATE_IDLE :
        default :
          break;
      }
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
  ptCtl->eState = STATE_IDLE;
  ptCtl->eError = eError;
  
  // check to see if the callback is not null
  if ( ptDef->pvCallBack != NULL )
  {
    // call it
    ptDef->pvCallBack( eError );
  }
}

/******************************************************************************
 * @function GetAddressByte
 *
 * @brief gets an address byte
 *
 * This function wiill return the address byte
 * appropriately
 *
 * @param[in]   ptCtl     pointer to the control structure
 *
 * @return      address byte
 *
 *****************************************************************************/
static U8 GetAddressByte( PI2CXFRCTL ptI2cCtl )
{
  U8  nData;

  // decrement the length
  ptI2cCtl->nAddrLen--;

  // get the next address byte and adjust
  switch( ptI2cCtl->nAddrLen )
  {
    case 3 :
      nData = ptI2cCtl->tAddress.anValue[ LE_U32_MSB_IDX ];
      break;

    case 2 :
      nData = ptI2cCtl->tAddress.anValue[ LE_U32_MS2_IDX ];
      break;

    case 1 :
      nData = ptI2cCtl->tAddress.anValue[ LE_U32_MS1_IDX ];
      break;

    case 0 :
      nData = ptI2cCtl->tAddress.anValue[ LE_U32_LSB_IDX ];
      break;

    default :
      break;
  }

#if ( I2C_DEBUG_ENABLED == 1 )
DebugManager_AddElement( I2C_DEBUG_BASE_VALUE + 0x20, nData );
#endif  // I2C_DEBUG_ENABLED
  // return the data
  return( nData );
}

/******************************************************************************
 * @function GetDataByte
 *
 * @brief get a data byte from the buffer
 *
 * This function will get a data byte from the buffer and adjust the length
 *
 * @param[in]   ptCtl     pointer to the control structure
 *
 * @return      data byte read
 *
 *****************************************************************************/
static U8 GetDataByte( PLCLCTL ptLclCtl )
{
  U8  nData = 0;

  // ensure there is some left
  if ( ptLclCtl->tI2cCtl.wDataLen != 0 )
  {
    // get the data/decrement the transmit length
    nData = *( ptLclCtl->tI2cCtl.pnData + ptLclCtl->nBufIdx++ );
    ptLclCtl->tI2cCtl.wDataLen--;
#if ( I2C_DEBUG_ENABLED == 1 )
DebugManager_AddElement( I2C_DEBUG_BASE_VALUE + 0x21, nData );
#endif  // I2C_DEBUG_ENABLED
  }

  // return the data byte
  return( nData );
}

/******************************************************************************
 * @function PutDataByte
 *
 * @brief stuffs a data byte into the buffer
 *
 * This function will stuff data into the buffer if room
 * appropriately
 *
 * @param[in]   ptCtl     pointer to the control structure
 * @param[in]   nData     data to stuff
 *
 *****************************************************************************/
static void PutDataByte( PLCLCTL ptLclCtl, U8 nData )
{
  // ensure there is room
  if ( ptLclCtl->tI2cCtl.wDataLen != 0 )
  {
    *( ptLclCtl->tI2cCtl.pnData + ptLclCtl->nBufIdx++ ) = nData;
    ptLclCtl->tI2cCtl.wDataLen--;
#if ( I2C_DEBUG_ENABLED == 1 )
DebugManager_AddElement( I2C_DEBUG_BASE_VALUE + 0x22, nData );
#endif  // I2C_DEBUG_ENABLED
  }
}

/**@} EOF I2c.c */
