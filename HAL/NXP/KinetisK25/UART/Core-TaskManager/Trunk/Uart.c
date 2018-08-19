/******************************************************************************
 * @file Uart.c
 *
 * @brief UART implementation
 *
 * This file provides the implemntation for the UART peripherals
 *
 * @copyright Copyright (c) 2012 CyberIntegration
 * This document contains proprietary data and information of CyberIntegration
 * LLC. It is the exclusive property of CyberIntegration, LLC and will not be
 * disclosed in any form to any party without prior written permission of
 * CyberIntegration, LLC. This document may not be reproduced or further used
 * without the prior written permission of CyberIntegration, LLC.
 *
 * Version History
 * ======
 * $Log: $
 * 
 *
 * \addtogroup UART
 * @{
 *****************************************************************************/

// local includes -------------------------------------------------------------
#include "UART/Uart.h"

// library includes -----------------------------------------------------------
#include "Clock/Clock.h"
#include "Interrupt/Interrupt.h"

// structures -----------------------------------------------------------------
/// define the buffer control structure
typedef struct _BUFCTL
{
  U16         wWrIdx;     ///< write index
  U16         wRdIdx;     ///< read index
  U16         wCount;     ///< buffer count
} BUFCTL, *PBUFCTL;
#define BUFCTL_SIZE         sizeof( BUFCTL )

/// define the local control structure
typedef struct _LCLCTL
{
  BUFCTL      tRxBuf;       ///< receive buffer control
  BUFCTL      tTxBuf;       ///< transmit buffer control
} LCLCTL, *PLCLCTL;
#define LCLCTL_SIZE         sizeof( LCLCTL )

/// define the UART pointer union
typedef union _UARTPTR
{
  UART0_Type* ptUart0;      ///< UART0 type pointer
  UART_Type*  ptUartx;      ///< UARTx type poiniter
} UARTPTR, *PUARTPTR;
#define UARTPTR_SIZE      sizeof( UARTPTR )

// local parameter declarations -----------------------------------------------
static  UARTDEVENUM aePhyToDefMap[ UART_CHAN_MAX ];
static  LCLCTL      atLclCtls[ UART_DEV_ENUM_MAX ];
static  UARTPTR     tUartPtr;

// local function prototypes --------------------------------------------------
static  void    ConfigureUart0( PUARTDEF ptDef );
static  void    ConfigureUartx( PUARTDEF ptDef );
static  void    GetUartPointer( UARTCHAN eChan );
static  U16     ComputeBaudrate( U32 uBaudrate );
static  void    IrqCommonHandler( UARTDEVENUM eDev );
static  void    IrqUart0Handler( PUARTDEF ptDef, PLCLCTL ptCtl );
static  void    IrqUartxHandler( PUARTDEF ptDef, PLCLCTL ptCtl );

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
  PUARTDEF                    ptDef;
  PLCLCTL                     ptCtl;
  UARTDEVENUM                 eDev;
  
  // for each defined UART
  for ( eDev = 0; eDev < UART_DEV_ENUM_MAX; eDev++ )
  {
    // get pointer to the definition/control structures
    ptDef = ( PUARTDEF )&atUartDefs[ eDev ];
    ptCtl = &atLclCtls[ eDev ];

    // clear the receive/transmit buffers
    ptCtl->tRxBuf.wWrIdx = ptCtl->tRxBuf.wRdIdx = ptCtl->tRxBuf.wCount = 0;
    ptCtl->tTxBuf.wWrIdx = ptCtl->tTxBuf.wRdIdx = ptCtl->tTxBuf.wCount = 0;
    
    // get the uart
    GetUartPointer( ptDef->eChan );
    
    // map physical to definition
    aePhyToDefMap[ ptDef->eChan ] = eDev;
    
    // process accordingly
    switch( ptDef->eChan )
    {
      case UART_CHAN_0 :
        ConfigureUart0( ptDef );
        break;
        
      case UART_CHAN_1 :
      case UART_CHAN_2 :
        ConfigureUartx( ptDef );
        break;
        
      default :
        break;
    }
  }
}

/******************************************************************************
 * @function Uart_CloseAll
 *
 * @brief UART close all devices
 *
 * This function will close all defined devices
 *
 *****************************************************************************/
