#include "pch.h"
#include "core/window.h"

#include "imgui.h"
#include "managers/inputmanager.h"

void Window::Draw()
{
	ImGui::Begin(GetWindowName());
	ProcessInputs();
	OnDraw();
	ImGui::End();
}

void Window::ProcessInputs()
{
	InputManager& inputManager = InputManagerProxy::Get();

	for (std::underlying_type_t<EInputAction> i = 0; i < std::underlying_type_t<EInputAction>(EInputAction::Count); ++i)
	{
		InputListener& listener = _registedInputListeners[i];
		if (!listener.IsSet())
		{
			continue;
		}

		const EInputAction action = (EInputAction)i;

		if (inputManager.IsInputPressed(action))
		{
			InputEvent e;
			_registedInputListeners[i].Callback(e);
		}
	}
}
