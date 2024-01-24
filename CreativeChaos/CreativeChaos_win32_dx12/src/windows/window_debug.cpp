#include "pch.h"
#include "windows/window_debug.h"

#include "imgui.h"
#include "managers/debugmanager.h"

void Window_Debug::OnDraw()
{
	for (const std::string& s : DebugManagerProxy::Get().GetDebugEntries())
	{
		ImGui::Text("%s", s.c_str());
	}
}

