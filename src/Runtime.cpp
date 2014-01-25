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
// This file contains routines that are handled during the Runtime.
//
// Including creating, display, and handling your object.
// 
// ============================================================================

#ifndef RUN_ONLY
#include <stdio.h>
#endif

#include <Windows.h>

#include "common.h" // Common Include

static void Extension_enableWinScroll( LPRDATA rdPtr, BOOL enabled )
{
    HWND hWnd = Extension_getFrameHandle( rdPtr );
    int winLong;
    
    /* Retrieve previous GWL_STYLE */
    SetLastError( ERROR_SUCCESS );
    winLong = GetWindowLong( hWnd, GWL_STYLE );

    if ( winLong == 0 )
    {
        DWORD error = GetLastError();
        if ( error != ERROR_SUCCESS )
        {
            Extension_setError( rdPtr, error );
            return;
        }
    }

    /* New GWL_STYLE */
    if ( ( winLong & ( WS_HSCROLL | WS_VSCROLL ) )
        != ( enabled ? ( WS_HSCROLL | WS_VSCROLL ) : 0 ) )
    {
        SetLastError( ERROR_SUCCESS );
        if ( !SetWindowLong(
            hWnd,
            GWL_STYLE,
            enabled
            ? ( winLong | WS_HSCROLL | WS_VSCROLL ) 
            : ( winLong & ~(WS_HSCROLL | WS_VSCROLL) ) ) )
        {
            DWORD error = GetLastError();
            if ( error != ERROR_SUCCESS )
            {
                Extension_setError( rdPtr, error );
                return;
            }
        }
        if ( !SetWindowPos(
            hWnd,
            0,
            0,
            0,
            0,
            0,
            SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED ) )
        {
            Extension_setError( rdPtr, GetLastError() );
            return;
        }
    }
}

// DEBUGGER /////////////////////////////////////////////////////////////////

#if !defined(RUN_ONLY)
// Identifiers of items displayed in the debugger
enum
{
    DB_ERRORSCOUNT,
    DB_LASTERROR,
};

// Items displayed in the debugger
WORD DebugTree[]=
{
    DB_ERRORSCOUNT,
    DB_LASTERROR,

	// End of table (required)
	DB_END
};

#endif // !defined(RUN_ONLY)


// --------------------
// GetRunObjectDataSize
// --------------------
// Returns the size of the runtime datazone of the object
// 
ushort WINAPI DLLExport GetRunObjectDataSize(fprh rhPtr, LPEDATA edPtr)
{
	return (sizeof(RUNDATA));
}

// ---------------
// CreateRunObject
// ---------------
// The routine where the object is actually created
//
short WINAPI DLLExport CreateRunObject(LPRDATA rdPtr, LPEDATA edPtr, fpcob cobPtr)
{
    const mv * mV   = rdPtr->rHo.hoAdRunHeader->rh4.rh4Mv;
    CRunApp  * cApp = rdPtr->rHo.hoAdRunHeader->rhApp;

    if ( mV->mvGetExtUserData( cApp, hInstLib ) != NULL )
    {
        return 1; /* Failed */
    }

    /* Init data */
#ifndef RUN_ONLY
    rdPtr->errorsCount    = 0;
#endif
    rdPtr->lastError      = ERROR_SUCCESS;
    rdPtr->eventsFlag     = 0;
    rdPtr->conditionsFlag = 0;

    /* Register for WindowProc */
    mV->mvSetExtUserData( cApp, hInstLib, rdPtr );

    Extension_enableWinScroll( rdPtr, TRUE );

	/* No errors */
	return 0;
}


// ----------------
// DestroyRunObject
// ----------------
// Destroys the run-time object
// 
short WINAPI DLLExport DestroyRunObject(LPRDATA rdPtr, long fast)
{
/*
   When your object is destroyed (either with a Destroy action or at the end of
   the frame) this routine is called. You must free any resources you have allocated!
*/
    const mv * mV   = rdPtr->rHo.hoAdRunHeader->rh4.rh4Mv;
    CRunApp  * cApp = rdPtr->rHo.hoAdRunHeader->rhApp;

    if ( mV->mvGetExtUserData( cApp, hInstLib ) == rdPtr )
    {
        mV->mvSetExtUserData( cApp, hInstLib, NULL );

        Extension_enableWinScroll( rdPtr, FALSE );
    }

	// No errors
	return 0;
}

