/******************************************************************************
 * @file Gmac.h
 *
 * @brief Media Access Controller Declarations
 *
 * This file provides the declarations for the media access controller
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
 * \addtogroup Gmac
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _GMAC_H
#define _GMAC_H

// system includes ------------------------------------------------------------
#include "Types/Types.h"

// local includes -------------------------------------------------------------

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------
#define GMAC_RXD_ADDR_MASK      0xFFFFFFFC
#define GMAC_RXD_WRAP           (1ul << 1)  /**< Wrap bit */
#define GMAC_RXD_OWNERSHIP      (1ul << 0)  /**< Ownership bit */

#define GMAC_RXD_BROADCAST      (1ul << 31) /**< Broadcast detected */
#define GMAC_RXD_MULTIHASH      (1ul << 30) /**< Multicast hash match */
#define GMAC_RXD_UNIHASH        (1ul << 29) /**< Unicast hash match */
#define GMAC_RXD_ADDR_FOUND     (1ul << 27) /**< Specific address match found */
#define GMAC_RXD_ADDR           (3ul << 25) /**< Address match */
#define GMAC_RXD_RXCOEN         (1ul << 24) /**< RXCOEN related function */
#define GMAC_RXD_TYPE           (3ul << 22) /**< Type ID match */
#define GMAC_RXD_VLAN           (1ul << 21) /**< VLAN tag detected */
#define GMAC_RXD_PRIORITY       (1ul << 20) /**< Priority tag detected */
#define GMAC_RXD_PRIORITY_MASK  (3ul << 17) /**< VLAN priority */
#define GMAC_RXD_CFI            (1ul << 16) /**< Concatenation Format Indicator only if bit 21 is set */
#define GMAC_RXD_EOF            (1ul << 15) /**< End of frame */
#define GMAC_RXD_SOF            (1ul << 14) /**< Start of frame */
#define GMAC_RXD_FCS            (1ul << 13) /**< Frame check sequence */
#define GMAC_RXD_OFFSET_MASK                /**< Receive buffer offset */
#define GMAC_RXD_LEN_MASK       (0xFFF)     /**< Length of frame including FCS (if selected) */
#define GMAC_RXD_LENJUMBO_MASK  (0x3FFF)    /**< Jumbo frame length */

#define GMAC_TXD_USED           (1ul << 31) /**< Frame is transmitted */
#define GMAC_TXD_WRAP           (1ul << 30) /**< Last descriptor */
#define GMAC_TXD_ERROR          (1ul << 29) /**< Retry limit exceeded, error */
#define GMAC_TXD_UNDERRUN       (1ul << 28) /**< Transmit underrun */
#define GMAC_TXD_EXHAUSTED      (1ul << 27) /**< Buffer exhausted */
#define GMAC_TXD_LATE           (1ul << 26) /**< Late collision,transmit  error  */
#define GMAC_TXD_CHECKSUM_ERROR (7ul << 20) /**< Checksum error */
#define GMAC_TXD_NOCRC          (1ul << 16) /**< No CRC */
#define GMAC_TXD_LAST           (1ul << 15) /**< Last buffer in frame */
#define GMAC_TXD_LEN_MASK       (0x1FFF)     /**< Length of buffer */

/** The MAC can support frame lengths up to 1536 bytes */
#define GMAC_FRAME_LENTGH_MAX   1536

#define GMAC_RX_UNITSIZE        128     /**< Fixed size for RX buffer  */
#define GMAC_TX_UNITSIZE        1518    /**< Size for ETH frame length */

/** GMAC clock speed */
#define GMAC_MCK_SPEED_240MHZ   (240000000u)
#define GMAC_MCK_SPEED_160MHZ   (160000000u)
#define GMAC_MCK_SPEED_120MHZ   (120000000u)
#define GMAC_MCK_SPEED_80MHZ    (80000000u)
#define GMAC_MCK_SPEED_40MHZ    (40000000u)
#define GMAC_MCK_SPEED_20MHZ    (20000000u)

/** GMAC maintain code default value*/
#define GMAC_MAN_CODE_VALUE     10

/** GMAC maintain start of frame default value*/
#define GMAC_MAN_SOF_VALUE      1

/** GMAC maintain read/write*/
#define GMAC_MAN_RW_TYPE        2

/** GMAC maintain read only*/
#define GMAC_MAN_READ_ONLY      1

/** GMAC address length */
#define GMAC_ADDR_LENGTH        6

#define GMAC_DUPLEX_HALF        0
#define GMAC_DUPLEX_FULL        1

#define GMAC_SPEED_10M          0
#define GMAC_SPEED_100M         1

/// special compile pack alighment macros
#define COMPILER_PRAGMA(arg)            _Pragma(#arg)
#define COMPILER_PACK_SET(alignment)    COMPILER_PRAGMA(pack(alignment))
#define COMPILER_PACK_RESET()           COMPILER_PRAGMA(pack())

