/******************************************************************************
 * @file LTECellModem.c
 *
 * @brief LTE Cellular modem implementation
 *
 * This file provides the implementation for the LTE cellular modem
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

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "LTECellModem/LTECellModem.h"

// library includes -----------------------------------------------------------
#include "GPIO/Gpio.h"
#include "RedirectionManager/RedirectionManager.h"
#include "UART/Uart.h"

// Macros and Defines ---------------------------------------------------------
/// define the size of the transmit/receive buffers
#define XMIT_BUF_SIZE                       ( 512 )
#define RECV_BUF_SIZE                       ( 512 )

/// define the message timeout
#define XMIT_MSG_TMO                        ( TASK_TIME_MSECS( 500 ))

// enumerations ---------------------------------------------------------------
/// enumerate the states
typedef enum _CTLSTATE
{
  CTL_STATE_PWRUP = 0,
  CTL_STATE_PWRDLY,
  CTL_STATE_IDLE,
  CTL_STATE_WAIT,
  CTL_STATE_MAX
} CTLSTATE;

/// enumerate the events
typedef enum _CTLEVENT
{
  CTL_EVENT_XMTMSG = 0xCE10,
  CTL_EVENT_RCVMSG,
} CTLEVENT;

// structures -----------------------------------------------------------------

// global parameter declarations ----------------------------------------------

// local parameter declarations -----------------------------------------------
static  CTLSTATE      eCtlState;
static  U8            anXmitBuffer[ XMIT_BUF_SIZE ];
static  U8            anRecvBuffer[ RECV_BUF_SIZE ];
static  U16           wXmitBufIdx;
static  U16           wRecvBufIdx;
static  PVLTECALLBACK pvCurCallback;

// local function prototypes --------------------------------------------------
static  void  ParseResponse( void );
static  PVOID ParseResponseValue( void );

// constant parameter initializations -----------------------------------------
/// define the commands
/// general
static  const CODE C8  szCmdAT[ ]        = { "AT" };
static  const CODE C8  szCmdECHO[ ]      = { "E%d" };
static  const CODE C8  szCmdCGSN[ ]      = { "+CGSN" };
static  const CODE C8  szCmdCIMI[ ]      = { "+CIMI" };
static  const CODE C8  szCmdCCID[]       = { "+CCID" };

/// control and status
static  const CODE C8  szCmdCFUN[ ]      = { "+CFUN" };
static  const CODE C8  szCmdCCLK[ ]      = { "+CCLK" };
static  const CODE C8  szCmdCTZU[ ]      = { "+CTZU" };

/// Network service
static  const CODE C8  szCmdUMNOPROF[ ]  = { "+UMNOPROF" }; 
static  const CODE C8  szCmdCSQ[ ]       = { "+CSQ" };
static  const CODE C8  szCmdCREG[ ]      = { "+CREG" };
static  const CODE C8  szCmdCGDCONT[ ]   = { "+CGDCONT" };
static  const CODE C8  szCmdPPP[ ]       = { "D" };
static  const CODE C8  szCmdCOPS[ ]      = { "+COPS" };

/// baud rate
static  const CODE C8  szCmdIPR[ ]       = { "+IPR" };

/// GPIO
static  const CODE C8  szCmdUGPIOC[ ]    = { "+UGPIOC=%d,%d" };

/// IP
static  const CODE C8  szCmdUSOCR[ ]     = { "+USOCR" };
static  const CODE C8  szCmdUSCOL[ ]     = { "+USOCL" };
static  const CODE C8  szCmdUSOCO[ ]     = { "+USOCO" };
static  const CODE C8  szCmdUSOWR[ ]     = { "+USOWR" };
static  const CODE C8  szCmdUSORD[ ]     = { "+USORD" };
static  const CODE C8  szCmdUSOLI[ ]     = { "+USOLI" };

/// SMS
static  const CODE C8  szCmdCMGF[ ]      = { "+CMGF" };
static  const CODE C8  szCmdCMGS[ ]      = { "+CMGS" };

/// location
static  const CODE C8  szCmdUPGS[ ]      = { "+UGPS" };
static  const CODE C8  szCmdULOC[ ]      = { "+ULOC" };
static  const CODE C8  szCmdUGPRMC[ ]    = { "+UGRMC" };

/// MQQT

/// on/off codes
static  const CODE C8 szOptOff[ ]       = { "0" };
static  const CODE C8 szOptOn[ ]        = { "1" };

/// response codes
static  const CODE C8 szRspOK[ ]        = { "OK" };
static  const CODE C8 szRspError[ ]     = { "ERROR" };

/// general format strings
static  const CODE C8 szFmtCmd[ ]       = { "%s" };

/// GPIO mapping
static  const CODE U8 anGpioMode[ LTE_GPIO_MAX] =
{
  16, 23, 24, 25, 42, 19
};

/******************************************************************************
 * @function LTECellModem_Intialize
 *
 * @brief LTE cellular modem initialization
 *
 * This function initializes the cellular modem
 *
 *****************************************************************************/
