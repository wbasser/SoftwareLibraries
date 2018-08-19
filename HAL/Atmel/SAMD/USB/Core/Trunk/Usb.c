/******************************************************************************
 * @file Usb.c
 *
 * @brief USB implementaion 
 *
 * This file pfovides the implementation for the USB
 *
 * @copyright Copyright (c) 2012 CyberIntegration
 * This document contains proprietary data and information of CyberIntegration 
 * LLC. It is the exclusive property of CyberIntegration, LLC and will not be 
 * disclosed in any form to any party without prior written permission of 
 * CyberIntegration, LLC. This document may not be r nEndpoint roduced or further used 
 * without the prior written permission of CyberIntegration, LLC.
 *
 * $Date: $
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
#include "Usb/Usb_prm.h"

// library includes -----------------------------------------------------------
#include "Types/Types.h"

// Macros and Defines ---------------------------------------------------------
/// define the position and size of the USB PAD NVM bits
#define NVM_USB_PAD_TRANSN_POS          ( 45 )
#define NVM_USB_PAD_TRANSN_SIZE         ( 5 )
#define NVM_USB_PAD_TRANSP_POS          ( 50 )
#define NVM_USB_PAD_TRANSP_SIZE         ( 5 )
#define NVM_USB_PAD_TRIM_POS            ( 55 )
#define NVM_USB_PAD_TRIM_SIZE           ( 3 )

/// define the translation from size to mask
#define USB_EP_SIZE_TO_GC(x)  ((x <= 8   )?0:\
                               (x <= 16  )?1:\
                               (x <= 32  )?2:\
                               (x <= 64  )?3:\
                               (x <= 128 )?4:\
                               (x <= 256 )?5:\
                               (x <= 512 )?6:\
                                           7)

/// define the endoint type mask
#define ENDPOINT_IN_TYPE_MASK           ( 0x80 )

/// define the endpoint number mask
#define ENDPOINT_INDEX_MASK             ( 0x3F )

/// define the serial number length
#define SER_NUM_LEN                     ( 27 )

// enumerations ---------------------------------------------------------------
/// enumerate the endoint types
typedef enum _ENDPOINTTYPE
{
  ENDPOINT_TYPE_DISBLED = 0,      ///< disabled
  ENDPOINT_TYPE_CONTROL,          ///< control
  ENDPOINT_TYPE_ISOSYNCHRONOUS,   ///< isosynchronous
  ENDOINT_TYPE_BULK,              ///< bulk
  ENDPOINT_TYPE_INTERRUPT,        ///< interrupt
  ENDPOINT_TYPE_DUAL_BANK,        ///< dual bank
} ENDPOINTTYPE;

/// enumerate the device descriptor bank indices
typedef enum _DEVDISCBANKINDICES
{
  DEVDISCBANK_INDEX_OUT = 0,      ///< output uses bank 0
  DEVDISCBANK_INDEX_IN            ///< inptut uses bank 1
} DEVDISCBANKINDICES;

// structures -----------------------------------------------------------------

// global parameter declarations ----------------------------------------------
ALIGNED4  U8  anEp0BufIn[ USB_EP0_BUF_SIZE ];
ALIGNED4  U8  anEp0BufOut[ USB_EP0_BUF_SIZE ];

// local parameter declarations -----------------------------------------------
static  UsbDeviceDescriptor atUsbEndpoints[ USB_NUM_ENDPOINTS ];
static  U8                  anSerialNumber[ SER_NUM_LEN ];

// local function prototypes --------------------------------------------------

// constant parameter initializations -----------------------------------------
/// define the serial number ascii values
static const CODE C8 acSerialVals[ ] =
{
  "0123456789ABCDEFGHJKLMNPQRSTVWXYZ"
};

/******************************************************************************
 * @function Usb_Initialize
 *
 * @brief initialization
 *
 * This function will initialize the USB for operation
 *
 *****************************************************************************/
