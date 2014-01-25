/*
 * Copyright 2009-2013 Olivier WENGER
 *     http://www.magicspark.org/ or http://oulydev.free.fr/
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

// ============================================================================
//
// This file are where the Conditions/Actions/Expressions are defined.
// See the Extension FAQ in this SDK for more info and where to download it
//
// ============================================================================

#include <Windows.h>

#include "common.h" // Common Include
#include "extmem.h"

// ============================================================================
//
// ACTIONS ROUTINES
// 
// ============================================================================

short WINAPI DLLExport Action_showHorzScrollBar(
    LPRDATA rdPtr,
    long param1,
    long param2 )
{
    if ( !ShowScrollBar(
        Extension_getFrameHandle( rdPtr ),
        SB_HORZ,
        param1 ? true : false ) )
    {
        Extension_setError( rdPtr, GetLastError() );
    }

	return 0;
}

short WINAPI DLLExport Action_showVertScrollBar(
    LPRDATA rdPtr,
    long param1,
    long param2 )
{
    if ( !ShowScrollBar(
        Extension_getFrameHandle( rdPtr ),
        SB_VERT,
        param1 ? true : false ) )
    {
        Extension_setError( rdPtr, GetLastError() );
    }

	return 0;
}

short WINAPI DLLExport Action_setHorzScrollBarPos(
    LPRDATA rdPtr,
    long param1,
    long param2 )
{
    SCROLLINFO scrollInfo;

    scrollInfo.cbSize = sizeof(SCROLLINFO);
    scrollInfo.fMask  = SIF_POS;
    scrollInfo.nPos   = param1;

    SetScrollInfo(
        Extension_getFrameHandle( rdPtr ),
        SB_HORZ,
        &scrollInfo,
        true );

    return 0;
}

short WINAPI DLLExport Action_setVertScrollBarPos(
    LPRDATA rdPtr,
    long param1,
    long param2 )
{
    SCROLLINFO scrollInfo;

    scrollInfo.cbSize = sizeof(SCROLLINFO);
    scrollInfo.fMask  = SIF_POS;
    scrollInfo.nPos   = param1;

    SetScrollInfo(
        Extension_getFrameHandle( rdPtr ),
        SB_VERT,
        &scrollInfo,
        true );

    return 0;
}

short WINAPI DLLExport Action_setHorzScrollBarRange(
    LPRDATA rdPtr,
    long param1,
    long param2 )
{
    SCROLLINFO scrollInfo;

    scrollInfo.cbSize = sizeof(SCROLLINFO);
    scrollInfo.fMask  = SIF_RANGE;
    scrollInfo.nMin   = param1;
    scrollInfo.nMax   = param2;

    SetScrollInfo(
        Extension_getFrameHandle( rdPtr ),
        SB_HORZ,
        &scrollInfo,
        true );

    return 0;
}

short WINAPI DLLExport Action_setVertScrollBarRange(
    LPRDATA rdPtr,
    long param1,
    long param2 )
{
    SCROLLINFO scrollInfo;

    scrollInfo.cbSize = sizeof(SCROLLINFO);
    scrollInfo.fMask  = SIF_RANGE;
    scrollInfo.nMin   = param1;
    scrollInfo.nMax   = param2;

    SetScrollInfo(
        Extension_getFrameHandle( rdPtr ),
        SB_VERT,
        &scrollInfo,
        true );

    return 0;
}

short WINAPI DLLExport Action_setHorzScrollBarPage(
    LPRDATA rdPtr,
    long param1,
    long param2 )
{
    SCROLLINFO scrollInfo;

    scrollInfo.cbSize = sizeof(SCROLLINFO);
    scrollInfo.fMask  = SIF_PAGE;
    scrollInfo.nPage  = param1;

    SetScrollInfo(
        Extension_getFrameHandle( rdPtr ),
        SB_HORZ,
        &scrollInfo,
        true );

    return 0;
}

short WINAPI DLLExport Action_setVertScrollBarPage(
    LPRDATA rdPtr,
    long param1,
    long param2 )
{
    SCROLLINFO scrollInfo;

    scrollInfo.cbSize = sizeof(SCROLLINFO);
    scrollInfo.fMask  = SIF_PAGE;
    scrollInfo.nPage  = param1;

    SetScrollInfo(
        Extension_getFrameHandle( rdPtr ),
        SB_VERT,
        &scrollInfo,
        true );

    return 0;
}

short WINAPI DLLExport Action_clearLastError(
    LPRDATA rdPtr,
    long param1,
    long param2 )
{
    rdPtr->lastError = ERROR_SUCCESS;

    return 0;
}

// ============================================================================
//
// CONDITION ROUTINES
// 
// ============================================================================

long WINAPI DLLExport Condition_onError(
    LPRDATA rdPtr,
    long param1,
    long param2 )
{
	// Always TRUE if first condition
    if ( rdPtr->rHo.hoFlags & HOF_TRUEEVENT )
		return TRUE;

    return rdPtr->lastError != ERROR_SUCCESS;
}

long WINAPI DLLExport Condition_onHSBScroll(
    LPRDATA rdPtr,
    long param1,
    long param2 )
{
	// Always TRUE if first condition
    if ( rdPtr->rHo.hoFlags & HOF_TRUEEVENT ) 
		return TRUE;

    return rdPtr->conditionsFlag & EXTENSION_HSBSCROLLFLAG;
}

long WINAPI DLLExport Condition_onVSBScroll(
    LPRDATA rdPtr,
    long param1,
    long param2 )
{
	// Always TRUE if first condition
    if ( rdPtr->rHo.hoFlags & HOF_TRUEEVENT ) 
		return TRUE;

    return rdPtr->conditionsFlag & EXTENSION_VSBSCROLLFLAG;
}

long WINAPI DLLExport Condition_needHSB(
    LPRDATA rdPtr,
    long param1,
    long param2 )
{
    SCROLLINFO scrollInfo;

    Extension_memset( &scrollInfo, sizeof(scrollInfo) );
    scrollInfo.cbSize = sizeof(SCROLLINFO);
    scrollInfo.fMask  = SIF_RANGE | SIF_PAGE;

    if ( !GetScrollInfo(
        Extension_getFrameHandle( rdPtr ),
        SB_HORZ,
        &scrollInfo ) )
    {
        Extension_setError( rdPtr, GetLastError() );
    }

    return scrollInfo.nPage
        < (UINT) ( scrollInfo.nMax - scrollInfo.nMin ) + 1;
}

long WINAPI DLLExport Condition_needVSB(
    LPRDATA rdPtr,
    long param1,
    long param2 )
{
    SCROLLINFO scrollInfo;

    Extension_memset( &scrollInfo, sizeof(scrollInfo) );
    scrollInfo.cbSize = sizeof(SCROLLINFO);
    scrollInfo.fMask  = SIF_RANGE | SIF_PAGE;

    if ( !GetScrollInfo(
        Extension_getFrameHandle( rdPtr ),
        SB_VERT,
        &scrollInfo ) )
    {
        Extension_setError( rdPtr, GetLastError() );
    }

    return scrollInfo.nPage
        < (UINT) ( scrollInfo.nMax - scrollInfo.nMin ) + 1;
}

// ============================================================================
//
// EXPRESSIONS ROUTINES
// 
// ============================================================================

long WINAPI DLLExport Expression_getHorzScrollBarPos(
    LPRDATA rdPtr,
    long param1 )
{
    SCROLLINFO scrollInfo;
    
    Extension_memset( &scrollInfo, sizeof(scrollInfo) );
    scrollInfo.cbSize = sizeof(SCROLLINFO);
    scrollInfo.fMask  = SIF_POS;

    if ( !GetScrollInfo(
        Extension_getFrameHandle( rdPtr ),
        SB_HORZ,
        &scrollInfo ) )
    {
        Extension_setError( rdPtr, GetLastError() );
    }

    return scrollInfo.nPos;
}

long WINAPI DLLExport Expression_getVertScrollBarPos(
    LPRDATA rdPtr,
    long param1 )
{
    SCROLLINFO scrollInfo;
    
    Extension_memset( &scrollInfo, sizeof(scrollInfo) );
    scrollInfo.cbSize = sizeof(SCROLLINFO);
    scrollInfo.fMask  = SIF_POS;

    if ( !GetScrollInfo(
        Extension_getFrameHandle( rdPtr ),
        SB_VERT,
        &scrollInfo ) )
    {
        Extension_setError( rdPtr, GetLastError() );
    }

    return scrollInfo.nPos;
}

long WINAPI DLLExport Expression_getHorzScrollBarPage(
    LPRDATA rdPtr,
    long param1 )
{
    SCROLLINFO scrollInfo;
    
    Extension_memset( &scrollInfo, sizeof(scrollInfo) );
    scrollInfo.cbSize = sizeof(SCROLLINFO);
    scrollInfo.fMask  = SIF_PAGE;

    if ( !GetScrollInfo(
        Extension_getFrameHandle( rdPtr ),
        SB_HORZ,
        &scrollInfo ) )
    {
        Extension_setError( rdPtr, GetLastError() );
    }

    return scrollInfo.nPage;
}

long WINAPI DLLExport Expression_getVertScrollBarPage(
    LPRDATA rdPtr,
    long param1 )
{
    SCROLLINFO scrollInfo;
    
    Extension_memset( &scrollInfo, sizeof(scrollInfo) );
    scrollInfo.cbSize = sizeof(SCROLLINFO);
    scrollInfo.fMask  = SIF_PAGE;

    if ( !GetScrollInfo(
        Extension_getFrameHandle( rdPtr ),
        SB_VERT,
        &scrollInfo ) )
    {
        Extension_setError( rdPtr, GetLastError() );
    }

    return scrollInfo.nPage;
}

long WINAPI DLLExport Expression_getHorzScrollBarMin(
    LPRDATA rdPtr,
    long param1 )
{
    SCROLLINFO scrollInfo;
    
    Extension_memset( &scrollInfo, sizeof(scrollInfo) );
    scrollInfo.cbSize = sizeof(SCROLLINFO);
    scrollInfo.fMask  = SIF_RANGE;

    if ( !GetScrollInfo(
        Extension_getFrameHandle( rdPtr ),
        SB_HORZ,
        &scrollInfo ) )
    {
        Extension_setError( rdPtr, GetLastError() );
    }

    return scrollInfo.nMin;
}

long WINAPI DLLExport Expression_getVertScrollBarMin(
    LPRDATA rdPtr,
    long param1 )
{
    SCROLLINFO scrollInfo;
    
    Extension_memset( &scrollInfo, sizeof(scrollInfo) );
    scrollInfo.cbSize = sizeof(SCROLLINFO);
    scrollInfo.fMask  = SIF_RANGE;

    if ( !GetScrollInfo(
        Extension_getFrameHandle( rdPtr ),
        SB_VERT,
        &scrollInfo ) )
    {
        Extension_setError( rdPtr, GetLastError() );
    }

    return scrollInfo.nMin;
}

long WINAPI DLLExport Expression_getHorzScrollBarMax(
    LPRDATA rdPtr,
    long param1 )
{
    SCROLLINFO scrollInfo;
    
    Extension_memset( &scrollInfo, sizeof(scrollInfo) );
    scrollInfo.cbSize = sizeof(SCROLLINFO);
    scrollInfo.fMask  = SIF_RANGE;

    if ( !GetScrollInfo(
        Extension_getFrameHandle( rdPtr ),
        SB_HORZ,
        &scrollInfo ) )
    {
        Extension_setError( rdPtr, GetLastError() );
    }

    return scrollInfo.nMax;
}

long WINAPI DLLExport Expression_getVertScrollBarMax(
    LPRDATA rdPtr,
    long param1 )
{
    SCROLLINFO scrollInfo;
    
    Extension_memset( &scrollInfo, sizeof(scrollInfo) );
    scrollInfo.cbSize = sizeof(SCROLLINFO);
    scrollInfo.fMask  = SIF_RANGE;

    if ( !GetScrollInfo(
        Extension_getFrameHandle( rdPtr ),
        SB_VERT,
        &scrollInfo ) )
    {
        Extension_setError( rdPtr, GetLastError() );
    }

    return scrollInfo.nMax;
}

long WINAPI DLLExport Expression_getLastError(
    LPRDATA rdPtr,
    long    param1 )
{
    return rdPtr->lastError;
}

long WINAPI DLLExport Expression_getHSBHeight(
    LPRDATA rdPtr,
    long    param1 )
{
    SCROLLBARINFO sbi;

    Extension_memset( &sbi, sizeof(sbi) );
    sbi.cbSize = sizeof(sbi);

    if ( !GetScrollBarInfo(
        Extension_getFrameHandle( rdPtr ),
        OBJID_HSCROLL,
        &sbi ) )
    {
        Extension_setError( rdPtr, GetLastError() );
    }

    if ( sbi.rgstate[0]
        & ( STATE_SYSTEM_INVISIBLE | STATE_SYSTEM_OFFSCREEN ) )
    {
        return 0;
    }

    return sbi.rcScrollBar.bottom - sbi.rcScrollBar.top;
}

long WINAPI DLLExport Expression_getVSBWidth(
    LPRDATA rdPtr,
    long    param1 )
{
    SCROLLBARINFO sbi;

    Extension_memset( &sbi, sizeof(sbi) );
    sbi.cbSize = sizeof(sbi);

    if ( !GetScrollBarInfo(
        Extension_getFrameHandle( rdPtr ),
        OBJID_VSCROLL,
        &sbi ) )
    {
        Extension_setError( rdPtr, GetLastError() );
    }

    if ( sbi.rgstate[0]
        & ( STATE_SYSTEM_INVISIBLE | STATE_SYSTEM_OFFSCREEN ) )
    {
        return 0;
    }

    return sbi.rcScrollBar.right - sbi.rcScrollBar.left;
}

/*-------------------------------------------------------------------------*/

