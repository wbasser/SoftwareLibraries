/******************************************************************************
 * @file NormalCommands.c
 *
 * @brief 
 *
 * This file 
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
 * $Rev: $
 * 
 *
 * \addtogroup DALIMessageHandler
 * @{
 *****************************************************************************/

// system includes ------------------------------------------------------------

// library includes -----------------------------------------------------------
#include "ParameterManager/ParameterManager.h"

// local includes -------------------------------------------------------------
#include "DALIMessageHandler/DALIMessageHandler_cfg.h"
#include "DALIMessageHandler/NormalCommands/NormalCommands.h"

// Macros and Defines ---------------------------------------------------------
/// Define Scene Reset Value
#define SCENE_RESET          ( 0xFF )

/// Define status byte mask for detecting reset state
#define STATUS_RESET_MASK    ( 0x98 )

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations ----------------------------------------------

// local parameter declarations -----------------------------------------------

// local function prototypes --------------------------------------------------
static	void	CmdOff( PDALICTL ptDaliCtl );
static	void	CmdUp( PDALICTL ptDaliCtl );
static	void	CmdDown( PDALICTL ptDaliCtl );
static	void	CmdStepUp( PDALICTL ptDaliCtl );
static	void	CmdStepDown( PDALICTL ptDaliCtl );
static	void	CmdRecallMaxLvl( PDALICTL ptDaliCtl );
static	void	CmdRecallMinLvl( PDALICTL ptDaliCtl );
static	void	CmdStepDownOff( PDALICTL ptDaliCtl );
static	void	CmdOnStepUp( PDALICTL ptDaliCtl );
static	void	CmdEnbDAPCSeq( PDALICTL ptDaliCtl );
static	void	CmdScene( PDALICTL ptDaliCtl );
static	void	CmdReset( PDALICTL ptDaliCtl );
static	void	CmdStoreLevelInDtr( PDALICTL ptDaliCtl );
static	void	CmdStoreDtrMaxLvl( PDALICTL ptDaliCtl );
static	void	CmdStoreDtrMinLvl( PDALICTL ptDaliCtl );
static	void	CmdStoreDtrSysFailLvl( PDALICTL ptDaliCtl );
static	void	CmdStoreDtrPwrOnLvl( PDALICTL ptDaliCtl );
static	void	CmdStoreDtrFadeTime( PDALICTL ptDaliCtl );
static	void	CmdStoreDtrFadeRate( PDALICTL ptDaliCtl );
static	void	CmdStoreDtrScene( PDALICTL ptDaliCtl );
static	void	CmdRemoveScene( PDALICTL ptDaliCtl );
static	void	CmdAddToGroup( PDALICTL ptDaliCtl );
static	void	CmdRemoveFromGroup( PDALICTL ptDaliCtl );
static	void	CmdStoreDtrAddress( PDALICTL ptDaliCtl );
static	void	CmdEnableWriteMemory( PDALICTL ptDaliCtl );
static	void	CmdQryStatus( PDALICTL ptDaliCtl );
static	void	CmdQryBallast( PDALICTL ptDaliCtl );
static	void	CmdQryLampFail( PDALICTL ptDaliCtl );
static	void	CmdQryLampPower( PDALICTL ptDaliCtl );
static	void	CmdQryLimitError( PDALICTL ptDaliCtl );
static	void	CmdQryResetState( PDALICTL ptDaliCtl );
static	void	CmdQryMissingAddress( PDALICTL ptDaliCtl );
static	void	CmdQryVersionNumber( PDALICTL ptDaliCtl );
static	void	CmdQryContentDtr( PDALICTL ptDaliCtl );
static	void	CmdQryDeviceType( PDALICTL ptDaliCtl );
static	void	CmdQryPhysicalMinLvl( PDALICTL ptDaliCtl );
static	void	CmdQryPowerFailure( PDALICTL ptDaliCtl );
static	void	CmdQryContentDtr1( PDALICTL ptDaliCtl );
static	void	CmdQryContentDtr2( PDALICTL ptDaliCtl );
static	void	CmdQryActualLvl( PDALICTL ptDaliCtl );
static	void	CmdQryMaxLvl( PDALICTL ptDaliCtl );
static	void	CmdQryMinLvl( PDALICTL ptDaliCtl );
static	void	CmdQryPwrOnLvl( PDALICTL ptDaliCtl );
static	void	CmdQrySysFailLvl( PDALICTL ptDaliCtl );
static	void	CmdQryFadeTimeRate( PDALICTL ptDaliCtl );
static	void	CmdQrySceneLvl( PDALICTL ptDaliCtl );
static	void	CmdQryGroups0007( PDALICTL ptDaliCtl );
static	void	CmdQryGroups0815( PDALICTL ptDaliCtl );
static	void	CmdQryRandAddrH( PDALICTL ptDaliCtl );
static	void	CmdQryRandAddrM( PDALICTL ptDaliCtl );
static	void	CmdQryRandAddrL( PDALICTL ptDaliCtl );
static	void	CmdQryMemoryLoc( PDALICTL ptDaliCtl );
static  void  CmdRefSysPwr( PDALICTL ptDaliCtl );               
static  void  CmdEnableCurProtect( PDALICTL ptDaliCtl );          
static  void  CmdDisableCurProtect( PDALICTL ptDaliCtl );         
static  void  CmdSelDimCurve( PDALICTL ptDaliCtl );            
static  void  CmdStoreDtrFastFadetime( PDALICTL ptDaliCtl );    
static  void  CmdQryGearType( PDALICTL ptDaliCtl );            
static  void  CmdQryDimCurve( PDALICTL ptDaliCtl );          
static  void  CmdQryPossibleOpModes( PDALICTL ptDaliCtl );    
static  void  CmdQryFeatures( PDALICTL ptDaliCtl );            
static  void  CmdQryFailiureStatus( PDALICTL ptDaliCtl );      
static  void  CmdQryShortCircuit( PDALICTL ptDaliCtl );       
static  void  CmdQryOpenCircuit( PDALICTL ptDaliCtl );          
static  void  CmdQryLoadDec( PDALICTL ptDaliCtl );        
static  void  CmdQryLoadInc( PDALICTL ptDaliCtl );            
static  void  CmdQryCurProtectActive( PDALICTL ptDaliCtl );    
static  void  CmdQryThermalShutdown( PDALICTL ptDaliCtl );   
static  void  CmdQryThermalOverload( PDALICTL ptDaliCtl );    
static  void  CmdQryRefRunning( PDALICTL ptDaliCtl );       
static  void  CmdQryRefMeasureFail( PDALICTL ptDaliCtl );   
static  void  CmdQryCurProtectEnabled( PDALICTL ptDaliCtl );   
static  void  CmdQryOpMode( PDALICTL ptDaliCtl );              
static  void  CmdQryFastFadeTime( PDALICTL ptDaliCtl );       
static  void  CmdQryMinFastFadeTime( PDALICTL ptDaliCtl );     
static  void  CmdQryExtVerNumber( PDALICTL ptDaliCtl );       
static	void	CmdQryExtVerNumber( PDALICTL ptDaliCtl );

