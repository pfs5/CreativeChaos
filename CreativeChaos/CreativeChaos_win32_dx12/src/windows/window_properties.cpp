#include "pch.h"
#include "windows/window_properties.h"

#include "imgui.h"
#include "managers/statemanager.h"
#include "managers/TaskManager.h"

void Window_Properties::OnDraw()
{
	const TaskManager::TaskPtr currentSelectedTask = StateManagerProxy::Get().GetCurrentSelectedTask();
	if (!currentSelectedTask.IsValid())
	{
		return;
	}

	if (ImGui::BeginTable("###PropertiesTable", 2, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg))
	{
		const Task& task = currentSelectedTask.GetTask();

		// Setup cols
		ImGui::TableSetupColumn("Property", ImGuiTableColumnFlags_WidthFixed, 60.f);
		ImGui::TableSetupColumn("Value", ImGuiTableColumnFlags_WidthStretch);
		
		// ID
		ImGui::TableNextRow();

		ImGui::TableSetColumnIndex(0);
		ImGui::Text("ID:");

		ImGui::TableSetColumnIndex(1);
		ImGui::Text("%04d", task.GetID());

		// Content
		ImGui::TableNextRow();

		ImGui::TableSetColumnIndex(0);
		ImGui::Text("Content:");

		ImGui::TableSetColumnIndex(1);
		ImGui::Text("%s", task.GetName());

		// Status
		ImGui::TableNextRow();

		ImGui::TableSetColumnIndex(0);
		ImGui::Text("Status:");

		ImGui::TableSetColumnIndex(1);
		ImGui::Text("%s", task.IsActive() ? "Active" : "-");

		// Time elapsed
		ImGui::TableNextRow();

		ImGui::TableSetColumnIndex(0);
		ImGui::Text("Progress:");

		ImGui::TableSetColumnIndex(1);
		ImGui::Text("");

		// Created timestamp
		ImGui::TableNextRow();

		ImGui::TableSetColumnIndex(0);
		ImGui::Text("Created:");

		ImGui::TableSetColumnIndex(1);
		ImGui::Text("");

		ImGui::EndTable();
	}
}
