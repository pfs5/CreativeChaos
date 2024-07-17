#include "pch.h"
#include "windows/window_debug.h"

#include "imgui.h"
#include "managers/debugmanager.h"
#include "managers/TaskManager.h"

void Window_Debug::OnDraw()
{
	if (ImGui::Button("Save tasks"))
	{
		TaskManagerProxy::Get().SaveTasks();
	}

	if(ImGui::Button("Load tasks"))
	{
		TaskManagerProxy::Get().LoadTasks();
	}

	for (const std::string& s : DebugManagerProxy::Get().GetDebugEntries())
	{
		ImGui::Text("%s", s.c_str());
	}
}

