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

// Used by Ext.rc

#ifndef __MMF2EXT_WINSCROLLBAR_RESOURCE_H__
#define __MMF2EXT_WINSCROLLBAR_RESOURCE_H__

#define _APS_NO_MFC                 1

// About box information
#define	IDST_OBJNAME                128
#define	IDST_AUTHOR                 129
#define	IDST_COPYRIGHT              130
#define	IDST_COMMENT                131
#define	IDST_HTTP                   132

// Dialog Box ID
#define DB_SETUP                    20003

// Help ID
#define ID_HELP                     20004

// 	Actions
//-------------------

// Identifier of actions menu
#define MN_ACTIONS                  20000

// Commands of action menu          (25000-25990)
#define ACT_IDMN_SHOWHSB            25000
#define ACT_IDMN_SHOWVSB            25010
#define ACT_IDMN_SETHSBPOS          25020
#define ACT_IDMN_SETVSBPOS          25030
#define ACT_IDMN_SETHSBRANGE        25040
#define ACT_IDMN_SETVSBRANGE        25050
#define ACT_IDMN_SETHSBPAGE         25060
#define ACT_IDMN_SETVSBPAGE         25070
#define ACT_IDMN_CLEARLASTERROR     25080

// Action strings                   (5000-5990)
#define ACT_IDS_SHOWHSB             5000
#define ACT_IDS_SHOWVSB             5010
#define ACT_IDS_SETHSBPOS           5020
#define ACT_IDS_SETVSBPOS           5030
#define ACT_IDS_SETHSBRANGE         5040
#define ACT_IDS_SETVSBRANGE         5050
#define ACT_IDS_SETHSBPAGE          5060
#define ACT_IDS_SETVSBPAGE          5070
#define ACT_IDS_CLEARLASTERROR      5080

// Titles of action parameters      (5xx1-5xx9)
#define ACT_IDP_SHOWHSB_0           5001
#define ACT_IDP_SHOWVSB_0           5011
#define ACT_IDP_SETHSBPOS_0         5021
#define ACT_IDP_SETVSBPOS_0         5031
#define ACT_IDP_SETHSBRANGE_0       5041
#define ACT_IDP_SETHSBRANGE_1       5042
#define ACT_IDP_SETVSBRANGE_0       5051
#define ACT_IDP_SETVSBRANGE_1       5052
#define ACT_IDP_SETHSBPAGE_0        5061
#define ACT_IDP_SETVSBPAGE_0        5071

//	Conditions
//--------------------

// Identifier of conditions menu
#define MN_CONDITIONS               20001

// Commands of condition menu       (26000-26990)
#define CND_IDMN_ONERROR            26000
#define CND_IDMN_ONHSBSCROLL        26010
#define CND_IDMN_ONVSBSCROLL        26020
#define CND_IDMN_NEEDHSB            26030
#define CND_IDMN_NEEDVSB            26040

// Condition strings                (6000-6990)
#define CND_IDS_ONERROR             6000
#define CND_IDS_ONHSBSCROLL         6010
#define CND_IDS_ONVSBSCROLL         6020
#define CND_IDS_NEEDHSB             6030
#define CND_IDS_NEEDVSB             6040

// Titles of cnds parameters        (6xx1-6xx9)

//	EXPRESSIONS
//---------------------

// Identifier of expressions menu
#define MN_EXPRESSIONS              20002

// Commands of expressions menu     (27000-27990)
#define EXP_IDMN_GETHSBPOS          27000
#define EXP_IDMN_GETVSBPOS          27010
#define EXP_IDMN_GETHSBPAGE         27020
#define EXP_IDMN_GETVSBPAGE         27030
#define EXP_IDMN_GETHSBMIN          27040
#define EXP_IDMN_GETVSBMIN          27050
#define EXP_IDMN_GETHSBMAX          27060
#define EXP_IDMN_GETVSBMAX          27070
#define EXP_IDMN_GETLASTERROR       27080
#define EXP_IDMN_GETHSBHEIGHT       27090
#define EXP_IDMN_GETVSBWIDTH        27100

// Expression strings               (7000-7990)
#define EXP_IDS_GETHSBPOS           7000
#define EXP_IDS_GETVSBPOS           7010
#define EXP_IDS_GETHSBPAGE          7020
#define EXP_IDS_GETVSBPAGE          7030
#define EXP_IDS_GETHSBMIN           7040
#define EXP_IDS_GETVSBMIN           7050
#define EXP_IDS_GETHSBMAX           7060
#define EXP_IDS_GETVSBMAX           7070
#define EXP_IDS_GETLASTERROR        7080
#define EXP_IDS_GETHSBHEIGHT        7090
#define EXP_IDS_GETVSBWIDTH         7100

// Names of expression parameters   (7xx1-7xx9)

// 	Debug                           (8000-8999)
//-------------------
#if !defined(RUN_ONLY)
#define DB_IDS_ERRORSCOUNT          8000
#define DB_IDS_LASTERROR            8001
#endif

// Next default values for new objects
// 
#ifdef APSTUDIO_INVOKED
#ifndef APSTUDIO_READONLY_SYMBOLS

#define _APS_NEXT_RESOURCE_VALUE        101
#define _APS_NEXT_COMMAND_VALUE         40001
#define _APS_NEXT_CONTROL_VALUE         1001
#define _APS_NEXT_SYMED_VALUE           101
#endif
#endif

#endif /* __MMF2EXT_WINSCROLLBAR_RESOURCE_H__ */
