#pragma once

#include "core/window.h"

class Window_Properties : public Window
{
private:
	const char* GetWindowName() const final { return "Properties"; }
	void OnDraw() final;
};