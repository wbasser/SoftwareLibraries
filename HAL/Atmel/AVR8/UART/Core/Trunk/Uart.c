/******************************************************************************
 * @file Uart.c
 *
 * @brief Uart implementation 
 *
 * This file 
 *
 * @copyright Copyright (c) 2012 Cyber Intergration
 * This document contains proprietary data and information of Cyber Integration 
 * LLC. It is the exclusive property of Cyber Integration, LLC and will not be 
 * disclosed in any form to any party without prior written permission of 
 * Cyber Integration, LLC. This document may not be reproduced or further used 
 * without the prior written permission of Cyber Integration, LLC.
 *
 * Version History
 * ======
 * $Log: $
 * 
 *
 * \addtogroup Uart
 * @{
 *****************************************************************************/

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "UART/Uart.h"

// library includes -----------------------------------------------------------
#include "Interrupt/Interrupt.h"
#include "Clock/Clock.h"

// Macros and Defines ---------------------------------------------------------

// define the masks
#define RXCIE_MASK    BIT( 7 )
#define TXCIE_MASK    BIT( 6 )
#define UDRE_MASK     BIT( 5 )
#define RXEN_MASK     BIT( 4 )
#define TXEN_MASK     BIT( 3 )
#define FRMERR_MASK   BIT( 4 )
#define PARERR_MASK   BIT( 2 )
#define TXC_MASK      BIT( 6 )

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
  U16 wWrIdx;     ///< write index
  U16 wRdIdx;     ///< read index
  U16 wCount;     ///< buffer count
} BUFCTL, *PBUFCTL;
#define BUFCTL_SIZE sizeof( BUFCTL )

