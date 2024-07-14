#pragma once

#include "time/time.h"
#include "util/jsonfwd.h"
#include "model/taskpriority.h"
#include "util/hash.h"

enum class ETaskCategory : uint8_t
{
	Main,
	Backlog,
	Trash,
	Count
};

struct TaskEvent
{
	// ToString below
	enum class EType : uint8_t
	{
		Start,
		Stop
	};

	EType Type;
	Timestamp Time;

	TaskEvent() = default;
	TaskEvent(EType type, Timestamp time) :
		Type{ type },
		Time{ time }
	{

	}
};

inline const char* ToString(TaskEvent::EType taskEventType)
{
	switch (taskEventType)
	{
		case TaskEvent::EType::Start: return "Activate";
		case TaskEvent::EType::Stop: return "Deactivate";
	}

	return "UNKNOWN";
}

struct Task
{
	uint32_t ID;
	std::string Name;

	ETaskCategory Category;
	
	TaskPriority Priority;	// ptodo - serialize
	Timestamp CreatedAt;
	TimeSpan Progress;

	// ptodo - to flags?
	bool Active = false;
	bool Done = false;

	// Private data that shouldn't be manipulated directly (except serialization, but that should be fixed).
	std::string _customCategory;
	HashType _customCategoryHash;	// Derived
	std::vector<TaskEvent> _events;	// ensure events are sorted

	// Accessors
	const std::string& GetCustomCategory() const { return _customCategory; }
	HashType GetCustomCategoryHash() const { return _customCategoryHash; }
	const std::vector<TaskEvent>&  Events() const { return _events; }
	
	Task() = default;
	Task(const char* name, ETaskCategory category, uint32_t id, Timestamp createdAt = Timestamp::Now()) :
		Name{ name },
		Category{ category },
		ID{ id },
		CreatedAt{ createdAt }
	{

	}

	TimeSpan CalculateProgress() const;

	void SetCustomCategory(const char* value);
	
	template<typename... Args>
	void PushEvent(const Args&... args)
	{
		_events.emplace_back(std::forward<const Args>(args)...);
		Progress = CalculateProgress();
	}

};

// Serialization
void to_json(nlohmann::json& json, const Task& task);
void from_json(const nlohmann::json& json, Task& task);

void to_json(nlohmann::json& json, const TaskEvent& taskEvent);
void from_json(const nlohmann::json& json, TaskEvent& taskEvent);