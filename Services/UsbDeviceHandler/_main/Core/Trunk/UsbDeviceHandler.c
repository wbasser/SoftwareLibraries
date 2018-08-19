/******************************************************************************
 * @file  UsbDeviceHandler.c
 *
 * @brief USB device handler implementation
 *
 * This file provides the implementation for the USB device handler
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
 * \addtogroup UsbDeviceHandler
 * @{
 *****************************************************************************/

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "UsbDeviceHandler/UsbDeviceHandler_cfg.h"

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------
#define USB_EP_CDC_NOTIFICATION_BUF_SIZE            ( 8 )

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations ----------------------------------------------

// local parameter declarations -----------------------------------------------
static  USBSETUPPACKET  tSetupPacket;
static  PU8             pnMultiPacketPtr;
static  U16             wMultiPacketSize;
static  VU8             nCurrentConfig;
static  ALIGNED4  U8    anLclBuffer[ 146 ];

// local function prototypes --------------------------------------------------
static  U16   GetDescriptor( U8 nType, U8 nIndex, PU8* ppDescriptor );
static  PU8   StringToDescriptor( PC8 pszString );
static  void  ProcessMultiPacket( void );
static  void  ProcessMsftCompatable( PUSBMSFTCOMPDESCRIPTOR ptMsftCamp, PUSBMSFTPROPDESCRIPTOR ptMsftProp );

// constant parameter initializations -----------------------------------------

/******************************************************************************
 * @function UsbDeviceHandler_Initialize
 *
 * @brief initialization
 *
 * This function will initialize the USB device
 *
 *****************************************************************************/
void UsbDeviceHandler_Initialize( void )
{
  // initialize the USB
  Usb_Initialize( );
  
  // attach it
  Usb_Attach( );
  
  // enable the CDC endpoints
  Usb_EnableEp( USB_EP_CDC_NOTIFICATION, USB_EP_TYPE_INTERRUPT, USB_CDC_NOTIFICATION_BUF_SIZE );
  Usb_EnableEp( USB_EP_CDC_OUT, USB_EP_TYPE_BULK, USB_CDC_IN_OUT_BUF_SIZE );
  Usb_EnableEp( USB_EP_CDC_IN,  USB_EP_TYPE_BULK, USB_CDC_IN_OUT_BUF_SIZE );
  
  // start the output endpoint
  Usb_EpStartOut( USB_EP_CDC_OUT, anUsbBufOut, USB_CDC_IN_OUT_BUF_SIZE );
}

/******************************************************************************
 * @function UsbDeviceHandler_HandleSetup
 *
 * @brief handle a setup packet
 *
 * This function will handle the setup packet
 *
 * @param[in]   ptSetup        pointer to the setup packet
 *
 *****************************************************************************/
