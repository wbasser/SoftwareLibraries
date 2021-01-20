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

// Macros and Defines ---------------------------------------------------------
/// define the size of the transmit/receive buffers
#define XMIT_BUF_SIZE                       ( 512 )
#define RECV_BUF_SIZE                       ( 512 )

/// define the message timeout
#define XMIT_MSG_TIME_MSECS                 ( 500 )

/// define the power up delay time
#define PWRUP_DELAY_TIME_MSECS              ( 3000 )

/// define the carriage return/linefeed charasters
#define CH_CR                               ( 0x0D )
#define CH_LF                               ( 0x0A )

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

/// enumerate the decode states
typedef enum _DECSTATE
{
  DEC_STATE_CR1 = 0,
  DEC_STATE_LF1,
  DEC_STATE_RSP,
  DEC_STATE_ERR,
  DEC_STATE_CR2,
  DEC_STATE_LF2,
  DEC_STATE_OK1,
  DEC_STATE_OK2,
  DEC_STATE_CR3,
  DEC_STATE_LF3,
  DEC_STATE_CR4,
  DEC_STATE_LF4,
  DEC_STATE_FLS,
} DECSTATE;

/// enumerate the MQTT op codes
typedef enum _LTEMQTTOPCODE
{
  LTE_MQTTOPCODE_CLIENTID = 0,
  LTE_MQTTOPCODE_LCLPORT,
  LTE_MQTTOPCODE_SERVERNAME,
  LTE_MQTTOPCODE_IPADDRESS,
  LTE_MQTTOPCODE_USERCRED,
  LTE_MQTTOPCODE_WILLQOS,
  LTE_MQTTOPCODE_WILLRETAIN,
  LTE_MQTTOPCODE_WILLTOPIC,
  LTE_MQTTOPCODE_WILLMESSAGE,
  LTE_MQTTOPCODE_INACTIVEPER,
  LTE_MQTTOPCODE_SSECURE
  LTE_MQTTOPCODE_CLEANSESSION,
  LTE_MQTTOPCODE_UNUSED,
  LTE_MQTTOPCODE_TERSEVERB,
  LTE_MQTTOPCODE_MAX
} LTEMQTTOPCODE;

// structures -----------------------------------------------------------------

// global parameter declarations ----------------------------------------------

// local parameter declarations -----------------------------------------------
static  CTLSTATE      eCtlState;
static  DECSTATE      eDecState;
static  U8            anXmitBuffer[ XMIT_BUF_SIZE ];
static  U8            anRecvBuffer[ RECV_BUF_SIZE ];
static  U16           wXmitBufIdx;
static  U16           wRecvBufIdx;
static  U16           wRespBufIdx;
static  PVLTECALLBACK pvCurCallback;
static  PCC8          pszExpectedResult;

// local function prototypes --------------------------------------------------
static  void  SendCommand( PCC8 pszCommand, PC8 pszOption, PCC8 pszExpectedResult, BOOL bSendAT, PVLTECALLBACK pvCallback );
static  void  ParseResponse( void );
static  PVOID ParseResponseValue( void );

// constant parameter initializations -----------------------------------------
/// define the commands
/// general
static  const CODE C8   szCmdAT[ ]          = { "AT" };
static  const CODE C8   szCmdECHO[ ]        = { "E%d" };
static  const CODE C8   szCmdCGSN[ ]        = { "+CGSN" };
static  const CODE C8   szCmdCIMI[ ]        = { "+CIMI" };
static  const CODE C8   szCmdCCID[]         = { "+CCID" };

/// control and status
static  const CODE C8   szCmdCFUN[ ]        = { "+CFUN" };
static  const CODE C8   szCmdCCLK[ ]        = { "+CCLK" };
static  const CODE C8   szCmdCTZU[ ]        = { "+CTZU" };

/// Network service
static  const CODE C8   szCmdUMNOPROF[ ]    = { "+UMNOPROF" }; 
static  const CODE C8   szCmdCSQ[ ]         = { "+CSQ" };
static  const CODE C8   szCmdCREG[ ]        = { "+CREG" };
static  const CODE C8   szCmdCGDCONT[ ]     = { "+CGDCONT" };
static  const CODE C8   szCmdPPP[ ]         = { "D" };
static  const CODE C8   szCmdCOPS[ ]        = { "+COPS" };

