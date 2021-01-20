/******************************************************************************
 * @file Usb.c
 *
 * @brief USB implementation
 *
 * This file provides the implementation for the USB hardware driver
 *
 * @copyright Copyright (c) 2012 Cyber Intergration
 * This document contains proprietary data and information of Cyber Integration 
 * LLC. It is the exclusive property of Cyber Integration, LLC and will not be 
 * disclosed in any form to any party without prior written permission of 
 * Cyber Integration, LLC. This document may not be rnEproduced or further used 
 * without the prior written permission of Cyber Integration, LLC.
 *
 * Version History
 * ======
 * $Rev: $
 * 
 *
 * \addtogroup Usb
 * @{
 *****************************************************************************/

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "USB/Usb.h"

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
    

// enumerations ---------------------------------------------------------------
/// enumerate the packet size
typedef enum _PKCTSIZE
{
  PCKT_SIZE_8 = 0,
  PCKT_SIZE_16,
  PCKT_SIZE_32,
  PCKT_SIZE_64,
  PCKT_SIZE_128,
  PCKT_SIZE_256,
  PCKT_SIZE_512,
  PCKT_SIZE_1023,
  PCKT_SIZE_MAX
} PKCTSIZE;

/// enumerate the endpoint type
typedef enum _USB_EPTYPE
{
  USB_EP_TYPE_DISABLED = 0,
  USB_EP_TYPE_CONTROL,
  USB_EP_TYPE_ISOCHRONOUS,
  USB_EP_TYPE_BULK,
  USB_EP_TYPE_INTERRUPT,
  USB_EP_TYPE_DUAL_BANK,
  USB_EPTYPE_MAX
} USB_EPTYPE;


// structures -----------------------------------------------------------------
typedef union _USBMEM
{
  UsbDeviceDescBank    tBank[ 2 ];
  struct
  {
    UsbDeviceDescBank  tOut;
    UsbDeviceDescBank  tIn;
  } tDirBanks;
} USBMEM, *PUSBMEM;
#define USBMEM_SIZE                         sizeof( USBMEM )

// global parameter declarations ----------------------------------------------

// local parameter declarations -----------------------------------------------
static USBMEM       atUsbMem[ USB_NUM_ENDPOINTS ];
static ALIGNED4 U8  anCtlInBuf[ USB_MAX_PACKET_SIZE ];
static ALIGNED4 U8  anCtlOutBuf[ USB_MAX_PACKET_SIZE ];

// local function prototypes --------------------------------------------------

// constant parameter initializations -----------------------------------------

/******************************************************************************
 * @function Usb_Initialize
 *
 * @brief initialize the device
 *
 * This function will perform any needed initialization for the USB controller
 *
 *****************************************************************************/
void Usb_Initialize( void )
{
  U8  nEp;
  
  // reset the device
  USB->DEVICE.CTRLA.bit.SWRST = 1;
  while( USB->DEVICE.SYNCBUSY.bit.SWRST );

  // set the pad calibration values
  USB->DEVICE.PADCAL.bit.TRANSN = NVM_READ_CAL( USB_TRANSN );
  USB->DEVICE.PADCAL.bit.TRANSP = NVM_READ_CAL( USB_TRANSP );
  USB->DEVICE.PADCAL.bit.TRIM   = NVM_READ_CAL( USB_TRIM );

  // clear the bank endpoint control/set the address
  memset(( PU8 )atUsbMem, 0, sizeof( atUsbMem ));
  USB->DEVICE.DESCADD.reg = ( U32 )atUsbMem;

  // set the device/runstandby/speed/attach
  USB->DEVICE.CTRLA.bit.MODE = USB_CTRLA_MODE_DEVICE_Val;
  USB->DEVICE.CTRLA.bit.RUNSTDBY = 1;
  USB->DEVICE.CTRLB.bit.SPDCONF = USB_DEVICE_CTRLB_SPDCONF_FS_Val;

  // enable the reset interrupt/enable setup interrupt on endpoint 0
  USB->DEVICE.INTENSET.reg = USB_DEVICE_INTENSET_EORST;
  USB->DEVICE.DeviceEndpoint[ 0 ].EPINTENSET.bit.RXSTP = 1;

  for ( nEp = 0; nEp < USB_NUM_ENDPOINTS; nEp++ )
  {
    Usb_ResetEndpoint( nEp, USB_ENDPOINT_IN );
    Usb_ResetEndpoint( nEp, USB_ENDPOINT_OUT );
  }

  // enable the interrupts/enable the device
  NVIC_EnableIRQ( USB_IRQn );
  USB->DEVICE.CTRLA.reg |= USB_CTRLA_ENABLE;
}