// constant parameter initializations -----------------------------------------
/// define the normal commands
const CODE CMDTBL	atNormCmds[ ] =
{
  CMDTBL_NORMENT( FALSE, RESPONSE_TYPE_NONE, CmdOff ),                  // 0
  CMDTBL_NORMENT( FALSE, RESPONSE_TYPE_NONE, CmdUp ),                   // 1
  CMDTBL_NORMENT( FALSE, RESPONSE_TYPE_NONE, CmdDown ),                 // 2
  CMDTBL_NORMENT( FALSE, RESPONSE_TYPE_NONE, CmdStepUp ),               // 3
  CMDTBL_NORMENT( FALSE, RESPONSE_TYPE_NONE, CmdStepDown ),             // 4
  CMDTBL_NORMENT( FALSE, RESPONSE_TYPE_NONE, CmdRecallMaxLvl ),         // 5
  CMDTBL_NORMENT( FALSE, RESPONSE_TYPE_NONE, CmdRecallMinLvl ),         // 6
  CMDTBL_NORMENT( FALSE, RESPONSE_TYPE_NONE, CmdStepDownOff ),          // 7
  CMDTBL_NORMENT( FALSE, RESPONSE_TYPE_NONE, CmdOnStepUp ),             // 8
  CMDTBL_NORMENT( FALSE, RESPONSE_TYPE_NONE, CmdEnbDAPCSeq ),           // 9
  CMDTBL_NORMENT( FALSE, RESPONSE_TYPE_NONE, NULL ),                    // 10
  CMDTBL_NORMENT( FALSE, RESPONSE_TYPE_NONE, NULL ),                    // 11
  CMDTBL_NORMENT( FALSE, RESPONSE_TYPE_NONE, NULL ),                    // 12
  CMDTBL_NORMENT( FALSE, RESPONSE_TYPE_NONE, NULL ),                    // 13
  CMDTBL_NORMENT( FALSE, RESPONSE_TYPE_NONE, NULL ),                    // 14
  CMDTBL_NORMENT( FALSE, RESPONSE_TYPE_NONE, NULL ),                    // 15
  CMDTBL_NORMENT( FALSE, RESPONSE_TYPE_NONE, CmdScene ),                // 16
  CMDTBL_NORMENT( FALSE, RESPONSE_TYPE_NONE, CmdScene ),                // 17
  CMDTBL_NORMENT( FALSE, RESPONSE_TYPE_NONE, CmdScene ),                // 18
  CMDTBL_NORMENT( FALSE, RESPONSE_TYPE_NONE, CmdScene ),                // 19
  CMDTBL_NORMENT( FALSE, RESPONSE_TYPE_NONE, CmdScene ),                // 20
  CMDTBL_NORMENT( FALSE, RESPONSE_TYPE_NONE, CmdScene ),                // 21
  CMDTBL_NORMENT( FALSE, RESPONSE_TYPE_NONE, CmdScene ),                // 22
  CMDTBL_NORMENT( FALSE, RESPONSE_TYPE_NONE, CmdScene ),                // 23
  CMDTBL_NORMENT( FALSE, RESPONSE_TYPE_NONE, CmdScene ),                // 24
  CMDTBL_NORMENT( FALSE, RESPONSE_TYPE_NONE, CmdScene ),                // 25
  CMDTBL_NORMENT( FALSE, RESPONSE_TYPE_NONE, CmdScene ),                // 26
  CMDTBL_NORMENT( FALSE, RESPONSE_TYPE_NONE, CmdScene ),                // 27
  CMDTBL_NORMENT( FALSE, RESPONSE_TYPE_NONE, CmdScene ),                // 28
  CMDTBL_NORMENT( FALSE, RESPONSE_TYPE_NONE, CmdScene ),                // 29
  CMDTBL_NORMENT( FALSE, RESPONSE_TYPE_NONE, CmdScene ),                // 30
  CMDTBL_NORMENT( FALSE, RESPONSE_TYPE_NONE, CmdScene ),                // 31
  CMDTBL_NORMENT( TRUE,  RESPONSE_TYPE_NONE, CmdReset ),                // 32
  CMDTBL_NORMENT( TRUE,  RESPONSE_TYPE_NONE, CmdStoreLevelInDtr ),      // 33
  CMDTBL_NORMENT( FALSE, RESPONSE_TYPE_NONE, NULL ),                    // 34
  CMDTBL_NORMENT( FALSE, RESPONSE_TYPE_NONE, NULL ),                    // 35
  CMDTBL_NORMENT( FALSE, RESPONSE_TYPE_NONE, NULL ),                    // 36
  CMDTBL_NORMENT( FALSE, RESPONSE_TYPE_NONE, NULL ),                    // 37
  CMDTBL_NORMENT( FALSE, RESPONSE_TYPE_NONE, NULL ),                    // 38
  CMDTBL_NORMENT( FALSE, RESPONSE_TYPE_NONE, NULL ),                    // 39
  CMDTBL_NORMENT( FALSE, RESPONSE_TYPE_NONE, NULL ),                    // 40
  CMDTBL_NORMENT( FALSE, RESPONSE_TYPE_NONE, NULL ),                    // 41
  CMDTBL_NORMENT( TRUE,  RESPONSE_TYPE_NONE, CmdStoreDtrMaxLvl ),       // 42
  CMDTBL_NORMENT( TRUE,  RESPONSE_TYPE_NONE, CmdStoreDtrMinLvl ),       // 43
  CMDTBL_NORMENT( TRUE,  RESPONSE_TYPE_NONE, CmdStoreDtrSysFailLvl ),   // 44
  CMDTBL_NORMENT( TRUE,  RESPONSE_TYPE_NONE, CmdStoreDtrPwrOnLvl ),	    // 45
  CMDTBL_NORMENT( TRUE,  RESPONSE_TYPE_NONE, CmdStoreDtrFadeTime ),	    // 46
  CMDTBL_NORMENT( TRUE,  RESPONSE_TYPE_NONE, CmdStoreDtrFadeRate ),	    // 47
  CMDTBL_NORMENT( FALSE, RESPONSE_TYPE_NONE, NULL ),             		    // 48
  CMDTBL_NORMENT( FALSE, RESPONSE_TYPE_NONE, NULL ),             		    // 49
  CMDTBL_NORMENT( FALSE, RESPONSE_TYPE_NONE, NULL ),             		    // 50
  CMDTBL_NORMENT( FALSE, RESPONSE_TYPE_NONE, NULL ),             		    // 51
  CMDTBL_NORMENT( FALSE, RESPONSE_TYPE_NONE, NULL ),                    // 52
  CMDTBL_NORMENT( FALSE, RESPONSE_TYPE_NONE, NULL ),                    // 53
  CMDTBL_NORMENT( FALSE, RESPONSE_TYPE_NONE, NULL ),                    // 54
  CMDTBL_NORMENT( FALSE, RESPONSE_TYPE_NONE, NULL ),                    // 55
  CMDTBL_NORMENT( FALSE, RESPONSE_TYPE_NONE, NULL ),                    // 56
  CMDTBL_NORMENT( FALSE, RESPONSE_TYPE_NONE, NULL ),                    // 57
  CMDTBL_NORMENT( FALSE, RESPONSE_TYPE_NONE, NULL ),                    // 58
  CMDTBL_NORMENT( FALSE, RESPONSE_TYPE_NONE, NULL ),                    // 59
  CMDTBL_NORMENT( FALSE, RESPONSE_TYPE_NONE, NULL ),                    // 60
  CMDTBL_NORMENT( FALSE, RESPONSE_TYPE_NONE, NULL ),                    // 61
  CMDTBL_NORMENT( FALSE, RESPONSE_TYPE_NONE, NULL ),                    // 62
  CMDTBL_NORMENT( FALSE, RESPONSE_TYPE_NONE, NULL ),                    // 63
  CMDTBL_NORMENT( TRUE,  RESPONSE_TYPE_NONE, CmdStoreDtrScene ),			  // 64
  CMDTBL_NORMENT( TRUE,  RESPONSE_TYPE_NONE, CmdStoreDtrScene ),			  // 65
  CMDTBL_NORMENT( TRUE,  RESPONSE_TYPE_NONE, CmdStoreDtrScene ),			  // 66
  CMDTBL_NORMENT( TRUE,  RESPONSE_TYPE_NONE, CmdStoreDtrScene ),			  // 67
  CMDTBL_NORMENT( TRUE,  RESPONSE_TYPE_NONE, CmdStoreDtrScene ),			  // 68
  CMDTBL_NORMENT( TRUE,  RESPONSE_TYPE_NONE, CmdStoreDtrScene ),			  // 69
  CMDTBL_NORMENT( TRUE,  RESPONSE_TYPE_NONE, CmdStoreDtrScene ),			  // 70
  CMDTBL_NORMENT( TRUE,  RESPONSE_TYPE_NONE, CmdStoreDtrScene ),			  // 71
  CMDTBL_NORMENT( TRUE,  RESPONSE_TYPE_NONE, CmdStoreDtrScene ),			  // 72
  CMDTBL_NORMENT( TRUE,  RESPONSE_TYPE_NONE, CmdStoreDtrScene ),			  // 73
  CMDTBL_NORMENT( TRUE,  RESPONSE_TYPE_NONE, CmdStoreDtrScene ),			  // 74
  CMDTBL_NORMENT( TRUE,  RESPONSE_TYPE_NONE, CmdStoreDtrScene ),			  // 75
  CMDTBL_NORMENT( TRUE,  RESPONSE_TYPE_NONE, CmdStoreDtrScene ),			  // 76
  CMDTBL_NORMENT( TRUE,  RESPONSE_TYPE_NONE, CmdStoreDtrScene ),			  // 77
  CMDTBL_NORMENT( TRUE,  RESPONSE_TYPE_NONE, CmdStoreDtrScene ),			  // 78
  CMDTBL_NORMENT( TRUE,  RESPONSE_TYPE_NONE, CmdStoreDtrScene ),			  // 79
  CMDTBL_NORMENT( TRUE,  RESPONSE_TYPE_NONE, CmdRemoveScene ),			    // 80
  CMDTBL_NORMENT( TRUE,  RESPONSE_TYPE_NONE, CmdRemoveScene ),			    // 81
  CMDTBL_NORMENT( TRUE,  RESPONSE_TYPE_NONE, CmdRemoveScene ),			    // 82
  CMDTBL_NORMENT( TRUE,  RESPONSE_TYPE_NONE, CmdRemoveScene ),			    // 83
  CMDTBL_NORMENT( TRUE,  RESPONSE_TYPE_NONE, CmdRemoveScene ),			    // 84
  CMDTBL_NORMENT( TRUE,  RESPONSE_TYPE_NONE, CmdRemoveScene ),			    // 85
  CMDTBL_NORMENT( TRUE,  RESPONSE_TYPE_NONE, CmdRemoveScene ),			    // 86
  CMDTBL_NORMENT( TRUE,  RESPONSE_TYPE_NONE, CmdRemoveScene ),			    // 87
  CMDTBL_NORMENT( TRUE,  RESPONSE_TYPE_NONE, CmdRemoveScene ),			    // 88
  CMDTBL_NORMENT( TRUE,  RESPONSE_TYPE_NONE, CmdRemoveScene ),			    // 89
  CMDTBL_NORMENT( TRUE,  RESPONSE_TYPE_NONE, CmdRemoveScene ),			    // 90
  CMDTBL_NORMENT( TRUE,  RESPONSE_TYPE_NONE, CmdRemoveScene ),			    // 91
  CMDTBL_NORMENT( TRUE,  RESPONSE_TYPE_NONE, CmdRemoveScene ),			    // 92
  CMDTBL_NORMENT( TRUE,  RESPONSE_TYPE_NONE, CmdRemoveScene ),			    // 93
  CMDTBL_NORMENT( TRUE,  RESPONSE_TYPE_NONE, CmdRemoveScene ),			    // 94
  CMDTBL_NORMENT( TRUE,  RESPONSE_TYPE_NONE, CmdRemoveScene ),			    // 95
  CMDTBL_NORMENT( TRUE,  RESPONSE_TYPE_NONE, CmdAddToGroup ),           // 96
  CMDTBL_NORMENT( TRUE,  RESPONSE_TYPE_NONE, CmdAddToGroup ),           // 97
  CMDTBL_NORMENT( TRUE,  RESPONSE_TYPE_NONE, CmdAddToGroup ),           // 98
  CMDTBL_NORMENT( TRUE,  RESPONSE_TYPE_NONE, CmdAddToGroup ),           // 99
  CMDTBL_NORMENT( TRUE,  RESPONSE_TYPE_NONE, CmdAddToGroup ),           // 100
  CMDTBL_NORMENT( TRUE,  RESPONSE_TYPE_NONE, CmdAddToGroup ),           // 101
  CMDTBL_NORMENT( TRUE,  RESPONSE_TYPE_NONE, CmdAddToGroup ),           // 102
  CMDTBL_NORMENT( TRUE,  RESPONSE_TYPE_NONE, CmdAddToGroup ),           // 103
  CMDTBL_NORMENT( TRUE,  RESPONSE_TYPE_NONE, CmdAddToGroup ),           // 104
  CMDTBL_NORMENT( TRUE,  RESPONSE_TYPE_NONE, CmdAddToGroup ),           // 105
  CMDTBL_NORMENT( TRUE,  RESPONSE_TYPE_NONE, CmdAddToGroup ),           // 106
  CMDTBL_NORMENT( TRUE,  RESPONSE_TYPE_NONE, CmdAddToGroup ),           // 107
  CMDTBL_NORMENT( TRUE,  RESPONSE_TYPE_NONE, CmdAddToGroup ),           // 108
  CMDTBL_NORMENT( TRUE,  RESPONSE_TYPE_NONE, CmdAddToGroup ),           // 109
  CMDTBL_NORMENT( TRUE,  RESPONSE_TYPE_NONE, CmdAddToGroup ),           // 110
  CMDTBL_NORMENT( TRUE,  RESPONSE_TYPE_NONE, CmdAddToGroup ),           // 111
  CMDTBL_NORMENT( TRUE,  RESPONSE_TYPE_NONE, CmdRemoveFromGroup ),		  // 112
  CMDTBL_NORMENT( TRUE,  RESPONSE_TYPE_NONE, CmdRemoveFromGroup ),		  // 113
  CMDTBL_NORMENT( TRUE,  RESPONSE_TYPE_NONE, CmdRemoveFromGroup ),		  // 114
  CMDTBL_NORMENT( TRUE,  RESPONSE_TYPE_NONE, CmdRemoveFromGroup ),		  // 115
  CMDTBL_NORMENT( TRUE,  RESPONSE_TYPE_NONE, CmdRemoveFromGroup ),		  // 116
  CMDTBL_NORMENT( TRUE,  RESPONSE_TYPE_NONE, CmdRemoveFromGroup ),		  // 117
  CMDTBL_NORMENT( TRUE,  RESPONSE_TYPE_NONE, CmdRemoveFromGroup ),		  // 118
  CMDTBL_NORMENT( TRUE,  RESPONSE_TYPE_NONE, CmdRemoveFromGroup ),		  // 119
  CMDTBL_NORMENT( TRUE,  RESPONSE_TYPE_NONE, CmdRemoveFromGroup ),		  // 120
  CMDTBL_NORMENT( TRUE,  RESPONSE_TYPE_NONE, CmdRemoveFromGroup ),		  // 121
  CMDTBL_NORMENT( TRUE,  RESPONSE_TYPE_NONE, CmdRemoveFromGroup ),		  // 122
  CMDTBL_NORMENT( TRUE,  RESPONSE_TYPE_NONE, CmdRemoveFromGroup ),		  // 123
  CMDTBL_NORMENT( TRUE,  RESPONSE_TYPE_NONE, CmdRemoveFromGroup ),		  // 124
  CMDTBL_NORMENT( TRUE,  RESPONSE_TYPE_NONE, CmdRemoveFromGroup ),		  // 125
  CMDTBL_NORMENT( TRUE,  RESPONSE_TYPE_NONE, CmdRemoveFromGroup ),		  // 126
  CMDTBL_NORMENT( TRUE,  RESPONSE_TYPE_NONE, CmdRemoveFromGroup ),		  // 127
  CMDTBL_NORMENT( TRUE,  RESPONSE_TYPE_NONE, CmdStoreDtrAddress ),	    // 128
  CMDTBL_NORMENT( TRUE,  RESPONSE_TYPE_NONE, CmdEnableWriteMemory ), 	  // 129
  CMDTBL_NORMENT( FALSE, RESPONSE_TYPE_NONE, NULL ),                    // 130
  CMDTBL_NORMENT( FALSE, RESPONSE_TYPE_NONE, NULL ),                    // 131
  CMDTBL_NORMENT( FALSE, RESPONSE_TYPE_NONE, NULL ),                    // 132
  CMDTBL_NORMENT( FALSE, RESPONSE_TYPE_NONE, NULL ),                    // 133
  CMDTBL_NORMENT( FALSE, RESPONSE_TYPE_NONE, NULL ),                    // 134
  CMDTBL_NORMENT( FALSE, RESPONSE_TYPE_NONE, NULL ),                    // 135
  CMDTBL_NORMENT( FALSE, RESPONSE_TYPE_NONE, NULL ),                    // 136
  CMDTBL_NORMENT( FALSE, RESPONSE_TYPE_NONE, NULL ),                    // 137
  CMDTBL_NORMENT( FALSE, RESPONSE_TYPE_NONE, NULL ),                    // 138
  CMDTBL_NORMENT( FALSE, RESPONSE_TYPE_NONE, NULL ),                    // 139
  CMDTBL_NORMENT( FALSE, RESPONSE_TYPE_NONE, NULL ),                    // 140
  CMDTBL_NORMENT( FALSE, RESPONSE_TYPE_NONE, NULL ),                    // 141
  CMDTBL_NORMENT( FALSE, RESPONSE_TYPE_NONE, NULL ),                    // 142
  CMDTBL_NORMENT( FALSE, RESPONSE_TYPE_NONE, NULL ),                    // 143
  CMDTBL_NORMENT( FALSE, RESPONSE_TYPE_SEND, CmdQryStatus ),      		  // 144
  CMDTBL_NORMENT( FALSE, RESPONSE_TYPE_SEND, CmdQryBallast ),           // 145
  CMDTBL_NORMENT( FALSE, RESPONSE_TYPE_FLAG, CmdQryLampFail ),				  // 146
  CMDTBL_NORMENT( FALSE, RESPONSE_TYPE_FLAG, CmdQryLampPower ),         // 147
  CMDTBL_NORMENT( FALSE, RESPONSE_TYPE_FLAG, CmdQryLimitError ),			  // 148
  CMDTBL_NORMENT( FALSE, RESPONSE_TYPE_FLAG, CmdQryResetState ),			  // 149
  CMDTBL_NORMENT( FALSE, RESPONSE_TYPE_FLAG, CmdQryMissingAddress ),	  // 150
  CMDTBL_NORMENT( FALSE, RESPONSE_TYPE_SEND, CmdQryVersionNumber ),     // 151
  CMDTBL_NORMENT( FALSE, RESPONSE_TYPE_SEND, CmdQryContentDtr ),			  // 152
  CMDTBL_NORMENT( FALSE, RESPONSE_TYPE_SEND, CmdQryDeviceType ),			  // 153
  CMDTBL_NORMENT( FALSE, RESPONSE_TYPE_SEND, CmdQryPhysicalMinLvl ),	  // 154
  CMDTBL_NORMENT( FALSE, RESPONSE_TYPE_FLAG, CmdQryPowerFailure ),		  // 155
  CMDTBL_NORMENT( FALSE, RESPONSE_TYPE_SEND, CmdQryContentDtr1 ),   	  // 156
  CMDTBL_NORMENT( FALSE, RESPONSE_TYPE_SEND, CmdQryContentDtr2 ),   	  // 157
  CMDTBL_NORMENT( FALSE, RESPONSE_TYPE_NONE, NULL ),                    // 158
  CMDTBL_NORMENT( FALSE, RESPONSE_TYPE_NONE, NULL ),                    // 159
  CMDTBL_NORMENT( FALSE, RESPONSE_TYPE_SEND, CmdQryActualLvl ),         // 160
  CMDTBL_NORMENT( FALSE, RESPONSE_TYPE_SEND, CmdQryMaxLvl ),					  // 161
  CMDTBL_NORMENT( FALSE, RESPONSE_TYPE_SEND, CmdQryMinLvl ),					  // 162
  CMDTBL_NORMENT( FALSE, RESPONSE_TYPE_SEND, CmdQryPwrOnLvl ),				  // 163
  CMDTBL_NORMENT( FALSE, RESPONSE_TYPE_SEND, CmdQrySysFailLvl),         // 164
  CMDTBL_NORMENT( FALSE, RESPONSE_TYPE_SEND, CmdQryFadeTimeRate ),		  // 165
  CMDTBL_NORMENT( FALSE, RESPONSE_TYPE_NONE, NULL ),                    // 166
  CMDTBL_NORMENT( FALSE, RESPONSE_TYPE_NONE, NULL ),                    // 167
  CMDTBL_NORMENT( FALSE, RESPONSE_TYPE_NONE, NULL ),                    // 168
  CMDTBL_NORMENT( FALSE, RESPONSE_TYPE_NONE, NULL ),                    // 169
  CMDTBL_NORMENT( FALSE, RESPONSE_TYPE_NONE, NULL ),                    // 170
  CMDTBL_NORMENT( FALSE, RESPONSE_TYPE_NONE, NULL ),                    // 171
  CMDTBL_NORMENT( FALSE, RESPONSE_TYPE_NONE, NULL ),                    // 172
  CMDTBL_NORMENT( FALSE, RESPONSE_TYPE_NONE, NULL ),                    // 173
  CMDTBL_NORMENT( FALSE, RESPONSE_TYPE_NONE, NULL ),                    // 174
  CMDTBL_NORMENT( FALSE, RESPONSE_TYPE_NONE, NULL ),                    // 175
  CMDTBL_NORMENT( FALSE, RESPONSE_TYPE_SEND, CmdQrySceneLvl ),				  // 176
  CMDTBL_NORMENT( FALSE, RESPONSE_TYPE_SEND, CmdQrySceneLvl ),				  // 177
  CMDTBL_NORMENT( FALSE, RESPONSE_TYPE_SEND, CmdQrySceneLvl ),				  // 178
  CMDTBL_NORMENT( FALSE, RESPONSE_TYPE_SEND, CmdQrySceneLvl ),				  // 179
  CMDTBL_NORMENT( FALSE, RESPONSE_TYPE_SEND, CmdQrySceneLvl ),				  // 180
  CMDTBL_NORMENT( FALSE, RESPONSE_TYPE_SEND, CmdQrySceneLvl ),				  // 181
  CMDTBL_NORMENT( FALSE, RESPONSE_TYPE_SEND, CmdQrySceneLvl ),				  // 182
  CMDTBL_NORMENT( FALSE, RESPONSE_TYPE_SEND, CmdQrySceneLvl ),				  // 183
  CMDTBL_NORMENT( FALSE, RESPONSE_TYPE_SEND, CmdQrySceneLvl ),				  // 184
  CMDTBL_NORMENT( FALSE, RESPONSE_TYPE_SEND, CmdQrySceneLvl ),				  // 185
  CMDTBL_NORMENT( FALSE, RESPONSE_TYPE_SEND, CmdQrySceneLvl ),				  // 186
  CMDTBL_NORMENT( FALSE, RESPONSE_TYPE_SEND, CmdQrySceneLvl ),				  // 187
  CMDTBL_NORMENT( FALSE, RESPONSE_TYPE_SEND, CmdQrySceneLvl ),				  // 188
  CMDTBL_NORMENT( FALSE, RESPONSE_TYPE_SEND, CmdQrySceneLvl ),				  // 189
  CMDTBL_NORMENT( FALSE, RESPONSE_TYPE_SEND, CmdQrySceneLvl ),				  // 190
  CMDTBL_NORMENT( FALSE, RESPONSE_TYPE_SEND, CmdQrySceneLvl ),				  // 191
  CMDTBL_NORMENT( FALSE, RESPONSE_TYPE_SEND, CmdQryGroups0007 ),			  // 192
  CMDTBL_NORMENT( FALSE, RESPONSE_TYPE_SEND, CmdQryGroups0815 ),			  // 193
  CMDTBL_NORMENT( FALSE, RESPONSE_TYPE_SEND, CmdQryRandAddrH ),         // 194
  CMDTBL_NORMENT( FALSE, RESPONSE_TYPE_SEND, CmdQryRandAddrM ),         // 195
  CMDTBL_NORMENT( FALSE, RESPONSE_TYPE_SEND, CmdQryRandAddrL ),         // 196
  CMDTBL_NORMENT( FALSE, RESPONSE_TYPE_FLAG, CmdQryMemoryLoc ),         // 197
  CMDTBL_NORMENT( FALSE, RESPONSE_TYPE_NONE, NULL ),                    // 198
  CMDTBL_NORMENT( FALSE, RESPONSE_TYPE_NONE, NULL ),                    // 199
  CMDTBL_NORMENT( FALSE, RESPONSE_TYPE_NONE, NULL ),                    // 200
  CMDTBL_NORMENT( FALSE, RESPONSE_TYPE_NONE, NULL ),                    // 201
  CMDTBL_NORMENT( FALSE, RESPONSE_TYPE_NONE, NULL ),                    // 202
  CMDTBL_NORMENT( FALSE, RESPONSE_TYPE_NONE, NULL ),                    // 203
  CMDTBL_NORMENT( FALSE, RESPONSE_TYPE_NONE, NULL ),                    // 204
  CMDTBL_NORMENT( FALSE, RESPONSE_TYPE_NONE, NULL ),                    // 205
  CMDTBL_NORMENT( FALSE, RESPONSE_TYPE_NONE, NULL ),                    // 206
  CMDTBL_NORMENT( FALSE, RESPONSE_TYPE_NONE, NULL ),                    // 207
  CMDTBL_NORMENT( FALSE, RESPONSE_TYPE_NONE, NULL ),                    // 208
  CMDTBL_NORMENT( FALSE, RESPONSE_TYPE_NONE, NULL ),                    // 209
  CMDTBL_NORMENT( FALSE, RESPONSE_TYPE_NONE, NULL ),                    // 210
  CMDTBL_NORMENT( FALSE, RESPONSE_TYPE_NONE, NULL ),                    // 211
  CMDTBL_NORMENT( FALSE, RESPONSE_TYPE_NONE, NULL ),                    // 212
  CMDTBL_NORMENT( FALSE, RESPONSE_TYPE_NONE, NULL ),                    // 213
  CMDTBL_NORMENT( FALSE, RESPONSE_TYPE_NONE, NULL ),                    // 214
  CMDTBL_NORMENT( FALSE, RESPONSE_TYPE_NONE, NULL ),                    // 215
  CMDTBL_NORMENT( FALSE, RESPONSE_TYPE_NONE, NULL ),                    // 216
  CMDTBL_NORMENT( FALSE, RESPONSE_TYPE_NONE, NULL ),                    // 217
  CMDTBL_NORMENT( FALSE, RESPONSE_TYPE_NONE, NULL ),                    // 218
  CMDTBL_NORMENT( FALSE, RESPONSE_TYPE_NONE, NULL ),                    // 219
  CMDTBL_NORMENT( FALSE, RESPONSE_TYPE_NONE, NULL ),                    // 220
  CMDTBL_NORMENT( FALSE, RESPONSE_TYPE_NONE, NULL ),                    // 221
  CMDTBL_NORMENT( FALSE, RESPONSE_TYPE_NONE, NULL ),                    // 222
  CMDTBL_NORMENT( FALSE, RESPONSE_TYPE_NONE, NULL ),                    // 223
  CMDTBL_EXTNENT( TRUE,  RESPONSE_TYPE_NONE, CmdRefSysPwr ),            // 224
  CMDTBL_EXTNENT( TRUE,  RESPONSE_TYPE_NONE, CmdEnableCurProtect ),     // 225
  CMDTBL_EXTNENT( TRUE,  RESPONSE_TYPE_NONE, CmdDisableCurProtect ),    // 226
  CMDTBL_EXTNENT( TRUE,  RESPONSE_TYPE_NONE, CmdSelDimCurve ),          // 227
  CMDTBL_EXTNENT( TRUE,  RESPONSE_TYPE_NONE, CmdStoreDtrFastFadetime ), // 228 
  CMDTBL_EXTNENT( FALSE, RESPONSE_TYPE_NONE, NULL ),                    // 229
  CMDTBL_EXTNENT( FALSE, RESPONSE_TYPE_NONE, NULL ),                    // 230
  CMDTBL_EXTNENT( FALSE, RESPONSE_TYPE_NONE, NULL ),                    // 231
  CMDTBL_EXTNENT( FALSE, RESPONSE_TYPE_NONE, NULL ),                    // 232
  CMDTBL_EXTNENT( FALSE, RESPONSE_TYPE_NONE, NULL ),                    // 233
  CMDTBL_EXTNENT( FALSE, RESPONSE_TYPE_NONE, NULL ),                    // 234
  CMDTBL_EXTNENT( FALSE, RESPONSE_TYPE_NONE, NULL ),                    // 235
  CMDTBL_EXTNENT( FALSE, RESPONSE_TYPE_NONE, NULL ),                    // 236
  CMDTBL_EXTNENT( FALSE, RESPONSE_TYPE_SEND, CmdQryGearType ),          // 237
  CMDTBL_EXTNENT( FALSE, RESPONSE_TYPE_SEND, CmdQryDimCurve ),          // 238
  CMDTBL_EXTNENT( FALSE, RESPONSE_TYPE_SEND, CmdQryPossibleOpModes ),   // 239
  CMDTBL_EXTNENT( FALSE, RESPONSE_TYPE_SEND, CmdQryFeatures ),          // 240
  CMDTBL_EXTNENT( FALSE, RESPONSE_TYPE_NONE, CmdQryFailiureStatus ),    // 241
  CMDTBL_EXTNENT( FALSE, RESPONSE_TYPE_NONE, CmdQryShortCircuit ),      // 242
  CMDTBL_EXTNENT( FALSE, RESPONSE_TYPE_NONE, CmdQryOpenCircuit ),       // 243
  CMDTBL_EXTNENT( FALSE, RESPONSE_TYPE_NONE, CmdQryLoadDec ),           // 244
  CMDTBL_EXTNENT( FALSE, RESPONSE_TYPE_NONE, CmdQryLoadInc ),           // 245
  CMDTBL_EXTNENT( FALSE, RESPONSE_TYPE_NONE, CmdQryCurProtectActive ),  // 246
  CMDTBL_EXTNENT( FALSE, RESPONSE_TYPE_NONE, CmdQryThermalShutdown ),   // 247
  CMDTBL_EXTNENT( FALSE, RESPONSE_TYPE_NONE, CmdQryThermalOverload ),   // 248
  CMDTBL_EXTNENT( FALSE, RESPONSE_TYPE_NONE, CmdQryRefRunning ),        // 249
  CMDTBL_EXTNENT( FALSE, RESPONSE_TYPE_NONE, CmdQryRefMeasureFail ),    // 250
  CMDTBL_EXTNENT( FALSE, RESPONSE_TYPE_NONE, CmdQryCurProtectEnabled ), // 251  
  CMDTBL_EXTNENT( FALSE, RESPONSE_TYPE_SEND, CmdQryOpMode ),            // 252
  CMDTBL_EXTNENT( FALSE, RESPONSE_TYPE_SEND, CmdQryFastFadeTime ),      // 253
  CMDTBL_EXTNENT( FALSE, RESPONSE_TYPE_SEND, CmdQryMinFastFadeTime ),   // 254
  CMDTBL_EXTNENT( FALSE, RESPONSE_TYPE_SEND, CmdQryExtVerNumber ),      // 255
};

