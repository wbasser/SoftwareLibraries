/******************************************************************************
 * @file NeoPixelHandler.c
 *
 * @brief Neo Pixel Led Controller Handler implementation 
 *
 * This file provides the implementation for the Neo Pixel LED controller
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
 * \addtogroup NeoPixelHandler
 * @{
 *****************************************************************************/

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "NeoPixelHandler/NeoPixelHandler.h"
#include "NeoPixelHandler/NeoPixelHandler_prm.h"

// library includes -----------------------------------------------------------
#include "Interrupt/Interrupt.h"

// Macros and Defines ---------------------------------------------------------
/// define the sizes for an RGB/RGBW devices
#define RGB_SIZE            ( 3 )
#define RGBW_SIZE           ( 4 )

/// define the maximum number of devices
#define MAX_NUM_DEVICES     ( 64 )

/// define the pin
#define NEO_PIXEL_PIN       ( 1 )

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations ----------------------------------------------

// local parameter declarations -----------------------------------------------
static  U8            nRedOffset;
static  U8            nGrnOffset;
static  U8            nBluOffset;
static  U8            nWhtOffset;
static  PU8           pnPixelData;
static  U16           wByteCount;
static  U8            nLclNumDevs;
static  NEOPIXELTYPE  eLclType;
static  NEOPIXELSPEED eLclSpeed;

// EEPROM parameters
static  EEPROM  NEOPIXELSPEED eEepSpeed;
static  EEPROM  U8            nEepNumDevs;
static  EEPROM  NEOPIXELTYPE  eEepType;

// local function prototypes --------------------------------------------------
static  void  SetOffsets( void );

// constant parameter initializations -----------------------------------------

/******************************************************************************
 * @function NeoPixelHandler_Initialize
 *
 * @brief initilziation
 *
 * This function will perform any initialization needed by the Neo Pixel
 * handler
 *
 *****************************************************************************/
void NeoPixelHandler_Initialize( void )
{
  // reset the seped
  eLclSpeed = EEP_RDBYTE( eEepSpeed );

  // test for speed
  if ( eLclSpeed > NEOPIXEL_SPEED_MAX )
  {
    // restore to original/update it
    eLclSpeed = NEOPIXEL_SPEED_400HZ;
  EEP_WRBYTE( eEepSpeed, eLclSpeed );
  }

  // read the type
  eLclType = EEP_RDBYTE( eEepType );

  if ( eLclType > NEOPIXEL_TYPE_MAX )
  {
    // restore to original/update it
    eLclType = NEOPIXEL_TYPE_RGB;
    EEP_WRBYTE( eEepType, eLclType );
  }

  // set the default offsets for a RGB, no white
  SetOffsets( );

  // get the default number of devices
  nLclNumDevs = EEP_RDBYTE( nEepNumDevs );
  if ( nLclNumDevs > MAX_NUM_DEVICES )
  {
    // store it/update it
    nLclNumDevs = 1;
    EEP_WRBYTE( nEepNumDevs, nLclNumDevs );
  }

  // allocate memory for the single device
  wByteCount = nLclNumDevs * ( nWhtOffset == 0xFF ) ? RGB_SIZE : RGBW_SIZE;
  pnPixelData = malloc( wByteCount );
}

/******************************************************************************
 * @function NeoPixelHandler_SetConfiguration
 *
 * @brief set the type
 *
 * This function will set the type and adjust the offsets appropriately
 * as wellas the number of devices
 *
 * @param[in]   eSpeed      speed
 * @param[in]   eType       type of device
 * @param[in]   nNumDevices number of devices
 *
 *****************************************************************************/
void NeoPixelHandler_SetConfiguration( NEOPIXELSPEED eSpeed, NEOPIXELTYPE eReqType, U8 nNumDevices )
{
  // test for speed
  if ( eSpeed > NEOPIXEL_SPEED_MAX )
  {
    // set to default
    eSpeed = NEOPIXEL_SPEED_400HZ;
  }

  // update eeprom
  eLclSpeed = eSpeed;
  EEP_WRBYTE( eEepSpeed, eLclSpeed );

  // is this a valid type
  if ( eReqType > NEOPIXEL_TYPE_MAX )
  {
    // set to default
    eReqType = NEOPIXEL_TYPE_RGB;
  }

  // update eeprom
  eLclType = eReqType;
  EEP_WRBYTE( eEepType, eReqType );

  // set the offsets
  SetOffsets( );

  // set the number of devices
  nLclNumDevs = nNumDevices;

  // check for maximum number of devices
  if ( nLclNumDevs > MAX_NUM_DEVICES )
  {
    // clamp at maximum
    nLclNumDevs = MAX_NUM_DEVICES;
  }

  // update eeprom
  EEP_WRBYTE( nEepNumDevs, nLclNumDevs );

  // free the memory
  free( pnPixelData );

  // now - rellaocate memory
  wByteCount = nLclNumDevs * ( nWhtOffset == 0xFF ) ? RGB_SIZE : RGBW_SIZE;
  pnPixelData = malloc( wByteCount );
}