void Usb_Initialize( void )
{
  U32 uPadTransNeg, uPadTransPos, uPadTrim;
  
  // reset the device/wait done
  USB->DEVICE.CTRLA.reg = USB_CTRLA_SWRST;
  while( USB->DEVICE.SYNCBUSY.bit.SWRST );
  
  // enable it and set device/wait done
  USB->DEVICE.CTRLA.reg = USB_CTRLA_ENABLE | USB_CTRLA_MODE_DEVICE;
  while( USB->DEVICE.SYNCBUSY.bit.ENABLE );
  
  // load pad trimmable output driver impedance N
  uPadTransNeg = *(( PU32 )( NVMCTRL_OTP4 ) + ( NVM_USB_PAD_TRANSN_POS / 32 ));
  uPadTransNeg >>= ( NVM_USB_PAD_TRANSN_POS % 32 );
  uPadTransNeg &= ( BIT( NVM_USB_PAD_TRANSN_SIZE ) - 1 );
  if ( uPadTransNeg == 0x1F )
  {
    // set the default
    uPadTransNeg = 5;
  }
  
  // load pad trimmable output driver impedance P
  uPadTransPos = *(( PU32 )( NVMCTRL_OTP4 ) + ( NVM_USB_PAD_TRANSP_POS / 32 ));
  uPadTransPos >>= ( NVM_USB_PAD_TRANSP_POS % 32 );
  uPadTransPos &= ( BIT( NVM_USB_PAD_TRANSP_SIZE ) - 1 );
  if ( uPadTransPos == 0x1F )
  {
    // set the default
    uPadTransPos = 29;
  }
  
  // load pad trim bits
  uPadTrim = *(( PU32 )( NVMCTRL_OTP4 ) + ( NVM_USB_PAD_TRIM_POS / 32 ));
  uPadTrim >>= ( NVM_USB_PAD_TRIM_POS % 32 );
  uPadTrim &= ( BIT( NVM_USB_PAD_TRIM_SIZE ) - 1 );
  if ( uPadTrim == 0x7 )
  {
    // set the default
    uPadTrim = 3;
  }
  
  // now set the trim
  USB->DEVICE.PADCAL.reg = USB_PADCAL_TRANSN( uPadTransNeg ) | USB_PADCAL_TRANSP( uPadTransPos ) | USB_PADCAL_TRIM( uPadTrim );
  
  // clear the endpoints
  memset( atUsbEndpoints, 0, USB_NUM_ENDPOINTS * sizeof ( UsbDeviceDescriptor ));
  
  // set the descriptor address
  USB->DEVICE.DESCADD.reg = ( U32 )&atUsbEndpoints[ 0 ];

  // enable the end of reset interrupt
  USB->DEVICE.INTENSET.reg = USB_DEVICE_INTENSET_EORST;
  
  // now reset the device
  Usb_Reset( );
}

/******************************************************************************
 * @function Usb_Reset
 *
 * @brief reset the USB controller
 *
 * This function will reset the USB controller
 *
 *****************************************************************************/
void Usb_Reset( void )
{
  // clear the device descriptor SRAM registers
  atUsbEndpoints[ 0 ].DeviceDescBank[ DEVDISCBANK_INDEX_OUT ].ADDR.reg = ( U32 )&anEp0BufOut;
  atUsbEndpoints[ 0 ].DeviceDescBank[ DEVDISCBANK_INDEX_OUT ].PCKSIZE.bit.SIZE=USB_EP_SIZE_TO_GC( USB_EP0_BUF_SIZE );
  atUsbEndpoints[ 0 ].DeviceDescBank[ DEVDISCBANK_INDEX_IN ].ADDR.reg = ( U32 )&anEp0BufIn;
  atUsbEndpoints[ 0 ].DeviceDescBank[ DEVDISCBANK_INDEX_IN ].PCKSIZE.bit.SIZE=USB_EP_SIZE_TO_GC( USB_EP0_BUF_SIZE );
  atUsbEndpoints[ 0 ].DeviceDescBank[ DEVDISCBANK_INDEX_IN ].PCKSIZE.bit.AUTO_ZLP = 1;
  
  // now set the interrupts
  USB->DEVICE.DeviceEndpoint[ 0 ].EPINTENSET.reg = USB_DEVICE_EPINTENSET_RXSTP;
  
  // set the endpoint config
  USB->DEVICE.DeviceEndpoint[ 0 ].EPCFG.reg = USB_DEVICE_EPCFG_EPTYPE0( ENDPOINT_TYPE_CONTROL ) | USB_DEVICE_EPCFG_EPTYPE0( ENDPOINT_TYPE_CONTROL );
}

