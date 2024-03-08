#include "pch.h"
#include "platform/windowmanager_windows.h"

void WindowManager_Windows::MinimizeWindow()
{
	PostMessage(_handle, WM_SYSCOMMAND, SC_MINIMIZE, 0);
}

