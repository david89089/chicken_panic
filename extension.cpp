/**
 * vim: set ts=4 :
 * =============================================================================
 * SourceMod Sample Extension
 * Copyright (C) 2004-2008 AlliedModders LLC.  All rights reserved.
 * =============================================================================
 *
 * This program is free software; you can redistribute it and/or modify it under
 * the terms of the GNU General Public License, version 3.0, as published by the
 * Free Software Foundation.
 * 
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * As a special exception, AlliedModders LLC gives you permission to link the
 * code of this program (as well as its derivative works) to "Half-Life 2," the
 * "Source Engine," the "SourcePawn JIT," and any Game MODs that run on software
 * by the Valve Corporation.  You must obey the GNU General Public License in
 * all respects for all other code used.  Additionally, AlliedModders LLC grants
 * this exception to all derivative works.  AlliedModders LLC defines further
 * exceptions, found in LICENSE.txt (as of this writing, version JULY-31-2007),
 * or <http://www.sourcemod.net/license.php>.
 *
 * Version: $Id$
 */

#include "extension.h"
#include "CDetour/detours.h"
#include "IGameConfig.h"
/**
 * @file extension.cpp
 * @brief Implement extension code here.
 */

Sample g_Sample;		/**< Global singleton for extension's main interface */

SMEXT_LINK(&g_Sample);

IGameConfig *g_pGameConf = NULL;

DETOUR_DECL_MEMBER0(CDetour_Chicken_Brain, void, void*, fleeFrom, float, duration)
{
	//DETOUR_MEMBER_CALL(CDetour_Chicken_Brain)(fleeFrom, duration);
}

bool Sample::SDK_OnLoad(char *error, size_t maxlength, bool late)
{
	//g_pSM->LogError(myself, "Dave is join game.");
	
	char conf_error[255];
    if (!gameconfs->LoadGameConfigFile("chicken_panic.games", &g_pGameConf, conf_error, sizeof(conf_error)))
    {
        g_pSM->LogError(error, "Could not read chicken_panic.games: %s", conf_error);
        return false;
    }
	
	CDetourManager::Init(g_pSM->GetScriptingEngine(), g_pGameConf);
	
	CDetour* detour = DETOUR_CREATE_MEMBER(CDetour_Chicken_Brain, "CChicken::Flee");
    if (!detour)
    {
        return false;
    }
    detour->EnableDetour();
	//GET_DETOUR(Chicken_Brain, DETOUR_CREATE_MEMBER(CDetour_Chicken_Brain, "AllocateDefaultRelationships"));
	
	return true;
}

void Sample::SDK_OnUnload()
{
	g_pSM->LogError(myself, "Bye.");
}

/*
void Sample::OnMapStart()
{
	g_pSM->LogError(myself, "Dave is join game OnMapStart.");
}
*/