/******************************************************************************
 * @file KSZ8051MNLPhy_prv.h
 *
 * @brief KSZ8051MPN Physical Interface private declarations
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
 
// ensure only one instantiation
#ifndef _KSZ8051MNLPHY_PRV_H
#define _KSZ8051MNLPHY_PRV_H

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------
// IEEE defined Registers
#define GMII_BMCR               ( 0x00 )   // Basic Control
#define GMII_BMSR               ( 0x01 )   // Basic Status
#define GMII_PHYID1             ( 0x02 )   // PHY Idendifier 1
#define GMII_PHYID2             ( 0x03 )   // PHY Idendifier 2
#define GMII_ANAR               ( 0x04 )   // Auto_Negotiation Advertisement
#define GMII_ANLPAR             ( 0x05 )   // Auto_negotiation Link Partner Ability
#define GMII_ANER               ( 0x06 )   // Auto-negotiation Expansion
#define GMII_ANNPR              ( 0x07 )   // Auto-negotiation Next Page
#define GMII_ANLPNPAR           ( 0x08 )   // Link Partner Next Page Ability
#define GMII_AFECR1             ( 0x11 )   // AFE Control 1
#define GMII_RXERCR             ( 0x15 )   // RXER Counter

#define GMII_OMSOR              ( 0x16 )   // Operation Mode Strap Override
#define GMII_OMSSR              ( 0x17 )   // Operation Mode Strap Status
#define GMII_ECR                ( 0x18 )   // Expanded Control
#define GMII_ICSR               ( 0x1B )   // Interrupt Control/Status
#define GMII_LCSR               ( 0x1D )   // LinkMD Control/Status

//29-30 reserved
#define GMII_PCR1               ( 0x1E )   // PHY Control 1
#define GMII_PCR2               ( 0x1F )   // PHY Control 2

// Bit definitions: GMII_BMCR 0x00 Basic Control
#define GMII_RESET              (1 << 15) // 1= Software Reset; 0=Normal Operation
#define GMII_LOOPBACK           (1 << 14) // 1=loopback Enabled; 0=Normal Operation
#define GMII_SPEED_SELECT       (1 << 13) // 1=100Mbps; 0=10Mbps
#define GMII_AUTONEG            (1 << 12) // Auto-negotiation Enable
#define GMII_POWER_DOWN         (1 << 11) // 1=Power down 0=Normal operation
#define GMII_ISOLATE            (1 << 10) // 1 = Isolates 0 = Normal operation
#define GMII_RESTART_AUTONEG    (1 << 9)  // 1 = Restart auto-negotiation 0 = Normal operation
#define GMII_DUPLEX_MODE        (1 << 8)  // 1 = Full duplex operation 0 = Normal operation
#define GMII_COLLISION_TEST     (1 << 7)  // 1 = Enable COL test; 0 = Disable COL test

// Bit definitions: GMII_BMSR 0x01 Basic Status
#define GMII_100BASE_T4        (1 << 15) // 100BASE-T4 Capable
#define GMII_100BASE_TX_FD     (1 << 14) // 100BASE-TX Full Duplex Capable
#define GMII_100BASE_TX_HD     (1 << 13) // 100BASE-TX Half Duplex Capable
#define GMII_10BASE_T_FD       (1 << 12) // 10BASE-T Full Duplex Capable
#define GMII_10BASE_T_HD       (1 << 11) // 10BASE-T Half Duplex Capable
#define GMII_MF_PREAMB_SUPPR   (1 << 6)  // MII Frame Preamble Suppression
#define GMII_AUTONEG_COMP      (1 << 5)  // Auto-negotiation Complete
#define GMII_REMOTE_FAULT      (1 << 4)  // Remote Fault
#define GMII_AUTONEG_ABILITY   (1 << 3)  // Auto Configuration Ability
#define GMII_LINK_STATUS       (1 << 2)  // Link Status
#define GMII_JABBER_DETECT     (1 << 1)  // Jabber Detect
#define GMII_EXTEND_CAPAB      (1 << 0)  // Extended Capability


// Bit definitions: GMII_PHYID1 0x02 PHY Idendifier 1
// Bit definitions: GMII_PHYID2 0x03 PHY Idendifier 2
#define GMII_LSB_MASK           ( 0x3F )
#define GMII_OUI_MSB            ( 0x0022 )
#define GMII_OUI_LSB            ( 0x05 )


// Bit definitions: GMII_ANAR   0x04 Auto_Negotiation Advertisement
// Bit definitions: GMII_ANLPAR 0x05 Auto_negotiation Link Partner Ability
#define GMII_NP               (1 << 15)   // Next page Indication
#define GMII_RF               (1 << 13)   // Remote Fault
#define GMII_PAUSE_MASK       (3 << 10)   // 0,0 = No Pause 1,0 = Asymmetric Pause(link partner)
                                          // 0,1 = Symmetric Pause 1,1 = Symmetric&Asymmetric Pause(local device)   
#define GMII_100T4            (1 << 9)    // 100BASE-T4 Support
#define GMII_100TX_FDX        (1 << 8)    // 100BASE-TX Full Duplex Support
#define GMII_100TX_HDX        (1 << 7)    // 100BASE-TX Half Duplex Support
#define GMII_10_FDX           (1 << 6)    // 10BASE-T Full Duplex Support
#define GMII_10_HDX           (1 << 5)    // 10BASE-T Half Duplex Support
#define GMII_AN_IEEE_802_3    ( 0x0001 )  // [00001] = IEEE 802.3


// Bit definitions: GMII_ANER 0x06 Auto-negotiation Expansion
#define GMII_PDF              (1 << 4)    // Local Device Parallel Detection Fault
#define GMII_LP_NP_ABLE       (1 << 3)    // Link Partner Next Page Able
#define GMII_NP_ABLE          (1 << 2)    // Local Device Next Page Able
#define GMII_PAGE_RX          (1 << 1)    // New Page Received
#define GMII_LP_AN_ABLE       (1 << 0)    // Link Partner Auto-negotiation Able

// Bit definitions: GMII_PCR1 0x1E PHY Control 1
#define GMII_OMI_10BASE_T_HD    ( 0x0001 )
#define GMII_OMI_100BASE_TX_HD  ( 0x0002 )
#define GMII_OMI_10BASE_T_FD    ( 0x0005 )
#define GMII_OMI_100BASE_TX_FD	( 0x0006 )

/**@} EOF KSZ8051MNLPhy_prv.h */

#endif  // _KSZ8051MNLPHY_PRV_H