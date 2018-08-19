/******************************************************************************
 * @file KSZ8051MNLPhy.c
 *
 * @brief 
 *
 * This file 
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
 * \addtogroup KSZ8051MNLPhy
 * @{
 *****************************************************************************/

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "KSZ8051MNLPhy/KSZ8051MNLPhy.h"
#include "KSZ8051MNLPhy/KSZ8051MNLPhy_prv.h"

// library includes -----------------------------------------------------------
#include "Clock/Clock.h"
#include "GMAC/Gmac.h"

// Macros and Defines ---------------------------------------------------------
/// define the maximum PHY address
#define PHY_MAX_ADDR            ( 31 )

/// defne the retry count
#define PHY_RETRY_MAX           ( 100000 )

/// define the timeout
#define PHY_TIMEOUT             ( 10 )

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations ----------------------------------------------

// local parameter declarations -----------------------------------------------

// local function prototypes --------------------------------------------------

// constant parameter initializations -----------------------------------------

/******************************************************************************
 * @function KSZ8051MNLPhy_Initialize
 *
 * @brief initailization
 *
 * This function will initialize the PHY
 *
 * @param[in]   puGmacAddr      pointer to the GMAC
 * @parma[in]   nPhyAddr    Phy address
 *
 * @return      approriate GMACSTATUS
 *
 *****************************************************************************/
GMACSTATUS KSZ8051MNLPhy_Initialize( PU32 puGmacAddr, U8 nPhyAddr )
{
  GMACSTATUS  eStatus = GMAC_STATUS_OK;
  U8          nFoundPhy;
  U32         uClockFreq;
  
  // reset the phy
  KSZ8051MNLPhy_Reset( puGmacAddr, nPhyAddr );

  // set the clock speed
  uClockFreq = Clock_GetFreq( );
  KSZ8051MNLPhy_SetMdcClock( puGmacAddr, uClockFreq );
  
  // return the status
  return( eStatus );
}

/******************************************************************************
 * @function KSZ8051MNLPhy_SetLink
 *
 * @brief Set the link parameters
 *
 * This function will set up the link/speed setting 
 *
 * @param[in]   puGmacAddr      pointer to the GMAC
 * @parma[in]   nPhyAddr    Phy address
 * @param[in]   bApplyFlags TRUE if flags are to be applied
 *
 * @return      approriate GMACSTATUS
 *
 *****************************************************************************/
GMACSTATUS KSZ8051MNLPhy_SetLink( PU32 puGmacAddr, U8 nPhyAddr, BOOL bApplyFlags )
{
  GMACSTATUS  eStatus = GMAC_STATUS_OK;
  U32         uStatus1, uStatus2;
  BOOL        bSpeed, bFullDuplex;
  
  // enable the managmenet
  KSZ8051MNLPhy_EnableManagement( puGmacAddr, TRUE );
  
  // get the current status
  if (( eStatus = KSZ8051MNLPhy_PhyRead( puGmacAddr, nPhyAddr, GMII_BMSR, &uStatus1 )) == GMAC_STATUS_OK )
  {
    // do we have a valid link status
    if ( uStatus1 & GMII_LINK_STATUS )
    {
      // do we have something to do
      if ( bApplyFlags )
      {
        // get the current advertisement
        if (( eStatus = KSZ8051MNLPhy_PhyRead( puGmacAddr, nPhyAddr, GMII_PCR1, &uStatus2 )) == GMAC_STATUS_OK )
        {
          // determine the speeds
          if (( uStatus1 & GMII_100BASE_TX_FD ) && ( uStatus2 & GMII_OMI_100BASE_TX_FD ))
          {
            // set 100baseT and full duplex
            bSpeed = TRUE;
            bFullDuplex = TRUE;
          }
          else if (( uStatus1 & GMII_10BASE_T_FD ) && ( uStatus2 & GMII_OMI_10BASE_T_FD ))
          {
            // clear speed/set full duplex
            bSpeed = FALSE;
            bFullDuplex = TRUE;
          }
          else if (( uStatus1 & GMII_10BASE_T_HD ) && ( uStatus2 & GMII_OMI_100BASE_TX_HD ))
          {
            // clear speed/set full duplex
            bSpeed = TRUE;
            bFullDuplex = FALSE;
          }
          else if (( uStatus1 & GMII_10BASE_T_HD ) && ( uStatus2 & GMII_OMI_10BASE_T_HD ))
          {
            // clear speed/set full duplex
            bSpeed = FALSE;
            bFullDuplex = FALSE;
          }
          
          // set the speed
          KSZ8051MNLPhy_SetSpeed( puGmacAddr, bSpeed );
          KSZ8051MNLPhy_EnableFullDuplex( puGmacAddr, bSpeed );
        }
      }
    }
    else
    {
      // set the invalid error
      eStatus = GMAC_STATUS_INVALID;
    }
  }
  else
  
  // disable the managmenet
  KSZ8051MNLPhy_EnableManagement( puGmacAddr, FALSE );
  
  // return the status
  return( eStatus );
}

