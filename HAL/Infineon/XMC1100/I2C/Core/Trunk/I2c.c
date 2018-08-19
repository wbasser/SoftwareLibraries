/******************************************************************************
 * @file I2C.c
 *
 * @brief I2C implementation
 *
 * This file provides the implemntation for the I2C peripherals
 *
 * @copyright Copyright (c) 2014 Nan Technologies
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
 * \addtogroup I2C
 * @{
 *****************************************************************************/

// system includes ------------------------------------------------------------

// library includes -----------------------------------------------------------

// local includes -------------------------------------------------------------
#include "I2C/I2c.h"
#include "Interrupt/Interrupt.h"
#include "SystemTick/SystemTick.h"

// Macros and Defines ---------------------------------------------------------
/// define the delay count for clock gating
#define CLK_DELAY_COUNT         ( 0x3FFU )

/// define the uart mode
#define DFLT_I2C_MODE_VAL     	( 4 )

/// define the default frame length/word length/MSB Direction
#define DFLT_I2C_SCTR_FLE_VAL 	( 0x3F )
#define DFLT_I2C_SCTR_WLE_VAL 	( 0x07 )
#define DFLT_I2C_SCTR_SDR_VAL 	( 0x01 )
#define	DFLT_I2C_SCTR_TRM_VAL		( 0x03 )

/// define the default TCSR values
#define	DFLT_I2C_TCSR_TDEN_VAL	( 0x01 )
#define	DFLT_I2C_TCSR_TDSSM_VAL	( 0x01 )

/// define the default transmit size/pointers
#define DFLT_TXBUF_SIZE         ( 1 )
#define DFLT_TXBUF_PTR_CH0      ( 0 )
#define DFLT_TXBUF_PTR_CH1      ( 2 )

/// define the default receive size/pointers
#define	DFLT_RXBUF_SIZE					( 1 )
#define	DFLT_RXBUF_PTR_CH0			( 0 )
#define	DFLT_RXBUF_PTR_CH1			( 2 )

/// define the maximum value for divisor for the UARt
#define PDIV_MAX_VALUE        	( 1024U )

/// define the PDIV step value
#define PDIV_STEP_VALUE       	( 500 )

/// define the default DCTQ times 
#define DFLT_DCTQ_100K_VAL    	( 0x09 )
#define DFLT_DCTQ_400K_VAL    	( 0x18 )

/// define the default status Interrupt Node Pointer
#define DFLT_CHAN0_IRQ_LVL    	( 2 )
#define DFLT_CHAN1_IRQ_LVL    	( 5 )

/// define the macros to allow mapping the interrupts
#define I2C_Irq_Handler0     		IRQ_Hdlr_11
#define I2C_Irq_Handler1     		IRQ_Hdlr_14

/// define the interrupts
#define I2C_IRQ_HAND0        		USIC0_2_IRQn
#define I2C_IRQ_HAND1        		USIC0_5_IRQn

/// define the transmit data formats
#define	TDF_SEND_DATA_BYTE			0
#define	TDF_RCVD_SEND_ACK				2
#define	TDF_RCVD_SEND_NAK				3
#define	TDF_SEND_START					4
#define	TDF_SEND_REPT_START			5
#define	TDF_SEND_STOP						6

/// define the data/TDF offsets for the transmit data
#define	XMT_DATA_OFFSET					LE_U16_LSB_IDX
#define	XMT_TDF_OFFSET					LE_U16_MSB_IDX

// enumerations ---------------------------------------------------------------
/// enumerate the I2C physical devices
typedef enum _PHYDEV
{
  PHY_I2C_DEV0 = 0,      ///< physical I2C 0
  PHY_I2C_DEV1,          ///< physical I2C 1
  PHY_I2C_MAX
} PHYDEV;

/// enumerate the bus direction
typedef enum _BUSDIR
{
	BUS_DIR_XMIT = 0,			///< transmit mode
	BUS_DIR_RECV					///< receive mode
} BUSDIR;