// ----------------
// HandleRunObject
// ----------------
// Called (if you want) each loop, this routine makes the object live
// 
short WINAPI DLLExport HandleRunObject(LPRDATA rdPtr)
{
    /* Reset */
    if ( rdPtr->conditionsFlag )
        rdPtr->conditionsFlag = 0;

    /* Error event handling */
    if ( rdPtr->eventsFlag & EXTENSION_ERRORFLAG )
    {
        rdPtr->eventsFlag ^= EXTENSION_ERRORFLAG;
        callRunTimeFunction(
            rdPtr,
            RFUNCTION_PUSHEVENTSTOP,
            CND_ONERROR,
            NULL );
    }

    /* Horizontal scrollbar event handling */
    if ( rdPtr->eventsFlag & EXTENSION_HSBSCROLLFLAG )
    {
        rdPtr->eventsFlag ^= EXTENSION_HSBSCROLLFLAG;
        rdPtr->conditionsFlag |= EXTENSION_HSBSCROLLFLAG;
        callRunTimeFunction(
            rdPtr,
            RFUNCTION_PUSHEVENTSTOP,
            CND_ONHSBSCROLL,
            NULL );
    }

    /* Vertical scrollbar event handling */
    if ( rdPtr->eventsFlag & EXTENSION_VSBSCROLLFLAG )
    {
        rdPtr->eventsFlag ^= EXTENSION_VSBSCROLLFLAG;
        rdPtr->conditionsFlag |= EXTENSION_VSBSCROLLFLAG;
        callRunTimeFunction(
            rdPtr,
            RFUNCTION_PUSHEVENTSTOP,
            CND_ONVSBSCROLL,
            NULL );
    }

	return 0;
}

// ----------------
// DisplayRunObject
// ----------------
// Draw the object in the application screen.
// 
short WINAPI DLLExport DisplayRunObject(LPRDATA rdPtr)
{
/*
   If you return REFLAG_DISPLAY in HandleRunObject this routine will run.
*/
	// Ok
	return 0;
}

// -------------------
// GetRunObjectSurface
// -------------------
// Implement this function instead of DisplayRunObject if your extension
// supports ink effects and transitions. Note: you can support ink effects
// in DisplayRunObject too, but this is automatically done if you implement
// GetRunObjectSurface (MMF applies the ink effect to the surface).
//
// Note: do not forget to enable the function in the .def file 
// if you remove the comments below.
/*
cSurface* WINAPI DLLExport GetRunObjectSurface(LPRDATA rdPtr)
{
	return NULL;
}
*/

// -------------------------
// GetRunObjectCollisionMask
// -------------------------
// Implement this function if your extension supports fine collision mode (OEPREFS_FINECOLLISIONS),
// Or if it's a background object and you want Obstacle properties for this object.
//
// Should return NULL if the object is not transparent.
//
// Note: do not forget to enable the function in the .def file 
// if you remove the comments below.
//
/*
LPSMASK WINAPI DLLExport GetRunObjectCollisionMask(LPRDATA rdPtr, LPARAM lParam)
{
	// Typical example for active objects
	// ----------------------------------
	// Opaque? collide with box
	if ( (rdPtr->rs.rsEffect & EFFECTFLAG_TRANSPARENT) == 0 )	// Note: only if your object has the OEPREFS_INKEFFECTS option
		return NULL;

	// Transparent? Create mask
	LPSMASK pMask = rdPtr->m_pColMask;
	if ( pMask == NULL )
	{
		if ( rdPtr->m_pSurface != NULL )
		{
			DWORD dwMaskSize = rdPtr->m_pSurface->CreateMask(NULL, lParam);
			if ( dwMaskSize != 0 )
			{
				pMask = (LPSMASK)calloc(dwMaskSize, 1);
				if ( pMask != NULL )
				{
					rdPtr->m_pSurface->CreateMask(pMask, lParam);
					rdPtr->m_pColMask = pMask;
				}
			}
		}
	}

	// Note: for active objects, lParam is always the same.
	// For background objects (OEFLAG_BACKGROUND), lParam maybe be different if the user uses your object
	// as obstacle and as platform. In this case, you should store 2 collision masks
	// in your data: one if lParam is 0 and another one if lParam is different from 0.

	return pMask;
}
*/

