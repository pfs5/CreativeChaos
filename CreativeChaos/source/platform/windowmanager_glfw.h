#pragma once
#include "managers/WindowManager.h"

class GLFWwindow;

class WindowManager_GLFW : public WindowManagerInterface
{
public:
    WindowManager_GLFW(GLFWwindow* handle):
        _handle{ handle }
    {

    }

    void MinimizeWindow() override;

private:
    GLFWwindow* _handle;
};