#include "pch.h"
#include "windows/window_tasks.h"

#include "imgui.h"
#include "model/task.h"
#include "managers/TaskManager.h"
#include "util/imguihelpers.h"
#include "util/tostring.h"
#include "managers/inputmanager.h"
#include "managers/statemanager.h"
#include "core/staticconfig.h"

#include "taskviews/taskview_categories.h"
#include "taskviews/taskview_activity.h"

Window_Tasks::Window_Tasks()
{
	_taskViews.emplace_back(new TaskView_Categories());
	_taskViews.emplace_back(new TaskView_Activity());
}

Window_Tasks::~Window_Tasks() = default;

void Window_Tasks::OnRegister()
{
	// Setup inputs
	InputManager& inputManager = InputManagerProxy::Get();
	
	RegisterInputCallbackTemplated<Window_Tasks>(this, EInputAction::NextTask, &Window_Tasks::OnInput_NextTask);
	RegisterInputCallbackTemplated<Window_Tasks>(this, EInputAction::PreviousTask, &Window_Tasks::OnInput_PreviousTask);
	RegisterInputCallbackTemplated<Window_Tasks>(this, EInputAction::NextTaskView, &Window_Tasks::OnInput_NextTaskView);
	RegisterInputCallbackTemplated<Window_Tasks>(this, EInputAction::ToggleCategory_1, &Window_Tasks::OnInput_ToggleCategory<0>);
	RegisterInputCallbackTemplated<Window_Tasks>(this, EInputAction::ToggleCategory_2, &Window_Tasks::OnInput_ToggleCategory<1>);
	RegisterInputCallbackTemplated<Window_Tasks>(this, EInputAction::ToggleCategory_3, &Window_Tasks::OnInput_ToggleCategory<2>);
	RegisterInputCallbackTemplated<Window_Tasks>(this, EInputAction::ToggleCategory_4, &Window_Tasks::OnInput_ToggleCategory<3>);
	RegisterInputCallbackTemplated<Window_Tasks>(this, EInputAction::ToggleCategory_5, &Window_Tasks::OnInput_ToggleCategory<4>);
	RegisterInputCallbackTemplated<Window_Tasks>(this, EInputAction::ToggleCategory_6, &Window_Tasks::OnInput_ToggleCategory<5>);
	RegisterInputCallbackTemplated<Window_Tasks>(this, EInputAction::ToggleCategory_7, &Window_Tasks::OnInput_ToggleCategory<6>);
	RegisterInputCallbackTemplated<Window_Tasks>(this, EInputAction::ToggleCategory_8, &Window_Tasks::OnInput_ToggleCategory<7>);
	RegisterInputCallbackTemplated<Window_Tasks>(this, EInputAction::ToggleCategory_9, &Window_Tasks::OnInput_ToggleCategory<8>);
	RegisterInputCallbackTemplated<Window_Tasks>(this, EInputAction::ScrollDown, &Window_Tasks::OnInput_ScrollDown);
	RegisterInputCallbackTemplated<Window_Tasks>(this, EInputAction::ScrollUp, &Window_Tasks::OnInput_ScrollUp);
	RegisterInputCallbackTemplated<Window_Tasks>(this, EInputAction::EditTask, &Window_Tasks::OnInput_EditTask);
	RegisterInputCallbackTemplated<Window_Tasks>(this, EInputAction::ToggleTaskActive, &Window_Tasks::OnInput_ToggleTaskActive);
	RegisterInputCallbackTemplated<Window_Tasks>(this, EInputAction::NewTask, &Window_Tasks::OnInput_NewTask);
	RegisterInputCallbackTemplated<Window_Tasks>(this, EInputAction::DeleteTask, &Window_Tasks::OnInput_DeleteTask);
	RegisterInputCallbackTemplated<Window_Tasks>(this, EInputAction::ChangeTaskCategory, &Window_Tasks::OnInput_ChangeTaskCategory);
	RegisterInputCallbackTemplated<Window_Tasks>(this, EInputAction::ConfirmEditTask, &Window_Tasks::OnInput_ConfirmEditTask);
	RegisterInputCallbackTemplated<Window_Tasks>(this, EInputAction::CancelEditTask, &Window_Tasks::OnInput_CancelEditTask);
}

