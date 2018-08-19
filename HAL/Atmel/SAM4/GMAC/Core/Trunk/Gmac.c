/******************************************************************************
 * @file Gmac.c
 *
 * @brief MAC implementation
 *
 * This file provides the implementation for the GMAC
 *
 * @copyright Copyright (c) 2012 CyberIntegration
 * This document contains proprietary data and information of CyberIntegration 
 * LLC. It is the exclusive property of CyberIntegration, LLC and will not be 
 * disclosed in any form to any party without prior written permission of 
 * CyberIntegration, LLC. This document may not be reproduced or further used 
 * without the prior written permission of CyberIntegration, LLC.
 *
 * $Date: $
 *
 * Version History
 * ======
 * $Rev: $
 * 
 *
 * \addtogroup 
 * @{
 *****************************************************************************/

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "GMAC/Gmac.h"

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------
#define MAC_PHY_RETRY_MAX                            ( 1000000 )

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations ----------------------------------------------

// local parameter declarations -----------------------------------------------

// local function prototypes --------------------------------------------------
static GMACSTATUS PhyWait( Gmac* ptGmac, U32 uRetry );

// constant parameter initializations -----------------------------------------

/******************************************************************************
 * @function 
 *
 * @brief 
 *
 * This function 
 *
 * @param[in]   
 *
 *****************************************************************************/
void Gmac_NetworkControl( Gmac* ptGmac, U32 uNcr )
{
  // set the network control
  ptGmac->GMAC_NCR = uNcr;
}

/******************************************************************************
 * @function 
 *
 * @brief 
 *
 * This function 
 *
 * @param[in]   ptGmac    pointer to the GMAC
 *
 * @return      
 *
 *****************************************************************************/
U32 Gmac_GetNetworkControl( Gmac* ptGmac )
{
  // return the control value
  return( ptGmac->GMAC_NCR );
}

/******************************************************************************
 * @function 
 *
 * @brief 
 *
 * This function 
 *
 * @param[in]   ptGmac    pointer to the GMAC
 * @parampin[   bEnable   TRUE to enable, FALSE to disable
 *
 *****************************************************************************/
void Gmac_EnableReceive( Gmac* ptGmac, BOOL bEnable )
{
  // diable/enable the receive
  if ( bEnable )
  {
    // enable it
    ptGmac->GMAC_NCR |= GMAC_NCR_RXEN;
  }
  else
  {
    // disable it
    ptGmac->GMAC_NCR &= ~( GMAC_NCR_RXEN );
  }
}

/******************************************************************************
 * @function 
 *
 * @brief 
 *
 * This function 
 *
 * @param[in]   ptGmac    pointer to the GMAC
 * @parampin[   bEnable   TRUE to enable, FALSE to disable
 *
 *****************************************************************************/
void Gmac_EnableTransmit( Gmac* ptGmac, BOOL bEnable )
{
  // diable/enable the transmit
  if ( bEnable )
  {
    // enable it
    ptGmac->GMAC_NCR |= GMAC_NCR_TXEN;
  }
  else
  {
    // disable it
    ptGmac->GMAC_NCR &= ~( GMAC_NCR_TXEN );
  }
}

/******************************************************************************
 * @function 
 *
 * @brief 
 *
 * This function 
 *
 * @param[in]   ptGmac    pointer to the GMAC
 * @parampin[   bEnable   TRUE to enable, FALSE to disable
 *
 *****************************************************************************/
void Gmac_EnableManagement( Gmac* ptGmac, BOOL bEnable )
{
  // diable/enable the management
  if ( bEnable )
  {
    // enable it
    ptGmac->GMAC_NCR |= GMAC_NCR_MPE;
  }
  else
  {
    // disable it
    ptGmac->GMAC_NCR &= ~( GMAC_NCR_MPE );
  }
}

/******************************************************************************
 * @function 
 *
 * @brief 
 *
 * This function 
 *
 * @param[in]   ptGmac    pointer to the GMAC
 *
 *****************************************************************************/
void Gmac_ClearStatistics( Gmac* ptGmac )
{
  // set clear the stats
  ptGmac->GMAC_NCR |= GMAC_NCR_CLRSTAT;
}

/******************************************************************************
 * @function 
 *
 * @brief 
 *
 * This function 
 *
 * @param[in]   ptGmac    pointer to the GMAC
 *
 *****************************************************************************/
void Gmac_IncreaseStatistics( Gmac* ptGmac )
{
  // incease all statistic regs
  ptGmac->GMAC_NCR |= GMAC_NCR_INCSTAT;
}

