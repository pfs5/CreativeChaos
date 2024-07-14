#include "pch.h"
#include "taskviews/taskview_customcategories.h"

#include "managers/configmanager.h"
#include "managers/TaskManager.h"
#include "model/task.h"

void TaskView_CustomCategories::UpdateTasks()
{
	if (!_initialized)
	{
		Initialize();
	}
	
	const std::vector<std::string>& categories = ConfigManagerProxy::Get().GetData().TaskCategories;
	const size_t numCategories = categories.size() + 1;

	// 0 - special case for no category
	TaskCollection& collection = _collections[0];
	collection.Tasks.clear();
	collection.Tasks.reserve(64);
	TaskManagerProxy::Get().FindTasksWithPredicate(collection.Tasks, [](const Task& t) -> bool
	{
		return t.GetCustomCategoryHash() == InvalidHash;
	});
	
	for (uint32_t i = 1; i < numCategories; ++i)
	{
		TaskCollection& collection = _collections[i];
		collection.Tasks.clear();
		collection.Tasks.reserve(64);

		const HashType categoryHash = HashFun(categories[i - 1]);
		TaskManagerProxy::Get().FindTasksWithPredicate(collection.Tasks, [categoryHash](const Task& t) -> bool
		{
			return t.GetCustomCategoryHash() == categoryHash;
		});
	}
}

void TaskView_CustomCategories::ChangeTaskCollectionUp(TaskPtr task) const
{
	// ptodo
}

void TaskView_CustomCategories::ChangeTaskCollectionDown(TaskPtr task) const
{
	// ptodo
}

void TaskView_CustomCategories::Initialize()
{
	const std::vector<std::string>& categories = ConfigManagerProxy::Get().GetData().TaskCategories;
	const size_t numCategories = categories.size() + 1;
	
	_collections.resize(numCategories);

	// 0 is reserved for empty
	_collections[0].Title = "-";

	for (uint32_t i = 1; i < numCategories; ++i)
	{
		_collections[i].Title = categories[i - 1];
	}
	
	_initialized = true;
}
