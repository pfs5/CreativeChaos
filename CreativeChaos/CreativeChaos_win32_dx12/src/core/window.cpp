#include "pch.h"
#include "core/window.h"

#include "imgui.h"

void Window::Draw()
{
	ImGui::Begin(GetWindowName());
	OnDraw();
	ImGui::End();
}
