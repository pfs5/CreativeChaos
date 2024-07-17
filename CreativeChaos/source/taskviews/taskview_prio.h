#pragma once

#include "model/taskview.h"

class TaskView_Prio : public TaskView
{
public:
	TaskView_Prio();

	const char* GetTitle() const { return "Priorities"; };
	void UpdateTasks() override;
	void ChangeTaskCollectionUp(TaskPtr task)  const override;
	void ChangeTaskCollectionDown(TaskPtr task) const override;

private:
	TaskCollection& GetCollection(TaskPriority priority);

};