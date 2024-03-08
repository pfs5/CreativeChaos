#pragma once

#include "model/taskview.h"

class TaskView_Prio : public TaskView
{
public:
	TaskView_Prio();

	void UpdateTasks() override;
	void ChangeTaskCollectionUp(TaskPtr task)  const override;
	void ChangeTaskCollectionDown(TaskPtr task) const override;

private:
	TaskCollection& GetCollection(TaskPriority priority);

};