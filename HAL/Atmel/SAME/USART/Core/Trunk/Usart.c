/******************************************************************************
 * @file Usart.c
 *
 * @brief USART implementation
 *
 * This file provides the implemntation for the USART peripherals
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
 * \addtogroup USART
 * @{
 *****************************************************************************/

// local includes -------------------------------------------------------------
#include "USART/Usart.h"

// library includes -----------------------------------------------------------
#include "Clock/Clock.h"
#include "Interrupt/Interrupt.h"
#if ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_FREERTOS )
#include "FreeRtos/FreeRTOS.h"
#endif // SYSTEMDEFINE_OS_SELECTION

// structures -----------------------------------------------------------------
/// define the buffer control structure
#if ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_TASKMANAGER )
typedef struct _BUFCTL
{
  U16 wWrIdx;     ///< write index
  U16 wRdIdx;     ///< read index
  U16 wCount;     ///< buffer count
} BUFCTL, *PBUFCTL;
#define BUFCTL_SIZE   sizeof( BUFCTL )
#endif // SYSTEMDEFINE_OS_SELECTION

/// define the local control structure
typedef struct _LCLCTL
{
  #if ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_TASKMANAGER )
  BUFCTL            tRxBuf;       ///< receive buffer control
  BUFCTL            tTxBuf;       ///< transmit buffer control
  #elif ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_FREERTOS )
  xQueueHandle      xRxQueue;     ///< receive queue
  xQueueHandle      xTxQueue;     ///< transmit queue
  #endif // SYSTEMDEFINE_OS_SELECTION
  Usart*            ptUsart;      ///< pointer to the usart
  U32               uInterrupt;   ///< interrupt
} LCLCTL, *PLCLCTL;
#define LCLCTL_SIZE   sizeof( LCLCTL )

// local parameter declarations -----------------------------------------------
static  USARTDEVENUM  aePhyToDefMap[ USART_CHAN_MAX ];
static  LCLCTL        atLclCtls[ USART_DEV_ENUM_MAX ];

// local function prototypes --------------------------------------------------
static  void  GetUsartPointer( USARTCHAN eChan, PLCLCTL ptCtl );
static  U16   ComputeBaudrate( U32 uBaudrate, USARTCLKSRC eClockSrc );
static  void  IrqCommonHandler( USARTDEVENUM eDev );

/******************************************************************************
 * @function Usart_Initialize
 *
 * @brief USART initialization
 *
 * This function will initialize a given device 
 *
 *****************************************************************************/
void Usart_Initialize( void )
{
  PUSARTDEF     ptDef;
  PLCLCTL       ptCtl;
  U16           wBaudRate;
  USARTDEVENUM  eDev;
  
  // for each defined USART
  for ( eDev = 0; eDev < USART_DEV_ENUM_MAX; eDev++ )
  {
    // get pointer to the definition/control structures
    ptDef = ( PUSARTDEF )&atUsartDefs[ eDev ];
    ptCtl = &atLclCtls[ eDev ];
    
    // clear the control
    memset( ptCtl, 0, LCLCTL_SIZE );

    #if ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_TASKMANAGER )
    // clear the receive/transmit buffers
    ptCtl->tRxBuf.wWrIdx = ptCtl->tRxBuf.wRdIdx = ptCtl->tRxBuf.wCount = 0;
    ptCtl->tTxBuf.wWrIdx = ptCtl->tTxBuf.wRdIdx = ptCtl->tTxBuf.wCount = 0;
    #endif // SYSTEMDEFINE_OS_SELECTION
    
    // get the uart
    GetUsartPointer( ptDef->eChan, ptCtl );
      
    // now compute the baud rate
    if (( wBaudRate = ComputeBaudrate( ptDef->uBaudRate, ptDef->eClkSource )) != 0 )
    {
      // valid baud rate
      ptCtl->ptUsart->US_BRGR = wBaudRate;
      
      // determine mode
      switch( ptDef->eMode )
      {
        case USART_MODE_NORMAL :
          // set the frame/parity/interrupts
          ptCtl->ptUsart->US_MR = US_MR_PAR( ptDef->eParity ) | US_MR_CHRL( ptDef->eWrdLen ) | US_MR_NBSTOP( ptDef->eStopBits ) | US_MR_CLKO;
          ptCtl->ptUsart->US_IER = ( US_IER_RXRDY );

          // if this free-rtos create the queues
          #if ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_FREERTOS )
          ptCtl->xRxQueue = xQueueCreate( sizeof( U8 ), ptDef->wRxBufSize );
          ptCtl->xTxQueue = xQueueCreate( sizeof( U8 ), ptDef->wTxBufSize );
          #endif // SYSTEMDEFINE_OS_SELECTION

          // enable the interrupt in the NVIC
          aePhyToDefMap[ ptDef->eChan ] = eDev;
          NVIC_EnableIRQ( ptCtl->uInterrupt );
          #if ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_FREERTOS )
          NVIC_SetPriority( ptCtl->uInterrupt, configMAX_SYSCALL_INTERRUPT_PRIORITY - 1 );
          #endif // SYSTEMDEFINE_OS_SELECTION
          break;
          
        case USART_MODE_SPI_MASTER :
          ptCtl->ptUsart->US_MR = (( ptDef->eClkPhase == USART_CLKPHA_TE ) ? US_MR_CPHA : 0 ) |
                                  (( ptDef->eClkPolarity == USART_CLKPOL_FE ) ? US_MR_CPOL : 0 ) |
                                  ( US_MR_CLKO );
          break;
      }
      
      // now enable the device
      ptCtl->ptUsart->US_CR = US_CR_RXEN | US_CR_TXEN | US_CR_RSTSTA;
    }
  }
}

