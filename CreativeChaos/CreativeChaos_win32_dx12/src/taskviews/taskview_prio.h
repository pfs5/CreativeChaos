#pragma once

#include "model/taskview.h"

class TaskView_Prio : public TaskView
{
public:
	TaskView_Prio();

	void UpdateTasks() override;
	void ChangeTaskCollectionUp(TaskManager::TaskPtr task)  const override;
	void ChangeTaskCollectionDown(TaskManager::TaskPtr task) const override;

private:
	TaskCollection& GetCollection(TaskPriority priority);

};