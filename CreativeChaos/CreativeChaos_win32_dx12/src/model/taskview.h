#pragma once
#include "managers/TaskManager.h"

struct TaskCollection
{
	std::string Title;
	std::vector<TaskManager::TaskPtr> Tasks;
	bool IsOpen = true;
};

class TaskView
{
public:
	virtual void UpdateTasks() {}

	const std::vector<TaskCollection>& GetCollections() const { return _collections; }
	std::vector<TaskCollection>& GetCollections() { return _collections; }
	uint32_t GetNumTasks() const;
	uint32_t GetNumVisibleTasks() const;
	TaskCollection* FindCollectionForTask(TaskManager::TaskPtr task);

protected:
	std::vector<TaskCollection> _collections;
};