/******************************************************************************
 * @function Usart_CloseAll
 *
 * @brief USART close all devices
 *
 * This function will close all defined devices
 *
 *****************************************************************************/
void Usart_CloseAll( void )
{
  PLCLCTL       ptCtl;
  USARTDEVENUM  eDev;
  
  // for each defined USART
  for ( eDev = 0; eDev < USART_DEV_ENUM_MAX; eDev++ )
  {
    // get pointer to the definition/control structures
    ptCtl = &atLclCtls[ eDev ];

    // enable the interrupt in the NVIC
    NVIC_DisableIRQ( ptCtl->uInterrupt );

    // delete the queues if neceesary
    #if ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_FREERTOS )
    if (( ptCtl->xRxQueue != NULL ) && ( ptCtl->xTxQueue != NULL ))
    {
      vQueueDelete( ptCtl->xRxQueue );
      vQueueDelete( ptCtl->xTxQueue );
    }    
    #endif // SYSTEMDEFINE_OS_SELECTION
    
    // turn of the USART
    ptCtl->ptUsart->US_CR = US_CR_RXDIS | US_CR_TXDIS;
  }
}

/******************************************************************************
 * @function Usart_Write
 *
 * @brief write some characters to the USART
 *
 * This function will write some characters to the USART buffer if room
 *
 * @param[in]   eDev        Device enumeration
 * @param[in]   pnData      pointer to the data buffer
 * @param[in]   wLength     size of the data buffer
 * @param[i0]   pwBytesWritten  the storage to return the number of bytes written
 *
 * @return      appropriate error value
 *
 *****************************************************************************/