void LTECellModem_Intialize( void )
{
  // set the state
  eCtlState = CTL_STATE_PWRUP;

  // clear the buffer indices
  wXmitBufIdx = wRecvBufIdx = 0;
}

/******************************************************************************
 * @function LTECellModem_PutChar
 *
 * @brief output a single character
 *
 * This function will write a single character
 *
 * @param[in]    nData     data to send
 *
 *****************************************************************************/
void LTECellModem_PutChar( U8 nData )
{
  U16 wBytesWritten = 0;

  // write a byte of data to the device
  Uart_Write( UART_DEV_ENUM_LTE, ( PU8 )&nData, 1, &wBytesWritten );
}

/******************************************************************************
 * @function LTECellModem_Write
 *
 * @brief output a block of characters
 *
 * This function will output a block of characters to the approrpiate device
 *
 * @param[in]    pcData     pointer to the data to send
 * @param[in]   nLength     length of the data
 *
 *****************************************************************************/
void LTECellModem_Write( PC8 pcData, U16 wLength )
{
  U16 wBytesWritten = 0;

  // write a byte of data to the device
  do
  {
    // write a block to the UART
    Uart_Write( UART_DEV_ENUM_LTE, ( PU8 )pcData, wLength, &wBytesWritten );
    wLength -= wBytesWritten;
    pcData += wBytesWritten;
  } while( wLength != 0 ); 
}

/******************************************************************************
 * @function LTECellModem_ProcessChar
 *
 * @brief process character task
 *
 * This function will process the incoming character
 *
 * @param[in]   xArg    task argument
 *
 * @return      TRUE to flush events
 *
 *****************************************************************************/
BOOL LTECellModem_ProcessChar( TASKARG xArg )
{
  // process redirection
  if ( !RedirectionManager_RedirectChar( REDMNGR_DEV_LTE, ( U8 )xArg ))
  {
    // add the character to the buffer
    anRecvBuffer[ wRecvBufIdx++ ] = ( U8 )xArg;

    // check for a carriage return
    if ( xArg == '\r' )
    {
      // message received - terminate it
      anRecvBuffer[ wRecvBufIdx - 1 ] = '\0';

      // parse the buffer
      ParseResponse( );

      // in all cases - post a message received event
      TaskManager_PostEvent( TASK_SCHD_ENUM_LEDCTL, CTL_EVENT_RCVMSG );
    }
  }

  // return true to flush event
  return( TRUE );
}

/******************************************************************************
 * @function LTECellModem_ProcessCtrl
 *
 * @brief process control task
 *
 * This function will process the control event
 *
 * @param[in]   xArg    task argument
 *
 * @return      TRUE to flush events
 *
 *****************************************************************************/