/// baud rate
static  const CODE C8   szCmdIPR[ ]         = { "+IPR" };

/// GPIO
static  const CODE C8   szCmdUGPIOC[ ]      = { "+UGPIOC" };

/// IP
static  const CODE C8   szCmdUSOCR[ ]       = { "+USOCR" };
static  const CODE C8   szCmdUSCOL[ ]       = { "+USOCL" };
static  const CODE C8   szCmdUSOCO[ ]       = { "+USOCO" };
static  const CODE C8   szCmdUSOWR[ ]       = { "+USOWR" };
static  const CODE C8   szCmdUSORD[ ]       = { "+USORD" };
static  const CODE C8   szCmdUSOLI[ ]       = { "+USOLI" };

/// SMS
static  const CODE C8   szCmdCMGF[ ]        = { "+CMGF" };
static  const CODE C8   szCmdCMGS[ ]        = { "+CMGS" };

/// location
static  const CODE C8   szCmdUPGS[ ]        = { "+UGPS" };
static  const CODE C8   szCmdULOC[ ]        = { "+ULOC" };
static  const CODE C8   szCmdUGPRMC[ ]      = { "+UGRMC" };

/// MQQT

/// on/off codes
static  const CODE C8   szOptOff[ ]         = { "0" };
static  const CODE C8   szOptOn[ ]          = { "1" };

/// query codes
static  const CODE C8   szQuery[ ]          = { "?" };

/// response codes
static  const CODE C8   szRspOK[ ]          = { "OK" };
static  const CODE C8   szRspError[ ]       = { "ERROR" };

/// general format strings
static  const CODE C8   szFmtStr[ ]         = { "%s" };
static  const CODE C8   szFmtVal[ ]         = { "%d" };

/// argument format strings
static  const CODE C8   szFmtSngArg[ ]      = { "=%d" };
static  const CODE C8   szFmtDblArg[ ]      = { "=%d,%d" };
static  const CODE C8   szFmtCopsArg[ ]     = { "=%d,\"%s\",\"%S\"" };

/// GPIO mapping
static  const CODE U8   anGpioMode[ LTE_GPIO_MAX ] =
{
  16, 23, 24, 25, 42, 19
};

/// PDP types
static  const CODE PC8 apszPdpType[ ]  =
{
  "IP",
  "NONIP",
  "IPV4V6",
  "IPV6"
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
  // call the local initialization
  LTECellModem_LocalInitialize( );
  
  // set the state
  eCtlState = CTL_STATE_PWRUP;

  // clear the buffer indices
  wXmitBufIdx = wRecvBufIdx = 0;
}

/******************************************************************************
 * @function LTECellModem_CharProcess
 *
 * @brief process character task
 *
 * This function will process the incoming character
 *
 * @param[in]   nChar   character to process
 *
 *****************************************************************************/