/******************************************************************************
 * @function KSZ8051MNLPhy_AutoNegotiate
 *
 * @brief setup auto negotiation
 *
 * This function will set up auto negotiation
 *
 * @param[in]   puGmacAddr      pointer to the GMAC
 * @parma[in]   nPhyAddr    Phy address
 *
 * @return      approriate GMACSTATUS
 *
 *****************************************************************************/
GMACSTATUS KSZ8051MNLPhy_AutoNegotiate( PU32 puGmacAddr, U8 nPhyAddr )
{
  GMACSTATUS  eStatus = GMAC_STATUS_OK;
  U32         uValue, uPhyAnar, uPhyAnalPr, uRetryCount;
  BOOL        bSpeed, bFullDuplex, bRunFlag;
  U8          nAutoNegIdx = 0;
  
  // enable the managmenet
  KSZ8051MNLPhy_EnableManagement( puGmacAddr, TRUE );

  // clear the value 
  uValue = 0;
  
  // while runflag
  bRunFlag = TRUE;
  while(( eStatus == GMAC_STATUS_OK ) && ( bRunFlag == TRUE ))
  {
    // process step
    switch( nAutoNegIdx )
    {
      // read/modify the control register value
      case 0 :
      case 3 :
        eStatus = KSZ8051MNLPhy_PhyRead( puGmacAddr, nPhyAddr, GMII_BMCR, &uValue );
        nAutoNegIdx++;
        break;
        
      // clear auto negation
      case 1 :
        uValue &= ~(GMII_AUTONEG | GMII_LOOPBACK | GMII_POWER_DOWN );
        uValue |= GMII_ISOLATE;
        eStatus = KSZ8051MNLPhy_PhyWrite( puGmacAddr, nPhyAddr, GMII_BMCR, uValue );
        nAutoNegIdx++;
        break;
        
      // set up auto neg advertisement reg
      case 2 :
        uPhyAnar = ( GMII_100TX_FDX | GMII_100TX_HDX | GMII_10_FDX | GMII_10_HDX | GMII_AN_IEEE_802_3 );
        eStatus = KSZ8051MNLPhy_PhyWrite( puGmacAddr, nPhyAddr, GMII_ANAR, uPhyAnar );
        nAutoNegIdx++;
        break;
        
      // set up speed/duplex/mode
      case 4 :
        uValue |= ( GMII_SPEED_SELECT | GMII_AUTONEG | GMII_DUPLEX_MODE );
        eStatus = KSZ8051MNLPhy_PhyWrite( puGmacAddr, nPhyAddr, GMII_BMCR, uValue );
        nAutoNegIdx++;
        break;
        
      // restart auto negotiation
      case 5 :
        uValue |= GMII_RESTART_AUTONEG;
        uValue &= ~( GMII_ISOLATE );
        eStatus = KSZ8051MNLPhy_PhyWrite( puGmacAddr, nPhyAddr, GMII_BMCR, uValue );
        nAutoNegIdx++;
        uRetryCount = 0;
        break;
      
      // check for done
      case 6 :

        eStatus = KSZ8051MNLPhy_PhyRead( puGmacAddr, nPhyAddr, GMII_BMSR, &uValue );
        if ( uValue & GMII_AUTONEG_COMP )
        {
          // increment step
          nAutoNegIdx++;
        }
        else
        {
          if ( ++uRetryCount >= PHY_RETRY_MAX )
          {
            // force error
            eStatus = GMAC_STATUS_TIMEOUT;
          }
        }
        break;
        
      // get the auto negotiate link partner base page
      case 7 :
        eStatus = KSZ8051MNLPhy_PhyRead( puGmacAddr, nPhyAddr, GMII_ANLPAR, &uPhyAnalPr );
        nAutoNegIdx++;
        break;
        
      // set up link speed/duplex/enable transmit/receive
      case 8 :
        if (( uPhyAnar & uPhyAnalPr ) & GMII_100TX_FDX )
        {
          // set up MII for 100BaseT and Full duplex
          bSpeed = TRUE;
          bFullDuplex = TRUE;
        }
        else if (( uPhyAnar & uPhyAnalPr ) & GMII_10_FDX )
        {
          // set up MII for 10uPhyAnalPrand full duplex
          bSpeed = FALSE;
          bFullDuplex = TRUE;
        }
        else if (( uPhyAnar & uPhyAnalPr ) & GMII_100TX_HDX )
        {
          // set up MII for 100BaseT and half duplex
          bSpeed = TRUE;
          bFullDuplex = FALSE;
        }
        else if (( uPhyAnar & uPhyAnalPr ) & GMII_10_HDX )
        {
          // set up MII for 10BaseT and half duplex
          bSpeed = FALSE;
          bFullDuplex = FALSE;
        }
        
        // set up speed/duplex
        KSZ8051MNLPhy_SetSpeed( puGmacAddr, bSpeed );
        KSZ8051MNLPhy_EnableFullDuplex( puGmacAddr, bFullDuplex );
        
        // select MII
        KSZ8051MNLPhy_SelectMiiMode( puGmacAddr, GMAC_PHY_MII );
        
        // enabled transmit/receive
        KSZ8051MNLPhy_EnableTransmit( puGmacAddr, TRUE );
        KSZ8051MNLPhy_EnableReceive( puGmacAddr, TRUE );

        // clear the run flag
        bRunFlag = FALSE;
        break;
        
      default :
        // force error
        eStatus = GMAC_STATUS_TIMEOUT;
        break;
    }
  }
  
  // disable the managmenet
  KSZ8051MNLPhy_EnableManagement( puGmacAddr, FALSE );
  
  // return the status
  return( eStatus );
}