/******************************************************************************
 * @function NormalCommands_Initialize
 *
 * @brief	initialize the normal command handler
 *
 * This function will perform any neccesary intialization for the normal
 * command handler
 *
 *****************************************************************************/
void NormalCommands_Initialize( void )
{
}

/******************************************************************************
 * @function CmdOff
 *
 * @brief set the level to 0
 *
 * This function will set the actual level to 0
 *
 *****************************************************************************/
static void	CmdOff( PDALICTL ptDaliCtl )
{
  // set the actual level to 0
  ptDaliCtl->nActualDimLevel = 0;
  ptDaliCtl->tDaliSts.tBits.bLimitError = FALSE;
}

/******************************************************************************
 * @function CmdUp
 *
 * @brief up output
 *
 * This function will increase the output and will apply the rade rate
 *
 *****************************************************************************/
static void	CmdUp( PDALICTL ptDaliCtl )
{
  // check if lamp is on and fade rate is not equal to 0
  if (( ptDaliCtl->tDaliPrm.nFadeRate != 0 ) && ( ptDaliCtl->nActualDimLevel >= ptDaliCtl->tDaliPrm.nMinLevel ))
  {
    // check for valid requested rate
    if ( ptDaliCtl->nActualDimLevel < ptDaliCtl->tDaliPrm.nMaxLevel )
    {
      // set the fade increment amount to Microseconds per step  ( fade rate = steps/s * 10 )
      ptDaliCtl->uFadeIncrement = 10000000 / awFadeRates[ ptDaliCtl->tDaliPrm.nFadeRate ];    

      // set the fade period to 200ms/set the fade direction
      ptDaliCtl->uFadePeriod = UP_DOWN_FADE_TIME;
      ptDaliCtl->eFadeDir = FADE_DIR_UP;
      ptDaliCtl->tFlags.bFadeRateReq = TRUE;
    }
    else
    {
      // set the fade period to 0
      ptDaliCtl->uFadePeriod = 0l;
    }
  }
  else
  {
    // set the fade period to 0
    ptDaliCtl->uFadePeriod = 0l;
  }
}

