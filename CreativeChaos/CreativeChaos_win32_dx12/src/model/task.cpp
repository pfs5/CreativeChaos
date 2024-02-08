#include "pch.h"
#include "model/task.h"

#include "util/json.h"

TimeSpan Task::CalculateProgress() const
{
	TimeSpan progress {};

	Timestamp lastActivate;
	bool lastActivateValid = false;

	for (const TaskEvent& taskEvent : Events)
	{
		if (taskEvent.Type == TaskEvent::EType::Start)
		{
			lastActivate = taskEvent.Time;
			lastActivateValid = true;
		}
		else if (taskEvent.Type == TaskEvent::EType::Stop)
		{
			progress += Timestamp::Difference(taskEvent.Time, lastActivate, Timestamp::EDifferenceType::OnlyWorkingHours);
			lastActivateValid = false;
		}
	}

	if (lastActivateValid)
	{
		progress += Timestamp::Difference(Timestamp::Now(), lastActivate, Timestamp::EDifferenceType::OnlyWorkingHours);
	}

	return progress;
}

void to_json(nlohmann::json& json, const Task& task)
{
	json = nlohmann::json
	{
		{"id", task.ID},
		{"name", task.Name},
		{"category", (std::underlying_type_t<ETaskCategory>) task.Category},
		{"active", task.Active},
		{"createdAt", task.CreatedAt.ToUnix()},
		{"taskEvents", nlohmann::json {}}
	};

	for (const TaskEvent& taskEvent : task.Events)
	{
		json["taskEvents"].push_back(taskEvent);
	}
}

void from_json(const nlohmann::json& json, Task& task)
{
	json.at("id").get_to(task.ID);
	json.at("name").get_to(task.Name);

	std::underlying_type_t<ETaskCategory> category;
	json.at("category").get_to(category);
	task.Category = static_cast<ETaskCategory>(category);

	json.at("active").get_to(task.Active);

	uint64_t createdAt;
	json.at("createdAt").get_to(createdAt);
	task.CreatedAt = Timestamp::FromUnix(createdAt);
}

void to_json(nlohmann::json& json, const TaskEvent& taskEvent)
{
	json = nlohmann::json
	{
		{"type", (std::underlying_type_t<TaskEvent::EType>(taskEvent.Type))},
		{"time", taskEvent.Time.ToUnix()}
	};
}

void from_json(const nlohmann::json& json, TaskEvent& taskEvent)
{
	std::underlying_type_t<TaskEvent::EType> type;
	json.at("type").get_to(type);
	taskEvent.Type = static_cast<TaskEvent::EType>(type);

	uint64_t time;
	json.at("time").get_to(time);
	taskEvent.Time = Timestamp::FromUnix(time);
}
