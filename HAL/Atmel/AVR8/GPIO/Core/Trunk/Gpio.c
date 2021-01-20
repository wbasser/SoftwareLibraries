/******************************************************************************
 * @file GPIO.c
 *
 * @brief GPIO implementation
 *
 * This file implements the GPIO subsystem 
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

// system includes ------------------------------------------------------------

// library includes -----------------------------------------------------------

// local includes -------------------------------------------------------------
#include "GPIO/Gpio.h"

// Macros and Defines ---------------------------------------------------------
/// define the number of pin change callback structures there are
#ifdef PCINT0 
  #define NUM_PCIRQ_BLKS  1
#endif // PCINT0
#ifdef PCINT1
  #undef  NUM_PCIRQ_BLKS
  #define NUM_PCIRQ_BLKS  2
#endif // PCINT1
#ifdef PCINT2
  #undef  NUM_PCIRQ_BLKS
  #define NUM_PCIRQ_BLKS  4
#endif // PCINT2
#ifdef PCINT3
  #undef  NUM_PCIRQ_BLKS
  #define NUM_PCIRQ_BLKS  4
#endif // PCINT3

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------
/// define the control structure
typedef struct _GPIOCTL
{
  PU8 pnPortReg;
  PU8 pnDirReg;
  PU8 pnPinReg;
  U8  nMask;
} GPIOCTL, *PGPIOCTL;
#define GPIOCTL_SIZE    sizeof( GPIOCTL )

/// define the pin interrupt structure
typedef struct _PINIRQMAP
{
  U8            nPrvCap;
  PU8           pnPinReg;
  GPIOPINENUM   aePins[ 8 ];
} PINIRQMAP, *PPINIRQMAP;
#define PINIRQMAP_SIZE    sizeof( PINIRQMAP )  

/// define the external interrupt structure
typedef struct _EXTIRQMAP
{
  U8            nPrvCap;
  PU8           pnPinReg;
  GPIOPINENUM   ePin;
} EXTIRQMAP, *PEXTIRQMAP;
#define EXTIRQMAP_SIZE    sizeof( EXTIRQMAP )

// global parameter declarations ----------------------------------------------

// local parameter declarations -----------------------------------------------
static  GPIOCTL   atUserCtls[ GPIO_PIN_ENUM_MAX ];
static  GPIOCTL   atFuncCtls[ GPIO_FNC_ENUM_MAX ];
static  GPIOCTL   atGrpCtls[ GPIO_GRP_ENUM_MAX ];
static  PINIRQMAP atPinIrqMaps[ NUM_PCIRQ_BLKS ];
static  EXTIRQMAP atExtIrqMaps[ GPIO_EXTIRQ_MAX ];

// local function prototypes --------------------------------------------------
static  void  GetRegisters( GPIOPORT ePort, PGPIOCTL ptCtl );
static  void  PinIrqCommonHandler( PPINIRQMAP ptMap, U8 nMaskReg );
static  void  ExtIrqCommonHandler( PEXTIRQMAP ptMap );

/******************************************************************************
 * @function Gpio_Initialize
 *
 * @brief initialize the GPIO handler
 *
 * This function initialized the GPIO handler, configures each pin in the list
 *
 *****************************************************************************/