USARTERR Usart_Write( USARTDEVENUM eDev, PU8 pnData, U16 wLength, PU16 pwBytesWritten )
{
  USARTERR            eError = USART_ERR_NONE;
  PUSARTDEF           ptDef;
  PLCLCTL             ptCtl;
  U16                 wBytesRemaining, wIdx;
  U8                  nData;
  PVUSARTINTCALLBACK  pvCallback;

  // check for a valid USART
  if ( eDev < USART_DEV_ENUM_MAX )
  {
    // set the bytes written to zero
    *pwBytesWritten = 0;

    // get pointers to the control/def structure
    ptDef = ( PUSARTDEF )&atUsartDefs[ eDev ];
    ptCtl = &atLclCtls[ eDev ];
    
    // determine mode
    switch( ptDef->eMode )
    {
      case USART_MODE_NORMAL :
        // disable interrupts
        Interrupt_Disable( );
    
        #if ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_TASKMANAGER )
        // compute the bytes written
        wBytesRemaining = ptDef->wTxBufSize - ptCtl->tTxBuf.wCount;
        *pwBytesWritten = MIN( wBytesRemaining, wLength );
    
        // for each byte
        for ( wIdx = 0; wIdx < *pwBytesWritten; wIdx++ )
        {
          // put in buffer/adjust pointer/increment count
          *( ptDef->pnTxBuffer + ptCtl->tTxBuf.wWrIdx++ ) = *( pnData + wIdx );
          ptCtl->tTxBuf.wWrIdx %= ptDef->wTxBufSize;
          ptCtl->tTxBuf.wCount++;
        }
        #elif ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_FREERTOS )
        // set the bytes written
        wBytesRemaining = uxQueueSpacesAvailable( ptCtl->xTxQueue );
        *pwBytesWritten = MIN( wBytesRemaining, wLength );
    
        // for each character, stuff in queue
        for ( wIdx = 0; wIdx < wLength; wIdx++ )
        {
          // shove in queue with no timeout
          xQueueSendToBack( ptCtl->xTxQueue, ( pnData + wIdx ), 0 );
        }
        #endif // SYSTEMDEFINE_OS_SELECTION

        // check for function handler/
        if ((( pvCallback = ptDef->pvCallback ) != NULL ) && ( ptDef->tEventFlags.bTxEnb ))
        {
          // call the callback to enable the transmit
          pvCallback( USART_IRQ_EVENT_TXENB, 0 );
        }

        // enable the data register enmpty interrupt
        ptCtl->ptUsart->US_IER = ( US_IER_TXRDY | US_IER_TXEMPTY );

        // enable the interrupt
        Interrupt_Enable( );
        break;
        
      case USART_MODE_SPI_MASTER :
        // set the number of bytes written
        *pwBytesWritten = wLength;
        
        // loop
        do
        {
          // output data
          ptCtl->ptUsart->US_THR = *( pnData++ );
          
          // wait till done
          while ( !( ptCtl->ptUsart->US_CSR & US_CSR_TXRDY ));
        }
        while( --wLength != 0 );
        break;
        
      default :
        break;
    }
  }
  else
  {
    // illegal device
    eError = USART_ERR_ILLDEV;
  }
  
  // return the error
  return( eError );
}

/******************************************************************************
 * @function Usart_Read
 *
 * @brief read some characters from the USART
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
USARTERR Usart_Read( USARTDEVENUM eDev, PU8 pnData, U16 wLength, PU16 pwBytesRead )
{
  USARTERR  eError = USART_ERR_NONE;
  PUSARTDEF ptDef;
  PLCLCTL   ptCtl;
  U16       wBytesAvailable, wIdx;
  
  // check for a valid USART
  if ( eDev < USART_DEV_ENUM_MAX )
  {
    // get pointers to the control/def structure
    ptDef = ( PUSARTDEF )&atUsartDefs[ eDev ];
    ptCtl = &atLclCtls[ eDev ];
    
    // determine mode
    switch( ptDef->eMode )
    {
      case USART_MODE_NORMAL :
        #if ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_TASKMANAGER )
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
        #elif ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_FREERTOS )
        *pwBytesRead = 1;

        // wait for a character from the queue
        xQueueReceive( ptCtl->xRxQueue, pnData, portMAX_DELAY );
        #endif // SYSTEMDEFINE_OS_SELECTION
        break;
        
      case USART_MODE_SPI_MASTER :
        // set the number of bytes read
        *pwBytesRead = wLength;
        
        // loop
        do
        {
          // output data
          ptCtl->ptUsart->US_THR = 0xFF;
          
          // wait till done
          while ( !( ptCtl->ptUsart->US_CSR & US_CSR_TXRDY ));
          
          // read the data
          *( pnData++ ) = ptCtl->ptUsart->US_RHR;
        }
        while( --wLength != 0 );
        break;
        
      default :
        break;
    }
  }
  else
  {
    // illegal device
    eError = USART_ERR_ILLDEV;
  }
  
  // return the error
  return( eError );
}

/******************************************************************************
 * @function Usart_Close
 *
 * @brief Close the USART
 *
 * This function will disable any interrupt, unregister the interrupt handler
 *
 * @param[in]   eDev        Device enumeration
 *
 * @return      appropriate error value
 *
 *****************************************************************************/
