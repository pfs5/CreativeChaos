#include "pch.h"
#include "taskviews/taskview_categories.h"

#include "managers/TaskManager.h"

TaskView_Categories::TaskView_Categories()
{
	_collections.resize((std::underlying_type_t<ETaskCategory>)(ETaskCategory::Count));
	
	GetCollection(ETaskCategory::Main).Title = "Main";
	GetCollection(ETaskCategory::Backlog).Title = "Backlog";
	GetCollection(ETaskCategory::Trash).Title = "Trash";
}

void TaskView_Categories::UpdateTasks()
{
	TaskCollection& tcollMain = GetCollection(ETaskCategory::Main);
	tcollMain.Tasks.clear();
	tcollMain.Tasks.reserve(64);
	TaskManagerProxy::Get().FindTasksWithCategory(tcollMain.Tasks, ETaskCategory::Main);

	TaskCollection& tcollBacklog = GetCollection(ETaskCategory::Backlog);
	tcollBacklog.Tasks.clear();
	tcollBacklog.Tasks.reserve(64);
	TaskManagerProxy::Get().FindTasksWithCategory(tcollBacklog.Tasks, ETaskCategory::Backlog);

	TaskCollection& tcollTrash = GetCollection(ETaskCategory::Trash);
	tcollTrash.Tasks.clear();
	tcollTrash.Tasks.reserve(64);
	TaskManagerProxy::Get().FindTasksWithCategory(tcollTrash.Tasks, ETaskCategory::Trash);
}

TaskCollection& TaskView_Categories::GetCollection(ETaskCategory category)
{
	return _collections[(std::underlying_type_t<ETaskCategory>(category))];
}
