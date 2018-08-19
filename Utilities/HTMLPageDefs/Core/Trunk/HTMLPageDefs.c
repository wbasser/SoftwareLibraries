/*****************************************************************************
//   $Workfile: pagdefs.c $
//    Function: page generic string implementations
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
//  $History: pagdefs.c $
 * 
 ******************************************************************************/

// system include files
#include <stdio.h>

// types include files
#include "Types/Types.h"

// local include files
#include "HTMLPageDefs/HTMLPageDefs.h"

// global string constants
const C8	g_szFmtStrn[ ]	= { "%s" };
const C8	g_szFmtHexb[ ]	= { "%02X" };
const C8	g_szFmtHexw[ ]	= { "%04X" };
const C8	g_szFmtHexl[ ]	= { "%08X" };
const C8	g_szBtnsSub[ ]	= { "<input type=\"submit\" value=\"%s\">" };
const C8	g_szBtnsSsz[ ]	= { "<input type=\"submit\" value=\"%s\" size=\"%s\">" };
const C8	g_szClgrPrm[ ]	= { "span=\"%d\" width=\"%dpx\">" };
const C8	g_szFormAct[ ] 	= { "<form action=\"%s\" method=\"%s\" >" };
const C8	g_szFormEnd[ ]	= { "</form>" };
const C8	g_szFormGet[ ]	= { "get" };
const C8	g_szFormPut[ ]	= { "put" };
const C8	g_szHtmlEbk[ ]	= { "\">" };
const C8	g_szHtmlLbk[ ]	= { ">" };
const C8	g_szHtmlLnk[ ]	= { "<p><a href=\"%s\">%s</a></p>" };
const C8	g_szHtmlQot[ ]	= { "\"" };
const C8	g_szImagSrc[ ]	= { "<img src=\"%s\"></img>" };
const C8	g_szInptChk[ ]	= { "<input type=\"checkbox\" name=\"%s\" value=\"%s\" %s>%s" };
const C8	g_szInptRad[ ]	= { "<input type=\"radio\" name=\"%s\" value=\"%d\" checked=\"%s\">%s" };
const C8	g_szInptNum[ ] 	= { "<input type=\"text\" name=\"%s\" size=\"%d\" value=\"%d\">" };
const C8	g_szInptFlt[ ]	= { "<input type=\"text\" name=\"%s\" size=\"%d\" value=\"%4.1f\">" };
const C8	g_szInptH08[ ] 	= { "<input type=\"text\" name=\"%s\" size=\"%d\" value=\"0x%02X\">" };
const C8	g_szInptH16[ ] 	= { "<input type=\"text\" name=\"%s\" size=\"%d\" value=\"0x%04X\">" };
const C8	g_szInptTxt[ ] 	= { "<input type=\"text\" name=\"%s\" size=\"%d\" value=\"%s\">" };
const C8	g_szMetaBeg[ ] 	= { "<meta http-equiv=\"" };
const C8	g_szMetaCnt[ ]	= { "\" content=\"" };
const C8	g_szMetaEnd[ ]	= { "></meta>" };
const C8	g_szMetaExp[ ]	= { "Expires" };
const C8	g_szMetaRef[ ]	= { "Refresh" };
const C8	g_szMetaUrl[ ]	= { " URL=\"%s\">" };
const C8	g_szNewLine[ ]	= { "\r\n" };
const C8	g_szPageBdb[ ]	= { "<body" };
const C8	g_szPageBde[ ] 	= { "</body>" };
const C8	g_szPageBfn[ ]	= { "<basefont size = \"%d\" color=\"#%06X\" face=\"%s\"/>" };
const C8	g_szPageBrb[ ]	= { "<br>" };
const C8	g_szPageBre[ ]	= { "</br>" };
const C8	g_szPageBrs[ ]	= { "<br />" };
const C8	g_szPageClr[ ] 	= { " bgcolor=\"#%06X\" text=\"#%06X\" link=\"#%06X\" vlink=\"#%06X\" alink=\"#%06X\"" };
const C8	g_szPageCnb[ ]	= { "<center>" };
const C8	g_szPageCne[ ]	= { "</center>" };
const C8	g_szPageDvb[ ]	= { "<div>" };
const C8	g_szPageDve[ ]	= { "</div>" };
const C8	g_szPageHdb[ ] 	= { "<head>" };
const C8	g_szPageHde[ ]	= { "</head>" };
const C8	g_szPageH1b[ ]	= { "<h1>" };
const C8	g_szPageH1e[ ]	= { "</h1>" };
const C8	g_szPageH2b[ ]	= { "<h2>" };
const C8	g_szPageH2e[ ]	= { "</h2>" };
const C8	g_szPageHrw[ ]	= { "<hr align=\"center\" size=\"%d\" color=\"#%06X\">" };
const C8	g_szPageHtb[ ]	= { "<html>" };
const C8	g_szPageHte[ ]	= { "</html>" };
const C8	g_szPageSty[ ] 	= { " style=\"MARGIN: 1em auto; FONT: 8px arial,sans-serif; TEXT-ALIGN: center\"" };
const C8	g_szPageTtb[ ]	= { "<title>" };
const C8	g_szPageTte[ ] 	= { "</title>" };
const C8	g_szSlctBeg[ ]	= { "<select name= \"%s\">" };
const C8	g_szSlctEnd[ ]	= { "</select>" };
const C8	g_szSlctOpt[ ]	= { "<option value= \"%s\" %s>%s</option>" };
const C8	g_szStrnChk[ ]	= { "checked" };
const C8	g_szStrnCon[ ]	= { "on" };
const C8	g_szStrnEmp[ ]	= { "" };
const C8	g_szStrnSlc[ ]	= { "selected=\"selected\"" };
const C8	g_szTablBeg[ ] 	= { "<table" };
const C8	g_szTablCgb[ ]	= { "<colgroup " };
const C8	g_szTablCge[ ] 	= { "</colgroup>" };
const C8	g_szTablCsb[ ]	= { "<th colspan=\"%d\">%s</th>" };
const C8	g_szTablEnd[ ] 	= { "</table>" };
const C8	g_szTablHdb[ ] 	= { "<th>" };
const C8	g_szTablHde[ ] 	= { "</th>" };
const C8	g_szTablHdr[ ]	= { "<tr><th height=\"%d\" align=\"center\" colspan=\"%d\">%s</th></tr>" };
const C8	g_szTablPrm[ ]	= { " border=\"%d\" cellspacing=\"%d\" bgcolor=\"#%06X\" width=\"%dpx\">" };
const C8	g_szTablRsb[ ]	= { "<th rowspan=\"" };
const C8	g_szTablRwb[ ] 	= { "<tr>" };
const C8	g_szTablRwe[ ] 	= { "</tr>" };
const C8	g_szTablSep[ ]	= { "<tr bgcolor=\"#%06X\"><td colspan=\"%d\"height=\"%d\"></td></tr>" };
const C8	g_szTablTdb[ ]	= { "<td>" };
const C8	g_szTablTde[ ]	= { "</td>" };
const C8	g_szTablTfb[ ]	= { "<tfoot>" };
const C8	g_szTablTfe[ ]	= { "</tfoot>" };
const C8	g_szTablThb[ ]	= { "<thead>" };
const C8	g_szTablThe[ ]	= { "</thead>" };

// fonts
const C8	g_szFntTahoma[ ]	= { "Tahoma" };

// parameter error
const C8  g_szErrParam[ ] = { "/prmerr.htm" };