/******************************************************************************
 * @function NeoPixelHandler_GetConfiguration
 *
 * @brief set the configuration
 *
 * This function will get the configuration
 *
 * @param[io]   peSpeed      speed
 * @param[io]   peType       type of device
 * @param[io]   pnNumDevices number of devices
 *
 *****************************************************************************/
void NeoPixelHandler_GetConfiguration( PNEOPIXELSPEED peSpeed, PNEOPIXELTYPE peType, PU8 pnNumDevices )
{
  // copy the configuration
  *( peSpeed ) = eLclSpeed;
  *( peType ) = eLclType;
  *( pnNumDevices ) = nLclNumDevs;
}

/******************************************************************************
 * @function NeoPixelHandler_SetPixelColor
 *
 * @brief set the pixel color
 *
 * This function will set the pixel color for a single device or all devices
 *
 * @param[in]   nDeviceIndex      index of the device
 * @param[in]   nRed              red color
 * @param[in]   nGrn              green color
 * @param[in]   nBlu              blue color
 * @param[in]   nWht              white color
 *
 *****************************************************************************/
void NeoPixelHandler_SetPixelColor( U8 nDeviceIndex, U8 nRed, U8 nGrn, U8 nBlu, U8 nWht )
{
  U8  nDevIdx;
  U16 wOffset;

  // determine if this is a global 
  if ( nDeviceIndex == NEOPIXEL_ALL_DEVICES )
  {
    // for each device
    for ( nDevIdx = 0; nDevIdx < nLclNumDevs; nDevIdx++ )
    {
      // compute the device offset
      wOffset = ( nDevIdx * ( nWhtOffset == 0xFF ) ? RGB_SIZE : RGBW_SIZE );

      // now set the RGB data
      *( pnPixelData +  wOffset + nRedOffset ) = nRed;
      *( pnPixelData +  wOffset + nGrnOffset ) = nGrn;
      *( pnPixelData +  wOffset + nBluOffset ) = nBlu;

      // optional white
      if ( nWhtOffset != 0xFF )
      {
        // set the white data
        *( pnPixelData +  wOffset + nBluOffset ) = nWht;
      }
    }
  }
  else
  {
    // check for a valid device
    if ( nDeviceIndex < MAX_NUM_DEVICES )
    {
      // compute the device offset
      wOffset = ( nDeviceIndex * ( nWhtOffset == 0xFF ) ? RGB_SIZE : RGBW_SIZE );

      // now set the RGB data
      *( pnPixelData +  wOffset + nRedOffset ) = nRed;
      *( pnPixelData +  wOffset + nGrnOffset ) = nGrn;
      *( pnPixelData +  wOffset + nBluOffset ) = nBlu;

      // optional white
      if ( nWhtOffset != 0xFF )
      {
        // set the white data
        *( pnPixelData +  wOffset + nBluOffset ) = nWht;
      }
    }
  }
}

/******************************************************************************
 * @function NeoPixelHandler_Refresh
 *
 * @brief refresh all the neo pixels
 *
 * This function will output the data to the neo pixels
 *
 *****************************************************************************/
