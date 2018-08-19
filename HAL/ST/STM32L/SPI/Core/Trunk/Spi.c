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
static  SPI_TypeDef* GetSpiPointer( SPICHAN eChan );

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
  SPIDEVENUM    eSpiDev;
  PSPIDEF       ptSpiDef;
  SPI_TypeDef*  ptSpi;
  U32           uReg;

  // for each device, configure
  for ( eSpiDev = 0; eSpiDev < SPI_DEV_ENUM_MAX; eSpiDev++ )
  {
    // set the pointer
    ptSpiDef =( PSPIDEF )&atSpiDefs[ eSpiDev ];
    ptSpi = GetSpiPointer( ptSpiDef->eChan );

    // build the CR1 register value
    uReg = BITS( ptSpiDef->eMode, SPI_CR1_MSTR_Pos );
    uReg |= BITS( ptSpiDef->eClkPol, SPI_CR1_CPOL_Pos );
    uReg |= BITS( ptSpiDef->eClkPhase, SPI_CR1_CPHA_Pos );
    uReg |= BITS( ptSpiDef->eBitOrder, SPI_CR1_LSBFIRST_Pos );
    uReg |= BITS( ptSpiDef->ePrescale, SPI_CR1_BR_Pos );
    
    // now write it
    ptSpi->CR1 = uReg;
    
    // build the CR2 register value
    ptSpi->CR2 = 0;
  }
}

/******************************************************************************
 * @function Spi_Write
 *
 * @brief write a single byte
 *
 * This function will write a single byte to the device
 *
 * @param[in]    eDev     SPI selection enumeration
 * @param[in]    nData    data to write
 *
 * @return      appropriate error
 *
 *****************************************************************************/
SPIERR Spi_Write( SPIDEVENUM eDev, U8 nData )
{
  SPIERR      eSpiErr = SPI_ERR_NONE;
  SPI_TypeDef*  ptSpi;
  PSPIDEF     ptSpiDef;
 
  // check for valid device
  if ( eDev < SPI_DEV_ENUM_MAX )
  {
    // get the pointer to the channel
    ptSpiDef =( PSPIDEF )&atSpiDefs[ eDev ];
    ptSpi = GetSpiPointer( ptSpiDef->eChan );

    // now enable the spi
    ptSpi->CR1 |= SPI_CR1_SPE;
    
    // wait for not busy
    while( !( ptSpi->SR & SPI_SR_TXE ));

    // now write the value
    ptSpi->DR = nData;
    
    // disable the SPI
    ptSpi->CR1 &= ~SPI_CR1_SPE;
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
 * @brief read a single byte
 *
 * This function will read a single byte to the device
 *
 * @param[in]     eDev      SPI selection enumeration
 * @param[in]     nXmtData  data to be sent to cause a read
 * @param[in]     pnData    pointer to store data read
 *
 * @return      appropriate error
 *
 *****************************************************************************/
SPIERR Spi_Read( SPIDEVENUM eDev, U8 nXmtData, PU8 pnData )
{
  SPIERR        eSpiErr = SPI_ERR_NONE;
  SPI_TypeDef*  ptSpi;
  PSPIDEF       ptSpiDef;

  // check for valid device
  if ( eDev < SPI_DEV_ENUM_MAX )
  {
    // get the pointer to the channel
    ptSpiDef =( PSPIDEF )&atSpiDefs[ eDev ];
    ptSpi = GetSpiPointer( ptSpiDef->eChan );

    // now enable the spi
    ptSpi->CR1 |= SPI_CR1_SPE;
    
    // wait for not busy
    while( !( ptSpi->SR & SPI_SR_TXE ));

    // now write the value
    ptSpi->DR = nXmtData;

    // wait for RX complete
    while( !( ptSpi->SR & SPI_SR_RXNE ));

    // now return the data
    *( pnData ) = ptSpi->DR;
    
    // disable the SPI
    ptSpi->CR1 &= ~SPI_CR1_SPE;
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
 * @function Spi_WriteBlock
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
  SPI_TypeDef*  ptSpi;
  PSPIDEF     ptSpiDef;
  U16         wBufIdx;

  // check for valid device
  if ( eDev < SPI_DEV_ENUM_MAX )
  {
    // get the pointer to the channel
    ptSpiDef =( PSPIDEF )&atSpiDefs[ eDev ];
    ptSpi = GetSpiPointer( ptSpiDef->eChan );

    // now enable the spi
    ptSpi->CR1 |= SPI_CR1_SPE;
    
    // for each byte in the buffer
    for ( wBufIdx = 0; wBufIdx < wSize; wBufIdx++ )
    {
      // wait for not busy
      while( !( ptSpi->SR & SPI_SR_TXE ));

      // now write the value
      ptSpi->DR = *( pnBuffer + wBufIdx );

      // check for read
      if ( bStoreRead )
      {
        // wait for RX complete
        while( !( ptSpi->SR & SPI_SR_RXNE ));

        // now return the data
        *( pnBuffer + wBufIdx ) = ptSpi->DR;
      }
    }
    
    // disable the SPI
    ptSpi->CR1 &= ~SPI_CR1_SPE;
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
 * @function Spi_ReadBlock
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
  SPIERR        eSpiErr = SPI_ERR_NONE;
  SPI_TypeDef*  ptSpi;
  PSPIDEF       ptSpiDef;
  U16           wBufIdx;

  // check for valid device
  if ( eDev < SPI_DEV_ENUM_MAX )
  {
    // get the pointer to the channel
    ptSpiDef =( PSPIDEF )&atSpiDefs[ eDev ];
    ptSpi = GetSpiPointer( ptSpiDef->eChan );

    // now enable the spi
    ptSpi->CR1 |= SPI_CR1_SPE;
    
    // for each byte in the buffer
    for ( wBufIdx = 0; wBufIdx < wSize; wBufIdx++ )
    {
      // wait for not busy
      while( !( ptSpi->SR & SPI_SR_TXE ));

      // now write the value
      ptSpi->DR = nOutData;

        // wait for RX complete
        while( !( ptSpi->SR & SPI_SR_RXNE ));

        // now return the data
        *( pnBuffer + wBufIdx ) = ptSpi->DR;
    }

    // disable the SPI
    ptSpi->CR1 &= ~SPI_CR1_SPE;
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
  SPI_TypeDef*  ptSpi;
  U8          nTemp;
  PSPIDEF     ptSpiDef;

  // check for valid device
  if ( eDev < SPI_DEV_ENUM_MAX )
  {
    // get the pointer to the channel
    ptSpiDef =( PSPIDEF )&atSpiDefs[ eDev ];
    ptSpi = GetSpiPointer( ptSpiDef->eChan );

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
 * @function GetSpiPointer
 *
 * @brief gets a pointer to the control structure
 *
 * This function will return a pointer to the correct control structure
 * for a given channel
 *
 * @param[in]   eChan			SERCOM channel
 *
 * @return      pointer to a SPI_TypeDefI2c or NULL if illegal channel
 *
 *****************************************************************************/
static SPI_TypeDef* GetSpiPointer( SPICHAN eChan )
{
  SPI_TypeDef* ptSPI = NULL;
  
  switch( eChan )
  {
    case SPI_CHAN_1 :
      ptSPI = SPI1;
      break;
    
    case SPI_CHAN_2:
      ptSPI = SPI2;
      break;
    
    case SPI_CHAN_3 :
      ptSPI = SPI3;
      break;
    
    default :
      ptSPI = NULL;
      break;
  }
  
  // return the pointer to the channlel
  return( &ptSPI );
}

/**@} EOF Spi.c */