// Quick memo: content of the eventInformations arrays
// ---------------------------------------------------
// Menu ID
// String ID
// Code
// Flags
// Number_of_parameters
// Parameter_type [Number_of_parameters]
// Parameter_TitleString [Number_of_parameters]

// Definitions of parameters for each action
short actionsInfos[]=
	{
        ACT_IDMN_SHOWHSB,           ACT_IDS_SHOWHSB,        ACT_SHOWHSB,        0,  1,  PARAM_EXPRESSION,   ACT_IDP_SHOWHSB_0,
        ACT_IDMN_SHOWVSB,           ACT_IDS_SHOWVSB,        ACT_SHOWVSB,        0,  1,  PARAM_EXPRESSION,   ACT_IDP_SHOWVSB_0,
        ACT_IDMN_SETHSBPOS,         ACT_IDS_SETHSBPOS,      ACT_SETHSBPOS,      0,  1,  PARAM_EXPRESSION,   ACT_IDP_SETHSBPOS_0,
        ACT_IDMN_SETVSBPOS,         ACT_IDS_SETVSBPOS,      ACT_SETVSBPOS,      0,  1,  PARAM_EXPRESSION,   ACT_IDP_SETVSBPOS_0,
        ACT_IDMN_SETHSBRANGE,       ACT_IDS_SETHSBRANGE,    ACT_SETHSBRANGE,    0,  2,  PARAM_EXPRESSION,   PARAM_EXPRESSION,   ACT_IDP_SETHSBRANGE_0,  ACT_IDP_SETHSBRANGE_1,
        ACT_IDMN_SETVSBRANGE,       ACT_IDS_SETVSBRANGE,    ACT_SETVSBRANGE,    0,  2,  PARAM_EXPRESSION,   PARAM_EXPRESSION,   ACT_IDP_SETVSBRANGE_0,  ACT_IDP_SETVSBRANGE_1,
        ACT_IDMN_SETHSBPAGE,        ACT_IDS_SETHSBPAGE,     ACT_SETHSBPAGE,     0,  1,  PARAM_EXPRESSION,   ACT_IDP_SETHSBPAGE_0,
        ACT_IDMN_SETVSBPAGE,        ACT_IDS_SETVSBPAGE,     ACT_SETVSBPAGE,     0,  1,  PARAM_EXPRESSION,   ACT_IDP_SETVSBPAGE_0,
        ACT_IDMN_CLEARLASTERROR,    ACT_IDS_CLEARLASTERROR, ACT_CLEARLASTERROR, 0,  0,

        0 /* End of array */
	};

