#include "pch.h"
#include "managers/taskmanager.h"

Task& TaskManager::TaskPtr::GetTask() const
{
	return TaskManagerProxy::Get().GetTask(Index);
}

TaskManager::TaskManager::TaskPtr& TaskManager::TaskPtr::operator++()
{
	const uint32_t numTasks = TaskManagerProxy::Get().GetNumTasks();
	if (numTasks == 0)
	{
		return *this;
	}

	Index = (Index + 1) % numTasks;
	return *this;
}

TaskManager::TaskManager::TaskPtr& TaskManager::TaskPtr::operator--()
{
	const uint32_t numTasks = TaskManagerProxy::Get().GetNumTasks();
	if (numTasks == 0)
	{
		return *this;
	}

	--Index;
	Index = Index < 0 ? Index + numTasks : Index;
	return *this;
}

const TaskManager::TaskPtr TaskManager::TaskPtr::Invalid;

TaskManager::TaskManager()
{
	// Init dummy tasks
	_tasks.emplace_back("Implement list of tasks in model.", ETaskCategory::Main, (uint32_t)_tasks.size());
	_tasks.emplace_back("Implement basic view with <main> tasks and <backlog> tasks.", ETaskCategory::Main, (uint32_t)_tasks.size());
	_tasks.emplace_back("Implement ability to switch views. The first view is per task type. The other view uses filters (show active?)", ETaskCategory::Main, (uint32_t)_tasks.size());
	_tasks.emplace_back("Implement ability to iterate trough tasks via arrow keys.", ETaskCategory::Main, (uint32_t)_tasks.size());
	_tasks.emplace_back("Pressing <rename> shortcut ALT+F2 opens a rename edit box to rename the selected task.", ETaskCategory::Main, (uint32_t)_tasks.size());
	_tasks.emplace_back("Pressing ENTER opens the task in the properties window.", ETaskCategory::Main, (uint32_t)_tasks.size());
	_tasks.emplace_back("The properties window will display additional info about the task.", ETaskCategory::Main, (uint32_t)_tasks.size());

	_tasks.emplace_back("Implement main menu bar.", ETaskCategory::Backlog, (uint32_t)_tasks.size());
	_tasks.emplace_back("Implement ability to add a new task. Automatically add to main.", ETaskCategory::Backlog, (uint32_t)_tasks.size());
	_tasks.emplace_back("Shortcut to move current task to backlog/main.", ETaskCategory::Backlog, (uint32_t)_tasks.size());
	_tasks.emplace_back("Shortcut to delete current task. Delete moves to <trash> category.", ETaskCategory::Backlog, (uint32_t)_tasks.size());

}

void TaskManager::CreateNewTask(const char* name, ETaskCategory category)
{
	_tasks.emplace_back(name, category, (uint32_t)_tasks.size());
}

void TaskManager::FindTasksWithCategory(std::vector<TaskPtr>& outTasks, ETaskCategory category)
{
	for (uint32_t i = 0; i < _tasks.size(); ++i)
	{
		const Task& t = _tasks[i];
		if (t.GetCategory() == category)
		{
			outTasks.emplace_back(i);
		}
	}
}

TaskManagerProxy::TaskManagerProxy() = default;