/******************************************************************************
 * @function 
 *
 * @brief 
 *
 * This function 
 *
 * @param[in]   ptGmac    pointer to the GMAC
 * @parampin[   bEnable   TRUE to enable, FALSE to disable
 *
 *****************************************************************************/
void Gmac_EnableStatisticsWrite( Gmac* ptGmac, BOOL bEnable )
{
  // diable/enable the management
  if ( bEnable )
  {
    // enable it
    ptGmac->GMAC_NCR |= GMAC_NCR_WESTAT;
  }
  else
  {
    // disable it
    ptGmac->GMAC_NCR &= ~( GMAC_NCR_WESTAT );
  }
}

/******************************************************************************
 * @function 
 *
 * @brief 
 *
 * This function 
 *
 * @param[in]   ptGmac    pointer to the GMAC
 * @parampin[   bEnable   TRUE to enable, FALSE to disable
 *
 *****************************************************************************/
void Gmac_EnableBackPressure( Gmac* ptGmac, BOOL bEnable )
{
  // diable/enable the management
  if ( bEnable )
  {
    // enable it
    ptGmac->GMAC_NCR |= GMAC_NCR_BP;
  }
  else
  {
    // disable it
    ptGmac->GMAC_NCR &= ~( GMAC_NCR_BP );
  }
}

/******************************************************************************
 * @function 
 *
 * @brief 
 *
 * This function 
 *
 * @param[in]   ptGmac    pointer to the GMAC
 *
 *****************************************************************************/
void Gmac_StartTransmission( Gmac* ptGmac )
{
  // start transmission
  ptGmac->GMAC_NCR |= GMAC_NCR_TSTART;
}

/******************************************************************************
 * @function 
 *
 * @brief 
 *
 * This function 
 *
 * @param[in]   ptGmac    pointer to the GMAC
 *
 *****************************************************************************/
void Gmac_HaltTransmission( Gmac* ptGmac )
{
  // halt transmission
  ptGmac->GMAC_NCR |= GMAC_NCR_THALT;
}

/******************************************************************************
 * @function 
 *
 * @brief 
 *
 * This function 
 *
 * @param[in]   ptGmac    pointer to the GMAC
 *
 *****************************************************************************/
void Gmac_TxPauseFrame( Gmac* ptGmac )
{
  // transmit puase frame
  ptGmac->GMAC_NCR |= GMAC_NCR_TXPF;
}

/******************************************************************************
 * @function 
 *
 * @brief 
 *
 * This function 
 *
 * @param[in]   ptGmac    pointer to the GMAC
 *
 *****************************************************************************/
void Gmac_TxPauseZeroQuantumFrame( Gmac* ptGmac )
{
  // transmit zero quantum pause frame
  ptGmac->GMAC_NCR |= GMAC_NCR_TXZQPF;
}

/******************************************************************************
 * @function 
 *
 * @brief 
 *
 * This function 
 *
 * @param[in]   ptGmac    pointer to the GMAC
 * @parampin[   bEnable   TRUE to enable, FALSE to disable
 *
 *****************************************************************************/
void Gmac_StoreRxTimeStamp( Gmac* ptGmac, BOOL bEnable )
{
  // diable/enable the management
  if ( bEnable )
  {
    // enable it
    ptGmac->GMAC_NCR |= GMAC_NCR_SRTSM;
  }
  else
  {
    // disable it
    ptGmac->GMAC_NCR &= ~( GMAC_NCR_SRTSM );
  }
}

/******************************************************************************
 * @function 
 *
 * @brief 
 *
 * This function 
 *
 * @param[in]   ptGmac    pointer to the GMAC
 * @parampin[   bEnable   TRUE to enable, FALSE to disable
 *
 *****************************************************************************/
void Gmac_EnablePfcPauseFrame( Gmac* ptGmac, BOOL bEnable )
{
  // diable/enable the management
  if ( bEnable )
  {
    // enable it
    ptGmac->GMAC_NCR |= GMAC_NCR_ENPBPR;
  }
  else
  {
    // disable it
    ptGmac->GMAC_NCR &= ~( GMAC_NCR_ENPBPR );
  }
}

/******************************************************************************
 * @function 
 *
 * @brief 
 *
 * This function 
 *
 * @param[in]   ptGmac    pointer to the GMAC
 *
 *****************************************************************************/