// Definitions of parameters for each condition
short conditionsInfos[]=
	{
        CND_IDMN_ONERROR,           CND_IDS_ONERROR,        CND_ONERROR,        0,                              0,
        CND_IDMN_ONHSBSCROLL,       CND_IDS_ONHSBSCROLL,    CND_ONHSBSCROLL,    0,                              0,
        CND_IDMN_ONVSBSCROLL,       CND_IDS_ONVSBSCROLL,    CND_ONVSBSCROLL,    0,                              0,
        CND_IDMN_NEEDHSB,           CND_IDS_NEEDHSB,        CND_NEEDHSB,        EVFLAGS_ALWAYS|EVFLAGS_NOTABLE, 0,
        CND_IDMN_NEEDVSB,           CND_IDS_NEEDVSB,        CND_NEEDVSB,        EVFLAGS_ALWAYS|EVFLAGS_NOTABLE, 0,

        0 /* End of array */
	};

// Definitions of parameters for each expression
short expressionsInfos[]=
	{
        EXP_IDMN_GETHSBPOS,         EXP_IDS_GETHSBPOS,      EXP_GETHSBPOS,      0,  0,
        EXP_IDMN_GETVSBPOS,         EXP_IDS_GETVSBPOS,      EXP_GETVSBPOS,      0,  0,
        EXP_IDMN_GETHSBPAGE,        EXP_IDS_GETHSBPAGE,     EXP_GETHSBPAGE,     0,  0,
        EXP_IDMN_GETVSBPAGE,        EXP_IDS_GETVSBPAGE,     EXP_GETVSBPAGE,     0,  0,
        EXP_IDMN_GETHSBMIN,         EXP_IDS_GETHSBMIN,      EXP_GETHSBMIN,      0,  0,
        EXP_IDMN_GETVSBMIN,         EXP_IDS_GETVSBMIN,      EXP_GETVSBMIN,      0,  0,
        EXP_IDMN_GETHSBMAX,         EXP_IDS_GETHSBMAX,      EXP_GETHSBMAX,      0,  0,
        EXP_IDMN_GETVSBMAX,         EXP_IDS_GETVSBMAX,      EXP_GETVSBMAX,      0,  0,
        EXP_IDMN_GETLASTERROR,      EXP_IDS_GETLASTERROR,   EXP_GETLASTERROR,   0,  0,
        EXP_IDMN_GETHSBHEIGHT,      EXP_IDS_GETHSBHEIGHT,   EXP_GETHSBHEIGHT,   0,  0,
        EXP_IDMN_GETVSBWIDTH,       EXP_IDS_GETVSBWIDTH,    EXP_GETVSBWIDTH,    0,  0,

        0 /* End of array */
    };

