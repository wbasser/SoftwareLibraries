/******************************************************************************
 * @file StellarisUSBCDCHandler.c
 *
 * @brief USB CDC handler implementation
 *
 * This file provides the implementation for the CDC handler 
 *
 * @copyright Copyright (c) 2012 Cyber Intergration
 * This document contains proprietary data and information of Endurance Products 
 * LLC. It is the exclusive property of Endurance Products, LLC and will not be 
 * disclosed in any form to any party without prior written permission of 
 * Endurance Products, LLC. This document may not be reproduced or further used 
 * without the prior written permission of Endurance Products, LLC.
 *
 * Version History
 * ======
 * $Log: $
 * 
 *
 * \addtogroup StellarisUSBCDCHandler
 * @{
 *****************************************************************************/

// system includes ------------------------------------------------------------
#include "Types/Types.h"
#include <stdarg.h>

// library includes -----------------------------------------------------------
#include "driverlib/usb.h"
#include "usblib/usblib.h"
#include "usblib/usbcdc.h"
#include "usblib/usb-ids.h"
#include "usblib/device/usbdevice.h"
#include "usblib/device/usbdcdc.h"
#include "TaskManager/TaskManager.h"
#include "GPIO/Gpio.h"

// local includes -------------------------------------------------------------
#include "StellarisUSBCDCHandler/StellarisUSBCDCHandler_prm.h"
#include "StellarisUSBCDCHandler/StellarisUSBCDCHandler.h"

// Macros and Defines ---------------------------------------------------------
/// define the buffer size
#define CDC_BUFFER_SIZE   128

/// define the number of strings
#define USBCDC_NUM_STRING_DESCRIPTORS ( sizeof( apStellarisUSBCDCStringDescriptors ) /  sizeof( PU8 ))

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations ----------------------------------------------

// local parameter declarations -----------------------------------------------
static	tCDCSerInstance		tInstance;
static	tUSBDCDCDevice		tCdcDevice;
static	tUSBBuffer			  tRcvBuffer;
static	tUSBBuffer			  tXmtBuffer;
static	tLineCoding			  tLclLineCoding;
static	U8					      anRxBuffer[ CDC_BUFFER_SIZE ];
static	U8					      anTxBuffer[ CDC_BUFFER_SIZE ];
static	U8					      anRxBufferWorkspace[ USB_BUFFER_WORKSPACE_SIZE ];
static	U8					      anTxBufferWorkspace[ USB_BUFFER_WORKSPACE_SIZE ];

// local function prototypes --------------------------------------------------
static	U32		RxHandler( PVOID pvCBData, U32 uEvent, U32 uMsgValue, PVOID pvMsgData );
static	U32		TxHandler( PVOID pvCBData, U32 uEvent, U32 uMsgValue, PVOID pvMsgData );
static	U32		CtrlHandler( PVOID pvCBData, U32 uEvent, U32 uMsgValue, PVOID pvMsgData );

// constant initializations ---------------------------------------------------
static const char * const pcHex = "0123456789ABCDEF";

/******************************************************************************
 * @function StellarisUSBCDCHandler_Initialize
 *
 * @brief intialize the USB CDC handler
 *
 * This function will initialize the USB CDC handler
 *
 *****************************************************************************/