USARTERR Usart_Close( USARTDEVENUM eDev )
{
  USARTERR  eError = USART_ERR_NONE;
  PLCLCTL   ptCtl;
  
  // check for a valid USART
  if ( eDev < USART_DEV_ENUM_MAX )
  {
    // disable the USART
    ptCtl = &atLclCtls[ eDev ];
    ptCtl->ptUsart->US_CR &= ~( US_CR_RXEN | US_CR_TXEN );
  }
  else
  {
    // illegal device
    eError = USART_ERR_ILLDEV;
  }
  
  // return the error
  return( eError );
}

/******************************************************************************
 * @function USART0_Handler
 *
 * @brief interrupt USART0 handler
 *
 * This function handles the interrupts from USART0
 *
 *****************************************************************************/
void USART0_Handler( void )
{
  // call the common handler
  IrqCommonHandler( aePhyToDefMap[ USART_CHAN_0 ] );
}

/******************************************************************************
 * @function USART1_Handler
 *
 * @brief interrupt USART1 handler
 *
 * This function handles the interrupts from USART1
 *
 *****************************************************************************/
void USART1_Handler( void )
{
  // call the common handler
  IrqCommonHandler( aePhyToDefMap[ USART_CHAN_1 ] );
}

/******************************************************************************
 * @function USART2_Handler
 *
 * @brief interrupt USART2 handler
 *
 * This function handles the interrupts from USART2
 *
 *****************************************************************************/
