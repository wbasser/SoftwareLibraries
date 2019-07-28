/******************************************************************************
 * @file Spi.c
 *
 * @brief SPI module implementation
 *
 * This file provides the SPI module implementation
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
 * \addtogroup SPI
 * @{
 *****************************************************************************/

// local includes -------------------------------------------------------------
#include "SPI/Spi.h"

// library includes -----------------------------------------------------------
#include "Clock/Clock.h"

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------
typedef struct _LCLCTL
{
  SercomSpi*    ptSpi;
  CLOCKMUXID    eClkMuxId;
} LCLCTL, *PLCLCTL;
#define LCLCTL_SIZE                     sizeof( LCLCTL )

// local parameter declarations -----------------------------------------------
static  LCLCTL  atLclCtls[ SPI_DEV_ENUM_MAX ];

// local function prototypes --------------------------------------------------
static  void GetSercomChannel( SPICHAN eChan, PLCLCTL ptLclCtl );

/******************************************************************************
 * @function Spi_Initialize
 *
 * @brief SPI initialization
 *
 * This function will initialize all SPI ports configured in the table
 *
 *****************************************************************************/
void Spi_Initialize( void )
{
  SPIDEVENUM            eSpiDev;
  PSPIDEF               ptSpiDef;
  PLCLCTL               ptLclCtl;
  SERCOM_SPI_CTRLA_Type tCtrlA;

  // for each device, configure
  for ( eSpiDev = 0; eSpiDev < SPI_DEV_ENUM_MAX; eSpiDev++ )
  {
    // set the pointer
    ptSpiDef = ( PSPIDEF )&g_atSpiDefs[ eSpiDev ];
    ptLclCtl = &atLclCtls[ eSpiDev ];
    GetSercomChannel( ptSpiDef->eChan, ptLclCtl );

    // disable - just in case
    ptLclCtl->ptSpi->CTRLA.bit.SWRST = 1;
    while( ptLclCtl->ptSpi->SYNCBUSY.bit.SWRST );
    
    // enable the receiver 
    ptLclCtl->ptSpi->CTRLB.bit.RXEN = ON;

    // now set the baudrate
    ptLclCtl->ptSpi->BAUD.reg = Clock_GetMultiplexerFreq( ptLclCtl->eClkMuxId ) / ptSpiDef->uBaudrate;

    // now build the control A value
    tCtrlA.reg = SERCOM_SPI_CTRLA_FORM( 0 );
    tCtrlA.bit.DORD = ptSpiDef->eBitOrder;
    tCtrlA.bit.CPOL = ptSpiDef->eClkPol;
    tCtrlA.bit.CPHA = ptSpiDef->eClkPhase;

    // now select the pin modes
    switch( ptSpiDef->ePinConfig )
    {
      case SPI_PINCONFIG_MISO0_MOSI3_SCLK1 :
        tCtrlA.bit.DIPO = 0;
        tCtrlA.bit.DOPO = 1;
        break;

      case SPI_PINCONFIG_MISO1_MOSI2_SCLK3 :
        tCtrlA.bit.DIPO = 1;
        tCtrlA.bit.DOPO = 1;
        break;

      case SPI_PINCONFIG_MISO1_MOSI0_SCLK3 :
        tCtrlA.bit.DIPO = 1;
        tCtrlA.bit.DOPO = 3;
        break;

      case SPI_PINCONFIG_MISO2_MOSI0_SCLK1 :
        tCtrlA.bit.DIPO = 2;
        tCtrlA.bit.DOPO = 0;
        break;

      case SPI_PINCONFIG_MISO2_MOSI3_SCLK1 :
        tCtrlA.bit.DIPO = 2;
        tCtrlA.bit.DOPO = 2;
        break;

      case SPI_PINCONFIG_MISO2_MOSI0_SCLK3 :
        tCtrlA.bit.DIPO = 2;
        tCtrlA.bit.DOPO = 3;
        break;

      case SPI_PINCONFIG_MISO3_MOSI0_SCLK1 :
        tCtrlA.bit.DIPO = 3;
        tCtrlA.bit.DOPO = 0;
        break;

      case SPI_PINCONFIG_MISO0_MOSI2_SCLK3 :
      default :
        tCtrlA.bit.DIPO = 0;
        tCtrlA.bit.DOPO = 1;
        break;
    }

    // set the mode
    tCtrlA.bit.MODE = ( ptSpiDef->eMode == SPI_MODE_MASTER ) ? SERCOM_SPI_CTRLA_MODE_SPI_MASTER_Val : SERCOM_SPI_CTRLA_MODE_SPI_SLAVE_Val;

    // now write the control register
    ptLclCtl->ptSpi->CTRLA = tCtrlA;

    // now enable it
    ptLclCtl->ptSpi->CTRLA.bit.ENABLE = ON;
    while( ptLclCtl->ptSpi->SYNCBUSY.reg != 0 );
  }
}

