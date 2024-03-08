#include "pch.h"
#include "windows/window_properties.h"

#include "imgui.h"
#include "managers/statemanager.h"
#include "managers/TaskManager.h"

void Window_Properties::OnDraw()
{
	const TaskPtr currentSelectedTask = StateManagerProxy::Get().GetCurrentSelectedTask();
	if (!currentSelectedTask.IsValid())
	{
		return;
	}

	const Task& task = currentSelectedTask.GetTask();

	if (ImGui::BeginTable("###PropertiesTable", 2, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg))
	{
		// Setup cols
		ImGui::TableSetupColumn("Property", ImGuiTableColumnFlags_WidthFixed, 60.f);
		ImGui::TableSetupColumn("Value", ImGuiTableColumnFlags_WidthStretch);
		
		// ID
		ImGui::TableNextRow();

		ImGui::TableSetColumnIndex(0);
		ImGui::Text("ID:");

		ImGui::TableSetColumnIndex(1);
		ImGui::Text("%04d", task.ID);

		// Content
		ImGui::TableNextRow();

		ImGui::TableSetColumnIndex(0);
		ImGui::Text("Content:");

		ImGui::TableSetColumnIndex(1);
		ImGui::Text("%s", task.Name.c_str());

		// Status
		ImGui::TableNextRow();

		ImGui::TableSetColumnIndex(0);
		ImGui::Text("Status:");

		ImGui::TableSetColumnIndex(1);
		ImGui::Text("%s", task.Active ? "Active" : "-");

		// Time elapsed
		ImGui::TableNextRow();

		ImGui::TableSetColumnIndex(0);
		ImGui::Text("Progress:");

		ImGui::TableSetColumnIndex(1);
		ImGui::Text("%s", task.CalculateProgress().ToString().c_str());

		// Created timestamp
		ImGui::TableNextRow();

		ImGui::TableSetColumnIndex(0);
		ImGui::Text("Created:");

		ImGui::TableSetColumnIndex(1);
		ImGui::Text("%s", task.CreatedAt.ToString().c_str());

		ImGui::EndTable();
	}

	ImGui::NewLine();

	if (ImGui::TreeNodeEx("Events:", ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_NoTreePushOnOpen))
	{
		if (!task.Events.empty())
		{
			if (ImGui::BeginTable("###EventsTable", 2, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg))
			{
				// Setup cols
				ImGui::TableSetupColumn("Timestamp", ImGuiTableColumnFlags_WidthFixed, 140.f);
				ImGui::TableSetupColumn("Action", ImGuiTableColumnFlags_WidthStretch);

				for (const TaskEvent& taskEvent : task.Events)
				{
					ImGui::TableNextRow();

					ImGui::TableSetColumnIndex(0);
					ImGui::Text("%s", taskEvent.Time.ToString().c_str());

					ImGui::TableSetColumnIndex(1);
					ImGui::Text("%s", ToString(taskEvent.Type));

				}

				ImGui::EndTable();
			}
		}
	}
}
