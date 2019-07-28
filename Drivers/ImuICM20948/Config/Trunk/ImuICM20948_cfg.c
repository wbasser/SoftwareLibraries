/******************************************************************************
 * @file ImuICM20948_cfg.c
 *
 * @brief IMU ICM20948 configuration implementations
 *
 * This file provides the configuration implementation for the IMU ICM-20948
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
 * \addtogroup ImuICM20948
 * @{
 *****************************************************************************/

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "ImuICM20948/ImuICM20948.h"

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations ----------------------------------------------

// local parameter declarations -----------------------------------------------

// local function prototypes --------------------------------------------------

// constant parameter initializations -----------------------------------------

#if ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_TASKMANAGER )
/******************************************************************************
 * @function ImuICM20948_ProcessCallback
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
BOOL ImuICM20948_ProcessCallback ( TASKARG xArg )
{
  // call the base event
  ImuICM20948_ProcessIrq( );
}
#endif // ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_TASKMANAGER )

/******************************************************************************
 * @function ImuICM20948_ProcessIrqCallback
 *
 * @brief IRQ callback
 *
 * This function will process the IRQ callback
 *
 * @param[in]   nIrq      IRQ enumeration
 * @param[in]   bState    state of the pin
 *
 *****************************************************************************/
void ImuICM20948_ProcessIrqCallback( U8 nIrq, BOOL bState )
{
  #if ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_TASKMANAGER )
  // post the event
  TaskManager_PostIrqEvent( SENBMP388_DATAREADY_TASKENUM, SENBMP388_DATAREADY_EVENT );
  #endif // ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_TASKMANAGER )
}

/******************************************************************************
 * @function ImuICM20948_ReadRegisters
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
BOOL ImuICM20948_ReadRegisters( U8 nBaseReg, PU8 pnData, U8 nLength )
{
  BOOL bStatus = FALSE;
  
  // return the status
  return( bStatus );
}

/******************************************************************************
 * @function ImuICM20948_WriteRegisters
 *
 * @brief write registers from the device
 *
 * This function will write the registers from the device per the established
 * interface
 *
 * @param[in]   nBaseReg        base register
 * @param[in]   pnData          pointer to the data
 * @param[in]   nLength         length of the data
 *
 * @return      TRUE if no errors, FALSE otherwise
 *
 *****************************************************************************/
BOOL ImuICM20948_WriteRegisters( U8 nBaseReg, PU8 pnData, U8 nLength )
{
  BOOL bStatus = FALSE;
  
  // return the status
  return( bStatus );
}

/**@} EOF ImuICM20948_cfg.c */
