/******************************************************************************
 * @file LUFACDCHandler.c
 *
 * @brief LUFA CDC handler implementation
 *
 * This file provides the LUFA CDC handler implementation
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
 * $Log: $
 * 
 *
 * \addtogroup LUFA
 * @{
 *****************************************************************************/
// system includes -------------------------------------------------------------
#include <stdio.h>

// library includes ------------------------------------------------------------
#include "GPIO/Gpio.h"

// local includes -------------------------------------------------------------
#include "LUFACDCHandler/LUFACDCHandler.h"
#include "LUFACDCHandler/LUFACDCHandler_prm.h"

// global parameter declarations ----------------------------------------------
FILE* g_ptLUFACDCStream;

// local parameter declarations -----------------------------------------------
static  FILE  tLUFACDCStream;

// constant parameter initializations -----------------------------------------
static	USB_ClassInfo_CDC_Device_t tCDCInterface =
{
  .Config                     =
  {
    .ControlInterfaceNumber   = INTERFACE_ID_CDC_CCI,
    .DataINEndpoint           =
    {
      .Address                = CDC_TX_EPADDR,
      .Size                   = CDC_TXRX_EPSIZE,
      .Banks                  = 1,
    },
    .DataOUTEndpoint          =
    {
      .Address                = CDC_RX_EPADDR,
      .Size                   = CDC_TXRX_EPSIZE,
      .Banks                  = 1,
    },
    .NotificationEndpoint     =
    {
      .Address                = CDC_NOTIFICATION_EPADDR,
      .Size                   = CDC_NOTIFICATION_EPSIZE,
      .Banks                  = 1,
    },
  },
};

/******************************************************************************
 * @function LUFACDCHandler_Initialize
 *
 * @brief LUFA CDC handler initialization
 *
 * This function will save the callback for a receive character and
 * call the USB stack initialization
 *
 *****************************************************************************/
void LUFACDCHandler_Initialize( void )
{
  // initialize the stack
  USB_Init( );

  // create the stream device
  CDC_Device_CreateStream( &tCDCInterface, &tLUFACDCStream );

  // set the pointer
  g_ptLUFACDCStream = &tLUFACDCStream;
}

/******************************************************************************
 * @function LUFACDCHandler_ProcessIdle
 *
 * @brief LUFA CDC handler idle process
 *
 * This function is called in the idle time to see if any characters have been
 * received and redirected to the stored receive character handler.  It also
 * calls the device class task as well as the USB task
 *
 *****************************************************************************/
void LUFACDCHandler_ProcessIdle( void )
{
  S16 iChar;
  
  // while there are characters to process
  while(( iChar = CDC_Device_ReceiveByte( &tCDCInterface )) != -1 )
  {
    // post to the task
    TaskManager_PostEvent( LUFACDC_PROCESSCHAR_TASK, ( TASKARG )iChar );
  }
  
  // execute the device/base USB tasks
  CDC_Device_USBTask( &tCDCInterface );
  USB_USBTask( );
}

/******************************************************************************
 * @function LUFADCDHandler_PutChar
 *
 * @brief send a character
 *
 * This function will send a character to the USB device
 *
 * @param[in]   nChar   character to send
 *
 *****************************************************************************/
void LUFACDCHandler_PutChar( U8 nChar )
{
  // output it
  CDC_Device_SendByte( &tCDCInterface, nChar );
}

/******************************************************************************
 * @function LUFADCDHandler_WriteData
 *
 * @brief write a block of data
 *
 * This function will output a block of data
 *
 * @param[in]   pnData      pointer to the data
 * @paraq[in]   wLength     length of the data to send
 *
 *****************************************************************************/
void LUFACDCHandler_WriteData( PU8 pnData, U16 wLength )
{
  // do it
	CDC_Device_SendData( &tCDCInterface, pnData, wLength );
}


/******************************************************************************
 * @function EVENT_USB_Device_Connect
 *
 * @brief device connect notification
 *
 * This function processes the device connect notification
 *
 *****************************************************************************/
void EVENT_USB_Device_Connect( void )
{
  // post to the task
  TaskManager_PostEvent( LUFACDC_PROCESSCON_TASK, ( TASKARG )LUFACDC_CONNECT_EVENT );
}

/******************************************************************************
 * @function EVENT_USB_Device_Disconnect
 *
 * @brief device disconnect notification
 *
 * This function processes the device disconnect notification
 *
 *****************************************************************************/
void EVENT_USB_Device_Disconnect( void )
{
  // post to the task
  TaskManager_PostEvent( LUFACDC_PROCESSCON_TASK, ( TASKARG )LUFACDC_DISCONNECT_EVENT );
}

/******************************************************************************
 * @function EVENT_USB_ConfigurationChanged
 *
 * @brief configuration change notification
 *
 * This function processes the configuration change notification
 *
 *****************************************************************************/
void EVENT_USB_Device_ConfigurationChanged( void )
{
	BOOL bConfigSuccess = TRUE;

  // process the configuration change
	bConfigSuccess &= CDC_Device_ConfigureEndpoints( &tCDCInterface);
}

/******************************************************************************
 * @function EVENT_USB_Device_ControlRequest
 *
 * @brief control requestion notification
 *
 * This function processes the control request notification
 *
 *****************************************************************************/
void EVENT_USB_Device_ControlRequest( void )
{
  // process a control request
  CDC_Device_ProcessControlRequest( &tCDCInterface );
}

/******************************************************************************
 * @function CALLBACK_USB_GetDescriptor
 *
 * @brief process a get desciptor request
 *
 * This function will parst the passed value and return the appropriate descriptor
 *
 * @param[in]   wValue    descriptor type and number
 * @param[in]   wIndex    descriptor index
 * @param[i0]   pvvDescriptorAddress  pointer to store the descriptor address
 *
 * @return      the size of the requested descriptor
 *
 *****************************************************************************/
U16 CALLBACK_USB_GetDescriptor( const U16 wValue, const U16 wIndex, const void** const ppvDescriptorAddress )
{
	const U8    nDescriptorType   = ( wValue >> 8 );
	const U8    nDescriptorNumber = ( wValue & 0xFF );
	void const * pvAddress = NULL;
	U16         wSize = NO_DESCRIPTOR;

	switch ( nDescriptorType )
	{
		case DTYPE_Device:
			pvAddress = &tDeviceDescriptor;
			wSize     = sizeof( USB_Descriptor_Device_t );
			break;
      
		case DTYPE_Configuration:
			pvAddress = &tConfigurationDescriptor;
			wSize     = sizeof( USB_Descriptor_Configuration_t );
			break;
      
		case DTYPE_String:
			switch ( nDescriptorNumber )
			{
				case STRING_ID_LANGUAGE :
					pvAddress = &tLanguageString;
					wSize     = PGM_RDBYTE( tLanguageString.Header.Size );
					break;
          
				case STRING_ID_MANUFACTURER :
					pvAddress = &tManufacturerString;
					wSize     = PGM_RDBYTE( tManufacturerString.Header.Size );
					break;
          
				case STRING_ID_PRODUCT :
					pvAddress = &tProductString;
					wSize     = PGM_RDBYTE( tProductString.Header.Size );
					break;
          
        default :
          break;
			}
			break;
      
    default :
        break;
	}

	*ppvDescriptorAddress = pvAddress;
	return ( wSize );
}

/**@} EOF LUFACDCHandler.c */
