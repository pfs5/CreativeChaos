#pragma once
#include "core/window.h"
#include "managers/TaskManager.h"

class Window_Tasks : public Window
{
public:
	void NewTaskModal();

private:
	struct TaskList
	{
		std::string Title;
		std::vector<TaskManager::TaskPtr> Tasks;
	};

private:
	uint32_t _currentSelectedTaskIdx = -1;

	static constexpr uint32_t INPUT_BUFFER_SIZE = 256;
	char _inputBuffer [INPUT_BUFFER_SIZE];

	// Updated every draw
	std::vector<TaskList> _taskLists;

private:
	const char* GetWindowName() const final { return "Tasks"; }
	void OnDraw() final;

	void InitTasks();
	void ProcessInputs();
	void DrawTasks();
	void DrawNewTaskModal();
	
	void DrawActiveButton();

	size_t GetTotalNumTasks() const;
	void ClearInputBuffer() { _inputBuffer[0] = '\0'; }
};