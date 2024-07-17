#pragma once

#include "util/proxy.h"

class WindowManagerInterface
{
public:
	virtual void MinimizeWindow() = 0;
};

struct WindowManagerProxy : Proxy<WindowManagerInterface>
{
	WindowManagerProxy();
};