/// enumerate the baud rate fractional divider options
typedef enum _BGENFDIV
{
  BGEN_FDIV_OFF = 0,
  BGEN_FDIV_NRM,
  BGEN_FDIM_FRC,
} BGENFDIV;

/// define the all IRQ status masks
#define I2C_ALL_IRQ_STS_MASKS    ( USIC_CH_PSR_IICMode_SCR_Msk | USIC_CH_PSR_IICMode_PCR_Msk | USIC_CH_PSR_IICMode_NACK_Msk | USIC_CH_PSR_IICMode_ARL_Msk | \
                                   USIC_CH_PSR_IICMode_ERR_Msk | USIC_CH_PSR_IICMode_ACK_Msk | USIC_CH_PSR_IICMode_RIF_Msk | USIC_CH_PSR_IICMode_AIF_Msk )

/// define the IRQ status combos
#define I2C_IRQ_STS_SCRACK      ( USIC_CH_PSR_IICMode_SCR_Msk | USIC_CH_PSR_IICMode_ACK_Msk )

/// define the NAK on poll
#define I2C_IRQ_STS_PCRNAK      ( USIC_CH_PSR_IICMode_PCR_Msk | USIC_CH_PSR_IICMode_NACK_Msk )

// structures -----------------------------------------------------------------
/// define the local control structure
typedef struct _LCLCTLS
{
  USIC_CH_TypeDef*  ptI2cRegs;   	///< pointer to the UART registers
  U8                nInprLvl;  		///< interrupt level
  PU8     					pnData ;      ///< pointer to the data storage
  U8      					nXmtLen;      ///< transmit length
  U8      					nRcvLen;      ///< receive length
  U8      					nBufIdx;      ///< buffer index
  U8      					nSlvAddr;     ///< slave address
  I2CERR  					eError;       ///< current error
  BOOL   			 			bRunning;     ///< running state
  BUSDIR						eBusDir;			///< bus direction
  BOOL              bStartRcvd;   ///< start recieved
} LCLCTL, *PLCLCTL;
#define LCLCTLS_SIZE    sizeof( LCLCTLS )

/// define the physical to logical mapping structure
typedef struct _PHYMAP
{
  I2CDEVENUM eDev;   ///< logical enumeration
} PHYMAP, *PPHYMAP;
#define PHYMAP_SIZE  sizeof( PHYMAP )

// global parameter declarations ----------------------------------------------

// local parameter declarations -----------------------------------------------
static  LCLCTL  atLclCtls[ I2C_DEV_MAX ];
static  PHYMAP  atPhyMaps[ PHY_I2C_MAX ];

// global interrupt handlers
void  I2C_Irq_Handler0( void );
void  I2C_Irq_Handler1( void );

// local function prototypes --------------------------------------------------
static 	void		XmitNextByte( PLCLCTL ptCtl );
static	void		RecvNextByte( PLCLCTL ptCtl );
static	BOOL		WriteData( PLCLCTL ptCtl, U16 wData );
static  void    IrqCommonHandler( I2CDEVENUM eDev );
static  void    ProcessComplete( I2CERR eError, PI2CDEF ptDef, PLCLCTL ptCtl );

// constant parameter initializations -----------------------------------------

/******************************************************************************
 * @function I2c_Initialize
 *
 * @brief I2C initialization
 *
 * This function will initialize all defined devices
 *
 *****************************************************************************/
