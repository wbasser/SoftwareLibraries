/******************************************************************************
 * @file USBCDChandler_cfg.c
 *
 * @brief USB CDC handler configuration implementations
 *
 * This file provides the CDC handler configuration implementation
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
 * \addtogroup USBCDCHandler
 * @{
 *****************************************************************************/
// local includes -------------------------------------------------------------
#include "USBCDCHandler/USBCDCHandler_cfg.h"

// library includes -----------------------------------------------------------
#include "usblib/usblib.h"

// Macros and Defines ---------------------------------------------------------
// define the wide character macro
#define	WIDE_CHAR( a )			( a ) , 0

// constant parameter initializations -----------------------------------------
/// The languages supported by this device.
static	const U8	anLangDescriptor[ ] =
{
  4,
  USB_DTYPE_STRING,
  USBShort(USB_LANG_EN_US)
};

/// The manufacturer string.
static	const U8	anManufacturerString[ ] =
{
  ( 17 + 1 ) * 2,
  USB_DTYPE_STRING,
	WIDE_CHAR( 'T' ),
	WIDE_CHAR( 'e' ),
	WIDE_CHAR( 'x' ),
	WIDE_CHAR( 'a' ),
	WIDE_CHAR( 's' ),
	WIDE_CHAR( ' ' ),
	WIDE_CHAR( 'I' ),
	WIDE_CHAR( 'n' ),
	WIDE_CHAR( 's' ),
	WIDE_CHAR( 't' ),
	WIDE_CHAR( 'r' ),
	WIDE_CHAR( 'u' ),
	WIDE_CHAR( 'm' ),
	WIDE_CHAR( 'e' ),
	WIDE_CHAR( 'n' ),
	WIDE_CHAR( 't' ),
	WIDE_CHAR( 's' ),
};

/// The product string.
static	const U8	anProductString[ ] =
{
  ( 16 * 2 ) + 2,
  USB_DTYPE_STRING,
	WIDE_CHAR( 'V' ),
	WIDE_CHAR( 'i' ),
	WIDE_CHAR( 'r' ),
	WIDE_CHAR( 't' ),
	WIDE_CHAR( 'u' ),
	WIDE_CHAR( 'a' ),
	WIDE_CHAR( 'l' ),
	WIDE_CHAR( ' ' ),
	WIDE_CHAR( 'C' ),
	WIDE_CHAR( 'O' ),
	WIDE_CHAR( 'M' ),
	WIDE_CHAR( ' ' ),
	WIDE_CHAR( 'P' ),
	WIDE_CHAR( 'o' ),
	WIDE_CHAR( 'r' ),
	WIDE_CHAR( 't' ),
};

/// The serial number string.
static	const U8	 anSerialNumberString[ ] =
{
  ( 8 * 2 ) + 2,
  USB_DTYPE_STRING,
	WIDE_CHAR( '1' ),
	WIDE_CHAR( '2' ),
	WIDE_CHAR( '3' ),
	WIDE_CHAR( '4' ),
	WIDE_CHAR( '5' ),
	WIDE_CHAR( '6' ),
	WIDE_CHAR( '7' ),
	WIDE_CHAR( '8' ),
};

/// The data interface description string.
static	const U8	 anDataInterfaceString[ ] =
{
  ( 21 * 2 ) + 2,
  USB_DTYPE_STRING,
	WIDE_CHAR( 'A' ),
	WIDE_CHAR( 'C' ),
	WIDE_CHAR( 'M' ),
	WIDE_CHAR( ' ' ),
	WIDE_CHAR( 'C' ),
	WIDE_CHAR( 'o' ),
	WIDE_CHAR( 'n' ),
	WIDE_CHAR( 't' ),
	WIDE_CHAR( 'r' ),
	WIDE_CHAR( 'o' ),
	WIDE_CHAR( 'l' ),
	WIDE_CHAR( ' ' ),
	WIDE_CHAR( 'I' ),
	WIDE_CHAR( 'n' ),
	WIDE_CHAR( 't' ),
	WIDE_CHAR( 'r' ),
	WIDE_CHAR( 't' ),
	WIDE_CHAR( 'f' ),
	WIDE_CHAR( 'a' ),
	WIDE_CHAR( 'c' ),
	WIDE_CHAR( 'e' ),
};

/// The configuration description string.
static	const U8	anConfigString[ ] =
{
  ( 26 * 2 ) + 2,
  USB_DTYPE_STRING,
	WIDE_CHAR( 'S' ),
	WIDE_CHAR( 'e' ),
	WIDE_CHAR( 'l' ),
	WIDE_CHAR( 'f' ),
	WIDE_CHAR( ' ' ),
	WIDE_CHAR( 'P' ),
	WIDE_CHAR( 'o' ),
	WIDE_CHAR( 'w' ),
	WIDE_CHAR( 'e' ),
	WIDE_CHAR( 'r' ),
	WIDE_CHAR( 'e' ),
	WIDE_CHAR( 'd' ),
	WIDE_CHAR( ' ' ),
	WIDE_CHAR( 'C' ),
	WIDE_CHAR( 'o' ),
	WIDE_CHAR( 'n' ),
	WIDE_CHAR( 'f' ),
	WIDE_CHAR( 'i' ),
	WIDE_CHAR( 'g' ),
	WIDE_CHAR( 'u' ),
	WIDE_CHAR( 'r' ),
	WIDE_CHAR( 'a' ),
	WIDE_CHAR( 't' ),
	WIDE_CHAR( 'i' ),
	WIDE_CHAR( 'o' ),
	WIDE_CHAR( 'n' ),
};

/// The configuration description string.
const PU8 const apUSBCDCStringDescriptors[ ] =
{
  ( PU8 )anLangDescriptor,
	( PU8 )anManufacturerString,
  ( PU8 )anProductString,
  ( PU8 )anSerialNumberString,
  ( PU8 )anDataInterfaceString,
  ( PU8 )anConfigString
};

/// define the number of strings
#define USBCDC_NUM_OF_STRINGS   ( sizeof( apUSBCDCStringDescriptors ) / sizeof( PU8 ))

/******************************************************************************
 * @function USBCDCHandler_PutChar
 *
 * @brief output a character on the USB
 *
 * This function will send a chanracter on the USB
 *
 * @param[in]   nData   data to send
 *
 *****************************************************************************/
U8 USBCDCHandler_GetNumberOfStrings( void )
{
	// stuff the character into the buffer
	return( USBCDC_NUM_OF_STRINGS );
}

/**@} EOF USBCDCHandler_cfg.c */
