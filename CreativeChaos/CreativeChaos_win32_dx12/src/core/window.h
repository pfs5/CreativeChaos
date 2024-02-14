#pragma once

#include "managers/inputscommon.h"

class Window
{
public:
	void Draw();
	virtual void OnRegister() {}

protected:
	using InputCallback = std::function<void(const InputEvent& e)>;
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

protected:
	void RegisterInputCallback(EInputAction action, const InputCallback& callback)
	{
		_registedInputListeners[std::underlying_type_t<EInputAction>(action)] = InputListener{ callback };
	}

	template <typename WindowClass>
	void RegisterInputCallbackTemplated(WindowClass* object, EInputAction action, void (WindowClass::*callback)(const InputEvent& e))
	{
		RegisterInputCallback(action, [object, callback](const InputEvent& e) { (object->*callback)(e); });
	}

private:
	InputListener _registedInputListeners [std::underlying_type_t<EInputAction>(EInputAction::Count)];

private:
	virtual const char* GetWindowName() const = 0;
	virtual void OnDraw() = 0;

	void ProcessInputs();

};