void Gmac_TransmitPfcPauseFrame( Gmac* ptGmac )
{
  // transmit PFC priority based pause reception
  ptGmac->GMAC_NCR |= GMAC_NCR_TXPBPF;
}

/******************************************************************************
 * @function 
 *
 * @brief 
 *
 * This function 
 *
 * @param[in]   ptGmac    pointer to the GMAC
 *
 *****************************************************************************/
void Gmac_FlushNextPacket( Gmac* ptGmac )
{
  // flush next packet
  ptGmac->GMAC_NCR |= GMAC_NCR_FNP;
}

/******************************************************************************
 * @function 
 *
 * @brief 
 *
 * This function 
 *
 * @param[in]   ptGmac    pointer to the GMAC
 *
 *****************************************************************************/
void Gmac_SetConfig( Gmac* ptGmac, U32 uCfg )
{
  // set the config
  ptGmac->GMAC_NCFGR = uCfg;
}

/******************************************************************************
 * @function 
 *
 * @brief 
 *
 * This function 
 *
 * @param[in]   ptGmac    pointer to the GMAC
 *
 * @return      
 *
 *****************************************************************************/
U32 Gmac_GetConfig( Gmac* ptGmac )
{
  // return the config
  return( ptGmac->GMAC_NCFGR );
}

/******************************************************************************
 * @function 
 *
 * @brief 
 *
 * This function 
 *
 * @param[in]   ptGmac    pointer to the GMAC
 * @parampin[   bEnable   TRUE to enable, FALSE to disable
 *
 *****************************************************************************/
void Gmac_SetSpeed( Gmac* ptGmac, BOOL bEnable )
{
  // diable/enable the management
  if ( bEnable )
  {
    // enable it
    ptGmac->GMAC_NCFGR |= GMAC_NCFGR_SPD;
  }
  else
  {
    // disable it
    ptGmac->GMAC_NCFGR &= ~( GMAC_NCFGR_SPD );
  }
}

/******************************************************************************
 * @function 
 *
 * @brief 
 *
 * This function 
 *
 * @param[in]   ptGmac    pointer to the GMAC
 * @parampin[   bEnable   TRUE to enable, FALSE to disable
 *
 *****************************************************************************/
void Gmac_EnableFullDuplex( Gmac* ptGmac, BOOL bEnable )
{
  // diable/enable the management
  if ( bEnable )
  {
    // enable it
    ptGmac->GMAC_NCFGR |= GMAC_NCFGR_FD;
  }
  else
  {
    // disable it
    ptGmac->GMAC_NCFGR &= ~( GMAC_NCFGR_FD );
  }
}

/******************************************************************************
 * @function 
 *
 * @brief 
 *
 * This function 
 *
 * @param[in]   ptGmac    pointer to the GMAC
 * @parampin[   bEnable   TRUE to enable, FALSE to disable
 *
 *****************************************************************************/
void Gmac_EnableCopyAll( Gmac* ptGmac, BOOL bEnable )
{
  // diable/enable the management
  if ( bEnable )
  {
    // enable it
    ptGmac->GMAC_NCFGR |= GMAC_NCFGR_CAF;
  }
  else
  {
    // disable it
    ptGmac->GMAC_NCFGR &= ~( GMAC_NCFGR_CAF );
  }
}

/******************************************************************************
 * @function 
 *
 * @brief 
 *
 * This function 
 *
 * @param[in]   ptGmac    pointer to the GMAC
 * @parampin[   bEnable   TRUE to enable, FALSE to disable
 *
 *****************************************************************************/
void Gmac_EnableJumboFrames( Gmac* ptGmac, BOOL bEnable )
{
  // diable/enable the management
  if ( bEnable )
  {
    // enable it
    ptGmac->GMAC_NCFGR |= GMAC_NCFGR_JFRAME;
  }
  else
  {
    // disable it
    ptGmac->GMAC_NCFGR &= ~( GMAC_NCFGR_JFRAME );
  }
}

/******************************************************************************
 * @function 
 *
 * @brief 
 *
 * This function 
 *
 * @param[in]   ptGmac    pointer to the GMAC
 * @parampin[   bEnable   TRUE to enable, FALSE to disable
 *
 *****************************************************************************/
void Gmac_DisableBroadcast( Gmac* ptGmac, BOOL bEnable )
{
  // diable/enable the management
  if ( bEnable )
  {
    // enable it
    ptGmac->GMAC_NCFGR |= GMAC_NCFGR_NBC;
  }
  else
  {
    // disable it
    ptGmac->GMAC_NCFGR &= ~( GMAC_NCFGR_NBC );
  }
}

