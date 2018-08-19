/******************************************************************************
 * @file Uart.c
 *
 * @brief UART implementation
 *
 * This file provides the implemntation for the UART peripherals
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
 * \addtogroup UART
 * @{
 *****************************************************************************/

// system includes ------------------------------------------------------------

// library includes -----------------------------------------------------------

// local includes -------------------------------------------------------------
#include "UART/Uart.h"
#include "Interrupt/Interrupt.h"

// Macros and Defines ---------------------------------------------------------
#define MAX_NUM_UARTS 3
// enumerations ---------------------------------------------------------------
/// enumerate the UART physical devices
typedef enum _PHYDEV
{
  PHY_UART_DEV0 = 0,      ///< physical UART 0
  PHY_UART_DEV1,          ///< physical UART 1
  PHY_UART_DEV2,          ///< physical UART 2
} PHYDEV;

// structures -----------------------------------------------------------------
/// define the buffer control structure
typedef struct _BUFCTL
{
  U16  wWrIdx;     ///< write index
  U16  wRdIdx;     ///< read index
  U16  wCount;     ///< buffer count
} BUFCTL, *PBUFCTL;
#define BUFCTL_SIZE sizeof( BUFCTL )

/// define the local control structure
typedef struct _LCLCTL
{
  BUFCTL  tRxBuf; ///< receive buffer control
  BUFCTL  tTxBuf; ///< transmit buffer control
} LCLCTL, *PLCLCTL;
#define LCLCTL_SIZE sizeof( LCLCTL )

/// define the physical to logical mapping structure
typedef struct _PHYLOGMAP
{
  UARTDEVENUM eDev;   ///< logical enumeration
} PHYLOGMAP, *PPHYLOGMAP;
#define PHYLOGMAP_SIZE  sizeof( PHYLOGMAP )

// global parameter declarations ----------------------------------------------

// local parameter declarations -----------------------------------------------
static  LCLCTL    atLclCtls[ UART_DEV_ENUM_MAX ];
static  PHYLOGMAP atMaps[ MAX_NUM_UARTS ];

// local function prototypes --------------------------------------------------
static  void  SetConfig( UARTDEVENUM eDev, U32 uDevBase, U32 uBaudRate, U32 uConfig );
static  void  Irq0Handler( void );
static  void  Irq1Handler( void );
static  void  Irq2Handler( void );
static  void  IrqCommonHandler( PHYDEV ePhy );
static  void  PrimeTransmit( PHYDEV ePhy, PUARTDEF ptDef, PLCLCTL ptCtl );

// constant parameter initializations -----------------------------------------

/******************************************************************************
 * @function Uart_Initialize
 *
 * @brief UART initialization
 *
 * This function will initialize a given device 
 *
 *****************************************************************************/
void Uart_Initialize( void )
{
  UARTERR     eError = UART_ERR_NONE;
  PUARTDEF    ptDef;
  PLCLCTL     ptCtl;
  U32         uConfig;
  UARTDEVENUM eDev;

  // check for a valid UART
  for ( eDev = 0; eDev < UART_DEV_ENUM_MAX; eDev++ )
  {
    // get pointer to the definition structure
    ptDef = ( PUARTDEF )&atUartDefs[ eDev ];
    
    // get pointer to the control
    ptCtl = &atLclCtls[ eDev ];
    
    // clear the receive/transmit buffers
    ptCtl->tRxBuf.wWrIdx = ptCtl->tRxBuf.wRdIdx = ptCtl->tRxBuf.wCount = 0;
    ptCtl->tTxBuf.wWrIdx = ptCtl->tTxBuf.wRdIdx = ptCtl->tTxBuf.wCount = 0;
    
    // build the config value
    uConfig = ptDef->eParity | ptDef->eWrdLength | ptDef->eStop;
    
    // initialize the UART/GPIO peripherals
    MAP_SysCtlPeripheralEnable( ptDef->uDevPeriph );
    MAP_SysCtlPeripheralEnable( ptDef->uGpioPeriph );
      
    // initialize the pins
    if ( CLASS_IS_TEMPEST || CLASS_IS_FIRESTORM )
    {
      MAP_GPIOPinConfigure( ptDef->uRxPinFunc	 );
      MAP_GPIOPinConfigure( ptDef->uTxPinFunc	 );
    }
    
    // initialize the normal GPIO pins as UARTS
    MAP_GPIOPinTypeUART( ptDef->uGpioBase, ( ptDef->nRxPin | ptDef->nTxPin ));
    
    // set the configuration
    SetConfig( eDev, ptDef->uDevBase, ptDef->uBaudRate, uConfig );
  }
}

