#include "pch.h"
#include "platform/windowmanager_glfw.h"

#include <GLFW/glfw3.h>

void WindowManager_GLFW::MinimizeWindow()
{
    glfwIconifyWindow(_handle);
}