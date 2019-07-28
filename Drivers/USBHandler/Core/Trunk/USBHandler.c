/******************************************************************************
 * @file USBHandler.c
 *
 * @brief USB Handler implementation 
 *
 * This file provides the implementation for the USB handler
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
 * \addtogroup USBHandler
 * @{
 *****************************************************************************/

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "USBHandler/USBHandler.h"

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations ----------------------------------------------

// local parameter declarations -----------------------------------------------
static  U16   wCurDescSize;
static  PU8   pnCurDescriptor;
static  U8    nCurConfiguration;

// local function prototypes --------------------------------------------------
static  U16   GetDescriptor( U8 nType, U8 nIndex, PU8* ppnDescriptor );
static  void  MultiTranserEp0( void );

// constant parameter initializations -----------------------------------------

/******************************************************************************
 * @function USBHandler_ProcessSetupRequest
 *
 * @brief process the setup request
 *
 * This function will parse the setup request and act upon the content
 *
 *****************************************************************************/
void USBHandler_ProcessSetupRequest( void )
{
  USBREQHEADER  tReqHeader;
  U8            nBufIdx, nDescType, nDescIndex;
  U16           wSize;
  PU8           pnDescriptor;
  
  // copy the data from endpoint 0
  memcpy( &tReqHeader, &g_anUsbEp0BufOut, USBREQHEADER_SIZE );
  
  // clear the buffer index
  nBufIdx = 0;
  
  // now process it
  if (( tReqHeader.nRequestType & USB_REQTYPE_TYPE_MASK ) == USB_REQTYPE_STANDARD )
  {
    // now process the request
    switch( tReqHeader.nRequest )
    {
      case USB_CNTLREQ_GETSTATUS :
        // fill the buffer and output it
        g_anUsbEp0BufIn[ nBufIdx++ ] = 0;
        g_anUsbEp0BufIn[ nBufIdx++ ] = 0;
        Usb_Ep0In( nBufIdx );
        
        // restart buffer out
        Usb_Ep0Out( );
        break;
        
      case USB_CNTLREQ_CLEARFEATURE :
      case USB_CNTLREQ_SETFEATURE :
        // send null buffer/restart buffer out
        Usb_Ep0In( nBufIdx );
        Usb_Ep0Out( );
        break;

      case USB_CNTLREQ_SETADDRESS :
        // send null buffer/restart buffer out
        Usb_Ep0In( nBufIdx );
        Usb_Ep0Out( );
        break;
        
      case USB_CNTLREQ_GETDESCRIPTOR :
        // get the descriptor type/index
        nDescType = tReqHeader.wValue >> 8;
        nDescIndex = tReqHeader.wValue & 0xFF;
        pnDescriptor = NULL;
        
        // now get the descriptor
        wSize = GetDescriptor( nDescType, nDescIndex, &pnDescriptor );
        
        // check for valid
        if (( wSize != 0 ) && ( pnDescriptor != NULL ))
        {
          // check for size
          if ( wSize > tReqHeader.wLength )
          {
            // force it to the max requested
            wSize = tReqHeader.wLength;
          }
          
          // check for empty
          if ( pnDescriptor == g_anUsbEp0BufIn )
          {
            // set size to 0
            wCurDescSize = 0;
            Usb_Ep0In( wSize );
          }
          else
          {
            // set up for multi
            wCurDescSize = wSize;
            pnCurDescriptor = pnDescriptor;
            
            // start a multi transfer
            MultiTranserEp0( );
          }
        }
        break;
        
      case USB_CNTLREQ_GETCONFIGURATION :
        // fill and send
        g_anUsbEp0BufIn[ nBufIdx++ ] = nCurConfiguration;
        Usb_Ep0In( nBufIdx );
        break;
        
      case USB_CNTLREQ_SETCONFIGURATION :
        // call the abstraction layer
        if ( USBHandler_SetConfigurtionCB( tReqHeader.wValue ) == FALSE )
        {
          // copy the configuration value
          nCurConfiguration = ( U8 )tReqHeader.wValue;
          
          // send null buffer/restart buffer out
          Usb_Ep0In( nBufIdx );
          Usb_Ep0Out( );
        }
        else
        {
          // stall the indpoint
          Usb_Ep0Stall( );
        }
        break;
        
      case USB_CNTLREQ_SETINTERFACE :
        // call the abstraction layer
        if ( USBHandler_SetInterfaceCB( tReqHeader.wValue ) == FALSE )
        {
          // send null buffer/restart buffer out
          Usb_Ep0In( nBufIdx );
          Usb_Ep0Out( );
        }
        else
        {
          // stall the indpoint
          Usb_Ep0Stall( );
        }
        break;
        
      default :
        break;
    }
  }
  else
  {
    // stall the endpoint 
    Usb_Ep0Stall( );
  }
}

