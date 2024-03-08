#include "pch.h"
#include "windows/modalwindow_newtask.h"

#include "imgui.h"
#include "managers/TaskManager.h"

void ModalWindow_NewTask::OnDraw()
{
	Super::OnDraw();

	ImGui::Text("ENTER - Accept | ESC - Cancel");

	ImGui::SetNextItemWidth(580.f);
	ImGui::InputText("###NewTaskInputText", _inputBuffer, INPUT_BUFFER_SIZE);
	ImGui::SetKeyboardFocusHere(-1);
}

void ModalWindow_NewTask::OnOpen()
{
	Super::OnOpen();

	ClearInputBuffer();
}

void ModalWindow_NewTask::SetupInputs(InputHandler& inputHandler)
{
	Super::SetupInputs(inputHandler);

	inputHandler.RegisterInputCallbackTemplated<ModalWindow_NewTask>(this, EInputAction::Confirm, &ModalWindow_NewTask::OnInput_Confirm);
	inputHandler.RegisterInputCallbackTemplated<ModalWindow_NewTask>(this, EInputAction::Cancel, &ModalWindow_NewTask::OnInput_Cancel);
}

ImVec2 ModalWindow_NewTask::GetModalSize() const
{
	return ImVec2{ 600.f, 100.f };
}

void ModalWindow_NewTask::OnInput_Confirm(const InputEvent& e)
{
	TaskManagerProxy::Get().CreateNewTask(_inputBuffer, ETaskCategory::Main);
	CloseModal();
}

void ModalWindow_NewTask::OnInput_Cancel(const InputEvent& e)
{
	CloseModal();
}
