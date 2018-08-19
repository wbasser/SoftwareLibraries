/******************************************************************************
 * @file Uart.c
 *
 * @brief UART implementation
 *
 * This file provides the implemntation for the UART peripherals
 *
 * @copyright Copyright (c) 2012 Nan Technologies
 * This document contains proprietary data and information of Nan Technologies
 * LLC. It is the exclusive property of Nan Technologies, LLC and will not be
 * disclosed in any form to any party without prior written permission of
 * Nan Technologies, LLC. This document may not be reproduced or further used
 * without the prior written permission of Nan Technologies, LLC.
 *
 * Version History
 * ======
 * $Rev: $
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
/// define the default DCTQ value for the baud rate generator
#define DFLT_DCTQ_VAL           9

/// define the default sample position for the baud rate generator
#define DFLT_SP_VAL             6

/// define the default transmit/receive buffer sizes
#define DFLT_TXBUF_SIZE         3             ///< 8 bytes
#define DFLT_TXBUF_PTR_CH0      0             ///< 0
#define DFLT_TXBUF_PTR_CH1      8             ///< 8

/// define the maximum value for divisor for the UARt
#define PDIV_MAX_VALUE          ( 1024U )

/// define the delay count for clock gating
#define CLK_DELAY_COUNT         ( 0x3FFU )

/// define the uart mode
#define DFLT_UART_MODE_VAL      ( 2 )

/// define the DCQT value
#define DFLT_DCQT_VAL           ( 0x0A )

/// define the default status Interrupt Node Pointer
#define DFLT_CHAN0_RCVIRQ_LVL   ( 0 )
#define DFLT_CHAN0_XMTIRQ_LVL   ( 1 )
#define DFLT_CHAN1_RCVIRQ_LVL   ( 3 )
#define DFLT_CHAN1_XMTIRQ_LVL   ( 4 )

/// define the macros to allow mapping the interrupts
#define UART_RcvIrq_Handler0    IRQ_Hdlr_9
#define UART_XmtIrq_Handler0    IRQ_Hdlr_10
#define UART_RcvIrq_Handler1    IRQ_Hdlr_12
#define UART_XmtIrq_Handler1    IRQ_Hdlr_13

/// define the interrupts
#define UART_RCVIRQ_HAND0       USIC0_0_IRQn
#define UART_XMTIRQ_HAND0       USIC0_1_IRQn
#define UART_RCVIRQ_HAND1       USIC0_3_IRQn
#define UART_XMTIRQ_HAND1       USIC0_4_IRQn

// enumerations ---------------------------------------------------------------
/// enumerate the local physical devices
typedef enum _PHYDEV
{
  PHY_DEV_0 = 0,
  PHY_DEV_1,
  PHY_DEV_MAX
} PHYDEV;

/// enumerate the baud rate fractional divider options
typedef enum _BGENFDIV
{
  BGEN_FDIV_OFF = 0,
  BGEN_FDIV_NRM,
  BGEN_FDIM_FRC,
} BGENFDIV;

// structures -----------------------------------------------------------------
/// define the buffer control structure
typedef struct _BUFCTL
{
  U8  nWrIdx;     ///< write index
  U8  nRdIdx;     ///< read index
  U8  nCount;     ///< buffer count
} BUFCTL, *PBUFCTL;
#define BUFCTL_SIZE   sizeof( BUFCTL )

/// define the local control structure
typedef struct _LCLCTL
{
  USIC_CH_TypeDef*  ptUartRegs;   ///< pointer to the UART registers
  U8                nRcvInprLvl;  ///< receive interrupt level
  U8                nXmtInprLvl;  ///< transmit interrupt level
  BUFCTL            tRxBuf;       ///< receive buffer control
  BUFCTL            tTxBuf;       ///< transmit buffer control
} LCLCTL, *PLCLCTL;
#define LCLCTL_SIZE   sizeof( LCLCTL )

/// define the physical to control map
typedef struct _PHYMAP
{
  UARTDEVENUM eDev;             ///< enumerated device
} PHYMAP, *PPHYMAP;
#define PHYMAP_SIZE   sizseof( PHYMAP )

// global parameter declarations ----------------------------------------------

// global interrupt handlers
void  UART_RcvIrq_Handler0( void );    ///< physically mapped to USIC0_2_IrqHandler (IRQ_Hdlr11)
void  UART_XmtIrq_Handler0( void );    ///< physically mapped to USIC0_3_IrqHandler (IRQ_Hdlr12)
void  UART_RcvIrq_Handler1( void );    ///< physically mapped to USIC0_4_IrqHandler (IRQ_Hdlr13)
void  UART_XmtIrq_Handler1( void );    ///< physically mapped to USIC0_5_IrqHandler (IRQ_Hdlr15)
 
// local parameter declarations -----------------------------------------------
static  LCLCTL  atLclCtls[ UART_DEV_MAX ];
static  PHYMAP  atPhyMaps[ PHY_DEV_MAX ];

// local function prototypes --------------------------------------------------
static  void  ComputeBaudrate( U32 uBaudrate, PU32 puStep, PU32 puPdiv );
static  void  RcvIrqCommonHandler( UARTDEVENUM eDev );
static  void  XmtIrqCommonHandler( UARTDEVENUM eDev );
static  void  PrimeTransmitFifo( PLCLCTL ptCtl, PUARTDEF ptDef );

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
  PUARTDEF    ptDef;
  PLCLCTL     ptCtl;
  U32         uTemp, uRcvIrqLvl, uXmtIrqLvl, uXmtPtr;
  U32         uStep, uPdiv;
  PHYDEV      ePhyDev;
  UARTDEVENUM eDev;
 
  // disable clock gating
  SCU_GENERAL->PASSWD = 0x000000C0UL;
  WR_REG( SCU_CLK->CLKCR, SCU_CLK_CLKCR_CNTADJ_Msk, SCU_CLK_CLKCR_CNTADJ_Pos, CLK_DELAY_COUNT );
  SET_BIT( SCU_CLK->CGATCLR0, SCU_CLK_CGATCLR0_USIC0_Pos );
  while (( SCU_CLK->CLKCR ) & ( SCU_CLK_CLKCR_VDDC2LOW_Msk ));
  SCU_GENERAL->PASSWD = 0x000000C3UL;

  // for each defined UART
  for ( eDev = 0; eDev < UART_DEV_MAX; eDev++ )
  {
    // get pointer to the definition/control structures
    ptDef = ( PUARTDEF )&atUartDefs[ eDev ];
    ptCtl = &atLclCtls[ eDev ];

    // clear the receive/transmit buffers
    ptCtl->tRxBuf.nWrIdx = ptCtl->tRxBuf.nRdIdx = ptCtl->tRxBuf.nCount = 0;
    ptCtl->tTxBuf.nWrIdx = ptCtl->tTxBuf.nRdIdx = ptCtl->tTxBuf.nCount = 0;

    // determine the port
    switch( ptDef->eChan )
    {
      case UART_CHAN_SC00 :
        ptCtl->ptUartRegs = USIC0_CH0;
        ptCtl->nRcvInprLvl = DFLT_CHAN0_RCVIRQ_LVL;
        ptCtl->nXmtInprLvl = DFLT_CHAN0_XMTIRQ_LVL;
        uRcvIrqLvl = UART_RCVIRQ_HAND0;
        uXmtIrqLvl = UART_XMTIRQ_HAND0;
        ePhyDev = PHY_DEV_0;
        uXmtPtr = DFLT_TXBUF_PTR_CH0;
        break;

      case UART_CHAN_SC01 :
        ptCtl->ptUartRegs = USIC0_CH1;
        ptCtl->nRcvInprLvl = DFLT_CHAN1_RCVIRQ_LVL;
        ptCtl->nXmtInprLvl = DFLT_CHAN1_XMTIRQ_LVL;
        uRcvIrqLvl = UART_RCVIRQ_HAND1;
        uXmtIrqLvl = UART_XMTIRQ_HAND1;
        ePhyDev = PHY_DEV_1;
        uXmtPtr = DFLT_TXBUF_PTR_CH1;
        break;

      default :
        uXmtIrqLvl = 0;
        uRcvIrqLvl = 0;
        ePhyDev = PHY_DEV_0;
        uXmtPtr = 0;
        break;
    }

    // Enable the USIC Channel
    ptCtl->ptUartRegs->KSCFG |= ( USIC_CH_KSCFG_MODEN_Msk | BIT( USIC_CH_KSCFG_BPMODEN_Pos ));
    uTemp = ptCtl->ptUartRegs->KSCFG;
    
    // compute the step value and divider for the baud rate
    ComputeBaudrate( ptDef->uBaudRate, &uStep, &uPdiv);

    // Configure the uFraction divider/step value
    ptCtl->ptUartRegs->FDR |= BITS( BGEN_FDIM_FRC, USIC_CH_FDR_DM_Pos );
    ptCtl->ptUartRegs->FDR |= ( 384 & USIC_CH_FDR_STEP_Msk );
//    ptCtl->ptUartRegs->FDR |= ( uStep & USIC_CH_FDR_STEP_Msk );

    // configure the baud rate
    uTemp = BITS( DFLT_DCTQ_VAL, USIC_CH_BRG_DCTQ_Pos );
    uTemp |= ( BITS( 124, USIC_CH_BRG_PDIV_Pos ) & USIC_CH_BRG_PDIV_Msk );
    //uTemp |= (( BITS( uPdiv, USIC_CH_BRG_PDIV_Pos ) & USIC_CH_BRG_PDIV_Msk ));
    ptCtl->ptUartRegs->BRG |= uTemp;

    // configure the shift control
    uTemp = BIT( USIC_CH_SCTR_PDL_Pos ) | BIT( USIC_CH_SCTR_TRM_Pos );
    ptCtl->ptUartRegs->SCTR |= uTemp;
    
    // Set Word Length (WLE) & Frame Length (FLE) */
    uTemp = BITS(( ptDef->eWrdLen - 1 ), USIC_CH_SCTR_FLE_Pos ) | BITS(( ptDef->eWrdLen - 1 ), USIC_CH_SCTR_WLE_Pos );
    ptCtl->ptUartRegs->SCTR |= uTemp;

    // configure the transmit control/status register
    uTemp = BIT( USIC_CH_TCSR_TDEN_Pos ) | BIT( USIC_CH_TCSR_TDSSM_Pos );
    ptCtl->ptUartRegs->TCSR |= uTemp; 

    // configure the protocol configuration register
    uTemp = BIT( USIC_CH_PCR_ASCMode_SMD_Pos ) | BITS( ptDef->eStop, USIC_CH_PCR_ASCMode_STPB_Pos ) | BITS( DFLT_SP_VAL, USIC_CH_PCR_ASCMode_SP_Pos );
    ptCtl->ptUartRegs->PCR_ASCMode |= uTemp;

    // configure the protocol status register
    ptCtl->ptUartRegs->PSR_ASCMode |= BIT( USIC_CH_PSR_TBIF_Pos );
    
    // configure the transmit buffer control register
    uTemp = BITS( DFLT_TXBUF_SIZE, USIC_CH_TBCTR_LIMIT_Pos );
    uTemp |= BITS( uXmtPtr, USIC_CH_TBCTR_DPTR_Pos );
    uTemp |= BITS( DFLT_TXBUF_SIZE, USIC_CH_TBCTR_SIZE_Pos );
    uTemp |= BIT( USIC_CH_TBCTR_STBTM_Pos );
    uTemp |= BIT( USIC_CH_TBCTR_STBTEN_Pos );
    uTemp |= BITS( ptCtl->nXmtInprLvl, USIC_CH_TBCTR_STBINP_Pos );
    ptCtl->ptUartRegs->TBCTR = uTemp;
   
    // configure the channel control register
    uTemp = DFLT_UART_MODE_VAL | BITS( ptDef->eParity, USIC_CH_CCR_PM_Pos );
    ptCtl->ptUartRegs->CCR |= uTemp;
    
    // map the input mux
    ptCtl->ptUartRegs->DX0CR = ptDef->eInputMux;

    // store the interrupt
    atPhyMaps[ ePhyDev ].eDev = eDev;
    
    // now set the interrupt sources to point to one interrupt
    uTemp = BITS( ptCtl->nXmtInprLvl, USIC_CH_INPR_TBINP_Pos ) | BITS( ptCtl->nRcvInprLvl, USIC_CH_INPR_RINP_Pos );
    ptCtl->ptUartRegs->INPR |= uTemp;

    // enable the receive/transmit interrupts
    uTemp = BIT( USIC_CH_CCR_RIEN_Pos ) | BIT( USIC_CH_CCR_TBIEN_Pos );
    ptCtl->ptUartRegs->CCR |= uTemp;

    // register the interrupt/enable it
    NVIC_SetPriority( uRcvIrqLvl, 3 );
    NVIC_SetPriority( uXmtIrqLvl, 3 );
    NVIC_EnableIRQ( uRcvIrqLvl );
    NVIC_EnableIRQ( uXmtIrqLvl );
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
 * @param[in]   nLength     size of the data buffer
 * @param[i0]   pnBytesWritten  the storage to return the number of bytes written
 *
 * @return      appropriate error value
 *
 *****************************************************************************/