void Gpio_Initialize( void )
{
  GPIODEF     tDef;
  GPIOGRPDEF  tGrpDef;
  PGPIOCTL    ptCtl;
  U8          nMask, nTemp, nIdx;

  // clear the IRQ pin maps
  for ( nTemp = 0; nTemp < NUM_PCIRQ_BLKS; nTemp++ )
  {
    // for each bit
    for ( nMask = 0; nMask < 8; nMask++ )
    {
      // set an illegal pin
      atPinIrqMaps[ nTemp ].aePins[ nMask ] = GPIO_PIN_ENUM_ILLEGAL;
    }

    // reset the external IRQ maps also
    atExtIrqMaps[ nTemp ].ePin = GPIO_PIN_ENUM_ILLEGAL;
  }

  // for each list in the list
  for( nIdx = 0; nIdx < GPIO_PIN_ENUM_MAX; nIdx++ )
  {
    // get a pointer to the definition structure
    memcpy_P( &tDef, &atGpioPinDefs[ nIdx ], GPIODEF_SIZE );
    ptCtl = &atUserCtls[ nIdx ];

    // get the registers
    GetRegisters(( GPIOPORT )tDef.ePort, ptCtl );
    
    // set the mask
    ptCtl->nMask = BIT( tDef.nPin );
    
    // determine the mode
    switch( tDef.eDir )
    {
      case GPIO_DIR_OUT :
        // set the initial state
        nMask = ( tDef.tFlags.bInitialState ^ tDef.tFlags.bInvert ) ? ptCtl->nMask : 0;
        if ( nMask != 0 )
        {
          // set it
          *( ptCtl->pnPortReg ) |= nMask;
        }
        else
        {
          // clear it
          *( ptCtl->pnPortReg ) &= ~nMask;
        }

        // set the direction
        *( ptCtl->pnDirReg ) |= ptCtl->nMask;
        break;
        
      case GPIO_DIR_IN :
        // clear the direction
        *( ptCtl->pnDirReg ) &= ~ptCtl->nMask;
        
        // determine mode
        if ( tDef.eMode == GPIO_MODE_PULLUP )
        {
          // enable the pullup
          *( ptCtl->pnPortReg ) |= ptCtl->nMask;
        }
        break;
        
      case GPIO_DIR_IO :
        // set the direction based on initial mode
        // clear the direction
        *( ptCtl->pnDirReg ) |= ( tDef.eInitDir == GPIO_DIR_OUT ) ? ptCtl->nMask : 0;

        // determine mode
        if ( tDef.eMode == GPIO_MODE_PULLUP )
        {
          // enable the pullup
          *( ptCtl->pnPortReg ) |= ptCtl->nMask;
        }
        break;
        
      case GPIO_DIR_PINIRQ :
        // clear the direction
        *( ptCtl->pnDirReg ) &= ~ptCtl->nMask;
        
        // determine mode
        if ( tDef.eMode == GPIO_MODE_PULLUP )
        {
          // enable the pullup
          *( ptCtl->pnPortReg ) |= ptCtl->nMask;
        }
        
        // enable the interrupts
        nTemp = tDef.tIrqs.ePinIrq / 8;
        nMask = tDef.tIrqs.ePinIrq % 8;
        atPinIrqMaps[ nTemp ].aePins[ nMask ] = nIdx;
        atPinIrqMaps[ nTemp ].pnPinReg = ptCtl->pnPinReg;
        
        // set the appropriate mask register
        switch( nTemp )
        {
          #ifdef PCINT0_vect
          case 0 :
            PCMSK0 |= ( tDef.tFlags.bEnabled ) ? BIT( nMask ) : 0;
            #ifdef _AVR_IOTN84_H_
            GIMSK |= BIT( PCIE0 );
            #else
            PCICR |= BIT( PCIE0 );
            #endif // _AVR_IOTN84_H_
            // set the previous cap
            atPinIrqMaps[ nTemp ].nPrvCap = *( ptCtl->pnPinReg ) & PCMSK0;
            break;
          #endif // PCINT0_vect
                      
          #ifdef PCINT1_vect
          case 1 :
            PCMSK1 |= ( tDef.tFlags.bEnabled ) ? BIT( nMask ) : 0;
            #ifdef _AVR_IOTN84_H_
            GIMSK |= BIT( PCIE1 );
            #else
            PCICR |= BIT( PCIE1 );
            #endif // _AVR_IOTN84_H_
            
            // set the previous cap
            atPinIrqMaps[ nTemp ].nPrvCap = *( ptCtl->pnPinReg ) & PCMSK1;
            break;
          #endif // PCINT1_vect
                      
          #ifdef PCINT2_vect
            case 2 :
            PCMSK2 |= ( tDef.tFlags.bEnabled ) ? BIT( nMask ) : 0;
            PCICR |= BIT ( PCIE2 );
            
            // set the previous cap
            atPinIrqMaps[ nTemp ].nPrvCap = *( ptCtl->pnPinReg ) & PCMSK2;
            break;
          #endif // PCINT2_vect
                      
          #ifdef PCINT3_vect
            case 3 :
            PCMSK3 |= ( tDef.tFlags.bEnabled ) ? BIT( nMask ) : 0;
            PCICR |= BIT( PCIE3 );
            
            // set the previous cap
            atPinIrqMaps[ nTemp ].nPrvCap = *( ptCtl->pnPinReg ) & PCMSK3;
            break;
          #endif // PCINT3_vect
        }
        break;
        
      case GPIO_DIR_EXTIRQ :
        // clear the direction
        *( ptCtl->pnDirReg ) &= ~ptCtl->nMask;
        
        // determine mode
        if ( tDef.eMode == GPIO_MODE_PULLUP )
        {
          // enable the pullup
          *( ptCtl->pnPortReg ) |= ptCtl->nMask;
        }
  
        // set the maps
        atExtIrqMaps[ tDef.tIrqs.eExtIrq ].ePin = nIdx;
        atExtIrqMaps[ nTemp ].pnPinReg = ptCtl->pnPinReg;

        // determine the external interrupt request register
        switch( tDef.tIrqs.eExtIrq )
        {
          #ifdef EICRA
          case GPIO_EXTIRQ_INT0 :
            EICRA |= BITS( tDef.tEdges.eExtEdge, ISC00 );
            break;
            
          case GPIO_EXTIRQ_INT1 :
            EICRA |= BITS( tDef.tEdges.eExtEdge, ISC10 );
            break;
          
          #ifdef ISC20             
          case GPIO_EXTIRQ_INT2 :
            EICRA |= BITS( tDef.tEdges.eExtEdge, ISC20 );
            break;
          #endif // ISC20
            
          #ifdef ISC30
          case GPIO_EXTIRQ_INT3 :
            EICRA |= BITS( tDef.tEdges.eExtEdge, ISC30 );
            break;
          #endif // ISC30
          #endif // EICRA

          #ifdef EICRB            
          #ifdef ISC40
          case GPIO_EXTIRQ_INT4 :
            EICRB |= BITS( tDef.tEdges.eExtEdge, ISC40 );
            break;
          #endif // ISC40
            
          #ifdef ISC50
          case GPIO_EXTIRQ_INT5 :
            EICRB |= BITS( tDef.tEdges.eExtEdge, ISC50 );
            break;
          #endif // ISC50
            
          #ifdef ISC60
          case GPIO_EXTIRQ_INT6 :
            EICRB |= BITS( tDef.tEdges.eExtEdge, ISC60 );
            break;
          #endif // ISC60
            
          #ifdef ISC70
          case GPIO_EXTIRQ_INT7 :
            EICRB |= BITS( tDef.tEdges.eExtEdge, ISC70 );
            break;
          #endif // ISC70
          #endif // EICRB 

          default :
            break;
        }

        // enable the bit
        #ifdef EIMSK 
        EIMSK |= ( tDef.tFlags.bEnabled ) ? BIT( tDef.tIrqs.eExtIrq ) : 0;
        #else
        #endif // EIMSK
        break;
        
      default :
        break;
    }
  }

  // for each list in the list
  for( nIdx = 0; nIdx < GPIO_FNC_ENUM_MAX; nIdx++ )
  {
    // get a pointer to the definition structure
    memcpy_P( &tDef, &atGpioFncDefs[ nIdx ], GPIODEF_SIZE );
    ptCtl = &atFuncCtls[ nIdx ];

    // get the registers
    GetRegisters(( GPIOPORT )tDef.ePort, ptCtl );
    
    // set the mask
    ptCtl->nMask = BIT( tDef.nPin );
    *( ptCtl->pnDirReg ) |= ptCtl->nMask;
  }

  // for each list in the group list
  for( nIdx = 0; nIdx < GPIO_GRP_ENUM_MAX; nIdx++ )
  {
    // get a pointer to the definition structure
    memcpy_P( &tGrpDef, &atGpioGrpDefs[ nIdx ], GPIOGRPDEF_SIZE );
    ptCtl = &atGrpCtls[ nIdx ];

    // get the registers
    GetRegisters(( GPIOPORT )tGrpDef.ePort, ptCtl );

    // set the mask
    ptCtl->nMask = tGrpDef.nMask;

    // now set the direction in the port
    if ( tGrpDef.eDir == GPIO_DIR_IN )
    {
      // clear the direction pins
      *( ptCtl->pnDirReg ) &= ~ptCtl->nMask;
    }
    else
    {
      // set the direction pins
      *( ptCtl->pnDirReg ) |= ptCtl->nMask;
    }
  }
}

