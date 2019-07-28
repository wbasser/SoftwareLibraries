/******************************************************************************
 * @file USBCBCHandler.c
 *
 * @brief USB-CDC handler implementation
 *
 * This file provides the implementation for the USB-CDC handler
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
 * \addtogroup USBCBCHandler
 * @{
 *****************************************************************************/

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "USBCDCHandler/USBCBCHandler.h"

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------
#define NVM_USB_PAD_TRANSN_POS                ( 45 )
#define NVM_USB_PAD_TRANSN_SIZE               ( 5 )
#define NVM_USB_PAD_TRANSP_POS                ( 50 )
#define NVM_USB_PAD_TRANSP_SIZE               ( 5 )
#define NVM_USB_PAD_TRIM_POS                  ( 55 )
#define NVM_USB_PAD_TRIM_SIZE                 ( 3 )

/// define the endpoint size
#define ENDPOINT_MASK                         ( 0x3F )

/// define the endpoint mask
#if ( USB_ENDPOINT0_SIZE <= 8 )
#define USB_ENDPOINT0_GC                      ( 0 )
#elif ( USB_ENDPOINT0_SIZE <= 16 )
#define USB_ENDPOINT0_GC                      ( 1 )
#elif ( USB_ENDPOINT0_SIZE <= 32 )
#define USB_ENDPOINT0_GC                      ( 2 )
#elif ( USB_ENDPOINT0_SIZE <= 64 )
#define USB_ENDPOINT0_GC                      ( 3 )
#elif ( USB_ENDPOINT0_SIZE <= 128 )
#define USB_ENDPOINT0_GC                      ( 4 )
#elif ( USB_ENDPOINT0_SIZE <= 256 )
#define USB_ENDPOINT0_GC                      ( 5 )
#elif ( USB_ENDPOINT0_SIZE <= 512 )
#define USB_ENDPOINT0_GC                      ( 6 )
#else
#define USB_ENDPOINT0_GC                      ( 7 )
#endif

/// define the request type masks
#define USB_REQTYPE_DIRECTION_MASK            ( 0x80 )
#define USB_REQTYPE_TYPE_MASK                 ( 0x60 )
#define USB_REQTYPE_RECIPIENT_MASK            ( 0x1F )

/// define the request types
#define USB_REQTYPE_STANDARD                  ( 0x00 )
#define USB_REQTYPE_CLASS                     ( 0x20 )
#define USB_REQTYPE_VENDOR                    ( 0x40 ) 

/// define the endpoint
#define EP_CTLIO_IDX                          ( 0 )
#define EP_CDCIN_IDX                          ( 1 )
#define EP_CDCOUT_IDX                         ( 2 )
#define EP_DCDCOM_IDX                         ( 3 )

// enumerations ---------------------------------------------------------------
/// enumerate the standard request types
typedef enum _USBSTDREQ
{
  USB_STDREQ_GETSTATUS          = 0,
  USB_STDREQ_CLEARFEATURE       = 1,
  USB_STDREQ_SETFEATURE         = 3,
  USB_STDREQ_SETADDRESS         = 5,
  USB_STDREQ_GETDESCRIPTOR      = 6,
  USB_STDREQ_SETDESCRIPTOR      = 7,
  USB_STDREQ_GETCONFIGURATION   = 8,
  USB_STDREQ_SETCONFIGURATION   = 9,
  USB_STDREQ_GETINTERFACE       = 10,
  USB_STDREQ_SETINTERFACE       = 11,
  USB_STDREQ_SYNCHFRAME         = 12,
} USBSTDREQ;

