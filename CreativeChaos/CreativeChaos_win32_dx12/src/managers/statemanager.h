#pragma once

#include "util/proxy.h"
#include "core/manager.h"
#include "managers/TaskManager.h"

enum class EApplicationMode : uint8_t
{
	Browse,
	EditTask,
	NewTask
};

inline const char* ToString(EApplicationMode applicationMode)
{
	switch (applicationMode)
	{
		case EApplicationMode::Browse: return "Browse";
		case EApplicationMode::EditTask: return "EditTask";
		case EApplicationMode::NewTask: return "NewTask";
	}

	return "";
}

class StateManager : public Manager
{
public:
	void Update() override;

	EApplicationMode GetMode() { return _mode; }
	void SetMode(EApplicationMode value) { _mode = value; }

	TaskManager::TaskPtr GetCurrentSelectedTask() const { return _currentSelectedTask; }
	void SetCurrentSelectedTask(TaskManager::TaskPtr value) { _currentSelectedTask = value; }

private:
	EApplicationMode _mode = EApplicationMode::Browse;
	TaskManager::TaskPtr _currentSelectedTask;

};

struct StateManagerProxy : public Proxy<StateManager>
{
	StateManagerProxy();
};