BOOL LTECellModem_ProcessCtrl( TASKARG xArg )
{
  // determine the state
  switch( eCtlState )
  {
    case CTL_STATE_PWRUP :
      // turn on the power control/start a time delay
      Gpio_Set( GPIO_PIN_ENUM_CELPWR, ON );
      TaskManager_StartTimer( TASK_SCHD_ENUM_LTECTRL, TASK_TIME_SECS( 3 ));
      eCtlState = CTL_STATE_PWRDLY;
      break;

    case CTL_STATE_PWRDLY :
      // if this is a timeout - reset the 
      if ( xArg == TASK_TIMEOUT_EVENT )
      {
        Gpio_Set( GPIO_PIN_ENUM_CELPWR, OFF );
        eCtlState = CTL_STATE_IDLE;
      }
      break;

    case CTL_STATE_IDLE :
      // check for a message request
      if ( xArg == CTL_EVENT_XMTMSG )
      {
        // send it/start timer/goto wait state
        LTECellModem_Write( anXmitBuffer, wXmitBufIdx );
        TaskManager_StartTimer( TASK_SCHD_ENUM_LTECTRL, XMIT_MSG_TMO );
        eCtlState = CTL_STATE_WAIT;
      }
      break;

    case CTL_STATE_WAIT :
      // determine the event
      switch( xArg )
      {
        case CTL_EVENT_RCVMSG :
          // stop the timer
          TaskManager_StopTimer( TASK_SCHD_ENUM_LTECTRL );
          break;

        case TASK_TIMEOUT_EVENT :
          // message timeout
          if ( pvCurCallback != NULL )
          {
            // call the callback
            pvCurCallback( LTE_ERR_TIMEOUT, NULL );
          }
          break;

        default :
          break;
      }
      break;

    default :
      eCtlState = CTL_STATE_IDLE;
      break;
  }

  // return true to flush event
  return( TRUE );
}

/******************************************************************************
 * @function LteCellModem_EchoControl
 *
 * @brief turn on/off the echo on the modem
 *
 * This function will ping and determine if the modem is there
 *
 * @param[in]   bState        state of the echo
 * @param[in]   pvCallback    pointer to the callback function
 *
 *****************************************************************************/
void LteCellModem_EchoControlOff( BOOL bState, PVLTECALLBACK pvCallback )
{
  // store the callback
  pvCurCallback = pvCallback;

  // build the command
  wXmitBufIdx = SPRINTF_P( anXmitBuffer, ( const char* )szFmtCmd, ( const char * )szCmdAT );
  wXmitBufIdx += SPRINTF_P( &anXmitBuffer[ wXmitBufIdx ], ( const char *)szCmdECHO, bState );
  anXmitBuffer[ wXmitBufIdx++ ] = '\r';

  // post the message
  TaskManager_PostEvent( TASK_SCHD_ENUM_LTECTRL, CTL_EVENT_XMTMSG );
}

/******************************************************************************
 * @function LTECellModem_SetGpio
 *
 * @brief set the gpio
 *
 * This function sill set the GPIO to a givenfunction
 *
 * @param[in]   eGpioEnum     desried GPIO enumeration
 * @param[in]   eGpioMode     desired GPIO mode
 * @param[in]   pvCallback    pointer to the callback function
 *
 * @return      appropriate error
 *
 *****************************************************************************/