void Window_Tasks::OnDraw()
{
	// todo - shortcuts bar
	if (ImGui::TreeNodeEx("Shortcuts", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::Text("F1 - Activate | F2 - Edit | Delete - Move to trash | Space - Change category");
		ImGui::NewLine();
		ImGui::TreePop();
	}

	UpdateTasks();
	DrawTasks();
	DrawNewTaskModal();

	_selectionChanged = false;
}

void Window_Tasks::UpdateTasks()
{
	for (std::unique_ptr<TaskView>& taskView : _taskViews)
	{
		taskView->UpdateTasks();
	}
}

void Window_Tasks::DrawTasks()
{
	TaskView* taskView = GetCurrentTaskView();
	if (taskView == nullptr)
	{
		return;
	}

	std::vector<TaskCollection>& collections = taskView->GetCollections();
	uint32_t taskIdx = 0;
	for (uint32_t collectionIdx = 0; collectionIdx < collections.size(); ++collectionIdx)
	{
		TaskCollection& taskCollection = collections[collectionIdx];

		std::string headerTitle = std::format("[{}] {}", collectionIdx + 1, taskCollection.Title.c_str());
		ImGui::SetNextItemOpen(taskCollection.IsOpen);
		taskCollection.IsOpen = ImGui::CollapsingHeader(headerTitle.c_str(), ImGuiTreeNodeFlags_DefaultOpen);
		if (taskCollection.IsOpen)
		{
			ImGuiScopedIndent();

			if (taskCollection.Tasks.empty())
			{
				continue;
			}

			const std::string tableId = StringFormat("###Table%s", taskCollection.Title.c_str());
			if (!ImGui::BeginTable(tableId.c_str(), 2, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg))
			{
				continue;
			}

			// Setup columns
			ImGui::TableSetupColumn("TaskStatus", ImGuiTableColumnFlags_WidthFixed, 60.f);
			ImGui::TableSetupColumn("TaskText", ImGuiTableColumnFlags_WidthStretch);

			for (TaskManager::TaskPtr taskPtr : taskCollection.Tasks)
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
						ImGui::TextColored(ImVec4{ 1.f, 1.f, 1.f, 1.f }, "%04d %s", task.ID, task.Name.c_str());

						if (_selectionChanged)
						{
							const float itemMaxY = ImGui::GetItemRectMax().y;
							const float winMaxY = ImGui::GetWindowPos().y + ImGui::GetWindowHeight();
							const float itemMinY = ImGui::GetItemRectMin().y - ImGui::GetWindowPos().y;

							static const float PADDING_BOTTOM = 60.f;
							static const float PADDING_TOP = 150.f;

							if (itemMaxY + PADDING_BOTTOM > winMaxY)
							{
								ImGui::SetScrollY(ImGui::GetScrollY() + itemMaxY + PADDING_BOTTOM - winMaxY);
							}
							else if (itemMinY < PADDING_TOP)
							{
								ImGui::SetScrollY(ImGui::GetScrollY() - PADDING_TOP + itemMinY);
							}
						}
					}
				}
				else
				{
					ImGui::Text("%04d %s", task.ID, task.Name.c_str());
				}

				++taskIdx;
			}

			ImGui::EndTable();
			ImGui::NewLine();
		}
	}

	ImGui::NewLine();
	ImGui::NewLine();
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
	const TaskView* const currentView = GetCurrentTaskView();
	ensure(currentView != nullptr);

	return currentView->GetNumVisibleTasks();
}

void Window_Tasks::OnInput_NextTask(const InputEvent& e)
{
	const uint32_t totalTasksSize = (uint32_t)GetTotalNumTasks();
	_currentSelectedTaskIdx = (_currentSelectedTaskIdx + 1) % totalTasksSize;

	_selectionChanged = true;
}

