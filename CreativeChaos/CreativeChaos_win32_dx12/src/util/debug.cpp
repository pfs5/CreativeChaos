#include "pch.h"
#include "util/debug.h"

#include "managers/debugmanager.h"

void DebugLog(const char* text)
{
	DebugManagerProxy::Get().PushLog(text);
}
