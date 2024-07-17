#pragma once
#include "managers/WindowManager.h"
#include "windows.h"

class WindowManager_Windows : public WindowManagerInterface
{
public:
	WindowManager_Windows(HWND handle):
		_handle{ handle }
	{

	}

	void MinimizeWindow() override;

private:
	HWND _handle;
};