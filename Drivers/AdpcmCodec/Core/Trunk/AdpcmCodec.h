/******************************************************************************
 * @file AdpcmCodec.h
 *
 * @brief ADPCM encoder/decoder handler 
 *
 * This file provides the declarations for the ADPM encoder/decoder
 *
 * @copyright Copyright (c) 2012 CyberIntegration
 * This document contains proprietary data and information of CyberIntegration 
 * LLC. It is the exclusive property of CyberIntegration, LLC and will not be 
 * disclosed in any form to any party without prior written permission of 
 *CyberIntegration, LLC. This document may not be reproduced or further used 
 * without the prior written permission of CyberIntegration, LLC.
 *
 * $Date: $
 *
 * Version History
 * ======
 * $Rev: $
 * 
 *
 * \addtogroup AdpcmCodec
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _ADPCMCODEC_H
#define _ADPCMCODEC_H

// system includes ------------------------------------------------------------
#include "Types/Types.h"

// local includes -------------------------------------------------------------

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------
// define the ADPCM control structure
typedef	struct _ADPCMCTL
{
	S32		lPrevSample;
	C8		cPrevIndex;
} ADPCMCTL, *PADPCMCTL;
#define	ADPCMCTL_SIZE	              sizeof( ADPCMCTL )

// global parameter declarations -----------------------------------------------

// global function prototypes --------------------------------------------------
extern	void	AdpcmCodec_Initialize( void );
extern	C8		AdpcmCodec_Encode( PADPCMCTL ptCtl, S16 iValue );
extern	S16		AdpcmCodec_Decode( PADPCMCTL ptCtl, C8 cValue );

/**@} EOF AdpcmCodec.h */

#endif  // _ADPCMCODEC_H
