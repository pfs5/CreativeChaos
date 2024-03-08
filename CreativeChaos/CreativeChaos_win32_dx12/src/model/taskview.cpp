#include "pch.h"
#include "model/taskview.h"

uint32_t TaskView::GetNumTasks() const
{
	uint32_t num = 0;
	for (const TaskCollection& collection : _collections)
	{
		num += (uint32_t) collection.Tasks.size();
	}

	return num;
}

uint32_t TaskView::GetNumVisibleTasks() const
{
	uint32_t num = 0;
	for (const TaskCollection& collection : _collections)
	{
		if (collection.IsOpen)
		{
			num += (uint32_t)collection.Tasks.size();
		}
	}

	return num;

}

TaskCollection* TaskView::FindCollectionForTask(TaskPtr task)
{
	for (TaskCollection& collection : _collections)
	{
		if (std::any_of(collection.Tasks.begin(), collection.Tasks.end(), [task](const TaskPtr& t) { return task == t; }))
		{
			return &collection;
		}
	}

	return nullptr;
}
