/******************************************************************************
 * @file Gpio_def.h
 *
 * @brief GPIO module configuration definitions
 *
 * This file provides the definitions of the structures and enumerations
 * used in defining the GPIO pins and usages
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
 * \addtogroup GPIO
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _GPIO_DEF_H
#define _GPIO_DEF_H

// library includes -----------------------------------------------------------
#include "Types/Types.h"

// Macros and Defines ---------------------------------------------------------
/// define the helper macro for creating output pins
#define GPIO_OUT( port, pin, mode, invert, initval ) \
  { \
    .ePort                = port, \
    .nPin                 = pin, \
    .eDir                 = GPIO_DIR_OUT, \
    .eMode                = mode, \
    .tFlags.bInvert       = invert, \
    .tFlags.bInitialState = initval, \
  }

/// helper macro for creating a function output pin  
#define GPIO_FUNC( port, pin ) \
  { \
    .ePort          = port, \
    .eDir           = GPIO_DIR_FOUT, \
    .nPin           = pin, \
  }
  
/// define the helper macro for creating input pins
#define GPIO_IN( port, pin, mode, invert ) \
  { \
    .ePort          = port, \
    .nPin           = pin, \
    .eDir           = GPIO_DIR_IN, \
    .eMode          = mode, \
    .tFlags.bInvert = invert, \
  }

/// define the helper macro for creating input/output pins
#define GPIO_INOUT( port, pin, mode, invert, initval, initdir ) \
  { \
    .ePort                  = port, \
    .nPin                   = pin, \
    .eDir                   = GPIO_DIR_IO, \
    .eMode                  = mode, \
    .tFlags.bInvert         = invert, \
    .tFlags.bInitialState   = initval, \
    .eInitDir               = initdir, \
  }
  
/// define the helper macro for creating an interrupt pin
#define GPIO_PINIRQ( port, pin, irq, mode, edge, enabled, callback ) \
  { \
    .ePort            = port, \
    .nPin             = pin, \
    .tIrqs.ePinIrq    = irq, \
    .eDir             = GPIO_DIR_PINIRQ, \
    .eMode            = mode, \
    .tEdges.ePinEdge  = edge, \
    .tFlags.bEnabled  = enabled, \
    .pvCallback       = callback, \
  }

/// define the helper macro for creating an interrupt pin
#define GPIO_EXTIRQ( port, pin, irq, mode, edge, enabled, callback ) \
  { \
    .ePort            = port, \
    .nPin             = pin, \
    .tIrqs.eExtIrq    = irq, \
    .eDir             = GPIO_DIR_EXTIRQ, \
    .eMode            = mode, \
    .tEdges.eExtEdge  = edge, \
    .tFlags.bEnabled  = enabled, \
    .pvCallback       = callback, \
  }

/// define the helper macro for creating a group
#define GPIO_GROUP( port, dir, mask, shift ) \
  { \
    .ePort            = port, \
    .eDir             = dir, \
    .nMask            = mask, \
    .nShift           = shift, \
  }

// enumerations ---------------------------------------------------------------
/// enumerate the ports
typedef enum _GPIOPORT
{
  GPIO_PORT_A = 0,
  GPIO_PORT_B,
  GPIO_PORT_C,
  GPIO_PORT_D,
  GPIO_PORT_E,
  GPIO_PORT_F,
  GPIO_PORT_G,
  GPIO_PORT_H,
  GPIO_PORT_J,
  GPIO_PORT_K,
  GPIO_PORT_MAX
} GPIOPORT;

/// enumerate the pin change interrupts
typedef enum _GPIOPINIRQ
{
  #ifdef PCINT0
    GPIO_PINIRQ_PCINT0 = 0,  
    GPIO_PINIRQ_PCINT1,
    GPIO_PINIRQ_PCINT2,
    GPIO_PINIRQ_PCINT3,
    GPIO_PINIRQ_PCINT4,
    GPIO_PINIRQ_PCINT5,
    GPIO_PINIRQ_PCINT6,
    GPIO_PINIRQ_PCINT7,
  #endif // PCINT0
  #ifdef PCINT1
    GPIO_PINIRQ_PCINT8,
    GPIO_PINIRQ_PCINT9,
    GPIO_PINIRQ_PCINT10,
    GPIO_PINIRQ_PCINT11,
    GPIO_PINIRQ_PCINT12,
    GPIO_PINIRQ_PCINT14,
    GPIO_PINIRQ_PCINT15,
    GPIO_PINIRQ_PCINT16,
  #endif // PCINT1
  #ifdef PCINT2
    GPIO_PINIRQ_PCINT17,
    GPIO_PINIRQ_PCINT18,
    GPIO_PINIRQ_PCINT19,
    GPIO_PINIRQ_PCINT20,
    GPIO_PINIRQ_PCINT21,
    GPIO_PINIRQ_PCINT22,
    GPIO_PINIRQ_PCINT23,
    GPIO_PINIRQ_PCINT24,
  #endif // PCINT2
  #ifdef PCINT3
    GPIO_PINIRQ_PCINT25,
    GPIO_PINIRQ_PCINT26,
    GPIO_PINIRQ_PCINT27,
    GPIO_PINIRQ_PCINT28,
    GPIO_PINIRQ_PCINT29,
    GPIO_PINIRQ_PCINT30,
    GPIO_PINIRQ_PCINT31,
    GPIO_PINIRQ_PCINT32,
  #endif // PCINT3
  GPIO_PINIRQ_MAX
} GPIOPINIRQ;

/// enumerate the external interrupt
typedef enum _GPIOEXTIRQ
{
  GPIO_EXTIRQ_INT0 = 0,
  GPIO_EXTIRQ_INT1,
  GPIO_EXTIRQ_INT2,
  GPIO_EXTIRQ_INT3,
  GPIO_EXTIRQ_INT4,
  GPIO_EXTIRQ_INT5,
  GPIO_EXTIRQ_INT6,
  GPIO_EXTIRQ_INT7,
  GPIO_EXTIRQ_MAX
} GPIOEXTIRQ;

/// enumerate the pin direction
typedef enum _GPIODIR
{
  GPIO_DIR_IN = 0,      ///< input
  GPIO_DIR_OUT,         ///< output
  GPIO_DIR_IO,          ///< input/output
  GPIO_DIR_FOUT,        ///< function output
  GPIO_DIR_PINIRQ,      ///< IRQ input
  GPIO_DIR_EXTIRQ,      ///< external IRQ input
  GPIO_DIR_MAX
} GPIODIR;

/// enumerate the pin mode
typedef enum _GPIOMODE
{
  GPIO_MODE_TOTEM = 0,  ///< totem mode
  GPIO_MODE_PULLUP,     ///< pull up
  GPIO_MODE_MAX
} GPIOMODE;

/// enumerate the IRQ pin edge
typedef enum _GPIOPINIRQEDGE
{
  GPIO_PINEDGE_FALL = 0,
  GPIO_PINEDGE_RISE,
  GPIO_PINEDGE_BOTH,
  GPIO_PINEDGE_MAX
} GPIOPINIRQEDGE;

/// enumerate the extern IRQ pin edge
typedef enum _GPIOEXTIRQEDGE
{
  GPIO_EXTEDGE_LVL = 0,
  GPIO_EXTEDGE_BOTH,
  GPIO_EXTEDGE_FALL,
  GPIO_EXTEDGE_RISE,
  GPIO_EXTEDGE_MAX
} GPIOEXTIRQEDGE;

/// enumerate the callback events
typedef enum _GPIOIRQCBEVENT
{
  GPIO_IRQCB_EVENT_NONE = 0,
  GPIO_IRQCB_EVENT_FALL,
  GPIO_IRQCB_EVENT_RISE,
  GPIO_IRQCB_EVENT_LEVEL,
} GPIOIRQCBEVENT;

// structures -----------------------------------------------------------------
/// define the callback
typedef void ( *PVGPIOIRQCALLBACK )( U8, U8 );

/// define the pin definition structure
typedef struct _GPIODEF
{
  GPIOPORT          ePort;          ///< port selection
  U8                nPin;           ///< port pin
  union
  {
    GPIOPINIRQ      ePinIrq;        ///< pin IRQ
    GPIOEXTIRQ      eExtIrq;        ///< external IRQ
  } tIrqs;  
  GPIODIR           eDir;           ///< pin direction
  GPIODIR           eInitDir;       ///< initial pin direction for I/O pins
  GPIOMODE          eMode;          ///< mode
  union
  {
    GPIOPINIRQEDGE  ePinEdge;       ///< pin interrupt edge
    GPIOEXTIRQEDGE  eExtEdge;       ///< external interrupt edge
  } tEdges;
  struct
  {
    BOOL            bInitialState : 1;  ///< initial state
    BOOL            bEnabled      : 1;  ///< irq enabled
    BOOL            bInvert       : 1;  ///< pin is inverted
  } tFlags;
  PVGPIOIRQCALLBACK pvCallback;         ///< callback
} GPIODEF, *PGPIODEF;
#define GPIODEF_SIZE        sizeof( GPIODEF )

/// define the group definition structure
typedef struct _GPIOGRPDEF
{
  GPIOPORT        ePort;            ///< port 
  GPIODIR         eDir;             ///< pin direction
  U8              nMask;            ///< mask value
  U8              nShift;           ///< data shift value
} GPIOGRPDEF, *PGPIOGRPDEF;
#define GPIOGRPDEF_SIZE     sizeof( GPIOGRPDEF )

/**@} EOF Gpio_def.h */

#endif  // _GPIO_DEF_H