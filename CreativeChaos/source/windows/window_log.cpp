#include "pch.h"
#include "window_log.h"

#include "imgui.h"
#include "managers/debugmanager.h"

void Window_Log::OnDraw()
{
	if (ImGui::BeginChild("###LogScrolling", ImVec2(0, 0), ImGuiChildFlags_None, ImGuiWindowFlags_HorizontalScrollbar | ImGuiWindowFlags_AlwaysVerticalScrollbar))
	{
		const std::vector<DebugManager::LogEntry>& logEntries = DebugManagerProxy::Get().GetLogEntries();
		for (const DebugManager::LogEntry& log : logEntries)
		{
			ImGui::Text("[%s] %s", log.Time.ToString().c_str(), log.Text.c_str());
		}
	}

	if (DebugManagerProxy::Get().GetLogDirty())
	{
		ImGui::SetScrollHereY();
		//ImGui::SetScrollY(ImGui::GetScrollMaxY());
	}

	ImGui::EndChild();

	DebugManagerProxy::Get().ClearLogDirty();
}