void UsbDeviceHandler_HandleSetup( PUSBSETUPPACKET ptSetup )
{
  U8  nType, nIndex, nRecipient;
  PU8 pnDescriptor;
  U16 wSize;
  
  // copy the data
  memcpy( &tSetupPacket, ptSetup, USBSETUPPACKET_SIZE );
  
  // check for a standard request
  if (( tSetupPacket.nRequestType & USB_REQTYPE_TYPE_MASK ) == USB_REQTYPE_STANDARD )
  {
    // process the request
    switch( tSetupPacket.nRequest )
    {
      case USB_STDREQ_GETSTATUS :
        // set the status/output it
        anEp0BufIn[ 0 ] = 0;
        anEp0BufIn[ 1 ] = 0;
        Usb_Ep0In( 2 );
        Usb_Ep0Out( );
        break;
        
      case USB_STDREQ_CLEARFEATURE :
      case USB_STDREQ_SETFEATURE :
        Usb_Ep0In( 0 );
        Usb_Ep0Out( );
        break;
        
      case USB_STDREQ_SETADDRESS :
        Usb_Ep0In( 0 );
        Usb_Ep0Out( );
        break;
        
      case USB_STDREQ_GETDESCRIPTOR :
        // get the type/index
        nType = tSetupPacket.wValue >> 8;
        nIndex = tSetupPacket.wValue & 0xFF;
        pnDescriptor = NULL;
        
        // get the size
        wSize = GetDescriptor( nType, nIndex, &pnDescriptor );
        
        // now test for a valid descriptor
        if (( wSize != 0 ) && ( pnDescriptor != NULL ))
        {
          // Is our size the same as a setup packet length
          if ( wSize > tSetupPacket.wLength )
          {
            // set it to the setup packet length
            wSize = tSetupPacket.wLength;
          }
          
          // check to see if the descriptor ispointing to EP0
          if ( pnDescriptor == anEp0BufIn )
          {
            // clear the size
            wMultiPacketSize = 0;
            
            // start EP0 
            Usb_EpStartIn( 0x80, pnDescriptor, wSize, TRUE );
          }
          else
          {
            // setup for a multi packet
            wMultiPacketSize = wSize;
            pnMultiPacketPtr = pnDescriptor;
            
            // start a multi packet output
            ProcessMultiPacket( );
          }
        }
        else
        {
          // stall the endpoit
          Usb_Ep0Stall( );
        }
        break;
        
      case USB_STDREQ_GETCONFIGURATION :
        // set the current configuration/output it
        anEp0BufIn[ 0 ] = nCurrentConfig; 
        Usb_Ep0In( 1 );
        Usb_Ep0Out( );
        break;
          
      case USB_STDREQ_SETCONFIGURATION :
        // call the abstraction handler
        if ( UsbDeviceHandler_SetConfigurationCallback( tSetupPacket.wValue ))
        {
          // store the configuration
          nCurrentConfig = ( U8 )tSetupPacket.wValue;
          
          // set up read
          Usb_Ep0In( 0 );
          Usb_Ep0Out( );
        }
        else
        {
          // stall the intpoit
          Usb_Ep0Stall( );
        }
        break;
      
      case USB_STDREQ_SETINTERFACE :
        if ( UsbDeviceHandler_SetInterfaceCallback( tSetupPacket.wValue ))
        {
          // set up read
          Usb_Ep0In( 0 );
          Usb_Ep0Out( );
        } else
        {
          // stall the endpoit
          Usb_Ep0Stall( );
        }
        break;
        
      default :
        // stall the endpoit
        Usb_Ep0Stall( );
        break;
    }
  }
  else
  {
    // get the recipient
  	nRecipient = tSetupPacket.nRequestType & USB_REQTYPE_RECIPIENT_MASK;
    
    // is this a DEVICE
    if ( nRecipient == USB_RECIPIENT_DEVICE )
    {
      switch( tSetupPacket.nRequestType )
      {
        case USB_MSFT_ID :
          ProcessMsftCompatable(( PUSBMSFTCOMPDESCRIPTOR )&tUsbMstComp, ( PUSBMSFTPROPDESCRIPTOR )&tUsbMstProd);          
          break;
          
        default :
          UsbDeviceHandler_HandleControlReceipients( tSetupPacket.wIndex, tSetupPacket.wValue );
          break;
      }
    }
    else if ( nRecipient == USB_RECIPIENT_INTERFACE )
    {
      switch( tSetupPacket.nRequestType )
      {
        case USB_MSFT_ID :
          ProcessMsftCompatable(( PUSBMSFTCOMPDESCRIPTOR )&tUsbMstComp, ( PUSBMSFTPROPDESCRIPTOR )&tUsbMstProd );          
          break;
          
        default :
          break;
      }
    }
    else
    {
      // stall the endpoit
      Usb_Ep0Stall( );
    }
  }
}

/******************************************************************************
 * @function GetDescriptor
 *
 * @brief get s a descriptor
 *
 * This function will determine what descriptor to get and returns a pointer
 * to it.
 *
 * @param[in]   nTYpe         descriptor type
 * @param[in]   nIndex        descriptor index
 * @param[io]   ppDescriptor  pointer to a pointer to store the descriptor
 *
 * @return      size of the descriptor
 *
 *****************************************************************************/
static U16 GetDescriptor( U8 nType, U8 nIndex, PU8* ppDescriptor )
{
  PU8   pnAddress = NULL;
  U16   wSize = 0;

  // determine the type
	switch ( nType )
  {
		case USB_DTYPE_DEVICE :
			pnAddress = ( PU8 )&tUsbDeviceDescriptor;
			wSize = USBDEVICEDESCRIPTOR_SIZE;
			break;
      
		case USB_DTYPE_CONFIGURATION :
			pnAddress = ( PU8 )&tUsbDeviceDescriptor;
			wSize = USBCONFIGURATIONDESCRIPTOR_SIZE;
			break;
      
		case USB_DTYPE_STRING :
			switch ( nIndex )
      {
				case USB_STRING_ID_LANGUAGE :
					pnAddress = ( PU8 )&tUsbLangueString;
					break;
			
        case USB_STRING_ID_MANUFACTURER :
					pnAddress = StringToDescriptor(( const PC8 )szUsbManfString );
					break;
				
        case USB_STRING_ID_PRODUCT :
					pnAddress = StringToDescriptor(( const PC8 )szUsbProdString );
					break;
        
        case USB_STRING_ID_SERIALNUM :
					pnAddress = StringToDescriptor( Usb_GetSerialNumberSctringDescriptor( ));
					break;
				
        case USB_STRING_ID_MSFT :
					pnAddress = ( PU8 )&tUsbMsftOSString;
					break;
			}
      
      // now set the size
			wSize = (( PUSBSTRINGDESCRIPTOR )pnAddress )->nLength;
			break;
	}

  // return the size
	return ( wSize );
}

