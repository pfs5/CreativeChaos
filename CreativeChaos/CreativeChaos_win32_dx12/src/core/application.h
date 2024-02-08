#pragma once

#include "managers/debugmanager.h"
#include "managers/inputmanager.h"
#include "managers/statemanager.h"
#include "managers/taskmanager.h"
#include "windows/window_debug.h"
#include "windows/window_log.h"
#include "windows/window_properties.h"
#include "windows/window_tasks.h"

class Manager;
class Window;

class Application
{
public:
	void Init();
	void Shutdown();

	void Update();

private:
	DebugManager _debugManager;
	TaskManager _taskManager;
	InputManager _inputManager;
	StateManager _stateManager;
	
	// Windows
	Window_Tasks _windowTasks;
	Window_Properties _windowProperties;
	Window_Debug _windowDebug;
	Window_Log _windowLog;

	std::vector<Window*> _windows;
	std::vector<Manager*> _managers;

private:
	void RegisterWindow(Window& w) { _windows.emplace_back(&w); }
	void RegisterManager(Manager& m) { _managers.emplace_back(&m); }

	void InitTheme_1();
	void InitTheme_2();
};