/// enumerate the standard Descriptor Types
typedef enum _USBDESCTYPE
{
  USB_DESCTYPE_DEVICE                      = 0x01, ///< Indicates that the descriptor is a device descriptor
  USB_DESCTYPE_CONFIGURATION               = 0x02, ///< Indicates that the descriptor is a configuration descriptor
  USB_DESCTYPE_STRING                      = 0x03, ///< Indicates that the descriptor is a string descriptor
  USB_DESCTYPE_INTERFACE                   = 0x04, ///< Indicates that the descriptor is an interface descriptor
  USB_DESCTYPE_ENDPOINT                    = 0x05, ///< Indicates that the descriptor is an endpoint descriptor
  USB_DESCTYPE_DEVICEQUALIFIER             = 0x06, ///< Indicates that the descriptor is a device qualifier descriptor
  USB_DESCTYPE_OTHER                       = 0x07, ///< Indicates that the descriptor is of other type
  USB_DESCTYPE_INTERFACEPOWER              = 0x08, ///< Indicates that the descriptor is an interface power descriptor
  USB_DESCTYPE_INTERFACEASSOCIATION        = 0x0B, ///< Indicates that the descriptor is an interface association descriptor
  USB_DESCTYPE_CSINTERFACE                 = 0x24, ///< Indicates that the descriptor is a class specific interface descriptor
  USB_DESCTYPE_CSENDPOINT                  = 0x25, ///< Indicates that the descriptor is a class specific endpoint descriptor
} USBDESCTYPE;

/// enumerate the local endpoint buffer indices
typedef enum _LCLBUFIDX
{
  LCLBUF_IDX_CTL = 0,
  LCLBUF_IDX_BLK,
  LCLBUF_IDX_MAX
} LCLBUFIDX;

/// define the out/in device descriptor indices
typedef enum _EPINOUTIDX
{
  EPINOUT_OUT_IDX = 0,
  EPINOUT_IN_IDX,
  EPINOUT_MAX
} EPINOUTIDX;


// structures -----------------------------------------------------------------
/// define the request header 
typedef struct _USBREQHEADER
{
  U8            nRequestType;   ///< Type of the request.
  U8            nRequest;       ///< Request command code.
  U16           wValue;         ///< wValue parameter of the request.
  U16           wIndex;         ///< wIndex parameter of the request.
  U16           wLength;        ///< Length of the data to transfer in bytes.
} PACKED USBREQHEADER, *PUSBREQHEADER;
#define USBREQHEADER_SIZE             sizeof( USBREQHEADER )

// global parameter declarations ----------------------------------------------
static  U8  ALIGNED4 anUsbEpBufIn[ LCLBUF_IDX_MAX ][ USB_ENDPOINT_SIZE ];
static  U8  ALIGNED4 anUsbEpBufOut[ LCLBUF_IDX_MAX ][ USB_ENDPOINT_SIZE ];

// local parameter declarations -----------------------------------------------
static  UsbDeviceDescriptor atUsbEndpoints[ USB_NUMBER_ENDPOINTS ];
static  U8                  nConfiguration;

// local function prototypes --------------------------------------------------
static  void  ResetControlEndpoint( void );
static  void  ProcessStandardRequest( void );
static  void  SendDataEp( U8 nEndpoint, PU8 pnData, U16 wLength );
static  void  SendZlp( void );
static  void  SendStall( EPINOUTIDX eIdx );
static  U16   GetDescriptor( USBDESCTYPE eDescType, PU8 pnData );
static  void  CdcSetupEndpoints( void );

// constant parameter initializations -----------------------------------------

/******************************************************************************
 * @function USBCBCHandler_Initialize
 *
 * @brief USB CDC handler initialization
 *
 * This function will implement any needed initialization
 *
 *****************************************************************************/
