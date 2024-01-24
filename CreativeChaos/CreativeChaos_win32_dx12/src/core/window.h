#pragma once

class Window
{
public:
	void Draw();

private:
	virtual const char* GetWindowName() const = 0;
	virtual void OnDraw() = 0;
};