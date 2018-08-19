/******************************************************************************
 * @file DALIBusMaster.h
 *
 * @brief DALI Bus master handler declarations 
 *
 * This file provides the DALI bus master declarations
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
 * \addtogroup DALIBusMaster
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _DALIBUSMASTER_H
#define _DALIBUSMASTER_H

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "DALIBusMaster/DALIBusMaster_def.h"
#include "DALIBusMaster/DALIBusMaster_cfg.h"

// library includes -----------------------------------------------------------
#include "TaskManager/TaskManager.h"

// Macros and Defines ---------------------------------------------------------
/// define the number of transmit queue events
#define DALIBUSMASTER_XMT_QUEUE_SIZE              ( 2 )

/// define the number of events
#define DALIBUSMASTER_XMT_NUM_EVENTS              ( 4 )

/// define the number of events for the bus monitor
#define DALIBUSMASTER_MON_NUM_EVENTS              ( 1 )

/// define the number of events for the bus manager
#define DALIBUSMASTER_BUS_NUM_EVENTS              ( 2 )

/// define the execution rate for the bus monitor
#define DALIBUSMASTER_BUSMON_EXEC_RATE            ( TASK_TIME_MSECS( 250 ))

/// define the number of groups
#define DALI_MAX_NUM_OF_GROUPS                    ( 16 )

/// define the normal commands
#define DALI_CMD_OFF                              ( 0x00 )
#define DALI_CMD_UP                               ( 0x01 )
#define DALI_CMD_DN                               ( 0x02 )
#define DALI_CMD_STEPUP                           ( 0x03 )
#define DALI_CMD_STEPDN                           ( 0x04 )
#define DALI_CMD_RECALLMAXLVL                     ( 0x05 )
#define DALI_CMD_RECALLMINLVL                     ( 0x06 )
#define DALI_CMD_STEPDNANDOFF                     ( 0x07 )
#define DALI_CMD_ONANDSTEPUP                      ( 0x08 )
#define DALI_CMD_ENABLEDAPC                       ( 0x09 )
#define DALI_CMD_GOTOSCENE                        ( 0x10 )
#define DALI_CMD_RESET                            ( 0x20 )
#define DALI_CMD_STOREACTLVLDTR                   ( 0x21 )
#define DALI_CMD_STOREDTRMAXLVL                   ( 0x2A )
#define DALI_CMD_STOREDTRMINLVL                   ( 0x2B )
#define DALI_CMD_ADDTOGROUP                       ( 0x60 )
#define DALI_CMD_REMOVEFROMGROUP                  ( 0x70 )
#define DALI_CMD_ENABLEWRTMEM                     ( 0x81 )
#define DALI_CMD_QUERYSTATUS                      ( 0x90 )
#define DALI_CMD_QUERYCONTROLGEAR                 ( 0x91 )
#define DALI_CMD_QUERYLAMPFAILURE                 ( 0x92 )
#define DALI_CMD_QUERYMISSINGSHORTADDR            ( 0x96 )
#define DALI_CMD_QUERYVERSIONNUM                  ( 0x97 )
#define DALI_CMD_QUERYCONTENTDTR                  ( 0x98 )
#define DALI_CMD_QUERYDEVICETYPE                  ( 0x99 )
#define DALI_CMD_QUERYPHYMINLVL                   ( 0x9A )
#define DALI_CMD_QUERYPOWERFAILURE                ( 0x9B )
#define DALI_CMD_QUERYCONTENTDTR1                 ( 0x9C )
#define DALI_CMD_QUERYCONTENTDTR2                 ( 0x9D )
#define DALI_CMD_QUERYACTUALLVL                   ( 0xA0 )
#define DALI_CMD_QUERYMAXLVL                      ( 0xA1 )
#define DALI_CMD_QUERYMINLVL                      ( 0xA2 )
#define DALI_CMD_QUERYPOWERONLVL                  ( 0xA3 )
#define DALI_CMD_QUERYSYSFAILURELVL               ( 0xA4 )
#define DALI_CMD_QUERYFADETIMERATE                ( 0xA5 )
#define DALI_CMD_QUERYSCENELVL00                  ( 0xB0 )
#define DALI_CMD_READMEMLOCATION                  ( 0xC5 )

/// define the special commands
#define DALI_CMD_TERMINATE                        ( 0xA1 )
#define DALI_CMD_INITIALIZE                       ( 0xA5 )
#define DALI_CMD_RANDOMIZE                        ( 0xA7 )
#define DALI_CMD_COMPARE                          ( 0xA9 )
#define DALI_CMD_WITHDRAW                         ( 0xAB )
#define DALI_CMD_SEARCHADDRH                      ( 0xB1 )
#define DALI_CMD_SEARCHADDRM                      ( 0xB3 )
#define DALI_CMD_SEARCHADDRL                      ( 0xB5 )
#define DALI_CMD_PROGRAMSHORTADDR                 ( 0xB7 )
#define DALI_CMD_VERIFYSHORTADDR                  ( 0xB9 )
#define DALI_CMD_QUERYSHORTADDR                   ( 0xBB )
#define DALI_CMD_WRITEMEMLOC                      ( 0xC7 )
#define DALI_CMD_ENABLEDEVTYPE                    ( 0xC1 )
#define DALI_CMD_DTR                              ( 0xA3 )
#define DALI_CMD_DTR1                             ( 0xC3 )
#define DALI_CMD_DTR2                             ( 0xC5 )

/// define the application extended commands
#define DALI_CMD_REFERENCESYSPOWER                 ( 0xE0 )
#define DALI_CMD_ENABLECURRENTPROTECT              ( 0xE1 )
#define DALI_CMD_DISABLECURRENTPROTECT             ( 0xE2 )
#define DALI_CMD_SELECTDIMCURVE                    ( 0xE3 )
#define DALI_CMD_QUERYGEARTYPE                     ( 0xED )
#define DALI_CMD_QUERYDIMCURVE                     ( 0xEE )
#define DALI_CMD_QUERYPOSSIBLEOPMODES              ( 0xEF )
#define DALI_CMD_QUERYFEATURES                     ( 0xF0 )
#define DALI_CMD_QUERYFAILURESTATUS                ( 0xF1 )
#define DALI_CMD_QUERYSHORTCIRCUIT                 ( 0xF2 )
#define DALI_CMD_QUERYOPENCIRCUIT                  ( 0xF3 )
#define DALI_CMD_QUERYLOADDECREASE                 ( 0xF4 )
#define DALI_CMD_QUERYLOADINCREASE                 ( 0xF5 )
#define DALI_CMD_QUERYCURRENTPROTECTACTIVE         ( 0xF6 )
#define DALI_CMD_QUERYTHERMALSHUTDOWN              ( 0xF7 )
#define DALI_CMD_QUERYTHERMALOVERLOAD              ( 0xF8 )
#define DALI_CMD_QUERYREFERENCERUNNING             ( 0xF9 )
#define DALI_CMD_QUERYREFERENCEFAILURE             ( 0xFA )
#define DALI_CMD_QUERYCURRENTPROTECTENABLE         ( 0xFB )
#define DALI_CMD_QUERYOPMODE                       ( 0xFC )
#define DALI_CMD_QUERYFASTFADETIME                 ( 0xFD )
#define DALI_CMD_QUERYMINFASTFADETIME              ( 0xFE )
#define DALI_CMD_QUERYEXTVERNUM                    ( 0xFF )

#define DALI_CMD_COMMAND_MASK                      ( 0x01 )

/// define the broadcase address
#define DALI_ADDR_BROADCAST_LEVEL                 ( 254 )
#define DALI_ADDR_BROADCAST_COMMAND               ( 255 )

/// define the DALI min/max group address
#define DALI_BUS_MINGRP_ADDR                      ( 64 )
#define DALI_BUS_MAXGRP_ADDR                      ( 79 )

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations -----------------------------------------------

// global function prototypes --------------------------------------------------
extern  void                  DALIBusMaster_Initialize( void );
extern  PC8                   DALIBusMaster_GetStatusDescription( DALIBUSMASTERSTS eStatus );
extern  U8                    DALIBusMaster_GetDevPresentCnt( void );
extern  U64                   DALIBusMaster_GetDeviceStatusHuge( DALIBUSMASTERDEVSTS eStatus );
extern  DALIBUSMASTERDEVSTS   DALIBusMaster_GetDeviceTableEntry( U8 nIndex );
extern  void                  DALIBusMaster_SetDeviceTableEntry( U8 nIndex, DALIBUSMASTERDEVSTS eStatus );
extern  BOOL                  DALIBusMaster_ProcessBusEvent( TASKARG xArg );
extern  BOOL                  DALIBusMaster_ProcessXmtEvent( TASKARG xArg );
extern  BOOL                  DALIBusMaster_ProcessMonEvent( TASKARG xArg );
extern  void                  DALIBusMaster_StoreDevStatus( void );

/**@} EOF DALIBusMaster.h */

#endif  // _DALIBUSMASTER_H