UARTERR Uart_Write( UARTDEVENUM eDev, PU8 pnData, U8 nLength, PU8 pnBytesWritten )
{
  UARTERR   eError = UART_ERR_NONE;
  PUARTDEF  ptDef;
  PLCLCTL   ptCtl;
  U8        nBytesRemaining, nIdx;
 
  // check for a valid UART
  if ( eDev < UART_DEV_MAX )
  {
    // set the bytes written to zero
    *pnBytesWritten = 0;

    // get pointers to the control/def structure
    ptDef = ( PUARTDEF )&atUartDefs[ eDev ];
    ptCtl = &atLclCtls[ eDev ];
    
    // compute the bytes written
    nBytesRemaining = ptDef->nTxBufSize - ptCtl->tTxBuf.nCount;
    *pnBytesWritten = MIN( nBytesRemaining, nLength );
    
    // for each byte
    Interrupt_Disable( );
    for ( nIdx = 0; nIdx < *pnBytesWritten; nIdx++ )
    {
      // put in buffer/adjust pointer/increment count
      *( ptDef->pnTxBuffer + ptCtl->tTxBuf.nWrIdx++ ) = *( pnData + nIdx );
      ptCtl->tTxBuf.nWrIdx %= ptDef->nTxBufSize;
      ptCtl->tTxBuf.nCount++;
    }

    // prime the transmit
    PrimeTransmitFifo( ptCtl, ptDef );

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
 * @param[in]   nLength     size of the data buffer
 * @param[i0]   pnBytesRead the storage to return the number of bytes written
 *
 * @return      appropriate error value
 *
 *****************************************************************************/
UARTERR Uart_Read( UARTDEVENUM eDev, PU8 pnData, U8 nLength, PU8 pnBytesRead )
{
  UARTERR   eError = UART_ERR_NONE;
  PUARTDEF  ptDef;
  PLCLCTL   ptCtl;
  U8        nBytesAvailable, nIdx;
  
  // check for a valid UART
  if ( eDev < UART_DEV_MAX )
  {
    // get pointers to the control/def structure
    ptDef = ( PUARTDEF )&atUartDefs[ eDev ];
    ptCtl = &atLclCtls[ eDev ];
    
    // compute the bytes written
    nBytesAvailable = ptDef->nRxBufSize - ptCtl->tRxBuf.nCount;
    *pnBytesRead = MIN( nBytesAvailable, nLength );
    
    // for each byte
    Interrupt_Disable( );
    for ( nIdx = 0; nIdx < *pnBytesRead; nIdx++ )
    {
      // put in buffer/adjust pointer/increment count
      *( pnData + nIdx ) = *( ptDef->pnRxBuffer + ptCtl->tRxBuf.nRdIdx++ );
      ptCtl->tRxBuf.nRdIdx %= ptDef->nRxBufSize;
      ptCtl->tRxBuf.nCount--;
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
  if ( eDev < UART_DEV_MAX )
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
  if ( eDev < UART_DEV_MAX )
  {
    // disable the USART/free the memory
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
 * @function UART_RcvIrq_Handler0
 *
 * @brief interrupt device 0 handler
 *
 * This function handles the interrupts from UART 0
 *
 *****************************************************************************/
void UART_RcvIrq_Handler0( void )
{
  // call the common handler
  RcvIrqCommonHandler( atPhyMaps[ PHY_DEV_0 ].eDev );
}

/******************************************************************************
 * @function UART_XmtIrq_Handler0
 *
 * @brief interrupt device 0 handler
 *
 * This function handles the interrupts from UART 0
 *
 *****************************************************************************/
void UART_XmtIrq_Handler0( void )
{
  // call the common handler
  XmtIrqCommonHandler( atPhyMaps[ PHY_DEV_0 ].eDev );
}

/******************************************************************************
 * @function UART_RcvIrq_Handler1
 *
 * @brief interrupt device 1 handler
 *
 * This function handles the interrupts from UART 1
 *
 *****************************************************************************/
void UART_RcvIrq_Handler1( void )
{
  // call the common handler
  RcvIrqCommonHandler( atPhyMaps[ PHY_DEV_1 ].eDev );
}

/******************************************************************************
 * @function UART_XmtIrq_Handler1
 *
 * @brief interrupt device 1 handler
 *
 * This function handles the interrupts from UART 1
 *
 *****************************************************************************/
void UART_XmtIrq_Handler1( void )
{
  // call the common handler
  XmtIrqCommonHandler( atPhyMaps[ PHY_DEV_1 ].eDev );
}

/******************************************************************************
 * @function RcvIrqCommandHandler
 *
 * @brief common interrupt handler
 *
 * This function checks for the type of interrupts and processes them
 * appropriately
 *
 * @param[in]   eDev        Device enumeration
 *
 *****************************************************************************/
static void RcvIrqCommonHandler( UARTDEVENUM eDev )
{
  PUARTDEF          ptDef;
  PLCLCTL           ptCtl;
  U8                nData;
  PVUARTINTCALLBACK pvCallBack;

  // get pointer to the definition/control structures
  ptDef = ( PUARTDEF )&atUartDefs[ eDev ];
  ptCtl = &atLclCtls[ eDev ];

  // get the callback
  pvCallBack = ptDef->pvCallBack;

  // was this a RX interrupt
  if ( ptCtl->ptUartRegs->PSR_ASCMode & USIC_CH_PSR_ASCMode_RIF_Msk )
  {
    // clear the flag
    ptCtl->ptUartRegs->PSCR |= USIC_CH_PSCR_CRIF_Msk;
    
    // get the data
    nData = ( U8 )ptCtl->ptUartRegs->RBUF;

    // check for callback
    if ( ptDef->tEventFlags.bRxChar )
    {
      if ( pvCallBack != NULL )
      {
        // call the callback with a receive char event
        pvCallBack( UART_IRQ_EVENT_RXCHAR, nData );
      }
      // check for room in local buffer
      else if ( ptCtl->tRxBuf.nCount < ptDef->nRxBufSize )
      {
        // stuff in buffer/adust index/increment count
        *( ptDef->pnRxBuffer + ptCtl->tRxBuf.nWrIdx++ ) = nData;
        ptCtl->tRxBuf.nWrIdx %= ptDef->nRxBufSize;
        ptCtl->tRxBuf.nCount++;
      }
    }
  }
}

/******************************************************************************
 * @function XmtIrqCommandHandler
 *
 * @brief common interrupt handler
 *
 * This function checks for the type of interrupts and processes them
 * appropriately
 *
 * @param[in]   eDev        Device enumeration
 *
 *****************************************************************************/
static void XmtIrqCommonHandler( UARTDEVENUM eDev )
{
  PUARTDEF          ptDef;
  PLCLCTL           ptCtl;
  PVUARTINTCALLBACK pvCallBack;

  // get pointer to the definition/control structures
  ptDef = ( PUARTDEF )&atUartDefs[ eDev ];
  ptCtl = &atLclCtls[ eDev ];

  // get the callback
  pvCallBack = ptDef->pvCallBack;

  // was this a TX interrupt
  if ( ptCtl->ptUartRegs->PSR_ASCMode & USIC_CH_PSR_ASCMode_TBIF_Msk )
  {
    // clear the flag
    ptCtl->ptUartRegs->PSCR |= BIT( USIC_CH_PSCR_CTBIF_Pos );
    
    // prime the fifo
    PrimeTransmitFifo( ptCtl, ptDef );
    
    // clear the standard interrupt flag
    ptCtl->ptUartRegs->TRBSCR |= BIT( USIC_CH_TRBSCR_CSTBI_Pos );
    
    // check for callback
    if (( pvCallBack ) != NULL && ( ptDef->tEventFlags.bTxCmp ))
    {
      // call back with the appropriate TX event
      pvCallBack( UART_IRQ_EVENT_TXEMP, 0 );
    }
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
 * @param[io]   puStep      pointer to the storage for the computed step
 * @param[io]   puPdiv      pointer to the storage for the computed divisor
 *
 *****************************************************************************/
static void ComputeBaudrate( U32 uBaudrate, PU32 puStep, PU32 puPdiv )
{
  U32   uFdrStep = 0x00U;
  U32   uBrgPdiv = 0x00U;
  U32   uDivisorIn = uBaudrate ;
  U32   uDividendIn = 0U;
  U32   auDivisor[ 4 ];
  U32   auDividend[ 4 ];
  U32   auRemainder[ 4 ];
  U32   auDivisorArray[ 4 ];
  U32   auDividendArray[ 4 ];
  U32   auDivArray[ 4 ];
  U32   uMax, uFrac, uDivisor1, uDivisor2, uDividend1, uDividend2;
  U32   uArrayCount, uArrayCount1, uArrayCount2;
  U32   uTemp = 0U;
  FLOAT fDivFactor;
  U8    nLoopCnt;
  BOOL  bSwapped, bRunFlag;

  // calculate the dividend in
  uDividendIn = ( U32 )(( U32 )( SystemCoreClock * 1000000 ) / ( PDIV_MAX_VALUE * DFLT_DCQT_VAL ));

  // swap if uDivisorIn/uDividendIn > uMax_auDivisor/uMax_auDividend
  fDivFactor = ( FLOAT )uDivisorIn / ( FLOAT )uDividendIn;
  if ( fDivFactor > 1.0 )
  {
    uMax = PDIV_MAX_VALUE;
    bSwapped = 1U;
    uTemp = uDivisorIn;
    uDivisorIn = uDividendIn;
    uDividendIn = uTemp;
  }
  else
  {
    bSwapped = 0U;
    uMax = PDIV_MAX_VALUE;
  }

  // *Init parameters
  nLoopCnt = 0U;
  uArrayCount = 0U;
  uArrayCount1 = 0U;
  auDivisor[0] = uDivisorIn;
  auRemainder[0] = uDividendIn;
  auDivisorArray[0] = 1U;
  auDividendArray[0] = 0U;

  // set the run flag
  bRunFlag = TRUE;

  // calculation loop
  do
  {
    ++nLoopCnt;
    uArrayCount2 = uArrayCount1;
    uArrayCount1 = uArrayCount;
    uArrayCount = nLoopCnt % 4U;
    auDividend[ uArrayCount ] = auDivisor[ uArrayCount1 ];
    auDivisor[ uArrayCount ] = auRemainder[ uArrayCount1 ];
    auDivArray[ uArrayCount ] = auDividend[ uArrayCount ] / auDivisor[ uArrayCount ];
    auRemainder[ uArrayCount ] = auDividend[ uArrayCount ] % auDivisor[ uArrayCount ];

    // check for first iteration
    if ( nLoopCnt == 1 )
    {
      // only on first iteration
      auDivisorArray[ 1 ] = auDivArray[ 1 ];
      auDividendArray[ 1 ] = 1U;
    }
    else
    {
      auDivisorArray[ uArrayCount ] = ( auDivArray[ uArrayCount ] * auDivisorArray[ uArrayCount1 ]) + auDivisorArray[ uArrayCount2 ];
      auDividendArray[ uArrayCount ] = ( auDivArray[ uArrayCount ] * auDividendArray[ uArrayCount1 ]) + auDividendArray[ uArrayCount2 ];
    }

    if ( auDividendArray[ uArrayCount ] > uMax )
    {
      uDivisor1 = auDivisorArray[ uArrayCount1 ];
      uDividend1 = auDividendArray[ uArrayCount1 ];
      uFrac = ( U32 )( uMax - auDividendArray[ uArrayCount2 ]) / auDividendArray[ uArrayCount1 ];
      uDivisor2 = ( U32 )( uFrac * auDivisorArray[ uArrayCount - 1 ]) + auDivisorArray[ uArrayCount - 2 ];
      uDividend2 = ( U32 )( uFrac * auDividendArray[ uArrayCount - 1 ]) + auDividendArray[ uArrayCount2 ];

      // Swap if required
      if ( bSwapped )
      {
        // Swap uDivisor1 and uDividend1
        uTemp = uDivisor1;
        uDivisor1 = uDividend1;
        uDividend1 = uTemp;

        // Swap uDivisor2 and uDividend2
        uTemp = uDivisor2;
        uDivisor2 = uDividend2;
        uDividend2 = uTemp;
      }

      // Remove if one of has auDivisor 0
      if (( uDividend1 == 0U ) || ( uDivisor1 == 0U ))
      {
        uFdrStep = uDivisor2;
        uBrgPdiv = uDividend2;
      }
      else if (( uDividend2 == 0U ) || ( uDivisor2 == 0U ))
      {
        uFdrStep = uDivisor1;
        uBrgPdiv = uDividend1;
      }
      else
      {
        //  Find the most nearest to target division
        if ( FABS( fDivFactor - (( FLOAT )uDivisor1 / ( FLOAT )uDividend1 )) > FABS( fDivFactor - (( FLOAT )uDivisor2 / ( FLOAT )uDividend2 )))
        {
          uFdrStep = uDivisor2;
          uBrgPdiv = uDividend2;
        }
        else
        {
          uFdrStep = uDivisor1;
          uBrgPdiv = uDividend1;
        }
      }

      // clear the run flag
      bRunFlag = FALSE;
    }
    else if ( auRemainder[ uArrayCount ]== 0U )
    {
      uFdrStep = auDivisorArray[ uArrayCount ];
      uBrgPdiv = auDividendArray[ uArrayCount ];
      if ( bSwapped )
      {
        // Swap uFdrStep and uBrgPdiv
        uTemp = uFdrStep;
        uFdrStep = uBrgPdiv;
        uBrgPdiv = uTemp;
      }

      // clear the run flag
      bRunFlag = FALSE;
    }
  } while ( bRunFlag );

  // check for limits
  if( uFdrStep >= PDIV_MAX_VALUE )
  {
    // set it to max
    uFdrStep = 1023U;
  }

  // set the return values
  *puStep = uFdrStep;
  *puPdiv = uBrgPdiv - 1U;
}

/******************************************************************************
 * @function PrimeTransmitFifo
 *
 * @brief prime the transmit buffer
 *
 * This function will move as many characters into the transmit fifo as allowed
 *
 * @param[in]   ptCtl       pointer to the control structure
 * @param[in]   ptDef       pointer to the definition structure
 *
 *****************************************************************************/
static void PrimeTransmitFifo( PLCLCTL ptCtl, PUARTDEF ptDef )
{
  // while there is room in the fifo and more data to transmit
  while(( ptCtl->tTxBuf.nCount != 0 ) && !( ptCtl->ptUartRegs->TRBSR & USIC_CH_TRBSR_TFULL_Msk ))
  {
    // fill buffer/adjust pointer/decrement count
    ptCtl->ptUartRegs->IN[ 0 ] = *( ptDef->pnTxBuffer + ptCtl->tTxBuf.nRdIdx++ );
    ptCtl->tTxBuf.nRdIdx %= ptDef->nTxBufSize;
    ptCtl->tTxBuf.nCount--;
  }
}

/**@} EOF Uart.c */
