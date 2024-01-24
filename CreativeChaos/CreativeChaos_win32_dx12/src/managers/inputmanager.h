#pragma once

#include "core/manager.h"
#include "managers/inputscommon.h"
#include "util/proxy.h"

class InputManager : public Manager
{
public:
	InputManager();

	void Update() override;

	bool IsInputPressed(EInputAction input) const { return GetInputValue(input); }

private:
	std::bitset<(std::underlying_type_t<EInputAction>)(EInputAction::Count)> _inputPressed;

private:
	void ResetInputs();

	void UpdateInputs_Browse();
	void UpdateInputs_EditTask();
	void UpdateInputs_NewTask();

	bool GetInputValue(EInputAction input) const { return _inputPressed.test((std::underlying_type_t<EInputAction>) input); }
	void SetInputValue(EInputAction input, bool value) 
	{
		if (value) 
		{
			_inputPressed.set((std::underlying_type_t<EInputAction>)input);
		}
		else
		{
			_inputPressed.reset((std::underlying_type_t<EInputAction>)input);
		}
	}
};

struct InputManagerProxy : public Proxy<InputManager>
{
	InputManagerProxy();
};