/******************************************************************************
 * @function Gpio_Set
 *
 * @brief set a GPIO pin 
 *
 * This function will set a GPIO pin to a given state
 *
 * @param[in]   eGpioSel  GPIO pin enumeration
 * @param[in]   bState    desired state of the pin
 *
 * @return      GPIOERR   appropriate error if any
 *
 *****************************************************************************/
GPIOERR Gpio_Set( GPIOPINENUM eGpioSel, BOOL bState )
{
  GPIOERR   eError = GPIO_ERR_NONE;
  GPIODEF   tDef;
  PGPIOCTL  ptCtl;
  
  // test for valid GPIO select
  if ( eGpioSel < GPIO_PIN_ENUM_MAX )
  {
    // get a pointer to the definition structure
    memcpy_P( &tDef, &atGpioPinDefs[ eGpioSel ], GPIODEF_SIZE );
    ptCtl = &atUserCtls[ eGpioSel ];
    
    // if this pin is an input - report error
    if ( tDef.eDir == GPIO_DIR_IN )
    {
      // report the error
      eError = GPIO_ERR_ILLMODE;
    }
    else
    {
      // set the state of the pin
      if( bState ^ tDef.tFlags.bInvert )
      {
        // set the bit
        *( ptCtl->pnPortReg ) |= ptCtl->nMask;
      }
      else
      {
        // clear the bit
        *( ptCtl->pnPortReg ) &= ~ptCtl->nMask;
      }
    }
   
  }
  else
  {
    // set the error
    eError = GPIO_ERR_ILLGPIO;
  }
  
  // return the error
  return( eError );
}

