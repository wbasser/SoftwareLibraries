/****************************************7**************************************
 * @file  DALIMessageHandler_cfg.h
 *
 * @brief  DALI message handler declarations
 *
 * This file provides the declarations for the DALI message handler
 *
 * @copyright Copyright (c) 2012 Nan Technologies
 * This document contains proprietary data and information of Nan Technologies
 * LLC. It is the exclusive property of Nan Technologies, LLC and will not be
 * disclosed in any form to any party without prior written permission of
 * Nan Technologies, LLC. This document may not be reproduced or further used
 * without the prior written permission of Nan Technologies, LLC.
 *
 * Version History
 * ======
 * $Log: $
 *
 *
 * \addtogroup  DALIMessageHandler
 * @{
 *****************************************************************************/

// ensure only one instantiation
#ifndef _DALIMESSAGEHANDLER_CFG_H
#define _DALIMESSAGEHANDLER_CFG_H

// system includes ------------------------------------------------------------

// library includes -----------------------------------------------------------

// local includes -------------------------------------------------------------
#include "DALIMessageHandler/DALIMessageHandler_def.h"

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations -----------------------------------------------

// global function prototypes --------------------------------------------------
extern  void  DALIMessageHandler_SetLedLevel( U8 nTblSel, U8 nLevel );
extern  BOOL  DALIMessageHandler_GetLedStatus( void );
extern  BOOL  DALIMessageHandler_GetEOLTestMode( void );
extern  void  DALIMessageHandler_SeedRandom( U32 uValue );
extern  U32   DALIMessageHandler_GenerateRandom(( void );

/**@} EOF DALIMessageHandler_cfg.h */
#endif  // _DALIMESSAGEHANDLER_CFG_H