/******************************************************************************
 * @function Usb_AttachDetach
 *
 * @brief attach/detach the device
 *
 * This function will set/clear thel detach bit appropriately
 *
 * @param[in]   bDetach   TRUE to detach, FALSE to attach
 *
 *****************************************************************************/
void Usb_AttachDetach( BOOL bDetach )
{
  // set/clear the detach bit
  USB->DEVICE.CTRLB.bit.DETACH = bDetach;
}

/******************************************************************************
 * @function Usb_ResetEndpoint
 *
 * @brief reset an endpont
 *
 * This function will reset/disable a given endpoint
 *
 * @param[in]   nEp     endpoint index
 * @param[in[   nDir    endpoint direction
 *
 *****************************************************************************/
void Usb_ResetEndpoint( U8 nEp, U8 nDir )
{
  // clera  the endpont point type
  if ( USB_ENDPOINT_IN == nDir )
  {
    USB->DEVICE.DeviceEndpoint[ nEp ].EPCFG.bit.EPTYPE1 = USB_DEVICE_EPCFG_EPTYPE0( USB_EP_TYPE_DISABLED );
  }
  else
  {
    USB->DEVICE.DeviceEndpoint[ nEp ].EPCFG.bit.EPTYPE0 = USB_DEVICE_EPCFG_EPTYPE0( USB_EP_TYPE_DISABLED );
  }
}

/******************************************************************************
 * @function Usb_ConfigureEndpoint
 *
 * @brief configure an endpoint
 *
 * This function will configure an endpont
 *
 * @param[in]   nEndpoint     desired endpoint
 * @param[in]   nDir          direction
 * @param[in]   nType         endpoint type
 * @param[in]   wSize         endpoint size
 *
 *****************************************************************************/
void Usb_ConfigureEndpoint( U8 nEndpoint, U8 nType, U16 wSize )
{
  PKCTSIZE    ePktSize;
  USB_EPTYPE  eEpType;
  U8          nDir;

  // get the direction
  nDir = nEndpoint & USB_ENDPOINT_IN;

  // mask out the direction on the endpoint
  nEndpoint &= USB_ENDPOINT_INDEX_MASK;
  
  // raeset the endpoint
  Usb_ResetEndpoint( nEndpoint, nDir );

  // now determine the size
  if ( wSize <= 8 )
  {
    ePktSize = PCKT_SIZE_8;
  }
  else if ( wSize <= 16 )
  {
    ePktSize = PCKT_SIZE_16;
  }
  else if ( wSize <= 32 )
  {
    ePktSize = PCKT_SIZE_32;
  }
  else if ( wSize <= 64 )
  {
    ePktSize = PCKT_SIZE_64;
  }
  else if ( wSize <= 128 )
  {
    ePktSize = PCKT_SIZE_128;
  }
  else if ( wSize <= 256 )
  {
    ePktSize = PCKT_SIZE_256;
  }
  else if ( wSize <= 512 )
  {
    ePktSize = PCKT_SIZE_512;
  }
  else if ( wSize <= 1023 )
  {
    ePktSize = PCKT_SIZE_1023;
  }
  else
  {
    ePktSize = PCKT_SIZE_8;
  }
    
  // now set the type
  switch( nType )
  {
    case USB_ENDPNTTYPE_CONTROL :
      eEpType = USB_EP_TYPE_CONTROL;
      break;

    case USB_ENDPNTTYPE_ISOCHRONOUS :
      eEpType = USB_EP_TYPE_ISOCHRONOUS;
      break;

    case USB_ENDPNTTYPE_BULK :
      eEpType = USB_EP_TYPE_BULK;
      break;
      
    default :
      eEpType = USB_EP_TYPE_INTERRUPT;
      break;
  }

  // determine the direction
  if ( USB_ENDPOINT_IN == nDir )
  {
    USB->DEVICE.DeviceEndpoint[ nEndpoint ].EPCFG.bit.EPTYPE1 = eEpType;
    USB->DEVICE.DeviceEndpoint[ nEndpoint ].EPINTENSET.bit.TRCPT1 = 1;
    USB->DEVICE.DeviceEndpoint[ nEndpoint ].EPSTATUSCLR.bit.DTGLIN = 1;
    USB->DEVICE.DeviceEndpoint[ nEndpoint ].EPSTATUSCLR.bit.BK1RDY = 1;
    atUsbMem[ nEndpoint ].tDirBanks.tIn.PCKSIZE.bit.SIZE = ePktSize;
  }
  else
  {
    USB->DEVICE.DeviceEndpoint[ nEndpoint ].EPCFG.bit.EPTYPE0 = eEpType;
    USB->DEVICE.DeviceEndpoint[ nEndpoint ].EPINTENSET.bit.TRCPT0 = 1;
    USB->DEVICE.DeviceEndpoint[ nEndpoint ].EPSTATUSCLR.bit.DTGLOUT = 1;
    USB->DEVICE.DeviceEndpoint[ nEndpoint ].EPSTATUSSET.bit.BK0RDY = 1;
    atUsbMem[ nEndpoint ].tDirBanks.tOut.PCKSIZE.bit.SIZE = ePktSize;
  }
}