void NeoPixelHandler_Refresh( void )
{
  U8  nPortHi, nPortLo, nBuf1, nBuf2, nCurByte;
  PU8 pnCurPtr, pnPort;
  U16 wCount;

  // set up the variables
  nPortHi = NEOPIXELHANDLER_OUTPUT_PORT | BIT( NEOPIXELHANDLER_OUTPUT_PIN );
  nPortLo = NEOPIXELHANDLER_OUTPUT_PORT & ~( BIT( NEOPIXELHANDLER_OUTPUT_PIN ));
  nBuf1 = nBuf2 = nPortLo;

  // set the pointer
  pnCurPtr = pnPixelData;

  // set the current byte
  nCurByte = *( pnCurPtr++ );

  // calculate the count
  wCount = wByteCount;

  // initialize the port pointer
  pnPort = ( PU8 )&PORTB;

  // disable interrupts
  Interrupt_Disable( );

  // determine speed
  if ( eLclSpeed == NEOPIXEL_SPEED_400HZ )
  {
    // set up bit count in nBuf2
    nBuf2 = 8;

    // output loop
    asm volatile
    (
      "output400:"                        "\n\t"
        "st   %a[pnPort], %[nPortHi]"     "\n\t"
        "sbrc %[nCurByte], 7"             "\n\t"
        "mov  %[nBuf1], %[nPortHi]"       "\n\t"
        "st   %a[pnPort], %[nBuf1]"       "\n\t"
        "mov  %[nBuf1], %[nPortLo]"       "\n\t"
        "dec  %[nBuf2]"                   "\n\t"
        "breq next400"                    "\n\t"
        "rol  %[nCurByte]"                "\n\t"
        "st   %a[pnPort], %[nPortLo]"     "\n\t"
        "rjmp .+0"                        "\n\t"
        "rjmp .+0"                        "\n\t"
        "rjmp .+0"                        "\n\t"
        "rjmp output400"                  "\n\t"
      "next400:"                          "\n\t"
        "st   %a[pnPort], %[nPortLo]"     "\n\t"
        "nop"                             "\n\t"
        "ldi  %[nBuf2], 8"                "\n\t"
        "ld   %[nCurByte], %a[pnCurPtr]+" "\n\t"
        "sbiw %[wCount],  1"              "\n\t"
        "brne output400"                  "\n"
      : [ pnPort ]    "+e"  ( pnPort ),
        [ nCurByte ]  "+r"  ( nCurByte ),
        [ nBuf1 ]     "+r"  ( nBuf1 ),
        [ nBuf2 ]     "+r"  ( nBuf2 ),
        [ wCount ]    "+w"  ( wCount )
      : [ pnCurPtr ]  "e"   ( pnCurPtr ),
        [ nPortHi ]   "r"   ( nPortHi ),
        [ nPortLo ]   "r"   ( nPortLo )
    );
  }
  else
  {
    // output loop
    asm volatile
    (
      "output800:"                        "\n\t"
        // bit 7
        "out  %[pnPort], %[nPortHi]"      "\n\t"
        "mov  %[nBuf2], %[nPortLo]"       "\n\t"
        "out  %[pnPort], %[nBuf1]"        "\n\t"
        "rjmp .+0"                        "\n\t"
        "sbrc %[nCurByte], 6"             "\n\t"
        "mov  %[nBuf2], %[nPortHi]"       "\n\t"
        "out  %[pnPort], %[nPortLo]"      "\n\t"
        "rjmp .+0"                        "\n\t"
        // bit 6
        "out  %[pnPort], %[nPortHi]"      "\n\t"
        "mov  %[nBuf1], %[nPortLo]"       "\n\t"
        "out  %[pnPort], %[nBuf2]"        "\n\t"
        "rjmp .+0"                        "\n\t"
        "sbrc %[nCurByte], 5"             "\n\t"
        "mov  %[nBuf1], %[nPortHi]"       "\n\t"
        "out  %[pnPort], %[nPortLo]"      "\n\t"
        "rjmp .+0"                        "\n\t"
        // bit 5
        "out  %[pnPort], %[nPortHi]"      "\n\t"
        "mov  %[nBuf2], %[nPortLo]"       "\n\t"
        "out  %[pnPort], %[nBuf1]"        "\n\t"
        "rjmp .+0"                        "\n\t"
        "sbrc %[nCurByte], 4"             "\n\t"
        "mov  %[nBuf2], %[nPortHi]"       "\n\t"
        "out  %[pnPort], %[nPortLo]"      "\n\t"
        "rjmp .+0"                        "\n\t"
        // bit 4
        "out  %[pnPort], %[nPortHi]"      "\n\t"
        "mov  %[nBuf1], %[nPortLo]"       "\n\t"
        "out  %[pnPort], %[nBuf2]"        "\n\t"
        "rjmp .+0"                        "\n\t"
        "sbrc %[nCurByte], 3"             "\n\t"
        "mov  %[nBuf1], %[nPortHi]"       "\n\t"
        "out  %[pnPort], %[nPortLo]"      "\n\t"
        "rjmp .+0"                        "\n\t"
        // bit 3
        "out  %[pnPort], %[nPortHi]"      "\n\t"
        "mov  %[nBuf2], %[nPortLo]"       "\n\t"
        "out  %[pnPort], %[nBuf1]"        "\n\t"
        "rjmp .+0"                        "\n\t"
        "sbrc %[nCurByte], 2"             "\n\t"
        "mov  %[nBuf2], %[nPortHi]"       "\n\t"
        "out  %[pnPort], %[nPortLo]"      "\n\t"
        "rjmp .+0"                        "\n\t"
        // bit 2
        "out  %[pnPort], %[nPortHi]"      "\n\t"
        "mov  %[nBuf1], %[nPortLo]"       "\n\t"
        "out  %[pnPort], %[nBuf2]"        "\n\t"
        "rjmp .+0"                        "\n\t"
        "sbrc %[nCurByte], 1"             "\n\t"
        "mov  %[nBuf1], %[nPortHi]"       "\n\t"
        "out  %[pnPort], %[nPortLo]"      "\n\t"
        "rjmp .+0"                        "\n\t"
        // bit 1
        "out  %[pnPort], %[nPortHi]"      "\n\t"
        "mov  %[nBuf2], %[nPortLo]"       "\n\t"
        "out  %[pnPort], %[nBuf1]"        "\n\t"
        "rjmp .+0"                        "\n\t"
        "sbrc %[nCurByte], 0"             "\n\t"
        "mov  %[nBuf2], %[nPortHi]"       "\n\t"
        "out  %[pnPort], %[nPortLo]"      "\n\t"
        "sbiw %[wCount],  1"              "\n\t"
        // bit 0
        "out  %[pnPort], %[nPortHi]"      "\n\t"
        "mov  %[nBuf1], %[nPortLo]"       "\n\t"
        "out  %[pnPort], %[nBuf2]"        "\n\t"
        "ld   %[nCurByte], %a[pnCurPtr]+" "\n\t"
        "sbrc %[nCurByte], 7"             "\n\t"
        "mov  %[nBuf1], %[nPortHi]"       "\n\t"
        "out  %[pnPort], %[nPortLo]"      "\n\t"
        "brne output800"                  "\n"
      : [ nCurByte ]  "+r"  ( nCurByte ),
        [ nBuf1 ]     "+r"  ( nBuf1 ),
        [ nBuf2 ]     "+r"  ( nBuf2 ),
        [ wCount ]    "+w"  ( wCount )
      : [ pnPort ]    "I"   ( _SFR_IO_ADDR( PORTB )),
        [ pnCurPtr ]  "e"   ( pnCurPtr ),
        [ nPortHi ]   "r"   ( nPortHi ),
        [ nPortLo ]   "r"   ( nPortLo )
    );
  }

  // re-enable interrupts
  Interrupt_Enable( );
}