LTEERR LTECellModem_SetGpio( LTEGPIO eGpioEnum, LTEGPIOMODE eGpioMode, PVLTECALLBACK pvCallback )
{
  LTEERR eError = LTE_ERR_NONE;

  // save the callback
  pvCurCallback = pvCallback;

  // test for valid enum
  if ( eGpioEnum < LTE_GPIO_MAX )
  {
    // build the command
    wXmitBufIdx = SPRINTF_P( anXmitBuffer, ( const char* )szFmtCmd, ( const char * )szCmdAT );
    wXmitBufIdx += SPRINTF_P( &anXmitBuffer[ wXmitBufIdx ], ( const char *)szCmdUGPIOC, eGpioEnum, anGpioMode[ eGpioMode ] );
    anXmitBuffer[ wXmitBufIdx++ ] = '\r';

    // post the message
    TaskManager_PostEvent( TASK_SCHD_ENUM_LTECTRL, CTL_EVENT_XMTMSG );
  }
  else
  {
    // set the error
    eError = LTE_ERR_ILLGPIOENUM;
  }

  // return the error
  return( eError );
}

/******************************************************************************
 * @function LTECellModem_GetGpio
 *
 * @brief wet the gpio
 *
 * This function will get the GPIO to a givenfunction
 *
 * @param[in]   eGpioEnum     desried GPIO enumeration
 * @param[in]   eGpioMode     desired GPIO mode
 * @param[in]   pvCallback    pointer to the callback function
 *
 * @return      appropriate error
 *
 *****************************************************************************/
LTEERR LTECellModem_GetGpio( LTEGPIO eGpioEnum, PLTEGPIOMODE peGpioMode, PVLTECALLBACK pvCallback )
{
  LTEERR eError = LTE_ERR_NONE;

  // save the callback
  pvCurCallback = pvCallback;

  // test for valid enum
  if ( eGpioEnum < LTE_GPIO_MAX )
  {
  }
  else
  {
    // set the error
    eError = LTE_ERR_ILLGPIOENUM;
  }

  // return the error
  return( eError );
}

/******************************************************************************
 * @function LteCellModem_SetAutoTimeZone
 *
 * @brief enable/disable the auto time zone
 *
 * This function will enable/disable the auto time zone
 *
 * @param[in]   bState        auto time zone state
 * @param[in]   pvCallback    pointer to the callback function
 *
 *****************************************************************************/
void LteCellModem_SetAutoTimeZone( BOOL bState, PVLTECALLBACK pvCallback )
{
  // store the callback
  pvCurCallback = pvCallback;

  // build the command
  wXmitBufIdx = SPRINTF_P( anXmitBuffer, ( const char* )szFmtCmd, ( const char * )szCmdAT );
  wXmitBufIdx += SPRINTF_P( &anXmitBuffer[ wXmitBufIdx ], ( const char *)szCmdCTZU, bState );
  anXmitBuffer[ wXmitBufIdx++ ] = '\r';

  // post the message
  TaskManager_PostEvent( TASK_SCHD_ENUM_LTECTRL, CTL_EVENT_XMTMSG );
}

/******************************************************************************
 * @function ParseResponse
 *
 * @brief parse the response
 *
 * This function will parse the response
 *
 *****************************************************************************/
static void ParseResponse( void )
{
  LTEERR  eError = LTE_ERR_NONE;
  PVOID   pvRetValue = NULL;

  // determine the response
  if ( STRNCMP_P( szRspOK, anRecvBuffer, STRLEN_P( szRspOK )) == 0 )
  {
    // set the error
    eError = LTE_ERR_NONE;
  }
  else if ( STRNCMP_P( szRspError, anRecvBuffer, STRLEN_P( szRspError )) == 0 )
  {
    // set the error
    eError = LTE_ERR_ERROR;
  }
  else
  {
    // special processing
    pvRetValue = ParseResponseValue( );
  }

  // process the callback if not null
  if ( pvCurCallback != NULL )
  {
    pvCurCallback( eError, pvRetValue );
  }
}
 
/******************************************************************************
 * @function ParseResponseValue
 *
 * @brief prase the returned message for content
 *
 * This function will parse the content of the returned message
 *
 * @return      pointer to the parsed content
 *
 *****************************************************************************/
static PVOID ParseResponseValue( void )
{
  PVOID pvRetValue = NULL;

  return( pvRetValue );
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
/**@} EOF LTECellModem.c */
