#pragma once

enum class EInputAction : uint8_t
{
	NextTask,
	PreviousTask,

	ScrollDown,
	ScrollUp,
	
	NextTaskView,

	ToggleCategory_1,
	ToggleCategory_2,
	ToggleCategory_3,
	ToggleCategory_4,
	ToggleCategory_5,
	ToggleCategory_6,
	ToggleCategory_7,
	ToggleCategory_8,
	ToggleCategory_9,

	EditTask,
	NewTask,
	DeleteTask,
	ChangeTaskPrio,
	ChangeTaskCategory,
	ToggleTaskActive,

	ConfirmEditTask,
	CancelEditTask,

	Confirm,
	Cancel,

	MinimizeApp,
	ExitApp,

	Count
};

struct InputEvent
{

};

using InputCallback = std::function<void(const InputEvent& e)>;