/******************************************************************************
 * @function Spi_Write
 *
 * @brief write a single byte
 *
 * This function will write a single byte to the device
 *
 * @param[in]     eDev    SPI selection enumeration
 * @param[in]    nData    data to write
 *
 * @return      appropriate error
 *
 *****************************************************************************/
SPIERR Spi_Write( SPIDEVENUM eDev, U8 nData )
{
  SPIERR  eSpiErr = SPI_ERR_NONE;
  PLCLCTL ptLclCtl;
 
  // check for valid device
  if ( eDev < SPI_DEV_ENUM_MAX )
  {
    // get the pointer to the channel
    ptLclCtl = &atLclCtls[ eDev ];

    // wait for not busy
    while( !ptLclCtl->ptSpi->INTFLAG.reg & SERCOM_SPI_INTFLAG_DRE );

    // now write the value
    ptLclCtl->ptSpi->DATA.reg = nData;
  }
  else
  {
    // set the error
    eSpiErr = SPI_ERR_ILLDEV;
  }

  // return the error
  return( eSpiErr ); 
}

/******************************************************************************
 * @function Spi_Read
 *
 * @brief read one byte
 *
 * @param[in]     eDev      SPI selection enumeration
 * @param[in]     nXmtData  data to write
 * @param[in]     pnRcvData pointer to store the receive data in
 *
 * @return      appropriate error
 *
 *****************************************************************************/
SPIERR Spi_Read( SPIDEVENUM eDev, U8 nXmtData, PU8 pnRcvData )
{
  SPIERR  eSpiErr = SPI_ERR_NONE;
  PLCLCTL ptLclCtl;

  // check for valid device
  if ( eDev < SPI_DEV_ENUM_MAX )
  {
    // get the pointer to the channel
    ptLclCtl = &atLclCtls[ eDev ];

    // wait for not busy
    while( !( ptLclCtl->ptSpi->INTFLAG.reg & SERCOM_SPI_INTFLAG_DRE ));

    // now write the value
    ptLclCtl->ptSpi->DATA.reg = nXmtData;

    // wait for done
    while( !( ptLclCtl->ptSpi->INTFLAG.reg & SERCOM_SPI_INTFLAG_TXC ));

    // wait for RX complete
    while( !( ptLclCtl->ptSpi->INTFLAG.reg & SERCOM_SPI_INTFLAG_RXC ));

    // now return the data
    *( pnRcvData ) = ptLclCtl->ptSpi->DATA.reg;
  }
  else
  {
    // set the error
    eSpiErr = SPI_ERR_ILLDEV;
  }

  // return the error
  return( eSpiErr ); 
}

/******************************************************************************
 * @function Spi_WriteBLock
 *
 * @brief write data to the SPI port
 *
 * This function will write the prescribed number of bytes to the SPI
 *
 * @param[in]   eSpiSel     SPI selection enumeration
 * @param[io]   pnBuffer    pointer to the read/write buffer
 * @param[in]   wSize       number of bytes to write
 * @param[in]   bStoreRead  TRUE, storsnarf7550
 e
 *
 *****************************************************************************/
SPIERR Spi_WriteBlock( SPIDEVENUM eDev, PU8 pnBuffer, U16 wSize, BOOL bStoreRead )
{
  SPIERR  eSpiErr = SPI_ERR_NONE;
  PLCLCTL ptLclCtl;
  U16     wBufIdx;

  // check for valid device
  if ( eDev < SPI_DEV_ENUM_MAX )
  {
    // get the pointer to the channel
    ptLclCtl = &atLclCtls[ eDev ];

    // for each byte in the buffer
    for ( wBufIdx = 0; wBufIdx < wSize; wBufIdx++ )
    {
      // wait for not busy
      while( !( ptLclCtl->ptSpi->INTFLAG.reg & SERCOM_SPI_INTFLAG_DRE ));

      // write it
      ptLclCtl->ptSpi->DATA.reg = *( pnBuffer + wBufIdx );

      // check for read
      if ( bStoreRead )
      {
        // wait for receive ready
        while( !( ptLclCtl->ptSpi->INTFLAG.reg & SERCOM_SPI_INTFLAG_RXC ));

        // now read the data
        *( pnBuffer + wBufIdx ) = ptLclCtl->ptSpi->DATA.reg;
      }
    }
  }
  else
  {
    // set the error
    eSpiErr = SPI_ERR_ILLDEV;
  }

  // return the error
  return( eSpiErr ); 
}

/******************************************************************************
 * @function Spi_ReadBLock
 *
 * @brief read data from the SPI bus
 *
 * This function will read the proscribed number of bytes from the bus
 *
 * @param[in]   eSpiSel   SPI selection enumeration
 * @param[in]   nOutDat   data to send to force read
 * @param[io]   pnBuffer  pointer to the store the data read
 * @param[in]   wSize     number of bytes to read
 *
 * @return  error   appropriate error value
 *
 *****************************************************************************/