/******************************************************************************
 * @function 
 *
 * @brief 
 *
 * This function 
 *
 * @param[in]   ptGmac    pointer to the GMAC
 * @parampin[   bEnable   TRUE to enable, FALSE to disable
 *
 *****************************************************************************/
void Gmac_EnableMulticastHash( Gmac* ptGmac, BOOL bEnable )
{
  // diable/enable the management
  if ( bEnable )
  {
    // enable it
    ptGmac->GMAC_NCFGR |= GMAC_NCFGR_UNIHEN;
  }
  else
  {
    // disable it
    ptGmac->GMAC_NCFGR &= ~( GMAC_NCFGR_UNIHEN );
  }
}

/******************************************************************************
 * @function 
 *
 * @brief 
 *
 * This function 
 *
 * @param[in]   ptGmac    pointer to the GMAC
 * @parampin[   bEnable   TRUE to enable, FALSE to disable
 *
 *****************************************************************************/
void Gmac_EnableBigFrame( Gmac* ptGmac, BOOL bEnable )
{
  // diable/enable the management
  if ( bEnable )
  {
    // enable it
    ptGmac->GMAC_NCFGR |= GMAC_NCFGR_MAXFS;
  }
  else
  {
    // disable it
    ptGmac->GMAC_NCFGR &= ~( GMAC_NCFGR_MAXFS );
  }
}

/******************************************************************************
 * @function 
 *
 * @brief 
 *
 * This function 
 *
 * @param[in]   ptGmac    pointer to the GMAC
 *
 * @return      
 *
 *****************************************************************************/
GMACSTATUS Gmac_SetMdcClock( Gmac* ptGmac, U32 uMck )
{
  U32 uClkMask;
  GMACSTATUS  eStatus = GMAC_STATUS_OK;
  
  // determine clock speed
  if ( uMck > GMAC_MCK_SPEED_240MHZ )
  {
    // error status
    eStatus = GMAC_STATUS_INVALID;
  }
  else if ( uMck > GMAC_MCK_SPEED_160MHZ )
  {
    // set GMAC clock to 96
    uClkMask = GMAC_NCFGR_CLK_MCK_96;
  }
  else if ( uMck > GMAC_MCK_SPEED_120MHZ )
  {
    // set GMAC clock to 64
    uClkMask = GMAC_NCFGR_CLK_MCK_64;
  }
  else if ( uMck > GMAC_MCK_SPEED_80MHZ )
  {
    // set GMAC clock to 48
    uClkMask = GMAC_NCFGR_CLK_MCK_48;
  }
  else if ( uMck > GMAC_MCK_SPEED_40MHZ )
  {
    // set GMAC clock to 32
    uClkMask = GMAC_NCFGR_CLK_MCK_32;
  }
  else if ( uMck > GMAC_MCK_SPEED_20MHZ )
  {
    // set GMAC clock to 16
    uClkMask = GMAC_NCFGR_CLK_MCK_16;
  }
  else
  {
    // set GMAC clock to 8
    uClkMask = GMAC_NCFGR_CLK_MCK_8;
  }
  
  // if valid set the clock
  if ( eStatus == GMAC_STATUS_OK )
  {
    // set the new clock valud
    ptGmac->GMAC_NCFGR &= ~( GMAC_NCFGR_CLK_Msk );
    ptGmac->GMAC_NCFGR |= uClkMask;
  }
  
  // return it
  return( eStatus );
}

/******************************************************************************
 * @function 
 *
 * @brief 
 *
 * This function 
 *
 * @param[in]   ptGmac    pointer to the GMAC
 * @parampin[   bEnable   TRUE to enable, FALSE to disable
 *
 *****************************************************************************/
void Gmac_EnableRetryTest( Gmac* ptGmac, BOOL bEnable )
{
  // diable/enable the management
  if ( bEnable )
  {
    // enable it
    ptGmac->GMAC_NCFGR |= GMAC_NCFGR_RTY;
  }
  else
  {
    // disable it
    ptGmac->GMAC_NCFGR &= ~( GMAC_NCFGR_RTY );
  }
}

/******************************************************************************
 * @function 
 *
 * @brief 
 *
 * This function 
 *
 * @param[in]   ptGmac    pointer to the GMAC
 * @parampin[   bEnable   TRUE to enable, FALSE to disable
 *
 *****************************************************************************/