/******************************************************************************
 * @function KSZ8051MNLPhy_Reset
 *
 * @brief reset the PHY
 *
 * This function will reset the PHY
 *
 * @param[in]   puGmacAddr      pointer to the GMAC
 * @parma[in]   nPhyAddr    Phy address
 *
 * @return      approriate GMACSTATUS
 *
 *****************************************************************************/
GMACSTATUS KSZ8051MNLPhy_Reset( PU32 puGmacAddr, U8 nPhyAddr )
{
  U32         uBmcr;
  U32         uTimeout = PHY_TIMEOUT;
  GMACSTATUS  eStatus = GMAC_STATUS_TIMEOUT;
  
  // enable the managmenet
  KSZ8051MNLPhy_EnableManagement( puGmacAddr, TRUE );
  
  // reset the PHY
  uBmcr = GMII_RESET;
  KSZ8051MNLPhy_PhyWrite( puGmacAddr, nPhyAddr, GMII_BMCR, uBmcr );
  
  // now wait till reset is set or timeout
  do
  {
    // read the control register/test for reset
    KSZ8051MNLPhy_PhyRead( puGmacAddr, nPhyAddr, GMII_BMCR, &uBmcr );
  } while(( uBmcr & GMII_RESET ) && ( --uTimeout != 0 ));
  
  // disable the managmenet
  KSZ8051MNLPhy_EnableManagement( puGmacAddr, FALSE );

  // check for okey
  if ( uTimeout != 0 )
  {
    // set good status
    eStatus = GMAC_STATUS_OK;
  }
  
  // return the status
  return( eStatus );
}

/**@} EOF KSZ8051MNLPhy.c */
