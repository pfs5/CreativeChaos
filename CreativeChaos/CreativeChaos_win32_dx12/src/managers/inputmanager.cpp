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

void InputManager::ResetInputs()
{
	_inputPressed.reset();
}

void InputManager::UpdateInputs_Browse()
{
	SetInputValue(EInputAction::NextTask, ImGui::IsKeyPressed(ImGuiKey_DownArrow));
	SetInputValue(EInputAction::PreviousTask, ImGui::IsKeyPressed(ImGuiKey_UpArrow));

	SetInputValue(EInputAction::EditTask, ImGui::IsKeyPressed(ImGuiKey_F2));
	SetInputValue(EInputAction::NewTask, ImGui::IsKeyDown(ImGuiKey_LeftCtrl) && ImGui::IsKeyPressed(ImGuiKey_N));
	SetInputValue(EInputAction::DeleteTask, ImGui::IsKeyPressed(ImGuiKey_Delete));
	SetInputValue(EInputAction::ChangeTaskCategory, ImGui::IsKeyPressed(ImGuiKey_Space));
	SetInputValue(EInputAction::ToggleTaskActive, ImGui::IsKeyPressed(ImGuiKey_Enter));
}

void InputManager::UpdateInputs_EditTask()
{
	SetInputValue(EInputAction::ConfirmEditTask,  ImGui::IsKeyPressed(ImGuiKey_Enter));
	SetInputValue(EInputAction::CancelEditTask,  ImGui::IsKeyPressed(ImGuiKey_Escape));
}

void InputManager::UpdateInputs_NewTask()
{
	SetInputValue(EInputAction::ConfirmNewTask, ImGui::IsKeyPressed(ImGuiKey_Enter));
	SetInputValue(EInputAction::CancelNewTask, ImGui::IsKeyPressed(ImGuiKey_Escape));
}
