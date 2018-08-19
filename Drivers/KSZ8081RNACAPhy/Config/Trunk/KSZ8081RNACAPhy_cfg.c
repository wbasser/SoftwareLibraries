/******************************************************************************
 * @file KSZ8081RNACAPhycfg.c
 *
 * @brief KSZ8081RNACAPhy Physical configuration implementaion
 *
 * This file provides the implementation for the PHY
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
 * \addtogroup KSZ8081RNACAPhy
 * @{
 *****************************************************************************/

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "KSZ8081RNACAPhy/KSZ8081RNACAPhy_cfg.h"

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations ----------------------------------------------

// local parameter declarations -----------------------------------------------

// local function prototypes --------------------------------------------------

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
 * @return      
 *
 *****************************************************************************/
void KSZ8081RNACAPhy_SetMdcClock( PU32 puGmacAddr, U32 uClockRate )
{
  // call the default function
  Gmac_SetMdcClock(( Gmac* )puGmacAddr, uClockRate );
}

/******************************************************************************
 * @function 
 *
 * @brief 
 *
 * This function 
 *
 * @param[in]   
 *
 * @return      
 *
 *****************************************************************************/
void KSZ8081RNACAPhy_EnableManagement( PU32 puGmacAddr, BOOL bState )
{
  // call the default function
  Gmac_EnableManagement(( Gmac* )puGmacAddr, bState );
}

/******************************************************************************
 * @function 
 *
 * @brief 
 *
 * This function 
 *
 * @param[in]   
 *
 * @return      
 *
 *****************************************************************************/
GMACSTATUS KSZ8081RNACAPhy_PhyWrite( PU32 puGmacAddr, U8 nPhyAddress, U8 naddress, U32 uValue )
{
  // write the value
  return( Gmac_PhyWrite(( Gmac* )puGmacAddr, nPhyAddress, naddress, uValue ));
}

/******************************************************************************
 * @function 
 *
 * @brief 
 *
 * This function 
 *
 * @param[in]   
 *
 * @return      
 *
 *****************************************************************************/
GMACSTATUS KSZ8081RNACAPhy_PhyRead( PU32 puGmacAddr, U8 nPhyAddress, U8 naddress, PU32 puValue )
{
  // read the value
  return( Gmac_PhyRead(( Gmac* )puGmacAddr, nPhyAddress, naddress, puValue ));
}

/******************************************************************************
 * @function 
 *
 * @brief 
 *
 * This function 
 *
 * @param[in]   
 *
 * @return      
 *
 *****************************************************************************/
void KSZ8081RNACAPhy_SetSpeed( PU32 puGmacAddr, BOOL bSpeed )
{
  // set the speed
  Gmac_SetSpeed(( Gmac* )puGmacAddr, bSpeed );
}

/******************************************************************************
 * @function 
 *
 * @brief 
 *
 * This function 
 *
 * @param[in]   
 *
 * @return      
 *
 *****************************************************************************/
void KSZ8081RNACAPhy_EnableFullDuplex( PU32 puGmacAddr, BOOL bFullDuplex )
{
  // set the duplex
  Gmac_EnableFullDuplex(( Gmac* )puGmacAddr, bFullDuplex );
}

/******************************************************************************
 * @function 
 *
 * @brief 
 *
 * This function 
 *
 * @param[in]   
 *
 * @return      
 *
 *****************************************************************************/
void KSZ8081RNACAPhy_SelectMiiMode( PU32 puGmacAddr, GMACMIIMODE eMode )
{
  // set the mode
  Gmac_SelectMiiMode(( Gmac* )puGmacAddr, eMode );
}

/******************************************************************************
 * @function 
 *
 * @brief 
 *
 * This function 
 *
 * @param[in]   
 *
 * @return      
 *
 *****************************************************************************/
void KSZ8081RNACAPhy_EnableTransmit( PU32 puGmacAddr, BOOL bState )
{
  // enable the transmit
  Gmac_EnableTransmit(( Gmac* )puGmacAddr, bState );
}
 
/******************************************************************************
 * @function 
 *
 * @brief 
 *
 * This function 
 *
 * @param[in]   
 *
 * @return      
 *
 *****************************************************************************/
void KSZ8081RNACAPhy_EnableReceive( PU32 puGmacAddr, BOOL bState )
{
  // enable the receive
  Gmac_EnableReceive(( Gmac* )puGmacAddr, bState );
}

/**@} EOF KSZ8081RNACAPhycfg.c */