void Gmac_EnablePauseRrame( Gmac* ptGmac, BOOL bEnable )
{
  // diable/enable the management
  if ( bEnable )
  {
    // enable it
    ptGmac->GMAC_NCFGR |= GMAC_NCFGR_PEN;
  }
  else
  {
    // disable it
    ptGmac->GMAC_NCFGR &= ~( GMAC_NCFGR_PEN );
  }
}

/******************************************************************************
 * @function 
 *
 * @brief 
 *
 * This function 
 *
 * @param[in]   ptGmac    pointer to the GMAC
 *
 *****************************************************************************/
void Gmac_SetRxBufferOffset( Gmac* ptGmac, U8 nOffset )
{
  // set the new buffer offset
  ptGmac->GMAC_NCFGR &= ~( GMAC_NCFGR_RXBUFO_Msk );
  ptGmac->GMAC_NCFGR |= nOffset;
}

/******************************************************************************
 * @function 
 *
 * @brief 
 *
 * This function 
 *
 * @param[in]   ptGmac    pointer to the GMAC
 * @parampin[   bEnable   TRUE to enable, FALSE to disable
 *
 *****************************************************************************/
void Gmac_EnableRxLengthCheck( Gmac* ptGmac, BOOL bEnable )
{
  // diable/enable the management
  if ( bEnable )
  {
    // enable it
    ptGmac->GMAC_NCFGR |= GMAC_NCFGR_LFERD;
  }
  else
  {
    // disable it
    ptGmac->GMAC_NCFGR &= ~( GMAC_NCFGR_LFERD );
  }
}

/******************************************************************************
 * @function 
 *
 * @brief 
 *
 * This function 
 *
 * @param[in]   ptGmac    pointer to the GMAC
 * @parampin[   bEnable   TRUE to enable, FALSE to disable
 *
 *****************************************************************************/
void Gmac_EnableDiscardFcs( Gmac* ptGmac, BOOL bEnable )
{
  // diable/enable the management
  if ( bEnable )
  {
    // enable it
    ptGmac->GMAC_NCFGR |= GMAC_NCFGR_RFCS;
  }
  else
  {
    // disable it
    ptGmac->GMAC_NCFGR &= ~( GMAC_NCFGR_RFCS );
  }
}

/******************************************************************************
 * @function 
 *
 * @brief 
 *
 * This function 
 *
 * @param[in]   ptGmac    pointer to the GMAC
 * @parampin[   bEnable   TRUE to enable, FALSE to disable
 *
 *****************************************************************************/
void Gmac_EnableEfrhd( Gmac* ptGmac, BOOL bEnable )
{
  // diable/enable the management
  if ( bEnable )
  {
    // enable it
    ptGmac->GMAC_NCFGR |= GMAC_NCFGR_EFRHD;
  }
  else
  {
    // disable it
    ptGmac->GMAC_NCFGR &= ~( GMAC_NCFGR_EFRHD );
  }
}

/******************************************************************************
 * @function 
 *
 * @brief 
 *
 * This function 
 *
 * @param[in]   ptGmac    pointer to the GMAC
 * @parampin[   bEnable   TRUE to enable, FALSE to disable
 *
 *****************************************************************************/
void Gmac_EnableIgnoreRxFcs( Gmac* ptGmac, BOOL bEnable )
{
  // diable/enable the management
  if ( bEnable )
  {
    // enable it
    ptGmac->GMAC_NCFGR |= GMAC_NCFGR_IRXFCS;
  }
  else
  {
    // disable it
    ptGmac->GMAC_NCFGR &= ~( GMAC_NCFGR_IRXFCS );
  }
}

/******************************************************************************
 * @function 
 *
 * @brief 
 *
 * This function 
 *
 * @param[in]   ptGmac    pointer to the GMAC
 *
 * @return      
 *
 *****************************************************************************/
U32 Gmac_GetStatus( Gmac* ptGmac )
{
  // return the status
  return( ptGmac->GMAC_NSR );
}

/******************************************************************************
 * @function 
 *
 * @brief 
 *
 * This function 
 *
 * @param[in]   ptGmac    pointer to the GMAC
 *
 * @return      
 *
 *****************************************************************************/
BOOL Gmac_GetMDIO( Gmac* ptGmac )
{
  // return the MDIO
  return(( ptGmac->GMAC_NSR & GMAC_NSR_MDIO ) ? TRUE : FALSE );
}

/******************************************************************************
 * @function 
 *
 * @brief 
 *
 * This function 
 *
 * @param[in]   ptGmac    pointer to the GMAC
 *
 * @return      
 *
 *****************************************************************************/