void USBCBCHandler_Initialize( void )
{
  U32 uTransN, uTransP, uTrim;

  // Reset the device/wait for sync
  USB->DEVICE.CTRLA.reg = USB_CTRLA_SWRST;
  while ( USB->DEVICE.SYNCBUSY.bit.SWRST );

  // Load Pad Calibration
  uTransN = ( *(( PU32 )( NVMCTRL_OTP4 )	+ ( NVM_USB_PAD_TRANSN_POS / 32 )) >> ( NVM_USB_PAD_TRANSN_POS % 32 )) & (( BIT( NVM_USB_PAD_TRANSN_SIZE )) - 1 );
  if ( uTransN = 0x1F )
  {
    // clamp it at 5
    uTransN = 5;
  }
  
  uTransP = ( *(( PU32 )( NVMCTRL_OTP4 )	+ ( NVM_USB_PAD_TRANSP_POS / 32 )) >> ( NVM_USB_PAD_TRANSP_POS % 32 )) & (( BIT( NVM_USB_PAD_TRANSP_SIZE )) - 1 );
  if ( uTransP = 0x1F )
  {
    // clamp it at 29
    uTransP = 29;
  }
  
  uTrim = ( *(( PU32 )( NVMCTRL_OTP4 )	+ ( NVM_USB_PAD_TRIM_POS / 32 )) >> ( NVM_USB_PAD_TRIM_POS % 32 )) & (( BIT( NVM_USB_PAD_TRIM_SIZE )) - 1 );
  if ( uTransP = 0x7 )
  {
    // clamp it at 3
    uTrim = 3;
  }

  // now set it
  USB->DEVICE.PADCAL.reg = USB_PADCAL_TRANSN( uTransN ) | USB_PADCAL_TRANSP( uTransP ) | USB_PADCAL_TRIM( uTrim );
  
  // clear the endpoints
  memset( atUsbEndpoints, 0, USB_NUMBER_ENDPOINTS * sizeof( UsbDeviceDescriptor ));
  USB->DEVICE.DESCADD.reg = ( U32 )( &atUsbEndpoints[ 0 ] );
  USB->DEVICE.INTENSET.reg = USB_DEVICE_INTENSET_EORST;
  
  // full-speed
  USB->DEVICE.CTRLB.bit.SPDCONF = USB_DEVICE_CTRLB_SPDCONF_FS_VAL;

  // enable it
  USB->DEVICE.CTRLA.reg = USB_CTRLA_ENABLE | USB_CTRLA_MODE_DEVICE;
  while ( USB->DEVICE.SYNCBUSY.bit.ENABLE );

  // cause a reset
  ResetControlEndpoint( );
  
  // enable the interrupt/clear the detach bit
  NVIC_EnableIRQ( USB_IRQn );
  USB->DEVICE.CTRLB.bit.DETACH = 0;
  
  // clear the configuration
  nConfiguration = 0;
}
 
/******************************************************************************
 * @function Usb_Handler
 *
 * @brief USB interrupt handler
 *
 * This function will process the USB interrupt handler
 *
 *****************************************************************************/
void USB_Handler( ) 
{
  U32 uSummary, uStatus;
  U16 iIdx;

  // get the summary and status flags
  uSummary = USB->DEVICE.EPINTSMRY.reg;
  uStatus = USB->DEVICE.INTFLAG.reg;
  
  // is this an EORST
  if ( uStatus & USB_DEVICE_INTFLAG_EORST )
  {
    USB->DEVICE.INTFLAG.reg = USB_DEVICE_INTFLAG_EORST;
    ResetControlEndpoint( );
  }
  else
  {
    // check for normal request
    if ( uSummary & 1 )
    {
      U32 uFlags = USB->DEVICE.DeviceEndpoint[ 0 ].EPINTFLAG.reg;
      USB->DEVICE.DeviceEndpoint[ 0 ].EPINTFLAG.reg = USB_DEVICE_EPINTFLAG_TRCPT1 | USB_DEVICE_EPINTFLAG_TRCPT0 | USB_DEVICE_EPINTFLAG_RXSTP;
      
      // now check for a setup packet
      if ( uFlags & USB_DEVICE_EPINTFLAG_RXSTP )
      {
        // handle the request for standard requests
        ProcessStandardRequests( );
      }

      // check for a control out packet
      if ( uFlags & USB_DEVICE_EPINTFLAG_TRCPT0 )
      {
        // call the abstraction handler
      }

      // check for a control in packet
      if ( uFlags & USB_DEVICE_EPINTFLAG_TRCPT1 )
      {
        // call the abstraction handler
      }
    }

    // now ceck for each endpoint
    for ( iIdx = 1; iIdx < USB_NUMBER_ENDPOINTS; iIdx++ )
    {
      if ( uSummary & BIT( iIdx ))
      {
        uStatus = USB->DEVICE.DeviceEndpoint[ iIdx ].EPINTFLAG.reg;
        USB->DEVICE.DeviceEndpoint[ iIdx ].EPINTENCLR.reg = uStatus;
      }
    }

    // handle the endpoints callback
  }
}

 /******************************************************************************
 * @function ResetControlEndpoint
 *
 * @brief reset the control endpoint
 *
 * This function will reset the endpoint and reset the controller
 *
 *****************************************************************************/