/******************************************************************************
 * @function CmdDown
 *
 * @brief down output
 *
 * This function will decrease the output and will apply the ramp rate
 *
 *****************************************************************************/
static void	CmdDown( PDALICTL ptDaliCtl )
{
  // check if lamp is on and fade rate is not equal to 0
  if (( ptDaliCtl->tDaliPrm.nFadeRate != 0 ) && ( ptDaliCtl->nActualDimLevel >= ptDaliCtl->tDaliPrm.nMinLevel ))
  {
    // check for valid requested rate
    if ( ptDaliCtl->nActualDimLevel > ptDaliCtl->tDaliPrm.nMinLevel )
    {
      // set the fade increment amount to Microseconds per step  ( fade rate = steps/s * 10 )
      ptDaliCtl->uFadeIncrement = 10000000 / awFadeRates[ ptDaliCtl->tDaliPrm.nFadeRate ];   
      
      // set the fade period to 200ms/set the fade direction
      ptDaliCtl->uFadePeriod = UP_DOWN_FADE_TIME;
      ptDaliCtl->eFadeDir = FADE_DIR_DN;
      ptDaliCtl->tFlags.bFadeRateReq = TRUE;
    }
    else
    {
      // set the fade period to 0
      ptDaliCtl->uFadePeriod = 0l;
    }
  }
  else
  {
    // set the fade period to 0
    ptDaliCtl->uFadePeriod = 0l;
  }
}

/******************************************************************************
 * @function CmdStepUp
 *
 * @brief step up the actual level
 *
 * This function will increase the actual dim level
 *
 *****************************************************************************/
static void	CmdStepUp( PDALICTL ptDaliCtl )
{
  // check to make sure we are not at max and that the lamp is on
  if (( ptDaliCtl->nActualDimLevel < ptDaliCtl->tDaliPrm.nMaxLevel ) && ( ptDaliCtl->nActualDimLevel >= ptDaliCtl->tDaliPrm.nMinLevel ))
  {
    // increase the actual level
    ptDaliCtl->nActualDimLevel++;
  }
}

/******************************************************************************
 * @function CmdStepDown
 *
 * @brief step down the actual level
 *
 * This function will decrease the actual dim level
 *
 *****************************************************************************/
static void	CmdStepDown( PDALICTL ptDaliCtl )
{
  // check to make sure we are not at max and that the lamp is on
  if (( ptDaliCtl->nActualDimLevel > ptDaliCtl->tDaliPrm.nMinLevel ) && ( ptDaliCtl->nActualDimLevel >= ptDaliCtl->tDaliPrm.nMinLevel ))
  {
    // increaset the actual level
    ptDaliCtl->nActualDimLevel--;
  }
}

