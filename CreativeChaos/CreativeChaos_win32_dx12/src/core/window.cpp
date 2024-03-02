#include "pch.h"
#include "core/window.h"

#include "imgui.h"
#include "managers/inputmanager.h"
#include "windows/modalwindow.h"
#include "managers/statemanager.h"

Window::Window() = default;
Window::~Window() = default;

void Window::Draw()
{
	ImGui::Begin(GetWindowName());
	ProcessInputs();
	OnDraw();
	DrawModals();
	ProcessActionQueue();
	ImGui::End();
}

void Window::PopModal()
{
	_queuedActions.emplace_back(QueuedAction::EType::PopModal);
}

const ModalWindow* Window::GetCurrentModal() const
{
	if (_modalWindowsStack.empty())
	{
		return nullptr;
	}

	ModalWindow* ret = _modalWindowsStack.back().get();
	ensure(ret != nullptr);	// unique_ptr should always be valid if it's in the stack
	return ret;
}

void Window::ProcessInputs()
{
	if (ModalWindow* modal = GetCurrentModal())
	{
		// Modal has priority.
		modal->ProcessInputs();
	}
	else
	{
		_inputHandler.ProcessInputs();
	}
}

void Window::ProcessActionQueue()
{
	for (const QueuedAction& action : _queuedActions)
	{
		if (action.Type == QueuedAction::EType::PopModal)
		{
			PopModalInternal();
		}
	}

	_queuedActions.clear();
}

void Window::DrawModals()
{
	ModalWindow* modal = GetCurrentModal();
	if (modal == nullptr)
	{
		return;
	}

	modal->Draw();
}

void Window::PushModalInternal(ModalWindow& modal)
{
	StateManagerProxy::Get().SetMode(EApplicationMode::Modal);
	modal.Init(*this);
	modal.OnOpen();
}

void Window::PopModalInternal()
{
	if (!_modalWindowsStack.empty())
	{
		_modalWindowsStack.pop_back();
	}

	if (_modalWindowsStack.empty())
	{
		StateManagerProxy::Get().SetMode(EApplicationMode::Default);
	}
}

ModalWindow* Window::GetCurrentModal()
{
	return const_cast<ModalWindow*>(const_cast<const Window&>(*this).GetCurrentModal());
}