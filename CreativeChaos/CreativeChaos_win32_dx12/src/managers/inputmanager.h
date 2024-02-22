#pragma once

#include "core/manager.h"
#include "managers/inputscommon.h"
#include "util/proxy.h"

class InputManager : public Manager
{
public:
	using InputCallback = std::function<void(const InputEvent& e)>;

public:
	InputManager();

	void Update() override;

	bool IsInputPressed(EInputAction input) const { return GetInputValue(input); }

	void RegisterInputCallback(EInputAction action, const InputCallback& callback);
	void UnregisterInputCallback(EInputAction action, const InputCallback& callback);

private:
	std::bitset<(std::underlying_type_t<EInputAction>)(EInputAction::Count)> _inputPressed;
	InputCallback _inputCallbacks [(std::underlying_type_t<EInputAction>)(EInputAction::Count)];

private:
	void ResetInputs();

	void UpdateInputs_App();

	void UpdateInputs_Browse();
	void UpdateInputs_EditTask();
	void UpdateInputs_NewTask();

	bool GetInputValue(EInputAction input) const { return _inputPressed.test((std::underlying_type_t<EInputAction>) input); }
	void SetInputValue(EInputAction input, bool value);

	InputCallback& GetInputCallback(EInputAction action) { return _inputCallbacks[(std::underlying_type_t<EInputAction>) action]; }
};

struct InputManagerProxy : public Proxy<InputManager>
{
	InputManagerProxy();
};