/******************************************************************************
 * @function Uart_Write
 *
 * @brief write some characters to the UART
 *
 * This function will write some characters to the UART buffer if room
 *
 * @param[in]   eDev        Device enumeration
 * @param[in]   pnData      pointer to the data buffer
 * @param[in]   wLength     size of the data buffer
 * @param[i0]   pwBytesWritten  the storage to return the number of bytes written
 *
 * @return      appropriate error value
 *
 *****************************************************************************/
UARTERR Uart_Write( UARTDEVENUM eDev, PU8 pnData, U16 wLength, PU16 pwBytesWritten )
{
  UARTERR   eError = UART_ERR_NONE;
  PUARTDEF  ptDef;
  PLCLCTL   ptCtl;
  U16       wBytesRemaining, wIdx;
  PHYDEV    ePhy;
 
  // check for a valid UART
  if ( eDev < UART_DEV_ENUM_MAX )
  {
    // get pointers to the control/def structure
    ptDef = ( PUARTDEF )&atUartDefs[ eDev ];
    ptCtl = &atLclCtls[ eDev ];
    
    // compute the bytes written
    wBytesRemaining = ptDef->wTxBufSize - ptCtl->tTxBuf.wCount;
    *pwBytesWritten = MIN( wBytesRemaining, wLength );
    
    // for each byte
    Interrupt_Disable( );
    for ( wIdx = 0; wIdx < *pwBytesWritten; wIdx++ )
    {
      // put in buffer/adjust pointer/increment count
      *( ptDef->pnTxBuffer + ptCtl->tTxBuf.wWrIdx++ ) = *( pnData + wIdx );
      ptCtl->tTxBuf.wWrIdx %= ptDef->wTxBufSize;
      ptCtl->tTxBuf.wCount++;
    }
    Interrupt_Enable( );
    
    // set the appropriate interrupt handler/IRQ
    switch( ptDef->uDevBase )
    {
      case UART0_BASE :
        ePhy = PHY_UART_DEV0;
        break;
        
      case UART1_BASE :
        ePhy = PHY_UART_DEV1;
        break;
        
      case UART2_BASE :
        ePhy = PHY_UART_DEV2;
        break;
        
      default :
        break;
    }

    // prime the transmitter
    PrimeTransmit( ePhy, ptDef, ptCtl );
    MAP_UARTIntEnable( ptDef->uDevBase, UART_INT_TX );
  }
  else
  {
    // illegal device
    eError = UART_ERR_ILLDEV;
  }
  
  // return the error
  return( eError );
}

/******************************************************************************
 * @function Uart_Read
 *
 * @brief read some characters from the UART
 *
 * This function will read bytes from the buffer
 *
 * @param[in]   eDev        Device enumeration
 * @param[in]   pnData      pointer to the data buffer
 * @param[in]   wLength     size of the data buffer
 * @param[i0]   pwBytesRead the storage to return the number of bytes written
 *
 * @return      appropriate error value
 *
 *****************************************************************************/
UARTERR Uart_Read( UARTDEVENUM eDev, PU8 pnData, U16 wLength, PU16 pwBytesRead )
{
  UARTERR   eError = UART_ERR_NONE;
  PUARTDEF  ptDef;
  PLCLCTL   ptCtl;
  U16       wBytesAvailable, wIdx;
  
  // check for a valid UART
  if ( eDev < UART_DEV_ENUM_MAX )
  {
    // get pointers to the control/def structure
    ptDef = ( PUARTDEF )&atUartDefs[ eDev ];
    ptCtl = &atLclCtls[ eDev ];
    
    // compute the bytes written
    wBytesAvailable = ptDef->wRxBufSize - ptCtl->tRxBuf.wCount;
    *pwBytesRead = MIN( wBytesAvailable, wLength );
    
    // for each byte
    Interrupt_Disable( );
    for ( wIdx = 0; wIdx < *pwBytesRead; wIdx++ )
    {
      // put in buffer/adjust pointer/increment count
       *( pnData + wIdx ) = *( ptDef->pnRxBuffer + ptCtl->tRxBuf.wRdIdx++ );
      ptCtl->tRxBuf.wRdIdx %= ptDef->wRxBufSize;
      ptCtl->tRxBuf.wCount--;
    }
    Interrupt_Enable( );
  }
  else
  {
    // illegal device
    eError = UART_ERR_ILLDEV;
  }
  
  // return the error
  return( eError );
}

/******************************************************************************
 * @function Uart_Ioctl
 *
 * @brief UART IOCTL functions
 *
 * This function provides functionality to modify the UARTs parameters
 *
 * @param[in]   eDev        Device enumeration
 * @param[in]   eAction     action to take
 * @param[io]   pvData      pointer to data storage/retrieval
 *
 * @return      appropriate error value
 *
 *****************************************************************************/