/******************************************************************************
 * @function StringToDescriptor
 *
 * @brief convert a string to a descriptor
 *
 * This function will set up a descriptor for a stinrg
 * to it.
 *
 * @param[io]   pszString       pointer to the string
 *
 * @return      pointer to the converted descriptor
 *
 *****************************************************************************/
static PU8 StringToDescriptor( PC8 pszString )
{
  U16 wActLen, wMaxLen;
  
  // cast the endpoint buffer to be a descriptor pointer
	PUSBSTRINGDESCRIPTOR ptDesc = ( PUSBSTRINGDESCRIPTOR )anEp0BufIn;
  
  // comepute the length
  wActLen = strlen( pszString );
  wMaxLen = ( USB_EP0_BUF_SIZE - 2 ) / 2;
  
  // cap the actual length if necessary
  if ( wActLen > wMaxLen )
  {
    // set the actual length to the maximum
    wActLen = wMaxLen;
  }

  // set the length/type for this descriptor
  ptDesc->nLength = USB_STRING_LEN( wActLen );
  ptDesc->nDescriptorType = USB_DTYPE_STRING;

  // now copy the string
  for ( wMaxLen = 0; wMaxLen < wActLen; wMaxLen++ )
  {
    // copy the byte
    ptDesc->tString[ wMaxLen ] = *( pszString + wMaxLen );
  }

  // return the pointer
	return(( PU8 )ptDesc );
}

/******************************************************************************
 * @function ProcessMultiPacket
 *
 * @brief start a multi packet otuput
 *
 * This function start a packet output, adjust the size and pointers
 *
 *****************************************************************************/
static void ProcessMultiPacket( void )
{
  U16 wLclSize;
  
  // set the size
  wLclSize = wMultiPacketSize;
  
  // check if greater that endpoint size
  if ( wLclSize > USB_EP0_BUF_SIZE )
  {
    // set the local size to the maximum buffer size
    wLclSize = USB_EP0_BUF_SIZE;
  }
  
  // copy the data in
  memcpy( anEp0BufIn, pnMultiPacketPtr, wLclSize );
  
  // start a transfer
  Usb_EpStartIn( 0x80, anEp0BufIn, wLclSize, FALSE );
  
  // check for a size of 0
  if ( wLclSize == 0 )
  {
    // perform an input
    Usb_Ep0Out( );
  }
  
  // adjust the size and pointer
  wMultiPacketSize -= wLclSize;
  pnMultiPacketPtr += wLclSize;
}

/******************************************************************************
 * @function ProcessMsftCompatable
 *
 * @brief process a Microsoft request
 *
 * This function will set up a descriptor for a stinrg
 * to it.
 *
 * @param[in]   ptMsftComp    pointer to the compatable structure
 * @param[in]   ptMsftProp    pointer to the properties structure
 *
 *****************************************************************************/
static void ProcessMsftCompatable( PUSBMSFTCOMPDESCRIPTOR ptMsftCamp, PUSBMSFTPROPDESCRIPTOR ptMsftProp )
{
  U16   wLength;
  BOOL  bContinue = FALSE;
  
  if ( tSetupPacket.wIndex == 0x0005 )
  {
    // set the length/copy the props
    wLength = ptMsftProp->uLength;
    memcpy( anLclBuffer, ptMsftProp, wLength );
    bContinue = TRUE;
  }
  else if ( tSetupPacket.wIndex = 0x0004 )
  {
    // set the length/copy the comp
    wLength = ptMsftCamp->uLength;
    memcpy( anLclBuffer, ptMsftCamp, wLength );
    bContinue = TRUE;
  }
  else
  {
    // stall the endpoit
    Usb_Ep0Stall( );
  }
  
  // adjust length
  if ( wLength > tSetupPacket.wLength )
  {
    wLength = tSetupPacket.wLength;
  }
  
  // if continue flag
  if ( bContinue )
  {
    // start a transfer
    Usb_EpStartIn( 0x80, anLclBuffer, wLength, TRUE );
    Usb_Ep0Out( );
  }
}

/**@} EOF UsbDeviceHandler.c */
