/******************************************************************************
 * @file 
 *
 * @brief 
 *
 * This file 
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
 * \addtogroup 
 * @{
 *****************************************************************************/

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "Usb/Usb.h"

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------
#define NVM_USB_PAD_TRANSN_POS              ( 45 )
#define NVM_USB_PAD_TRANSN_SIZE             ( 5 )
#define NVM_USB_PAD_TRANSP_POS              ( 50 )
#define NVM_USB_PAD_TRANSP_SIZE             ( 5 )
#define NVM_USB_PAD_TRIM_POS                ( 55 )
#define NVM_USB_PAD_TRIM_SIZE               ( 3 )

/// define the endpoint size
#define ENDPOINT_MASK                       ( 0x3F )

#define USB_EP_size_to_gc(x)  ((x <= 8   )?0:\
                               (x <= 16  )?1:\
                               (x <= 32  )?2:\
                               (x <= 64  )?3:\
                               (x <= 128 )?4:\
                               (x <= 256 )?5:\
                               (x <= 512 )?6:\
                                           7)

/// define the endpoint mask
#if ( USB_ENDPOINT0_SIZE <= 8 )
#define USB_ENDPOINT0_GC                    ( 0 )
#elif ( USB_ENDPOINT0_SIZE <= 16 )
#define USB_ENDPOINT0_GC                    ( 1 )
#elif ( USB_ENDPOINT0_SIZE <= 32 )
#define USB_ENDPOINT0_GC                    ( 2 )
#elif ( USB_ENDPOINT0_SIZE <= 64 )
#define USB_ENDPOINT0_GC                    ( 3 )
#elif ( USB_ENDPOINT0_SIZE <= 128 )
#define USB_ENDPOINT0_GC                    ( 4 )
#elif ( USB_ENDPOINT0_SIZE <= 256 )
#define USB_ENDPOINT0_GC                    ( 5 )
#elif ( USB_ENDPOINT0_SIZE <= 512 )
#define USB_ENDPOINT0_GC                    ( 6 )
#else
#define USB_ENDPOINT0_GC                    ( 7 )
#endif

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations ----------------------------------------------
U8  ALIGNED4 g_anUsbEp0BufIn[ USB_ENDPOINT0_SIZE ];
U8  ALIGNED4 g_anUsbEp0BufOut[ USB_ENDPOINT0_SIZE ];

// local parameter declarations -----------------------------------------------
static  UsbDeviceDescriptor atUsbEndpoints[ USB_NUMBER_ENDPOINTS ];

// local function prototypes --------------------------------------------------
// constant parameter initializations -----------------------------------------

/******************************************************************************
 * @function Usb_Initialize
 *
 * @brief USB initialization
 *
 * This function will initialize the USB controller
 *
 *****************************************************************************/
void Usb_Initialize( void )
{
  U32 uTransN, uTransP, uTrim;

  // call the local initialization
  Usb_LocalInitialize( );

  // Reset the device/wait for sync
  USB->DEVICE.CTRLA.reg = USB_CTRLA_SWRST;
  while ( USB->DEVICE.SYNCBUSY.bit.SWRST );

  // enable it
  USB->DEVICE.CTRLA.reg = USB_CTRLA_ENABLE | USB_CTRLA_MODE_DEVICE;
  while ( USB->DEVICE.SYNCBUSY.bit.ENABLE );

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
  USB->DEVICE.DESCADD.reg = ( U32 )( & atUsbEndpoints[ 0 ] );
  USB->DEVICE.INTENSET.reg = USB_DEVICE_INTENSET_EORST;

  // cause a reset
  Usb_Reset();
}

/******************************************************************************
 * @function Usb_Attach
 *
 * @brief attach the device
 *
 * This function will cause an attachment to the bus
 *
 *****************************************************************************/
void Usb_Attach( void )
{
  // enable the interrupt/clear the detach bit
  NVIC_EnableIRQ( USB_IRQn );
  USB->DEVICE.CTRLB.bit.DETACH = 0;
}

/******************************************************************************
 * @function Usb_Detach
 *
 * @brief detach from bus
 *
 * This function will cause a detachment from the bus
 *
 *****************************************************************************/
