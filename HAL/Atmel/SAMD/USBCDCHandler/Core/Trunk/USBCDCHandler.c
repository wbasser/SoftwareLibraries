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
#include "USBCDCHandler/USBCDCHandler.h"

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------
/// define the ovffset/shifts for the NVM 
#define NVM_USB_TRANSN_POS                    ( 45 )
#define NVM_USB_TRANSN_SIZE                   (  5 )
#define NVM_USB_TRANSP_POS                    ( 50 )
#define NVM_USB_TRANSP_SIZE                   (  5 )
#define NVM_USB_TRIM_POS                      ( 55 )
#define NVM_USB_TRIM_SIZE                     (  3 )

/// define the macro for getting a NVM value
#define NVM_READ_CAL(cal) \
    ((*(( PU32 )NVMCTRL_OTP4 + NVM_##cal##_POS / 32)) >> (NVM_##cal##_POS % 32)) & ((1 << NVM_##cal##_SIZE) - 1)

/// define the endpoint size
#define USB_ENDPOINT_MASK                     ( 0x3F )


#define USB_ENDPOINT0_SIZE                    ( USB_ENDPOINT_SIZE )

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

/// define the standard request
#define STD_GET_STATUS_ZERO           0x0080
#define STD_GET_STATUS_INTERFACE      0x0081
#define STD_GET_STATUS_ENDPOINT       0x0082
#define STD_CLEAR_FEATURE_ZERO        0x0100
#define STD_CLEAR_FEATURE_INTERFACE   0x0101
#define STD_CLEAR_FEATURE_ENDPOINT    0x0102
#define STD_SET_FEATURE_ZERO          0x0300
#define STD_SET_FEATURE_INTERFACE     0x0301
#define STD_SET_FEATURE_ENDPOINT      0x0302
#define STD_SET_ADDRESS               0x0500
#define STD_GET_DESCRIPTOR            0x0680
#define STD_SET_DESCRIPTOR            0x0700
#define STD_GET_CONFIGURATION         0x0880
#define STD_SET_CONFIGURATION         0x0900
#define STD_GET_INTERFACE             0x0A81
#define STD_SET_INTERFACE             0x0B01
#define STD_SYNCH_FRAME               0x0C82

/// define the CDC Class Specific Request Code
#define GET_LINE_CODING               0x21A1
#define SET_LINE_CODING               0x2021
#define SET_CONTROL_LINE_STATE        0x2221

// enumerations ---------------------------------------------------------------
/// enumerate the local buffers
typedef enum _LCLBUFIDX
{
  LCLBUF_IDX_CTL = 0,
  LCLBUF_IDX_BLK,
  LCLBUF_IDX_MAX
} LCLBUFIDX;

/// define the endpoint type for config
typedef enum _USBEPTYPECNFG
{
  USB_EPTYPE_CNFG_DISABLED = 0,
  USB_EPTYPE_CNFG_CONTROL,
  USB_EPTYPE_CNFG_ISOCHRONOUS,
  USB_EPTYPE_CNFG_BULK,
  USB_EPTYPE_CNFG_INTERRUPT,
  USB_EPTYPE_CNFG_MAX
} USBEPTYPECNFG;

// structures -----------------------------------------------------------------
/// define the line coding structure
typedef struct _USBCDCLINECODING
{
  U32     uBaudrate;        ///< baudrate
  U8      nCharFormat;      ///< character format
  U8      nParity;          ///< parity
  U8      nDataBits;        ///< number of data bits
} USBCDCLINECODING, *PUSBCDCLINECODING;
#define USBCDCLINECODING_SIZE                   sizeof( USBCDCLINECODING )

// global parameter declarations ----------------------------------------------
static  U8  ALIGNED4 anUsbEpBufIn[ LCLBUF_IDX_MAX ][ USB_ENDPOINT_SIZE ];
static  U8  ALIGNED4 anUsbEpBufOut[ LCLBUF_IDX_MAX ][ USB_ENDPOINT_SIZE ];

// local parameter declarations -----------------------------------------------
static  UsbDeviceDescriptor ALIGNED4  atUsbEndpoints[ USB_ENDPOINT_MAX ];
static  U8                            nConfiguration;
static  U8                            nCdcConfiguration;
static  VBOOL                         bReadInProgress;

// local function prototypes --------------------------------------------------
static  U8    IsConfigured( void );
static  void  CdcEnumerate( void );
static  void  LclSendZlp( void );
static  void  LclSendStall( BOOL bDirectionIn );
static  void  LclSendData( USBENDPOINT eEndpoint, PU8 pnData, U16 wLength );

// constant parameter initializations -----------------------------------------
static  const USBCDCLINECODING tLineCoding =
{
  115200,     ///< baudrate
  0,          ///< stop bit
  0,          ///< parity
  8           ///< data bits
};

/******************************************************************************
 * @function USBCDCHandler_Initialize
 *
 * @brief USB CDC handler initialization
 *
 * This function will implement any needed initialization
 *
 *****************************************************************************/
void USBCDCHandler_Initialize( void )
{
  U32 uTransN, uTransP, uTrim;

  // local initialization
  USBCDCHandler_LocalInitialize( );

  // Reset the device/wait for sync
  USB->DEVICE.CTRLA.reg = USB_CTRLA_SWRST;
  while ( USB->DEVICE.SYNCBUSY.bit.SWRST );

  // set the pad calibration values
  USB->DEVICE.PADCAL.bit.TRANSN = NVM_READ_CAL( USB_TRANSN );
  USB->DEVICE.PADCAL.bit.TRANSP = NVM_READ_CAL( USB_TRANSP );
  USB->DEVICE.PADCAL.bit.TRIM   = NVM_READ_CAL( USB_TRIM );

  // set device mode/enable run in standby/set the descriptor address/set full speed/attach
  USB->DEVICE.CTRLA.bit.MODE = 0;
  USB->DEVICE.CTRLA.bit.RUNSTDBY = ON;
  USB->DEVICE.DESCADD.reg = ( U32 )&atUsbEndpoints[ 0 ];
  USB->DEVICE.CTRLB.bit.SPDCONF = USB_DEVICE_CTRLB_SPDCONF_FS_Val;
  USB->DEVICE.CTRLB.reg &= ~USB_DEVICE_CTRLB_DETACH;
  
  // clear the configuration
  nConfiguration = nCdcConfiguration = 0;

  // clear read in progress
  bReadInProgress = FALSE;

  // clear the enpoint table
  memset( &atUsbEndpoints[ 0 ], 0, sizeof( atUsbEndpoints ));

  // enable the device
  USB->DEVICE.CTRLA.bit.ENABLE = ON;
}

/******************************************************************************
 * @function USBCDCHandler_AreCharsAvailable
 *
 * @brief test for available characters
 *
 * This function check for characters available
 *
 * @param[in]   nDev   device enum
 *
 * @return    TRUE if characters are available
 *
 *****************************************************************************/
BOOL USBCDCHandler_AreCharsAvailable( U8 nDev )
{
  BOOL  bCharsAvail = FALSE;

  // check for configuration
  if ( IsConfigured( ) != 0 )
  {
    // we are configured - check for received characters
    bCharsAvail = ( USB->DEVICE.DeviceEndpoint[ USB_ENDPOINT_OUT ].EPINTFLAG.reg & USB_DEVICE_EPINTFLAG_TRCPT0 ) ? TRUE : FALSE;
  }

  // return the status
  return( bCharsAvail );
}

/******************************************************************************
 * @function USBCDCHandler_IsConfigured
 *
 * @brief test for configuration
 *
 * This function check for characters available
 *
 * @param[in]   nDev   device enum
 *
 * @return    TRUE if configured
 *
 *****************************************************************************/
BOOL USBCDCHandler_IsConfigured( U8 nDev )
{
  // return the configuration
  return(( USB_FSMSTATUS_FSMSTATE_ON_Val  == USB->DEVICE.FSMSTATUS.bit.FSMSTATE ) ? TRUE : FALSE );
}


/******************************************************************************
 * @function USBCDCHandler_GetData
 *
 * @brief returns the available characters
 *
 * This function returns the available character and returns the number
 *
 * @param[in]   nDev        device enumeration
 * @param[in[   pnData      pointer to the data
 * @param[in]   wLength     length of the data
 *
 * @return    number characters read
 *
 *****************************************************************************/
U16 USBCDCHandler_GetData( U8 nDev, PU8 pnData, U16 wLength )
{
  U16 wNumChars = 0;

  // check for configuration
  if ( IsConfigured( ) != 0 )
  {
    // check for a read in progress
    if ( bReadInProgress == FALSE )
    {
      // set the buffer address/byte count to 0/clear multi pakcet size
      atUsbEndpoints[ USB_ENDPOINT_OUT ].DeviceDescBank[ 0 ].ADDR.reg = ( U32 )&anUsbEpBufOut[ LCLBUF_IDX_BLK ];
      atUsbEndpoints[ USB_ENDPOINT_OUT ].DeviceDescBank[ 0 ].PCKSIZE.bit.BYTE_COUNT = 0;
      atUsbEndpoints[ USB_ENDPOINT_OUT ].DeviceDescBank[ 0 ].PCKSIZE.bit.MULTI_PACKET_SIZE = USB_ENDPOINT_SIZE;
      USB->DEVICE.DeviceEndpoint[ USB_ENDPOINT_OUT ].EPINTFLAG.reg = USB_DEVICE_EPINTFLAG_TRCPT0;

      // clear the bankd ready bit/set the read in progress
      USB->DEVICE.DeviceEndpoint[ USB_ENDPOINT_OUT ].EPSTATUSCLR.bit.BK0RDY = ON;
      bReadInProgress = TRUE;
    }

    // check for transfer complete
    if ( USB->DEVICE.DeviceEndpoint[ USB_ENDPOINT_OUT ].EPINTFLAG.reg & USB_DEVICE_EPINTFLAG_TRCPT0 )
    {
      // get the data size/copy the data/clear transfer complete flag/clear read in progress
      wNumChars = MIN( atUsbEndpoints[ USB_ENDPOINT_OUT ].DeviceDescBank[ 0 ].PCKSIZE.bit.BYTE_COUNT, wLength );
      memcpy( pnData, &anUsbEpBufOut[ LCLBUF_IDX_BLK ], wNumChars );
      USB->DEVICE.DeviceEndpoint[ USB_ENDPOINT_OUT ].EPINTFLAG.reg = USB_DEVICE_EPINTFLAG_TRCPT0;
      bReadInProgress = FALSE;
    }
  }

  // return it
  return( wNumChars );
}

/******************************************************************************
 * @function USBCDCHandler_PutData
 *
 * @brief write characters
 *
 * This function writes character
 *
 * @param[in]   nDev        device enumeration
 * @param[in[   pnData      pointer to the data
 * @param[in]   wLength     length of the data
 *
 * @return    number characters written
 *
 *****************************************************************************/
U16 USBCDCHandler_PutData( U8 nDev, PU8 pnData, U16 wLength )
{
  U16 wNumChars = 0;

  // call the local write
  LclSendData( USB_ENDPOINT_IN, pnData, wLength );

  // set the number of characters to the length
  wNumChars = wLength;

  // return it
  return( wNumChars );
}

/******************************************************************************
 * @function USBCDCHandler_Close
 *
 * @brief close the device
 *
 * This function closes the device
 *
 * @param[in]   nDev        device enumeration
 *
 *****************************************************************************/
void USBCDCHandler_Close( U8 nDev )
{
  USB->DEVICE.CTRLB.reg |= USB_DEVICE_CTRLB_DETACH;
  
  // clear the configuration
  nConfiguration = 0;

  // dsable the device
  USB->DEVICE.CTRLA.bit.ENABLE = OFF;
}
 
/******************************************************************************
 * @function IsConfigured
 *
 * @brief checks for configuration
 *
 * This function will check for configuration and return the current config
 *
 * @return      current configuration
 *
 *****************************************************************************/
static U8 IsConfigured( void )
{
  // check for End of Reset Flag
  if ( USB->DEVICE.INTFLAG.reg & USB_DEVICE_INTFLAG_EORST )
  {
    // clear the flag
    USB->DEVICE.INTFLAG.reg = USB_DEVICE_INTFLAG_EORST;

    // set device address to 0
    USB->DEVICE.DADD.reg = USB_DEVICE_DADD_ADDEN | 0;

    // configure enpoint 0 for control IN/and control OUT
    USB->DEVICE.DeviceEndpoint[ USB_ENDPOINT_CTL ].EPCFG.reg = USB_DEVICE_EPCFG_EPTYPE0( USB_EPTYPE_CNFG_CONTROL ) | USB_DEVICE_EPCFG_EPTYPE1( USB_EPTYPE_CNFG_CONTROL );
    USB->DEVICE.DeviceEndpoint[ USB_ENDPOINT_CTL ].EPSTATUSSET.reg = USB_DEVICE_EPSTATUSSET_BK0RDY;
    USB->DEVICE.DeviceEndpoint[ USB_ENDPOINT_CTL ].EPSTATUSCLR.reg = USB_DEVICE_EPSTATUSCLR_BK1RDY;

    // configure the packet sizes
    atUsbEndpoints[ USB_ENDPOINT_CTL ].DeviceDescBank[ 0 ].PCKSIZE.bit.SIZE = USB_ENDPOINT0_GC;
    atUsbEndpoints[ USB_ENDPOINT_CTL ].DeviceDescBank[ 1 ].PCKSIZE.bit.SIZE = USB_ENDPOINT0_GC;

    // set the addresses
    atUsbEndpoints[ USB_ENDPOINT_CTL ].DeviceDescBank[ 0 ].ADDR.reg = ( U32 )&anUsbEpBufOut[ 0 ];
    atUsbEndpoints[ USB_ENDPOINT_CTL ].DeviceDescBank[ 1 ].ADDR.reg = ( U32 )&anUsbEpBufIn[ 0 ];

    // set the multi packet size to 8 and byte count to 0
    atUsbEndpoints[ USB_ENDPOINT_CTL ].DeviceDescBank[ 0 ].PCKSIZE.bit.MULTI_PACKET_SIZE = 8;
    atUsbEndpoints[ USB_ENDPOINT_CTL ].DeviceDescBank[ 0 ].PCKSIZE.bit.BYTE_COUNT = 0;

    // clear BK0RDY
    USB->DEVICE.DeviceEndpoint[ USB_ENDPOINT_CTL ].EPSTATUSCLR.reg = USB_DEVICE_EPSTATUSCLR_BK0RDY;

    // reset configuration
    nConfiguration = 0;
  }
  else
  {
    // check for enuemration
    if ( USB->DEVICE.DeviceEndpoint[ USB_ENDPOINT_CTL ].EPINTFLAG.reg & USB_DEVICE_EPINTFLAG_RXSTP )
    {
      // process the enumeration
      CdcEnumerate( );
    }
  }

  // return the current configuration
  return( nConfiguration );
}

/******************************************************************************
 * @function nConfiguration
 *
 * @brief enumeration
 *
 * This function will process the enumeration
 *
 *****************************************************************************/
static void CdcEnumerate( void )
{
  VU8   nRequestType, nRequest, nDir;
  VU16  wValue, wIndex, wLength, wStatus;

  // clear the receive setup flag
  USB->DEVICE.DeviceEndpoint[ USB_ENDPOINT_CTL ].EPINTFLAG.reg = USB_DEVICE_EPINTFLAG_RXSTP;

  // read the request parameters
  nRequestType = anUsbEpBufOut[ LCLBUF_IDX_CTL ][ 0 ];
  nRequest = anUsbEpBufOut[ LCLBUF_IDX_CTL ][ 1 ];
  wValue = MAKEU16( anUsbEpBufOut[ LCLBUF_IDX_CTL ][ 3 ], anUsbEpBufOut[ LCLBUF_IDX_CTL ][ 2 ] );
  wIndex = MAKEU16( anUsbEpBufOut[ LCLBUF_IDX_CTL ][ 5 ], anUsbEpBufOut[ LCLBUF_IDX_CTL ][ 4 ] );
  wLength = MAKEU16( anUsbEpBufOut[ LCLBUF_IDX_CTL ][ 7 ], anUsbEpBufOut[ LCLBUF_IDX_CTL ][ 6 ] );

  // clear the bank ready flag on control out
  USB->DEVICE.DeviceEndpoint[ USB_ENDPOINT_CTL ].EPSTATUSCLR.reg = USB_DEVICE_EPSTATUSCLR_BK0RDY;

  // determine which requests
  switch( MAKEU16( nRequest, nRequestType ))
  {
    case STD_GET_DESCRIPTOR :
      // determine which one
      switch( wValue )
      {
        case 0x100 :
          LclSendData( USB_ENDPOINT_CTL, ( PU8 )&g_tUsbDevDescriptor, MIN( USBDEVDESCRIPTOR_SIZE, wLength ));
          break;

        case 0x200 :
          LclSendData( USB_ENDPOINT_CTL, ( PU8 )&g_tUsbConfiguration, MIN( USBCONFIGURATION_SIZE, wLength ));
          break;

        default :
          LclSendStall( TRUE );
          break;
      }
      break;

    case STD_SET_ADDRESS :
      // send a ZLP/set the address
      LclSendZlp( );
      USB->DEVICE.DADD.reg = USB_DEVICE_DADD_ADDEN | wValue;
      break;

    case STD_SET_CONFIGURATION :
      // store the current configuration
      nConfiguration = ( U8 )wValue;

      // send a ZLP
      LclSendZlp( );

      // configure bulk out endpoint cdc data interface/set pkt size/set the data buffer
      atUsbEndpoints[ USB_ENDPOINT_OUT ].DeviceDescBank[ 0 ].PCKSIZE.bit.SIZE = USB_ENDPOINT0_GC;
      atUsbEndpoints[ USB_ENDPOINT_OUT ].DeviceDescBank[ 0 ].ADDR.reg = ( U32 )&anUsbEpBufOut[ LCLBUF_IDX_BLK ];
      USB->DEVICE.DeviceEndpoint[ USB_ENDPOINT_OUT ].EPCFG.reg = USB_DEVICE_EPCFG_EPTYPE0( USB_EPTYPE_CNFG_BULK );
      USB->DEVICE.DeviceEndpoint[ USB_ENDPOINT_OUT ].EPSTATUSSET.reg = USB_DEVICE_EPSTATUSSET_BK0RDY;

      // configure bulk in endpoint cdc data interface/set pkt size/set the data buffer
      atUsbEndpoints[ USB_ENDPOINT_IN ].DeviceDescBank[ 1 ].PCKSIZE.bit.SIZE = USB_ENDPOINT0_GC;
      atUsbEndpoints[ USB_ENDPOINT_IN ].DeviceDescBank[ 1 ].ADDR.reg = ( U32 )&anUsbEpBufIn[ LCLBUF_IDX_BLK ];
      USB->DEVICE.DeviceEndpoint[ USB_ENDPOINT_IN ].EPCFG.reg = USB_DEVICE_EPCFG_EPTYPE1( USB_EPTYPE_CNFG_BULK );
      USB->DEVICE.DeviceEndpoint[ USB_ENDPOINT_IN ].EPSTATUSCLR.reg = USB_DEVICE_EPSTATUSCLR_BK1RDY;

      // configure control in endpoint cdc data interface/set pkt size/set the data buffer
      USB->DEVICE.DeviceEndpoint[ USB_ENDPOINT_COMM ].EPCFG.reg = USB_DEVICE_EPCFG_EPTYPE1( USB_EPTYPE_CNFG_INTERRUPT );
      atUsbEndpoints[ USB_ENDPOINT_COMM ].DeviceDescBank[ 0 ].PCKSIZE.bit.SIZE = 0;
      USB->DEVICE.DeviceEndpoint[ USB_ENDPOINT_COMM ].EPSTATUSCLR.reg = USB_DEVICE_EPSTATUSCLR_BK1RDY;
      break;

    case STD_GET_CONFIGURATION :
      // return the current configuration
      LclSendData( USB_ENDPOINT_CTL, &nConfiguration, sizeof( nConfiguration ));
      break;

    case STD_GET_STATUS_ZERO :
    case STD_GET_STATUS_INTERFACE :
      wStatus = 0;
      LclSendData( USB_ENDPOINT_CTL, ( PU8 )&wStatus, sizeof( wStatus ));
      break;

    case STD_GET_STATUS_ENDPOINT :
      wStatus = 0;
      nDir = wIndex & 0x80;
      wIndex &= 0x0F;

      // if valid endpoint
      if ( wIndex <= 3 )
      {
        // get the direction
        if ( nDir != 0 )
        {
          wStatus = ( USB->DEVICE.DeviceEndpoint[ wIndex ].EPSTATUS.reg & USB_DEVICE_EPSTATUSSET_STALLRQ1 ) ? TRUE : FALSE;
        }
        else
        {
          wStatus = ( USB->DEVICE.DeviceEndpoint[ wIndex ].EPSTATUS.reg & USB_DEVICE_EPSTATUSSET_STALLRQ0 ) ? TRUE : FALSE;
        }

        // send the current status
        LclSendData( USB_ENDPOINT_CTL, ( PU8 )&wStatus, sizeof( wStatus ));
      }
      else
      {
        // send a stall
        LclSendStall( TRUE );
      }
      break;

    case STD_SET_FEATURE_ZERO :
      // stall the request
      LclSendStall( TRUE );
      break;

    case STD_SET_FEATURE_INTERFACE :
      // send a zero length packet
      LclSendZlp( );
      break;

    case STD_SET_FEATURE_ENDPOINT :
      nDir = wIndex & 0x80;
      wIndex &= 0x0F;

      // if valid endpoint
      if (( wValue == 0 ) && ( wIndex != 0 ) && ( wIndex <= 3 ))
      {
        // set the stall for the endpoint
        if ( nDir != 0 )
        {
          USB->DEVICE.DeviceEndpoint[ wIndex ].EPSTATUSSET.reg = USB_DEVICE_EPSTATUSSET_STALLRQ1;
        }
        else
        {
          USB->DEVICE.DeviceEndpoint[ wIndex ].EPSTATUSSET.reg = USB_DEVICE_EPSTATUSSET_STALLRQ0;
        }

        // send a zero length packet
        LclSendZlp( );
      }
      else
      {
        // stall the request
        LclSendStall( TRUE );
      }
      break;
        
    case STD_CLEAR_FEATURE_ZERO :
      // send a stall
      LclSendStall( TRUE );
      break;

    case STD_CLEAR_FEATURE_INTERFACE :
      // send a zero length packet
      LclSendZlp( );
      break;

    case STD_CLEAR_FEATURE_ENDPOINT :
      nDir = wIndex & 0x80;
      wIndex &= 0x0F;
      
      // if valid endpoint
      if (( wValue == 0 ) && ( wIndex != 0 ) && ( wIndex <= 3 ))
      {
        // set the stall for the endpoint
        if ( nDir != 0 )
        {
          // if we're in a stall
          if ( USB->DEVICE.DeviceEndpoint[ wIndex ].EPSTATUS.reg & USB_DEVICE_EPSTATUSSET_STALLRQ1 )
          {
            // remove the stall request
            USB->DEVICE.DeviceEndpoint[ wIndex ].EPSTATUSCLR.reg = USB_DEVICE_EPSTATUSCLR_STALLRQ1;

            // check for stall occurence
            if ( USB->DEVICE.DeviceEndpoint[ wIndex ].EPINTFLAG.reg & USB_DEVICE_EPINTFLAG_STALL1 )
            {
              // clear it
              USB->DEVICE.DeviceEndpoint[ wIndex ].EPINTFLAG.reg = USB_DEVICE_EPINTFLAG_STALL1;

              // reset the data toggle
              USB->DEVICE.DeviceEndpoint[ wIndex ].EPSTATUSCLR.reg = USB_DEVICE_EPSTATUSSET_DTGLIN;
            }
          }
        }
        else
        {
          // if we're in a stall
          if ( USB->DEVICE.DeviceEndpoint[ wIndex ].EPSTATUS.reg & USB_DEVICE_EPSTATUSSET_STALLRQ0 )
          {
            // remove the stall request
            USB->DEVICE.DeviceEndpoint[ wIndex ].EPSTATUSCLR.reg = USB_DEVICE_EPSTATUSCLR_STALLRQ0;

            // check for stall occurence
            if ( USB->DEVICE.DeviceEndpoint[ wIndex ].EPINTFLAG.reg & USB_DEVICE_EPINTFLAG_STALL0 )
            {
              // clear it
              USB->DEVICE.DeviceEndpoint[ wIndex ].EPINTFLAG.reg = USB_DEVICE_EPINTFLAG_STALL0;

              // reset the data toggle
              USB->DEVICE.DeviceEndpoint[ wIndex ].EPSTATUSCLR.reg = USB_DEVICE_EPSTATUSSET_DTGLOUT;
            }
          }
        }

        // send a zero length packet
        LclSendZlp( );
      }
      else
      {
        // stall the request
        LclSendStall( TRUE );
      }
      break;

    case SET_LINE_CODING :
      // send a zero length packet
      LclSendZlp( );
      break;

    case GET_LINE_CODING :
      // send the data
      LclSendData( USB_ENDPOINT_CTL, ( PU8 )&tLineCoding, MIN( USBCDCLINECODING_SIZE, wLength ));
      break;

    case SET_CONTROL_LINE_STATE :
      // set the configuration
      nCdcConfiguration = wValue;

      // send a zero length packet
      LclSendZlp( );
      break;

    default :
      // stall the request
      LclSendStall( TRUE );
      break;
  }
}

/******************************************************************************
 * @function LclSendZlp 
 *
 * @brief send a zero length packet
 *
 * This function will send a zero length packet on the control endpoint
 *
 *****************************************************************************/
static void LclSendZlp( void )
{
  // set the byte count to zero
  atUsbEndpoints[ USB_ENDPOINT_CTL ].DeviceDescBank[ 1 ].PCKSIZE.bit.BYTE_COUNT = 0;

  // clear the transfer complete flag/set bank as ready/wait for complete
  USB->DEVICE.DeviceEndpoint[ USB_ENDPOINT_CTL ].EPINTFLAG.reg |= USB_DEVICE_EPINTFLAG_TRCPT1;
  USB->DEVICE.DeviceEndpoint[ USB_ENDPOINT_CTL ].EPSTATUSSET.bit.BK1RDY = TRUE;
  while( !( USB->DEVICE.DeviceEndpoint[ USB_ENDPOINT_CTL ].EPINTFLAG.reg & USB_DEVICE_EPINTFLAG_TRCPT1 ));
}

/******************************************************************************
 * @function LclSendStall
 *
 * @brief stall the control endpoint
 *
 * This function will stall the control endpoint
 *
 * @param[in]   bDirectionIn    TRUE if direction is in
 *
 *****************************************************************************/
static void LclSendStall( BOOL bDirectionIn )
{
  // check for direction
  if ( bDirectionIn )
  {
    // stall the in direction
    USB->DEVICE.DeviceEndpoint[ USB_ENDPOINT_CTL ].EPSTATUSSET.reg = USB_DEVICE_EPSTATUSSET_STALLRQ1;
  }
  else
  {
    // stall the out direction
    USB->DEVICE.DeviceEndpoint[ USB_ENDPOINT_CTL ].EPSTATUSSET.reg = USB_DEVICE_EPSTATUSSET_STALLRQ0;
  }
}

/******************************************************************************
 * @function LclSendData 
 *
 * @brief send data on the designated endpoint
 *
 * This function will send data on the designated endpoint
 *
 * @param[in]   eEndpoint   endpoint selection
 * @param[in]   pnData      pointer to the data
 * @param[in]   wLength     length of the data
 *
 *****************************************************************************/
static void LclSendData( USBENDPOINT eEndpoint, PU8 pnData, U16 wLength )
{
  U32       uDataAddress;
  LCLBUFIDX eBufIdx;

  // set the buffer index
  eBufIdx = ( eEndpoint == USB_ENDPOINT_CTL ) ? LCLBUF_IDX_CTL : LCLBUF_IDX_BLK;

  // check for multipacket
  if ( wLength >= BIT(( atUsbEndpoints[ eEndpoint ].DeviceDescBank[ 1 ].PCKSIZE.bit.SIZE + 3 )))
  {
    // update the data address/enable auto ZLP
    uDataAddress = ( U32 )pnData;
    atUsbEndpoints[ eEndpoint ].DeviceDescBank[ 1 ].PCKSIZE.bit.AUTO_ZLP = ON;
  }
  else
  {
    // copy to local/update the data address
    memcpy( anUsbEpBufIn[ eBufIdx ], pnData, wLength );
    uDataAddress = ( U32)&anUsbEpBufIn[ eBufIdx ];
  }

  // set the buffer address for endpoint data/set byte count to length/set multi packet size to 0
  atUsbEndpoints[ eEndpoint ].DeviceDescBank[ 1 ].ADDR.reg = uDataAddress;
  atUsbEndpoints[ eEndpoint ].DeviceDescBank[ 1 ].PCKSIZE.bit.BYTE_COUNT = wLength;
  atUsbEndpoints[ eEndpoint ].DeviceDescBank[ 1 ].PCKSIZE.bit.MULTI_PACKET_SIZE = 0;

  // clear the transfer flag/set bank as ready/wait for transfer complete
  USB->DEVICE.DeviceEndpoint[ eEndpoint ].EPINTFLAG.reg |= USB_DEVICE_EPINTFLAG_TRCPT1;
  USB->DEVICE.DeviceEndpoint[ eEndpoint ].EPSTATUSSET.bit.BK1RDY = ON;
  while( !( USB->DEVICE.DeviceEndpoint[ eEndpoint ].EPINTFLAG.reg & USB_DEVICE_EPINTFLAG_TRCPT1 ));
}

/**@} EOF USBCBCHandler.c */