BOOL Gmac_IsPhyIdle(Gmac* ptGmac )
{
  // return the PHY idle
  return(( ptGmac->GMAC_NSR & GMAC_NSR_IDLE ) ? TRUE : FALSE );
}

/******************************************************************************
 * @function 
 *
 * @brief 
 *
 * This function 
 *
 * @param[in]   ptGmac    pointer to the GMAC
 *
 * @return      
 *
 *****************************************************************************/
U32 Gmac_GetTxStatus( Gmac* ptGmac )
{
  // return the TX status
  return( ptGmac->GMAC_TSR );
}

/******************************************************************************
 * @function 
 *
 * @brief 
 *
 * This function 
 *
 * @param[in]   ptGmac    pointer to the GMAC
 *
 *****************************************************************************/
void Gmac_ClearTxStatus( Gmac* ptGmac, U32 uStatus )
{
  // clear the TX status
  ptGmac->GMAC_TSR = uStatus;
}

/******************************************************************************
 * @function 
 *
 * @brief 
 *
 * This function 
 *
 * @param[in]   ptGmac    pointer to the GMAC
 *
 * @return      
 *
 *****************************************************************************/
U32 Gmac_GetRxStatus( Gmac* ptGmac )
{
  // return the RX status
  return( ptGmac->GMAC_RSR );
}

/******************************************************************************
 * @function 
 *
 * @brief 
 *
 * This function 
 *
 * @param[in]   ptGmac    pointer to the GMAC
 *
 *****************************************************************************/
void Gmac_ClearRxStatus( Gmac* ptGmac, U32 uStatus )
{
  // clear the RX status
  ptGmac->GMAC_RSR = uStatus;
}

/******************************************************************************
 * @function 
 *
 * @brief 
 *
 * This function 
 *
 * @param[in]   ptGmac    pointer to the GMAC
 *
 *****************************************************************************/
void Gmac_SetRxQueue( Gmac* ptGmac, U32 uAddr )
{
  // set the RX queue Address
  ptGmac->GMAC_RBQB = ( GMAC_RBQB_ADDR_Msk & uAddr );
}

/******************************************************************************
 * @function 
 *
 * @brief 
 *
 * This function 
 *
 * @param[in]   ptGmac    pointer to the GMAC
 *
 *****************************************************************************/
void Gmac_SetRxBufSize( Gmac* ptGmac, U32 uCode )
{
  // set the RX buffer size
	ptGmac->GMAC_DCFGR = ( ptGmac->GMAC_DCFGR & ~GMAC_DCFGR_DRBS_Msk ) | GMAC_DCFGR_DRBS( uCode );
}

/******************************************************************************
 * @function 
 *
 * @brief 
 *
 * This function 
 *
 * @param[in]   ptGmac    pointer to the GMAC
 *
 * @return      
 *
 *****************************************************************************/
U32 Gmac_GetRxQueue( Gmac* ptGmac )
{
  // return the RX queue address
  return( ptGmac->GMAC_RBQB );
}

/******************************************************************************
 * @function 
 *
 * @brief 
 *
 * This function 
 *
 * @param[in]   ptGmac    pointer to the GMAC
 *
 *****************************************************************************/
void Gmac_SetTxQueue( Gmac* ptGmac, U32 uAddr )
{
  // set the TX queue Address
  ptGmac->GMAC_TBQB = ( GMAC_TBQB_ADDR_Msk & uAddr );
}

/******************************************************************************
 * @function 
 *
 * @brief 
 *
 * This function 
 *
 * @param[in]   ptGmac    pointer to the GMAC
 *
 * @return      
 *
 *****************************************************************************/
U32 Gmac_GetTxQueue( Gmac* ptGmac )
{
  // return the TX queue address
  return( ptGmac->GMAC_TBQB );
}

/******************************************************************************
 * @function 
 *
 * @brief 
 *
 * This function 
 *
 * @param[in]   ptGmac    pointer to the GMAC
 *
 *****************************************************************************/
void Gmac_EnableInterrupt( Gmac* ptGmac, U32 uSource )
{
  // set the interrupt enable
  ptGmac->GMAC_IER = uSource;
}

/******************************************************************************
 * @function 
 *
 * @brief 
 *
 * This function 
 *
 * @param[in]   ptGmac    pointer to the GMAC
 *
 *****************************************************************************/
void Gmac_DisableInterrupt( Gmac* ptGmac, U32 uSource )
{
  // disable the interrupts
  ptGmac->GMAC_IDR = uSource;
}

