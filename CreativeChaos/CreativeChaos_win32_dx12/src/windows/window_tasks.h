#pragma once
#include "core/window.h"
#include "managers/TaskManager.h"

struct InputEvent;
class TaskView;

class Window_Tasks : public Window
{
public:
	Window_Tasks();
	~Window_Tasks();

	void OnRegister() override;

private:
	struct TaskList
	{
		std::string Title;
		std::vector<TaskManager::TaskPtr> Tasks;
		bool Open = true;
	};

private:
	uint32_t _currentSelectedTaskIdx = -1;

	static constexpr uint32_t INPUT_BUFFER_SIZE = 256;
	char _inputBuffer [INPUT_BUFFER_SIZE];

	// Updated every draw
	std::vector<TaskList> _taskLists;

	std::vector<std::unique_ptr<TaskView>> _taskViews;
	uint32_t _currentTaskViewIdx = 0;

	// true if selected task has changed since last frame. cleared at draw.
	bool _selectionChanged = false;

private:
	const char* GetWindowName() const final { return "Tasks"; }
	void OnDraw() final;

	const TaskView* GetCurrentTaskView() const { return _currentTaskViewIdx < _taskViews.size() ? _taskViews[_currentTaskViewIdx].get() : nullptr; }
	TaskView* GetCurrentTaskView() { return const_cast<TaskView*>(const_cast<const Window_Tasks&>(*this).GetCurrentTaskView()); }

	void UpdateTasks();
	void DrawTasks();
	void DrawNewTaskModal();
	
	void DrawActiveButton();

	size_t GetTotalNumTasks() const;
	void ClearInputBuffer() { _inputBuffer[0] = '\0'; }

	// Inputs
	void OnInput_NextTask(const InputEvent& e);
	void OnInput_PreviousTask(const InputEvent& e);
	void OnInput_NextTaskView(const InputEvent& e);
	template<uint32_t CategoryIdx> void OnInput_ToggleCategory(const InputEvent& e) { OnInput_ToggleCategory(e, CategoryIdx); }
	void OnInput_ToggleCategory(const InputEvent& e, uint32_t categoryIdx);
	void OnInput_ScrollDown(const InputEvent& e);
	void OnInput_ScrollUp(const InputEvent& e);
	void OnInput_EditTask(const InputEvent& e);
	void OnInput_ToggleTaskActive(const InputEvent& e);
	void OnInput_NewTask(const InputEvent& e);
	void OnInput_DeleteTask(const InputEvent& e);
	void OnInput_ChangeTaskCategory(const InputEvent& e);
	void OnInput_ConfirmEditTask(const InputEvent& e);
	void OnInput_CancelEditTask(const InputEvent& e);
	void OnInput_ExitApp(const InputEvent& e);
};