static void ResetControlEndpoint( void )
{
  // reset the control endpoint
  atUsbEndpoints[ EP_CTLIO_IDX ].DeviceDescBank[ 0 ].ADDR.reg = ( U32 )&anUsbEpBufOut[ LCLBUF_IDX_CTL ];
  atUsbEndpoints[ EP_CTLIO_IDX ].DeviceDescBank[ 0 ].PCKSIZE.bit.SIZE = USB_ENDPOINT0_GC;
  atUsbEndpoints[ EP_CTLIO_IDX ].DeviceDescBank[ 1 ].ADDR.reg = ( U32 )&anUsbEpBufIn[ LCLBUF_IDX_CTL ];
  atUsbEndpoints[ EP_CTLIO_IDX ].DeviceDescBank[ 1 ].PCKSIZE.bit.SIZE = USB_ENDPOINT0_GC;
  atUsbEndpoints[ EP_CTLIO_IDX ].DeviceDescBank[ 1 ].PCKSIZE.bit.MULTI_PACKET_SIZE = 8;
  atUsbEndpoints[ EP_CTLIO_IDX ].DeviceDescBank[ 1 ].PCKSIZE.bit.BYTE_COUNT = 0;

  // enable the reset interrupt
  USB->DEVICE.DeviceEndpoint[ EP_CTLIO_IDX ].EPINTENSET.reg = USB_DEVICE_EPINTENSET_RXSTP;
  USB->DEVICE.DeviceEndpoint[ EP_CTLIO_IDX ].EPCFG.reg  = USB_DEVICE_EPCFG_EPTYPE0( 1 ) | USB_DEVICE_EPCFG_EPTYPE1( 1 );
}

/******************************************************************************
 * @function ProcessStandRequest
 *
 * @brief standard request handler
 *
 * This function will process the standard requests
 *
 *****************************************************************************/
static  void  ProcessStandardRequest( void )
{
  PUSBREQHEADER ptReqHeader;
  U8            anBufIdx = 0;
  U16UN         tTemp;
  PU8           pnData;
  
  // point to the buffer
  ptReqHeader = ( PUSBREQHEADER )&anUsbEpBufOut[ LCLBUF_IDX_CTL ][ 0 ];
  
  if (( ptReqHeader->nRequestType & USB_REQTYPE_TYPE_MASK ) == USB_REQTYPE_STANDARD )
  {
    // now determine the request
    switch( ptReqHeader->nRequest )
    {
      case USB_STDREQ_GETSTATUS :
        // set the status to 0, send it
        tTemp.wValue = 0;
        SendDataEp( EP_CTLIO_IDX, &tTemp, U16UNSIZE );
        break;
        
      case USB_STDREQ_CLEARFEATURE :
        // send ZLP
        SendZlp();
        break;
        
      case USB_STDREQ_SETFEATURE :
        // stall
        SendStall( EPINOUT_IDX_IN );
        break;
        
      case USB_STDREQ_SETADDRESS :
        // get the value
        tTemp.anValue[ LE_U16_LSB_IDX ] = anUsbEpBufOut[ LCLBUF_IDX_CTL ][ 2 ];
        tTemp.anValue[ LE_U16_MSB_IDX ] = anUsbEpBufOut[ LCLBUF_IDX_CTL ][ 3 ];
        
        // send ZLP/set the address
        SendZlp();
        USB->DEVICE>DADD.reg = USB_DEVICE_DADD_ADDEN | tTemp.wValue;
        beak;
        
      case USB_STDREQ_GETDESCRIPTOR :
        if (( tTemp.wValue = GetDescriptor( ptReqHeader->nRequest )) != 0 )
        {
          // send it
          SendDataEp( EP_CTLIO_IDX, pnData, MIN( tTemp.wValue, ptReqHeader->wLength ));
        }
        else
        {
          // send a stall
          SendStall( EPINTOUT_IDX_IN );
        }
        break;
        
      case USB_STDREQ_SETDESCRIPTOR :
        break;
        
      case USB_STDREQ_GETCONFIGURATION :
        // send the config
        SendDataEp( 0, &nConfiguration, sizeof( U8 ));
        break;
        
      case USB_STDREQ_SETCONFIGURATION :
        // store the configuration/send ZLP
        nConfiguration = anUsbEpBufOut[ LCLBUF_IDX_CTL ][ 2 ];
        SendZlp();
        
        // set up the CDC endpoints
        CdcSetupEndpoints( );
        break;
        
      case USB_STDREQ_GETINTERFACE :
        break;
        
      case USB_STDREQ_SETINTERFACE :
        break;
        
      case USB_STDREQ_SYNCHFRAME :
        break;
        
      // should never happen
      default :
        break;
    }
  }
}