/******************************************************************************
 * @function USBHandler_ProcessControlOut
 *
 * @brief process the control out
 *
 * This function will handle the control out request
 *
 *****************************************************************************/
void USBHandler_ProcessControlOut( void )
{
  PUSBREQHEADER ptRequest;
  
  // set the pointer
  ptRequest = ( PUSBREQHEADER )&g_anUsbEp0BufOut;
  
  // check for a standard request
  if (( ptRequest->nRequestType & USB_REQTYPE_TYPE_MASK ) != USB_REQTYPE_STANDARD )
  {
    // call the local completion routine
    USBHandler_ControlOutCB( );
  }
}

/******************************************************************************
 * @function USBHandler_ProcessControlIn
 *
 * @brief process the control in
 *
 * This function will handle the control in request
 *
 *****************************************************************************/
void  USBHandler_ProcessControlIn( void )
{
  PUSBREQHEADER ptRequest;
  
  // set the pointer
  ptRequest = ( PUSBREQHEADER )&g_anUsbEp0BufOut;
  
  // check for a standard request
  if (( ptRequest->nRequestType & USB_REQTYPE_TYPE_MASK ) == USB_REQTYPE_STANDARD )
  {
    // process it
    switch( ptRequest->nRequest )
    {
      case USB_STDREQ_SETADDRESS :
        // set the address
        Usb_SetAddress( ptRequest->wValue & 0x7F );
        break;
        
      case USB_STDREQ_GETDESCRIPTOR :
        // call the multi handler
        MultiTranserEp0( );
        break;
        
      default :
        break;
    }
  }
  else
  {
    // call the local completion routine
    USBHandler_ControlInCB( );
  }
}
 
/******************************************************************************
 * @function GetDescriptor
 *
 * @brief get a descriptor
 *
 * This function will get and return the pointer to the selected descriptor
 *
 * @param[in]   nType         descriptor type
 * @param[in]   nIndex        descriptor index
 * @param[io]   ppnDescriptor pointer to store the descriptor in
 *
 * @return      size of the descriptor
 *
 *****************************************************************************/
static U16 GetDescriptor( U8 nType, U8 nIndex, PU8* ppnDescriptor )
{
  PU8       pnAddress;
  U16       wSize;
  
  // clear the size
  wSize = 0;

  // now determine the type
  switch ( nType)
  {
    case USB_DESCTYPES_DEVICE:
      // set the address/size
      pnAddress = ( PU8 )&g_tDeviceDescriptor;
      wSize = USBDEVDESC_SIZE;
      break;
      
    case USB_DESCTYPES_CONFIGURATION:
      // set the address/size
      pnAddress = ( PU8 )&g_tConfigDescriptor;
      wSize = USBCONFDESC_SIZE;
      break;
      
    case USB_DESCTYPES_STRING:
      // determine the string index
      switch ( nIndex )
      {
        case STRING_ID_LANGUAGE:
          pnAddress = ( PU8 )&g_tLanguageString;
          break;
          
        case STRING_ID_MANUFACUTRER:
          pnAddress = ( PU8 )&g_tManufacuterString;
          break;
          
        case STRING_ID_PRODUCT:
          pnAddress = ( PU8 )&g_tProductString;
          break;
          
        default :
          break;
      }
      
      // set the size
      wSize = (( PUSBSTRDESC )pnAddress )->tHeader.nLength;
      break;
  }

  // store the address
  *( ppnDescriptor ) = pnAddress;
  
  // return the size
  return( wSize );
}

/******************************************************************************
 * @function MultiTranserEp0
 *
 * @brief multi size endpoint 0 handler
 *
 * This function will handle transfers to endpoint 0 that require multiple 
 * transfers
 *
 *****************************************************************************/
static void MultiTranserEp0( void )
{
  U16 wSize;
  
  // get the current size
  wSize =wCurDescSize;
  
  // check for max
  if ( wSize > USB_ENDPOINT0_SIZE )
  {
    // set the to the max
    wSize = USB_ENDPOINT0_SIZE;
  }
  
  // copy the data into the buffer
  memcpy( g_anUsbEp0BufIn, pnCurDescriptor, wSize );
  Usb_Ep0In( wSize );
  
  // if size is 0
  if ( wSize == 0 )
  {
    // restart the out
    Usb_Ep0Out( );
  }
  
  // adjust the size
  wCurDescSize -= wSize;
  pnCurDescriptor += wSize;
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

/**@} EOF USBHandler.c */
