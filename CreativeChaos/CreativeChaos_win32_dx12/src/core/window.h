#pragma once

#include "managers/inputscommon.h"
#include "managers/inputhandler.h"

class ModalWindow;

class Window
{
public:
	Window();
	~Window();

	void Draw();
	void PopModal();	// modal will be popped at the end of the frame
	const ModalWindow* GetCurrentModal() const;

	virtual void OnRegister() { SetupInputs(); }

protected:
	virtual void SetupInputs() {}

	void RegisterInputCallback(EInputAction action, const InputCallback& callback)
	{
		_inputHandler.RegisterInputCallback(action, callback);
	}

	template <typename WindowClass>
	void RegisterInputCallbackTemplated(WindowClass* object, EInputAction action, void (WindowClass::*callback)(const InputEvent& e))
	{
		_inputHandler.RegisterInputCallbackTemplated<WindowClass>(object, action, callback);
	}

	template <typename ModalWindowClass, typename... Args>
	void PushModal(const Args&&... params)
	{
		static_assert(std::is_base_of_v<ModalWindow, ModalWindowClass>);
		_modalWindowsStack.emplace_back(new ModalWindowClass(std::forward<Args>(params)...));
		PushModalInternal(*_modalWindowsStack.back());
	}

private:
	struct QueuedAction
	{
		enum class EType : uint8_t
		{
			PopModal
		};

		EType Type;

		QueuedAction(EType type) :
			Type{ type }
		{

		}
	};

private:
	InputHandler _inputHandler;
	std::vector<std::unique_ptr<ModalWindow>> _modalWindowsStack;
	std::vector<QueuedAction> _queuedActions;

private:
	virtual const char* GetWindowName() const = 0;
	virtual void OnDraw() = 0;

	void ProcessInputs();
	void ProcessActionQueue();

	void DrawModals();
	void PushModalInternal(ModalWindow& modal);
	void PopModalInternal();
	ModalWindow* GetCurrentModal();

};