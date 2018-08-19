/******************************************************************************
 * @file SPI.c
 *
 * @brief SPI implementation
 *
 * This file provides the implemntation for the SPI peripherals
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
 * \addtogroup SPI
 * @{
 *****************************************************************************/

// system includes ------------------------------------------------------------

// library includes -----------------------------------------------------------

// local includes -------------------------------------------------------------
#include "SPI/Spi.h"
#include "Interrupt/IrqManager.h"

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------
/// enumerate the SPI physical devices
typedef enum _PHYDEV
{
  PHY_SSI_DEV0 = 0,      ///< physical SSI 0
  PHY_SSI_DEV1,         ///< physical SSI 1
  PHY_SSI_DEV2,         ///< physcial SSI 2
  PHY_SSI_DEV3,         ///< physcial SSI 3
  PHY_SSI_MAX           ///< maximum SSI
} PHYDEV;

// structures -----------------------------------------------------------------
/// define the local control structure
typedef struct _LCLCTLS
{
  PU8     pnData ;      ///< pointer to the data storage
  U8      nXmtLen;      ///< transmit length
  U8      nRcvLen;      ///< receive length
  U8      nBufIdx;      ///< buffer index
  U8      nSlvAddr;     ///< slave address
  SPIERR  eError;       ///< current error
  BOOL    bRunning;     ///< running state
} LCLCTL, *PLCLCTL;
#define LCLCTLS_SIZE    sizeof( LCLCTLS )

/// define the physical to logical mapping structure
typedef struct _PHYLOGMAP
{
  SPIDEVENUM eDev;   ///< logical enumeration
} PHYLOGMAP, *PPHYLOGMAP;
#define PHYLOGMAP_SIZE  sizeof( PHYLOGMAP )

// global parameter declarations ----------------------------------------------

// local parameter declarations -----------------------------------------------
static  LCLCTL    atLclCtls[ SPI_DEV_MAX ];
static  PHYLOGMAP atMaps[ PHY_SSI_MAX ];

// local function prototypes --------------------------------------------------
static  void    Irq0Handler( void );
static  void    Irq1Handler( void );
static  void    Irq2Handler( void );
static  void    Irq3Handler( void );
static  void    IrqCommonHandler( PHYDEV ePhy );

// constant parameter initializations -----------------------------------------

/******************************************************************************
 * @function Spi_Initialize
 *
 * @brief SPI initialization
 *
 * This function will initialize a given device 
 *
 * @param[in]   eDev        Device enumeration
 * @param[in]   uBaudRate   desired baudrate
 * @param[in]   eParity     parity 
 * @param[in]   eWrdLen     word length
 * @param[in]   eStop       stop bits
 *
 * @return      appropriate error value
 *
 *****************************************************************************/
SPIERR Spi_Initialize( SPIDEVENUM eDev  )
{
  SPIERR    eError = SPI_ERR_NONE;
  PSPIDEF   ptDef;
  PLCLCTL   ptCtl;
  PVOID     pvIrqHandler;
  
  // check for a valid SPI
  if ( eDev < SPI_DEV_MAX )
  {
    // get pointer to the definition structure
    ptDef = ( PSPIDEF )&atSpiDefs[ eDev ];
    
    // check for peripheral present
    if ( MAP_SysCtlPeripheralPresent( ptDef->uDevPeriph ))
    {
      // get pointer to the control
      ptCtl = &atLclCtls[ eDev ];
      
      // initialize the SPI/GPIO peripherals
      MAP_SysCtlPeripheralEnable( ptDef->uDevPeriph );
      MAP_SysCtlPeripheralEnable( ptDef->uGpioPeriph );
      
      // initialize the pins
      if ( CLASS_IS_TEMPEST )
      {
        MAP_GPIOPinConfigure( ptDef->uSdaPinFunc	 );
        MAP_GPIOPinConfigure( ptDef->uSclPinFunc	 );
      }
      
      // set the appropriate interrupt handler/IRQ
      switch( ptDef->uDevBase )
      {
        case SSI0_BASE :
          pvIrqHandler = Irq0Handler;
          atMaps[ PHY_SSI_DEV0 ].eDev = eDev;
          break;
          
        case SSI1_BASE :
          pvIrqHandler = Irq1Handler;
          atMaps[ PHY_SSI_DEV1 ].eDev = eDev;
          break;

        case SSI2_BASE :
          pvIrqHandler = Irq2Handler;
          atMaps[ PHY_SSI_DEV2 ].eDev = eDev;
          break;
        
        case SSI3_BASE :
          pvIrqHandler = Irq3Handler;
          atMaps[ PHY_SSI_DEV3 ].eDev = eDev;
          break;
          
        default :
          break;
      }
  
      // register/enable the interrupt/enable the master interrupt
      SPIIntRegister( ptDef->uDevBase, pvIrqHandler );
      MAP_SPIMasterIntEnable( ptDef->uDevBase );
    }
    else
    {
      // illegal device
      eError = SPI_ERR_ILLPER;
    }
  }
  else
  {
    // illegal device
    eError = SPI_ERR_ILLDEV;
  }
  
  // return the error
  return( eError );
}

/******************************************************************************
 * @function Spi_Write
 *
 * @brief write some characters to the SPI
 *
 * This function will write some characters to the SPI buffer if room
 *
 * @param[in]   eDev        Device enumeration
 * @param[in]   nSlvAddr    slave address
 * @param[in]   pnData      pointer to the data buffer
 * @param[in]   nXmtLen     size of the data buffer
 *
 * @return      appropriate error value
 *
 *****************************************************************************/