/******************************************************************************
 * @function Usb_SetAddress
 *
 * @brief set the address
 *
 * This function will set the device address
 *
 * @param[in]   nAddr   desired address
 *
 *****************************************************************************/
void Usb_SetAddress( U8 nAddr )
{
  // store the address
  USB->DEVICE.DADD.reg = USB_DEVICE_DADD_ADDEN | nAddr;
}

/******************************************************************************
 * @function Usb_EnableEp
 *
 * @brief enable an indicated enpoint
 *
 * This function will enable an indicated endpoint
 *
 * @param[in]   nEndoint      endoint number
 * @param[in]   nTYpe         endpoint type
 * @param[in]   wBufSize      buffer size
 *
 *****************************************************************************/
void Usb_EnableEp( U8 nEndpoint, U8 nType, U16 wBufSize )
{
  U8  nEndpointOffset;
  
  // set up the endoint offset
  nEndpointOffset = nEndpoint & ENDPOINT_INDEX_MASK;
  
  // determine the type
  if ( nType & ENDPOINT_IN_TYPE_MASK )
  {
    // setup an input endpoint
    atUsbEndpoints[ nEndpointOffset ].DeviceDescBank[ DEVDISCBANK_INDEX_IN ].PCKSIZE.bit.SIZE = USB_EP_SIZE_TO_GC( wBufSize );
    USB->DEVICE.DeviceEndpoint[ nEndpointOffset ].EPCFG.bit.EPTYPE1 = nType + 1;
    USB->DEVICE.DeviceEndpoint[ nEndpointOffset ].EPSTATUSCLR.reg = USB_DEVICE_EPSTATUS_BK1RDY | USB_DEVICE_EPSTATUS_STALLRQ( 0x2 ) | USB_DEVICE_EPSTATUS_DTGLIN;
  }
  else
  {
    // setup an output endoint
		atUsbEndpoints[ nEndpointOffset ].DeviceDescBank[ DEVDISCBANK_INDEX_OUT ].PCKSIZE.bit.SIZE = USB_EP_SIZE_TO_GC( wBufSize );
		USB->DEVICE.DeviceEndpoint[ nEndpointOffset ].EPCFG.bit.EPTYPE0 = nType + 1;
		USB->DEVICE.DeviceEndpoint[ nEndpointOffset ].EPSTATUSSET.reg = USB_DEVICE_EPSTATUS_BK0RDY;
		USB->DEVICE.DeviceEndpoint[ nEndpointOffset ].EPSTATUSCLR.reg = USB_DEVICE_EPSTATUS_STALLRQ( 0x1 ) | USB_DEVICE_EPSTATUS_DTGLOUT;
  }
}

/******************************************************************************
 * @function Usb_DisableEp(
 *
 * @brief disable endpoint
 *
 * This function will the disable the given endpoint
 *
 * @param[in]   nEndoint      endoint number
 *
 *****************************************************************************/
void Usb_DisableEp( U8 nEndpoint )
{
  U8  nEndpointOffset;
  
  // set up the endoint offset
  nEndpointOffset = nEndpoint & ENDPOINT_INDEX_MASK;
  
  // determine the type
  if ( nEndpoint & ENDPOINT_IN_TYPE_MASK )
  {
    // disable endpoint
    USB->DEVICE.DeviceEndpoint[ nEndpointOffset ].EPSTATUSCLR.reg = USB_DEVICE_EPSTATUS_BK1RDY;
    USB->DEVICE.DeviceEndpoint[ nEndpointOffset ].EPCFG.bit.EPTYPE1 = ENDPOINT_TYPE_DISBLED;
  }
  else
  {
    // disable endpoint
		USB->DEVICE.DeviceEndpoint[ nEndpointOffset ].EPSTATUSSET.reg = USB_DEVICE_EPSTATUS_BK0RDY;
    USB->DEVICE.DeviceEndpoint[ nEndpointOffset ].EPCFG.bit.EPTYPE0 = ENDPOINT_TYPE_DISBLED;
  }
}

