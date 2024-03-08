#pragma once
#include "windows/modalwindow.h"
#include "model/taskscommon.h"

class ModalWindow_ChangePrio : public ModalWindow
{
	using Super = ModalWindow;

public:
	ModalWindow_ChangePrio(const TaskPtr& task) :
		_targetTask{ task }
	{

	}

protected:
	void OnDraw() final;
	void OnOpen() final;
	void SetupInputs(InputHandler& inputHandler) final;

private:
	static constexpr uint32_t INPUT_BUFFER_SIZE = 256;
	char _inputBuffer[INPUT_BUFFER_SIZE];

	TaskPtr _targetTask;

private:
	const char* GetModalTitle() const final { return "Change Priority"; }
	ImVec2 GetModalSize() const final;

	void ClearInputBuffer() { _inputBuffer[0] = '\0'; }

	// Inputs
	void OnInput_Confirm(const InputEvent& e);
	void OnInput_Cancel(const InputEvent& e);
};