// ----------------
// PauseRunObject
// ----------------
// Enters the pause mode
// 
short WINAPI DLLExport PauseRunObject(LPRDATA rdPtr)
{
	// Ok
	return 0;
}

// -----------------
// ContinueRunObject
// -----------------
// Quits the pause mode
//
short WINAPI DLLExport ContinueRunObject(LPRDATA rdPtr)
{
	// Ok
	return 0;
}

// -----------------
// SaveRunObject
// -----------------
// Saves the object to disk
// 

BOOL WINAPI SaveRunObject(LPRDATA rdPtr, HANDLE hf)
{
    // OK
	return TRUE;
}

// -----------------
// LoadRunObject
// -----------------
// Loads the object from disk
// 
BOOL WINAPI LoadRunObject(LPRDATA rdPtr, HANDLE hf)
{
    // OK
	return TRUE; 
}

// ============================================================================
//
// START APP / END APP / START FRAME / END FRAME routines
// 
// ============================================================================

// -------------------
// StartApp
// -------------------
// Called when the application starts or restarts.
// Useful for storing global data
// 
void WINAPI DLLExport StartApp( mv _far *mV, CRunApp* pApp )
{
	// Delete global data (if restarts application)
    mV->mvSetExtUserData(
        pApp,
        hInstLib,
        NULL );
}

// -------------------
// EndApp
// -------------------
// Called when the application ends.
// 
void WINAPI DLLExport EndApp( mv _far *mV, CRunApp* pApp )
{
	// Delete global data
    mV->mvSetExtUserData(
        pApp,
        hInstLib,
        NULL );
}

// -------------------
// StartFrame
// -------------------
// Called when the frame starts or restarts.
// 
void WINAPI DLLExport StartFrame(mv _far *mV, DWORD dwReserved, int nFrameIndex)
{
}

// -------------------
// EndFrame
// -------------------
// Called when the frame ends.
// 
void WINAPI DLLExport EndFrame(mv _far *mV, DWORD dwReserved, int nFrameIndex)
{
}

// ============================================================================
//
// TEXT ROUTINES (if OEFLAG_TEXT)
// 
// ============================================================================

// -------------------
// GetRunObjectFont
// -------------------
// Return the font used by the object.
// 
/*

  // Note: do not forget to enable the functions in the .def file 
  // if you remove the comments below.

void WINAPI GetRunObjectFont(LPRDATA rdPtr, LOGFONT* pLf)
{
	// Example
	// -------
	// GetObject(rdPtr->m_hFont, sizeof(LOGFONT), pLf);
}

// -------------------
// SetRunObjectFont
// -------------------
// Change the font used by the object.
// 
void WINAPI SetRunObjectFont(LPRDATA rdPtr, LOGFONT* pLf, RECT* pRc)
{
	// Example
	// -------
//	HFONT hFont = CreateFontIndirect(pLf);
//	if ( hFont != NULL )
//	{
//		if (rdPtr->m_hFont!=0)
//			DeleteObject(rdPtr->m_hFont);
//		rdPtr->m_hFont = hFont;
//		SendMessage(rdPtr->m_hWnd, WM_SETFONT, (WPARAM)rdPtr->m_hFont, FALSE);
//	}

}

// ---------------------
// GetRunObjectTextColor
// ---------------------
// Return the text color of the object.
// 
COLORREF WINAPI GetRunObjectTextColor(LPRDATA rdPtr)
{
	// Example
	// -------
	return 0;	// rdPtr->m_dwColor;
}

// ---------------------
// SetRunObjectTextColor
// ---------------------
// Change the text color of the object.
// 
void WINAPI SetRunObjectTextColor(LPRDATA rdPtr, COLORREF rgb)
{
	// Example
	// -------
	rdPtr->m_dwColor = rgb;
	InvalidateRect(rdPtr->m_hWnd, NULL, TRUE);
}
*/


// ============================================================================
//
// WINDOWPROC (interception of messages sent to hMainWin and hEditWin)
//
// Do not forget to enable the WindowProc function in the .def file if you implement it
// 
// ============================================================================