/******************************************************************************
 * @function CmdRecallMaxLvl
 *
 * @brief recall the max level
 *
 * This function will set the actual level to the max level
 *
 *****************************************************************************/
static void	CmdRecallMaxLvl( PDALICTL ptDaliCtl )
{
  // set the actual level to the stored max level
  ptDaliCtl->nActualDimLevel = ptDaliCtl->tDaliPrm.nMaxLevel;
  ptDaliCtl->tDaliSts.tBits.bLimitError = FALSE;
}

/******************************************************************************
 * @function CmdRecallMinLvl
 *
 * @brief recall the min level
 *
 * This function will set the actual level to the min level
 *
 *****************************************************************************/
static void	CmdRecallMinLvl( PDALICTL ptDaliCtl )
{
  // set the actual level to the stored min level
  ptDaliCtl->nActualDimLevel = ptDaliCtl->tDaliPrm.nMinLevel;
  ptDaliCtl->tDaliSts.tBits.bLimitError = FALSE;
}

/******************************************************************************
 * @function CmdStepDownOff
 *
 * @brief step down the level and go to off if required
 *
 * This function will step down the actual level if below the minimum will
 * set to off
 *
 *****************************************************************************/
static void	CmdStepDownOff( PDALICTL ptDaliCtl )
{
  // lamp has to be on
  if ( ptDaliCtl->nActualDimLevel >= ptDaliCtl->tDaliPrm.nMinLevel )
  {
    // are we about min level
    if ( ptDaliCtl->nActualDimLevel > ptDaliCtl->tDaliPrm.nMinLevel )
    {
      // decrement the actual level
      ptDaliCtl->nActualDimLevel--;
    }
    else
    {
      // set to off
      ptDaliCtl->nActualDimLevel = 0;
    }
  }
}

/******************************************************************************
 * @function CmdOnStepUp
 *
 * @brief step up and go to min level if below
 *
 * This function will step up the actual level and if above the minimum will
 * set to the in level
 *
 *****************************************************************************/
static void	CmdOnStepUp( PDALICTL ptDaliCtl )
{
  // lamp has to be on
  if ( ptDaliCtl->nActualDimLevel >= ptDaliCtl->tDaliPrm.nMinLevel )
  {
    // are we below max level
    if ( ptDaliCtl->nActualDimLevel < ptDaliCtl->tDaliPrm.nMaxLevel )
    {
      //  increment the actual level
      ptDaliCtl->nActualDimLevel++;
    }
  }
  else
  {
    // set to min level
    ptDaliCtl->nActualDimLevel = ptDaliCtl->tDaliPrm.nMinLevel;
  }
}

/******************************************************************************
 * @function
 *
 * @brief
 *
 * This function will
 *
 *****************************************************************************/
static void	CmdEnbDAPCSeq( PDALICTL ptDaliCtl )
{
  if ( ptDaliCtl->tFlags.bDAPCSequenceRunning == FALSE )
  {
    // set the fade time to 1
    ptDaliCtl->nCurFadeTime = ptDaliCtl->tDaliPrm.nFadeTime;
    ptDaliCtl->tDaliPrm.nFadeTime = 1;
  }
 
  // set the DAPC sequence running flag
  ptDaliCtl->tFlags.bDAPCSequenceRunning = TRUE;
}

/******************************************************************************
 * @function CmdScene
 *
 * @brief set the scenes level
 *
 * This function will set up for a ramp to the desired value based on the
 * scene selection
 *
 *****************************************************************************/
static void	CmdScene( PDALICTL ptDaliCtl )
{
  // set the requested level
  ptDaliCtl->nRequestedLevel = ptDaliCtl->tDaliPrm.anScenes[ ptDaliCtl->tCurMessage.tFields.nData & 0x0F ];
  ptDaliCtl->tFlags.bOutputChangeRequested = TRUE;
}

/******************************************************************************
 * @function
 *
 * @brief
 *
 * This function will
 *
 *****************************************************************************/
static void	CmdReset( PDALICTL ptDaliCtl )
{
  // set the reset requested flag
  ptDaliCtl->tFlags.bResetRequested = TRUE;
}

/******************************************************************************
 * @function CmdStoreLevelInDtr
 *
 * @brief store the actual level into the DTR
 *
 * This function will store the actual level into the DTR
 *
 *****************************************************************************/
static void	CmdStoreLevelInDtr( PDALICTL ptDaliCtl )
{
  // copy the value into the DTR
  ptDaliCtl->nDataTransReg = ptDaliCtl->nActualDimLevel;
}

/******************************************************************************
 * @function CmdStoreDtrMaxLvl
 *
 * @brief store the DTR as max level
 *
 * This function will take the current DTR level and update the max level
 *
 *****************************************************************************/
static void	CmdStoreDtrMaxLvl( PDALICTL ptDaliCtl )
{
  PARAMARG xValue;
  
  // copy the DTR into the max level
  ptDaliCtl->tDaliPrm.nMaxLevel = ptDaliCtl->nDataTransReg;

  // now check for valid
  if ( ptDaliCtl->tDaliPrm.nMaxLevel <= ptDaliCtl->tDaliPrm.nMinLevel )
  {
    // set the max level to in level
    ptDaliCtl->tDaliPrm.nMaxLevel = ptDaliCtl->tDaliPrm.nMinLevel;
  }

  // check for the actual value below or equal to the max level
  if ( ptDaliCtl->nActualDimLevel >= ptDaliCtl->tDaliPrm.nMaxLevel )
  {
    // set the actual level to the new max level
    ptDaliCtl->nActualDimLevel = ptDaliCtl->tDaliPrm.nMaxLevel;
  }

  // check for DTR = MASK (0xFF)
  if ( ptDaliCtl->tDaliPrm.nMaxLevel == 0xFF )
  {
    // set the max level to 0xFE (254)
    ptDaliCtl->tDaliPrm.nMaxLevel = 0xFE;
  }

  // compare to reset state value
  ParameterManager_GetDefaultValue( PARAMSEL_DALI_MAX_LEVEL, &xValue );
  ptDaliCtl->tResetState.tBits.bMaxLevel = ( ptDaliCtl->tDaliPrm.nMaxLevel != xValue ) ? TRUE : FALSE;

  // update the NVM
  ParameterManager_PutValue( PARAMSEL_DALI_MAX_LEVEL, ptDaliCtl->tDaliPrm.nMaxLevel );
}

/******************************************************************************
 * @function CmdStoreDtrMinLvl
 *
 * @brief store the DTR as min level
 *
 * This function will take the current DTR level and update the min level
 *
 *****************************************************************************/
static void	CmdStoreDtrMinLvl( PDALICTL ptDaliCtl )
{
  PARAMARG xValue;

  // copy the DTR into the max level
  ptDaliCtl->tDaliPrm.nMinLevel = ptDaliCtl->nDataTransReg;

  // now check for valid
  if ( ptDaliCtl->tDaliPrm.nMinLevel >= ptDaliCtl->tDaliPrm.nMaxLevel )
  {
    // set the min level to max level
    ptDaliCtl->tDaliPrm.nMinLevel = ptDaliCtl->tDaliPrm.nMaxLevel;
  }
  
  //  check for min level less than physical minimum level
  if ( ptDaliCtl->tDaliPrm.nMinLevel < ptDaliCtl->nPhysicalMinLvl )
  {
    ptDaliCtl->tDaliPrm.nMinLevel = ptDaliCtl->nPhysicalMinLvl;
  }

  // check for the actual value below to the min level
  if ( ptDaliCtl->nActualDimLevel < ptDaliCtl->tDaliPrm.nMinLevel )
  {
    // set the actual level to the new min level
    ptDaliCtl->nActualDimLevel = ptDaliCtl->tDaliPrm.nMinLevel;
  }

  // compare to reset state value
  ParameterManager_GetDefaultValue( PARAMSEL_DALI_MIN_LEVEL, &xValue );
  ptDaliCtl->tResetState.tBits.bMinLevel = ( ptDaliCtl->tDaliPrm.nMinLevel != xValue ) ? TRUE : FALSE;

  // update the NVM
  ParameterManager_PutValue( PARAMSEL_DALI_MIN_LEVEL, ptDaliCtl->tDaliPrm.nMinLevel );
}

/******************************************************************************
 * @function CmdStoreDtrSysFailLvl
 *
 * @brief store the DTR as system fail level
 *
 * This function will will udpate the system fail level with the value stored
 * in the DTR
 *
 *****************************************************************************/
static void	CmdStoreDtrSysFailLvl( PDALICTL ptDaliCtl )
{
  PARAMARG xValue;

  // update the value
  ptDaliCtl->tDaliPrm.nSystemFailureLevel = ptDaliCtl->nDataTransReg;
  
  // compare to reset state value
  ParameterManager_GetDefaultValue( PARAMSEL_DALI_SYSFAIL_LEVEL, &xValue );
  ptDaliCtl->tResetState.tBits.bSystemFailureLevel = ( ptDaliCtl->tDaliPrm.nSystemFailureLevel != xValue ) ? TRUE : FALSE;
  
  // update NVM
  ParameterManager_PutValue( PARAMSEL_DALI_SYSFAIL_LEVEL, ptDaliCtl->tDaliPrm.nSystemFailureLevel );
}

/******************************************************************************
 * @function CmdStoreDtrPwrOnLvl
 *
 * @brief store the DTR as the power on level
 *
 * This function will update the power on level with the current value  stroed
 * in the DTR
 *
 *****************************************************************************/
static void	CmdStoreDtrPwrOnLvl( PDALICTL ptDaliCtl )
{
  PARAMARG xValue;

  // update the value
  ptDaliCtl->tDaliPrm.nPowerOnLevel = ptDaliCtl->nDataTransReg;

  // compare to reset state value
  ParameterManager_GetDefaultValue( PARAMSEL_DALI_POWERON_LEVEL, &xValue );
  ptDaliCtl->tResetState.tBits.bPowerOnLevel = ( ptDaliCtl->tDaliPrm.nPowerOnLevel != xValue ) ? TRUE : FALSE;

  // update NVM
  ParameterManager_PutValue( PARAMSEL_DALI_POWERON_LEVEL, ptDaliCtl->tDaliPrm.nPowerOnLevel );
}

/******************************************************************************
 * @function CmdStoreDtrFadeTime
 *
 * @brief store the DTR as the fade time
 *
 * This function will update the fade time with the current value stored
 * in the DTR
 *
 *****************************************************************************/
static void	CmdStoreDtrFadeTime( PDALICTL ptDaliCtl )
{
  PARAMARG xValue;

  // update the value
  ptDaliCtl->tDaliPrm.nFadeTime = MIN( ptDaliCtl->nDataTransReg, 0x0F );

  // Compare to reset state value
  ParameterManager_GetDefaultValue( PARAMSEL_DALI_FADE_TIME, &xValue );
  ptDaliCtl->tResetState.tBits.bFadeTime = ( ptDaliCtl->tDaliPrm.nFadeTime != xValue ) ? TRUE : FALSE;

  // update the NVM
  ParameterManager_PutValue( PARAMSEL_DALI_FADE_TIME, ptDaliCtl->tDaliPrm.nFadeTime );
}