/******************************************************************************
 * @function Usb_ResetEp
 *
 * @brief reset an endpoint
 *
 * This function will reset a given endpoint
 *
 * @param[in]   nEndoint      endoint number
 *
 *****************************************************************************/
void Usb_ResetEp( U8 nEndpoint )
{
  U8  nEndpointOffset;
  
  // set up the endoint offset
  nEndpointOffset = nEndpoint & ENDPOINT_INDEX_MASK;
  
  // determine the type
  if ( nEndpoint & ENDPOINT_IN_TYPE_MASK )
  {
    // disable endpoint
    USB->DEVICE.DeviceEndpoint[ nEndpointOffset ].EPSTATUSCLR.reg = USB_DEVICE_EPSTATUS_BK1RDY;
  }
  else
  {
    // disable endpoint
		USB->DEVICE.DeviceEndpoint[ nEndpointOffset ].EPSTATUSSET.reg = USB_DEVICE_EPSTATUS_BK0RDY;
  }
}

/******************************************************************************
 * @function Usb_EpStartOut
 *
 * @brief start an output endpoint
 *
 * This function 
 *
 * @param[in]   nEndpoint     endpoint number
 * @param[in]   pnData        pointer to the data
 * @param[in]   wSize         size of the data
 *
 * @return      0
 *
 *****************************************************************************/
U8 Usb_EpStartOut( U8 nEndpoint, PU8 pnData, U16 wSize )
{
  // set up the endpoint control/set the  uStatus
	atUsbEndpoints[ nEndpoint ].DeviceDescBank[ DEVDISCBANK_INDEX_OUT ].PCKSIZE.bit.MULTI_PACKET_SIZE = wSize;
	atUsbEndpoints[ nEndpoint ].DeviceDescBank[ DEVDISCBANK_INDEX_OUT ].PCKSIZE.bit.BYTE_COUNT = 0;
	atUsbEndpoints[ nEndpoint ].DeviceDescBank[ DEVDISCBANK_INDEX_OUT ].ADDR.reg = ( U32 )pnData;
	USB->DEVICE.DeviceEndpoint[ nEndpoint ].EPINTFLAG.reg = USB_DEVICE_EPINTFLAG_TRCPT0 | USB_DEVICE_EPINTFLAG_TRFAIL0;
	USB->DEVICE.DeviceEndpoint[ nEndpoint ].EPINTENSET.reg = USB_DEVICE_EPINTENSET_TRCPT0;
	USB->DEVICE.DeviceEndpoint[ nEndpoint ].EPSTATUSCLR.reg = USB_DEVICE_EPSTATUS_BK0RDY;
  
  // return 0
  return( 0 );
}

/******************************************************************************
 * @function Usb_EpStartIn
 *
 * @brief start an input endpoint
 *
 * This function 
 *
 * @param[in]   nEndpoint     endpoint number
 * @param[in]   pnData        pointer to the data
 * @param[in]   wSize         size of the data
 * @param[in]   bZpl          ZLP uStatus
 *
 * @return      0
 *****************************************************************************/
U8 Usb_EpStartIn( U8 nEndpoint, PU8 pnData, U16 wSize, BOOL bZlp )
{
  // mask out the input bit
	 nEndpoint  &= ENDPOINT_INDEX_MASK;
   
   // set tup the endpoint control
	atUsbEndpoints[ nEndpoint ].DeviceDescBank[ DEVDISCBANK_INDEX_IN ].PCKSIZE.bit.AUTO_ZLP = bZlp;
	atUsbEndpoints[ nEndpoint ].DeviceDescBank[ DEVDISCBANK_INDEX_IN ].PCKSIZE.bit.MULTI_PACKET_SIZE = 0;
	atUsbEndpoints[ nEndpoint ].DeviceDescBank[ DEVDISCBANK_INDEX_IN ].PCKSIZE.bit.BYTE_COUNT = wSize;
	atUsbEndpoints[ nEndpoint ].DeviceDescBank[ DEVDISCBANK_INDEX_IN ].ADDR.reg = ( U32 )pnData;
  
  // setup the device
	USB->DEVICE.DeviceEndpoint[ nEndpoint ].EPINTFLAG.reg = USB_DEVICE_EPINTFLAG_TRCPT1 | USB_DEVICE_EPINTFLAG_TRFAIL1;
	USB->DEVICE.DeviceEndpoint[ nEndpoint ].EPINTENSET.reg = USB_DEVICE_EPINTENSET_TRCPT1;
	USB->DEVICE.DeviceEndpoint[ nEndpoint ].EPSTATUSSET.reg = USB_DEVICE_EPSTATUS_BK1RDY;
  
  // return 0
	return ( 0 );
}