// enumerations ---------------------------------------------------------------
/// enumerate the status codes
typedef enum  _GMACSTATUS
{
	GMAC_STATUS_OK = 0,           ///< OK
	GMAC_STATUS_TIMEOUT,          ///< GMAC operation timeout 
	GMAC_STATUS_TX_BUSY,          ///< TX in progress
	GMAC_STATUS_RX_ERROR,         ///< RX error
	GMAC_STATUS_RX_NO_DATA,       ///< No data received
	GMAC_STATUS_SIZE_TOO_SMALL,   ///< Buffer size not enough
	GMAC_STATUS_PARAM,            ///< Parameter error, TX packet invalid or RX size too small
	GMAC_STATUS_INVALID = 0xFF,   ///< Invalid 
} GMACSTATUS;

/// enumerate the MII modes
typedef enum  _GMACMIIMODE
{
	GMAC_PHY_MII = 0,             ///< MII mode
	GMAC_PHY_RMII,                ///< Reduced MII mode
	GMAC_PHY_INVALID = 0xFF,      ///< Invalid mode
} GMACMIIMODE;

// structures -----------------------------------------------------------------
COMPILER_PACK_SET( 8 )
/// define the RX Descriptor
typedef struct _GMACRXDESCRIPTOR
{
	union
  {
		struct
    {
			U32       bOwnership  :  1; //< User clear, GMAC sets this to 1 once it has successfully written a frame to memory
			U32       bWrap       :  1; //< Marks last descriptor in receive buffer
			U32       uAddrDWw    : 30; //< address
		} tBmFields;
		U32         uValue;
	} tAddr;
	union
  {
		struct 
    {
			U32       uLen              : 13;   //< Length of frame including FCS
			U32       bFcs              :  1;   //< Receive buffer offset, bits 13:12 of frame length for jumbo frame 
			U32       bSof              :  1;   //< Start of frame
			U32       bEof              :  1;   //< End of frame
			U32       bCfi              :  1;   //< Concatenation Format Indicator
			U32       uVlanPriority     :  3;   //< VLAN priority (if VLAN detected)
			U32       bPriorityDetected :  1;   //< Priority tag detected
			U32       bVlanDetected     :  1;   //< VLAN tag detected
			U32       uTypeIdMatch      :  2;   //< Type ID match 
			U32       bCheckSumOffload  :  1;   //< Checksum offload specific function
			U32       uAddrMatch        :  2;   //< Address register match
			U32       bExtAddrMatch     :  1;   //< External address match found
			U32       bReserved         :  1;   //< 
			U32       bUniHashMatch     :  1;   //< Unicast hash match
			U32       bMultiHashMatch   :  1;   //< Multicast hash match
			U32       bBroadcastDetect  :  1;   //< Global broadcast address detected
		} tBmFields;
		U32         uValue;
	} tStatus;
} GMACRXDESCRIPTOR, *PGMACRXDESCRIPTOR;
#define GMACRXDESCRIPTOR_SIZE             sizeof( GMACRXDESCRIPTOR )

/// define the TX Descriptor
COMPILER_PACK_SET( 8 )
typedef struct _GMACTXDESCRIPTOR
{
  U32 uAddress;
	union
  {
		struct 
    {
			U32       uLen              : 14;   //< Length of buffer
			U32       bReserved         :  1;
			U32       bLastBuffer       :  1;   //< Last buffer (in the current frame)
			U32       bNoCrc            :  1;   //< No CRC 
			U32       uReserved1        :  3;
			U32       uCheckSumOffload  :  3;   //< Transmit checksum generation offload errors
			U32       uReserved2        :  3;
			U32       bLco              :  1;   //< Late collision, transmit error detected
			U32       bExhausted        :  1;   //< Buffer exhausted in mid frame
			U32       bUnderrun         :  1;   //< Transmit underrun 
			U32       bError            :  1;   //< Retry limit exceeded, error detected
			U32       bWrap             :  1;   //< Marks last descriptor in TD list
			U32       bUsed             :  1;   //< User clear, GMAC sets this to 1 once a frame has been successfully transmitted
		} tBmFields;
    U32 uValue;
	} tStatus;
} GMACTXDESCRIPTOR, *PGMACTXDESCRIPTOR;
#define GMACTXDESCRIPTOR_SIZE             sizeof( GMACTXDESCRIPTOR )

// reset the compiler pack
COMPILER_PACK_RESET()

// global parameter declarations -----------------------------------------------

