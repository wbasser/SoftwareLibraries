/******************************************************************************
 * @file SenBMP388_cfg.c
 *
 * @brief Bosch Sensortech BMP388 configuration implementation
 *
 * This file provides the implementation for the BMP388 configuration
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
 * \addtogroup SenBMP388
 * @{
 *****************************************************************************/

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "SenBMP388/SenBMP388.h"

// library includes -----------------------------------------------------------
//#Include "I2C/I2c.h"

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations ----------------------------------------------

// local parameter declarations -----------------------------------------------

// local function prototypes --------------------------------------------------

// constant parameter initializations -----------------------------------------
const CODE SENBMP388CONFIG  g_tSenBMP388Config =
{
  // populate this structure using the helper macro
  // SENBMP388CONFIGM( mode, presssamprate, tempsamprate, datarate, filtcoeef, irqpin ) 
  SENBMP388CONFIGM( SENBMP388_MODE_NONBLKNORMAL, SENBMP388_OVER_SAMPRATE_32, SENBMP388_OVER_SAMPRATE_32, SENBMP388_OUTPUT_DATRATE_40MSEC, SENBMP388_FILTCOEEF_127, SENBMP388_IRQDEF_ODLO ) 
};

#if ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_TASKMANAGER )
/******************************************************************************
 * @function SenBMP388_ProcessCallback
 *
 * @brief process task event
 *
 * This function will process the task event
 *
 * @param[in]   xArg      task argument
 *
 * @return      TRUE to flush the event
 *
 *****************************************************************************/
BOOL SenBMP388_ProcessCallback ( TASKARG xArg )
{
  // call the base event
  SenBMP388_ProcessDataReady( );
}
#endif // ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_TASKMANAGER )

/******************************************************************************
 * @function SenBMP388_ProcessIrqCallback
 *
 * @brief IRQ callback
 *
 * This function will process the IRQ callback
 *
 * @param[in]   nIrq      IRQ enumeration
 * @param[in]   bState    state of the pin
 *
 *****************************************************************************/
void SenBMP388_ProcessIrqCallback( U8 nIrq, BOOL bState )
{
  #if ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_TASKMANAGER )
  // post the event
  TaskManager_PostEventIrq( SENBMP388_DATAREADY_TASKENUM, SENBMP388_DATAREADY_EVENT );
  #endif // ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_TASKMANAGER )
}

/******************************************************************************
 * @function SenBMP388_ReadRegisters
 *
 * @brief read registers from the device
 *
 * This function will read the registers from the device per the established
 * interface
 *
 * @param[in]   nBaseReg        base register
 * @param[in]   pnData          pointer to the data
 * @param[in]   nLength         length of the data
 *
 * @return      TRUE if no errors, FALSE otherwise
 *
 *****************************************************************************/
BOOL SenBMP388_Read( U8 nBaseReg, PU8 pnData, U8 nLength )
{
  BOOL bStatus = FALSE;
  
  // return the status
  return( bStatus );
}

/******************************************************************************
 * @function SenBMP388_WriteRegister
 *
 * @brief write registers from the device
 *
 * This function will write the registers from the device per the established
 * interface
 *
 * @param[in]   nBaseReg        base register
 * @param[in]   pnData          pointer to the data
 *
 * @return      TRUE if no errors, FALSE otherwise
 *
 *****************************************************************************/
BOOL SenBMP388_Write( U8 nBaseReg, PU8 pnData )
{
  BOOL bStatus = FALSE;
  
  // return the status
  return( bStatus );
}

/******************************************************************************
 * @function SenBMP388_DelayMsec
 *
 * @brief delay for some milliseconds
 *
 * This function will call the appropriae delay routine
*
 * @param[in]   wDelay    delay in milliseconds
*
 *****************************************************************************/
void SenBMP388_DelayMsec( U16 wDelay )
{
}

/**@} EOF SenBMP388_cfg.c */