/******************************************************************************
 * @function CmdStoreDtrFadeRate
 *
 * @brief store the DTR as fade rate
 *
 * This function will update the fade rate with the current value stored
 * in the DTR
 *
 *****************************************************************************/
static void	CmdStoreDtrFadeRate( PDALICTL ptDaliCtl )
{
  PARAMARG xValue;

  // update the value
  ptDaliCtl->tDaliPrm.nFadeRate = MIN( ptDaliCtl->nDataTransReg, 0x0F );
  ptDaliCtl->tDaliPrm.nFadeRate = MAX( ptDaliCtl->tDaliPrm.nFadeRate, 1 );
  
  // Compare to reset state value
  ParameterManager_GetDefaultValue( PARAMSEL_DALI_FADE_RATE, &xValue );
  ptDaliCtl->tResetState.tBits.bFadeRate = ( ptDaliCtl->tDaliPrm.nFadeRate != xValue ) ? TRUE : FALSE;

  // update the NVM
  ParameterManager_PutValue( PARAMSEL_DALI_FADE_RATE, ptDaliCtl->tDaliPrm.nFadeRate );
}

/******************************************************************************
 * @function CmdStoreDtrScene
 *
 * @brief store the DTR as the scene
 *
 * This function will store the DTR as the scene value
 *
 *****************************************************************************/
static void	CmdStoreDtrScene( PDALICTL ptDaliCtl )
{
  U8	nScene;

  // get the scene index/store the DTR
  nScene = ptDaliCtl->tCurMessage.tFields.nData & 0x0F;
  ptDaliCtl->tDaliPrm.anScenes[ nScene ] = ptDaliCtl->nDataTransReg;

  // set the appropriate scene bit in reset indicator structure
  if ( ptDaliCtl->tDaliPrm.anScenes[ nScene ] != SCENE_RESET )
  {
    // set scene modified bit
    ptDaliCtl->tResetState.tBits.wScene |= BIT( nScene );
  }
  else 
  {
    // clear scene modified bit
    ptDaliCtl->tResetState.tBits.wScene &= ~BIT( nScene );
  }

  // update the NVM
  ParameterManager_PutValue( PARAMSEL_DALI_SCENE00 + nScene, ptDaliCtl->tDaliPrm.anScenes[ nScene ] );
}

/******************************************************************************
 * @function CmdRemoveScene
 *
 * @brief clear a scene's value
 *
 * This function will set the scenes value to none and update the NVM
 *
 *****************************************************************************/
static void	CmdRemoveScene( PDALICTL ptDaliCtl )
{
  U8	nScene;

  // get the scene index/store the DTR
  nScene = ptDaliCtl->tCurMessage.tFields.nData & 0x0F;
  ptDaliCtl->tDaliPrm.anScenes[ nScene ] = 0xFF;

  // clear scene modified bit
  ptDaliCtl->tResetState.tBits.wScene &= ~BIT( nScene );

  // update the NVM
  ParameterManager_PutValue( PARAMSEL_DALI_SCENE00 + nScene, ptDaliCtl->tDaliPrm.anScenes[ nScene ] );
}

/******************************************************************************
 * @function CmdAddToGroup
 *
 * @brief add this device to the indicated group
 *
 * This function will set the appropriate group bit based on the data
 *
 *****************************************************************************/
static void	CmdAddToGroup( PDALICTL ptDaliCtl )
{
  U16	wGroupMask;

  // compute the group mask
  wGroupMask = BIT(( ptDaliCtl->tCurMessage.tFields.nData & 0x0F ));
  ptDaliCtl->tDaliPrm.tGroup.wValue |= wGroupMask;

  // set the reset bit to true - indicating groups are not in reset state
  ptDaliCtl->tResetState.tBits.bGroup = TRUE;

  // store the new group value
  ParameterManager_PutValue( PARAMSEL_DALI_GROUP0007, ptDaliCtl->tDaliPrm.tGroup.anValue[ LE_U16_LSB_IDX ] );
  ParameterManager_PutValue( PARAMSEL_DALI_GROUP0815, ptDaliCtl->tDaliPrm.tGroup.anValue[ LE_U16_MSB_IDX ] );
}

/******************************************************************************
 * @function CmdRemoveFromGroup
 *
 * @brief remove this device from the group
 *
 * This function will clear the group flag
 *
 *****************************************************************************/
static void	CmdRemoveFromGroup( PDALICTL ptDaliCtl )
{
  U16	wGroupMask;

  // compute the group mask
  wGroupMask = BIT(( ptDaliCtl->tCurMessage.tFields.nData & 0x0F ));
  ptDaliCtl->tDaliPrm.tGroup.wValue &= ~wGroupMask;

  // if control gear is not a part of a group, clear reset bit
  ptDaliCtl->tResetState.tBits.bGroup = ( ptDaliCtl->tDaliPrm.tGroup.wValue != 0 ) ? TRUE : FALSE;

  // store the new group value
  ParameterManager_PutValue( PARAMSEL_DALI_GROUP0007, ptDaliCtl->tDaliPrm.tGroup.anValue[ LE_U16_LSB_IDX ] );
  ParameterManager_PutValue( PARAMSEL_DALI_GROUP0815, ptDaliCtl->tDaliPrm.tGroup.anValue[ LE_U16_MSB_IDX ] );
}

/******************************************************************************
 * @function CmdStoreDtrAddress
 *
 * @brief store the DTR as the short address
 *
 * This function will update the short address with the value stored in
 * the DTR.
 *
 *****************************************************************************/
static void	CmdStoreDtrAddress( PDALICTL ptDaliCtl )
{
  // check for a valid address
  if ( ptDaliCtl->nDataTransReg == 0xFF )
  {
    // set the address
    ptDaliCtl->tDaliPrm.nShortAddress = 0xFF;
  }
  else if (( ptDaliCtl->nDataTransReg <= 127 ) && (( ptDaliCtl->nDataTransReg & 0x01 ) != 0))
  {
    // set the address
    ptDaliCtl->tDaliPrm.nShortAddress = (( ptDaliCtl->nDataTransReg >> 1 ) & 0x3F );
  }

  // update the status flag/NVM
  ptDaliCtl->tDaliSts.tBits.bMissingShortAddr = ( ptDaliCtl->tDaliPrm.nShortAddress == 0xFF ) ? TRUE : FALSE;
  ParameterManager_PutValue( PARAMSEL_DALI_SHORT_ADDR, ptDaliCtl->tDaliPrm.nShortAddress );
}

/******************************************************************************
 * @function EnableWriteMemory
 *
 * @brief enable the write to memory
 *
 * This function will allow the write to memory
 *
 *****************************************************************************/
static void	CmdEnableWriteMemory( PDALICTL ptDaliCtl )
{
  // set the enable write to memory flag
  ptDaliCtl->tFlags.bWriteMemoryEnabled = TRUE;
}

/******************************************************************************
 * @function CmdQryStatus
 *
 * @brief query the current device status1
 *
 * This function will reutn the current value of the system status
 *
 *****************************************************************************/
static void	CmdQryStatus( PDALICTL ptDaliCtl )
{
  // determine if control gear is in reset state
  if ( ( ptDaliCtl->tResetState.uWord == 0 ) && ( ( ptDaliCtl->tDaliSts.nByte & STATUS_RESET_MASK ) == 0 ) )
  {
    ptDaliCtl->tDaliSts.tBits.bResetState = TRUE;
  }
  else
  {
    ptDaliCtl->tDaliSts.tBits.bResetState = FALSE;
  }
  
  // set the return value
  ptDaliCtl->nResponseValue = ptDaliCtl->tDaliSts.nByte;
}

/******************************************************************************
 * @function CmdQryBallast
 *
 * @brief query this device's ability to communicate
 *
 * This function will always return a yes in the response value
 *
 *****************************************************************************/
static void	CmdQryBallast( PDALICTL ptDaliCtl )
{
  // set the return value
  ptDaliCtl->nResponseValue = DALI_RESPONSE_YES;
}

/******************************************************************************
 * @function CmdQryLampFail
 *
 * @brief return the lamp fail status
 *
 * This function will return the lamp failure status
 *
 *****************************************************************************/
static void	CmdQryLampFail( PDALICTL ptDaliCtl )
{
  // set the return value
  ptDaliCtl->nResponseValue = ( ptDaliCtl->tDaliSts.tBits.bLampFailure ) ? DALI_RESPONSE_YES : DALI_RESPONSE_NO;
  ptDaliCtl->tFlags.bResponseRequested = ( ptDaliCtl->nResponseValue != DALI_RESPONSE_NO ) ? TRUE : FALSE;
}

/******************************************************************************
 * @function CmdQryLampPower
 *
 * @brief query the lamp power
 *
 * This function will return the lamp power status
 *
 *****************************************************************************/
static void	CmdQryLampPower( PDALICTL ptDaliCtl )
{
  // set the return value
  ptDaliCtl->nResponseValue = ( ptDaliCtl->tDaliSts.tBits.bLampOn == TRUE ) ? DALI_RESPONSE_YES : DALI_RESPONSE_NO;
  ptDaliCtl->tFlags.bResponseRequested = ( ptDaliCtl->nResponseValue != DALI_RESPONSE_NO ) ? TRUE : FALSE;
}

/******************************************************************************
 * @function CmdQryLimitError
 *
 * @brief query the limit error flag
 *
 * This function will return the current status of the limit error
 *
 *****************************************************************************/
static void	CmdQryLimitError( PDALICTL ptDaliCtl )
{
  // set the return value
  ptDaliCtl->nResponseValue = ( ptDaliCtl->tDaliSts.tBits.bLimitError == TRUE ) ? DALI_RESPONSE_YES : DALI_RESPONSE_NO;
  ptDaliCtl->tFlags.bResponseRequested = ( ptDaliCtl->nResponseValue != DALI_RESPONSE_NO ) ? TRUE : FALSE;
}

/******************************************************************************
 * @function CmdQryResetState
 *
 * @brief query the reset state
 *
 * This function will return the current state of the reset
 *
 *****************************************************************************/
static void	CmdQryResetState( PDALICTL ptDaliCtl )
{
  // set the return value

  // determine if control gear is in reset state
  if ( ( ptDaliCtl->tResetState.uWord == 0 ) && ( ( ptDaliCtl->tDaliSts.nByte & STATUS_RESET_MASK ) == 0 ) )
  {
    // set the reset state
    ptDaliCtl->tDaliSts.tBits.bResetState = TRUE;
  }
  else
  {
    // clear the reset state
    ptDaliCtl->tDaliSts.tBits.bResetState = FALSE;
  }

  // set the response value/flag
  ptDaliCtl->nResponseValue = ( ptDaliCtl->tDaliSts.tBits.bResetState == TRUE ) ? DALI_RESPONSE_YES : DALI_RESPONSE_NO;
  ptDaliCtl->tFlags.bResponseRequested = ( ptDaliCtl->nResponseValue != DALI_RESPONSE_NO ) ? TRUE : FALSE;
}