SPIERR Spi_Write( SPIDEVENUM eDev, U8 nSlvAddr, PU8 pnData, U8 nXmtLen )
{
  SPIERR 	  eError = SPI_ERR_NONE;
  PSPIDEF 	ptDef;
  PLCLCTL 	ptCtl;

  // check for a valid SPI
  if ( eDev < SPI_DEV_MAX )
  {
    // get pointer to the definition structure
    ptDef = ( PSPIDEF )&atSpiDefs[ eDev ];
    
    }
  }
  else
  {
    // illegal device
    eError = SPI_ERR_ILLDEV;
  }

  // return the status
  return(( SPIERR )eError );
}

/******************************************************************************
 * @function Spi_Read
 *
 * @brief read some characters from the SPI
 *
 * This function will read bytes from the buffer
 *
 * @param[in]   eDev        Device enumeration
 * @param[in]   pnData      pointer to the data buffer
 * @param[in]   nXmtLen     length of the transmit data
 * @param[in]	  nRcvLen	    expected length of the receive data
 *
 * @return      appropriate error value
 *
 *****************************************************************************/
SPIERR Spi_Read( SPIDEVENUM eDev, U8 nSlvAddr, PU8 pnData, U8 nXmtLen, U8 nRcvLen )
{
  SPIERR   eError = SPI_ERR_NONE;
  PSPIDEF  ptDef;
  PLCLCTL  ptCtl;
  
  // check for a valid SPI
  if ( eDev < SPI_DEV_MAX )
  {
    // get pointers to the control/def structure
    ptDef = ( PSPIDEF )&atSpiDefs[ eDev ];
    ptCtl = &atLclCtls[ eDev ];
    
  }
  else
  {
    // illegal device
    eError = SPI_ERR_ILLDEV;
  }
  
  // return the error
  return(( SPIERR )eError );
}

/******************************************************************************
 * @function Spi_Ioctl
 *
 * @brief SPI IOCTL functions
 *
 * This function provides functionality to modify the SPIs parameters
 *
 * @param[in]   eDev        Device enumeration
 * @param[in]   eAction     action to take
 * @param[io]   pvData      pointer to data storage/retrieval
 *
 * @return      appropriate error value
 *
 *****************************************************************************/
SPIERR Spi_Ioctl( SPIDEVENUM eDev, SPIACTION eAction, PVOID pvData )
{
  SPIERR eError = SPI_ERR_NONE;
  
  // check for a valid SPI
  if ( eDev < SPI_DEV_MAX )
  {
    // process the action
    switch( eAction )
    {
      case SPI_ACTION_SET_DEVADDR :
        break;
        
      case SPI_ACTION_GET_DEVADDR :
        break;
        
      default :
        // illegal action
        eError = SPI_ERR_ILLACT;
        break;
    }
  }
  else
  {
    // illegal device
    eError = SPI_ERR_ILLDEV;
  }
  
  // return the error
  return( eError );
}

/******************************************************************************
 * @function Spi_Close
 *
 * @brief Close the SPI
 *
 * This function will disable any interrupt, unregister the interrupt handler
 *
 * @param[in]   eDev        Device enumeration
 *
 * @return      appropriate error value
 *
 *****************************************************************************/
SPIERR Spi_Close( SPIDEVENUM eDev )
{
  SPIERR eError = SPI_ERR_NONE;
  
  // check for a valid SPI
  if ( eDev < SPI_DEV_MAX )
  {
  }
  else
  {
    // illegal device
    eError = SPI_ERR_ILLDEV;
  }
  
  // return the error
  return( eError );
}

/******************************************************************************
 * @function Irq0Handler
 *
 * @brief interrupt device 0 handler
 *
 * This function handles the interrupts from SPI 0
 *
 *****************************************************************************/
static void Irq0Handler( void )
{
  // call the common handler
  IrqCommonHandler( PHY_SSI_DEV0 );
}

/******************************************************************************
 * @function Irq1Handler
 *
 * @brief interrupt device 1 handler
 *
 * This function handles the interrupts from SPI 1
 *
 *****************************************************************************/
static void Irq1Handler( void )
{
  // call the common handler
  IrqCommonHandler( PHY_SSI_DEV1 );
}

/******************************************************************************
 * @function Irq2Handler
 *
 * @brief interrupt device 2 handler
 *
 * This function handles the interrupts from SPI 2
 *
 *****************************************************************************/
static void Irq2Handler( void )
{
  // call the common handler
  IrqCommonHandler( PHY_SSI_DEV2 );
}

/******************************************************************************
 * @function Irq3Handler
 *
 * @brief interrupt device 3 handler
 *
 * This function handles the interrupts from SPI 3
 *
 *****************************************************************************/
static void Irq3Handler( void )
{
  // call the common handler
  IrqCommonHandler( PHY_SSI_DEV3 );
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
  PSPIDEF     ptDef;
  PLCLCTL     ptCtl;
  SPIDEVENUM  eDev;
  
  // get the definition/control structure pointers
  eDev = &atMaps[ ePhy ].eDev;
  ptDef = ( PSPIDEF )&atSpiDefs[ eDev ];
  ptCtl = &atLclCtls[ eDev ];
}


/**@} EOF Spi.c */