void Usb_Detach( void )
{
  // set the detach bit/disable interrupt
  USB->DEVICE.CTRLB.bit.DETACH = 1;
  NVIC_DisableIRQ( USB_IRQn );
}

/******************************************************************************
 * @function Usb_Reset
 *
 * @brief reset the USB
 *
 * This function will reset the endpoint and reset the controller
 *
 *****************************************************************************/
void Usb_Reset( void )
{
  // reset the control endpoint
  atUsbEndpoints[ 0 ].DeviceDescBank[ 0 ].ADDR.reg = ( U32 )&g_anUsbEp0BufOut;
  atUsbEndpoints[ 0 ].DeviceDescBank[ 0 ].PCKSIZE.bit.SIZE = USB_ENDPOINT0_GC;
  atUsbEndpoints[ 0 ].DeviceDescBank[ 1 ].ADDR.reg = ( U32 )&g_anUsbEp0BufIn;
  atUsbEndpoints[ 0 ].DeviceDescBank[ 1 ].PCKSIZE.bit.SIZE = USB_ENDPOINT0_GC;
  atUsbEndpoints[ 0 ].DeviceDescBank[ 1 ].PCKSIZE.bit.AUTO_ZLP = 1;
  USB->DEVICE.DeviceEndpoint[ 0 ].EPINTENSET.reg = USB_DEVICE_EPINTENSET_RXSTP;
//  USB->DEVICE.DeviceEndpoint[ 0 ].EPCFG.reg  = USB_DEVICE_EPCFG_EPTYPE0( USB_EPTYPE_CONTROL ) | USB_DEVICE_EPCFG_EPTYPE1( USB_EPTYPE_CONTROL );
  USB->DEVICE.DeviceEndpoint[ 0 ].EPCFG.reg  = USB_DEVICE_EPCFG_EPTYPE0( 1 ) | USB_DEVICE_EPCFG_EPTYPE1( 1 );
}


/******************************************************************************
 * @function Usb_SetAddress
 *
 * @brief set the device address
 *
 * This function will set the device address in the USB controller
 *
 * @param[in]   nAddr   addres to set
 *
 *****************************************************************************/
void Usb_SetAddress( U8 nAddr)
{
  // set the address
  USB->DEVICE.DADD.reg = USB_DEVICE_DADD_ADDEN | nAddr;
}

/******************************************************************************
 * @function Usb_EnableEp
 *
 * @brief enable the designated endpoint
 *
 * This function will set the type, size and enable an endoint
 *
 * @param[in]   nEp       endpoint
 * @param[in]   eType     endpoint type
 * @param[in]   nPktSize  size of the packet
 *
 *****************************************************************************/
void Usb_EnableEp( U8 nEp, USBEPTYPE eType, U8 nPktSize )
{
  U8  nEpNum;
  
  // get the endpoint number
  nEpNum = nEp & ENDPOINT_MASK;
  
  // check for direction
  if ( nEp & USB_DIRECTION_IN )
  {
    // 
    atUsbEndpoints[ nEpNum ].DeviceDescBank[ 1 ].PCKSIZE.bit.SIZE = USB_EP_size_to_gc( nPktSize );
    USB->DEVICE.DeviceEndpoint[ nEpNum ].EPCFG.bit.EPTYPE1 = eType + 1;
    USB->DEVICE.DeviceEndpoint[ nEpNum ].EPSTATUSCLR.reg = USB_DEVICE_EPSTATUS_BK1RDY | USB_DEVICE_EPSTATUS_STALLRQ( 0x2 ) | USB_DEVICE_EPSTATUS_DTGLIN;
  }
  else
  {
    atUsbEndpoints[ nEpNum ].DeviceDescBank[ 1 ].PCKSIZE.bit.SIZE = USB_EP_size_to_gc( nPktSize );
    USB->DEVICE.DeviceEndpoint[ nEpNum ].EPCFG.bit.EPTYPE0 = eType + 1;
    USB->DEVICE.DeviceEndpoint[ nEpNum ].EPSTATUSSET.reg = USB_DEVICE_EPSTATUS_BK0RDY;
    USB->DEVICE.DeviceEndpoint[ nEpNum ].EPSTATUSCLR.reg = USB_DEVICE_EPSTATUS_STALLRQ( 0x1 ) | USB_DEVICE_EPSTATUS_DTGLOUT;
  }
}

