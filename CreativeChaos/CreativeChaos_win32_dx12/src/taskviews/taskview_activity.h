#pragma once

#include "model/taskview.h"

class TaskView_Activity : public TaskView
{
public:
	TaskView_Activity();

	void UpdateTasks() override;

private:
	enum class ECategory : uint8_t
	{
		Active,
		Idle,
		Trash,

		Count
	};

private:
	TaskCollection& GetCollection(ECategory category);
};