#pragma once

#include "core/window.h"

class Window_Log : public Window
{
private:
	const char* GetWindowName() const final { return "Log"; }
	void OnDraw() final;
};