/******************************************************************************
 * @function Usb_IsEndpointConfigured
 *
 * @brief checks to see if ean endpoint is configure
 *
 * This function will return the configured status of a given endpoint
 *
 * @param[in]   nEp     endpoint index
 * @param[in[   nDir    endpoint direction
 *
 * @return      TRUE if endpoint congiured, FALSE otherwise
 *
 *****************************************************************************/
BOOL Usb_IsEndpointConfigured( U8 nEp, U8 nDir )
{
  BOOL  bConfigured;
  
  // determine the direction
  if ( USB_ENDPOINT_IN == nDir )
  {
    bConfigured = ( USB->DEVICE.DeviceEndpoint[ nEp ].EPCFG.bit.EPTYPE1 != USB_EP_TYPE_DISABLED );
  }
  else
  {
    bConfigured = ( USB->DEVICE.DeviceEndpoint[ nEp ].EPCFG.bit.EPTYPE0 != USB_EP_TYPE_DISABLED );
  }
  
  // return the status
  return( bConfigured );
}

/******************************************************************************
 * @function Usb_EndpointGetStatus
 *
 * @brief get the endpoint status
 *
 * This function will return the endpoint status
 *
 * @param[in]   nEp     endpoint index
 * @param[in[   nDir    endpoint direction
 *
 * @return      endpoint status
 *
 *****************************************************************************/
BOOL Usb_EndpointGetStatus( U8 nEp, U8 nDir )
{
  BOOL bStatus;
  
  // determine the direction
  if ( USB_ENDPOINT_IN == nDir )
  {
    bStatus = USB->DEVICE.DeviceEndpoint[ nEp ].EPSTATUS.bit.STALLRQ1;
  }
  else
  {
    bStatus = USB->DEVICE.DeviceEndpoint[ nEp ].EPSTATUS.bit.STALLRQ0;
  }
  
  // return the status
  return( bStatus );
}

/******************************************************************************
 * @function Usb_EndpointSetFeature
 *
 * @brief set an endpoint feature
 *
 * This function will set the designated endpoint's feature
 *
 * @param[in]   nEp     endpoint index
 * @param[in[   nDir    endpoint direction
 *
 *****************************************************************************/
void Usb_EndpointSetFeature( U8 nEp, U8 nDir )
{
  // determine the direction
  if ( USB_ENDPOINT_IN == nDir )
  {
    USB->DEVICE.DeviceEndpoint[ nEp ].EPSTATUSSET.bit.STALLRQ1 = ON;
  }
  else
  {
    USB->DEVICE.DeviceEndpoint[ nEp ].EPSTATUSSET.bit.STALLRQ0 = ON;
  }
}

/******************************************************************************
 * @function Usb_EndpointClearFeature
 *
 * @brief clear an endpoint feature
 *
 * This function will clear an endpoint feature
 *
 * @param[in]   nEp     endpoint index
 * @param[in[   nDir    endpoint direction
 *
 *****************************************************************************/
