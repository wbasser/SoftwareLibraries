/******************************************************************************
 * @file A2D.c
 *
 * @brief A2D implementation
 *
 * This file provides the implemntation for the A2D peripherals
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
 * \addtogroup A2D
 * @{
 *****************************************************************************/

// system includes ------------------------------------------------------------

// library includes -----------------------------------------------------------

// local includes -------------------------------------------------------------
#include "A2D/A2d.h"
#include "Interrupt/IrqManager.h"

// Macros and Defines ---------------------------------------------------------
/// define the sequence number
#define A2D_SEQ_NUM  3

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations ----------------------------------------------

// local parameter declarations -----------------------------------------------

// local function prototypes --------------------------------------------------

// constant parameter initializations -----------------------------------------

/******************************************************************************
 * @function A2d_Initialize
 *
 * @brief A2D initialization
 *
 * This function will initialize a given device 
 *
 *****************************************************************************/
void A2d_Initialize( void );
{
  PA2DDEF   ptDef;
  A2DDEVENUM eDev;
  
  // for each item in the table
  for ( eDev  = 0; eDev < A2D_DEV_MAX; eDev++ )
  {
    // get pointer to the definition structure
    ptDef = ( PA2DDEF )&atA2dDefs[ eDev ];
    
    // initialize the A2D
    MAP_SysCtlPeripheralEnable( ptDef->uDevPeriph );
    
    // test for temperature
    if ( ptDef->nChannel != ADC_CTL_TS )
    {
      // GPIO peripherals/initialize the normal GPIO pins as A2DS
      MAP_SysCtlPeripheralEnable( ptDef->uGpioPeriph );
      MAP_GPIOPinTypeA2D( ptDef->uGpioBase, ptDef->nPin );
    }
    
    // clear the sequence three interrupt
    MAP_ADCIntClear( ptDef->uDevBase, A2D_SEQ_NUM );
  }
}

/******************************************************************************
 * @function A2d_Convert
 *
 * @brief conert an A2D channel
 *
 * This function will configure the sequence, start a conversion, wait for the
 * result and return the value
 *
 * @param[in]   eDev        Device enumeration
 * @param[in]   pwData      pointer to the data
 *
 * @return      appropriate error value
 *
 *****************************************************************************/
A2DERR A2d_Convert( A2DDEVENUM eDev, PU16 pwData )
{
  A2DERR 	  eError = A2D_ERR_NONE;
  PA2DDEF 	ptDef;

  // check for a valid A2D
  if ( eDev < A2D_DEV_MAX )
  {
    // get pointer to the definition structure
    ptDef = ( PA2DDEF )&atA2dDefs[ eDev ];
    
    // configure sequence/sequence step/enable
    MAP_ADCSequenceConfigure( ptDef->uDevBase, A2D_SEQ_NUM, ADC_TRIGGER_PROCESSOR, 0 );
    MAP_ADCSequenceStepConfigure( ptDef->uDevBase, A2D_SEQ_NUM, 0, ( ptDef->nChannel | ADC_CTL_IE | ADC_CTL_END )); 
    MAP_ADCSequenceEnable( ptDef->uDevBase, A2D_SEQ_NUM );
    
    // Trigger the ADC conversion.
    MAP_ADCProcessorTrigger( ptDef->uDevBase, A2D_SEQ_NUM );

    // Wait for conversion to be completed.
    while( !MAP_ADCIntStatus( ptDef->uDevBase, A2D_SEQ_NUM, FALSE ));

    // Clear the ADC interrupt flag.
    MAP_ADCIntClear( ptDef->uDevBase, A2D_SEQ_NUM );

    // Read ADC Value.
    MAP_ADCSequenceDataGet( ptDef->uDevBase, A2D_SEQ_NUM, pwData );
  }
  else
  {
    // illegal device
    eError = A2D_ERR_ILLDEV;
  }

  // return the status
  return(( A2DERR )eError );
}

/******************************************************************************
 * @function A2d_Ioctl
 *
 * @brief A2D IOCTL functions
 *
 * This function provides functionality to modify the A2Ds parameters
 *
 * @param[in]   eDev        Device enumeration
 * @param[in]   eAction     action to take
 * @param[io]   pvData      pointer to data storage/retrieval
 *
 * @return      appropriate error value
 *
 *****************************************************************************/
A2DERR A2d_Ioctl( A2DDEVENUM eDev, A2DACTION eAction, PVOID pvData )
{
  A2DERR eError = A2D_ERR_NONE;
  
  // check for a valid A2D
  if ( eDev < A2D_DEV_MAX )
  {
    // process the action
    switch( eAction )
    {
      default :
        // illegal action
        eError = A2D_ERR_ILLACT;
        break;
    }
  }
  else
  {
    // illegal device
    eError = A2D_ERR_ILLDEV;
  }
  
  // return the error
  return( eError );
}

/******************************************************************************
 * @function A2d_Close
 *
 * @brief Close the A2D
 *
 * This function will 
 *
 *****************************************************************************/
void A2d_Close( void )
{
}

/**@} EOF A2d.c */
