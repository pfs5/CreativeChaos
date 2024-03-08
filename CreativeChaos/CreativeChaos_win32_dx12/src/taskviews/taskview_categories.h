#pragma once

#include "model/taskview.h"

struct TaskCollection;

enum class ETaskCategory : uint8_t;

class TaskView_Categories : public TaskView
{
public:
	TaskView_Categories();

	void UpdateTasks() override;
	void ChangeTaskCollectionUp(TaskPtr task)  const override;
	void ChangeTaskCollectionDown(TaskPtr task) const override;

private:
	TaskCollection& GetCollection(ETaskCategory category);
};