void Usb_EndpointClearFeature( U8 nEp, U8 nDir )
{
  // determine the direction
  if ( USB_ENDPOINT_IN == nDir )
  {
    // if stalled
    if ( USB->DEVICE.DeviceEndpoint[ nEp ].EPSTATUS.bit.STALLRQ1 )
    {
      // un-stall it
      USB->DEVICE.DeviceEndpoint[ nEp ].EPSTATUSCLR.bit.STALLRQ1 = ON;
      
      // if the stall req int flag is set
      if ( USB->DEVICE.DeviceEndpoint[ nEp ].EPINTFLAG.bit.STALL1 )
      {
        // clear it/clear data toggle
        USB->DEVICE.DeviceEndpoint[ nEp ].EPINTFLAG.reg = USB_DEVICE_EPINTFLAG_STALL1;
        USB->DEVICE.DeviceEndpoint[ nEp ].EPSTATUSCLR.bit.DTGLIN = ON;
      }
    }
  }
  else
  {
    // if stalled
    if ( USB->DEVICE.DeviceEndpoint[ nEp ].EPSTATUS.bit.STALLRQ0 )
    {
      // un-stall it
      USB->DEVICE.DeviceEndpoint[ nEp ].EPSTATUSCLR.bit.STALLRQ0 = ON;
      
      // if the stall req int flag is set
      if ( USB->DEVICE.DeviceEndpoint[ nEp ].EPINTFLAG.bit.STALL0 )
      {
        // clear it/clear data toggle
        USB->DEVICE.DeviceEndpoint[ nEp ].EPINTFLAG.reg = USB_DEVICE_EPINTFLAG_STALL0;
        USB->DEVICE.DeviceEndpoint[ nEp ].EPSTATUSCLR.bit.DTGLOUT = ON;
      }
    }
  }
}

/******************************************************************************
 * @function Usb_SetAddress
 *
 * @brief set the address
 *
 * This function will set the address
 *
 * @param[in]   nAddress    address to set
 *
 *****************************************************************************/
void Usb_SetAddress( U8 nAddress )
{
  // set the address
  USB->DEVICE.DADD.reg = USB_DEVICE_DADD_ADDEN | USB_DEVICE_DADD_DADD( nAddress );
}

/******************************************************************************
 * @function Usb_Send
 *
 * @brief send data
 *
 * This function will send the data
 *
 * @param[in]   nEp     endpoint selection
 * @param[in]   pnData  pointer to the data
 * @param[in]   wLength length of data to send
 *
 *****************************************************************************/
void Usb_Send( U8 nEp, PU8 pnData, U16 wLength )
{
  // set the data address/size/reset byte count
  atUsbMem[ nEp ].tDirBanks.tIn.ADDR.reg = ( U32 )pnData;
  atUsbMem[ nEp ].tDirBanks.tIn.PCKSIZE.bit.BYTE_COUNT = wLength;
  atUsbMem[ nEp ].tDirBanks.tIn.PCKSIZE.bit.MULTI_PACKET_SIZE = 0;
  
  // set the ready bit
  USB->DEVICE.DeviceEndpoint[ nEp ].EPSTATUSSET.bit.BK1RDY = ON;
}

/******************************************************************************
 * @function Usb_Recv
 *
 * @brief receive some data
 *
 * This function will queue up for a data receive
 *
 * @param[in]   nEp     endpoint selection
 * @param[in]   pnData  pointer to the data
 * @param[in]   wLength length of data to receive
 *
 *****************************************************************************/
void Usb_Recv( U8 nEp, PU8 pnData, U16 wLength )
{
  // set the address/size/clear byte count
  atUsbMem[ nEp ].tDirBanks.tOut.ADDR.reg = ( U32 )pnData;
  atUsbMem[ nEp ].tDirBanks.tOut.PCKSIZE.bit.BYTE_COUNT = 0;
  atUsbMem[ nEp ].tDirBanks.tOut.PCKSIZE.bit.MULTI_PACKET_SIZE = wLength;
  
  // set the ready bit
  USB->DEVICE.DeviceEndpoint[ nEp ].EPSTATUSCLR.bit.BK0RDY = ON;
}

