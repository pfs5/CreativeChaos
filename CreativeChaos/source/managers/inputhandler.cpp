#include "pch.h"
#include "managers/inputhandler.h"

#include "managers/inputmanager.h"

void InputHandler::ProcessInputs()
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
			listener.Callback(e);

		}
	}
}