// ----------------------------------------------------------
// Condition / Action / Expression jump table
// ----------------------------------------------------------
// Contains the address inside the extension of the different
// routines that handle the action, conditions and expressions.
// Located at the end of the source for convinience
// Must finish with a 0
//

short (WINAPI * ActionJumps[])(LPRDATA rdPtr, long param1, long param2) =
	{
        Action_showHorzScrollBar,
        Action_showVertScrollBar,
        Action_setHorzScrollBarPos,
        Action_setVertScrollBarPos,
        Action_setHorzScrollBarRange,
        Action_setVertScrollBarRange,
        Action_setHorzScrollBarPage,
        Action_setVertScrollBarPage,
        Action_clearLastError,

		0 /* End of array */
	};

long (WINAPI * ConditionJumps[])(LPRDATA rdPtr, long param1, long param2) = 
	{
        Condition_onError,
        Condition_onHSBScroll,
        Condition_onVSBScroll,
        Condition_needHSB,
        Condition_needVSB,

        0 /* End of array */
	};
	


long (WINAPI * ExpressionJumps[])(LPRDATA rdPtr, long param) = 
	{
        Expression_getHorzScrollBarPos,
        Expression_getVertScrollBarPos,
        Expression_getHorzScrollBarPage,
        Expression_getVertScrollBarPage,
        Expression_getHorzScrollBarMin,
        Expression_getVertScrollBarMin,
        Expression_getHorzScrollBarMax,
        Expression_getVertScrollBarMax,
        Expression_getLastError,
        Expression_getHSBHeight,
        Expression_getVSBWidth,

        0 /* End of array */
	};

/* END OF FILE */