UARTERR Uart_Ioctl( UARTDEVENUM eDev, UARTACTION eAction, PVOID pvData )
{
  UARTERR   eError = UART_ERR_NONE;
  PUARTDEF  ptDef;
  
  // check for a valid UART
  if ( eDev < UART_DEV_ENUM_MAX )
  {
    // process the action
    switch( eAction )
    {
      case UART_ACTION_SET_CONFIG :
        // point to the config
        ptDef = ( PUARTDEF )pvData;

        // now configure the UART

        break;

      case UART_ACTION_SET_BAUDRATE :
        break;

      case UART_ACTION_GET_BAUDRATE :
        break;

      case UART_ACTION_SET_PARITY :
        break;

      case UART_ACTION_GET_PARITY :
        break;

      case UART_ACTION_SET_WRDLEN :
        break;

      case UART_ACTION_GET_WRDLEN :
        break;

      case UART_ACTION_SET_STOPBITS :
        break;

      case UART_ACTION_GET_STOPBITS :
        break;

      default :
        eError = UART_ERR_ILLACT;
        break;
    }
  }
  else
  {
    // illegal device
    eError = UART_ERR_ILLDEV;
  }
  
  // return the error
  return( eError );
}

/******************************************************************************
 * @function Uart_Close
 *
 * @brief Close the UART
 *
 * This function will disable any interrupt, unregister the interrupt handler
 *
 * @param[in]   eDev        Device enumeration
 *
 * @return      appropriate error value
 *
 *****************************************************************************/
UARTERR Uart_Close( UARTDEVENUM eDev )
{
  UARTERR eError = UART_ERR_NONE;
  
  // check for a valid UART
  if ( eDev < UART_DEV_ENUM_MAX )
  {
    // disable the USART/free the memory
    MAP_UARTIntDisable( atUartDefs[ eDev ].uDevBase, 0xFFFFFFFF );
    MAP_UARTDisable( atUartDefs[ eDev ].uDevBase );
  }
  else
  {
    // illegal device
    eError = UART_ERR_ILLDEV;
  }
  
  // return the error
  return( eError );
}

/******************************************************************************
 * @function SetConfig
 *
 * @brief common configuration routines
 *
 * This function sets the clock, ocnfiguration and fifo levels as well as the
 * the interrupts
 *
 * @param[in]   eDev        Device enumeration
 * @param[in]   uDevBase    device base addres
 * @param[in]   uBaudRate   baud rate of the device
 * @param[in]   uConfig     the configuration value (parity, length, stop bits)
 *
 *****************************************************************************/
static void SetConfig( UARTDEVENUM eDev, U32 uDevBase, U32 uBaudRate, U32 uConfig )
{
  PVOID pvIrqHandler;
  U32   uInterrupt;
  
  // disable the USART
  MAP_UARTIntDisable( uDevBase, 0xFFFFFFFF );
  MAP_UARTDisable( uDevBase );

  // initialize
  MAP_UARTConfigSetExpClk( uDevBase, MAP_SysCtlClockGet( ), uBaudRate, uConfig );
  
  // set the fifo level
  MAP_UARTFIFOLevelSet( uDevBase, UART_FIFO_TX7_8, UART_FIFO_RX1_8 );
  
  // set the appropriate interrupt handler/IRQ
  switch( uDevBase )
  {
    case UART0_BASE :
      uInterrupt = INT_UART0;
      pvIrqHandler = Irq0Handler;
      atMaps[ PHY_UART_DEV0 ].eDev = eDev;
      break;
      
    case UART1_BASE :
      uInterrupt = INT_UART1;
      pvIrqHandler = Irq1Handler;
      atMaps[ PHY_UART_DEV1 ].eDev = eDev;
      break;
      
    case UART2_BASE :
      uInterrupt = INT_UART2;
      pvIrqHandler = Irq2Handler;
      atMaps[ PHY_UART_DEV2 ].eDev = eDev;
      break;
      
    default :
      break;
  }
  
  // register the interrupt/enable it
  UARTIntRegister( uDevBase, pvIrqHandler );
  MAP_IntEnable( uInterrupt );
  MAP_UARTIntEnable( uDevBase, UART_INT_RX | UART_INT_RT | UART_INT_BE );
}

/******************************************************************************
 * @function Irq0Handler
 *
 * @brief interrupt device 0 handler
 *
 * This function handles the interrupts from UART 0
 *
 *****************************************************************************/
static void Irq0Handler( void )
{
  // call the common handler
  IrqCommonHandler( PHY_UART_DEV0 );
}

/******************************************************************************
 * @function Irq1Handler
 *
 * @brief interrupt device 1 handler
 *
 * This function handles the interrupts from UART 1
 *
 *****************************************************************************/
static void Irq1Handler( void )
{
  // call the common handler
  IrqCommonHandler( PHY_UART_DEV1 );
}

