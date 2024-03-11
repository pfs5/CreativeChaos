#include "pch.h"
#include "taskviews/taskview_prio.h"

#include "managers/TaskManager.h"

TaskView_Prio::TaskView_Prio()
{
	static constexpr uint8_t NUM_PRIOS = TaskPriority::MAX_PRIO - TaskPriority::MIN_PRIO + 1;
	_collections.resize(NUM_PRIOS + 1); // One more category for deleted tasks.

	for (uint8_t prio = TaskPriority::MIN_PRIO; prio <= TaskPriority::MAX_PRIO; ++prio)
	{
		const TaskPriority priority{ prio };
		
		TaskCollection& col = GetCollection(priority);
		col.Title = StringFormat("P%d", prio);
		col.ShowIndex = false;
	}

	// Deleted category
	{
		TaskCollection& col = _collections.back();
		col.Title = "Deleted";
		col.ShowIndex = false;
	}
}

void TaskView_Prio::UpdateTasks()
{
	for (uint8_t prio = TaskPriority::MIN_PRIO; prio <= TaskPriority::MAX_PRIO; ++prio)
	{
		const TaskPriority priority{ prio };
		
		TaskCollection& col = GetCollection(priority);
		col.Tasks.clear();
		col.Tasks.reserve(64);
		TaskManagerProxy::Get().FindTasksWithPredicate(col.Tasks, [priority] (const Task& t) { return t.Category != ETaskCategory::Trash && t.Priority == priority; });
	}

	// Deleted category
	{
		TaskCollection& col = _collections.back();
		col.Tasks.clear();
		col.Tasks.reserve(64);
		TaskManagerProxy::Get().FindTasksWithPredicate(col.Tasks, [](const Task& t) { return t.Category == ETaskCategory::Trash; });
	}
}

void TaskView_Prio::ChangeTaskCollectionUp(TaskPtr task) const
{
	const Task& t = TaskManagerProxy::Get().GetTask(task);
	
	TaskPriority newPrio { t.Priority};
	newPrio -= 1;

	TaskManagerProxy::Get().SetTaskPriority(task, newPrio);
}

void TaskView_Prio::ChangeTaskCollectionDown(TaskPtr task) const
{
	const Task& t = TaskManagerProxy::Get().GetTask(task);

	TaskPriority newPrio{ t.Priority };
	newPrio += 1;

	TaskManagerProxy::Get().SetTaskPriority(task, newPrio);
}

TaskCollection& TaskView_Prio::GetCollection(TaskPriority priority)
{
	const uint8_t prio = priority.GetValue();
	ensure(prio >= TaskPriority::MIN_PRIO && prio <= TaskPriority::MAX_PRIO);

	const uint32_t index = prio - TaskPriority::MIN_PRIO;
	return _collections[index];
}