SPIERR Spi_ReadBlock( SPIDEVENUM eDev, U8 nOutData, PU8 pnBuffer, U16 wSize )
{
  SPIERR  eSpiErr = SPI_ERR_NONE;
  PLCLCTL ptLclCtl;
  U16     wBufIdx;

  // check for valid device
  if ( eDev < SPI_DEV_ENUM_MAX )
  {
    // get the pointer to the channel
    ptLclCtl = &atLclCtls[ eDev ];

    // for each byte in the buffer
    for ( wBufIdx = 0; wBufIdx < wSize; wBufIdx++ )
    {
      // wait for not busy
      while( !( ptLclCtl->ptSpi->INTFLAG.reg & SERCOM_SPI_INTFLAG_DRE ));

      // write it
      ptLclCtl->ptSpi->DATA.reg = nOutData;

      // wait for receive ready
      while( !( ptLclCtl->ptSpi->INTFLAG.reg & SERCOM_SPI_INTFLAG_RXC ));

      // now read the data
      *( pnBuffer + wBufIdx ) = ptLclCtl->ptSpi->DATA.reg;
    }
  }
  else
  {
    // set the error
    eSpiErr = SPI_ERR_ILLDEV;
  }

  // return the error
  return( eSpiErr ); 
}

/******************************************************************************
 * @function Spi_Ioctl
 *
 * @brief ioctl control of the SPI
 *
 * This function will modifiy the SPI according to the action
 *
 * @param[in]   eSpiSel   SPI selection enumeration
 * @param[in]   eAction   SPI IOCTL action
 * @param[io]   pvValue   pointer to the passed value
 *
 * @return  error   appropriate error value
 *
 *****************************************************************************/
SPIERR Spi_Ioctl( SPIDEVENUM eDev, SPIIOCTLACTIONS eAction, PVOID pvValue )
{
  SPIERR  eSpiErr = SPI_ERR_NONE;
  PLCLCTL ptLclCtl;
  U8      nTemp;

  // check for valid device
  if ( eDev < SPI_DEV_ENUM_MAX )
  {
    // get the pointer to the channel
    ptLclCtl = &atLclCtls[ eDev ];

    // get the arugment
    nTemp = *((PU32)pvValue );
    
    // determine action
    switch( eAction )
    {
      case SPI_IOCTLACTIONS_SETPOL :
        break;
        
      case SPI_IOCTLACTIONS_SETPHA :
        break;
        
      case SPI_IOCTLACTIONS_SETSPEED :
        break;
        
      case SPI_IOCTLACTION_ENABLEDISABLE :
        break;
        
      default :
        eSpiErr = SPI_ERR_ILLACT;
        break;
    }
  }
  else
  {
    // set the error
    eSpiErr = SPI_ERR_ILLDEV;
  }

  // return the error
  return( eSpiErr ); 
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
static void GetSercomChannel( SPICHAN eChan, PLCLCTL ptLclCtl )
{
  switch( eChan )
  {
    case SPI_CHAN_0 :
      ptLclCtl->ptSpi = &SERCOM0->SPI;
      ptLclCtl->eClkMuxId = CLOCK_MUXID_SERCOM_0;
      break;
      
    case SPI_CHAN_1 :
      ptLclCtl->ptSpi = &SERCOM1->SPI;
      ptLclCtl->eClkMuxId = CLOCK_MUXID_SERCOM_1;
      break;
    
    #ifdef SERCOM2
    case SPI_CHAN_2:
      ptLclCtl->ptSpi = &SERCOM2->SPI;
      ptLclCtl->eClkMuxId = CLOCK_MUXID_SERCOM_2;
      break;
    #endif // SERCOM2
    
    #ifdef SERCOM3
    case SPI_CHAN_3 :
      ptLclCtl->ptSpi = &SERCOM3->SPI;
      ptLclCtl->eClkMuxId = CLOCK_MUXID_SERCOM_3;
      break;
    #endif // SERCOM3
    
    #ifdef SERCOM4
    case SPI_CHAN_4 :
      ptLclCtl->ptSpi = &SERCOM4->SPI;
      ptLclCtl->eClkMuxId = CLOCK_MUXID_SERCOM_4;
      break;
    #endif // SERCOM4
    
    #ifdef SERCOM5
    case SPI_CHAN_5 :
      ptLclCtl->ptSpi = &SERCOM5->SPI;
      ptLclCtl->eClkMuxId = CLOCK_MUXID_SERCOM_5;
      break;
    #endif // SERCOM5
    
    default :
      break;
  }
}

/**@} EOF Spi.c */
