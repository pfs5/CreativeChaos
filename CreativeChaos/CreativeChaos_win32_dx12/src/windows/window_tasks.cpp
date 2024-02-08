#include "pch.h"
#include "windows/window_tasks.h"

#include "imgui.h"
#include "model/task.h"
#include "managers/TaskManager.h"
#include "util/imguihelpers.h"
#include "util/tostring.h"
#include "managers/inputmanager.h"
#include "managers/statemanager.h"

void Window_Tasks::OnDraw()
{
	// todo - shortcuts bar
	if (ImGui::TreeNodeEx("Shortcuts", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::Text("F1 - Activate | F2 - Edit | Delete - Move to trash | Space - Change category");
		ImGui::NewLine();
		ImGui::TreePop();
	}

	InitTasks();
	ProcessInputs();
	DrawTasks();
	DrawNewTaskModal();
}

void Window_Tasks::InitTasks()
{
	_taskLists.clear();
	_taskLists.reserve(3);

	TaskList& tasklistMain = _taskLists.emplace_back();
	tasklistMain.Title = "Main";
	tasklistMain.Tasks.reserve(64);
	TaskManagerProxy::Get().FindTasksWithCategory(tasklistMain.Tasks, ETaskCategory::Main);

	TaskList& tasklistBacklog = _taskLists.emplace_back();
	tasklistBacklog.Title = "Backlog";
	tasklistBacklog.Tasks.reserve(64);
	TaskManagerProxy::Get().FindTasksWithCategory(tasklistBacklog.Tasks, ETaskCategory::Backlog);

	TaskList& tasklistTrash = _taskLists.emplace_back();
	tasklistTrash.Title = "Trash";
	tasklistTrash.Tasks.reserve(64);
	TaskManagerProxy::Get().FindTasksWithCategory(tasklistTrash.Tasks, ETaskCategory::Trash);
}

void Window_Tasks::ProcessInputs()
{
	const uint32_t totalTasksSize = (uint32_t)GetTotalNumTasks();
	const InputManager& inputManager = InputManagerProxy::Get();

	// Get inputs
	if (inputManager.IsInputPressed(EInputAction::NextTask))
	{
		_currentSelectedTaskIdx = (_currentSelectedTaskIdx + 1) % totalTasksSize;
	}
	else if (inputManager.IsInputPressed(EInputAction::PreviousTask))
	{
		if (_currentSelectedTaskIdx == 0 || _currentSelectedTaskIdx == -1)
		{
			_currentSelectedTaskIdx = totalTasksSize - 1;
		}
		else
		{
			--_currentSelectedTaskIdx;
			if (_currentSelectedTaskIdx < 0)
			{
				_currentSelectedTaskIdx += totalTasksSize;
			}
		}
	}
	else if (inputManager.IsInputPressed(EInputAction::EditTask))
	{
		const TaskManager::TaskPtr currentSelectedTask = StateManagerProxy::Get().GetCurrentSelectedTask();
		if (currentSelectedTask.IsValid())
		{
			StateManagerProxy::Get().SetMode(EApplicationMode::EditTask);

			const char* taskName = currentSelectedTask.GetTask().Name.c_str();
			std::strcpy(_inputBuffer, taskName);
		}
	}
	else if (inputManager.IsInputPressed(EInputAction::ToggleTaskActive))
	{
		const TaskManager::TaskPtr currentSelectedTask = StateManagerProxy::Get().GetCurrentSelectedTask();
		if (currentSelectedTask.IsValid())
		{
			const bool isTaskActive = currentSelectedTask.GetTask().Active;
			if (isTaskActive)
			{
				TaskManagerProxy::Get().StopTask(currentSelectedTask);
			}
			else
			{
				TaskManagerProxy::Get().StartTask(currentSelectedTask);
			}
		}
	}
	else if (inputManager.IsInputPressed(EInputAction::NewTask))
	{
		StateManagerProxy::Get().SetMode(EApplicationMode::NewTask);
	}
	else if (inputManager.IsInputPressed(EInputAction::DeleteTask))
	{
		const TaskManager::TaskPtr currentSelectedTask = StateManagerProxy::Get().GetCurrentSelectedTask();
		if (currentSelectedTask.IsValid())
		{
			currentSelectedTask.GetTask().Category = ETaskCategory::Trash;
		}
	}
	else if (inputManager.IsInputPressed(EInputAction::ChangeTaskCategory))
	{
		const TaskManager::TaskPtr currentSelectedTask = StateManagerProxy::Get().GetCurrentSelectedTask();
		if (currentSelectedTask.IsValid())
		{
			// Shift category
			Task& task = currentSelectedTask.GetTask();
			if (task.Category == ETaskCategory::Trash)
			{
				task.Category = ETaskCategory::Backlog;
			}
			else if (task.Category == ETaskCategory::Backlog)
			{
				task.Category = ETaskCategory::Main;
			}
			else if (task.Category == ETaskCategory::Main)
			{
				task.Category = ETaskCategory::Backlog;
			}
		}
	}
	else if (inputManager.IsInputPressed(EInputAction::ConfirmEditTask))
	{
		const TaskManager::TaskPtr currentSelectedTask = StateManagerProxy::Get().GetCurrentSelectedTask();
		if (currentSelectedTask.IsValid())
		{
			TaskManagerProxy::Get().GetTask(currentSelectedTask).Name = _inputBuffer;
			StateManagerProxy::Get().SetMode(EApplicationMode::Browse);
		}
	}
	else if (inputManager.IsInputPressed(EInputAction::CancelEditTask))
	{
		StateManagerProxy::Get().SetMode(EApplicationMode::Browse);
	}
}

void Window_Tasks::DrawTasks()
{
	uint32_t taskIdx = 0;
	for (TaskList& taskList : _taskLists)
	{
		if (ImGui::CollapsingHeader(taskList.Title.c_str(), ImGuiTreeNodeFlags_DefaultOpen))
		{
			ImGuiScopedIndent();

			if (taskList.Tasks.empty())
			{
				continue;
			}

			const std::string tableId = StringFormat("###Table%s", taskList.Title.c_str());
			if (!ImGui::BeginTable(tableId.c_str(), 2, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg))
			{
				continue;
			}

			// Setup columns
			ImGui::TableSetupColumn("TaskStatus", ImGuiTableColumnFlags_WidthFixed, 60.f);
			ImGui::TableSetupColumn("TaskText", ImGuiTableColumnFlags_WidthStretch);

			for (TaskManager::TaskPtr taskPtr : taskList.Tasks)
			{
				Task& task = taskPtr.GetTask();

				ImGui::TableNextRow();

				// Col0 - Task status
				if (task.Active)
				{
					DrawActiveButton();
				}

				// Col1 - Task text
				ImGui::TableSetColumnIndex(1);

				if (_currentSelectedTaskIdx == taskIdx)
				{
					StateManagerProxy::Get().SetCurrentSelectedTask(taskPtr);

					if (StateManagerProxy::Get().GetMode() == EApplicationMode::EditTask)
					{
						const ImVec2 avail = ImGui::GetContentRegionAvail();

						ImGui::PushItemWidth(avail.x);	// todo
						ImGui::InputText("###TaskRename", _inputBuffer, INPUT_BUFFER_SIZE);
						ImGui::PopItemWidth();
						ImGui::SetKeyboardFocusHere(-1);
					}
					else
					{
						ImGui::TableSetBgColor(ImGuiTableBgTarget_RowBg1, ImGui::GetColorU32(ImVec4{ 0.8f, 0.3f, 0.f, 1.f }));
						ImGui::TextColored(ImVec4{ 1.f, 1.f, 1.f, 1.f }, "[%04d] %s", task.ID, task.Name.c_str());
					}
				}
				else
				{
					ImGui::Text("[%04d] %s", task.ID, task.Name.c_str());
				}

				++taskIdx;
			}

			ImGui::EndTable();
		}
	}
}

void Window_Tasks::DrawNewTaskModal()
{
	if (StateManagerProxy::Get().GetMode() != EApplicationMode::NewTask)
	{
		return;
	}

	const char* const popupName = "New Task";
	if (!ImGui::IsPopupOpen(popupName))
	{
		ClearInputBuffer();
		ImGui::OpenPopup(popupName);
	}

	const ImVec2 center = ImGui::GetMainViewport()->GetCenter();
	ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
	ImGui::SetNextWindowSize(ImVec2{ 500.f, 200.f });

	if (ImGui::BeginPopupModal(popupName, nullptr, ImGuiWindowFlags_AlwaysAutoResize))
	{
		ImGui::Text("ENTER - Accept | ESC - Cancel");

		ImGui::SetNextItemWidth(400.f);
		ImGui::InputText("###NewTaskInputText", _inputBuffer, INPUT_BUFFER_SIZE);
		ImGui::SetKeyboardFocusHere(-1);

		if (InputManagerProxy::Get().IsInputPressed(EInputAction::ConfirmNewTask))
		{
			TaskManagerProxy::Get().CreateNewTask(_inputBuffer, ETaskCategory::Main);
			StateManagerProxy::Get().SetMode(EApplicationMode::Browse);
			ImGui::CloseCurrentPopup();
		}
		else if (InputManagerProxy::Get().IsInputPressed(EInputAction::CancelNewTask))
		{
			StateManagerProxy::Get().SetMode(EApplicationMode::Browse);
			ImGui::CloseCurrentPopup();
		}

		ImGui::EndPopup();
	}
}

void Window_Tasks::DrawActiveButton()
{
	ImGui::TableSetColumnIndex(0);
	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 5.f, 2.f });
	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.f, 0.5f, 0.f, 1.f });
	ImGui::Button("active");
	ImGui::PopStyleVar();
	ImGui::PopStyleColor();
}

size_t Window_Tasks::GetTotalNumTasks() const
{
	size_t size = 0;
	for (const TaskList& t : _taskLists)
	{
		size += t.Tasks.size();
	}
	return size;
}
