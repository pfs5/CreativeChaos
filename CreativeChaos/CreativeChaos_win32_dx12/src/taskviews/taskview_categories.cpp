#include "pch.h"
#include "taskviews/taskview_categories.h"

#include "managers/TaskManager.h"
#include "model/task.h"

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

void TaskView_Categories::ChangeTaskCollectionUp(TaskManager::TaskPtr task) const
{
	const Task& t = TaskManagerProxy::Get().GetTask(task);
	switch (t.Category)
	{
		case ETaskCategory::Main:
		{
			// Can't go up from here.
			break;
		}
		case ETaskCategory::Backlog:
		{
			TaskManagerProxy::Get().SetTaskCategory(task, ETaskCategory::Main);
			break;
		}
		case ETaskCategory::Trash:
		{
			TaskManagerProxy::Get().SetTaskCategory(task, ETaskCategory::Backlog);
			break;
		}
	}
}

void TaskView_Categories::ChangeTaskCollectionDown(TaskManager::TaskPtr task) const
{
	const Task& t = TaskManagerProxy::Get().GetTask(task);
	switch (t.Category)
	{
		case ETaskCategory::Main:
		{
			TaskManagerProxy::Get().SetTaskCategory(task, ETaskCategory::Backlog);
			break;
		}
		case ETaskCategory::Backlog:
		{
			TaskManagerProxy::Get().SetTaskCategory(task, ETaskCategory::Trash);
			break;
		}
		case ETaskCategory::Trash:
		{
			// Can't go down from here.
			break;
		}
	}
}

TaskCollection& TaskView_Categories::GetCollection(ETaskCategory category)
{
	return _collections[(std::underlying_type_t<ETaskCategory>(category))];
}
