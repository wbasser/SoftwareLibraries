/******************************************************************************
 * @file VSK10xxHandler.h
 *
 * @brief VSK 10xx handler 
 *
 * This file 
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
 * \addtogroup VSK10XXHandler
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _VSK10XXHANDLER_H
#define _VSK10XXHANDLER_H

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------

// library includes -----------------------------------------------------------
#include "Types/Types.h"

// Macros and Defines ---------------------------------------------------------
/// define the minimum/maximum volume
#define VSK10XXHANDLER_MIN_VOL      ( 254 )
#define VSK10XXHANDLER_MAX_VOL      ( 0 )

/// define the buffer size for a block of data
#define VSK10XXHANDLER_BUF_SIZE     ( 32 )

// enumerations ---------------------------------------------------------------
typedef enum _VSK10XXREG
{
  VSK10XX_REG_MODE = 0,
  VSK10XX_REG_STAT,
  VSK10XX_REG_BASS,
  VSK10XX_REG_CLKF,
  VSK10XX_REG_DTIM,
  VSK10XX_REG_AUDT,
  VSK10XX_REG_WRAM,
  VSK10XX_REG_ADDR,
  VSK10XX_REG_HDT0,
  VSK10XX_REG_HDT1,
  VSK10XX_REG_AIAR,
  VSK10XX_REG_VOLM,
  VSK10XX_REG_AIC0,
  VSK10XX_REG_AIC1,
  VSK10XX_REG_AIC2,
  VSK10XX_REG_AIC3,
  VSK10XX_REG_MAX
} VSK10XXREG;

// structures -----------------------------------------------------------------

// global parameter declarations -----------------------------------------------

// global function prototypes --------------------------------------------------
extern  void  VSK10XXHandler_Initialize( void );
extern  void  VSK10XXHandler_Reset( void );
extern  void  VSK10XXHandler_StartPlay( void );
extern  void  VSK10XXHandler_WriteData( PU8 pnData, U8 nLength );
extern  void  VSK10XXHandler_SetVolume( U8 nLeft, U8 nRight );
extern  void  VSK10XXHandler_SetEnhance( U8 nTreble, U8 nTrbCut, U8 nBass, U8 nBasCut );
extern  BOOL  VSK10XXHandler_DataReady( void );
extern  void  VSK10XXHandler_GenerateTone( BOOL bOffOn );
extern  U16   VSK10XXHandler_ReadRegister( VSK10XXREG eReg );

/**@} EOF VSK10XXHandler.h */

#endif  // _VSK10XXHANDLER_H