void LTECellModem_CharProcess( U8 nChar )
{
  // process the state
  switch( eDecState )
  {
    case DEC_STATE_CR1 :
      if ( CH_CR == nChar )
      {
        // go to LF1
        eDecState = DEC_STATE_LF1;
      }
      else
      {
        // go to error
        eDecState = DEC_STATE_FLS;
      }
      break;

    case DEC_STATE_LF1 :
      if ( CH_LF == nChar )
      {
        // go to RSP
        eDecState = DEC_STATE_RSP;
      }
      else
      {
        // go to error
        eDecState = DEC_STATE_FLS;
      }
      break;

    case DEC_STATE_RSP :
      // check for the terminating carriage return
      if ( CH_CR == nChar )
      {
        // terminate the receive buffer
        anRecvBuffer[ wRecvBufIdx - 1 ] = '\0';

        // goto LF2
        eDecState = DEC_STATE_LF2;
      }
      else if ( *( pszExpectedResult + wRespBufIdx ) == nChar )
      {
        // add to receive buffer
        anRecvBuffer[ wRecvBufIdx++ ] = nChar;

        // increment the response index
        wRespBufIdx++;
      }
      else if ( szRspError[ wRespBufIdx ] == nChar )
      {
        // add to receive buffer
        anRecvBuffer[ wRecvBufIdx++ ] = nChar;

        // increment the response index
        wRespBufIdx++;

        // go to error
        eDecState = DEC_STATE_ERR;
      }
      break;

    case DEC_STATE_ERR :
      // check for the terminating carriage return
      if ( CH_CR == nChar )
      {
        // goto LF4
        eDecState = DEC_STATE_LF4;
      }
      else
      {
        // go to error
        eDecState = DEC_STATE_FLS;
      }
      break;

    case DEC_STATE_OK1 :
      if ( szRspOK[ 0 ] == nChar )
      {
        // go to OK2
        eDecState = DEC_STATE_OK2;
      }
      else
      {
        // go to error
        eDecState = DEC_STATE_FLS;
      }
      break;

    case DEC_STATE_OK2 :
      if ( szRspOK[ 1 ] == nChar )
      {
        // go to CR4
        eDecState = DEC_STATE_CR4;
      }
      else
      {
        // go to error
        eDecState = DEC_STATE_FLS;
      }
      break;

    case DEC_STATE_CR2 :
      if ( CH_CR == nChar )
      {
        // go to LF1
        eDecState = DEC_STATE_LF2;
      }
      else
      {
        // go to error
        eDecState = DEC_STATE_FLS;
      }
      break;

    case DEC_STATE_LF2 :
      if ( CH_LF == nChar )
      {
        // go to RSP
        eDecState = DEC_STATE_CR3;
      }
      else
      {
        // go to error
        eDecState = DEC_STATE_FLS;
      }
      break;

    case DEC_STATE_CR3 :
      if ( CH_CR == nChar )
      {
        // go to LF1
        eDecState = DEC_STATE_LF3;
      }
      else
      {
        // go to error
        eDecState = DEC_STATE_FLS;
      }
      break;

    case DEC_STATE_LF3 :
      if ( CH_LF == nChar )
      {
        // go to OK1
        eDecState = DEC_STATE_OK1;
      }
      else
      {
        // go to error
        eDecState = DEC_STATE_FLS;
      }
      break;

    case DEC_STATE_CR4 :
      if ( CH_CR == nChar )
      {
        // go to LF1
        eDecState = DEC_STATE_LF4;
      }
      else
      {
        // go to error
        eDecState = DEC_STATE_FLS;
      }
      break;

    case DEC_STATE_LF4 :
      if ( CH_LF == nChar )
      {
        // in all cases - post a message received event
        LTECellModem_PostCtrlEvent( LTE_LCLEVENT_RCVMSG );
      }
      else
      {
        // go to error
        eDecState = DEC_STATE_FLS;
      }
      break;

    case DEC_STATE_FLS :
      break;

    default :
      break;
  }
}

/******************************************************************************
 * @function LTECellModem_CtrlProcess
 *
 * @brief process control task
 *
 * This function will process the control event
 *
 * @param[in]   eEvent  event 
 *
 *****************************************************************************/