/******************************************************************************
 * @function Usb_ControlSendZlp
 *
 * @brief send a zero length packet on the control endpoint
 *
 * This function will send a ZLP ( zero length packet on the control endpoint
 *
 *****************************************************************************/
void Usb_ControlSendZlp( void )
{
  // clear the byte count
  atUsbMem[ 0 ].tDirBanks.tIn.PCKSIZE.bit.BYTE_COUNT = 0;
  
  // reset the complete flag/send it
  USB->DEVICE.DeviceEndpoint[ 0 ].EPINTFLAG.reg = USB_DEVICE_EPINTFLAG_TRCPT1;
  USB->DEVICE.DeviceEndpoint[ 0 ].EPSTATUSSET.bit.BK1RDY = ON;
  
  // wait till done
  while( !USB->DEVICE.DeviceEndpoint[ 0 ].EPINTFLAG.bit.TRCPT1 );
}

/******************************************************************************
 * @function Usb_ControlStall
 *
 * @brief stall the control port
 *
 * This function will stall the control port
 *
 *****************************************************************************/
void Usb_ControlStall( void )
{
  // set the stall bit
  USB->DEVICE.DeviceEndpoint[ 0 ].EPSTATUSSET.bit.STALLRQ1 = ON;
}

/******************************************************************************
 * @function Usb_ControlSend
 *
 * @brief send data on the control endpoint
 *
 * This function will send data on the control endpoint
 *
 * @param[in]   pnData    pointer to the data
 * @param[in]   wLength   length of data
 *
 *****************************************************************************/
void Usb_ControlSend( PU8 pnData, U16 wLength )
{
  // check for small payload
  if ( wLength <= USB_MAX_PACKET_SIZE )
  {
    // copy to an aligned buffer/set the address
    memcpy( anCtlInBuf, pnData, wLength );
    atUsbMem[ 0 ].tDirBanks.tIn.ADDR.reg = ( U32 )anCtlInBuf;
  }
  else
  {
    // large payouds, just set the addtess
    atUsbMem[ 0 ].tDirBanks.tIn.ADDR.reg = ( U32 )pnData;
  }
  
  // set the byte count/clear multi packet
  atUsbMem[ 0 ].tDirBanks.tIn.PCKSIZE.bit.BYTE_COUNT = wLength;
  atUsbMem[ 0 ].tDirBanks.tIn.PCKSIZE.bit.MULTI_PACKET_SIZE = 0;
  
  // reset the complete flag/send it
  USB->DEVICE.DeviceEndpoint[ 0 ].EPINTFLAG.reg = USB_DEVICE_EPINTFLAG_TRCPT1;
  USB->DEVICE.DeviceEndpoint[ 0 ].EPSTATUSSET.bit.BK1RDY = ON;
  
  // wait till done
  while( !USB->DEVICE.DeviceEndpoint[ 0 ].EPINTFLAG.bit.TRCPT1 );
}

/******************************************************************************
 * @function USB_Handler
 *
 * @brief USB interrupt handler
 *
 * This function will process any USB intterrupts
 *
 *****************************************************************************/
