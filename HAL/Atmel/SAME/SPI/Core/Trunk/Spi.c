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

// library includes ---------------------------------------------------------
#include "Clock/Clock.h"

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// local parameter declarations -----------------------------------------------

// local function prototypes --------------------------------------------------
static  void  SetupRegisters( Spi* ptSpi, PSPIDEF ptDef );
static  Spi*  GetSpiChannel( SPICHAN eChan );

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
  PSPIDEF     ptSpiDef;
  Spi*        ptSpi;
 
  // check for valid device
  if ( eDev < SPI_DEV_ENUM_MAX )
  {
    // get the pointer to the channel
    ptSpiDef =( PSPIDEF )&atSpiDefs[ eDev ];
    ptSpi = GetSpiChannel( ptSpiDef->eChan );
    
    // set up the registers
    SetupRegisters( ptSpi, ptSpiDef );

    // wait for ready/send the data
    while( !( ptSpi->SPI_SR & SPI_SR_TDRE ));
    ptSpi->SPI_TDR = nData;
      
    // release the chip select
    ptSpi->SPI_MR = SPI_MR_PCS( 0x0F );
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
 * @brief SPI read
 *
 * This function will initialize all SPI ports configured in the table
 *
 *****************************************************************************/
SPIERR Spi_Read( SPIDEVENUM eDev, U8 nXmtData, PU8 pnData )
{
  SPIERR      eSpiErr = SPI_ERR_NONE;
  PSPIDEF     ptSpiDef;
  Spi*        ptSpi;

  // check for valid device
  if ( eDev < SPI_DEV_ENUM_MAX )
  {
    // get the pointer to the channel
    ptSpiDef =( PSPIDEF )&atSpiDefs[ eDev ];
    ptSpi = GetSpiChannel( ptSpiDef->eChan );
    
    // set up the registers
    SetupRegisters( ptSpi, ptSpiDef );

    // wait for ready/send the data
    while( !( ptSpi->SPI_SR & SPI_SR_TDRE ));
    ptSpi->SPI_TDR = nXmtData;
    
    // wait for read
    while( !( ptSpi->SPI_SR & SPI_SR_RDRF ));
    *( pnData ) = ptSpi->SPI_RDR;
    
    // release the chip select
    ptSpi->SPI_MR = SPI_MR_PCS( 0x0F );
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
  PSPIDEF     ptSpiDef;
  Spi*        ptSpi;
  U16         wIdx;

  // check for valid device
  if ( eDev < SPI_DEV_ENUM_MAX )
  {
    // get the pointer to the channel
    ptSpiDef =( PSPIDEF )&atSpiDefs[ eDev ];
    ptSpi = GetSpiChannel( ptSpiDef->eChan );
    SetupRegisters( ptSpi, ptSpiDef );
    
    // now output the data
    for( wIdx = 0; wIdx < wSize; wIdx++ )
    {
      // wait for ready/send the data
      while( !( ptSpi->SPI_SR & SPI_SR_TDRE ));
      ptSpi->SPI_TDR = *( pnBuffer + wIdx );
      
      // if store read
      if ( bStoreRead )
      {
        // wait for receive/get the data
        while( !( ptSpi->SPI_SR & SPI_SR_RDRF ));
        *( pnBuffer + wIdx ) = ptSpi->SPI_RDR;
      }
    }
    
    // release the chip select
    ptSpi->SPI_MR = SPI_MR_PCS( 0x0F );
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
  SPIERR      eSpiErr = SPI_ERR_NONE;
  PSPIDEF     ptSpiDef;
  Spi*        ptSpi;
  U16         wIdx;

  // check for valid device
  if ( eDev < SPI_DEV_ENUM_MAX )
  {
    // get the pointer to the channel
    ptSpiDef =( PSPIDEF )&atSpiDefs[ eDev ];
    ptSpi = GetSpiChannel( ptSpiDef->eChan );
    SetupRegisters( ptSpi, ptSpiDef );

    // now output the data
    for( wIdx = 0; wIdx < wSize; wIdx++ )
    {
      // wait for ready/send the data
      while( !( ptSpi->SPI_SR & SPI_SR_TDRE ));
      ptSpi->SPI_TDR = nOutData;
      
      // wait for receive/ read the data
      while( !( ptSpi->SPI_SR & SPI_SR_RDRF ));
      *( pnBuffer + wIdx ) = ptSpi->SPI_RDR;
    }
    
    // release the chip select
    ptSpi->SPI_MR = SPI_MR_PCS( 0x0F );
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
  U8          nTemp;
  PSPIDEF     ptSpiDef;
  Spi*        ptSpi;

  // check for valid device
  if ( eDev < SPI_DEV_ENUM_MAX )
  {
    // get the pointer to the channel
    ptSpiDef =( PSPIDEF )&atSpiDefs[ eDev ];
    ptSpi = GetSpiChannel( ptSpiDef->eChan );

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
 * @function SetupRegisters
 *
 * @brief setsup the registers for a transer
 *
 * This function will initialize the control structure for a transfer
 *
 * @param[in]   ptSpi         pointer to the SPI
 * @param[in]   ptDef         pointer to the definitions structure
 *
 *****************************************************************************/
static void SetupRegisters( Spi* ptSpi, PSPIDEF ptDef )
{
  U32 uTempVal, uPeriphClock;
  // enable it
  ptSpi->SPI_CR = SPI_CR_SPIEN;
  
  // calulate the baudrate
  uPeriphClock = Clock_GetMastClockFreq( );
  uPeriphClock /= ptDef->uBaudRate;
  
  // now set up the trahsnfer
  uTempVal = SPI_CSR_SCBR( uPeriphClock );
  uTempVal |= SPI_CSR_BITS( ptDef->eNumBits );
  uTempVal |= ( ptDef->eClkPol == SPI_CLKPOL_RISING ) ? SPI_CSR_CPOL : 0;
  uTempVal |= ( ptDef->eClkPhase == SPI_CLKPHASE_TRAILING ) ? SPI_CSR_NCPHA : 0;
  uTempVal |= SPI_CSR_CSAAT;
  ptSpi->SPI_CSR[ ptDef->eChipSel ] = uTempVal;
  
  // set up the mode registers
  uTempVal = SPI_MR_MSTR;
  uTempVal |= SPI_MR_MODFDIS;
  uTempVal |= SPI_MR_PCS( ~ptDef->eChipSel );
  ptSpi->SPI_CR = uTempVal; 
}  

/******************************************************************************
 * @function GetSpiChannel
 *
 * @brief gets a pointer to the control structure
 *
 * This function will return a pointer to the correct control structure
 * for a given channel
 *
 * @param[in]   eChan			SPI channel
 *
 * @return      pointer to a SercomI2c or NULL if illegal channel
 *
 *****************************************************************************/
static Spi* GetSpiChannel( SPICHAN eChan )
{
  Spi*  ptSpi;
  
  switch( eChan )
  {
    case SPI_CHAN_0 :
      ptSpi = SPI0;  
      break;
      
    case SPI_CHAN_1 :
      ptSpi = SPI1;
      break;
      
    default :
      ptSpi = NULL;
      break;
  }
  
  // return it
  return( ptSpi );
}

/**@} EOF Spi.c */