/// define the local control structure
typedef struct _LCLCTL
{
  PU8     pnUDR;    ///< data register
  PU8     pnUCSRA;  ///< control/status register A
  PU8     pnUCSRB;  ///< control/status register B
  PU8     pnUCSRC;  ///< control/status register C
  PU8     pnUCSRD;  ///< control/status register D
  PU16    pwUBRR;   ///< baud rate register
  BUFCTL  tRxBuf;   ///< receive buffer control
  BUFCTL  tTxBuf;   ///< transmit buffer control
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
static  LCLCTL    atLclCtls[ UART_CHAN_MAX ];
static  PHYLOGMAP atMaps[ UART_CHAN_MAX ];

// local function prototypes --------------------------------------------------
static  void  IrqCommonHandler( PHYDEV ePhy, UARTIRQEVENTS eEvent );

// constant parameter initializations -----------------------------------------

/******************************************************************************
 * @function Uart_Initialize
 *
 * @brief initialization
 *
 * This function will initialize the UART to its defined operating
 * characteristics
 *
 *****************************************************************************/
void Uart_Initialize( void )
{
  UARTDEVENUM eUart;
  UARTDEF     tDef;
  U16         wDivisor;
  U8          nTemp;
  PLCLCTL     ptCtl;
  
  // for each defined UART
  for ( eUart = 0; eUart < UART_DEV_ENUM_MAX; eUart++ )
  {
    // get a local copy of the declaration/set pointer to the control
    MEMCPY_P( &tDef, ( PUARTDEF )&atUartDefs[ eUart ], UARTDEF_SIZE );
    ptCtl = &atLclCtls[ eUart ];

    // set the registers
    switch( tDef.eChan )
    {
      case UART_CHAN_0 :
        #ifdef UDR0
          // set the registers
          ptCtl->pnUDR = ( PU8 )&UDR0;
          ptCtl->pnUCSRA = ( PU8 )&UCSR0A;
          ptCtl->pnUCSRB = ( PU8 )&UCSR0B;
          ptCtl->pnUCSRC = ( PU8 )&UCSR0C;
          #ifdef USCR1D
            ptCtl->pnUCSRD = ( PU8 )&UCSR0D;
          #endif
          ptCtl->pwUBRR = ( PU16 )&UBRR0;
          
          // set the map to 0      
          atMaps[ PHY_UART_DEV0 ].eDev = eUart;
        #endif
        break;
        
      case UART_CHAN_1 :
        #ifdef UDR1
          // set the registers
          ptCtl->pnUDR = ( PU8 )&UDR1;
          ptCtl->pnUCSRA = ( PU8 )&UCSR1A;
          ptCtl->pnUCSRB = ( PU8 )&UCSR1B;
          ptCtl->pnUCSRC = ( PU8 )&UCSR1C;
          #ifdef USCR1D
            ptCtl->pnUCSRD = ( PU8 )&UCSR1D;
          #endif
          ptCtl->pwUBRR = ( PU16 )&UBRR1;

          // set the map to 1
          atMaps[ PHY_UART_DEV1 ].eDev = eUart;
        #endif
        break;
        
      case UART_CHAN_2 :
        #ifdef UDR2
          // set the registers
          ptCtl->pnUDR = ( PU8 )&UDR2;
          ptCtl->pnUCSRA = ( PU8 )&UCSR2A;
          ptCtl->pnUCSRB = ( PU8 )&UCSR2B;
          ptCtl->pnUCSRC = ( PU8 )&UCSR2C;
          #ifdef USCR1D
            ptCtl->pnUCSRD = ( PU8 )&UCSR2D;
          #endif
          ptCtl->pwUBRR = ( PU16 )&UBRR2;

          // set the map to 2
          atMaps[ PHY_UART_DEV2 ].eDev = eUart;
        #endif
        break;
        
      default :
        break;
    }
    
    // calculate the divisor/set the baud rate
    wDivisor = ( U16 )(( Clock_GetFreq( ) / ( 16 * tDef.uBaudRate )) - 1 );
    *( ptCtl->pwUBRR ) = wDivisor;
    
    // get the UCSRC control value
    nTemp = BITS( tDef.eMode, 6 );
    nTemp |= BITS( tDef.eParity, 4 );
    nTemp |= BITS( tDef.eStopBits, 3 );
    nTemp |= BITS(( tDef.eWordLen & 0x03 ), 1 );
    *( ptCtl->pnUCSRC ) = nTemp;

    // set up the control registers
    // now enable the transmitter, receiver and receiver interrupts/
    *( ptCtl->pnUCSRB ) |= ( RXCIE_MASK | RXEN_MASK | TXEN_MASK );
    
    // clear the indices
    ptCtl->tRxBuf.wWrIdx = ptCtl->tRxBuf.wRdIdx = ptCtl->tRxBuf.wCount = 0;
    ptCtl->tTxBuf.wWrIdx = ptCtl->tTxBuf.wRdIdx = ptCtl->tTxBuf.wCount = 0;
  }
}

/******************************************************************************
 * @function Uart_Read
 *
 * @brief read bytes from the UART
 *
 * This function will read bytes from the UART buffers
 *
 * @param[in]   eDev        device
 * @param[in]   pnData      pointer to the data storage
 * @param[in]   nLength     length of the data storage, number of bytes to read
 * @param[io]   pnBytesRead set to the number of actual bytes read
 *
 * @return      appropriate error
 *
 *****************************************************************************/
UARTERR Uart_Read( UARTDEVENUM eDev, PU8 pnData, U16 wLength, PU16 pwBytesRead )
{
  UARTERR eError = UART_ERR_NONE;
  UARTDEF tDef;
  PLCLCTL ptCtl;
  U16     wIdx;
  
  // check for a valid UART
  if ( eDev < UART_DEV_ENUM_MAX )
  {
    // get a copy of the def and set the pointer to the control
    MEMCPY_P( &tDef, ( PUARTDEF )&atUartDefs[ eDev ], UARTDEF_SIZE );
    ptCtl = &atLclCtls[ eDev ];
    
    // compute the number of bytes to read
    *pwBytesRead = MIN( ptCtl->tRxBuf.wCount, wLength );
    
    // disalbe interrupt
    Interrupt_Disable( );
    
    // copy the buffer
    for ( wIdx = 0; wIdx < *pwBytesRead; wIdx++ )
    {
      // stuff in buffer, adjust pointer/increemnt count
      *( pnData + wIdx ) = *( tDef.pnRxBuffer + ptCtl->tRxBuf.wRdIdx++ );
      ptCtl->tRxBuf.wRdIdx %= tDef.wRxBufSize;
      ptCtl->tRxBuf.wCount--;
    }
    
    // re-enable interrupts
    Interrupt_Enable( );
  }
  else
  {
    // set the error
    eError = UART_ERR_ILLDEV;
  }
  
  // return the error
  return( eError );
}

/******************************************************************************
 * @function Uart_Write
 *
 * @brief write data to the UART
 *
 * This function 
 *
 * @param[in]   eDev        device
 * @param[in]   pnData      pointer to the data storage
 * @param[in]   wLength     length of the data storage, number of bytes to write
 * @param[io]   pwBytesRead set to the number of actual bytes written
 *
 * @return      appropriate error
 *
 *****************************************************************************/
UARTERR Uart_Write( UARTDEVENUM eDev, PU8 pnData, U16 wLength, PU16 pwBytesWritten )
{
  UARTERR eError = UART_ERR_NONE;
  UARTDEF tDef;
  PLCLCTL ptCtl;
  U16     wBytesRemaining, wIdx;
  
  // check for a valid UART
  if ( eDev < UART_DEV_ENUM_MAX )
  {
    // get a copy of the def and set the pointer to the control
    MEMCPY_P( &tDef, ( PUARTDEF )&atUartDefs[ eDev ], UARTDEF_SIZE );
    ptCtl = &atLclCtls[ eDev ];
    
    // compute the bytes written
    wBytesRemaining = tDef.wTxBufSize - ptCtl->tTxBuf.wCount;
    *pwBytesWritten = MIN( wBytesRemaining, wLength );
    
    // disalb interrupt
    Interrupt_Disable( );
    
    // copy the buffer
    for ( wIdx = 0; wIdx < *pwBytesWritten; wIdx++ )
    {
      // stuff in buffer, adjust pointer/increemnt count
      *( tDef.pnTxBuffer + ptCtl->tTxBuf.wWrIdx++ ) = *( pnData + wIdx );
      ptCtl->tTxBuf.wWrIdx %= tDef.wTxBufSize;
      ptCtl->tTxBuf.wCount++;
    }
    
    // re-enable interrupts
    Interrupt_Enable( );
    
    if ( *( ptCtl->pnUCSRA ) & TXC_MASK )
    {
      // clear the mask
      *( ptCtl->pnUCSRA ) |= TXC_MASK;
      *( ptCtl->pnUCSRB ) |= TXCIE_MASK;
    }
    
    // always set the shift register empty
    *( ptCtl->pnUCSRB ) |= UDRE_MASK;
  }
  else
  {
    // set the error
    eError = UART_ERR_ILLDEV;
  }
  
  // return the error
  return( eError );
}

/******************************************************************************
 * @function Uart_Ioctl
 *
 * @brief 
 *
 * This function 
 *
 * @param[in]   
 *
 * @return      
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
    // set the error
    eError = UART_ERR_ILLDEV;
  }

  // return the error
  return( eError );
}

/******************************************************************************
 * @function Uart_CLose 
 *
 * @brief close the UART channel
 *
 * This function will close the UART channel
 *
 * @param[in]   eUart   uart device
 *
 * @return      appropriate error
 *
 *****************************************************************************/
UARTERR Uart_Close( UARTDEVENUM eUart )
{
  UARTERR eError = UART_ERR_NONE;
  
  // return the error
  return( eError );
}

#if defined ( UDR0 )
/******************************************************************************
 * @function USART0_RX_VECT
 *
 * @brief USART 0 RX vector
 *
 * This function handles the recieved character interrupt
 *
 *****************************************************************************/
ISR( USART0_RX_vect )
{
  // call the common handler
  IrqCommonHandler( PHY_UART_DEV0, UART_IRQ_EVENT_RXCHAR );
}
 
/******************************************************************************
 * @function USART0_UDRE_VECT
 *
 * @brief USART 0 UDRE vector
 *
 * This function handles the transmit shift register empty interrupt
 *
 *****************************************************************************/
ISR( USART0_UDRE_vect )
{
  // call the common handler
  IrqCommonHandler( PHY_UART_DEV0, UART_IRQ_EVENT_TXEMP );
}

/******************************************************************************
 * @function USART0_TX_VECT
 *
 * @brief USART 0 TX vector
 *
 * This function handles the transmit complete interrupt
 *
 *****************************************************************************/
ISR( USART0_TX_vect )
{
  // call the common handler
  IrqCommonHandler( PHY_UART_DEV0, UART_IRQ_EVENT_TXCMP );
}
#endif // UDDR0

#if defined ( UDR1 )
/******************************************************************************
 * @function USART1_RX_VECT
 *
 * @brief USART 1 RX vector
 *
 * This function handles the recieved character interrupt
 *
 *****************************************************************************/
ISR( USART1_RX_vect )
{
  // call the common handler
  IrqCommonHandler( PHY_UART_DEV1, UART_IRQ_EVENT_RXCHAR );
}
 
/******************************************************************************
 * @function USART1_UDRE_VECT
 *
 * @brief USART 1 UDRE vector
 *
 * This function handles the transmit shift register empty interrupt
 *
 *****************************************************************************/
ISR( USART1_UDRE_vect )
{
  // call the common handler
  IrqCommonHandler( PHY_UART_DEV1, UART_IRQ_EVENT_TXEMP );
}

/******************************************************************************
 * @function USART1_TX_VECT
 *
 * @brief USART 1 TX vector
 *
 * This function handles the transmit complete interrupt
 *
 *****************************************************************************/
ISR( USART1_TX_vect )
{
  // call the common handler
  IrqCommonHandler( PHY_UART_DEV1, UART_IRQ_EVENT_TXCMP );
}
#endif // UDDR1

#if defined ( UDR2 )
/******************************************************************************
 * @function USART2_RX_VECT
 *
 * @brief USART 2 RX vector
 *
 * This function handles the recieved character interrupt
 *
 *****************************************************************************/
ISR( USART2_RX_vect )
{
  // call the common handler
  IrqCommonHandler( PHY_UART_DEV2, UART_IRQ_EVENT_RXCHAR );
}
 
/******************************************************************************
 * @function USART2_UDRE_VECT
 *
 * @brief USART 2 UDRE vector
 *
 * This function handles the transmit shift register empty interrupt
 *
 *****************************************************************************/
ISR( USART2_UDRE_vect )
{
  // call the common handler
  IrqCommonHandler( PHY_UART_DEV2, UART_IRQ_EVENT_TXEMP );
}

/******************************************************************************
 * @function USART2_TX_VECT
 *
 * @brief USART 2 TX vector
 *
 * This function handles the transmit complete interrupt
 *
 *****************************************************************************/
ISR( USART2_TX_vect )
{
  // call the common handler
  IrqCommonHandler( PHY_UART_DEV2, UART_IRQ_EVENT_TXCMP );
}
#endif // UDDR2

/******************************************************************************
 * @function IrqCommonHandler
 *
 * @brief common interrupt handler
 *
 * This function handles the interrupts
 *
 * @param[in] ePhy    physical device
 * @param[in] eEvent  interrupt event
 *
 *****************************************************************************/
static void IrqCommonHandler( PHYDEV ePhy, UARTIRQEVENTS eEvent )
{
  UARTDEVENUM eDev;
  PLCLCTL     ptCtl;
  UARTDEF     tDef;
  U8          nChar, nSts;
  
  // get the logical device
  eDev = atMaps[ ePhy ].eDev;
  
  // get the definition/control
  MEMCPY_P( &tDef, ( PUARTDEF )&atUartDefs[ eDev ], UARTDEF_SIZE );
  ptCtl = &atLclCtls[ eDev ];
  
  // process the event
  switch( eEvent )
  {
    case UART_IRQ_EVENT_RXCHAR :
      // get the status and data
      nSts = *( ptCtl->pnUCSRA );
      nChar = *( ptCtl->pnUDR );
      
      // check for break detect
      if ( nSts & FRMERR_MASK )
      {
        // set the break detect event
        eEvent = UART_IRQ_EVENT_BRKDET;
      }
      else if ( nSts & PARERR_MASK )
      {
        // set the error event
        eEvent = UART_IRQ_EVENT_RXERR;
      }
      else
      {
        // if callback is not null
        if ( tDef.pvCallback == NULL )
        {
          // process the character
          if ( ptCtl->tRxBuf.wCount < tDef.wRxBufSize )
          {
            // stuff the char
            *( tDef.pnRxBuffer + ptCtl->tRxBuf.wWrIdx++ ) = nChar;
            ptCtl->tRxBuf.wWrIdx %= tDef.wRxBufSize;
            ptCtl->tRxBuf.wCount++;
          }
        }
      }
      break;
      
    case UART_IRQ_EVENT_TXEMP :
      // check for characters available
      if ( ptCtl->tTxBuf.wCount != 0 )
      {
        // output the character
        nChar = *( tDef.pnTxBuffer + ptCtl->tTxBuf.wRdIdx++ );
        *( ptCtl->pnUDR ) = nChar;
        ptCtl->tTxBuf.wRdIdx %= tDef.wTxBufSize;
        ptCtl->tTxBuf.wCount--;
      }
      else
      {
        // turn off the interrupts
        *( ptCtl->pnUCSRB ) &= ~UDRE_MASK;
      }
      
      // clear the option
      nChar = 0;
      break;
      
    case UART_IRQ_EVENT_TXCMP :
      // clear the interrupt
      if ( ptCtl->tTxBuf.wCount == 0 )
      {
        // just clear the interrupt
        *( ptCtl->pnUCSRB ) &= ~TXCIE_MASK;
      }
      
      // clear the option
      nChar = 0;
      break;
      
    default :
      // clear the option
      nChar = 0;
      break;
  }
  
  // check for a valid callback
  if ( tDef.pvCallback != NULL )
  {
    // call it
    tDef.pvCallback( eEvent, nChar );
  }
}

/**@} EOF Uart.c */