/******************************************************************************
 * @function Usb_EpEmpty
 *
 * @brief tests for an endpoint empty
 *
 * This function will return the uStatus of the endpoint
 *
 * @param[in]   nEndpoint     endpoint
 *
 * @return      TRUE if empty, FALSE if not
 *
 *****************************************************************************/
BOOL Usb_EpEmpty( U8 nEndpoint )
{
  U8    nEndpointOffset;
  BOOL  bStatus;
  
  // set up the endoint offset
  nEndpointOffset = nEndpoint & ENDPOINT_INDEX_MASK;
  
  // determine the type
  if ( nEndpoint & ENDPOINT_IN_TYPE_MASK )
  {
    // set the uStatus
    bStatus = !( USB->DEVICE.DeviceEndpoint[ nEndpointOffset ].EPSTATUSCLR.bit.BK1RDY || Usb_EpPending( nEndpoint ));
  }
  else
  {
    // disable endpoint
    bStatus = !( USB->DEVICE.DeviceEndpoint[ nEndpointOffset ].EPSTATUSCLR.bit.BK0RDY || Usb_EpPending( nEndpoint ));
  }
  
  // return the uStatus
  return( bStatus );
}

/******************************************************************************
 * @function Usb_EpReady
 *
 * @brief tests the ready uStatus
 *
 * This function will test to see if an endpoint is ready
 *
 * @param[in]   nEndpoint     endpoint
 *
 * @return      TRUE if empty, FALSE if not
 *
 *
 *****************************************************************************/
BOOL Usb_EpReady( U8 nEndpoint )
{
  // just call endpoint empty
  return( Usb_EpEmpty( nEndpoint ));
}

/******************************************************************************
 * @function Usb_EpPending
 *
 * @brief test pending uStatus
 *
 * This function test to see if an endpoint is pending completion
 *
 * @param[in]   nEndpoint     endpoint
 *
 * @return      TRUE if empty, FALSE if not
 *
 *****************************************************************************/
BOOL Usb_EpPending( U8 nEndpoint )
{
  U8    nEndpointOffset;
  BOOL  bStatus;
  
  // set up the endoint offset
  nEndpointOffset = nEndpoint & ENDPOINT_INDEX_MASK;
  
  // determine the type
  if ( nEndpoint & ENDPOINT_IN_TYPE_MASK )
  {
    // set the uStatus
    bStatus = USB->DEVICE.DeviceEndpoint[ nEndpointOffset ].EPINTFLAG.bit.TRCPT1;
  }
  else
  {
    // set the uStatus
    bStatus = USB->DEVICE.DeviceEndpoint[ nEndpointOffset ].EPINTFLAG.bit.TRCPT0;
  }
  
  // return the uStatus
  return( bStatus );
}

/******************************************************************************
 * @function Usb_EpHandled
 *
 * @brief clear the interrupt flag
 *
 * This function will clear the appropriate interrupt flag afte an
 * endpoint event is handler
 *
 * @param[in]   nEndpoint     endpoint
 *
 *****************************************************************************/
