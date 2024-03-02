#include "pch.h"
#include "windows/modalwindow.h"

#include "imgui.h"
#include "core/window.h"

void ModalWindow::Draw()
{
	const char* title = GetModalTitle();
	if (!ImGui::IsPopupOpen(title))
	{
		ImGui::OpenPopup(title);
		OnOpen();
	}

	const ImVec2 center = ImGui::GetMainViewport()->GetCenter();
	ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
	ImGui::SetNextWindowSize(GetModalSize());

	if (ImGui::BeginPopupModal(title, nullptr, ImGuiWindowFlags_AlwaysAutoResize))
	{
		OnDraw();

		ImGui::EndPopup();
	}
}

void ModalWindow::CloseModal()
{
	ensure(_parentWindow != nullptr);
	ensure(static_cast<const Window*>(_parentWindow)->GetCurrentModal() == this);

	_parentWindow->PopModal();
}

void ModalWindow::Init(Window& parentWindow)
{
	_parentWindow = &parentWindow;
}