/******************************************************************************
 * @function Gpio_Get
 *
 * @brief get a GPIO pin's value
 *
 * This function will return a GPIO's pin value
 *
 * @param[in]   eGpioSel  GPIO pin enumeration
 * @param[io]   pbState   pointer to the strorage element to return the state
 *                        of the GPIO pin
 *
 * @return      GPIOERR   appropriate error if any
 *
 *****************************************************************************/
GPIOERR Gpio_Get( GPIOPINENUM eGpioSel, PBOOL pbState )
{
  GPIOERR   eError = GPIO_ERR_NONE;
  GPIODEF   tDef;
  PGPIOCTL  ptCtl;
  U8        nTemp;

  // test for valid GPIO select
  if ( eGpioSel < GPIO_PIN_ENUM_MAX )
  {
    // get a pointer to the definition structure
    memcpy_P( &tDef, &atGpioPinDefs[ eGpioSel ], GPIODEF_SIZE );

    // get a pointer to the control
    ptCtl = &atUserCtls[ eGpioSel ];
    
    // get the value
    nTemp = *( ptCtl->pnPinReg ) & ptCtl->nMask;
    nTemp ^= ( tDef.tFlags.bInvert ) ? ptCtl->nMask : 0;
    *pbState = ( nTemp != 0 ) ? TRUE : FALSE;
  }
  else
  {
    // set the error
    eError = GPIO_ERR_ILLGPIO;
  }

  // return the error
  return( eError );
}

/******************************************************************************
 * @function Gpio_Toggle
 *
 * @brief toggle a GPIO pin
 *
 * This function will toggle the state of a GPIO pin
 *
 * @param[in]   eGpioSel  GPIO pin enumeration
 *
 * @return      GPIOERR   appropriate error if any
 *
 *****************************************************************************/
GPIOERR Gpio_Toggle( GPIOPINENUM eGpioSel )
{
  GPIOERR   eError = GPIO_ERR_NONE;
  PGPIOCTL  ptCtl;
  
  // test for valid GPIO select
  if ( eGpioSel < GPIO_PIN_ENUM_MAX )
  {
    // get a pointer to the def
    ptCtl = &atUserCtls[ eGpioSel ];

    // toggle the pin
    *( ptCtl->pnPortReg) ^= ptCtl->nMask;
  }
  else
  {
    // set the error
    eError = GPIO_ERR_ILLGPIO;
  }

  // return the error
  return( eError );
}

/******************************************************************************
 * @function Gpio_Ioctl
 *
 * @brief GPIO I/O control
 *
 * This function allows the state, interrupt level, interrupt callback and
 * other parameters to be set/retrieved
 *
 * @param[in]   eGpioSel  GPIO pin enumeration
 * @param[in]   eGpioAct  IOCTL action
 * @param[i0]   pvData    pointer to the data to set/retrieve
 *
 * @return      GPIOERR   appropriate error if any
 *
 *****************************************************************************/
GPIOERR Gpio_Ioctl( GPIOPINENUM eGpioSel, GPIOACT eGpioAct, PVOID pvData )
{
  GPIOERR   eError = GPIO_ERR_NONE;
  GPIODEF   tDef;
  PGPIOCTL  ptCtl;
  GPIODIR   eDir;
  U8        nMask, nGroup;
  BOOL      bState;
  PU8       pnPinMask;
  
  // test for valid GPIO select
  if ( eGpioSel < GPIO_PIN_ENUM_MAX )
  {
    // get a pointer to the definition structure
    memcpy_P( &tDef, &atGpioPinDefs[ eGpioSel ], GPIODEF_SIZE );
    ptCtl = &atUserCtls[ eGpioSel ];
    
    // perform the action
    switch( eGpioAct )
    {
      case GPIO_ACT_SETDIR :
        // now check to ensure pin is an I/O type
        if ( tDef.eDir == GPIO_DIR_IO )
        {
          // convert the argument
          eDir = (*((GPIODIR*)pvData ));

          // determine the mode
          switch( eDir )
          {
            case GPIO_DIR_OUT :
              // set the initial state
              nMask = ( tDef.tFlags.bInitialState ^ tDef.tFlags.bInvert ) ? ptCtl->nMask : 0;
              *( ptCtl->pnPortReg ) |= nMask;

              // set the direction
              *( ptCtl->pnDirReg ) |= ptCtl->nMask;
              break;
              
            case GPIO_DIR_IN :
              // clear the direction
              *( ptCtl->pnDirReg ) &= ~ptCtl->nMask;
              break;
              
            default :
              break;
          } 
        }
        else
        {
          // set the error
          eError = GPIO_ERR_ILLDIR;
        }
        break;
        
      case GPIO_ACT_SETMODE :
        break;
        
      case GPIO_ACT_IRQDSBENB :
        // get the argument
        bState = (*((BOOL*)pvData ));
        
        switch( tDef.eDir )
        {
          case GPIO_DIR_PINIRQ :
            // get the mask/Group
            nMask = tDef.tIrqs.ePinIrq % 8;
            nGroup = tDef.tIrqs.ePinIrq / 8;
            
            switch( nGroup )
            {
              #ifdef PCINT0_vect
              case 0 :
                pnPinMask = ( PU8 )&PCMSK0;
                break;
              #endif // PCINT0_vect
          
              #ifdef PCINT1_vect
              case 1 :
                pnPinMask = ( PU8 )&PCMSK1;
                break;
              #endif // PCINT1_vect
          
              #ifdef PCINT2_vect
              case 2 :
                pnPinMask = ( PU8 )&PCMSK2;
                break;
              #endif // PCINT2_vect
          
              #ifdef PCINT3_vect
              case 3 :
                pnPinMask = ( PU8 )&PCMSK3;
                break;
              #endif // PCINT3_vect
              
              default :
                nMask = 0;
                pnPinMask = NULL;
                break;
            }
            
            // now either enable/disable the interrupt
            if ( pnPinMask != NULL )
            {
              if ( bState )
              {
                // enable the interrupt
                *( pnPinMask ) |= nMask;
              }
              else 
              {
                // disable the interrupt
                *( pnPinMask ) &= ~nMask;
              }             
            }
            break;
            
          case GPIO_DIR_EXTIRQ :
            // now enable/disable
            if ( bState )
            {
              #ifdef EIMSK 
              // enable it
              EIMSK |= BIT( tDef.tIrqs.eExtIrq );
              #else
              #endif // EIMSK
            }
            else
            {
              #ifdef EIMSK
              // disable it
              EIMSK &= ~BIT( tDef.tIrqs.eExtIrq );
              #else
              #endif // EIMSK
            }
            break;
            
          default :
            eError = GPIO_ERR_NOTIRQ;
            break;          
        }
        break;
        
      default :
        // set the error
        eError = GPIO_ERR_ILLACT;
        break;
    }
  }
  else
  {
    // set the error
    eError = GPIO_ERR_ILLGPIO;
  }

  // return the error
  return( eError );
}

