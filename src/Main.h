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

#ifndef __MMF2EXT_WINSCROLLBAR_MAIN_H__
#define __MMF2EXT_WINSCROLLBAR_MAIN_H__

// ------------------------------
// OBJECT IDENTIFIER
// ------------------------------
// REQUIRED: If you do not do this, MMF2 could confuse
//           your object with another in the event editor.
#define IDENTIFIER	            0x01AA0007

// Object versions
#define	KCX_CURRENT_VERSION     1

// ---------------------------
// DEFINITION OF ACTIONS CODES
// ---------------------------
#define ACT_SHOWHSB             0
#define ACT_SHOWVSB             1
#define ACT_SETHSBPOS           2
#define ACT_SETVSBPOS           3
#define ACT_SETHSBRANGE         4
#define ACT_SETVSBRANGE         5
#define ACT_SETHSBPAGE          6
#define ACT_SETVSBPAGE          7
#define ACT_CLEARLASTERROR      8

#define ACT_LAST                9

// ------------------------------
// DEFINITION OF CONDITIONS CODES
// ------------------------------
#define CND_ONERROR             0
#define CND_ONHSBSCROLL         1
#define CND_ONVSBSCROLL         2
#define CND_NEEDHSB             3
#define CND_NEEDVSB             4

#define CND_LAST                5

// -------------------------------
// DEFINITION OF EXPRESSIONS CODES
// -------------------------------
#define EXP_GETHSBPOS           0
#define EXP_GETVSBPOS           1
#define EXP_GETHSBPAGE          2
#define EXP_GETVSBPAGE          3
#define EXP_GETHSBMIN           4
#define EXP_GETVSBMIN           5
#define EXP_GETHSBMAX           6
#define EXP_GETVSBMAX           7
#define EXP_GETLASTERROR        8
#define EXP_GETHSBHEIGHT        9
#define EXP_GETVSBWIDTH         10

#define EXP_LAST                11

// ---------------------
// OBJECT DATA STRUCTURE 
// ---------------------
// Used at edit time and saved in the MFA/CCN/EXE files

typedef struct tagEDATA_V1
{
	// Header - required
	extHeader		eHeader;

	// Object's data
//	short			swidth;
//	short			sheight;

} EDITDATA;
typedef EDITDATA * LPEDATA;

// --------------------------------
// RUNNING OBJECT DATA STRUCTURE
// --------------------------------
// Used at runtime. Initialize it in the CreateRunObject function.
// Free any allocated memory or object in the DestroyRunObject function.
//
// Note: if you store C++ objects in this structure and do not store 
// them as pointers, you must call yourself their constructor in the
// CreateRunObject function and their destructor in the DestroyRunObject
// function. As the RUNDATA structure is a simple C structure and not a C++ object.

#define EXTENSION_ERRORFLAG     (0x1)
#define EXTENSION_HSBSCROLLFLAG (0x2)
#define EXTENSION_VSBSCROLLFLAG (0x4)

typedef struct tagRDATA
{
	// Main header - required
	headerObject	rHo;					// Header

	// Optional headers - depend on the OEFLAGS value, see documentation and examples for more info
//	rCom			rc;				// Common structure for movements & animations
//	rMvt			rm;				// Movements
//	rSpr			rs;				// Sprite (displayable objects)
//	rVal			rv;				// Alterable values

	// Object's runtime data
    DWORD   lastError;
#ifndef RUN_ONLY
    int     errorsCount;
#endif
    char    eventsFlag;
    char    conditionsFlag;
} RUNDATA;
typedef	RUNDATA	* LPRDATA;

/**
 * Retrieve current frame handle.
 */
inline HWND Extension_getFrameHandle( LPRDATA rdPtr )
{
    return rdPtr->rHo.hoAdRunHeader->rhHEditWin;
}

/**
 * Set current error code and mark this object for future event
 * handling.
 */
inline void Extension_setError( LPRDATA rdPtr, DWORD error )
{
#ifndef RUN_ONLY
    rdPtr->errorsCount++;
#endif
    rdPtr->lastError   = error;
    rdPtr->eventsFlag |= EXTENSION_ERRORFLAG;
}

/**
 * Mark this object on horizontal scrollbar event received.
 */
inline void Extension_setHSBEvent( LPRDATA rdPtr )
{
    rdPtr->eventsFlag |= EXTENSION_HSBSCROLLFLAG;
}

/**
 * Mark this object on vertical scrollbar event received.
 */
inline void Extension_setVSBEvent( LPRDATA rdPtr )
{
    rdPtr->eventsFlag |= EXTENSION_VSBSCROLLFLAG;
}

// Size when editing the object under level editor
// -----------------------------------------------
#define	MAX_EDITSIZE			sizeof(EDITDATA)

// Default flags - see documentation for more info
// -------------
#define	OEFLAGS      			OEFLAG_DISPLAYINFRONT | OEFLAG_WINDOWPROC
#define	OEPREFS      			0


// If to handle message, specify the priority of the handling procedure
// 0= low, 255= very high. You should use 100 as normal.                                                
// --------------------------------------------------------------------
#define	WINDOWPROC_PRIORITY		100

#endif /* __MMF2EXT_WINSCROLLBAR_MAIN_H__ */