/******************************************************************************
 * @function 
 *
 * @brief 
 *
 * This function 
 *
 * @param[in]   ptGmac    pointer to the GMAC
 *
 * @return      
 *
 *****************************************************************************/
U32 Gmac_GetInterruptStatus( Gmac* ptGmac )
{
  // get the interrupt status
  return( ptGmac->GMAC_ISR );
}

/******************************************************************************
 * @function 
 *
 * @brief 
 *
 * This function 
 *
 * @param[in]   ptGmac    pointer to the GMAC
 *
 * @return      
 *
 *****************************************************************************/
U32 Gmac_GetInterruptMask( Gmac* ptGmac )
{
  // returm the interrupt mask
  return( ptGmac->GMAC_IMR );
}

/******************************************************************************
 * @function 
 *
 * @brief 
 *
 * This function 
 *
 * @param[in]   ptGmac    pointer to the GMAC
 *
 *****************************************************************************/
void Gmac_MaintainPhy( Gmac* ptGmac, U8 nPhyAddr, U8 nRegAddr, U8 nRw, U16 wData )
{
  // Wait until bus idle
	while (( ptGmac->GMAC_NSR & GMAC_NSR_IDLE) == 0 );
  
	// Write maintain register
	ptGmac->GMAC_MAN = GMAC_MAN_WTN( GMAC_MAN_CODE_VALUE )
                   | GMAC_MAN_CLTTO
                   | GMAC_MAN_PHYA( nPhyAddr )
                   | GMAC_MAN_REGA( nRegAddr )
                   | GMAC_MAN_OP(( nRw ? GMAC_MAN_RW_TYPE : GMAC_MAN_READ_ONLY))
                   | GMAC_MAN_DATA( wData);
}

/******************************************************************************
 * @function 
 *
 * @brief 
 *
 * This function 
 *
 * @param[in]   ptGmac    pointer to the GMAC
 *
 * @return      
 *
 *****************************************************************************/
U16 Gmac_GetPhyData( Gmac* ptGmac )
{
	// Wait until bus idle
	while (( ptGmac->GMAC_NSR & GMAC_NSR_IDLE ) == 0 );
  
	// return data
	return (( U16 )( ptGmac->GMAC_MAN & GMAC_MAN_DATA_Msk ));
}

/******************************************************************************
 * @function 
 *
 * @brief 
 *
 * This function 
 *
 * @param[in]   ptGmac    pointer to the GMAC
 *
 *****************************************************************************/
void Gmac_SetHash( Gmac* ptGmac, U32 uHashTop, U32 uHashBottom )
{
  // set the hash
  ptGmac->GMAC_HRB = uHashBottom;
  ptGmac->GMAC_HRT = uHashTop;
}

/******************************************************************************
 * @function 
 *
 * @brief 
 *
 * This function 
 *
 * @param[in]   ptGmac    pointer to the GMAC
 *
 *****************************************************************************/
void Gmac_SetHash64( Gmac* ptGmac, U64 hHash )
{
  U64UN tHash;
  
  // set the hash
  tHash.hValue = hHash;
  
  // set the 32 bit hash value
  ptGmac->GMAC_HRB = tHash.auValue[ LE_U64_LSU_IDX ];
  ptGmac->GMAC_HRT = tHash.auValue[ LE_U64_MSU_IDX ];
}

/******************************************************************************
 * @function 
 *
 * @brief 
 *
 * This function 
 *
 * @param[in]   ptGmac    pointer to the GMAC
 *
 *****************************************************************************/
void Gmac_SetAddress( Gmac* ptGmac, U8 nIndex, PU8 pnMacAddr )
{
  U32UN tValue;
  
  // build the address
  tValue.anValue[ LE_U32_MSB_IDX ] = *( pnMacAddr + 3 );
  tValue.anValue[ LE_U32_MS2_IDX ] = *( pnMacAddr + 2 );
  tValue.anValue[ LE_U32_MS1_IDX ] = *( pnMacAddr + 1 );
  tValue.anValue[ LE_U32_LSB_IDX ] = *( pnMacAddr + 0 );
  
  // set the SAB address
	ptGmac->GMAC_SA[ nIndex ].GMAC_SAB = tValue.uValue;

  // build the address
  tValue.anValue[ LE_U32_MS1_IDX ] = *( pnMacAddr + 5 );
  tValue.anValue[ LE_U32_LSB_IDX ] = *( pnMacAddr + 4 );
  
  // set the SAT address
	ptGmac->GMAC_SA[ nIndex ].GMAC_SAT = tValue.awValue[ LE_U32_LSW_IDX ];
}