void USART2_Handler( void )
{
  // call the common handler
  IrqCommonHandler( aePhyToDefMap[ USART_CHAN_2 ] );
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
static void IrqCommonHandler( USARTDEVENUM eDev )
{
  PUSARTDEF           ptDef;
  PLCLCTL             ptCtl;
  U8                  nOption;
  PVUSARTINTCALLBACK  pvCallBack;
  USARTIRQEVENTS      eEvent = USART_IRQ_EVENT_NONE;
  
  // get pointer to the definition/control structures
  ptDef = ( PUSARTDEF )&atUsartDefs[ eDev ];
  ptCtl = &atLclCtls[ eDev ];
  
  // get the callback
  pvCallBack = ptDef->pvCallback;
  
  // was this a RX interrupt
  if ( ptCtl->ptUsart->US_CSR & US_CSR_RXRDY )
  {
    // get the data
    nOption = ( U8 )ptCtl->ptUsart->US_RHR;

    // check for an error
    //if (( ptCtl->ptUsart->US_CSR & US_CSR_FRAME ) || ( ptCtl->ptUsart->US_CSR & US_CSR_PARE ) || ( ptCtl->ptUsart->US_CSR & US_CSR_OVRE ))
    //{
    //  // clear the error flag
    //  ptCtl->ptUsart->US_CR = ( US_CR_RSTSTA | US_CR_RXEN | US_CR_TXEN );
    //  
    //  // if callback process it
    //  if (( pvCallBack != NULL ) & ( ptDef->tEventFlags.bRxErr ))
    //  {
    //    // call the callback with a receive char event
    //    eEvent = USART_IRQ_EVENT_RXERR;
    //    pvCallBack( eEvent, nOption );
    //  }
    //}
    //else
    {
      // check for callback
      if ( ptDef->tEventFlags.bRxChar )
      {
        #if ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_TASKMANAGER )
        // if callback process it
        if (( pvCallBack != NULL ) & ( ptDef->tEventFlags.bRxChar ))
        {
          // call the callback with a receive char event
          eEvent = USART_IRQ_EVENT_RXCHAR;
          pvCallBack( eEvent, nOption );
        }
        // check for room in local buffer
        else if ( ptCtl->tRxBuf.wCount < ptDef->wRxBufSize )
        {
          // stuff in buffer/adust index/increment count
          *( ptDef->pnRxBuffer + ptCtl->tRxBuf.wWrIdx++ ) = nOption;
          ptCtl->tRxBuf.wWrIdx %= ptDef->wRxBufSize;
          ptCtl->tRxBuf.wCount++;
        }
        #elif ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_FREERTOS )
        static portBASE_TYPE xHigherPriorityTaskWoken = FALSE;
        
        // stuff character in queue
        xQueueSendToBackFromISR( ptCtl->xRxQueue, &nOption, &xHigherPriorityTaskWoken );
        
        // force a context switch
        portEND_SWITCHING_ISR( xHigherPriorityTaskWoken );
        #endif // SYSTEMDEFINE_OS_SELECTION
      }
    }
  }
  
  // check for a data register empty interrupt
  if ( ptCtl->ptUsart->US_CSR & US_CSR_TXRDY )
  {
    #if ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_TASKMANAGER )
    // check for characters available
    if ( ptCtl->tTxBuf.wCount != 0 )
    {
      // output the character
      nOption = *( ptDef->pnTxBuffer + ptCtl->tTxBuf.wRdIdx++ );
      ptCtl->tTxBuf.wRdIdx %= ptDef->wTxBufSize;
      ptCtl->tTxBuf.wCount--;
    #elif ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_FREERTOS )
    // check for characters available
    if ( xQueueIsQueueEmptyFromISR != pdFALSE )
    {
      // output the character
      xQueueReceiveFromISR( ptCtl->xTxQueue, &nOption, 0 );
    #endif // SYSTEMDEFINE_OS_SELECTION
    
      // send the character
      ptCtl->ptUsart->US_THR = nOption;
      
      // check for callback
      if (( pvCallBack != NULL ) && ( ptDef->tEventFlags.bTxEmp ))
      {
        // call back with the appropriate TX event
        eEvent = USART_IRQ_EVENT_TXEMP;
        nOption = 0;
        pvCallBack( eEvent, nOption );
      }
    }
    else
    {
      // turn off the data register empty interrupt
      ptCtl->ptUsart->US_IDR = US_IDR_TXRDY;
    }
  }  
    
  // check for a transmit complete interrupt
  if ( ptCtl->ptUsart->US_CSR & US_CSR_TXEMPTY )
  {
    #if ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_TASKMANAGER )
    // if data is done, perform the callback
    if ( ptCtl->tTxBuf.wCount == 0 )
    #elif ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_FREERTOS )
    // is anything still in queue
    if ( xQueueIsQueueEmptyFromISR( ptCtl->xTxQueue ) == TRUE )
    #endif // SYSTEMDEFINE_OS_SELECTION
    {
      if (( pvCallBack  != NULL ) && ( ptDef->tEventFlags.bTxCmp ))
      {
        // call back with the appropriate TX event
        eEvent = USART_IRQ_EVENT_TXCMP;
        nOption = 0;
        pvCallBack( eEvent, nOption );
      }
    }
    
    // disable the interrupt
    ptCtl->ptUsart->US_IDR = US_IDR_TXEMPTY;
  }
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
 * @Pparam[in]  ptCtl        pointer to the local control
 *
 *****************************************************************************/
static void GetUsartPointer( USARTCHAN eChan, PLCLCTL ptCtl )
{
  switch( eChan )
  {
    case USART_CHAN_0 :
      ptCtl->ptUsart = USART0;
      ptCtl->uInterrupt = USART0_IRQn;
      break;
      
    case USART_CHAN_1 :
      ptCtl->ptUsart = USART1;
      ptCtl->uInterrupt = USART1_IRQn;
      MATRIX->CCFG_SYSIO |= CCFG_SYSIO_SYSIO4;
      break;
    
    case USART_CHAN_2 :
      ptCtl->ptUsart = USART2;
      ptCtl->uInterrupt = USART2_IRQn;
      break;
    
    default :
      ptCtl->ptUsart = NULL;
      break;
  }
}

/******************************************************************************
 * @function ComputeBaudRate
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
static U16 ComputeBaudrate( U32 uBaudRate, USARTCLKSRC eClockSrc )
{
  U32   uPeripheralClock;
  U16   wBaudRate = 0;
  
  // get the system clock value
  switch( eClockSrc )
  {
    case USART_CLKSRC_MCK :
      uPeripheralClock = Clock_GetMastClockFreq( );
      break;
      
    case USART_CLKSRC_DIV8 :
      break;
      
    case USART_CLKSRC_PCK :
      break;
    
    default :  
      break;
  }
  
  // calculate the clock
  wBaudRate = uPeripheralClock / ( 16 * uBaudRate );
  
  // return the baud rate
  return( wBaudRate );
}

/**@} EOF Usart.c */