void USB_Handler( void )
{
  U16 wEpInt;
  U8  nEpFlags, nEp, nIntMask;
  
  // check for a End of reset
  if ( USB->DEVICE.INTFLAG.bit.EORST )
  {
    // clear the interrupt/reset the address
    USB->DEVICE.INTFLAG.reg = USB_DEVICE_INTFLAG_EORST;
    USB->DEVICE.DADD.reg = USB_DEVICE_DADD_ADDEN;
    
    // reset the endpoints
    for ( nEp = 0; nEp < USB_NUM_ENDPOINTS; nEp++ )
    {
      Usb_ResetEndpoint( nEp, USB_ENDPOINT_IN );
      Usb_ResetEndpoint( nEp, USB_ENDPOINT_OUT );
    }
    
    // enable control endpoints/reset the readys
    USB->DEVICE.DeviceEndpoint[ 0 ].EPCFG.reg = USB_DEVICE_EPCFG_EPTYPE0( USB_EP_TYPE_CONTROL )  | USB_DEVICE_EPCFG_EPTYPE1( USB_EP_TYPE_CONTROL );
    USB->DEVICE.DeviceEndpoint[ 0 ].EPSTATUSSET.bit.BK0RDY = ON;
    USB->DEVICE.DeviceEndpoint[ 0 ].EPSTATUSCLR.bit.BK1RDY = ON;
    
    // set the memory control for the control in endpoint
    atUsbMem[ 0 ].tDirBanks.tIn.ADDR.reg = ( U32 )anCtlInBuf;
    atUsbMem[ 0 ].tDirBanks.tIn.PCKSIZE.bit.SIZE = PCKT_SIZE_64;
    atUsbMem[ 0 ].tDirBanks.tIn.PCKSIZE.bit.BYTE_COUNT = 0;
    atUsbMem[ 0 ].tDirBanks.tIn.PCKSIZE.bit.MULTI_PACKET_SIZE = 0;
    
    // set the memory control for the control out endpoint
    atUsbMem[ 0 ].tDirBanks.tOut.ADDR.reg = ( U32 )anCtlOutBuf;
    atUsbMem[ 0 ].tDirBanks.tOut.PCKSIZE.bit.SIZE = PCKT_SIZE_64;
    atUsbMem[ 0 ].tDirBanks.tOut.PCKSIZE.bit.BYTE_COUNT = 0;
    atUsbMem[ 0 ].tDirBanks.tOut.PCKSIZE.bit.MULTI_PACKET_SIZE = 8;
    
    // clear the out ready/enable RXSTP interrupt
    USB->DEVICE.DeviceEndpoint[ 0 ].EPSTATUSCLR.bit.BK0RDY = ON;
    USB->DEVICE.DeviceEndpoint[ 0 ].EPINTENSET.bit.RXSTP = ON;
  }
  
  // now check for receive setup interrupt
  if ( USB->DEVICE.DeviceEndpoint[ 0 ].EPINTFLAG.bit.RXSTP )
  {
    // reset the interrupt/clear BK0RDY
    USB->DEVICE.DeviceEndpoint[ 0 ].EPINTFLAG.reg = USB_DEVICE_EPINTFLAG_RXSTP;
    USB->DEVICE.DeviceEndpoint[ 0 ].EPSTATUSCLR.bit.BK0RDY = ON;
    
    // handle the standard request
    Usb_HandleStandardRequest( anCtlOutBuf ); 
  }
  
  // get the endpoint summary 
  wEpInt = USB->DEVICE.EPINTSMRY.reg;
  
  // now process it
  for ( nEp = 0; ( nEp < USB_NUM_ENDPOINTS ) && ( wEpInt != 0 ); nEp++ )
  {
    // check for interrupt set intterup set
    nIntMask = BIT( nEp );
    if (( nIntMask & wEpInt ) != 0 )
    {
      // clear the interrupt
      wEpInt &= ~nIntMask;
      
      // get the endpoint flags
      nEpFlags = USB->DEVICE.DeviceEndpoint[ nEp ].EPINTFLAG.reg;
      
      // check for a transfer complete out
      if ( nEpFlags & USB_DEVICE_EPINTFLAG_TRCPT0 )
      {
        // clear the interrupt/set the BK0RDY
        USB->DEVICE.DeviceEndpoint[ nEp ].EPINTFLAG.reg = USB_DEVICE_EPINTFLAG_TRCPT0;
        USB->DEVICE.DeviceEndpoint[ nEp ].EPSTATUSSET.bit.BK0RDY = ON;
        
        // process the callback
        Usb_ProcessRecvCallback( nEp, atUsbMem[ nEp ].tDirBanks.tOut.PCKSIZE.bit.BYTE_COUNT );
      }

      // check for a transfer complete in
      if ( nEpFlags & USB_DEVICE_EPINTFLAG_TRCPT1 )
      {
        // clear the interrupt/set the BK1RDY
        USB->DEVICE.DeviceEndpoint[ nEp ].EPINTFLAG.reg = USB_DEVICE_EPINTFLAG_TRCPT1;
        USB->DEVICE.DeviceEndpoint[ nEp ].EPSTATUSSET.bit.BK1RDY = OFF;
        
        // process the callback
        Usb_ProcessSendCallback( nEp );
      }
    }
  }
}

/**@} EOF Usb.c */
