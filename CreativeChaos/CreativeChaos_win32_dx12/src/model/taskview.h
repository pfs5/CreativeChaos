#pragma once
#include "managers/TaskManager.h"

struct TaskCollection
{
	std::string Title;
	std::vector<TaskPtr> Tasks;
	bool IsOpen = true;
	bool ShowIndex = true;
};

class TaskView
{
public:
	virtual void UpdateTasks() {}

	// We provide controls to move tasks between collections. Since collections depend on task views, we leave this responsibilty to task view implementations.
	virtual void ChangeTaskCollectionUp(TaskPtr task)  const {}
	virtual void ChangeTaskCollectionDown(TaskPtr task) const {}

	const std::vector<TaskCollection>& GetCollections() const { return _collections; }
	std::vector<TaskCollection>& GetCollections() { return _collections; }
	uint32_t GetNumTasks() const;
	uint32_t GetNumVisibleTasks() const;
	TaskCollection* FindCollectionForTask(TaskPtr task);

protected:
	std::vector<TaskCollection> _collections;
};