void Window_Tasks::OnInput_PreviousTask(const InputEvent& e)
{
	const uint32_t totalTasksSize = (uint32_t)GetTotalNumTasks();
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

	_selectionChanged = true;
}

void Window_Tasks::OnInput_NextTaskView(const InputEvent& e)
{
	_currentTaskViewIdx = (_currentTaskViewIdx + 1) % _taskViews.size();
}

void Window_Tasks::OnInput_ToggleCategory(const InputEvent& e, uint32_t categoryIdx)
{
	TaskView* currentView = GetCurrentTaskView();
	ensure(currentView != nullptr);

	std::vector<TaskCollection>& collections = currentView->GetCollections();
	if (categoryIdx < collections.size())
	{
		collections[categoryIdx].IsOpen = !collections[categoryIdx].IsOpen;
	}
}

void Window_Tasks::OnInput_ScrollDown(const InputEvent& e)
{
	float y = ImGui::GetScrollY();
	ImGui::SetScrollY(y + StaticConfig::SCROLL_AMOUNT);
}

void Window_Tasks::OnInput_ScrollUp(const InputEvent& e)
{
	float y = ImGui::GetScrollY();
	ImGui::SetScrollY(y - StaticConfig::SCROLL_AMOUNT);
}

void Window_Tasks::OnInput_EditTask(const InputEvent& e)
{
	const TaskManager::TaskPtr currentSelectedTask = StateManagerProxy::Get().GetCurrentSelectedTask();
	if (currentSelectedTask.IsValid())
	{
		StateManagerProxy::Get().SetMode(EApplicationMode::EditTask);

		const char* taskName = currentSelectedTask.GetTask().Name.c_str();
		std::strcpy(_inputBuffer, taskName);
	}
}

void Window_Tasks::OnInput_ToggleTaskActive(const InputEvent& e)
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

void Window_Tasks::OnInput_NewTask(const InputEvent& e)
{
	StateManagerProxy::Get().SetMode(EApplicationMode::NewTask);
}

void Window_Tasks::OnInput_DeleteTask(const InputEvent& e)
{
	const TaskManager::TaskPtr currentSelectedTask = StateManagerProxy::Get().GetCurrentSelectedTask();
	if (currentSelectedTask.IsValid())
	{
		TaskManagerProxy::Get().SetTaskCategory(currentSelectedTask, ETaskCategory::Trash);
	}
}

void Window_Tasks::OnInput_ChangeTaskCategory(const InputEvent& e)
{
	const TaskManager::TaskPtr currentSelectedTask = StateManagerProxy::Get().GetCurrentSelectedTask();
	if (currentSelectedTask.IsValid())
	{
		// Shift category
		Task& task = currentSelectedTask.GetTask();
		if (task.Category == ETaskCategory::Trash)
		{
			TaskManagerProxy::Get().SetTaskCategory(currentSelectedTask, ETaskCategory::Backlog);
		}
		else if (task.Category == ETaskCategory::Backlog)
		{
			TaskManagerProxy::Get().SetTaskCategory(currentSelectedTask, ETaskCategory::Main);
		}
		else if (task.Category == ETaskCategory::Main)
		{
			TaskManagerProxy::Get().SetTaskCategory(currentSelectedTask, ETaskCategory::Backlog);
		}
	}
}

void Window_Tasks::OnInput_ConfirmEditTask(const InputEvent& e)
{
	const TaskManager::TaskPtr currentSelectedTask = StateManagerProxy::Get().GetCurrentSelectedTask();
	if (currentSelectedTask.IsValid())
	{
		TaskManagerProxy::Get().GetTask(currentSelectedTask).Name = _inputBuffer;
		StateManagerProxy::Get().SetMode(EApplicationMode::Browse);
	}
}

void Window_Tasks::OnInput_CancelEditTask(const InputEvent& e)
{
	StateManagerProxy::Get().SetMode(EApplicationMode::Browse);
}