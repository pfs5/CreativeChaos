#include "pch.h"
#include "managers/taskmanager.h"

#include "util/json.h"
#include "core/staticconfig.h"
#include "util/performance.h"
#include "debugmanager.h"

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

void TaskManager::Initialize()
{
	LoadTasks();
}

void TaskManager::CreateNewTask(const char* name, ETaskCategory category)
{
	_tasks.emplace_back(name, category, (uint32_t)_tasks.size());

	MarkDirty();
}

void TaskManager::FindTasksWithCategory(std::vector<TaskPtr>& outTasks, ETaskCategory category)
{
	for (uint32_t i = 0; i < _tasks.size(); ++i)
	{
		const Task& t = _tasks[i];
		if (t.Category == category)
		{
			outTasks.emplace_back(i);
		}
	}
}

void TaskManager::FindTasksWithPredicate(std::vector<TaskPtr>& outTasks, const FindTaskPredicate& predicate)
{
	for (uint32_t i = 0; i < _tasks.size(); ++i)
	{
		const Task& t = _tasks[i];
		if (predicate(t))
		{
			outTasks.emplace_back(i);
		}
	}
}

void TaskManager::StartTask(TaskPtr ptr)
{
	Task& task = GetTask(ptr);
	task.Active = true;
	task.PushEvent(TaskEvent::EType::Start, Timestamp::Now());

	MarkDirty();
}

void TaskManager::StopTask(TaskPtr ptr)
{
	Task& task = GetTask(ptr);
	task.Active = false;
	task.PushEvent(TaskEvent::EType::Stop, Timestamp::Now());

	MarkDirty();
}

void TaskManager::SetTaskCategory(TaskPtr ptr, ETaskCategory category)
{
	Task& task = GetTask(ptr);

	const ETaskCategory oldCategory = task.Category;
	task.Category = category;

	if (oldCategory != category)
	{
		MarkDirty();
	}
}

void TaskManager::MarkDirty()
{
	SaveTasks();
}

void TaskManager::SaveTasks() const
{
	PerformanceTimer timer;

	nlohmann::json allTasks;
	for (const Task& task : _tasks)
	{
		nlohmann::json taskObj = task;
		allTasks.push_back(taskObj);
	}

	nlohmann::json baseObject;
	baseObject["tasks"] = allTasks;

	std::ofstream outFile(StaticConfig::DB_PATH_TASKS);
	outFile << std::setw(4) << baseObject << std::endl;

	const float elapsedMs = timer.GetElapsedMs();
	DebugManagerProxy::Get().PushLog(std::format("Save successful! Elapsed {:.2f}ms", elapsedMs).c_str());
}

void TaskManager::LoadTasks()
{
	PerformanceTimer timer;

	_tasks.clear();

	std::ifstream f(StaticConfig::DB_PATH_TASKS);
	nlohmann::json allTasks = nlohmann::json::parse(f);

	for (nlohmann::json taskJson : allTasks["tasks"])
	{
		_tasks.emplace_back();
		Task& t = _tasks[_tasks.size() - 1];
		from_json(taskJson, t);

		for (nlohmann::json taskEventJson : taskJson["taskEvents"])
		{
			t.Events.emplace_back();
			TaskEvent& taskEvent = t.Events[t.Events.size() - 1];
			from_json(taskEventJson, taskEvent);
		}
	}

	const float elapsedMs = timer.GetElapsedMs();
	DebugManagerProxy::Get().PushLog(std::format("Load successful! Elapsed {:.2f}ms", elapsedMs).c_str());
}

TaskManagerProxy::TaskManagerProxy() = default;
