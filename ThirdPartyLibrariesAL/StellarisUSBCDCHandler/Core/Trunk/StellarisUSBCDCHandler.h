/******************************************************************************
 * @file StellarisUSBCDCHandler.h
 *
 * @brief USB CDC handler declarations
 *
 * This file provides the USB CDC handler declarations
 *
 * @copyright Copyright (c) 2012 Cyber Intergration
 * This document contains proprietary data and information of Endurance Products 
 * LLC. It is the eStellarisUSBCDCHandlerclusive property of Endurance Products, LLC and will not be 
 * disclosed in any form to any party without prior written permission of 
 * Endurance Products, LLC. This document may not be reproduced or further used 
 * without the prior written permission of Endurance Products, LLC.
 *
 * Version History
 * ======
 * $Log: $
 * 
 *
 * \addtogroup StellarisUSBCDCHandler
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _StellarisUSBCDCHandler_H
#define _StellarisUSBCDCHandler_H

// local includes -------------------------------------------------------------
#include "StellarisUSBCDCHandler/StellarisUSBCDCHandler_cfg.h"

// global function prototypes --------------------------------------------------
extern  void  StellarisUSBCDCHandler_Initialize( void );
extern  void  StellarisUSBCDCHandler_PutChar( U8 nData );
extern  void  StellarisUSBCDCHandler_Write( PU8 pcData, U8 nLength );
extern  void  StellarisUSBCDCHandler_Printf( const char *pcString, ...);

/**@} EOF StellarisUSBCDCHandler.h */

#endif  // _StellarisUSBCDCHandler_H