/******************************************************************************
 * @function SetOffsets
 *
 * @brief set the offsets
 *
 * This function will set the offsets
 *
 *****************************************************************************/
static void SetOffsets( void )
{
  // determine the device
  switch( eLclType )
  {
    case NEOPIXEL_TYPE_RGB :
    nRedOffset = 0;
    nGrnOffset = 0;
    nBluOffset = 0;
    nWhtOffset = 0xFF;
    break;

    case NEOPIXEL_TYPE_RBG :
    nRedOffset = 0;
    nGrnOffset = 2;
    nBluOffset = 1;
    nWhtOffset = 0xFF;
    break;

    case NEOPIXEL_TYPE_BRG :
    nRedOffset = 1;
    nGrnOffset = 2;
    nBluOffset = 0;
    nWhtOffset = 0xFF;
    break;

    case NEOPIXEL_TYPE_BGR :
    nRedOffset = 2;
    nGrnOffset = 1;
    nBluOffset = 0;
    nWhtOffset = 0xFF;
    break;

    case NEOPIXEL_TYPE_GBR :
    nRedOffset = 2;
    nGrnOffset = 0;
    nBluOffset = 1;
    nWhtOffset = 0xFF;
    break;

    case NEOPIXEL_TYPE_GRB :
    nRedOffset = 1;
    nGrnOffset = 0;
    nBluOffset = 2;
    nWhtOffset = 0xFF;
    break;

    case NEOPIXEL_TYPE_RGBW :
    nRedOffset = 0;
    nGrnOffset = 1;
    nBluOffset = 2;
    nWhtOffset = 3;
    break;

    case NEOPIXEL_TYPE_RBGW :
    nRedOffset = 0;
    nGrnOffset = 2;
    nBluOffset = 1;
    nWhtOffset = 3;
    break;

    case NEOPIXEL_TYPE_BRGW :
    nRedOffset = 1;
    nGrnOffset = 2;
    nBluOffset = 0;
    nWhtOffset = 3;
    break;

    case NEOPIXEL_TYPE_BGRW :
    nRedOffset = 2;
    nGrnOffset = 1;
    nBluOffset = 0;
    nWhtOffset = 3;
    break;

    case NEOPIXEL_TYPE_GBRW :
    nRedOffset = 2;
    nGrnOffset = 0;
    nBluOffset = 1;
    nWhtOffset = 3;
    break;

    case NEOPIXEL_TYPE_GRBW :
    nRedOffset = 1;
    nGrnOffset = 0;
    nBluOffset = 2;
    nWhtOffset = 3;
    break;

    case NEOPIXEL_TYPE_RGWB :
    nRedOffset = 0;
    nGrnOffset = 0;
    nBluOffset = 3;
    nWhtOffset = 2;
    break;

    case NEOPIXEL_TYPE_RBWG :
    nRedOffset = 0;
    nGrnOffset = 3;
    nBluOffset = 1;
    nWhtOffset = 2;
    break;

    case NEOPIXEL_TYPE_BRWG :
    nRedOffset = 1;
    nGrnOffset = 3;
    nBluOffset = 0;
    nWhtOffset = 2;
    break;

    case NEOPIXEL_TYPE_BGWR :
    nRedOffset = 3;
    nGrnOffset = 1;
    nBluOffset = 0;
    nWhtOffset = 2;
    break;

    case NEOPIXEL_TYPE_GBWR :
    nRedOffset = 3;
    nGrnOffset = 0;
    nBluOffset = 1;
    nWhtOffset = 2;
    break;

    case NEOPIXEL_TYPE_GRWB :
    nRedOffset = 1;
    nGrnOffset = 0;
    nBluOffset = 3;
    nWhtOffset = 2;
    break;

    case NEOPIXEL_TYPE_RWGB :
    nRedOffset = 0;
    nGrnOffset = 2;
    nBluOffset = 3;
    nWhtOffset = 1;
    break;

    case NEOPIXEL_TYPE_RWBG :
    nRedOffset = 0;
    nGrnOffset = 3;
    nBluOffset = 2;
    nWhtOffset = 1;
    break;

    case NEOPIXEL_TYPE_BWRG :
    nRedOffset = 2;
    nGrnOffset = 3;
    nBluOffset = 0;
    nWhtOffset = 1;
    break;

    case NEOPIXEL_TYPE_BWGR :
    nRedOffset = 3;
    nGrnOffset = 2;
    nBluOffset = 0;
    nWhtOffset = 1;
    break;

    case NEOPIXEL_TYPE_GWBR :
    nRedOffset = 3;
    nGrnOffset = 0;
    nBluOffset = 2;
    nWhtOffset = 1;
    break;

    case NEOPIXEL_TYPE_GWRB :
    nRedOffset = 2;
    nGrnOffset = 0;
    nBluOffset = 3;
    nWhtOffset = 1;
    break;

    case NEOPIXEL_TYPE_WRGB :
    nRedOffset = 1;
    nGrnOffset = 2;
    nBluOffset = 3;
    nWhtOffset = 0;
    break;

    case NEOPIXEL_TYPE_WRBG :
    nRedOffset = 1;
    nGrnOffset = 3;
    nBluOffset = 2;
    nWhtOffset = 0;
    break;

    case NEOPIXEL_TYPE_WBRG :
    nRedOffset = 2;
    nGrnOffset = 3;
    nBluOffset = 1;
    nWhtOffset = 0;
    break;

    case NEOPIXEL_TYPE_WBGR :
    nRedOffset = 3;
    nGrnOffset = 2;
    nBluOffset = 1;
    nWhtOffset = 0;
    break;

    case NEOPIXEL_TYPE_WGBR :
    nRedOffset = 3;
    nGrnOffset = 1;
    nBluOffset = 2;
    nWhtOffset = 0;
    break;

    case NEOPIXEL_TYPE_WGRB :
    nRedOffset = 2;
    nGrnOffset = 1;
    nBluOffset = 3;
    nWhtOffset = 0;
    break;

    default :
    nRedOffset = 0;
    nGrnOffset = 1;
    nBluOffset = 2;
    nWhtOffset = 0xFF;
    break;
  }
}

/**@} EOF NeoPixelHandler.c */