void Usb_EpHandler( U8 nEndpoint )
{
  U8    nEndpointOffset;
  
  // set up the endoint offset
  nEndpointOffset = nEndpoint & ENDPOINT_INDEX_MASK;
  
  // determine the type
  if ( nEndpoint & ENDPOINT_IN_TYPE_MASK )
  {
    // clear the int flag
   USB->DEVICE.DeviceEndpoint[ nEndpointOffset ].EPINTFLAG.reg = USB_DEVICE_EPINTFLAG_TRCPT1;
  }
  else
  {
    // clear the int flag
   USB->DEVICE.DeviceEndpoint[ nEndpointOffset ].EPINTFLAG.reg = USB_DEVICE_EPINTFLAG_TRCPT0;
  }
}

/******************************************************************************
 * @function Usb_EpOutLength
 *
 * @brief gets the current endpoint out length
 *
 * This function will return the lenght of an output endpoint
 *
 * @param[in]   eEndPoint
 *
 * @return      lelngth
 *
 *****************************************************************************/
U16 Usb_EpOutLength( U8 nEndpoint )
{
  // return the length
	return ( atUsbEndpoints[ nEndpoint ].DeviceDescBank[ DEVDISCBANK_INDEX_OUT ].PCKSIZE.bit.BYTE_COUNT );
}

/******************************************************************************
 * @function Usb_Detach
 *
 * @brief detach the USB device
 *
 * This function will cause the USB controller to detach from the bus
 *
 *****************************************************************************/
void Usb_Detach( void )
{
  // detach from bus/disable interrupt
  USB->DEVICE.CTRLB.bit.DETACH = ON;
	NVIC_DisableIRQ( USB_IRQn );
}

/******************************************************************************
 * @function Usb_Attach
 *
 * @brief attach the USB to the bus
 *
 * This function attach the USB device to the bus
 *
 *****************************************************************************/
void Usb_Attach( void )
{
  // enable the interrupt/attach the device
	NVIC_EnableIRQ( USB_IRQn );
  USB->DEVICE.CTRLB.bit.DETACH = OFF;
}

/******************************************************************************
 * @function Usb_Ep0Out
 *
 * @brief start an out transfer on EP0
 *
 * This function will start an output transfer on EP0
 *
 *****************************************************************************/
void Usb_Ep0Out( void )
{
  // call the start out function for endpoint 0
  Usb_EpStartOut( 0, anEp0BufOut, USB_EP0_BUF_SIZE );
}

/******************************************************************************
 * @function Usb_Ep0In
 *
 * @brief start an input transfer on Endoint 0
 *
 * This function will start an input transfer on endpoint 0
 *
 * @param[in]   nSize     number of bytes to transfer
 *
 *****************************************************************************/
void Usb_Ep0In( U8 nSize )
{
  // call the start in function for endpoint 0
  Usb_EpStartIn( 0, anEp0BufOut, USB_EP0_BUF_SIZE, TRUE );
}

/******************************************************************************
 * @function Usb_Ep0Stall
 *
 * @brief stall endpoint 0
 *
 * This function will set the stall condition for endpoint 0
 *
 *****************************************************************************/
void Usb_Ep0Stall( void )
{
  // set the stall request
  USB->DEVICE.DeviceEndpoint[ 0 ].EPSTATUSSET.reg = USB_DEVICE_EPSTATUS_STALLRQ( 0x03 );
}

/******************************************************************************
 * @function Usb_SetSpeed
 *
 * @brief sets the USB speed
 *
 * This function will set the speed
 *
 * @param[in]   eSPeed    speed enumeration
 *
 *****************************************************************************/
void Usb_SetSpeed( USBSPEED eSpeed )
{
	if ( USB_SPEED_FULL == eSpeed )
  {
    // set speed configutation 0 value
		USB->DEVICE.CTRLB.bit.SPDCONF = USB_DEVICE_CTRLB_SPDCONF_LS;
	}
  else if (USB_SPEED_LOW == eSpeed )
  {
    // set the speed configuration 1 value
		USB->DEVICE.CTRLB.bit.SPDCONF = USB_DEVICE_CTRLB_SPDCONF_FS;
	}
}

/******************************************************************************
 * @function Usb_GetSpeed
 *
 * @brief gets the current speed
 *
 * This function will get the speed
 *
 * @return      the current speed
 *
 *****************************************************************************/