// Called from the window proc of hMainWin and hEditWin.
// You can intercept the messages and/or tell the main proc to ignore them.
//
LRESULT CALLBACK DLLExport WindowProc(
    LPRH rhPtr,
    HWND hWnd,
    UINT nMsg,
    WPARAM wParam,
    LPARAM lParam )
{
    LPRDATA rdPtr = NULL;

    if ( lParam != NULL )
    {
        return 0;
    }

    rdPtr = (LPRDATA) rhPtr->rh4.rh4Mv->mvGetExtUserData(
        rhPtr->rhApp,
        hInstLib );
    if ( rdPtr == NULL )
    {
        return 0;
    }

    switch ( nMsg )
    {
        /*------------------------- WM_HSCROLL --------------------------*/
        case WM_HSCROLL:
            switch ( LOWORD( wParam ) )
            {
                case SB_ENDSCROLL:
                case SB_THUMBPOSITION:
                    Extension_setHSBEvent( rdPtr );
                    return REFLAG_MSGHANDLED;
                case SB_THUMBTRACK:
                {
                    SCROLLINFO si;

                    si.cbSize = sizeof( si );
                    si.fMask  = SIF_ALL;
                    if ( !GetScrollInfo(
                        Extension_getFrameHandle( rdPtr ),
                        SB_HORZ,
                        &si ) )
                    {
                        Extension_setError( rdPtr, GetLastError() );
                        return REFLAG_MSGHANDLED;
                    }
                    si.nPos  = si.nTrackPos;
                    si.fMask = SIF_POS;
                    SetScrollInfo(
                        Extension_getFrameHandle( rdPtr ),
                        SB_HORZ,
                        &si,
                        TRUE );
                    Extension_setHSBEvent( rdPtr );
                    return REFLAG_MSGHANDLED;
                }
                case SB_LEFT:
                case SB_RIGHT:
                case SB_LINELEFT:
                case SB_LINERIGHT:
                case SB_PAGELEFT:
                case SB_PAGERIGHT:
                {
                    SCROLLINFO si;

                    si.cbSize = sizeof( si );
                    si.fMask  = SIF_ALL;
                    if ( !GetScrollInfo(
                        Extension_getFrameHandle( rdPtr ),
                        SB_HORZ,
                        &si ) )
                    {
                        Extension_setError( rdPtr, GetLastError() );
                        return REFLAG_MSGHANDLED;
                    }
                    switch ( LOWORD( wParam ) )
                    {
                        case SB_LEFT:
                            si.nPos = si.nMin;
                            break;
                        case SB_RIGHT:
                            si.nPos = si.nMax;
                            break;
                        case SB_LINELEFT:
                            si.nPos -= 1;
                            break;
                        case SB_LINERIGHT:
                            si.nPos += 1;
                            break;
                        case SB_PAGELEFT:
                            si.nPos -= si.nPage;
                            break;
                        case SB_PAGERIGHT:
                            si.nPos += si.nPage;
                            break;
                        default:
                            return REFLAG_MSGHANDLED;
                    }
                    si.fMask = SIF_POS;
                    SetScrollInfo(
                        Extension_getFrameHandle( rdPtr ),
                        SB_HORZ,
                        &si,
                        TRUE );
                    Extension_setHSBEvent( rdPtr );
                    return REFLAG_MSGHANDLED;
                }
            }
            break;
        /*------------------------- WM_VSCROLL --------------------------*/
        case WM_VSCROLL:
            switch ( LOWORD( wParam ) )
            {
                case SB_ENDSCROLL:
                case SB_THUMBPOSITION:
                    Extension_setVSBEvent( rdPtr );
                    return REFLAG_MSGHANDLED;
                case SB_THUMBTRACK:
                {
                    SCROLLINFO si;

                    si.cbSize = sizeof( si );
                    si.fMask  = SIF_ALL;
                    if ( !GetScrollInfo(
                        Extension_getFrameHandle( rdPtr ),
                        SB_VERT,
                        &si ) )
                    {
                        Extension_setError( rdPtr, GetLastError() );
                        return REFLAG_MSGHANDLED;
                    }
                    si.nPos  = si.nTrackPos;
                    si.fMask = SIF_POS;
                    SetScrollInfo(
                        Extension_getFrameHandle( rdPtr ),
                        SB_VERT,
                        &si,
                        TRUE );
                    Extension_setVSBEvent( rdPtr );
                    return REFLAG_MSGHANDLED;
                }
                case SB_BOTTOM:
                case SB_LINEDOWN:
                case SB_LINEUP:
                case SB_PAGEDOWN:
                case SB_PAGEUP:
                case SB_TOP:
                {
                    SCROLLINFO si;

                    si.cbSize = sizeof( si );
                    si.fMask  = SIF_ALL;
                    if ( !GetScrollInfo(
                        Extension_getFrameHandle( rdPtr ),
                        SB_VERT,
                        &si ) )
                    {
                        Extension_setError( rdPtr, GetLastError() );
                        return REFLAG_MSGHANDLED;
                    }
                    switch ( LOWORD( wParam ) )
                    {
                        case SB_TOP:
                            si.nPos = si.nMin;
                            break;
                        case SB_BOTTOM:
                            si.nPos = si.nMax;
                            break;
                        case SB_LINEUP:
                            si.nPos -= 1;
                            break;
                        case SB_LINEDOWN:
                            si.nPos += 1;
                            break;
                        case SB_PAGEUP:
                            si.nPos -= si.nPage;
                            break;
                        case SB_PAGEDOWN:
                            si.nPos += si.nPage;
                            break;
                        default:
                            return REFLAG_MSGHANDLED;
                    }
                    si.fMask = SIF_POS;
                    SetScrollInfo(
                        Extension_getFrameHandle( rdPtr ),
                        SB_VERT,
                        &si,
                        TRUE );
                    Extension_setVSBEvent( rdPtr );
                    return REFLAG_MSGHANDLED;
                }
                default:
                    break;
            }
            break;
        /*--------------------------- default ---------------------------*/
        default:
            break;
    }

	return 0;
}

