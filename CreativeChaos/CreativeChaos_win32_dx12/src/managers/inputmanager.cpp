#include "pch.h"
#include "managers/inputmanager.h"

#include "managers/statemanager.h"
#include "imgui.h"

InputManagerProxy::InputManagerProxy() = default;

InputManager::InputManager()
{
	ResetInputs();
}

void InputManager::Update()
{
	ResetInputs();

	UpdateInputs_App();

	switch (StateManagerProxy::Get().GetMode())
	{
		case EApplicationMode::Browse:
		{
			UpdateInputs_Browse();
			break;
		}

		case EApplicationMode::EditTask:
		{
			UpdateInputs_EditTask();
			break;
		}

		case EApplicationMode::NewTask:
		{
			UpdateInputs_NewTask();
			break;
		}
	}
}

void InputManager::RegisterInputCallback(EInputAction action, const InputCallback& callback)
{
	GetInputCallback(action) = callback;
}

void InputManager::UnregisterInputCallback(EInputAction action, const InputCallback& callback)
{
	GetInputCallback(action) = nullptr;
}

void InputManager::ResetInputs()
{
	_inputPressed.reset();
}

void InputManager::UpdateInputs_App()
{
	SetInputValue(EInputAction::ExitApp, ImGui::IsKeyPressed(ImGuiKey_Escape));
}

void InputManager::UpdateInputs_Browse()
{
	SetInputValue(EInputAction::NextTask,			!ImGui::IsKeyDown(ImGuiKey_LeftCtrl) && ImGui::IsKeyPressed(ImGuiKey_DownArrow));
	SetInputValue(EInputAction::PreviousTask,		!ImGui::IsKeyDown(ImGuiKey_LeftCtrl) && ImGui::IsKeyPressed(ImGuiKey_UpArrow));

	SetInputValue(EInputAction::NextTaskView,		ImGui::IsKeyPressed(ImGuiKey_Tab));

	SetInputValue(EInputAction::ToggleCategory_1,	ImGui::IsKeyPressed(ImGuiKey_1));
	SetInputValue(EInputAction::ToggleCategory_2,	ImGui::IsKeyPressed(ImGuiKey_2));
	SetInputValue(EInputAction::ToggleCategory_3,	ImGui::IsKeyPressed(ImGuiKey_3));
	SetInputValue(EInputAction::ToggleCategory_4,	ImGui::IsKeyPressed(ImGuiKey_4));
	SetInputValue(EInputAction::ToggleCategory_5,	ImGui::IsKeyPressed(ImGuiKey_5));
	SetInputValue(EInputAction::ToggleCategory_6,	ImGui::IsKeyPressed(ImGuiKey_6));
	SetInputValue(EInputAction::ToggleCategory_7,	ImGui::IsKeyPressed(ImGuiKey_7));
	SetInputValue(EInputAction::ToggleCategory_8,	ImGui::IsKeyPressed(ImGuiKey_8));
	SetInputValue(EInputAction::ToggleCategory_9,	ImGui::IsKeyPressed(ImGuiKey_9));

	SetInputValue(EInputAction::ScrollDown,			ImGui::IsKeyDown(ImGuiKey_LeftCtrl) && ImGui::IsKeyPressed(ImGuiKey_DownArrow));
	SetInputValue(EInputAction::ScrollUp,			ImGui::IsKeyDown(ImGuiKey_LeftCtrl) && ImGui::IsKeyPressed(ImGuiKey_UpArrow));

	SetInputValue(EInputAction::EditTask,			ImGui::IsKeyPressed(ImGuiKey_F2));
	SetInputValue(EInputAction::NewTask,			ImGui::IsKeyDown(ImGuiKey_LeftCtrl) && ImGui::IsKeyPressed(ImGuiKey_N));
	SetInputValue(EInputAction::DeleteTask,			ImGui::IsKeyPressed(ImGuiKey_Delete));
	SetInputValue(EInputAction::ChangeTaskCategory, ImGui::IsKeyPressed(ImGuiKey_Space));
	SetInputValue(EInputAction::ToggleTaskActive,	ImGui::IsKeyPressed(ImGuiKey_Enter));
}

void InputManager::UpdateInputs_EditTask()
{
	SetInputValue(EInputAction::ConfirmEditTask,	ImGui::IsKeyPressed(ImGuiKey_Enter));
	SetInputValue(EInputAction::CancelEditTask,		ImGui::IsKeyPressed(ImGuiKey_Escape));
}

void InputManager::UpdateInputs_NewTask()
{
	SetInputValue(EInputAction::ConfirmNewTask,		ImGui::IsKeyPressed(ImGuiKey_Enter));
	SetInputValue(EInputAction::CancelNewTask,		ImGui::IsKeyPressed(ImGuiKey_Escape));
}

void InputManager::SetInputValue(EInputAction input, bool value)
{
	if (value)
	{
		_inputPressed.set((std::underlying_type_t<EInputAction>)input);
		if (GetInputCallback(input))
		{
			InputEvent e;
			GetInputCallback(input)(e);
		}
	}
	else
	{
		_inputPressed.reset((std::underlying_type_t<EInputAction>)input);
	}
}