/******************************************************************************
 * @function SendDataEp
 *
 * @brief send data
 *
 * This function will send data only the given endpoint
 *
 * @param[in]   nEndpoint   endpoint
 * @param[in[   pnData      pointer to the data
 * @param[in]   wLength     length of the data
 *
 *****************************************************************************/
static void SendDataEp( U8 nEndpoint, PU8 pnData, U16 wLength  )
{
  U32 uDataAddr;
  
  // check for multi packet
  if ( wLength > USB_ENDPOINT_SIZE )
  {
    // set the address
    uDataAddr = ( U32 )pnData;
    
    // enable the auto ZLP
    atUsbEndpoints[ nEndpoint ].DeviceDescBank[ EPINOUT_IN_IDX ].PCKSIZE.bit.AUTO_ZLP = ON;
  }
  else
  {
    // copy to local buffer/set the address
    memcpy( anUsbEpBufIn[ nEndpoint ], pnData, wLength );
    uDataAddr = ( U32 )&anUwbEpBufIn[ nEndpoint ];
  }

  // set the address/byte count/clear multi packet
  atUsbEndpoints[ nEndpoint ].DeviceDescBank[ EPINOUT_IN_IDX ].ADDR.reg = uDataAddr;
  atUsbEndpoints[ nEndpoint ].DeviceDescBank[ EPINOUT_IN_IDX ].PCKSIZE.bit.BYTE_COUNT = wLength;
  atUsbEndpoints[ nEndpoint ].DeviceDescBank[ EPINOUT_IN_IDX ].PCKSIZE.bit.MULTI_PACKET_SIZE = 0;
  
  // clear the transfer complete/set bank as ready
  USB->DEVICE.DeviceEndpoint[ nEndpoint ].EPINTFLAG.bit.TRCPT1 = ON;
  USB->DEVICE.DeviceEndpoint[ nEndpoint ].EPSTATUSSET.bit.BK1RDY = ON;
}
 
/******************************************************************************
 * @function SendZlp
 *
 * @brief send a zero length packet
 *
 * This function will send a zero length packet
 *
 *****************************************************************************/
static void SendZlp( void )
{
  // set the byte count to 0/clear the transfer complete/set bank as ready
  atUsbEndpoints[ nEndpoint ].DeviceDescBank[ EPINOUT_IN_IDX ].PCKSIZE.bit.BYTE_COUNT = wLength;
  USB->DEVICE.DeviceEndpoint[ nEndpoint ].EPINTFLAG.bit.TRCPT1 = ON;
  USB->DEVICE.DeviceEndpoint[ nEndpoint ].EPSTATUSSET.bit.BK1RDY = ON;
}

/******************************************************************************
 * @function SendStall
 *
 * @brief send a stall
 *
 * This function will stall a endpoint
 *
 * @param[in]   eIdx    in/out directon enumeration
 *
 *****************************************************************************/
static void SendStall( EPINOUTIDX eIdx )
{
  // set the stall bit
  if ( eIdx == EPINOUT_IDX_OUT )
  {
    // set the stallr equest
    USB->DEVICE.DeviceEndpoint[0].EPSTATUSSET.bit.STALL0 = ON;
  }
  else
  {
    // set the stallr equest
    USB->DEVICE.DeviceEndpoint[0].EPSTATUSSET.bit.STALL1 = ON;
  }
}