void Uart_CloseAll( void )
{
  PUARTDEF                    ptDef;
  UARTDEVENUM                 eDev;
  
  // for each defined UART
  for ( eDev = 0; eDev < UART_DEV_ENUM_MAX; eDev++ )
  {
    // get pointer to the definition/control structures
    ptDef = ( PUARTDEF )&atUartDefs[ eDev ];

    // get the uart
    GetUartPointer( ptDef->eChan );

    // enable the interrupt in the NVIC
    switch( ptDef->eChan )
      {
        case UART_CHAN_0 :
          break;

        case UART_CHAN_1 :
          break;

        case UART_CHAN_2 :
          break;

        default :
          break;
      }

    
    // turn of the UART
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
  UARTERR       eError = UART_ERR_NONE;
  PUARTDEF      ptDef;
  PLCLCTL       ptCtl;
  U16           wBytesRemaining, wIdx;
 
  // check for a valid UART
  if ( eDev < UART_DEV_ENUM_MAX )
  {
    // set the bytes written to zero
    *pwBytesWritten = 0;

    // get pointers to the control/def structure
    ptDef = ( PUARTDEF )&atUartDefs[ eDev ];
    ptCtl = &atLclCtls[ eDev ];
    
    // get the uart
    GetUartPointer( ptDef->eChan );

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

    // enable the transmitter 
    if ( ptDef->pvCallback != NULL )
    {
      // call it
      ptDef->pvCallback( UART_IRQ_EVENT_TXENB, 0, ptDef->eXmtCtlPin );
    }

    // enable the data register enmpty interrupt
    switch( ptDef->eChan )
    {
      case UART_CHAN_0 :
        tUartPtr.ptUart0->C2 |= ( UART0_C2_TCIE_MASK | UART0_C2_TIE_MASK );
        break;
        
      case UART_CHAN_1 :
      case UART_CHAN_2 :
        tUartPtr.ptUartx->C2 |= ( UART_C2_TCIE_MASK | UART_C2_TIE_MASK );
        break;
        
      default :
        break;
    }
   
    // enable the interrupt
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
  UARTERR eError = UART_ERR_NONE;
  
  // check for a valid UART
  if ( eDev < UART_DEV_ENUM_MAX )
  {
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
    // get the uart

    // disable the USART
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
 * @function UART0_Handler
 *
 * @brief interrupt UART0 handler
 *
 * This function handles the interrupts from UART0
 *
 *****************************************************************************/
void UART0_Handler( void )
{
  // call the common handler
  IrqCommonHandler( aePhyToDefMap[ UART_CHAN_0 ] );
}

/******************************************************************************
 * @function UART1_Handler
 *
 * @brief interrupt UART1 handler
 *
 * This function handles the interrupts from UART1
 *
 *****************************************************************************/
void UART1_Handler( void )
{
  // call the common handler
  IrqCommonHandler( aePhyToDefMap[ UART_CHAN_1 ] );
}

/******************************************************************************
 * @function UART2_Handler
 *
 * @brief interrupt UART2 handler
 *
 * This function handles the interrupts from UART2
 *
 *****************************************************************************/
void UART2_Handler( void )
{
  // call the common handler
  IrqCommonHandler( aePhyToDefMap[ UART_CHAN_2 ] );
}

/******************************************************************************
 * @function IrqCommandHandler
 *
 * @brief common interrupt handler
 *
 * This function checks for the type of interrupts and processes them
 * appropriately
 *
 * @param[in]   eDev        Device enumeration
 *
 *****************************************************************************/
static void IrqCommonHandler( UARTDEVENUM eDev )
{
  PUARTDEF            ptDef;
  PLCLCTL             ptCtl;
  PVUARTINTCALLBACK  pvCallBack;

  // get pointer to the definition/control structures
  ptDef = ( PUARTDEF )&atUartDefs[ eDev ];
  ptCtl = &atLclCtls[ eDev ];
  
  // get the uart
  GetUartPointer( ptDef->eChan );
    
  // get the callback
  pvCallBack = ptDef->pvCallback;
  
  // determine 
  switch( ptDef->eChan )
  {
    case UART_CHAN_0 :
      IrqUart0Handler( ptDef, ptCtl );
      break;
      
    case UART_CHAN_1 :
    case UART_CHAN_2 :
      IrqUartxHandler( ptDef, ptCtl );
      break;
      
    default :
      break;
  }
}

/******************************************************************************
 * @function IrqUart0Handler
 *
 * @brief UART0 interrupt handler
 *
 * This function processes the IRQ handler for UART0
 *
 * @param[in]   ptDef     pointer to the definition
 * @param[in]   ptCtl     pointer to the control structure
 *
 *****************************************************************************/
static void IrqUart0Handler( PUARTDEF ptDef, PLCLCTL ptCtl )
{
  U8                  nOption, nStatus;
  PVUARTINTCALLBACK   pvCallBack;
  UARTIRQEVENTS       eEvent = UART_IRQ_EVENT_NONE;
  
  // get the callback
  pvCallBack = ptDef->pvCallback;
  
  // get the status
  nStatus = tUartPtr.ptUart0->S1;
  
  // was this a RX interrupt
  if ( nStatus & UART0_S1_RDRF_MASK )
  {
    // get the data
    nOption = tUartPtr.ptUart0->D;

    // check for an error
    if (( nStatus & UART0_S1_PF_MASK ) || ( nStatus & UART0_S1_FE_MASK ) || ( nStatus & UART0_S1_OR_MASK ))
    {
      // clear the error flag
      tUartPtr.ptUart0->S1 |= ( UART0_S1_PF_MASK | UART0_S1_FE_MASK | UART0_S1_OR_MASK );
      
      // if callback process it
      if (( pvCallBack != NULL ) & ( ptDef->tEventFlags.bRxErr ))
      {
        // call the callback with a receive char event
        eEvent = UART_IRQ_EVENT_RXERR;
        pvCallBack( eEvent, nOption, 0 );
      }
    }
    else
    {
      // check for callback
      if ( ptDef->tEventFlags.bRxChar )
      {
        // if callback process it
        if (( pvCallBack != NULL ) & ( ptDef->tEventFlags.bRxChar ))
        {
          // call the callback with a receive char event
          eEvent = UART_IRQ_EVENT_RXCHAR;
          pvCallBack( eEvent, nOption, 0 );
        }
        // check for room in local buffer
        else if ( ptCtl->tRxBuf.wCount < ptDef->wRxBufSize )
        {
          // stuff in buffer/adust index/increment count
          *( ptDef->pnRxBuffer + ptCtl->tRxBuf.wWrIdx++ ) = nOption;
          ptCtl->tRxBuf.wWrIdx %= ptDef->wRxBufSize;
          ptCtl->tRxBuf.wCount++;
        }
      }
    }
  }
  
  // check for a data register empty interrupt
  if ( nStatus & UART0_S1_TDRE_MASK )
  {
    // check for characters available
    if ( ptCtl->tTxBuf.wCount != 0 )
    {
      // output the character
      nOption = *( ptDef->pnTxBuffer + ptCtl->tTxBuf.wRdIdx++ );
      tUartPtr.ptUart0->D = nOption;
      ptCtl->tTxBuf.wRdIdx %= ptDef->wTxBufSize;
      ptCtl->tTxBuf.wCount--;
        
      // check for callback
      if (( pvCallBack != NULL ) && ( ptDef->tEventFlags.bTxEmp ))
      {
        // call back with the appropriate TX event
        eEvent = UART_IRQ_EVENT_TXEMP;
        nOption = 0;
        pvCallBack( eEvent, nOption, 0 );
      }
    }
    else
    {
      // turn off the data register empty interrupt
      tUartPtr.ptUart0->C2 &= ~( UART0_C2_TIE_MASK );
    }
  }  
    
  // check for a transmit complete interrupt
  if ( nStatus & UART0_S1_TC_MASK )
  {
    // if data is done, perform the callback
    if ( ptCtl->tTxBuf.wCount == 0 )
    {
      if (( pvCallBack  != NULL ) && ( ptDef->tEventFlags.bTxCmp ))
      {
        // call back with the appropriate TX event
        eEvent = UART_IRQ_EVENT_TXCMP;
        nOption = 0;
        pvCallBack( eEvent, nOption, ptDef->eXmtCtlPin );
      }
    }
    
    // turn off the data register empty interrupt
    tUartPtr.ptUart0->C2 &= ~( UART0_C2_TCIE_MASK );
  }
}

/******************************************************************************
 * @function IrqUartxHandler
 *
 * @brief UARTX interrupt handler
 *
 * This function processes the IRQ handler for UARTX
 *
 * @param[in]   ptDef     pointer to the definition
 * @param[in]   ptCtl     pointer to the control structure
 *
 *****************************************************************************/
static void IrqUart1Handler( PUARTDEF ptDef, PLCLCTL ptCtl )
{
  U8                  nOption, nStatus;
  PVUARTINTCALLBACK   pvCallBack;
  UARTIRQEVENTS       eEvent = UART_IRQ_EVENT_NONE;
  
  // get the callback
  pvCallBack = ptDef->pvCallback;
  
  // get the status
  nStatus = tUartPtr.ptUartx->S1;
  
  // was this a RX interrupt
  if ( nStatus & UART_S1_RDRF_MASK )
  {
    // get the data
    nOption = tUartPtr.ptUartx->D;

    // check for an error
    if (( nStatus & UART_S1_PF_MASK ) || ( nStatus & UART_S1_FE_MASK ) || ( nStatus & UART_S1_OR_MASK ))
    {
      // clear the error flag
      // tUartPtr.ptUartx->S1 |= ( UART_S1_PF_MASK | UART_S1_FE_MASK | UART_S1_OR_MASK );
      
      // if callback process it
      if (( pvCallBack != NULL ) & ( ptDef->tEventFlags.bRxErr ))
      {
        // call the callback with a receive char event
        eEvent = UART_IRQ_EVENT_RXERR;
        pvCallBack( eEvent, nOption, 0 );
      }
    }
    else
    {
      // check for callback
      if ( ptDef->tEventFlags.bRxChar )
      {
        // if callback process it
        if (( pvCallBack != NULL ) & ( ptDef->tEventFlags.bRxChar ))
        {
          // call the callback with a receive char event
          eEvent = UART_IRQ_EVENT_RXCHAR;
          pvCallBack( eEvent, nOption, 0 );
        }
        // check for room in local buffer
        else if ( ptCtl->tRxBuf.wCount < ptDef->wRxBufSize )
        {
          // stuff in buffer/adust index/increment count
          *( ptDef->pnRxBuffer + ptCtl->tRxBuf.wWrIdx++ ) = nOption;
          ptCtl->tRxBuf.wWrIdx %= ptDef->wRxBufSize;
          ptCtl->tRxBuf.wCount++;
        }
      }
    }
  }
  
  // check for a data register empty interrupt
  if ( nStatus & UART_S1_TDRE_MASK )
  {
    // check for characters available
    if ( ptCtl->tTxBuf.wCount != 0 )
    {
      // output the character
      nOption = *( ptDef->pnTxBuffer + ptCtl->tTxBuf.wRdIdx++ );
      tUartPtr.ptUartx->D = nOption;
      ptCtl->tTxBuf.wRdIdx %= ptDef->wTxBufSize;
      ptCtl->tTxBuf.wCount--;
        
      // check for callback
      if (( pvCallBack != NULL ) && ( ptDef->tEventFlags.bTxEmp ))
      {
        // call back with the appropriate TX event
        eEvent = UART_IRQ_EVENT_TXEMP;
        nOption = 0;
        pvCallBack( eEvent, nOption, 0 );
      }
    }
    else
    {
      // turn off the data register empty interrupt
      tUartPtr.ptUartx->C2 &= ~( UART_C2_TIE_MASK );
    }
  }  
    
  // check for a transmit complete interrupt
  if ( nStatus & UART_S1_TC_MASK )
  {
    // if data is done, perform the callback
    if ( ptCtl->tTxBuf.wCount == 0 )
    {
      if (( pvCallBack  != NULL ) && ( ptDef->tEventFlags.bTxCmp ))
      {
        // call back with the appropriate TX event
        eEvent = UART_IRQ_EVENT_TXCMP;
        nOption = 0;
        pvCallBack( eEvent, nOption, ptDef->eXmtCtlPin );
      }
    }
    
    // turn off the data register empty interrupt
    tUartPtr.ptUartx->C2 &= ~( UART_C2_TCIE_MASK );
  }
}

/******************************************************************************
 * @function CongifureUart0
 *
 * @brief configure UART 0
 *
 * This function will configure the UART0
 *
 * @param[in]   ptDef     pointer to the definition structure
 *
 *****************************************************************************/
static void ConfigureUart0( PUARTDEF ptDef )
{
  U16UN tBaudRate;

  // calculate the baud rate
  tBaudRate.wValue = ComputeBaudrate( ptDef->uBaudRate );
  
  // now set the baudrate/stop bit
  tBaudRate.anValue[ LE_U16_MSB_IDX ] &= UART0_BDH_SBR_MASK;
  tBaudRate.anValue[ LE_U16_MSB_IDX ] |= BITS( ptDef->eStopBit, UART0_BDH_SBNS_SHIFT );
  tUartPtr.ptUart0->BDH = tBaudRate.anValue[ LE_U16_MSB_IDX ];
  tUartPtr.ptUart0->BDL = tBaudRate.anValue[ LE_U16_LSB_IDX ];
  
  // set up the control registers
  tUartPtr.ptUart0->C5 = 0;
  tUartPtr.ptUart0->C4 = 16;
  tUartPtr.ptUart0->C3 = 0;
  tUartPtr.ptUart0->C1 = (( ptDef->eParity != UART_PARITY_NONE ) ? UART_C1_PE_MASK : 0 ) | (( ptDef->eParity == UART_PARITY_ODD ) ? UART_C1_PT_MASK : 0 );
  tUartPtr.ptUart0->C2 = UART_C2_RE_MASK | UART_C2_TE_MASK | UART_C2_RIE_MASK;
  
  // enable the interrupt
  NVIC_EnableIRQ( UART0_IRQn );
}

/******************************************************************************
 * @function COnfigureUartx
 *
 * @brief confiure UART 1/2
 *
 * This function will configure the UART1/UART2
 *
 * @param[in]   ptDef     pointer to the definition structure
 *
 *****************************************************************************/
static void ConfigureUartx( PUARTDEF ptDef )
{
  U16UN tBaudRate;

  // calculate the baud rate
  tBaudRate.wValue = ComputeBaudrate( ptDef->uBaudRate );
  
  // now set the baudrate/stop bit
  tBaudRate.anValue[ LE_U16_MSB_IDX ] &= UART0_BDH_SBR_MASK;
  tBaudRate.anValue[ LE_U16_MSB_IDX ] |= BITS( ptDef->eStopBit, UART0_BDH_SBNS_SHIFT );
  tUartPtr.ptUartx->BDH = tBaudRate.anValue[ LE_U16_MSB_IDX ];
  tUartPtr.ptUartx->BDL = tBaudRate.anValue[ LE_U16_LSB_IDX ];
  
  // set up the control registers
  tUartPtr.ptUartx->C4 = 16;
  tUartPtr.ptUartx->C3 = 0;
  tUartPtr.ptUartx->C1 = (( ptDef->eParity != UART_PARITY_NONE ) ? UART_C1_PE_MASK : 0 ) | (( ptDef->eParity == UART_PARITY_ODD ) ? UART_C1_PT_MASK : 0 );
  tUartPtr.ptUartx->C2 = UART_C2_RE_MASK | UART_C2_TE_MASK | UART_C2_RIE_MASK;
  
  // enable the interrupt
  NVIC_EnableIRQ(( ptDef->eChan == UART_CHAN_1 ) ? UART1_IRQn : UART2_IRQn );
}

/******************************************************************************
 * @function GetUartPointer
 *
 * @brief gets a pointer to the control structure
 *
 * This function will return a pointer to the correct control structure
 * for a given channel
 *
 * @param[in]   eChan			SERCOM channel
 *
 *****************************************************************************/
static void GetUartPointer( UARTCHAN eChan )
{
  UART_Type*  aptUarts[ 2 ] = UART_BASES;

  switch( eChan )
  {
    case UART_CHAN_0 :
      tUartPtr.ptUart0 = UART0;
      break;
      
    case UART_CHAN_1 :
      tUartPtr.ptUartx = aptUarts[ 0 ];
      break;
    
    case UART_CHAN_2 :
      tUartPtr.ptUartx = aptUarts[ 1 ];
      break;
    
    default :
      break;
  }
}

/******************************************************************************
 * @function ComputeBaudrate
 *
 * @brief computes the step and divisor for a given baudrate
 *
 * This function will determine the best step and divisor for a given
 * baud rate
 *
 * @param[in]   uBaudrate   desired baudrate
 *
 * @return      baud rate value, 0 if not available
 *
 *****************************************************************************/
static U16 ComputeBaudrate( U32 uBaudRate )
{
  U32   uPeripheralClock;
  U16   wBaudRate = 0;
  
  // get the system clock value
  uPeripheralClock = Clock_GetFreq( );
  
  wBaudRate = uPeripheralClock / ( 16 * uBaudRate );
  
  // return the baud rate
  return( wBaudRate );
}

/**@} EOF Uart.c */
