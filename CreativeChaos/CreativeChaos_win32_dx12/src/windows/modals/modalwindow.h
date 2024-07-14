#pragma once
#include "managers/inputhandler.h"

struct ImVec2;

// Modal windows are handled as part of a parent window. Only a window can create modals.
class ModalWindow
{
friend class Window;

public:
	void Draw();

protected:
	ModalWindow() = default;

	virtual void OnDraw() {}
	virtual void OnOpen() { SetupInputs(_inputHandler); }
	virtual void SetupInputs(InputHandler& inputHandler) {}

	void CloseModal();

	// Window interface
	void Init(Window& parentWindow);
	void ProcessInputs() { _inputHandler.ProcessInputs(); }

private:
	Window* _parentWindow = nullptr;
	InputHandler _inputHandler;

private:
	virtual const char* GetModalTitle() const = 0;
	virtual ImVec2 GetModalSize() const = 0;

};