// global function prototypes --------------------------------------------------
extern  void        Gmac_NetworkControl( Gmac* ptGmac, U32 uNcr );
extern  U32         Gmac_GetNetworkControl( Gmac* ptGmac );
extern  void        Gmac_EnableReceive( Gmac* ptGmac, BOOL bEnable );
extern  void        Gmac_EnableTransmit( Gmac* ptGmac, BOOL bEnable );
extern  void        Gmac_EnableManagement( Gmac* ptGmac, BOOL bEnable );
extern  void        Gmac_ClearStatistics( Gmac* ptGmac );
extern  void        Gmac_IncreaseStatistics( Gmac* ptGmac );
extern  void        Gmac_EnableStatisticsWrite( Gmac* ptGmac, BOOL bEnable );
extern  void        Gmac_EnableBackPressure( Gmac* ptGmac, BOOL bEnable );
extern  void        Gmac_StartTransmission( Gmac* ptGmac );
extern  void        Gmac_HaltTransmission( Gmac* ptGmac );
extern  void        Gmac_TxPauseFrame( Gmac* ptGmac );
extern  void        Gmac_TxPauseZeroQuantumFrame( Gmac* ptGmac );
extern  void        Gmac_StoreRxTimeStamp( Gmac* ptGmac, BOOL bEnable );
extern  void        Gmac_EnablePfcPauseFrame( Gmac* ptGmac, BOOL bEnable );
extern  void        Gmac_TransmitPfcPauseFrame( Gmac* ptGmac );
extern  void        Gmac_FlushNextPacket( Gmac* ptGmac  );
extern  void        Gmac_SetConfig( Gmac* ptGmac, U32 uCfg );
extern  U32         Gmac_GetConfig( Gmac* ptGmac );
extern  void        Gmac_SetSpeed( Gmac* ptGmac, BOOL bEnable );
extern  void        Gmac_EnableFullDuplex( Gmac* ptGmac, BOOL bEnable );
extern  void        Gmac_EnableCopyAll( Gmac* ptGmac, BOOL bEnable );
extern  void        Gmac_EnableJumboFrames( Gmac* ptGmac, BOOL bEnable );
extern  void        Gmac_DisableBroadcast( Gmac* ptGmac, BOOL bEnable );
extern  void        Gmac_EnableMulticastHash( Gmac* ptGmac, BOOL bEnable );
extern  void        Gmac_EnableBigFrame( Gmac* ptGmac, BOOL bEnable );
extern  GMACSTATUS  Gmac_SetMdcClock( Gmac* ptGmac, U32 uMck );
extern  void        Gmac_EnableRetryTest( Gmac* ptGmac, BOOL bEnable );
extern  void        Gmac_EnablePauseRrame( Gmac* ptGmac, BOOL bEnable );
extern  void        Gmac_SetRxBufferOffset( Gmac* ptGmac, U8 nOffset );
extern  void        Gmac_EnableRxLengthCheck( Gmac* ptGmac, BOOL bEnable );
extern  void        Gmac_EnableDiscardFcs( Gmac* ptGmac, BOOL bEnable );
extern  void        Gmac_EnableEfrhd( Gmac* ptGmac, BOOL bEnable );
extern  void        Gmac_EnableIgnoreRxFcs( Gmac* ptGmac, BOOL bEnable );
extern  U32         Gmac_GetStatus( Gmac* ptGmac );
extern  BOOL        Gmac_GetMDIO( Gmac* ptGmac );
extern  BOOL        Gmac_IsPhyIdle(Gmac* ptGmac );
extern  U32         Gmac_GetTxStatus( Gmac* ptGmac );
extern  void        Gmac_ClearTxStatus( Gmac* ptGmac, U32 uStatus );
extern  U32         Gmac_GetRxStatus( Gmac* ptGmac );
extern  void        Gmac_ClearRxStatus( Gmac* ptGmac, U32 uStatus );
extern  void        Gmac_SetRxQueue( Gmac* ptGmac, U32 uAddr );
extern  void        Gmac_SetRxBufSize( Gmac* ptGmac, U32 uCode );
extern  U32         Gmac_GetRxQueue( Gmac* ptGmac );
extern  void        Gmac_SetTxQueue( Gmac* ptGmac, U32 uAddr );
extern  U32         Gmac_GetTxQueue( Gmac* ptGmac );
extern  void        Gmac_EnableInterrupt( Gmac* ptGmac, U32 uSource );
extern  void        Gmac_DisableInterrupt( Gmac* ptGmac, U32 uSource );
extern  U32         Gmac_GetInterruptStatus( Gmac* ptGmac );
extern  U32         Gmac_GetInterruptMask( Gmac* ptGmac );
extern  void        Gmac_MaintainPhy( Gmac* ptGmac, U8 nPhyAddr, U8 nRegAddr, U8 nRw, U16 wData );
extern  U16         Gmac_GetPhyData( Gmac* ptGmac );
extern  void        Gmac_SetHash( Gmac* ptGmac, U32 uHashTtop, U32 uHashBbottom );
extern  void        Gmac_SetHash64( Gmac* ptGmac, U64 hHash );
extern  void        Gmac_SetAddress( Gmac* ptGmac, U8 nindex, PU8 pnMacAddr );
extern  void        Gmac_SetAddress32( Gmac* ptGmac, U8 nindex,	U32 uMacTop, U32 uMacBottom );
extern  void        Gmac_SetAddress64( Gmac* ptGmac, U8 nindex,	U64 hMac );
extern  void        Gmac_SelectMiiMode( Gmac* ptGmac, GMACMIIMODE eMode );
extern  GMACSTATUS  Gmac_PhyRead( Gmac* ptGmac, U8 nPhyAddress, U8 naddress, PU32 puValue );
extern  GMACSTATUS  Gmac_PhyWrite( Gmac* ptGmac, U8 nPhyAddress, U8 naddress, U32 uValue );

/**@} EOF Gmac.h */

#endif  // _GMAC_H