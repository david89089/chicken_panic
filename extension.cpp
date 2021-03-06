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
#include <vector.h>

/**
 * @file extension.cpp
 * @brief Implement extension code here.
 */

Sample g_Sample;		/**< Global singleton for extension's main interface */

SMEXT_LINK(&g_Sample);

IGameConfig *g_pGameConf = NULL;

DETOUR_DECL_MEMBER0(CDetour_Chicken_FireGameEvent, void)
{
	//printf("lol\n");
	//DETOUR_MEMBER_CALL(CDetour_Chicken_Brain)(*event);
}

void test(void* Rustam)
{
    static void* func = NULL;
    if (!func)
    {
        if (!g_pGameConf->GetMemSig("CChicken::Run", &func))
        {
            assert(0);
            return;
        }
    }
	
	typedef void(*_func)(void*);
    _func thisfunc = (_func)func;
    thisfunc(Rustam);
}

DETOUR_DECL_MEMBER0(CDetour_Chicken_Walk, void)
{
	//printf("lol\n");
	test(this);
}

DETOUR_DECL_MEMBER2(CDetour_Chicken_ResolveCollisions, void, const Vector&, desiredPosition, float, deltaT)
{
	Vector testPosition = desiredPosition;
	
	void* chicken = (void*)this;
	Vector vec = *(Vector*)((char*)chicken + 484);
	
	testPosition = testPosition - vec;
	Vector velocityFromAnimation = testPosition/deltaT;

	deltaT = deltaT * 2.2;
	testPosition = vec + velocityFromAnimation * deltaT;
	
	DETOUR_MEMBER_CALL(CDetour_Chicken_ResolveCollisions)(testPosition, deltaT);
}

DETOUR_DECL_MEMBER1(CDetour_Chicken_Follow, void, void*, pPlayer)
{
	//printf("lol\n");
}

bool Sample::SDK_OnLoad(char *error, size_t maxlength, bool late)
{
	//g_pSM->LogError(myself, "Dave is join game.");
	
	char conf_error[255];
    if (!gameconfs->LoadGameConfigFile("chicken_panic.games", &g_pGameConf, conf_error, sizeof(conf_error)))
    {
        g_pSM->LogError(myself, "Could not read chicken_panic.games: %s", conf_error);
        return false;
    }
	
	CDetourManager::Init(g_pSM->GetScriptingEngine(), g_pGameConf);
	
	CDetour* detour = DETOUR_CREATE_MEMBER(CDetour_Chicken_FireGameEvent, "CChicken::FireGameEvent");
    if (!detour)
    {
        return false;
    }
	detour->EnableDetour();
	
	detour = DETOUR_CREATE_MEMBER(CDetour_Chicken_Walk, "CChicken::Walk");
    if (!detour)
    {
        return false;
    }
	detour->EnableDetour();
	
	detour = DETOUR_CREATE_MEMBER(CDetour_Chicken_ResolveCollisions, "CChicken::ResolveCollisions");
    if (!detour)
    {
        return false;
    }
	detour->EnableDetour();
	
	detour = DETOUR_CREATE_MEMBER(CDetour_Chicken_Follow, "CChicken::SetChickenStartFollowingPlayer");
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
	if(g_pGameConf != NULL)
	{
		gameconfs->CloseGameConfigFile(g_pGameConf);
		g_pGameConf = NULL;
	}
}