void StellarisUSBCDCHandler_Initialize( void )
{
	// fill the receive buffer structure
	tRcvBuffer.bTransmitBuffer        = FALSE;
	tRcvBuffer.pfnCallback		        = RxHandler;
  tRcvBuffer.pvCBData			          = ( PVOID )&tCdcDevice;
	tRcvBuffer.pfnTransfer		        = USBDCDCPacketRead;
	tRcvBuffer.pfnAvailable		        = USBDCDCRxPacketAvailable;
	tRcvBuffer.pvHandle			          = ( PVOID )&tCdcDevice;
  tRcvBuffer.pcBuffer			          = anRxBuffer;
	tRcvBuffer.ulBufferSize		        = CDC_BUFFER_SIZE;
  tRcvBuffer.pvWorkspace		        = anRxBufferWorkspace;

	// fill the transmit buffer structure
	tXmtBuffer.bTransmitBuffer	      = TRUE;
	tXmtBuffer.pfnCallback		        = TxHandler;
  tXmtBuffer.pvCBData		  	        = ( PVOID )&tCdcDevice;
	tXmtBuffer.pfnTransfer		        = USBDCDCPacketWrite;
	tXmtBuffer.pfnAvailable		        = USBDCDCTxPacketAvailable;
	tXmtBuffer.pvHandle			          = ( PVOID )&tCdcDevice;
  tXmtBuffer.pcBuffer			          = anTxBuffer;
	tXmtBuffer.ulBufferSize		        = CDC_BUFFER_SIZE;
  tXmtBuffer.pvWorkspace		        = anTxBufferWorkspace;

	// fill the device structure
	tCdcDevice.usVID					        = USB_VID_STELLARIS;
  tCdcDevice.usPID					        = USB_PID_SERIAL;
  tCdcDevice.usMaxPowermA				    = 500;
	tCdcDevice.pfnControlCallback	    = CtrlHandler;
  tCdcDevice.ucPwrAttributes		    = USB_CONF_ATTR_SELF_PWR;
  tCdcDevice.pfnRxCallback			    = USBBufferEventCallback;
	tCdcDevice.pvRxCBData				      = ( PVOID )&tRcvBuffer;
  tCdcDevice.pfnTxCallback			    = USBBufferEventCallback;
	tCdcDevice.pvTxCBData				      = ( PVOID )&tXmtBuffer;
	tCdcDevice.ppStringDescriptors		= (const unsigned char * const *)&apStellarisUSBCDCStringDescriptors;
  tCdcDevice.ulNumStringDescriptors	= StellarisUSBCDCHandler_GetNumberOfStrings( );
  tCdcDevice.psPrivateCDCSerData		= &tInstance;

	// register the interrupt
	USBIntRegister( USB0_BASE, USB0DeviceIntHandler );

	// initialize the buffers/device
	USBBufferInit( &tRcvBuffer );
	USBBufferInit( &tXmtBuffer );
	USBDCDCInit( 0, &tCdcDevice );
}

/******************************************************************************
 * @function StellarisUSBCDCHandler_PutChar
 *
 * @brief output a character on the USB
 *
 * This function will send a chanracter on the USB
 *
 * @param[in]   nData   data to send
 *
 *****************************************************************************/
void StellarisUSBCDCHandler_PutChar( U8 nData )
{
	// stuff the character into the buffer
	USBBufferWrite( &tXmtBuffer, &nData, 1 );
}

/******************************************************************************
 * @function StellarisUSBCDCHandler_PutChar
 *
 * @brief output a character on the USB
 *
 * This function will send a chanracter on the USB
 *
 * @param[in]   pnData    pointer to the data to send
 * @param[in]   nLength   lenght of the data to send
 *
 *****************************************************************************/
void StellarisUSBCDCHandler_Write( PU8 pnData, U8 nLength )
{
  // write the buffer
  USBBufferWrite( &tXmtBuffer, pnData, nLength );
}

/******************************************************************************
 * @function StellarisUSBCDCHandler_PutChar
 *
 * @brief output a character on the USB
 *
 * This function will send a chanracter on the USB
 *
 * @param[in]   nData   data to send
 *
 *****************************************************************************/