/******************************************************************************
 * @function Usb_DisableEp
 *
 * @brief disable endpoint
 *
 * This function will disable the designated endoint
 *
 * @param[in]   nEp     endpoint
 *
 *****************************************************************************/
void Usb_DisableEp( U8 nEp )
{
  U8  nEpNum;
  
  // get the endpoint number
  nEpNum = nEp & ENDPOINT_MASK;
  
  // check for direction
  if ( nEp & USB_DIRECTION_IN )
  {
    USB->DEVICE.DeviceEndpoint[ nEpNum ].EPSTATUSCLR.reg = USB_DEVICE_EPSTATUS_BK1RDY;
    USB->DEVICE.DeviceEndpoint[ nEpNum ].EPCFG.bit.EPTYPE1 = 0;
  }
  else
  {
    USB->DEVICE.DeviceEndpoint[ nEpNum ].EPSTATUSSET.reg = USB_DEVICE_EPSTATUS_BK0RDY;
    USB->DEVICE.DeviceEndpoint[ nEpNum ].EPCFG.bit.EPTYPE0 = 0;
  }
}

/******************************************************************************
 * @function Usb_ResetEp
 *
 * @brief reset and endpoint
 *
 * This function will reset the given endpoint
 *
 * @param[in]   nEp     endpoint
 *
 *****************************************************************************/
void Usb_ResetEp( U8 nEp )
{
  U8  nEpNum;
  
  // get the endpoint number
  nEpNum = nEp & ENDPOINT_MASK;
  
  if ( nEp & USB_DIRECTION_IN )
  {
    USB->DEVICE.DeviceEndpoint[ nEpNum ].EPSTATUSCLR.reg = USB_DEVICE_EPSTATUS_BK1RDY;
  }
  else
  {
    USB->DEVICE.DeviceEndpoint[ nEpNum ].EPSTATUSSET.reg = USB_DEVICE_EPSTATUS_BK0RDY;
  }
}

/******************************************************************************
 * @function Usb_EpReady
 *
 * @brief test for endpoint ready
 *
 * This function will return the ready uStatus
 *
 * @param[in]   nEp     endpoint
 *
 * @return      TRUE if ready, FALSE if not
 *
 *****************************************************************************/
BOOL Usb_EpReady( U8 nEp )
{
  BOOL  bReady;
  U8    nEpNum;
  
  // get the endpoint number
  nEpNum = nEp & ENDPOINT_MASK;
  
  // check for direction
  if ( nEp & USB_DIRECTION_IN )
  {
    bReady = !(USB->DEVICE.DeviceEndpoint[ nEpNum ].EPSTATUS.bit.BK1RDY || Usb_EpPending( nEp ));
  }
  else
  {
    bReady = !(USB->DEVICE.DeviceEndpoint[ nEpNum ].EPSTATUS.bit.BK0RDY || Usb_EpPending( nEp ));
  }
  
  // return the ready stattus
  return( bReady );
}

/******************************************************************************
 * @function Usb_EpPending
 *
 * @brief test for endoint pending
 *
 * This function will return the endpoint pending uStatus
 *
 * @param[in]   nEp     endpoint
 *
 * @return      TRUE if pending, FALSE if not
 *
 *****************************************************************************/
BOOL Usb_EpPending( U8 nEp )
{
  BOOL  bPending;
  U8    nEpNum;
  
  // get the endpoint number
  nEpNum = nEp & ENDPOINT_MASK;
  
  // check for direction
  if ( nEp & USB_DIRECTION_IN )
  {
    bPending = USB->DEVICE.DeviceEndpoint[ nEpNum ].EPINTFLAG.bit.TRCPT1;
  }
  else
  {
    bPending = USB->DEVICE.DeviceEndpoint[ nEpNum ].EPINTFLAG.bit.TRCPT0;
  }
  
  // reutrn the pending uStatus
  return( bPending );
}

/******************************************************************************
 * @function Usb_WaitForReady
 *
 * @brief wait for ready
 *
 * This function will wait for an endoint to become ready
 *
 * @param[in]   nEp     endpoint
 *
 *****************************************************************************/
