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

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// local parameter declarations -----------------------------------------------

// local function prototypes --------------------------------------------------
static  SercomSpi* GetSercomChannel( SPICHAN eChan );

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
  SercomSpi*            ptSpi;
  SERCOM_SPI_CTRLA_Type tCtrlA;

  // for each device, configure
  for ( eSpiDev = 0; eSpiDev < SPI_DEV_ENUM_MAX; eSpiDev++ )
  {
    // set the pointer
    ptSpiDef =( PSPIDEF )&atSpiDefs[ eSpiDev ];
    ptSpi = GetSercomChannel( ptSpiDef->eChan );

    // disable - just in case
    ptSpi->CTRLA.bit.SWRST = 1;
    while( ptSpi->SYNCBUSY.bit.SWRST );
    
    // enable the receiver 
    ptSpi->CTRLB.bit.RXEN = ON;

    // now set the baudrate
    ptSpi->BAUD.reg = ptSpiDef->nClockDivider;

    // now build the control A value
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
    ptSpi->CTRLA = tCtrlA;

    // now enable it
    ptSpi->CTRLA.bit.ENABLE = ON;
    while( ptSpi->SYNCBUSY.reg != 0 );
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
  SPIERR      eSpiErr = SPI_ERR_NONE;
  SercomSpi*  ptSpi;
  PSPIDEF     ptSpiDef;
 
  // check for valid device
  if ( eDev < SPI_DEV_ENUM_MAX )
  {
    // get the pointer to the channel
    ptSpiDef =( PSPIDEF )&atSpiDefs[ eDev ];
    ptSpi = GetSercomChannel( ptSpiDef->eChan );

    // wait for not busy
    while( !ptSpi->INTFLAG.reg & SERCOM_SPI_INTFLAG_DRE );

    // now write the value
    ptSpi->DATA.reg = nData;
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
 * @function Spi_Initialize
 *
 * @brief SPI initialization
 *
 * This function will initialize all SPI ports configured in the table
 *
 *****************************************************************************/
SPIERR Spi_Read( SPIDEVENUM eDev, U8 nXmtData, PU8 pnData )
{
  SPIERR      eSpiErr = SPI_ERR_NONE;
  SercomSpi*  ptSpi;
  PSPIDEF     ptSpiDef;

  // check for valid device
  if ( eDev < SPI_DEV_ENUM_MAX )
  {
    // get the pointer to the channel
    ptSpiDef =( PSPIDEF )&atSpiDefs[ eDev ];
    ptSpi = GetSercomChannel( ptSpiDef->eChan );

    // wait for not busy
    while( !( ptSpi->INTFLAG.reg & SERCOM_SPI_INTFLAG_DRE ));

    // now write the value
    ptSpi->DATA.reg = nXmtData;

    // wait for RX complete
    while( !( ptSpi->INTFLAG.reg & SERCOM_SPI_INTFLAG_RXC ));

    // now return the data
    *( pnData ) = ptSpi->DATA.reg;
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
 * @function Spi_Write
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
  SPIERR      eSpiErr = SPI_ERR_NONE;
  SercomSpi*  ptSpi;
  PSPIDEF     ptSpiDef;
  U16         wBufIdx;

  // check for valid device
  if ( eDev < SPI_DEV_ENUM_MAX )
  {
    // get the pointer to the channel
    ptSpiDef =( PSPIDEF )&atSpiDefs[ eDev ];
    ptSpi = GetSercomChannel( ptSpiDef->eChan );

    // for each byte in the buffer
    for ( wBufIdx = 0; wBufIdx < wSize; wBufIdx++ )
    {
      // wait for not busy
      while( !( ptSpi->INTFLAG.reg & SERCOM_SPI_INTFLAG_DRE ));

      // write it
      ptSpi->DATA.reg = *( pnBuffer + wBufIdx );

      // check for read
      if ( bStoreRead )
      {
        // wait for receive ready
        while( !( ptSpi->INTFLAG.reg & SERCOM_SPI_INTFLAG_RXC ));

        // now read the data
        *( pnBuffer + wBufIdx ) = ptSpi->DATA.reg;
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
 * @function Spi_Read
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
  SPIERR      eSpiErr = SPI_ERR_NONE;
  SercomSpi*  ptSpi;
  PSPIDEF     ptSpiDef;
  U16         wBufIdx;

  // check for valid device
  if ( eDev < SPI_DEV_ENUM_MAX )
  {
    // get the pointer to the channel
    ptSpiDef =( PSPIDEF )&atSpiDefs[ eDev ];
    ptSpi = GetSercomChannel( ptSpiDef->eChan );

    // for each byte in the buffer
    for ( wBufIdx = 0; wBufIdx < wSize; wBufIdx++ )
    {
      // wait for not busy
      while( !( ptSpi->INTFLAG.reg & SERCOM_SPI_INTFLAG_DRE ));

      // write it
      ptSpi->DATA.reg = nOutData;

      // wait for receive ready
      while( !( ptSpi->INTFLAG.reg & SERCOM_SPI_INTFLAG_RXC ));

      // now read the data
      *( pnBuffer + wBufIdx ) = ptSpi->DATA.reg;
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
  SPIERR      eSpiErr = SPI_ERR_NONE;
  SercomSpi*  ptSpi;
  U8          nTemp;
  PSPIDEF     ptSpiDef;

  // check for valid device
  if ( eDev < SPI_DEV_ENUM_MAX )
  {
    // get the pointer to the channel
    ptSpiDef =( PSPIDEF )&atSpiDefs[ eDev ];
    ptSpi = GetSercomChannel( ptSpiDef->eChan );

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
static SercomSpi* GetSercomChannel( SPICHAN eChan )
{
  Sercom* ptSercom = NULL;
  
  switch( eChan )
  {
    case SPI_CHAN_0 :
      ptSercom = SERCOM0;
      break;
      
    case SPI_CHAN_1 :
      ptSercom = SERCOM1;
      break;
    
    #ifdef SERCOM2
    case SPI_CHAN_2:
      ptSercom = SERCOM2;
      break;
    #endif // SERCOM2
    
    #ifdef SERCOM3
    case SPI_CHAN_3 :
      ptSercom = SERCOM3;
      break;
    #endif // SERCOM3
    
    #ifdef SERCOM4
    case SPI_CHAN_4 :
      ptSercom = SERCOM4;
      break;
    #endif // SERCOM4
    
    #ifdef SERCOM5
    case SPI_CHAN_5 :
      ptSercom = SERCOM5;
      break;
    #endif // SERCOM5
    
    default :
      ptSercom = NULL;
      break;
  }
  
  // return the pointer to the channlel
  return( &ptSercom->SPI );
}

/**@} EOF Spi.c */