USBSPEED Usb_GetSpeed( void )
{
  USBSPEED eSpeed;
  
  // determine the speed
  eSpeed = ( USB->DEVICE.STATUS.bit.SPEED == 0 ) ? USB_SPEED_LOW : USB_SPEED_FULL;
  
  // return the speed
  return( eSpeed );
}

/******************************************************************************
 * @function Usb_GetSerialNumberSctringDescriptor( )
 *
 * @brief builds an returns a serial number
 *
 * This function will return a serial number
 *
 * @return     pointer to the generated serial number
 *
 *****************************************************************************/
PU8 Usb_GetSerialNumberSctringDescriptor( void )
{
  U8  nKeyIdx, nPos, nCharIdx,  nSerIdx;
  C8  acKey[ 8 ] = { USB_SERIAL_KEY  };
  
  for ( nSerIdx = 0; nSerIdx < SER_NUM_LEN - 1; nSerIdx++ )
  {
    // compute key index/position/character indices
    nKeyIdx = ( nSerIdx * 5 ) / 8;
    nPos = ( nSerIdx * 5 ) % 8;
    nCharIdx = (( acKey[ nKeyIdx ]  >> nPos ) | ( acKey[ nKeyIdx + 1 ] << ( 8- nPos ))) & (( 1 << 5 ) - 1 );
    anSerialNumber[ nSerIdx ] = acSerialVals[ nCharIdx ];
  }
  
  // set the last byte to 0
  anSerialNumber[ SER_NUM_LEN - 1 ] = '0';
  
  return( anSerialNumber );
}

/******************************************************************************
 * @function USB_Handler
 *
 * @brief interrupt handler
 *
 * This function will lprocess the USB interrupts
 *
 *****************************************************************************/
void USB_Handler( )
{
	U32 uSummary, uStatus, uFlags;
  __CHAR16_TYPE__  cTemp;

  // get the summary and status regs
	uSummary = USB->DEVICE.EPINTSMRY.reg;
	uStatus = USB->DEVICE.INTFLAG.reg;
  
  // test for a reset
	if  (uStatus & USB_DEVICE_INTFLAG_EORST )
  {
    // clear the flag/reset the device
		USB->DEVICE.INTFLAG.reg = USB_DEVICE_INTFLAG_EORST;
		Usb_Reset( );
//		Usb_CbReset( );
	}
  else
  {
    if ( uSummary & ( 1 << 0 ))
    {
      // get the flags
      uFlags = USB->DEVICE.DeviceEndpoint[0].EPINTFLAG.reg;
      USB->DEVICE.DeviceEndpoint[0].EPINTFLAG.reg = USB_DEVICE_EPINTFLAG_TRCPT1 | USB_DEVICE_EPINTFLAG_TRCPT0 | USB_DEVICE_EPINTFLAG_RXSTP;
      
      // test for an setup
      if ( uFlags & USB_DEVICE_EPINTFLAG_RXSTP )
      {
        // call the local handler setup functon
        Usb_HandleSetUp( anEp0BufOut );
      }
      
      // test for an control output complete
      if ( uFlags & USB_DEVICE_EPINTFLAG_TRCPT0 )
      {
        // call the local handler contorl out function
        UsbDeviceHandler_HandleControlOutComplete( anEp0BufOut );
      }
      
      // test for coontrol input complete
      if ( uFlags & USB_DEVICE_EPINTFLAG_TRCPT1 )
      {
        // call the local handler contorl in function
        UsbDeviceHandler_HandleControlInComplete( anEp0BufOut );
      }
    }

    // for each endpoint
    for ( U8 nEndpoint = 1; nEndpoint  < USB_NUM_ENDPOINTS; nEndpoint++ )
    {
      // test for an endoint interrupt
      if ( uSummary & BIT( nEndpoint ))
      {
        uFlags = USB->DEVICE.DeviceEndpoint[ nEndpoint ].EPINTFLAG.reg;
        USB->DEVICE.DeviceEndpoint[ nEndpoint ].EPINTENCLR.reg = uFlags;
      }
    }

    // call the calllback completeion function
    UsbDeviceHandler_HandleCbComplete( );
  }
}

/**@} EOF Usb.c */