// ============================================================================
//
// DEBUGGER ROUTINES
// 
// ============================================================================

// -----------------
// GetDebugTree
// -----------------
// This routine returns the address of the debugger tree
//
LPWORD WINAPI DLLExport GetDebugTree(LPRDATA rdPtr)
{
#if !defined(RUN_ONLY)
	return DebugTree;
#else
	return NULL;
#endif // !defined(RUN_ONLY)
}

// -----------------
// GetDebugItem
// -----------------
// This routine returns the text of a given item.
//
void WINAPI DLLExport GetDebugItem(LPSTR pBuffer, LPRDATA rdPtr, int id)
{
#if !defined(RUN_ONLY)
	char temp[DB_BUFFERSIZE];

	switch (id)
	{
	    case DB_ERRORSCOUNT:
		    LoadString( hInstLib, DB_IDS_ERRORSCOUNT, temp, DB_BUFFERSIZE );
            sprintf_s( pBuffer, DB_BUFFERSIZE, temp, rdPtr->errorsCount );
		    break;
        case DB_LASTERROR:
		    LoadString( hInstLib, DB_IDS_LASTERROR, temp, DB_BUFFERSIZE );
            sprintf_s( pBuffer, DB_BUFFERSIZE, temp, rdPtr->lastError );
		    break;
        default:
            sprintf_s( pBuffer, DB_BUFFERSIZE, "??? %d", id );
            break;
	}
#endif // !defined(RUN_ONLY)
}

// -----------------
// EditDebugItem
// -----------------
// This routine allows to edit editable items.
//
void WINAPI DLLExport EditDebugItem(LPRDATA rdPtr, int id)
{
#if !defined(RUN_ONLY)

	// Example
	// -------
/*
	switch (id)
	{
	case DB_CURRENTSTRING:
		{
			EditDebugInfo dbi;
			char buffer[256];

			dbi.pText=buffer;
			dbi.lText=TEXT_MAX;
			dbi.pTitle=NULL;

			strcpy(buffer, rdPtr->text);
			long ret=callRunTimeFunction(rdPtr, RFUNCTION_EDITTEXT, 0, (LPARAM)&dbi);
			if (ret)
				strcpy(rdPtr->text, dbi.pText);
		}
		break;
	case DB_CURRENTVALUE:
		{
			EditDebugInfo dbi;

			dbi.value=rdPtr->value;
			dbi.pTitle=NULL;

			long ret=callRunTimeFunction(rdPtr, RFUNCTION_EDITINT, 0, (LPARAM)&dbi);
			if (ret)
				rdPtr->value=dbi.value;
		}
		break;
	}
*/
#endif // !defined(RUN_ONLY)
}

/* END OF FILE */