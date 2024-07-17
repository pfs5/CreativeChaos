#include "pch.h"
#include "taskview_activity.h"

TaskView_Activity::TaskView_Activity()
{
	_collections.resize((std::underlying_type_t<ECategory>)(ECategory::Count));

	GetCollection(ECategory::Active).Title = "Active";
	GetCollection(ECategory::Idle).Title = "Idle";
	GetCollection(ECategory::Trash).Title = "Trash";
}

void TaskView_Activity::UpdateTasks()
{
	TaskCollection& tcollActive = GetCollection(ECategory::Active);
	tcollActive.Tasks.clear();
	tcollActive.Tasks.reserve(64);
	TaskManagerProxy::Get().FindTasksWithPredicate(tcollActive.Tasks, [](const Task& t) { return t.Active; });

	TaskCollection& tcollIdle = GetCollection(ECategory::Idle);
	tcollIdle.Tasks.clear();
	tcollIdle.Tasks.reserve(64);
	TaskManagerProxy::Get().FindTasksWithPredicate(tcollIdle.Tasks, [](const Task& t) { return !t.Active; });

	TaskCollection& tcollTrash = GetCollection(ECategory::Trash);
	tcollTrash.Tasks.clear();
	tcollTrash.Tasks.reserve(64);
	TaskManagerProxy::Get().FindTasksWithCategory(tcollTrash.Tasks, ETaskCategory::Trash);
}

TaskCollection& TaskView_Activity::GetCollection(ECategory category)
{
	return _collections[(std::underlying_type_t<ECategory>(category))];
}