void I2c_Initialize( void )
{
  PI2CDEF     ptDef;
  PLCLCTL     ptCtl;
  U32         uTemp, uIrqLvl, uXmtPtr, uRcvPtr;
  PHYDEV      ePhyDev;
  I2CDEVENUM  eDev;
 
  // disable clock gating
  SCU_GENERAL->PASSWD = 0x000000C0UL;
  WR_REG( SCU_CLK->CLKCR, SCU_CLK_CLKCR_CNTADJ_Msk, SCU_CLK_CLKCR_CNTADJ_Pos, CLK_DELAY_COUNT );
  SET_BIT( SCU_CLK->CGATCLR0, SCU_CLK_CGATCLR0_USIC0_Pos );
  while (( SCU_CLK->CLKCR ) & ( SCU_CLK_CLKCR_VDDC2LOW_Msk ));
  SCU_GENERAL->PASSWD = 0x000000C3UL;

  // for each defined I2C
  for ( eDev = 0; eDev < I2C_DEV_MAX; eDev++ )
  {
    // get pointer to the definition/control structures
    ptDef = ( PI2CDEF )&atI2cDefs[ eDev ];
    ptCtl = &atLclCtls[ eDev ];

    // determine the port
    switch( ptDef->eChan )
    {
      case I2C_CHAN_SC00 :
        ptCtl->ptI2cRegs = USIC0_CH0;
        ptCtl->nInprLvl = DFLT_CHAN0_IRQ_LVL;
        uIrqLvl = I2C_IRQ_HAND0;
        ePhyDev = PHY_I2C_DEV0;
        uXmtPtr = DFLT_TXBUF_PTR_CH0;
        uRcvPtr = DFLT_RXBUF_PTR_CH0;
        break;

      case I2C_CHAN_SC01 :
        ptCtl->ptI2cRegs = USIC0_CH1;
        ptCtl->nInprLvl = DFLT_CHAN1_IRQ_LVL;
        uIrqLvl = I2C_IRQ_HAND1;
        ePhyDev = PHY_I2C_DEV1;
        uXmtPtr = DFLT_TXBUF_PTR_CH1;
        uRcvPtr = DFLT_RXBUF_PTR_CH1;
        break;

      default :
      	ePhyDev = PHY_I2C_DEV0;
        break;
    }

    // Enable the USIC Channel
    ptCtl->ptI2cRegs->KSCFG |= ( USIC_CH_KSCFG_MODEN_Msk | BIT( USIC_CH_KSCFG_BPMODEN_Pos ));
    uTemp = ptCtl->ptI2cRegs->KSCFG;

    // compute the step value and divider for the baud rate
    // ComputeBaudrate( ptDef->uBaudRate, &uStep, &uPdiv);

    // Configure the uFraction divider/step value
    ptCtl->ptI2cRegs->FDR |= BITS( BGEN_FDIM_FRC, USIC_CH_FDR_DM_Pos );
    ptCtl->ptI2cRegs->FDR |= (( 0x200 ) & USIC_CH_FDR_STEP_Msk );

    // configure the baud rate
    uTemp = BITS( DFLT_DCTQ_100K_VAL, USIC_CH_BRG_DCTQ_Pos );
    uTemp |= ( BITS( 0x0F, USIC_CH_BRG_PDIV_Pos ) & USIC_CH_BRG_PDIV_Msk );
    ptCtl->ptI2cRegs->BRG |= uTemp;

    // configure the shift control
    uTemp = DFLT_I2C_SCTR_SDR_VAL;
    uTemp |= BITS( DFLT_I2C_SCTR_TRM_VAL, USIC_CH_SCTR_TRM_Pos );
    uTemp |= BITS( DFLT_I2C_SCTR_WLE_VAL, USIC_CH_SCTR_WLE_Pos );
    uTemp |= BITS( DFLT_I2C_SCTR_FLE_VAL, USIC_CH_SCTR_FLE_Pos );
    ptCtl->ptI2cRegs->SCTR |= uTemp;

    // configure the transmit control/status register
    uTemp = BITS( DFLT_I2C_TCSR_TDEN_VAL, USIC_CH_TCSR_TDEN_Pos );
    uTemp |= BITS( DFLT_I2C_TCSR_TDSSM_VAL, USIC_CH_TCSR_TDSSM_Pos );
    ptCtl->ptI2cRegs->TCSR |= uTemp;

    // configure the protocol configuration register
    uTemp = ( ptDef->bFastBaud ) ? BIT( USIC_CH_PCR_IICMode_STIM_Pos ) : 0;
    uTemp |= USIC_CH_PCR_IICMode_SCRIEN_Msk | USIC_CH_PCR_IICMode_PCRIEN_Msk | USIC_CH_PCR_IICMode_NACKIEN_Msk;
    uTemp |= USIC_CH_PCR_IICMode_ACKIEN_Msk;
    //  USIC_CH_PCR_IICMode_ARLIEN_Msk | USIC_CH_PCR_IICMode_ERRIEN_Msk | 
    ptCtl->ptI2cRegs->PCR_IICMode = uTemp;

    // clear any pending interrupts
    ptCtl->ptI2cRegs->PSCR = I2C_ALL_IRQ_STS_MASKS;

    // configure the transmit buffer control register
    uTemp = BITS( DFLT_TXBUF_SIZE, USIC_CH_TBCTR_LIMIT_Pos );
    uTemp |= BITS( uXmtPtr, USIC_CH_TBCTR_DPTR_Pos );
    uTemp |= BITS( DFLT_TXBUF_SIZE, USIC_CH_TBCTR_SIZE_Pos );
    ptCtl->ptI2cRegs->TBCTR = uTemp;

    // configure the receive buffer control registers
    uTemp = BIT( USIC_CH_RBCTR_LOF_Pos );
    uTemp |= BITS( uRcvPtr, USIC_CH_RBCTR_DPTR_Pos );
    uTemp |= BITS( DFLT_RXBUF_SIZE, USIC_CH_RBCTR_SIZE_Pos );
    uTemp |= BIT( USIC_CH_RBCTR_SRBIEN_Pos );
    ptCtl->ptI2cRegs->RBCTR = uTemp;

    // configure the channel control register
    uTemp = DFLT_I2C_MODE_VAL;
    uTemp |= ( USIC_CH_CCR_RIEN_Msk | USIC_CH_CCR_AIEN_Msk );
    ptCtl->ptI2cRegs->CCR |= uTemp;

    // map the input mux
    ptCtl->ptI2cRegs->DX0CR = ptDef->eSdaMux;
    ptCtl->ptI2cRegs->DX1CR = ptDef->eSclMux;

    // now set the interrupt sources to point to one interrupt
    uTemp = BITS( ptCtl->nInprLvl, USIC_CH_INPR_PINP_Pos ) | BITS( ptCtl->nInprLvl, USIC_CH_INPR_RINP_Pos ) | BITS( ptCtl->nInprLvl, USIC_CH_INPR_AINP_Pos );
    ptCtl->ptI2cRegs->INPR |= uTemp;

    // store the interrupt
    atPhyMaps[ ePhyDev ].eDev = eDev;

    // register the interrupt/enable it
    NVIC_SetPriority( uIrqLvl, 3 );
    NVIC_EnableIRQ( uIrqLvl );
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
 * @param[in]   nSlvAddr    slave address
 * @param[in]   pnData      pointer to the data buffer
 * @param[in]   nXmtLen     size of the data buffer
 * @param[in]   uTimeout    timeout value in MSECS
 *
 * @return      appropriate error value
 *
 *****************************************************************************/
I2CERR I2c_Write( I2CDEVENUM eDev, U8 nSlvAddr, PU8 pnData, U8 nXmtLen , U32 uTimeout)
{
  I2CERR 	  eError = I2C_ERR_NONE;
  PI2CDEF 	ptDef;
  PLCLCTL 	ptCtl;
  U32     	uTime;
  U16UN			tXmtData;

  // check for a valid I2C
  if ( eDev < I2C_DEV_MAX )
  {
    // get pointer to the definition structure
    ptDef = ( PI2CDEF )&atI2cDefs[ eDev ];
    
		// copy the values to the local controls
		ptCtl = &atLclCtls[ eDev ];
		ptCtl->nSlvAddr = nSlvAddr;
		ptCtl->pnData = pnData;
		ptCtl->nXmtLen = nXmtLen;
		ptCtl->nBufIdx = 0;
		ptCtl->eBusDir = BUS_DIR_XMIT;
    ptCtl->bStartRcvd = FALSE;

		// send the slave with address with a start condition
		tXmtData.anValue[ XMT_TDF_OFFSET ] = TDF_SEND_START;
		tXmtData.anValue[ XMT_DATA_OFFSET ] = nSlvAddr << 1;

		// write the data
		if ( WriteData( ptCtl, tXmtData.wValue ))
		{
			// check for a blocking device
			if ( ptDef->pvCallBack == NULL )
			{
				// wait till done or timeout
				ptCtl->bRunning = TRUE;
				uTime = SystemTick_GetTimeMsec( ) + uTimeout;
				while( ptCtl->bRunning )
				{
					if ( uTimeout != 0 )
					{
						// check for timeout
						if ( SystemTick_GetTimeMsec( ) >= uTime )
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
			// set the error
			eError = I2C_ERR_SYSERR;
		}
	}
  else
  {
    // illegal device
    eError = I2C_ERR_ILLDEV;
  }

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
 * @param[in]   pnData      pointer to the data buffer
 * @param[in]	  nRcvLen	    expected length of the receive data
 * @param[in]   uTimeout    timeout value in MSECS
 *
 * @return      appropriate error value
 *
 *****************************************************************************/
I2CERR I2c_Read( I2CDEVENUM eDev, U8 nSlvAddr, PU8 pnData, U8 nRcvLen, U32 uTimeout )
{
  I2CERR  eError = I2C_ERR_NONE;
  PI2CDEF ptDef;
  PLCLCTL ptCtl;
  U32     uTime;
  U16UN		tXmtData;
  
  // check for a valid I2C
  if ( eDev < I2C_DEV_MAX )
  {
    // get pointers to the control/def structure
    ptDef = ( PI2CDEF )&atI2cDefs[ eDev ];
    ptCtl = &atLclCtls[ eDev ];
    
		// copy the values to the local controls
		ptCtl = &atLclCtls[ eDev ];
		ptCtl->nSlvAddr = nSlvAddr;
		ptCtl->pnData = pnData;
		ptCtl->nXmtLen = 0;
		ptCtl->nRcvLen = nRcvLen;
		ptCtl->nBufIdx = 0;
		ptCtl->eBusDir = BUS_DIR_RECV;
    ptCtl->bStartRcvd = FALSE;
      
		// send the slave with address with a start condition
		tXmtData.anValue[ XMT_TDF_OFFSET ] = TDF_SEND_START;
		tXmtData.anValue[ XMT_DATA_OFFSET ] = ( nSlvAddr << 1 ) | 0x01;

		// write the data
		if ( WriteData( ptCtl, tXmtData.wValue ))
		{
      // check for a blocking device
      if ( ptDef->pvCallBack == NULL )
      {
        // wait till done or timeout
        ptCtl->bRunning = TRUE;
        uTime = SystemTick_GetTimeMsec( ) + uTimeout;
        while( ptCtl->bRunning )
        {
          if ( uTimeout != 0 )
          {
            // check for timeout
            if ( SystemTick_GetTimeMsec( ) >= uTime )
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
			// set the error
			eError = I2C_ERR_SYSERR;
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
  I2CERR eError = I2C_ERR_NONE;
  
  // check for a valid I2C
  if ( eDev < I2C_DEV_MAX )
  {
    // process the action
    switch( eAction )
    {
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
 * @function I2C_Irq_Handler0
 *
 * @brief interrupt device 0 handler
 *
 * This function handles the interrupts from I2C 0
 *
 *****************************************************************************/
void I2C_Irq_Handler0( void )
{
  // call the common handler
  IrqCommonHandler( atPhyMaps[ PHY_I2C_DEV0 ].eDev );
}

/******************************************************************************
 * @function I2C_Irq_Handler1
 *
 * @brief interrupt device 1 handler
 *
 * This function handles the interrupts from I2C 1
 *
 *****************************************************************************/
void I2C_Irq_Handler1( void )
{
  // call the common handler
  IrqCommonHandler( atPhyMaps[ PHY_I2C_DEV1 ].eDev );
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
static void IrqCommonHandler( I2CDEVENUM eDev )
{
  U32         uStatus;
  PI2CDEF     ptDef;
  PLCLCTL     ptCtl;
  U16UN				tXmtData;
  
  // get the definition/control structure pointers
  ptDef = ( PI2CDEF )&atI2cDefs[ eDev ];
  ptCtl = &atLclCtls[ eDev ];

  // get the current interrupt status/clear the interrupt
  uStatus = ptCtl->ptI2cRegs->PSR_IICMode;
  uStatus &= I2C_ALL_IRQ_STS_MASKS;
  ptCtl->ptI2cRegs->PSCR = uStatus;
  
  // check for start received
  switch( uStatus )
  {
		case I2C_IRQ_STS_SCRACK :
      // set the start received flag
      ptCtl->bStartRcvd = TRUE;

			// determine the mode
			if ( ptCtl->eBusDir == BUS_DIR_XMIT )
			{
				// send the next byte or stop
				XmitNextByte( ptCtl );
			}
			else
			{
				// determine the first read command
				tXmtData.anValue[ XMT_TDF_OFFSET ] = ( ptCtl->nRcvLen == 1 ) ? TDF_RCVD_SEND_NAK : TDF_RCVD_SEND_ACK;
				tXmtData.anValue[ XMT_DATA_OFFSET ] = 0;
				WriteData( ptCtl, tXmtData.wValue );
			}
			break;

    case USIC_CH_PSR_IICMode_SCR_Msk :
      // set the start received flag
      ptCtl->bStartRcvd = TRUE;
      break;

		case USIC_CH_PSR_IICMode_ACK_Msk :
      // check for start received flag
      if ( ptCtl->bStartRcvd )
      {
        // determine the mode
        if ( ptCtl->eBusDir == BUS_DIR_XMIT )
        {
          // send the next byte or stop
          XmitNextByte( ptCtl );
        }
        else
        {
          // check for first read
          if ( ptCtl->nBufIdx == 0 )
          {
            // determine the first read command
            tXmtData.anValue[ XMT_TDF_OFFSET ] = ( ptCtl->nRcvLen == 1 ) ? TDF_RCVD_SEND_NAK : TDF_RCVD_SEND_ACK;
            tXmtData.anValue[ XMT_DATA_OFFSET ] = 0;
            WriteData( ptCtl, tXmtData.wValue );
          }
          else
          {
            // receive the next byte
            RecvNextByte( ptCtl );
          }
        }
      }
			break;

		case USIC_CH_PSR_IICMode_RIF_Msk :
		case USIC_CH_PSR_IICMode_AIF_Msk :
			// get the next received byte
			RecvNextByte( ptCtl );
			break;

		case USIC_CH_PSR_IICMode_PCR_Msk :
			ProcessComplete( I2C_ERR_NONE, ptDef, ptCtl );
			break;

		case I2C_IRQ_STS_PCRNAK :
      // send a stop
      tXmtData.anValue[ XMT_TDF_OFFSET ] = TDF_SEND_STOP;
      tXmtData.anValue[ XMT_DATA_OFFSET ] = 0;
      ProcessComplete( I2C_ERR_BUSNAK, ptDef, ptCtl );
			break;

		case USIC_CH_PSR_IICMode_ARL_Msk :
			ProcessComplete( I2C_ERR_ARBLOST, ptDef, ptCtl );
			break;

		case USIC_CH_PSR_IICMode_ERR_Msk :
			ProcessComplete( I2C_ERR_BUSERR, ptDef, ptCtl );
			break;

		default :
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
  
  // check to see if the callback is not null
  if ( ptDef->pvCallBack != NULL )
  {
    // call it
    ptDef->pvCallBack( eError );
  }
}

/******************************************************************************
 * @function XmitNextByte
 *
 * @brief process the next byte
 *
 * This function processes the next byte for transmit
 *
 * @param[in]   ptCtl			pointer to the control structure
 *
 *****************************************************************************/
static void	XmitNextByte( PLCLCTL ptCtl )
{
	U16UN	tXmtData;

	// check for more data
	if ( ptCtl->nXmtLen != 0 )
	{
		// send more data
		tXmtData.anValue[ XMT_TDF_OFFSET ] = TDF_SEND_DATA_BYTE;
		tXmtData.anValue[ XMT_DATA_OFFSET ] = *( ptCtl->pnData + ptCtl->nBufIdx++ );

		// decrement the length
		ptCtl->nXmtLen--;
	}
	else
	{
		// send a stop
		tXmtData.anValue[ XMT_TDF_OFFSET ] = TDF_SEND_STOP;
		tXmtData.anValue[ XMT_DATA_OFFSET ] = 0;
	}

  // write the data
  WriteData( ptCtl, tXmtData.wValue );
}

/******************************************************************************
 * @function RecvNextByte
 *
 * @brief process the next byte
 *
 * This function processes the next byte for both receive
 *
 * @param[in]   ptCtl			pointer to the control structure
 *
 *****************************************************************************/
static void	RecvNextByte( PLCLCTL ptCtl )
{
	U16UN	tXmtData;
  U8    nData;

	// read the data/increment the buffer index/decrement the length
  nData = ptCtl->ptI2cRegs->OUTR;
	ptCtl->pnData[ ptCtl->nBufIdx++ ] = nData;
  ptCtl->nRcvLen--;

	// check for more data
	if ( ptCtl->nRcvLen > 1 )
	{
		// send more receive ACK
		tXmtData.anValue[ XMT_TDF_OFFSET ] = TDF_RCVD_SEND_ACK;
		tXmtData.anValue[ XMT_DATA_OFFSET ] = 0;
	}
	else if ( ptCtl->nRcvLen == 1 )
	{
		// send receivve NAK
		tXmtData.anValue[ XMT_TDF_OFFSET ] = TDF_RCVD_SEND_NAK;
		tXmtData.anValue[ XMT_DATA_OFFSET ] = 0;
	}
	else
	{
		// send a stop
		tXmtData.anValue[ XMT_TDF_OFFSET ] = TDF_SEND_STOP;
		tXmtData.anValue[ XMT_DATA_OFFSET ] = 0;
	}

	// write the data
	WriteData( ptCtl, tXmtData.wValue );
}

/******************************************************************************
 * @function WriteData
 *
 * @brief write a data word to the fifo
 *
 * This function check to see if the fifo is empty and write a word to the fifo
 *
 * @param[in]   ptCtl			pointer to the control structure
 * @param[in]		wData			data to write
 *
 * @return      TRUE if data written, FALSE if not
 *
 *****************************************************************************/
static BOOL	WriteData( PLCLCTL ptCtl, U16 wData )
{
	BOOL	bStatus = FALSE;

	// check for fifo empty
	if ( !( ptCtl->ptI2cRegs->TRBSR & USIC_CH_TRBSR_TFULL_Msk ))
	{
		// write the data
		ptCtl->ptI2cRegs->IN[ 0 ] = wData;
		bStatus = TRUE;
	}

	return( bStatus );
}

/**@} EOF I2c.c */