void Usb_WaitForReady( U8 nEp )
{
  // loop here
  while( !Usb_EpReady( nEp ));
}

/******************************************************************************
 * @function Usb_WaitForPending
 *
 * @brief wait for pending
 *
 * This function will wait for an endoint to complete
 *
 * @param[in]   nEp   endont
 *
 *****************************************************************************/
void Usb_WaitForPending( U8 nEp )
{
  // loop here
  while( !Usb_EpPending( nEp ));
}

/******************************************************************************
 * @function Usb_StartOut
 *
 * @brief start an out endpoint
 *
 * This function will set up an out endpoint for data transmission and start the
 * transfer
 *
 * @param[in]   nEp     endpoint number
 * @param[in]   pnData  pointer to data
 * @param[in]   nLen    length of the data
 *
 * @return      0
 *
 *****************************************************************************/
U8 Usb_StartOut( U8 nEp, PU8 pnData, U8 nLen )
{
  atUsbEndpoints[ nEp ].DeviceDescBank[ 0 ].PCKSIZE.bit.MULTI_PACKET_SIZE = nLen;
  atUsbEndpoints[ nEp ].DeviceDescBank[ 0 ].PCKSIZE.bit.BYTE_COUNT = 0;
  atUsbEndpoints[ nEp ].DeviceDescBank[ 0 ].ADDR.reg = ( U32 )pnData;
  USB->DEVICE.DeviceEndpoint[ nEp ].EPINTFLAG.reg = USB_DEVICE_EPINTFLAG_TRCPT0 | USB_DEVICE_EPINTFLAG_TRFAIL0;
  USB->DEVICE.DeviceEndpoint[ nEp ].EPINTENSET.reg = USB_DEVICE_EPINTENSET_TRCPT0;
  USB->DEVICE.DeviceEndpoint[ nEp ].EPSTATUSCLR.reg = USB_DEVICE_EPSTATUS_BK0RDY;
  
  // return 0
  return( 0 );
}

/******************************************************************************
 * @function Usb_EpStartIn
 *
 * @brief start an in inepoint
 *
 * This function will set up an in endpoint for data reception and start the
 * reception
 *
 * @param[in]   nEp     endpoint number
 * @param[in]   pnData  pointer to data
 * @param[in]   nSize   size of the buffer
 * @param[in]   bZlp    allow zero packet 
 *
 * @return      0
 *
 *****************************************************************************/
U8 Usb_EpStartIn( U8 nEp, PU8 pnData, U8 nSize, BOOL bZlp )
{
  // clear the direction bit
  nEp &= ENDPOINT_MASK;
  
  atUsbEndpoints[ nEp ].DeviceDescBank[ 1 ].PCKSIZE.bit.AUTO_ZLP = bZlp;
  atUsbEndpoints[ nEp ].DeviceDescBank[ 1 ].PCKSIZE.bit.MULTI_PACKET_SIZE = 0;
  atUsbEndpoints[ nEp ].DeviceDescBank[ 1 ].PCKSIZE.bit.BYTE_COUNT = nSize;
  atUsbEndpoints[ nEp ].DeviceDescBank[ 1 ].ADDR.reg = ( U32 )pnData;
  USB->DEVICE.DeviceEndpoint[ nEp ].EPINTFLAG.reg = USB_DEVICE_EPINTFLAG_TRCPT1 | USB_DEVICE_EPINTFLAG_TRFAIL1;
  USB->DEVICE.DeviceEndpoint[ nEp ].EPINTENSET.reg = USB_DEVICE_EPINTENSET_TRCPT1;
  USB->DEVICE.DeviceEndpoint[ nEp ].EPSTATUSSET.reg = USB_DEVICE_EPSTATUS_BK1RDY;
  
  // return 0
  return( 0 );
}

/******************************************************************************
 * @function Usb_EpGetOutLength
 *
 * @brief get output legnth
 *
 * This function will get the output length
 *
 * @param[in]   nEp     endpoint
 *  
 * @return      byte count
 *
 *****************************************************************************/
U8 Usb_EpGetOutLength( U8 nEp )
{
  // return the byte count
  return( atUsbEndpoints[ nEp ].DeviceDescBank[ 0 ].PCKSIZE.bit.BYTE_COUNT );
}

