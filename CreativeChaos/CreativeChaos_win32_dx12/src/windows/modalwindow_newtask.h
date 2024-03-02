#pragma once

#include "windows/modalwindow.h"

class ModalWindow_NewTask : public ModalWindow
{
	using Super = ModalWindow;

public:

protected:
	void OnDraw() final;
	void OnOpen() final;
	void SetupInputs(InputHandler& inputHandler) final;

private:
	static constexpr uint32_t INPUT_BUFFER_SIZE = 256;
	char _inputBuffer [INPUT_BUFFER_SIZE];

private:
	const char* GetModalTitle() const final { return "New Task"; }
	ImVec2 GetModalSize() const final;

	void ClearInputBuffer() { _inputBuffer[0] = '\0'; }

	// Inputs
	void OnInput_ConfirmNewTask(const InputEvent& e);
	void OnInput_Cancel(const InputEvent& e);

};