/******************************************************************************
 * @function CmdQryMissingAddress
 *
 * @brief query the missing address status
 *
 * This function will return the current status of the missing address
 *
 *****************************************************************************/
static void	CmdQryMissingAddress( PDALICTL ptDaliCtl )
{
  // set the return value
  ptDaliCtl->nResponseValue = ( ptDaliCtl->tDaliSts.tBits.bMissingShortAddr == TRUE ) ? DALI_RESPONSE_YES : DALI_RESPONSE_NO;
  ptDaliCtl->tFlags.bResponseRequested = ( ptDaliCtl->nResponseValue != DALI_RESPONSE_NO ) ? TRUE : FALSE;
}

/******************************************************************************
 * @function CmdQryVersionNumber
 *
 * @brief query the version number
 *
 * This function will return the current version number
 *
 *****************************************************************************/
static void	CmdQryVersionNumber( PDALICTL ptDaliCtl )
{
  // set the return value
  ptDaliCtl->nResponseValue = DALI_VERSION_NUMBER;
}

/******************************************************************************
 * @function CmdQryContentDtr
 *
 * @brief query the contents of the DTR
 *
 * This function will return the value of the DTR
 *
 *****************************************************************************/
static void	CmdQryContentDtr( PDALICTL ptDaliCtl )
{
  // set the return value to the DTR
  ptDaliCtl->nResponseValue = ptDaliCtl->nDataTransReg;
}

/******************************************************************************
 * @function CmdQryDeviceType
 *
 * @brief query the device type
 *
 * This function will return the value of the device type
 *
 *****************************************************************************/
static void	CmdQryDeviceType( PDALICTL ptDaliCtl )
{
  // set the return value
  ptDaliCtl->nResponseValue = ptDaliCtl->nDeviceType;
}

/******************************************************************************
 * @function CmdQryPhysicalMinLvl
 *
 * @brief query the physical min level
 *
 * This function will retirm the physical minimum level
 *
 *****************************************************************************/
static void	CmdQryPhysicalMinLvl( PDALICTL ptDaliCtl )
{
  // set the return value
  ptDaliCtl->nResponseValue = ptDaliCtl->nPhysicalMinLvl;
}

/******************************************************************************
 * @function CmdQryPowerFailure
 *
 * @brief query the power failure status
 *
 * This function will return the current status of the power failure status
 *
 *****************************************************************************/
static void	CmdQryPowerFailure( PDALICTL ptDaliCtl )
{
  // set the return value
  ptDaliCtl->nResponseValue = ( ptDaliCtl->tDaliSts.tBits.bPowerFailure == TRUE ) ? DALI_RESPONSE_YES : DALI_RESPONSE_NO;
  ptDaliCtl->tFlags.bResponseRequested = ( ptDaliCtl->nResponseValue != DALI_RESPONSE_NO ) ? TRUE : FALSE;
}

/******************************************************************************
 * @function CmdQryContentDtr1
 *
 * @brief query the content of DTR 1
 *
 * This function will return the value of the DTR #1 register
 *
 *****************************************************************************/
static void	CmdQryContentDtr1( PDALICTL ptDaliCtl )
{
  // set the return value
  ptDaliCtl->nResponseValue = ptDaliCtl->nDataTransReg1;
}

/******************************************************************************
 * @function CmdQryContentDtr2
 *
 * @brief query the content of DTR 2
 *
 * This function will return the value of the DTR #2 register
 *
 *****************************************************************************/
static void	CmdQryContentDtr2( PDALICTL ptDaliCtl )
{
  // set the return value
  ptDaliCtl->nResponseValue = ptDaliCtl->nDataTransReg2;
}

/******************************************************************************
 * @function CmdQryActualLvl
 *
 * @brief query the actual level
 *
 * This function will return the value of the actual level
 *
 *****************************************************************************/
static void	CmdQryActualLvl( PDALICTL ptDaliCtl )
{
  // set the return value - set to mask - 0xFF if lamp failure is detected 
  ptDaliCtl->nResponseValue = ( ptDaliCtl->tDaliSts.tBits.bLampFailure == TRUE ) ? 0xFF : ptDaliCtl->nActualDimLevel;
}

/******************************************************************************
 * @function CmdQryMaxLvl
 *
 * @brief query the max level
 *
 * This function will return the value of the maximum level
 *
 *****************************************************************************/
static void	CmdQryMaxLvl( PDALICTL ptDaliCtl )
{
  // set the return value
  ptDaliCtl->nResponseValue = ptDaliCtl->tDaliPrm.nMaxLevel;
}

/******************************************************************************
 * @function CmdQryMinLvl
 *
 * @brief query the minimum level
 *
 * This function will reutrn the value of the minimum level
 *
 *****************************************************************************/
static void	CmdQryMinLvl( PDALICTL ptDaliCtl )
{
  // set the return value
  ptDaliCtl->nResponseValue = ptDaliCtl->tDaliPrm.nMinLevel;
}

/******************************************************************************
 * @function CmdQryPwrOnLvl
 *
 * @brief query the power on level
 *
 * This function will return the value of the power on level
 *
 *****************************************************************************/
static void	CmdQryPwrOnLvl( PDALICTL ptDaliCtl )
{
  // set the return value
  ptDaliCtl->nResponseValue = ptDaliCtl->tDaliPrm.nPowerOnLevel;
}

/******************************************************************************
 * @function CmdQrySysFailLvl
 *
 * @brief query the system fail level
 *
 * This function will return the value of the system fail level
 *
 *****************************************************************************/
static void	CmdQrySysFailLvl( PDALICTL ptDaliCtl )
{
  // set the return value
  ptDaliCtl->nResponseValue = ptDaliCtl->tDaliPrm.nSystemFailureLevel;
}

/******************************************************************************
 * @function CmdQryFadeTimeRate
 *
 * @brief query the fade time/rate
 *
 * This function will return the fade time in the upper four bits of the
 * of the return value and the fade rate in the lower four bits
 *
 *****************************************************************************/
static void	CmdQryFadeTimeRate( PDALICTL ptDaliCtl )
{
  // set the return value
  ptDaliCtl->nResponseValue = ptDaliCtl->tDaliPrm.nFadeTime << 4;
  ptDaliCtl->nResponseValue |= ( ptDaliCtl->tDaliPrm.nFadeRate & 0x0F );
}

/******************************************************************************
 * @function CmdQrySceneLvl
 *
 * @brief query the scene level
 *
 * This function will return the level of the designated scene
 *
 *****************************************************************************/
static void	CmdQrySceneLvl( PDALICTL ptDaliCtl )
{
  U8	nScene;

  // get the scene index/set the return value
  nScene = ptDaliCtl->tCurMessage.tFields.nData & 0x0F;
  ptDaliCtl->nResponseValue = ptDaliCtl->tDaliPrm.anScenes[ nScene ];
}

/******************************************************************************
 * @function CmdQryGroups0007
 *
 * @brief query the group 0 through 7
 *
 * This function will return the state of the groups 0 and 7
 *
 *****************************************************************************/
static void	CmdQryGroups0007( PDALICTL ptDaliCtl )
{
  // set the return value
  ptDaliCtl->nResponseValue = ptDaliCtl->tDaliPrm.tGroup.anValue[ LE_U16_LSB_IDX ];
}

/******************************************************************************
 * @function CmdQryGroups0815
 *
 * @brief query the group 8 through 15
 *
 * This function will return the state of the groups 8 and 15
 *
 *****************************************************************************/
static void	CmdQryGroups0815( PDALICTL ptDaliCtl )
{
  // set the return value
  ptDaliCtl->nResponseValue = ptDaliCtl->tDaliPrm.tGroup.anValue[ LE_U16_MSB_IDX ];
}

/******************************************************************************
 * @function CmdQryRandAddrH
 *
 * @brief query the random address high byte
 *
 * This function will return the value of the random address high byte
 *
 *****************************************************************************/
static void	CmdQryRandAddrH( PDALICTL ptDaliCtl )
{
  // set the return value
  ptDaliCtl->nResponseValue = ptDaliCtl->tDaliPrm.tRandomAddr.anValue[ RANDOM_ADDR_H ];
}

/******************************************************************************
 * @function CmdQryRandAddrM
 *
 * @brief query the random address mid byte
 *
 * This function will return the value of the random address mid byte
 *
 *****************************************************************************/
static void	CmdQryRandAddrM( PDALICTL ptDaliCtl )
{
  // set the return value
  ptDaliCtl->nResponseValue = ptDaliCtl->tDaliPrm.tRandomAddr.anValue[ RANDOM_ADDR_M ];
}

/******************************************************************************
 * @function CmdQryRandAddrL
 *
 * @brief query the random address low byte
 *
 * This function will return the value of the random address low byte
 *
 *****************************************************************************/
static void	CmdQryRandAddrL( PDALICTL ptDaliCtl )
{
  // set the return value
  ptDaliCtl->nResponseValue = ptDaliCtl->tDaliPrm.tRandomAddr.anValue[ RANDOM_ADDR_L ];
}

/******************************************************************************
 * @function CmdQryMemoryLoc
 *
 * @brief query a memory location
 *
 * This function will return the value of a memory bank, DTR1 is the memory
 * bank number, DTR is the address of the memory bank and the read value will
 * be stored in DTR2.  DTR will be incremented to the next location
 *
 *****************************************************************************/
static void	CmdQryMemoryLoc( PDALICTL ptDaliCtl )
{
  PARAMARG xValue;

  // clear the response requested flag
  ptDaliCtl->tFlags.bResponseRequested = FALSE;

  // read the parameter
  if ( ParameterManager_ReadMemoryBlock( ptDaliCtl->nDataTransReg1, ptDaliCtl->nDataTransReg, &xValue ) == PARAM_ERR_NONE )
  {
    //set the response value to the content of the memory block
    ptDaliCtl->nResponseValue = xValue;

    // increment the address
    ptDaliCtl->nDataTransReg++;

    // set the response requested flag to TRUE
    ptDaliCtl->tFlags.bResponseRequested = TRUE;
  }
  
  if ( ParameterManager_ReadMemoryBlock( ptDaliCtl->nDataTransReg1, ptDaliCtl->nDataTransReg, &xValue ) == PARAM_ERR_NONE )
  {
    // store the content of the next address in DTR2
    ptDaliCtl->nDataTransReg2 = xValue;
  }
}

/******************************************************************************
 * @function CmdRefSysPwr
 *
 * @brief NOT INCLUDED FEATURE
 *
 * This function will
 *
 *****************************************************************************/
static void CmdRefSysPwr( PDALICTL ptDaliCtl )             
{
  // set the response requested flag to false
  ptDaliCtl->tFlags.bResponseRequested = FALSE;
}

/******************************************************************************
 * @function CmdEnableCurProtect
 *
 * @brief NOT INCLUDED FEATURE
 *
 * This function will
 *
 *****************************************************************************/
static void CmdEnableCurProtect( PDALICTL ptDaliCtl )         
{
  // set the response requested flag to false
  ptDaliCtl->tFlags.bResponseRequested = FALSE;
}

