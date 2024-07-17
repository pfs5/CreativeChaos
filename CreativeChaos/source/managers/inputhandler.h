#pragma once

#include "managers/inputscommon.h"

class InputHandler
{
public:
	void RegisterInputCallback(EInputAction action, const InputCallback& callback)
	{
		_registedInputListeners[std::underlying_type_t<EInputAction>(action)] = InputListener{ callback };
	}

	template <typename ObjectClass>
	void RegisterInputCallbackTemplated(ObjectClass* object, EInputAction action, void (ObjectClass::* callback)(const InputEvent& e))
	{
		RegisterInputCallback(action, [object, callback](const InputEvent& e) { (object->*callback)(e); });
	}

	void ProcessInputs();

private:
	struct InputListener
	{
		InputCallback Callback;

		InputListener() = default;
		InputListener(const InputCallback& callback) :
			Callback{ callback }
		{

		}

		operator bool() const { return IsSet(); }
		bool IsSet() const { return Callback != nullptr; }
	};

private:
	InputListener _registedInputListeners[std::underlying_type_t<EInputAction>(EInputAction::Count)];

};