/******************************************************************************
 * @function Gpio_Set
 *
 * @brief set a GPIO pin 
 *
 * This function will set a GPIO pin to a given state
 *
 * @param[in]   eGpioSel  GPIO pin enumeration
 * @param[in]   bState    desired state of the pin
 *
 * @return      GPIOERR   appropriate error if any
 *
 *****************************************************************************/
GPIOERR Gpio_SetGroupDirection( GPIOGROUPENUM eGpioGrpSel, GPIODIR eDir )
{
  GPIOERR     eError = GPIO_ERR_NONE;
  PGPIOCTL    ptCtl;

  // is this a valid group
  if ( eGpioGrpSel < GPIO_GRP_ENUM_MAX )
  {
    // set the pointer to the control
    ptCtl = &atGrpCtls[ eGpioGrpSel ];

    // now set the direction in the port
    if ( eDir == GPIO_DIR_IN )
    {
      // clear the direction pins
      *( ptCtl->pnDirReg ) &= ~ptCtl->nMask;
    }
    else
    {
      // set the direction pins
      *( ptCtl->pnDirReg ) |= ptCtl->nMask;
    }
  }
  else
  {
    // set the error
    eError = GPIO_ERR_ILLGROUP;
  }

  // return the error
  return( eError );
}

/******************************************************************************
 * @function Gpio_Set
 *
 * @brief set a GPIO pin 
 *
 * This function will set a GPIO pin to a given state
 *
 * @param[in]   eGpioSel  GPIO pin enumeration
 * @param[in]   bState    desired state of the pin
 *
 * @return      GPIOERR   appropriate error if any
 *
 *****************************************************************************/
GPIOERR Gpio_SetGroup( GPIOGROUPENUM eGpioGrpSel, U8 nData )
{
  GPIOERR     eError = GPIO_ERR_NONE;
  GPIOGRPDEF  tGrpDef;
  PGPIOCTL    ptCtl;

  // is this a valid group
  if ( eGpioGrpSel < GPIO_GRP_ENUM_MAX )
  {
    // get a pointer to the definition structure
    memcpy_P( &tGrpDef, &atGpioGrpDefs[ eGpioGrpSel ], GPIOGRPDEF_SIZE );
    ptCtl = &atGrpCtls[ eGpioGrpSel ];

    // write the value
    *( ptCtl->pnPortReg ) &= ~ptCtl->nMask;
    *( ptCtl->pnPortReg ) |= (( nData << tGrpDef.nShift ) & ptCtl->nMask );
  }
  else
  {
    // set the error
    eError = GPIO_ERR_ILLGROUP;
  }

  // return the error
  return( eError );
}

/******************************************************************************
 * @function Gpio_GetGroup
 *
 * @brief get a GPIO pin's value
 *
 * This function will return a GPIO's pin value
 *
 * @param[in]   eGpioSel  GPIO pin enumeration
 * @param[io]   pbState   pointer to the strorage element to return the state
 *                        of the GPIO pin
 *
 * @return      GPIOERR   appropriate error if any
 *
 *****************************************************************************/
