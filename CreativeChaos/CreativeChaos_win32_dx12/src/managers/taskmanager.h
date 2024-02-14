#pragma once

#include "model/task.h"
#include "util/proxy.h"
#include "core/manager.h"

class TaskManager : public Manager
{
public:
	struct TaskPtr
	{
		uint32_t Index = -1;

		TaskPtr() = default;
		TaskPtr(uint32_t index) :
			Index{ index }
		{

		}

		bool IsValid() const { return Index != -1; }
		Task& GetTask() const;

		TaskPtr& operator++();
		TaskPtr& operator--();

		bool operator==(TaskPtr other) const { return Index == other.Index; }
		bool operator!=(TaskPtr other) const { return Index != other.Index; }

		static const TaskPtr Invalid;
	};

public:
	void Initialize();

	void CreateNewTask(const char* name, ETaskCategory category);

	void FindTasksWithCategory(std::vector<TaskPtr>& outTasks, ETaskCategory category);

	using FindTaskPredicate = std::function<bool(const Task& t)>;
	void FindTasksWithPredicate(std::vector<TaskPtr>& outTasks, const FindTaskPredicate& predicate);

	Task& GetTask(TaskPtr ptr) { return _tasks[ptr.Index]; }
	TaskPtr GetFirstTaskPtr() { return _tasks.size() > 0 ? TaskPtr{0} : TaskPtr::Invalid; }
	uint32_t GetNumTasks() const { return (uint32_t)_tasks.size(); }

	void StartTask(TaskPtr ptr);
	void StopTask(TaskPtr ptr);
	void SetTaskCategory(TaskPtr ptr, ETaskCategory category);

	void MarkDirty();

	// Database
	void SaveTasks() const;
	void LoadTasks();

private:
	std::vector<Task> _tasks;
};

struct TaskManagerProxy : public Proxy<TaskManager>
{
	TaskManagerProxy();
};