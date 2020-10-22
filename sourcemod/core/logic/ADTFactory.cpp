/**
 * vim: set ts=4 sw=4 tw=99 noet :
 * =============================================================================
 * SourceMod
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

#include "ADTFactory.h"
#include "ShareSys.h"

ADTFactory g_AdtFactory;

const char *ADTFactory::GetInterfaceName()
{
	return SMINTERFACE_ADTFACTORY_NAME;
}

unsigned int ADTFactory::GetInterfaceVersion()
{
	return SMINTERFACE_ADTFACTORY_VERSION;
}

void ADTFactory::OnSourceModAllInitialized()
{
	g_ShareSys.AddInterface(NULL, this);
}

IBasicTrie *ADTFactory::CreateBasicTrie()
{
	return new BaseTrie();
}

BaseTrie::BaseTrie()
{
}

BaseTrie::~BaseTrie()
{
}

bool BaseTrie::Insert(const char *key, void *value)
{
	return map_.insert(key, value);
}

bool BaseTrie::Retrieve(const char *key, void **value)
{
	return map_.retrieve(key, value);
}

bool BaseTrie::Delete(const char *key)
{
	return map_.remove(key);
}

void BaseTrie::Clear()
{
	map_.clear();
}

void BaseTrie::Destroy()
{
	delete this;
}

bool BaseTrie::Replace(const char *key, void *value)
{
	return map_.replace(key, value);
}
