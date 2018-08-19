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
  // make it a master
  SPCR = BIT( MSTR ) | BIT( SPE );
}

/******************************************************************************
 * @function Spi_Initialize
 *
 * @brief SPI initialization
 *
 * This function will initialize all SPI ports configured in the table
 *
 *****************************************************************************/
void Spi_Write( U8 nData )
{
  /// output data/wait till not busy
  SPDR = nData;
  while( !( SPSR & BIT( SPIF )));
}

/******************************************************************************
 * @function Spi_Initialize
 *
 * @brief SPI initialization
 *
 * This function will initialize all SPI ports configured in the table
 *
 *****************************************************************************/
U8 Spi_Read( U8 nData )
{
  // output data
  SPDR = nData;
  while( !( SPSR & BIT( SPIF )));
  return( SPDR );
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
void Spi_WriteBlock( PU8 pnBuffer, U16 wSize, BOOL bStoreRead )
{
  U16     wIdx;
  
  // for each byte
  for( wIdx = 0; wIdx < wSize; wIdx++ )
  {
    // output a byte
    SPDR = *( pnBuffer + wIdx );
      
    // wait till not busy
    while( !( SPSR & BIT( SPIF )));
      
    // do we want to read the data
    if ( bStoreRead )
    {
      // store the data
      *( pnBuffer + wIdx ) = SPDR;
    }
  }
}

/******************************************************************************
 * @function Spi_Write
 *
 * @brief write data to the SPI port
 *
 * This function will write the prescribed number of bytes to the SPI
 *
 * @param[in]   eSpiSel   SPI selection enumeration
 * @param[io]   pnBuffer  pointer to the read/write buffer
 * @param[in]   wSize     number of bytes to write
 * @param[in]   bStoreRead  TRUE, store the read data
 *
 * @return  error   appropriate error value
 *
 *****************************************************************************/
void Spi_WriteBlockP( PFU8 pnBuffer, U16 wSize, BOOL bStoreRead )
{
  PU8     pnLclBuffer;
  
  // attempt to allocate some local space  
  if (( pnLclBuffer = malloc( wSize )) != NULL )
  {
    // copy the data
    memcpy_P( pnLclBuffer, pnBuffer, wSize );
    
    // write it normally
    Spi_WriteBlock( pnLclBuffer, wSize, bStoreRead );
    
    // free buffer
    free( pnLclBuffer );
  }
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
void Spi_ReadBlock( U8 nOutData, PU8 pnBuffer, U16 wSize )
{
  U16     wIdx;
  
  // for each byte
  for ( wIdx = 0; wIdx < wSize; wIdx++ )
  {
    // output a byte
    SPDR = nOutData;
      
    // wait till not busy
    while( !( SPSR & BIT( SPIF )));
      
    // get the return value
    *( pnBuffer + wIdx ) = SPDR;
  }
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
SPIERR Spi_Ioctl( SPIIOCTLACTIONS eAction, PVOID pvValue )
{
  SPIERR  eSpiErr = SPI_ERR_NONE;
  U8      nTemp;
  
  // get the arugment
  nTemp = *((PU32)pvValue );
    
  // determine action
  switch( eAction )
  {
    case SPI_IOCTLACTIONS_SETPOL :
      // modify the polarity
      SPCR &= ~BIT( SPE );
      SPCR &= ~BIT( CPOL );
      SPCR |= BITS( nTemp, CPOL );
      SPCR |= BIT( SPE );
      break;
        
    case SPI_IOCTLACTIONS_SETPHA :
      // modify the phase
      SPCR &= ~BIT( SPE );
      SPCR &= ~BIT( CPHA );
      SPCR |= BITS( nTemp, CPHA );
      SPCR |= BIT( SPE );
      break;
        
    case SPI_IOCTLACTIONS_SETSPEED :
      // modify the speed
      SPCR &= ~BIT( SPE );
      SPCR &= ~( BIT( SPR0 ) | BIT( SPR1 ));
      SPCR |= (nTemp & 0x03 );
      SPSR |= ( nTemp > 3 ) ? BIT( SPI2X ) : 0;
      SPCR |= BIT( SPE );
      break;
        
    case SPI_IOCTLACTION_ENABLEDISABLE :
      if ( nTemp )
      {
        // enable the SPI
        SPCR |= BIT( SPE );
      }
      else
      {
        // disable the SPI
        SPCR &= ~BIT( SPE );
      }
      break;
        
    default :
      eSpiErr = SPI_ERR_ILLACT;
      break;
  }

  // return the error
  return( eSpiErr ); 
}

/******************************************************************************
 * @function Spi_Close
 *
 * @brief close the SPI port
 *
 * This function will turn off the SPI port
 *
 * @param[in]   eSpiSel   SPI selection enumeration
 *
 * @return  error   appropriate error value
 *
 *****************************************************************************/
void Spi_Close( void )
{
  // turn of the SPI
  SPCR &= ~BIT( SPE );
}

/**@} EOF Spi.c */
