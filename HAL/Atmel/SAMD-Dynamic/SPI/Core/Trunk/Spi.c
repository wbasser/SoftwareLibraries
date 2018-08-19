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
#include "PowerManager/PowerManager.h"

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// local parameter declarations -----------------------------------------------

// local function prototypes --------------------------------------------------
static  SercomSpi* GetSercomChannel( SPICHAN eChan );
static  void       EnableClockPower( SPICHAN eChan );

/******************************************************************************
 * @function Spi_Initialize
 *
 * @brief SPI initialization
 *
 * This function will initialize all SPI ports configured in the table
 *
 * @param[in]   ptDef       pointer to the definiation structure
 *
 *****************************************************************************/
void Spi_Initialize( PSPIDEF ptDef )
{
  SercomSpi*            ptSpi;
  SERCOM_SPI_CTRLA_Type tCtrlA;

  // set the pointer
  ptSpi = GetSercomChannel( ptDef->eChan );

  // enable the channel power and clock
  EnableClockPower( ptDef->eChan );
  
  // enable the gpio pins
  Gpio_Configure( ptDef->eDevPort, ptDef->nMosiPin, GPIO_MODE_OUTPUT_INPDSB, OFF, ptDef->eDevMux, FALSE );
  Gpio_Configure( ptDef->eDevPort, ptDef->nSclkPin, GPIO_MODE_OUTPUT_INPDSB, OFF, ptDef->eDevMux, FALSE );
  Gpio_Configure( ptDef->eDevPort, ptDef->nMisoPin, GPIO_MODE_INPUT_PULLUP,  OFF, ptDef->eDevMux, FALSE );

  // disable - just in case
  ptSpi->CTRLA.bit.SWRST = 1;
  while( ptSpi->SYNCBUSY.bit.SWRST );

  // now set the baudrate
  ptSpi->BAUD.reg = Clock_GetFreq( ) / ptDef->uBaudRate;

  // now build the control A value
  tCtrlA.bit.DORD = ptDef->eBitOrder;

  // set the clock phase and polarity
  tCtrlA.bit.CPOL = ( ptDef->eMode >= SPI_MODE_2 ) ? ON : OFF;
  tCtrlA.bit.CPHA = (( ptDef->eMode == SPI_MODE_1 ) || ( ptDef->eMode == SPI_MODE_3 )) ? ON : OFF;

  // now select the pin modes
  tCtrlA.reg |= SERCOM_SPI_CTRLA_DIPO( ptDef->eMisoSel );
  tCtrlA.reg |= SERCOM_SPI_CTRLA_DOPO( ptDef->eMosiSclkSel );

  // set the mode
  tCtrlA.reg |= SERCOM_SPI_CTRLA_MODE_SPI_MASTER;

  // now write the control register
  ptSpi->CTRLA = tCtrlA;

  // now enable it
  ptSpi->CTRLA.bit.ENABLE = ON;
  while( ptSpi->SYNCBUSY.reg != 0 );
}

/******************************************************************************
 * @function Spi_Write
 *
 * @brief write data to the SPI port
 *
 * This function will write the prescribed number of bytes to the SPI
 *
 * @param[in]   eChan       channel
 * @param[io]   pnBuffer    pointer to the read/write buffer
 * @param[in]   wSize       number of bytes to write
 * @param[in]   bStoreRead  TRUE, storsnarf7550
 e
 *
 *****************************************************************************/
void Spi_Write( SPICHAN eChan, PU8 pnBuffer, U16 wSize, BOOL bStoreRead )
{
  SercomSpi*  ptSpi;
  U16         wBufIdx;

  // get the pointer to the channel
  ptSpi = GetSercomChannel( eChan );

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

/******************************************************************************
 * @function Spi_Read
 *
 * @brief read data from the SPI bus
 *
 * This function will read the proscribed number of bytes from the bus
 *
 * @param[in]   eChan       channel
 * @param[in]   nOutDat   data to send to force read
 * @param[io]   pnBuffer  pointer to the store the data read
 * @param[in]   wSize     number of bytes to read
 *
 * @return  error   appropriate error value
 *
 *****************************************************************************/
void Spi_Read( SPICHAN eChan, U8 nOutData, PU8 pnBuffer, U16 wSize )
{
  SercomSpi*  ptSpi;
  U16         wBufIdx;

  // get the pointer to the channel
  ptSpi = GetSercomChannel( eChan );

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
  Sercom*     ptSercom = NULL;

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

/******************************************************************************
 * @function EnableClockPower
 *
 * @brief enables clock power for this device
 *
 * This function will enable the device in the power manager and the clocj
 *
 * @param[in]   eChan			SERCOM channel
 *
 *****************************************************************************/
static void EnableClockPower( SPICHAN eChan )
{
  CLOCKMUXID  eClockId;
  U32         uPeriphId;

  switch( eChan )
  {
    case SPI_CHAN_0 :
      eClockId = CLOCK_MUXID_SERCOM_0;
      uPeriphId = PM_APBCMASK_SERCOM0;
      break;
      
    case SPI_CHAN_1 :
      eClockId = CLOCK_MUXID_SERCOM_1;
      uPeriphId = PM_APBCMASK_SERCOM1;
      break;
    
    #ifdef SERCOM2
    case SPI_CHAN_2:
      eClockId = CLOCK_MUXID_SERCOM_2;
      uPeriphId = PM_APBCMASK_SERCOM2;
      break;
    #endif // SERCOM2
    
    #ifdef SERCOM3
    case SPI_CHAN_3 :
      eClockId = CLOCK_MUXID_SERCOM_3;
      uPeriphId = PM_APBCMASK_SERCOM3;
      break;
    #endif // SERCOM3
    
    #ifdef SERCOM4
    case SPI_CHAN_4 :
      eClockId = CLOCK_MUXID_SERCOM_4;
      uPeriphId = PM_APBCMASK_SERCOM4;
      break;
    #endif // SERCOM4
    
    #ifdef SERCOM5
    case SPI_CHAN_5 :
      eClockId = CLOCK_MUXID_SERCOM_5;
      uPeriphId = PM_APBCMASK_SERCOM5;
      break;
    #endif // SERCOM5
    
    default :
      break;
  }
  
  // now enable the clock and power mask
  Clock_PeriphEnable( eClockId, CLOCK_GENID_0 );
  PowerManager_DisableEnablePeriph( uPeriphId, ON );
}

/**@} EOF Spi.c */
