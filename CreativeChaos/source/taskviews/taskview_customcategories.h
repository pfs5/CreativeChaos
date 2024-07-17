#pragma once

#include "model/taskview.h"

struct TaskCollection;

enum class ETaskCategory : uint8_t;

class TaskView_CustomCategories : public TaskView
{
public:
	const char* GetTitle() const { return "Custom Categories"; };
	void UpdateTasks() override;
	void ChangeTaskCollectionUp(TaskPtr task)  const override;
	void ChangeTaskCollectionDown(TaskPtr task) const override;

private:
	bool _initialized = false;

private:
	void Initialize();
};