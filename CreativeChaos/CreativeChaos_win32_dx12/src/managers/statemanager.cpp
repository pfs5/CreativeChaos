#include "pch.h"
#include "managers/statemanager.h"

#include "util/tostring.h"
#include "managers/debugmanager.h"

StateManagerProxy::StateManagerProxy() = default;

void StateManager::Update()
{
	std::string modeStr = StringFormat("Mode = %s", ToString(_mode));
	DebugManagerProxy::Get().PushDebug(modeStr.c_str());
}