GPIOERR Gpio_GetGroup( GPIOGROUPENUM eGpioGrpSel, PU8 pnData )
{
  GPIOERR     eError = GPIO_ERR_NONE;
  GPIOGRPDEF  tGrpDef;
  PGPIOCTL    ptCtl;
  U8          nTemp;

  // is this a valid group
  if ( eGpioGrpSel < GPIO_GRP_ENUM_MAX )
  {
    // get a pointer to the definition structure
    memcpy_P( &tGrpDef, &atGpioGrpDefs[ eGpioGrpSel ], GPIOGRPDEF_SIZE );
    ptCtl = &atGrpCtls[ eGpioGrpSel ];

    // write the value
    nTemp = *( ptCtl->pnPinReg );

    // shift and mask it
    *( pnData ) = ( nTemp & ptCtl->nMask ) >> tGrpDef.nShift;
  }
  else
  {
    // set the error
    eError = GPIO_ERR_ILLGROUP;
  }

  // return the error
  return( eError );
}

/******************************************************************************
 * @function Gpio_Refresh
 *
 * @brief refresh the state of all the GPIO pins
 *
 * This function sell update the direction and current state of all GPIO pins
 *
  *****************************************************************************/
void Gpio_Refresh( void )
{
}

#ifdef PCINT0_vect
/******************************************************************************
 * @function PCINT0_vect
 *
 * @brief PCINT0 interrupt vector
 *
 * This provides the interrupt handler for PCINt0
 *
 *****************************************************************************/
ISR( PCINT0_vect )
{
  // call the common handler
  PinIrqCommonHandler( &atPinIrqMaps[ 0 ], PCMSK0 );
}
#endif

#ifdef PCINT1_vect
/******************************************************************************
 * @function PCINT1_vect
 *
 * @brief PCINT1 interrupt vector
 *
 * This provides the interrupt handler for PCINt1
 *
 *****************************************************************************/
ISR( PCINT1_vect )
{
  // call the common handler
  PinIrqCommonHandler( &atPinIrqMaps[ 1 ], PCMSK1 );
}
#endif

#ifdef PCINT2_vect
/******************************************************************************
 * @function PCINT2_vect
 *
 * @brief PCINT2 interrupt vector
 *
 * This provides the interrupt handler for PCINt2
 *
 *****************************************************************************/
ISR( PCINT2_vect )
{
  // call the common handler
  PinIrqCommonHandler( &atPinIrqMaps[ 2 ], PCMSK2 );
}
#endif

#ifdef PCINT3_vect
/******************************************************************************
 * @function PCINT3_vect
 *
 * @brief PCINT3 interrupt vector
 *
 * This provides the interrupt handler for PCINt3
 *
 *****************************************************************************/
ISR( PCINT3_vect )
{
  // call the common handler
  PinIrqCommonHandler( &atPinIrqMaps[ 3 ], PCMSK3 );
}
#endif

#ifdef INT0_vect
/******************************************************************************
 * @function INT0_vect
 *
 * @brief INT0 interrupt vector
 *
 * This provides the interrupt handler for INT0
 *
 *****************************************************************************/
ISR( INT0_vect )
{
  // call the common handler
  ExtIrqCommonHandler( &atExtIrqMaps[ 0 ] );
}
#endif

#ifdef INT1_vect
/******************************************************************************
 * @function INT1_vect
 *
 * @brief INT1 interrupt vector
 *
 * This provides the interrupt handler for INT1
 *
 *****************************************************************************/
ISR( INT1_vect )
{
  // call the common handler
  ExtIrqCommonHandler( &atExtIrqMaps[ 1 ] );
}
#endif

#ifdef INT2_vect
/******************************************************************************
 * @function INT2_vect
 *
 * @brief INT2 interrupt vector
 *
 * This provides the interrupt handler for INT2
 *
 *****************************************************************************/
ISR( INT2_vect )
{
  // call the common handler
  ExtIrqCommonHandler( &atExtIrqMaps[ 2 ] );
}
#endif

#ifdef INT3_vect
/******************************************************************************
 * @function INT3_vect
 *
 * @brief INT3 interrupt vector
 *
 * This provides the interrupt handler for INT3
 *
 *****************************************************************************/
ISR( INT3_vect )
{
  // call the common handler
  ExtIrqCommonHandler( &atExtIrqMaps[ 3 ] );
}
#endif

#ifdef INT4_vect
/******************************************************************************
 * @function INT4_vect
 *
 * @brief INT4 interrupt vector
 *
 * This provides the interrupt handler for INT4
 *
 *****************************************************************************/
ISR( INT4_vect )
{
  // call the common handler
  ExtIrqCommonHandler( &atExtIrqMaps[ 4 ] );
}
#endif