/******************************************************************************
 * @function 
 *
 * @brief 
 *
 * This function 
 *
 * @param[in]   ptGmac    pointer to the GMAC
 *
 *****************************************************************************/
void Gmac_SetAddress32( Gmac* ptGmac, U8 nIndex, U32 uMacTop, U32 uMacBottom )
{
  // set the SAB address
	ptGmac->GMAC_SA[ nIndex ].GMAC_SAB = uMacBottom;
	ptGmac->GMAC_SA[ nIndex ].GMAC_SAT = uMacTop;
}

/******************************************************************************
 * @function 
 *
 * @brief 
 *
 * This function 
 *
 * @param[in]   ptGmac    pointer to the GMAC
 *
 *****************************************************************************/
void Gmac_SetAddress64( Gmac* ptGmac, U8 nIndex,	U64 hMac )
{
  U64UN tValue;
  
  // set the address
  tValue.hValue = hMac;

  // set the address(s)
	ptGmac->GMAC_SA[ nIndex ].GMAC_SAB = tValue.auValue[ LE_U64_LSU_IDX ];
	ptGmac->GMAC_SA[ nIndex ].GMAC_SAT = tValue.auValue[ LE_U64_MSU_IDX ];
}

/******************************************************************************
 * @function 
 *
 * @brief 
 *
 * This function 
 *
 * @param[in]   ptGmac    pointer to the GMAC
 *
 *****************************************************************************/
void Gmac_SelectMiiMode( Gmac* ptGmac, GMACMIIMODE eMode )
{
  // process the mode
	switch ( eMode )
  {
    case GMAC_PHY_MII:
		case GMAC_PHY_RMII:
			ptGmac->GMAC_UR |= GMAC_UR_MII;
      break;

		default:
			ptGmac->GMAC_UR &= ~GMAC_UR_MII;
      break;
	}
}

/******************************************************************************
 * @function 
 *
 * @brief 
 *
 * This function 
 *
 * @param[in]   ptGmac    pointer to the GMAC
 *
 * @return      
 *
 *****************************************************************************/
GMACSTATUS Gmac_PhyRead( Gmac* ptGmac, U8 nPhyAddress, U8 nAddress, PU32 puValue )
{
  GMACSTATUS eStatus = GMAC_STATUS_OK;
  
  // maintain PHY
	Gmac_MaintainPhy( ptGmac, nPhyAddress, nAddress, 1, 0 );

  // wait for phy
	if (( eStatus = PhyWait( ptGmac, MAC_PHY_RETRY_MAX )) != GMAC_STATUS_TIMEOUT )
  {
    // get the value
    *( puValue ) = Gmac_GetPhyData( ptGmac );
  }
  
  // return the status
  return( eStatus );
}

/******************************************************************************
 * @function 
 *
 * @brief 
 *
 * This function 
 *
 * @param[in]   ptGmac    pointer to the GMAC
 *
 * @return      
 *
 *****************************************************************************/
GMACSTATUS Gmac_PhyWrite( Gmac* ptGmac, U8 nPhyAddress, U8 nAddress, U32 uValue )
{
  GMACSTATUS eStatus = GMAC_STATUS_OK;
  
  // maintain PHY
	Gmac_MaintainPhy( ptGmac, nPhyAddress, nAddress, 0, uValue );

  // wait for phy
	eStatus = PhyWait( ptGmac, MAC_PHY_RETRY_MAX );
  
  // return the status
  return( eStatus );
}

/******************************************************************************
 * @function 
 *
 * @brief 
 *
 * This function 
 *
 * @param[in]   ptGmac    pointer to the GMAC
 *
 * @return      
 *
 *****************************************************************************/
static GMACSTATUS PhyWait( Gmac* ptGmac, U32 uRetry )
{
  U32         uRetryCount = 0;
  GMACSTATUS  eStatus = GMAC_STATUS_TIMEOUT;
  
  // loop
  do
  {
    // test for idle
    if ( Gmac_IsPhyIdle( ptGmac ) == TRUE )
    {
      // exit loop
      eStatus = GMAC_STATUS_OK;
      break;
    }
    else
    {
      // if retry is not 0
      if ( uRetry != 0 )
      {
        // increment the count
        uRetryCount++;
      }
    }
  } while(( uRetryCount < uRetry ) || ( uRetry == 0 ));
  
  // return the status
  return( eStatus );
}

/**@} EOF .c */