void LTECellModem_CtrlProcess( LTELCLEVENT eEvent )
{
  // determine the state
  switch( eCtlState )
  {
    case CTL_STATE_PWRUP :
      // turn on the power control/start a time delay
      LTECellModem_PowerControl( ON );
      LTECellModem_StartStopTime( PWRUP_DELAY_TIME_MSECS );
      eCtlState = CTL_STATE_PWRDLY;
      break;

    case CTL_STATE_PWRDLY :
      // if this is a timeout - reset the 
      if ( LTE_LCLEVENT_TIMEOUT == eEvent )
      {
        LTECellModem_PowerControl( OFF );
        eCtlState = CTL_STATE_IDLE;
      }
      break;

    case CTL_STATE_IDLE :
      // check for a message request
      if ( LTE_LCLEVENT_XMTMSG == eEvent )
      {
        // send it/start timer/goto wait state
        LTECellModem_Write( anXmitBuffer, wXmitBufIdx );
        LTECellModem_StartStopTime( XMIT_MSG_TIME_MSECS );
        eCtlState = CTL_STATE_WAIT;
      }
      break;

    case CTL_STATE_WAIT :
      // determine the event
      switch( eEvent )
      {
        case LTE_LCLEVENT_RCVMSG :
          // stop the timer
          LTECellModem_StartStopTime( 0 );

          // parse the buffer
          ParseResponse( );
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
  C8    acLclBuffer[ 2 ];

  // create the option
  SPRINTF_P( acLclBuffer, ( PCC8 )szFmtVal, bState );
  
  // send the command
  SendCommand( szCmdECHO, acLclBuffer, szRspOK, TRUE, pvCallback );
}


/******************************************************************************
 * @function LTECellModem_SendRequest
 *
 * @brief send a request for data
 *
 * This function will send a request for data, register the callback
 *
 * @param[in]   eRequest    request enumeration
 * @param[in]   pvCallback  pointer to the callback
 *
 * @return      apropriate error 
 *
 *****************************************************************************/
LTEERR LTECellModem_SendRequest( LTEREQUEST eRequest, PVLTECALLBACK pvCallback )
{
  LTEERR  eError = LTE_ERR_NONE;
  PCC8    pszCommand;
  PCC8    pszOption = NULL;

  // check for a command in progress
  if ( CTL_STATE_IDLE != eCtlState )
  {
    // set the error
    eError = LTE_ERR_BUSY;
  }
  else
  {
    // now get the command
    switch( eRequest )
    {
      case LTE_REQUEST_CREG :
        pszCommand = szCmdCREG;
        break;

      case LTE_REQUEST_CGSN :
        pszCommand = szCmdCGSN;
        break;

      case LTE_REQUEST_CIMI :
        pszCommand = szCmdCIMI;
        break;

      case LTE_REQUEST_CCID :
        pszCommand = szCmdCCID;
        break;

      case LTE_REQUEST_CSQ :
        pszCommand = szCmdCSQ;
        break;

      case LTE_REQUEST_MNO :
        pszCommand = szCmdUMNOPROF;
        pszOption = szQuery;
        break;

      case LTE_REQUEST_APN :
        pszCommand = szCmdCOPS;
        pszOption = szQuery;
        break;

      default :
        // error
        eError = LTE_ERR_ILLREQUEST;
        pszCommand = NULL;
        break;
    }

    // now check for valid command
    if ( NULL != pszCommand )
    {
      // send the command
      SendCommand( pszCommand, ( PC8 )pszOption, szRspOK, TRUE, pvCallback );
    }
  }

  // return the error
  return( eError );
}

/******************************************************************************
 * @function LTECellModem_SetOperator
 *
 * @brief set the operator
 *
 * This function will set the operator
 *
 * @param[in]   eMno      MNO selection
 * @param[in]   pvCallback  pointer to the callback
 *
 * @return      apropriate error 
 *
 *****************************************************************************/
LTEERR LTECellModem_SetOperator( LTEMNO eMno, PVLTECALLBACK pvCallback )
{
  LTEERR  eError = LTE_ERR_NONE;
  C8      acArgument[ 8 ];

  // format the option
  SPRINTF_P( acArgument, ( PCC8 )szFmtSngArg, eMno );

  // send the command
  SendCommand( szCmdUMNOPROF, acArgument, szRspOK, TRUE, pvCallback );

  // return the error
  return( eError );
}

/******************************************************************************
 * @function LTECellModem_SetAPN
 *
 * @brief set the APN
 *
 * This function will set the APN
 *
 * @param[in]   pszOperator pointer to the operator
 * @param[in]   pvCallback  pointer to the callback
 *
 * @return      apropriate error 
 *
 *****************************************************************************/
LTEERR LTECellModem_SetAPN( PC8 pszOperator, LTEPDP ePdp, PVLTECALLBACK pvCallback )
{
  LTEERR  eError = LTE_ERR_NONE;
  C8      acArgument[ 24 ];

  // format the option
  SPRINTF_P( acArgument, szFmtCopsArg, 1, ( PC8 )apszPdpType[ ePdp ], pszOperator );

  // send the command
  SendCommand( szCmdCOPS, acArgument, szRspOK, TRUE, pvCallback );

  // return the error
  return( eError );
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
  LTEERR  eError = LTE_ERR_NONE;
  C8      acArguments[ 8 ];
  
  // test for valid enum
  if ( eGpioEnum < LTE_GPIO_MAX )
  {
    // create the option
    SPRINTF_P( acArguments, szFmtDblArg, eGpioEnum, eGpioMode );
    
    // send the command
    SendCommand( szCmdUGPIOC, acArguments, szRspOK, TRUE, pvCallback );
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
LTEERR LteCellModem_SetAutoTimeZone( BOOL bState, PVLTECALLBACK pvCallback )
{
  LTEERR  eError = LTE_ERR_NONE;
  C8      acArguments[ 8 ];

  // create the option
  SPRINTF_P( acArguments, ( PCC8 )szFmtSngArg, bState );
  
  // send the command
  SendCommand( szCmdCTZU, acArguments, szRspOK, TRUE, pvCallback );

  // return the error
  return( eError );
}

/******************************************************************************
 * @function SendCommand
 *
 * @brief send a command
 *
 * This function will stuff the transmit buffer and send the command
 *
 * @param[in]   pszCommand    pointer to the command
 * @param[in]   pszOption     pointer to the option
 * @param[in]   pszExpected   pointer to the expected result
 * @param[in]   bSendAT       send an AT also
 * @param[in]   pvCallback    pointer to the callback function
 *
 *****************************************************************************/
static void SendCommand( PCC8 pszCommand, PC8 pszOption, PCC8 pszExpected, BOOL bSendAT, PVLTECALLBACK pvCallback )
{
  // store the callback/result
  pvCurCallback = pvCallback;
  pszExpectedResult = pszExpected;

  // clear the index
  wXmitBufIdx = 0;
  wRecvBufIdx = 0;
  wRespBufIdx = 0;

  // set the decode state
  eDecState = DEC_STATE_CR1;
  
  // check for send AT
  if ( bSendAT )
  {
    // stuff in buffer
    wXmitBufIdx = SPRINTF_P( anXmitBuffer, ( PCC8 )szFmtStr, ( PCC8 )szCmdAT );
  }
  
  // now stuff command
  wXmitBufIdx += SPRINTF_P( &anXmitBuffer[ wXmitBufIdx ], ( PCC8 )szFmtStr, pszCommand );
  
  // check for option
  if ( NULL != pszOption )
  {
    // now stuff option
    wXmitBufIdx += SPRINTF_P( &anXmitBuffer[ wXmitBufIdx ], ( PCC8 )szFmtStr, pszOption );
  }
  
  // terminate it
  anXmitBuffer[ wXmitBufIdx++ ] = '\r';

  // post the message
  LTECellModem_PostCtrlEvent( LTE_LCLEVENT_XMTMSG );
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

///******************************************************************************
// * @function LTECellModem_GetGpio
// *
// * @brief wet the gpio
// *
// * This function will get the GPIO to a givenfunction
// *
// * @param[in]   eGpioEnum     desried GPIO enumeration
// * @param[in]   eGpioMode     desired GPIO mode
// * @param[in]   pvCallback    pointer to the callback function
// *
// * @return      appropriate error
// *
// *****************************************************************************/
//LTEERR LTECellModem_GetGpio( LTEGPIO eGpioEnum, PLTEGPIOMODE peGpioMode, PVLTECALLBACK pvCallback )
//{
//  LTEERR eError = LTE_ERR_NONE;

//  // save the callback
//  pvCurCallback = pvCallback;

//  // test for valid enum
//  if ( eGpioEnum < LTE_GPIO_MAX )
//  {
//  }
//  else
//  {
//    // set the error
//    eError = LTE_ERR_ILLGPIOENUM;
//  }

//  // return the error
//  return( eError );
//}



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