void StellarisUSBCDCHandler_Printf( const char *pcString, ...)
{
  U32 uIdx, uValue, uPos, uCount, uBase, uNeg;
  char *pcStr, pcBuf[16], cFill;
  va_list vaArgP;

  // Check the arguments.
  if ( pcString == NULL )
    return;

  // Start the varargs processing.
  va_start( vaArgP, pcString );

  // Loop while there are more characters in the string.
  while( *pcString )
  {
    // Find the first non-% character, or the end of the string.
    for( uIdx = 0; (pcString[uIdx] != '%') && (pcString[uIdx] != '\0'); uIdx++ )
    {
    }

    // Write this portion of the string.
    USBBufferWrite( &tXmtBuffer, pcString, uIdx );

    // Skip the portion of the string that was written.
    pcString += uIdx;

    // See if the next character is a %.
    if( *pcString == '%' )
    {
      // Skip the %.
      pcString++;

      // Set the digit count to zero, and the fill character to space
      // (i.e. to the defaults).
      uCount = 0;
      cFill = ' ';

      // It may be necessary to get back here to process more characters.
      // Goto's aren't pretty, but effective.  I feel extremely dirty for
      // using not one but two of the beasts.
again:

      // Determine how to handle the next character.
      switch( *pcString++ )
      {
        // Handle the digit characters.
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
        {
          // If this is a zero, and it is the first digit, then the
          // fill character is a zero instead of a space.
          if (( pcString[ -1 ] == '0' ) && ( uCount == 0 ))
          {
            cFill = '0';
          }

          // Update the digit count.
          uCount *= 10;
          uCount += pcString[ -1 ] - '0';

          // Get the next character.
          goto again;
        }

        // Handle the %c command.
        case 'c':
        {
          // Get the value from the varargs.
          uValue = va_arg( vaArgP, unsigned long );

          // Print out the character.
          StellarisUSBCDCHandler_PutChar( uValue );

          // This command has been handled.
          break;
        }

        // Handle the %d command.
        case 'd':
        {
          // Get the value from the varargs.
          uValue = va_arg( vaArgP, unsigned long );

          // Reset the buffer position.
          uPos = 0;

          // If the value is negative, make it positive and indicate that a minus sign is needed.
          if (( long )uValue < 0 )
          {
            // Make the value positive.
            uValue = -( long )uValue;

            // Indicate that the value is negative.
            uNeg = 1;
          }
          else
          {
            // Indicate that the value is positive so that a minus sign isn't inserted.
            uNeg = 0;
          }

          // Set the base to 10.
          uBase = 10;

          // Convert the value to ASCII.
          goto convert;
        }

        // Handle the %s command.
        case 's':
        {
          // Get the string pointer from the varargs.
          pcStr = va_arg( vaArgP, char * );

          // Determine the length of the string.
          for ( uIdx = 0; pcStr[ uIdx ] != '\0'; uIdx++ )
          {
          }

          // Write the string.
          USBBufferWrite( &tXmtBuffer, pcStr, uIdx );

          // Write any required padding spaces
          if ( uCount > uIdx )
          {
            uCount -= uIdx;
            while( uCount-- )
            {
              StellarisUSBCDCHandler_PutChar( ' ' );
            }
          }
          // This command has been handled.
          break;
        }

        // Handle the %u command.
        case 'u':
        {
          // Get the value from the varargs.
          uValue = va_arg( vaArgP, unsigned long );

          // Reset the buffer position.
          uPos = 0;

          // Set the base to 10.
          uBase = 10;

          // Indicate that the value is positive so that a minus sign isn't inserted.
          uNeg = 0;

          // Convert the value to ASCII.
          goto convert;
        }

        // Handle the %x and %X commands.  Note that they are treated
        // identically; i.e. %X will use lower case letters for a-f
        // instead of the upper case letters is should use.  We also
        // alias %p to %x.
        case 'x':
        case 'X':
        case 'p':
        {
          // Get the value from the varargs.
          uValue = va_arg( vaArgP, unsigned long );

          // Reset the buffer position.
          uPos = 0;

          // Set the base to 16.
          uBase = 16;

          // Indicate that the value is positive so that a minus sign isn't inserted.
          uNeg = 0;

          // Determine the number of digits in the string version of the value.
convert:
          for ( uIdx = 1; ((( uIdx * uBase ) <= uValue ) && ((( uIdx * uBase ) / uBase ) == uIdx )); uIdx *= uBase, uCount-- )
          {
          }

          // If the value is negative, reduce the count of padding characters needed.
          if (uNeg )
          {
            uCount--;
          }

          // If the value is negative and the value is padded with zeros, then place the minus sign before the padding.
          if ( uNeg && ( cFill == '0' ))
          {
            // Place the minus sign in the output buffer.
            pcBuf[ uPos++ ] = '-';

            // The minus sign has been placed, so turn off the negative flag.
            uNeg = 0;
          }

          // Provide additional padding at the beginning of the string conversion if needed.
          if (( uCount > 1) && ( uCount < 16 ))
          {
            for ( uCount--; uCount; uCount-- )
            {
              pcBuf[ uPos++ ] = cFill;
            }
          }

          // If the value is negative, then place the minus sign before the number.
          if ( uNeg )
          {
            // Place the minus sign in the output buffer.
            pcBuf[ uPos++ ] = '-';
          }

          // Convert the value into a string.
          for ( ; uIdx; uIdx /= uBase )
          {
            pcBuf[ uPos++ ] = pcHex[ ( uValue / uIdx ) % uBase ];
          }

          // Write the string.
          USBBufferWrite( &tXmtBuffer, pcBuf, uPos );

          // This command has been handled.
          break;
        }

        // Handle the %% command.
        case '%':
        {
          // Simply write a single %.
          StellarisUSBCDCHandler_PutChar(( U8 )*( pcString - 1 ));

          // This command has been handled.
          break;
        }

        // Handle all other commands.
        default:
        {
          // Indicate an error.
          USBBufferWrite( &tXmtBuffer, "ERROR", 5 );

          // This command has been handled.
          break;
        }
      }
    }
  }

  // End the varargs processing.
  va_end( vaArgP );
}


