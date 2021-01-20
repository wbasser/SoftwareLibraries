/******************************************************************************
 * @file LTECellModem.h
 *
 * @brief LTE Cellular Modem driver 
 *
 * This file provides the declarations for the LTE cellular modem
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
 * $Rev: $
 * 
 *
 * \addtogroup LTECellModem
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _LTECELLMODEM_H
#define _LTECELLMODEM_H

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "LTECellModem/LTECellModem_cfg.h"

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------
/// define the number of events for the control task
#define LTECELLMODEM_CTLTASK_NUM_EVENTS             ( 4 )

/// define the number of events for the character task
#define LTECELLMODEM_CHRTASK_NUM_EVENTS             ( 80 )

/// define the receive/transmit buffer sizes
#define LTECELLMODEM_RCVBUF_SIZE                    ( 0 )
#define LTECELLMODEM_XMTBUF_SIZE                    ( 80 )

// enumerations ---------------------------------------------------------------
/// enumerate the errors
typedef enum  _LTEERR
{
  LTE_ERR_NONE          = 0,
  LTE_ERR_TIMEOUT       = -1,
  LTE_ERR_ERROR         = -2,
  LTE_ERR_ILLGPIOENUM   = -3,
  LTE_ERR_BUSY          = -4,
  LTE_ERR_ILLREQUEST    = -5,
} LTEERR;

/// enumerate the LTE GPIO's
typedef enum _LTEGPIO
{
  LTE_GPIO_1 = 0,
  LTE_GPIO_2,
  LTE_GPIO_3,
  LTE_GPIO_4,
  LTE_GPIO_5,
  LTE_GPIO_6,
  LTE_GPIO_MAX
} LTEGPIO;

/// enumerate the GPIO pin modes
typedef enum _LTEGPIOMODE
{
  LTE_GPIO_MODE_OUTPUT = 0,
  LTE_GPIO_MODE_INPUT,
  LTE_GPIO_MODE_NETWORK_STS,
  LTE_GPIO_MODE_GNSS_PWR_ENB,
  LTE_GPIO_MODE_GNSS_DATA_RDY,
  LTE_GPIO_MODE_GNSS_RTC_SHARE,
  LTE_GPIO_MODE_SIM_CARD_DETECT,
  LTE_GPIO_MODE_HEADSET_DETECT,
  LTE_GPIO_MODE_GSM_TXBURST,
  LTE_GPIO_MODE_OPER_STATUS,
  LTE_GPIO_MODE_FUNC_STATUS,
  LTE_GPIO_MODE_I2S_AUD_INTF,
  LTE_GPIO_MODE_SPI_INTF,
  LTE_GPIO_MODE_MSTCLK_GEN,
  LTE_GPIO_MODE_UART_INTF,
  LTE_GPIO_MODE_WIFI_ENB,
  LTE_GPIO_MODE_RING_INDIC,
  LTE_GPIO_MODE_LAST_GAP_ENB,
  LTE_GPIO_PAD_DISABLED = 255
} LTEGPIOMODE;

/// enumerate the request
typedef enum _LTEREQUEST
{
  LTE_REQUEST_CREG = 0,
  LTE_REQUEST_CGSN,
  LTE_REQUEST_CIMI,
  LTE_REQUEST_CCID,
  LTE_REQUEST_CSQ,
  LTE_REQUEST_MNO,
  LTE_REQUEST_APN,
  LTE_REQUEST_MAX
} LTEREQUEST;

/// enumerate the network operators
typedef enum _LTEMNO
{
  LTE_MNO_DEFAULT = 0,
  LTE_MNO_SIMICCD,
  LTE_MNO_ATT,
  LTE_MNO_VERIZON,
  LTE_MNO_TELSTRA,
  LTE_MNO_TMO,
  LTE_MNO_CT,
  LTE_MNO_MAX,
  LTE_MNO_ILLEGAL = 255
} LTEMNO;

/// enumerate the Network type
typedef enum _LTEPDP
{
  LTE_PDP_IP = 0,
  LTE_PDP_NONIP,
  LTE_PDP_IPV4V6,
  LTE_PDP_IPV6,
  LTE_PDP_MAX,
  LTE_PDP_ILLEGAL
} LTEPDP;

/// enumerate the socket protocol
typedef enum _LTESKTPROT
{
  LTE_SKTPROT_TCP = 6,
  LTE_SKTPROT_UDP = 17
} LTESKTPROT;

/// enumerate the MQTT clean type
typedef enum _LTEMQTTCLEAN
{
  LTE_MQTTCLEAN_KEEPMSGS = 0,
  LTE_MQTTCLEAN_DELETALL,
  LTE_MQTTCLEAN_MAX
} LTEMQTTCLEAN;

// structures -----------------------------------------------------------------
/// define the callback
typedef void  ( *PVLTECALLBACK )( LTEERR, PU8 );

// global parameter declarations -----------------------------------------------

// global function prototypes --------------------------------------------------
extern  void    LTECellModem_Intialize( void );
extern  void    LTECellModem_CharProcess( U8 nChar );
extern  void    LTECellModem_CtrlProcess( LTELCLEVENT eEvent );
extern  LTEERR  LTECellModem_EchoControl( BOOL bState, PVLTECALLBACK pvCallback );
extern  LTEERR  LTECellModem_SendRequest( LTEREQUEST eRequest, PVLTECALLBACK pvCallback );
extern  LTEERR  LTECellModem_SetOperator( LTEMNO eMno, PVLTECALLBACK pvCallback );
extern  LTEERR  LTECellModem_SetAPN( PC8 pszOperator, LTEPDP ePdp, PVLTECALLBACK pvCallback );
extern  LTEERR  LTECellModem_SetGpio( LTEGPIO eGpioEnum, LTEGPIOMODE eGpioMode, PVLTECALLBACK pvCallback );
//extern  LTEERR  LTECellModem_GetGpio( LTEGPIO eGpioEnum, PLTEGPIOMODE peGpioMode, PVLTECALLBACK pvCallback );
extern  LTEERR  LTECellModem_SetAutoTimeZone( BOOL bState, PVLTECALLBACK pvCallback );
extern  LTEERR  LTECellModem_SocketOpen( LTESKTPROT eProtocol, U16 wLocalPort, PVLTECALLBACK pvCallback );
extern  LTEERR  LTECellModem_SocketClose( S8 cSocket, PVLTECALLBACK pvCallback );
extern  LTEERR  LTECellModem_SocketConnect( S8 cSocket, PCC8 pszAddress, U16 wPort, PVLTECALLBACK pvCallback ); 
extern  LTEERR  LTECellModem_SocketListen( S8 cSocket, U16 wPort, PVLTECALLBACK pvCallback );
extern  LTEERR  LTECellModem_SocketRead( S8 cSocket, PU8 pnBuffer, U16 wLength, PVLTECALLBACK pvCallback );
extern  LTEERR  LTECellModem_SocketWrite( S8 cSocket, PU8 pnBUffer, U16 wLength, PVLTECALLBACK pvCallback );
extern  LTEERR  LTECellModem_MqttCOnfigureClientID( U16 wClientId, PVLTECALLBACK pvCallback ); 
extern  LTEERR  LTECellModem_MqttConfigureLclPort( U16 wLocalPort, PVLTECALLBACK pvCallback ); 
extern  LTEERR  LTECellModem_MqttConfigureServerName( PC8 pszServerName, PVLTECALLBACK pvCallback );
extern  LTEERR  LTECellModem_MqttConfigureIpAddr( PC8 pszIpAddr, PVLTECALLBACK pvCallback );
extern  LTEERR  LTECellModem_MqttConfigureUserCred( PC8 pszName, PC8 pszPassword, PVLTECALLBACK pvCallback );
extern  LTEERR  LTECellModem_MqttConfigureTimeout( U32 uTimeout, PVLTECALLBACK pvCallback );
extern  LTEERR  LTECellModem_MqttConfigureSecure( BOOL bState, PVLTECALLBACK pvCallback );
extern  LTEERR  LTECellModem_MqttConfigureClean( LTEMQTTCLEAN eLteClean, PVLTECALLBACK pvCallback );
extern  LTEERR  LTECellModem_Mqtt
/**@} EOF LTECellModem.h */

#endif  // _LTECELLMODEM_H