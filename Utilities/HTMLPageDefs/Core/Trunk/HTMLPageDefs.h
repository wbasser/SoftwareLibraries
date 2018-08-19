/*****************************************************************************
//   $Workfile: pagdefs.h $
//    Function: page generic string definitions
//      Author: Bill Basser
//   $JustDate: $
//   $Revision: $
//
//	This document contains proprietary data and information of 
//  CyberIntegration LLC.  It is the exclusive property of 
//  CyberIntegration, LLC and will not be disclosed in any form to 
//  any party without prior written permission of CyberIntegration, LLC.
//  This document may not be reproduced or further used without the 
//  prior written permission of CyberIntegration, LLC.
//  
//  Copyright (C) 2009 CyberIntegration, LLC. All Rights Reserved
//
//  $History: pagdefs.h $
 * 
 ******************************************************************************/
 
#ifndef PAGDEFS_H
#define PAGDEFS_H

// library include file
#include "Types/types.h"

// define the color macro
#define		RGB(r,g,b)	((r<<16)|(g<<8)|(b))

// global string declarations
extern	const C8	g_szFmtStrn[ ];
extern	const C8	g_szFmtHexb[ ];
extern	const C8	g_szFmtHexw[ ];
extern	const C8	g_szFmtHexl[ ];
extern	const C8	g_szBtnsSub[ ];
extern	const C8	g_szBtnsSsz[ ];
extern	const C8	g_szClgrPrm[ ];
extern 	const C8	g_szFormAct[ ];
extern 	const C8	g_szFormEnd[ ];
extern 	const C8	g_szFormGet[ ];
extern 	const C8	g_szFormPut[ ];
extern 	const C8	g_szHtmlEbk[ ];
extern	const C8	g_szHtmlLbk[ ];
extern	const C8	g_szHtmlLnk[ ];
extern 	const C8	g_szHtmlQot[ ];
extern	const C8	g_szInptChk[ ];
extern	const C8	g_szInptNum[ ];
extern	const C8	g_szInptFlt[ ];
extern	const C8	g_szInptRad[ ];
extern	const C8	g_szInptTxt[ ];
extern	const C8	g_szInptH08[ ];
extern	const C8	g_szInptH16[ ];
extern	const C8	g_szImagSrc[ ];
extern 	const C8	g_szMetaBeg[ ];
extern 	const C8	g_szMetaCnt[ ];
extern 	const C8	g_szMetaEnd[ ];
extern 	const C8	g_szMetaExp[ ];
extern 	const C8	g_szMetaRef[ ];
extern	const C8	g_szMetaUrl[ ];
extern 	const C8	g_szNewLine[ ];
extern 	const C8	g_szPageBdb[ ];
extern 	const C8	g_szPageBde[ ];
extern	const C8	g_szPageBfn[ ];
extern 	const C8	g_szPageBrb[ ];
extern 	const C8	g_szPageBre[ ];
extern 	const C8	g_szPageBrs[ ];
extern 	const C8	g_szPageClr[ ];
extern	const C8	g_szPageCnb[ ];
extern	const C8	g_szPageCne[ ];
extern 	const C8	g_szPageDvb[ ];
extern 	const C8	g_szPageDve[ ];
extern 	const C8	g_szPageHdb[ ];
extern 	const C8	g_szPageHde[ ];
extern 	const C8	g_szPageH1b[ ];
extern 	const C8	g_szPageH1e[ ];
extern 	const C8	g_szPageH2b[ ];
extern 	const C8	g_szPageH2e[ ];
extern 	const C8	g_szPageHrw[ ];
extern 	const C8	g_szPageHtb[ ];
extern 	const C8	g_szPageHte[ ];
extern 	const C8	g_szPageSty[ ];
extern 	const C8	g_szPageTtb[ ];
extern 	const C8	g_szPageTte[ ];
extern 	const C8	g_szSlctBeg[ ];
extern 	const C8	g_szSlctEnd[ ];
extern 	const C8	g_szSlctOpt[ ];
extern	const C8	g_szStrnChk[ ];
extern	const C8	g_szStrnCon[ ];
extern	const C8	g_szStrnEmp[ ];
extern	const C8	g_szStrnSlc[ ];
extern 	const C8	g_szTablBeg[ ];
extern 	const C8	g_szTablCgb[ ];
extern 	const C8	g_szTablCge[ ];
extern 	const C8	g_szTablCsb[ ];
extern 	const C8	g_szTablEnd[ ];
extern 	const C8	g_szTablHdb[ ];
extern 	const C8	g_szTablHde[ ];
extern	const C8	g_szTablHdr[ ];
extern	const C8	g_szTablPrm[ ];
extern 	const C8	g_szTablRsb[ ];
extern 	const C8	g_szTablRwb[ ];
extern 	const C8	g_szTablRwe[ ];
extern 	const C8	g_szTablSep[ ];
extern 	const C8	g_szTablTdb[ ];
extern 	const C8	g_szTablTde[ ];
extern 	const C8	g_szTablTfb[ ];
extern 	const C8	g_szTablTfe[ ];
extern 	const C8	g_szTablThb[ ];
extern 	const C8	g_szTablThe[ ];

// fonts
extern	const C8	g_szFntTahoma[ ];

// parameter error
extern  const C8  g_szErrParam[ ];

// define the table entry macro
#define	TABLE_ENTRY_STRING(ptStream, szString)	\
	fputs( g_szTablTdb, ptStream ); 			\
	fputs( szString, ptStream ); 				\
	fputs( g_szTablTde, ptStream );

#define	TABLE_ENTRY_LED(ptStream, szLed) 		\
	fputs( g_szTablTdb, ptStream ); 			\
	fprintf( ptStream, g_szImagSrc, szLed );	\
	fputs( g_szTablTde, ptStream );
	
#define TABLE_ENTRY_BUTTON(ptStream, szCgi, szLabel)	\
	fputs( g_szTablTdb, ptStream );						\
	fprintf( ptStream, g_szBtnsSub, szCgi, szLabel );	\
	fputs( g_szTablTde, ptStream );
	
// global function prototypes

#endif