/******************************************************************************
 * @function CmdDisableCurProtect
 *
 * @brief NOT INCLUDED FEATURE
 *
 * This function will
 *
 *****************************************************************************/
static void CmdDisableCurProtect( PDALICTL ptDaliCtl )         
{
  // set the response requested flag to false
  ptDaliCtl->tFlags.bResponseRequested = FALSE;
}

/******************************************************************************
 * @function CmdSelDimCurve
 *
 * @brief Select the Dimming Curve
 *
 * This function will set the dimming curve of the control gear in accordance to the DTR.
 * DTR = 1 sets the dimming curve to linear, and DTR = 0 sets the dimming curve to logarithmic.
 * Changing the dimming curve also sets the PHYSICAL MINIMUM LEVEL to correspond to the 
 * physical minimum light output, which should not be affect by the choice of dimming curve.
 *
 *****************************************************************************/
static void CmdSelDimCurve( PDALICTL ptDaliCtl )             
{
  // Log is 0 Lin is 1
  if (( ptDaliCtl->nDataTransReg == 0x01 ) || ( ptDaliCtl->nDataTransReg == 0x00 ))
  {  
    ptDaliCtl->tDaliPrm.tOpMode.tFields.bDimCurve = ( BOOL )ptDaliCtl->nDataTransReg;
    ParameterManager_PutValue( PARAMSEL_DALI_OPMODE, ptDaliCtl->tDaliPrm.tOpMode.nOpMode );
  }
}

/******************************************************************************
 * @function CmdStoreDtrFastFadetime
 *
 * @brief Set the Fast Fade Time to the value currently stored in DTR
 *
 * This function will store the content of the DTR as the new Fast Fade Time if the 
 * content of the DTR is 0 or MIN FAST FADE TIME to 27. If the value of the DTR is 
 * in between 0 and MIN FAST FADE TIME, then MIN FAST FADE TIME will be the new
 * Fast Fade Time. If the content of the DTR is greater than 27 then 27 is the new fast fade time.
 * The control gear should only use fast fade time if the fade time is 0.
 * 
 *****************************************************************************/
static void CmdStoreDtrFastFadetime( PDALICTL ptDaliCtl )    
{
  // if DTR = 0 or DTR is in between the Minimum Fast Fade Time and 27, then store the DTR as Fast Fade Time
  if (( ptDaliCtl->nDataTransReg == 0 ) || 
      (( ptDaliCtl->nDataTransReg >= ptDaliCtl->nMinFastFadeTime ) && ( ptDaliCtl->nDataTransReg <= 27 )))
  {
     // update the value
    ptDaliCtl->tDaliPrm.nFastFadeTime = ptDaliCtl->nDataTransReg;
  }
  // if DTR is in between 0 and Minimum Fast Fade time then the Fast Fade Time shall be the Minimum Fast Fade Time
  else if (( ptDaliCtl->nDataTransReg > 0 ) && ( ptDaliCtl->nDataTransReg < ptDaliCtl->nMinFastFadeTime ))
  {
    // update the value
    ptDaliCtl->tDaliPrm.nFastFadeTime = ptDaliCtl->nMinFastFadeTime;
  }
  // if DTR is greater than 27 then Fast Fade Time shall be 27
  else if ( ptDaliCtl->nDataTransReg > 27 )
  {
    // update the value
    ptDaliCtl->tDaliPrm.nFastFadeTime = 27;
  }

  // update the NVM
  ParameterManager_PutValue( PARAMSEL_DALI_FASTFADETIME, ptDaliCtl->tDaliPrm.nFastFadeTime );
}

/******************************************************************************
 * @function CmdQryGearType
 *
 * @brief Responds with the Gear Type Byte
 *
 *****************************************************************************/
static void CmdQryGearType( PDALICTL ptDaliCtl )             
{
  ptDaliCtl->nResponseValue = ptDaliCtl->tGearType.nGearType; 

}

/******************************************************************************
 * @function CmdQryDimCurve
 *
 * @brief Respond with the dimming curve in use
 *
 * This function will respond with the dimming curve in use.
 * 0 Means standard logarithmic dimming curve
 * 1 means linear dimming curve
 *
 *****************************************************************************/
static void CmdQryDimCurve( PDALICTL ptDaliCtl )             
{
  // Log is 0 Lin is 1
  ptDaliCtl->nResponseValue = ptDaliCtl->tDaliPrm.tOpMode.tFields.bDimCurve;
}

/******************************************************************************
 * @function CmdQryPossibleOpModes
 *
 * @brief respond with the POSSIBLE OPERATING MODES byte
 *
 *****************************************************************************/
static void CmdQryPossibleOpModes( PDALICTL ptDaliCtl )     
{
  // return the op types
  ptDaliCtl->nResponseValue = ptDaliCtl->tPossibleOpTypes.nPossibleOpTypes;
}

/******************************************************************************
 * @function CmdQryFeatures
 *
 * @brief Respond with the Features Byte
 *
 * This function will Respond with the Features Byte. The features byte gives information about 
 * implemented optional features whose status can be queried from the control gear.
 *
 *****************************************************************************/
static void CmdQryFeatures( PDALICTL ptDaliCtl )          
{
  ptDaliCtl->nResponseValue = ptDaliCtl->tFeatures.nFeatures;
}

/******************************************************************************
 * @function CmdQryFailiureStatus
 *
 * @brief Respond with the failure status byte.
 *
 * This function will respond with the failure status byte.
 *
 *****************************************************************************/
static void CmdQryFailiureStatus( PDALICTL ptDaliCtl )      
{
  ptDaliCtl->nResponseValue = ptDaliCtl->tDaliPrm.tFailureStatus.nFailureStatus;
}

/******************************************************************************
 * @function CmdQryShortCircuit
 *
 * @brief NOT INCLUDED FEATURE
 *
 * This function will
 *
 *****************************************************************************/
static void CmdQryShortCircuit( PDALICTL ptDaliCtl )         
{
  // set the response requested flag to false
  ptDaliCtl->tFlags.bResponseRequested = FALSE;
}

/******************************************************************************
 * @function CmdQryOpenCircuit
 *
 * @brief NOT INCLUDED FEATURE
 *
 * This function will
 *
 *****************************************************************************/
static void CmdQryOpenCircuit( PDALICTL ptDaliCtl )          
{
  // set the response requested flag to false
  ptDaliCtl->tFlags.bResponseRequested = FALSE;
}

/******************************************************************************
 * @function CmdQryLoadDec
 *
 * @brief NOT INCLUDED FEATURE
 *
 * This function will
 *
 *****************************************************************************/
static void CmdQryLoadDec( PDALICTL ptDaliCtl )            
{
  // set the response requested flag to false
  ptDaliCtl->tFlags.bResponseRequested = FALSE;
}

/******************************************************************************
 * @function CmdQryLoadInc
 *
 * @brief NOT INCLUDED FEATURE
 *
 * This function will
 *
 *****************************************************************************/
static void CmdQryLoadInc( PDALICTL ptDaliCtl )           
{
  // set the response requested flag to false
  ptDaliCtl->tFlags.bResponseRequested = FALSE;
}

/******************************************************************************
 * @function CmdQryCurProtectActive
 *
 * @brief NOT INCLUDED FEATURE
 *
 * This function will
 *
 *****************************************************************************/
static void CmdQryCurProtectActive( PDALICTL ptDaliCtl )     
{
  // set the response requested flag to false
  ptDaliCtl->tFlags.bResponseRequested = FALSE;
}

/******************************************************************************
 * @function CmdQryThermalShutdown
 *
 * @brief NOT INCLUDED FEATURE
 *
 * This function will
 *
 *****************************************************************************/
static void CmdQryThermalShutdown( PDALICTL ptDaliCtl )     
{
  // set the response requested flag to false
  ptDaliCtl->tFlags.bResponseRequested = FALSE;
}

/******************************************************************************
 * @function CmdQryThermalOverload
 *
 * @brief NOT INCLUDED FEATURE
 *
 * This function will
 *
 *****************************************************************************/
static void CmdQryThermalOverload( PDALICTL ptDaliCtl )      
{
  // set the response requested flag to false
  ptDaliCtl->tFlags.bResponseRequested = FALSE;
}

/******************************************************************************
 * @function CmdQryRefRunning
 *
 * @brief NOT INCLUDED FEATURE
 *
 * This function will
 *
 *****************************************************************************/
static void CmdQryRefRunning( PDALICTL ptDaliCtl )          
{
  // set the response requested flag to false
  ptDaliCtl->tFlags.bResponseRequested = FALSE;
}

/******************************************************************************
 * @function CmdQryRefMeasureFail
 *
 * @brief NOT INCLUDED FEATURE
 * 
 * This function will
 *
 *****************************************************************************/
static void CmdQryRefMeasureFail( PDALICTL ptDaliCtl )       
{
  // set the response requested flag to false
  ptDaliCtl->tFlags.bResponseRequested = FALSE;
}

/******************************************************************************
 * @function CmdQryCurProtectEnabled
 *
 * @brief NOT INCLUDED FEATURE
 *
 * This function will
 *
 *****************************************************************************/
static void CmdQryCurProtectEnabled( PDALICTL ptDaliCtl )    
{
  // set the response requested flag to false
  ptDaliCtl->tFlags.bResponseRequested = FALSE;
}

/******************************************************************************
 * @function CmdQryOpMode
 *
 * @brief respond with the OPERATION MODE byte
 *
 * This function will respond with the OPERATION MODE byte
 *
 *****************************************************************************/
static void CmdQryOpMode( PDALICTL ptDaliCtl )               
{
   ptDaliCtl->nResponseValue = ptDaliCtl->tDaliPrm.tOpMode.nOpMode;
}

/******************************************************************************
 * @function CmdQryFastFadeTime
 *
 * @brief respond with Fast Fade Time as 8 bit value
 *
 * This function will respond with Fast Fade Time as 8 bit value
 *
 *****************************************************************************/
static void CmdQryFastFadeTime( PDALICTL ptDaliCtl )         
{
   ptDaliCtl->nResponseValue = ptDaliCtl->tDaliPrm.nFastFadeTime;
}

/******************************************************************************
 * @function CmdQryMinFastFadeTime
 *
 * @brief respond with minimum Fast Fade Time as 8 bit value
 *
 * This function will respond with minimum Fast Fade Time as 8 bit value
 *
 *****************************************************************************/
static void CmdQryMinFastFadeTime( PDALICTL ptDaliCtl )      //Query Minimum Fast Fade Time
{
  ptDaliCtl->nResponseValue = ptDaliCtl->nMinFastFadeTime;
}

/******************************************************************************
 * @function CmdQryExtVerNumber
 *
 * @brief respond with "1"
 *
 * This function will respond with "1"
 *
 *****************************************************************************/
static  void  CmdQryExtVerNumber( PDALICTL ptDaliCtl )
{
  // set the return value
  ptDaliCtl->nResponseValue = 1;
}

/**@} EOF NormalCommands.c */
