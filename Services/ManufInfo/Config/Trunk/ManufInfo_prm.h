/******************************************************************************
 * @file ManufInfo_prm.h
 *
 * @brief Manufacturing information parameteraizaion 
 *
 * This file provides the parameters to condtrol the the type of data contained
 * in the manufacturing info
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
 * $Log: $
 * 
 *
 * \addtogroup ManufInfo
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _MANUFINFO_PRM_H
#define _MANUFINFO_PRM_H

// Macros and Defines ---------------------------------------------------------
/// define the macro to enable debug commands
#define MANUFINFO_ENABLE_DEBUGCOMMANDS          ( 0 )

/// define the macro to enable software revision test
#define MANUFINFO_REV_SFW_TST_ENABLE 		        ( 1 )

/// define the boot loader reveison enable
#define MANUFINFO_REV_BLT_VER_ENABLE            ( 0 )

/// define the lengths for the serial number/part numbers
#define MANUFINFO_TITLE_LEN                     ( 16 )
#define MANUFINFO_SERNUM_LEN                    ( 8 )
#define MANUFINFO_PRTNUM1_LEN                   ( 14 )
#define MANUFINFO_PRTNUM2_LEN                   ( 11 )

/// define the major/minor/test version numbers
#define MANUFINFO_HDW_MAJ                       ( 0x01 )
#define MANUFINFO_HDW_MIN                       ( 0x00 )

/// define the major/minor/test version numbers
#define MANUFINFO_SFW_MAJ                       ( 0x00 )
#define MANUFINFO_SFW_MIN                       ( 0x00 )
#if ( MANUFINFO_REV_SFW_TST_ENABLE == 1 )
  #define MANUFINFO_SFW_TST                     ( 0x01 )
#endif // MANUFINFO_REV_SFW_TST_ENABLE

/// define the sernum
#define MANUFINFO_TITLE                         ( "" )
#define MANUFINFO_SERNUM                        ( "" )
#define MANUFINFO_PRTNUM1                       ( "" )
#define MANUFINFO_PRTNUM2                       ( "" )

/**@} EOF ManufInfo_prm.h */

#endif  // _MANUFINFO_PRM_H