/******************************************************************************
 * @function GetDescriptor
 *
 * @brief get a descriptor
 *
 * This function will determine if a valid descriptor is found and returns
 * it pointer and length
 *
 * @param[in]   eDescType   descriptor type
 * @param[i0]   pnData      pointer to the descriptor
 *
 * @return      the length of the descriptor if valid, 0 if not
 *
 *****************************************************************************/
static U16 GetDescriptor( USBDESCTYPE eDescType, PU8 pnData )
{
  U16 wLength = 0;

  // determine the type of descriptor
  switch( nDescType )
  {
    case USB_DESCTYPES_DEVICE :
      pnData = ( PU8 )&anDevDescriptor;
      wLength = sizeof( anDevDescriptor );
      break;
      
    case USB_DESCTYPES_CONFIGURATION :
      pnData = ( PU8 )&anCfgDescriptor;
      wLength = sizeof( anCfgDescriptor );
      break;
      
    case USB_DESCTYPES_STRING :
      break;
      
    case USB_DESCTYPES_INTERFACE :
      break;
      
    case USB_DESCTYPES_ENDPOINT :
      break;
      
    case USB_DESCTYPES_DEVICEQUALIFIER :
      break;
      
    case USB_DESCTYPES_OTHER : 
      break;
      
    case USB_DESCTYPES_INTERFACEPOWER :
      break;
      
    case USB_DESCTYPES_INTERFACEASSOCIATION :
      break;
      
    case USB_DESCTYPES_CSINTERFACE :
      break;
      
    case USB_DESCTYPES_CSENDPOINT :
      break;
  
    default :
      break;
  }
  
  // return the length
  return( wLength );
}

/******************************************************************************
 * @function CdcSetupEndpoints
 *
 * @brief set up the CDC endpoints
 *
 * This function will set up the endpoints for the CDC handler
 *
 *****************************************************************************/
static void CdcSetupEndpoints( void )
{
  // configure the output endpoing for CDC/64 bytes packets, 
  USB->DEVICE.DeviceEndpoint[ EP_CDCOUT_IDX ].EPCFG.reg = USB_DEVICE_EPCFG_EPTYPE0( 3 );
  usb_endpoint_table[USB_EP_OUT].DeviceDescBank[0].PCKSIZE.bit.SIZE = 3;
  pUsb->DEVICE.DeviceEndpoint[USB_EP_OUT].EPSTATUSSET.reg = USB_DEVICE_EPSTATUSSET_BK0RDY;
  /* Configure the data buffer */
  usb_endpoint_table[USB_EP_OUT].DeviceDescBank[0].ADDR.reg = (uint32_t)&udd_ep_out_cache_buffer[1];
  /* Configure BULK IN endpoint for CDC Data interface */
  pUsb->DEVICE.DeviceEndpoint[USB_EP_IN].EPCFG.reg = USB_DEVICE_EPCFG_EPTYPE1(3);
  /* Set maximum packet size as 64 bytes */
  usb_endpoint_table[USB_EP_IN].DeviceDescBank[1].PCKSIZE.bit.SIZE = 3;
  pUsb->DEVICE.DeviceEndpoint[USB_EP_IN].EPSTATUSCLR.reg = USB_DEVICE_EPSTATUSCLR_BK1RDY;
  /* Configure the data buffer */
  usb_endpoint_table[USB_EP_IN].DeviceDescBank[1].ADDR.reg = (uint32_t)&udd_ep_in_cache_buffer[1];
  /* Configure INTERRUPT IN endpoint for CDC COMM interface*/
  pUsb->DEVICE.DeviceEndpoint[USB_EP_COMM].EPCFG.reg = USB_DEVICE_EPCFG_EPTYPE1(4);
  /* Set maximum packet size as 64 bytes */
  usb_endpoint_table[USB_EP_COMM].DeviceDescBank[1].PCKSIZE.bit.SIZE = 0;
  pUsb->DEVICE.DeviceEndpoint[USB_EP_COMM].EPSTATUSCLR.reg = USB_DEVICE_EPSTATUSCLR_BK1RDY;
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
/**@} EOF USBCBCHandler.c */
