#include "pch.h"
#include "windows/modalwindow_changeprio.h"

#include "imgui.h"
#include "managers/TaskManager.h"
#include "model/task.h"

void ModalWindow_ChangePrio::OnDraw()
{
	Super::OnDraw();


	const Task& task = TaskManagerProxy::Get().GetTask(_targetTask);
	ImGui::Text("P%d: %s", task.Priority, task.Name.c_str());

	ImGui::SetNextItemWidth(580.f);
	ImGui::InputText("###ChangePrioInputText", _inputBuffer, INPUT_BUFFER_SIZE, ImGuiInputTextFlags_CharsDecimal | ImGuiInputTextFlags_EscapeClearsAll);
	ImGui::SetKeyboardFocusHere(-1);

	ImGui::Text("ENTER - Accept | ESC - Cancel");
}

void ModalWindow_ChangePrio::OnOpen()
{
	Super::OnOpen();

	ClearInputBuffer();
}

void ModalWindow_ChangePrio::SetupInputs(InputHandler& inputHandler)
{
	Super::SetupInputs(inputHandler);

	inputHandler.RegisterInputCallbackTemplated<ModalWindow_ChangePrio>(this, EInputAction::Confirm, &ModalWindow_ChangePrio::OnInput_Confirm);
	inputHandler.RegisterInputCallbackTemplated<ModalWindow_ChangePrio>(this, EInputAction::Cancel, &ModalWindow_ChangePrio::OnInput_Cancel);
}

ImVec2 ModalWindow_ChangePrio::GetModalSize() const
{
	return ImVec2{ 600.f, 125.f };
}

void ModalWindow_ChangePrio::OnInput_Confirm(const InputEvent& e)
{
	try
	{
		int32_t prio = std::stoi(_inputBuffer);
		if (prio < TaskPriority::MIN_PRIO || prio > TaskPriority::MAX_PRIO)
		{
			return;
		}

		TaskManagerProxy::Get().SetTaskPriority(_targetTask, TaskPriority{ prio });
		CloseModal();
	}
	catch (...)
	{

	}
}

void ModalWindow_ChangePrio::OnInput_Cancel(const InputEvent& e)
{
	CloseModal();
}
