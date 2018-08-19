/****************************************7**************************************
 * @file  DALIProtocolHandler_prm.h
 *
 * @brief  DALI message handler declarations
 *
 * This file provides the declarations for the DALI message handler
 *
 * @copyright Copyright (c) 2012 CyberIntegration
 * This document contains proprietary data and information of CyberIntegration
 * LLC. It is the exclusive property of CyberIntegration, LLC and will not be
 * disclosed in any form to any party without prior written permission of
 * CyberIntegration, LLC. This document may not be reproduced or further used
 * without the prior written permission of CyberIntegration, LLC.
 *
 * Version History
 * ======
 * $Rev: $
 *
 *
 * \addtogroup  DALIProtocolHandler
 * @{
 *****************************************************************************/

// ensure only one instantiation
#ifndef _DALIPROTOCOLHANDLER_PRM_H
#define _DALIPROTOCOLHANDLER_PRM_H

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------
/// define the DALI version number
#define	DALI_VERSION_NUMBER               ( 1 )

/// define the physical minimum level
#define	DALI_PHYS_MINLEVEL                ( 25 )

/// define the enable debug macro
#define DALI_ENABLE_DEBUG                 ( 1 )

/// define the delay time between forward frame/backward frame
#define DALI_FORWARD_BACKWARD_DELAY_TIME  ( 4 )

/// define the parameter enumerations
#define DALI_PARAM_SHORT_ADDR             ( 0 )
#define DALI_PARAM_POWERON_LEVEL          ( 0 )
#define DALI_PARAM_SYSFAIL_LEVEL          ( 0 )
#define DALI_PARAM_MIN_LEVEL              ( 0 )
#define DALI_PARAM_MAX_LEVEL              ( 0 )
#define DALI_PARAM_LASTREQ_LEVEL          ( 0 )
#define DALI_PARAM_FADE_RATE              ( 0 )
#define DALI_PARAM_FADE_TIME              ( 0 )
#define DALI_PARAM_RANDOM_ADDR_H          ( 0 )
#define DALI_PARAM_RANDOM_ADDR_M          ( 0 )
#define DALI_PARAM_RANDOM_ADDR_L          ( 0 )
#define DALI_PARAM_GROUP0007              ( 0 )
#define DALI_PARAM_GROUP0815              ( 0 )
#define DALI_PARAM_SCENE00                ( 0 )
#define DALI_PARAM_SCENE01                ( 0 )
#define DALI_PARAM_SCENE02                ( 0 )
#define DALI_PARAM_SCENE03                ( 0 )
#define DALI_PARAM_SCENE04                ( 0 )
#define DALI_PARAM_SCENE05                ( 0 )
#define DALI_PARAM_SCENE06                ( 0 )
#define DALI_PARAM_SCENE07                ( 0 )
#define DALI_PARAM_SCENE08                ( 0 )
#define DALI_PARAM_SCENE09                ( 0 )
#define DALI_PARAM_SCENE10                ( 0 )
#define DALI_PARAM_SCENE11                ( 0 )
#define DALI_PARAM_SCENE12                ( 0 )
#define DALI_PARAM_SCENE13                ( 0 )
#define DALI_PARAM_SCENE14                ( 0 )
#define DALI_PARAM_SCENE15                ( 0 )
#define DALI_PARAM_FAILURESTATUS          ( 0 )
#define DALI_PARAM_OPMODE                 ( 0 )
#define DALI_PARAM_FASTFADETIME           ( 0 )
#define DALI_PARAM_MB0_MINFASTFADETIME    ( 0 )
#define DALI_PARAM_MB0_GEARTYPE           ( 0 )
#define DALI_PARAM_MB0_POSSOPTYPES        ( 0 )
#define DALI_PARAM_MB0_FEATURES           ( 0 )
#define DALI_PARAM_MB0_EXTVERNUM          ( 0 )
#define DALI_PARAM_MB0_DEVTYPE            ( 0 )
#define DALI_PARAM_MB0_PHYSICALMINLVL     ( 0 )

/**@} EOF DALIProtocolHandler_prm.h */

#endif  // _DALIPROTOCOLHANDLER_PRM_H