/******************************************************************************
 * @function RxHandler
 *
 * @brief receieve interrupt handler
 *
 * This function process all receive characters that are received
 *
 * @param[in]   pvCBData    pointer to callback data
 * @param[in]   ulEvent     event
 * @param[in]   ulMsgValue  message value
 * @param[in]   pvMsgData   pointer to the message data
 *
 * @return      status      in this case 0
 *
 *****************************************************************************/
static U32 RxHandler( PVOID pvCBData, U32 uEvent, U32 uMsgValue, PVOID pvMsgData )
{
  U32 uStatus = 0;
  U8  nData;
  
	// process the event
	switch( uEvent )
	{
		case USB_EVENT_RX_AVAILABLE :
			// while there are characters to process, do it
			while( USBBufferRead( &tRcvBuffer, &nData, 1 ) != 0 )
			{
				// post it
        TaskManager_PostEvent( STELLARISUSBCDCHANDLER_RCVCHR_TASK, ( TASKARG )nData );
			}
			break;

		case USB_EVENT_DATA_REMAINING :
			// set the status to 0
			uStatus = 0;
			break;

		case USB_EVENT_REQUEST_BUFFER :
			// set the status to 0
			uStatus = 0;
			break;

		default :
			break;
	}

  // return the status
  return( uStatus );
}

/******************************************************************************
 * @function RxHandler
 *
 * @brief receieve interrupt handler
 *
 * This function process all receive characters that are received
 *
 * @param[in]   pvCBData    pointer to callback data
 * @param[in]   uEvent     event
 * @param[in]   uMsgValue  message value
 * @param[in]   pvMsgData   pointer to the message data
 *
 * @return      status      in this case 0
 *
 *****************************************************************************/
static U32 TxHandler( PVOID pvCBData, U32 uEvent, U32 uMsgValue, PVOID pvMsgData )
{
  U32 uStatus = 0;
  
	// process the event
	switch( uEvent )
	{
		default :
			break;
	}
  
  // return the status
  return( uStatus );
}

/******************************************************************************
 * @function RxHandler
 *
 * @brief receieve interrupt handler
 *
 * This function process all receive characters that are received
 *
 * @param[in]   pvCBData    pointer to callback data
 * @param[in]   uEvent     event
 * @param[in]   uMsgValue  message value
 * @param[in]   pvMsgData   pointer to the message data
 *
 * @return      status      in this case 0
 *
 *****************************************************************************/
static U32 CtrlHandler( PVOID pvCBData, U32 uEvent, U32 uMsgValue, PVOID pvMsgData )
{
  U32 uStatus = 0;
  
  	// process the event
	switch( uEvent )
	{
		case USB_EVENT_CONNECTED :
      // turn on the USB connected LED
      Gpio_Set( STELLARISUSBCDCHANDLER_USBCON_LED, ON );
			break;

		case USB_EVENT_DISCONNECTED :
      // turn off the USB connected LED
      Gpio_Set( STELLARISUSBCDCHANDLER_USBCON_LED, OFF );
			break;

		case USBD_CDC_EVENT_GET_LINE_CODING :
			memcpy( pvMsgData, &tLclLineCoding, sizeof( tLineCoding ));
			break;

		case USBD_CDC_EVENT_SET_LINE_CODING :
			memcpy( &tLclLineCoding, pvMsgData, sizeof( tLineCoding ));
			break;

		case USBD_CDC_EVENT_SET_CONTROL_LINE_STATE :
			break;

		case USBD_CDC_EVENT_SEND_BREAK :
			break;

		case USBD_CDC_EVENT_CLEAR_BREAK :
			break;

		case USB_EVENT_SUSPEND :
      // turn off the USB connected LED
      Gpio_Set( STELLARISUSBCDCHANDLER_USBCON_LED, OFF );
      break;

		case USB_EVENT_RESUME :
			break;

		default :
			break;
	}

  // return the status
  return( uStatus );
}
 
/**@} EOF StellarisUSBCDCHandler.c */