#ifdef INT5_vect
/******************************************************************************
 * @function INT5_vect
 *
 * @brief INT5 interrupt vector
 *
 * This provides the interrupt handler for INT5
 *
 *****************************************************************************/
ISR( INT5_vect )
{
  // call the common handler
  ExtIrqCommonHandler( &atExtIrqMaps[ 5 ] );
}
#endif

#ifdef INT6_vect
/******************************************************************************
 * @function INT6_vect
 *
 * @brief INT6 interrupt vector
 *
 * This provides the interrupt handler for INT6
 *
 *****************************************************************************/
ISR( INT6_vect )
{
  // call the common handler
  ExtIrqCommonHandler( &atExtIrqMaps[ 6 ] );
}
#endif

#ifdef INT7_vect
/******************************************************************************
 * @function INT7_vect
 *
 * @brief INT7 interrupt vector
 *
 * This provides the interrupt handler for INT7
 *
 *****************************************************************************/
ISR( INT7_vect )
{
  // call the common handler
  ExtIrqCommonHandler( &atExtIrqMaps[ 7 ] );
}
#endif

/******************************************************************************
 * @function PinIrqCommandHandler
 *
 * @brief pin change common IRQ handler
 *
 * This function processes pin interrupts
 *
 * @param[in]   ptMap     pointer to the correct MAP structure
 * @param[in]   nMaskReg  mask register value
 *
 *****************************************************************************/
static void PinIrqCommonHandler( PPINIRQMAP ptMap, U8 nMaskReg )
{
  PGPIODEF          ptDef;
  U8                nDelta, nBit, nCurCap, nMask;
  PVGPIOIRQCALLBACK pvCallback;
  GPIOPINENUM       ePin;
  GPIOPINIRQEDGE    eEdge;
  GPIOIRQCBEVENT    eEvent = GPIO_IRQCB_EVENT_NONE;
  
  // compute the bit change/update the old
  nCurCap = *( ptMap->pnPinReg ) & nMaskReg;
  nDelta = ( nCurCap ) ^ ptMap->nPrvCap;
  
  // now determine the bit
  for ( nBit = 0; nBit < 8; nBit++ )
  {
    // check to see if pin has a change
    if (( nMask = BIT( nBit ) & nDelta ))
    {
      // check to see if this a valid pin
      if (( ePin = ptMap->aePins[ nBit ] ) != GPIO_PIN_ENUM_ILLEGAL )
      {
        //  get the control/defs
        ptDef = ( PGPIODEF )&atGpioPinDefs[ ePin ];
        eEdge = ( GPIOPINIRQEDGE )PGM_RDBYTE( ptDef->tEdges.ePinEdge );
    
        // check for a change on this pin
        if (( nCurCap & nMask ) && (( eEdge == GPIO_PINEDGE_RISE ) || ( eEdge == GPIO_PINEDGE_BOTH )))
        {
          // set the rise event
          eEvent = GPIO_IRQCB_EVENT_RISE;
        }
        else if ( !( nCurCap & nMask ) && (( eEdge == GPIO_PINEDGE_FALL ) || ( eEdge == GPIO_PINEDGE_BOTH )))
        {
          // set the fall event
          eEvent = GPIO_IRQCB_EVENT_FALL;
        }
        else
        {
          // set no event
          eEvent = GPIO_IRQCB_EVENT_NONE;
        }

        // determine if there is an event
        if ( eEvent != GPIO_IRQCB_EVENT_NONE )
        {
          // get the callback
          pvCallback = ( PVGPIOIRQCALLBACK )PGM_RDWORD( ptDef->pvCallback );
          if ( pvCallback != NULL )
          {
            // call the function
            pvCallback( ePin, eEvent );
          }
        }
      }
    }
  }
  
  // copy the old to the new
  ptMap->nPrvCap = nCurCap;
}

/******************************************************************************
 * @function ExtIrqCommandHandler
 *
 * @brief external common IRQ handler
 *
 * This function processes external interrupts
 *
 * @param[in]   ptMap     pointer to the correct MAP structure
 *
 *****************************************************************************/