/******************************************************************************
 * @function Usb_EpSetHandled
 *
 * @brief set an endpoint to handler
 *
 * This function will clear the interuupt indicating that the endpoint was 
 * handled
 *
 * @param[in]   nEp     endpoint
 *
 *****************************************************************************/
void Usb_EpSetHandled( U8 nEp )
{
  U8    nEpNum;
  
  // get the endpoint number
  nEpNum = nEp & ENDPOINT_MASK;
  
  // check for direction
  if ( nEp & USB_DIRECTION_IN )
  {
    USB->DEVICE.DeviceEndpoint[ nEpNum ].EPINTFLAG.reg = USB_DEVICE_EPINTFLAG_TRCPT1;
  }
  else
  {
    USB->DEVICE.DeviceEndpoint[ nEpNum ].EPINTFLAG.reg = USB_DEVICE_EPINTFLAG_TRCPT0;
  }
}

/******************************************************************************
 * @function Usb_Ep0In
 *
 * @brief start EP #0 input
 *
 * This function will start endpoint 0 for input
 *
 * @param[in]   nSize   size of the transfer
 *
 *****************************************************************************/
void Usb_Ep0In( U8 nSize )
{
  // start endpoint 0 for input
  Usb_EpStartIn( USB_DIRECTION_IN | 0, g_anUsbEp0BufIn, nSize, TRUE );
}

/******************************************************************************
 * @function Usb_Ep0Out
 *
 * @brief start endpoint 0 for output
 *
 * This function will start endpoint 0 for an output
 *
 *****************************************************************************/
void Usb_Ep0Out( void )
{
  // start endpoint 0 for out
  Usb_StartOut( 0, g_anUsbEp0BufOut, USB_ENDPOINT0_SIZE );
}

/******************************************************************************
 * @function Usb_Ep0Stall
 *
 * @brief stall endpoint 0
 *
 * This function will stall endpoint 0
 *
 *****************************************************************************/
void Usb_Ep0Stall( void )
{
  // set the stall request for end point 0
  USB->DEVICE.DeviceEndpoint[ 0 ].EPSTATUSSET.reg = USB_DEVICE_EPSTATUS_STALLRQ( 0x3 );
}

/******************************************************************************
 * @function Usb_SetSpeed
 *
 * @brief set the speed       
 *
 * This function will set the low/full speed bits
 *
 * @param[in]   eSpeed      speed select
 *
 *****************************************************************************/
void Usb_SetSpeed( USBSPEED eSpeed )
{
  if ( USB_SPEED_FULL == eSpeed )
  { 
    USB->DEVICE.CTRLB.bit.SPDCONF = USB_DEVICE_CTRLB_SPDCONF_FS_Val;
  }
  else if ( USB_SPEED_LOW == eSpeed )
  {
    USB->DEVICE.CTRLB.bit.SPDCONF = USB_DEVICE_CTRLB_SPDCONF_LS_Val;
  }
}

/******************************************************************************
 * @function Usb_GetSpeed
 *
 * @brief get the speed
 *
 * This function will return the USB speed
 *
 * @return      will return the current speed
 *
 *****************************************************************************/
USBSPEED Usb_GetSpeed( void )
{
  USBSPEED  eSpeed;
  
  // set the speed
  eSpeed = ( USB->DEVICE.STATUS.bit.SPEED == 0 ) ? USB_SPEED_LOW : USB_SPEED_FULL;
  
  // return it
  return( eSpeed );
}

/******************************************************************************
 * @function Usb_Handler
 *
 * @brief USB interrupt handler
 *
 * This function will process the USB handler
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
    Usb_Reset( );
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
        // call the abstraction handler
        Usb_HandleSetup( );
      }

      // check for a control out packet
      if ( uFlags & USB_DEVICE_EPINTFLAG_TRCPT0 )
      {
        // call the abstraction handler
        Usb_HandleControlOutCallback( );
      }

      // check for a control in packet
      if ( uFlags & USB_DEVICE_EPINTFLAG_TRCPT1 )
      {
        // call the abstraction handler
        Usb_HandleCcontrolInCallback( );
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
    Usb_HandleEndpointsCallback( );
  }
}

/**@} EOF .c */
