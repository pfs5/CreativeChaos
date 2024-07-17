#pragma once

#include "core/window.h"

class Window_Debug : public Window
{
private:
	const char* GetWindowName() const final { return "Debug"; }
	void OnDraw() final;
};