static void ExtIrqCommonHandler( PEXTIRQMAP ptMap )
{
  PGPIODEF          ptDef;
  U8                nCurCap, nMask;
  PVGPIOIRQCALLBACK pvCallback;
  GPIOEXTIRQEDGE    eEdge;
  GPIOIRQCBEVENT    eEvent = GPIO_IRQCB_EVENT_NONE;
  
  // get a pointer to the def/get the edge/set the mask
  ptDef = ( PGPIODEF )&atGpioPinDefs[ ptMap->ePin ];
  eEdge = ( GPIOEXTIRQEDGE )PGM_RDBYTE( ptDef->tEdges.eExtEdge );
  nMask = BIT( PGM_RDBYTE( ptDef->nPin ));
  nCurCap = *( ptMap->pnPinReg ) & nMask;

  // special processing on level and both edges
  switch( eEdge )
  {
    case GPIO_EXTEDGE_LVL :
      // set the event
      eEvent = GPIO_IRQCB_EVENT_LEVEL;        
      break;
      
    case GPIO_EXTEDGE_BOTH :
      // set the event based on state of pin
      eEvent = ( nCurCap == TRUE ) ? GPIO_IRQCB_EVENT_RISE : GPIO_IRQCB_EVENT_FALL;
      break;
      
    case GPIO_EXTEDGE_FALL :
      // set the falling edge event
      eEvent = GPIO_IRQCB_EVENT_FALL;
      break;
      
    case GPIO_EXTEDGE_RISE :
      // set the rising edge event
      eEvent = GPIO_IRQCB_EVENT_RISE;
      break;
      
    default :
      break;
  }

  // determine if there is an event
  if ( eEvent != GPIO_IRQCB_EVENT_NONE )
  {
    // get the callback
    pvCallback = ( PVGPIOIRQCALLBACK )PGM_RDWORD( ptDef->pvCallback );
    if ( pvCallback != NULL )
    {
      // call the function
      pvCallback( ptMap->ePin, eEvent );
    }
  }  
}

/******************************************************************************
 * @function GetRegisters
 *
 * @brief get the registers
 *
 * This function sets the port addresses in the control structure
 *
 * @param[in]   ePort     port
 * @param[io]   ptCtl     pointer to the control structure
 *
 *****************************************************************************/
static void GetRegisters( GPIOPORT ePort, PGPIOCTL ptCtl )
{
  // set the controls
  switch( ePort )
  {
    #ifdef PORTA
    case GPIO_PORT_A :
      ptCtl->pnPortReg = ( PU8 )&PORTA;
      ptCtl->pnDirReg = ( PU8 )&DDRA;
      ptCtl->pnPinReg = ( PU8 )&PINA;
      break;
    #endif  // PORTA

    #ifdef PORTB
    case GPIO_PORT_B :
      ptCtl->pnPortReg = ( PU8 )&PORTB;
      ptCtl->pnDirReg = ( PU8 )&DDRB;
      ptCtl->pnPinReg = ( PU8 )&PINB;
      break;
    #endif  // PORTB

    #ifdef PORTC
    case GPIO_PORT_C :
      ptCtl->pnPortReg = ( PU8 )&PORTC;
      ptCtl->pnDirReg = ( PU8 )&DDRC;
      ptCtl->pnPinReg = ( PU8 )&PINC;
      break;  
    #endif  // PORTC

    #ifdef PORTD
    case GPIO_PORT_D :
      ptCtl->pnPortReg = ( PU8 )&PORTD;
      ptCtl->pnDirReg = ( PU8 )&DDRD;
      ptCtl->pnPinReg = ( PU8 )&PIND;
      break;
    #endif  // PORTD

    #ifdef PORTE
    case GPIO_PORT_E :
      ptCtl->pnPortReg = ( PU8 )&PORTE;
      ptCtl->pnDirReg = ( PU8 )&DDRE;
      ptCtl->pnPinReg = ( PU8 )&PINE;
      break;
    #endif  // PORTE

    #ifdef PORTF
    case GPIO_PORT_F :
      ptCtl->pnPortReg = ( PU8 )&PORTF;
      ptCtl->pnDirReg = ( PU8 )&DDRF;
      ptCtl->pnPinReg = ( PU8 )&PINF;
      break;
    #endif  // PORTF

    #ifdef PORTG
    case GPIO_PORT_G :
      ptCtl->pnPortReg = ( PU8 )&PORTG;
      ptCtl->pnDirReg = ( PU8 )&DDRG;
      ptCtl->pnPinReg = ( PU8 )&PING;
      break;
    #endif  // PORTG

    #ifdef PORTH
    case GPIO_PORT_H :
      ptCtl->pnPortReg = ( PU8 )&PORTH;
      ptCtl->pnDirReg = ( PU8 )&DDRH;
      ptCtl->pnPinReg = ( PU8 )&PINH;
      break;
    #endif  // PORTH

    #ifdef PORTJ
    case GPIO_PORT_J :
      ptCtl->pnPortReg = ( PU8 )&PORTJ;
      ptCtl->pnDirReg = ( PU8 )&DDRJ;
      ptCtl->pnPinReg = ( PU8 )&PINJ;
      break;
    #endif  // PORTJ

    #ifdef PORTK
    case GPIO_PORT_K :
      ptCtl->pnPortReg = ( PU8 )&PORTK;
      ptCtl->pnDirReg = ( PU8 )&DDRK;
      ptCtl->pnPinReg = ( PU8 )&PINK;
      break;
    #endif  // PORTK
      
    default :
      break;
  }
}

/**@} EOF Gpio.c */