/******************************************************************************
 * @function Irq2Handler
 *
 * @brief interrupt device 2 handler
 *
 * This function handles the interrupts from UART 2
 *
 *****************************************************************************/
static void Irq2Handler( void )
{
  // call the common handler
  IrqCommonHandler( PHY_UART_DEV2 );
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
  U32	              uInts;
  U32	              uChar;
  PUARTDEF          ptDef;
  PLCLCTL           ptCtl;
  PVUARTINTCALLBACK pvCallBack;
  UARTDEVENUM       eDev;
  
  // get the logical device
  eDev = atMaps[ ePhy ].eDev;
  
  // get pointers to the control/def structure
  ptDef = ( PUARTDEF )&atUartDefs[ eDev ];
  ptCtl = &atLclCtls[ eDev ];
 
  // get/clear the interrupt source
  uInts = MAP_UARTIntStatus( ptDef->uDevBase, TRUE );
  MAP_UARTIntClear( ptDef->uDevBase, uInts );
  
  // get the callback
  pvCallBack = ptDef->pvCallBack;
  
  // was this a RX interrupt
  if ( uInts & ( UART_INT_RX | UART_INT_RT ))
  {
    // get all the available character from the uART
    while( MAP_UARTCharsAvail( ptDef->uDevBase ))
    {
      // get the character/add to queue
      uChar = MAP_UARTCharGetNonBlocking( ptDef->uDevBase );
      
      // if room
      if ( ptCtl->tRxBuf.wCount < ptDef->wRxBufSize )
      {
        // stuff in buffer/adust index/increment count
        *( ptDef->pnRxBuffer + ptCtl->tRxBuf.wWrIdx++ ) = ( U8 )uChar;
        ptCtl->tRxBuf.wWrIdx %= ptDef->wRxBufSize;
        ptCtl->tRxBuf.wCount++;
      }

      // check for callback
      if ( pvCallBack != NULL )
      {
        // call the callback with a receive char event
        pvCallBack( UART_IRQ_EVENT_RXCHAR, ( U8 )uChar );
      }
    }
  }

  // was this a TX interrupt
  if ( uInts & UART_INT_TX )
  {
    // prime the transmitter
    PrimeTransmit( ePhy, ptDef, ptCtl );

    // if queue is empty, turn off interrupt
    if ( ptCtl->tTxBuf.wCount == 0 )
    {
      // turn off finterrupt
      MAP_UARTIntDisable( ptDef->uDevBase, UART_INT_TX );
    }
    
    // check for callback
    if ( pvCallBack != NULL )
    {
      // call back with the appropriate TX event
      pvCallBack(( ptCtl->tTxBuf.wCount == 0 ) ? UART_IRQ_EVENT_TXCMP : UART_IRQ_EVENT_TXEMP, 0 );
    }
  }

  // was this a break detect event
  if ( uInts & UART_INT_BE )
  {
    // check for callback
    if ( pvCallBack != NULL )
    {
      // call the callback with the breadk detect event
      pvCallBack( UART_IRQ_EVENT_BRKDET, 0 );
    }
  }
}

/******************************************************************************
 * @function PrimeTransmit
 *
 * @brief transmit buffer prime handler
 *
 * This function sets values into the transmit buffer and if required
 *
 * @param[in]   ePhy        physical device enumeration
 * @param[in]   ptDef       pointer to the logical device definition structure
 * @param[in]   ptCtl       pointer to the logical device control structure
 *
 *****************************************************************************/
static void PrimeTransmit( PHYDEV ePhy, PUARTDEF ptDef, PLCLCTL ptCtl )
{
  U8	nChar;
  U32 uInterrupt;
  
  // is the buffer empty
  if ( ptCtl->tTxBuf.wCount != 0 )
  {
    // get the interrupt
    switch( ePhy )
    {
      case PHY_UART_DEV0 :
        uInterrupt = INT_UART0;
        break;
        
      case PHY_UART_DEV1 :
        uInterrupt = INT_UART1;
        break;
        
      case PHY_UART_DEV2 :
        uInterrupt = INT_UART2;
        break;
        
      default :
        break;
    }
    
    // disable the UART interrupt
    MAP_IntDisable( uInterrupt );
    
    // while space available and not empty
    while( MAP_UARTSpaceAvail( ptDef->uDevBase ) && ( ptCtl->tTxBuf.wCount != 0 ))
    {
      // output it
      nChar = *( ptDef->pnTxBuffer + ptCtl->tTxBuf.wRdIdx++ );
      ptCtl->tTxBuf.wRdIdx %= ptDef->wTxBufSize;
      ptCtl->tTxBuf.wCount--;
      MAP_UARTCharPutNonBlocking( ptDef->uDevBase, nChar );
    }
    
    // renable the UART interrupt
    MAP_IntEnable( uInterrupt );
  }
}

/**@} EOF Uart.c */
