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

// library includes -----------------------------------------------------------
#include "TaskManager/TaskManager.h"

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
  LTE_ERR_NONE = 0,
  LTE_ERR_TIMEOUT = -1,
  LTE_ERR_ERROR = -2,
  LTE_ERR_ILLGPIOENUM = -3,
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
} LTEGPIOMODE, *PLTEGPIOMODE;

// structures -----------------------------------------------------------------
/// define the callback
typedef void  ( *PVLTECALLBACK )( LTEERR, PVOID );

// global parameter declarations -----------------------------------------------

// global function prototypes --------------------------------------------------
extern  void    LTECellModem_Intialize( void );
extern  void    LTECellModem_PutChar( U8 nChar );
extern  void    LTECellModem_Write( PC8 pcData, U16 wLength );
extern  BOOL    LTECellModem_ProcessChar( TASKARG xArg );
extern  BOOL    LTECellModem_ProcessCtrl( TASKARG xArg );
extern  void    LTECellModem_EchoControl( BOOL bState, PVLTECALLBACK pvCallback );
extern  LTEERR  LTECellModem_SetGpio( LTEGPIO eGpioEnum, LTEGPIOMODE eGpioMode, PVLTECALLBACK pvCallback );
extern  LTEERR  LTECellModem_GetGpio( LTEGPIO eGpioEnum, PLTEGPIOMODE peGpioMode, PVLTECALLBACK pvCallback );
extern  void    LTECellModem_SetAutoTimeZone( BOOL bState, PVLTECALLBACK pvCallback );

/**@} EOF LTECellModem.h */

#endif  // _LTECELLMODEM_H