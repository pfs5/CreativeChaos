#pragma once

enum class EInputAction : uint8_t
{
	NextTask,
	PreviousTask,
	
	EditTask,
	NewTask,
	DeleteTask,
	ChangeTaskCategory,
	ToggleTaskActive,

	ConfirmEditTask,
	CancelEditTask,

	ConfirmNewTask,
	CancelNewTask,

	Count
};