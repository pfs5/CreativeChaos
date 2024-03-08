#pragma once

#include "util/proxy.h"
#include "core/manager.h"
#include "managers/TaskManager.h"

enum class EApplicationMode : uint8_t
{
	Default,
	EditTask,
	Modal
};

inline const char* ToString(EApplicationMode applicationMode)
{
	switch (applicationMode)
	{
		case EApplicationMode::Default: return "Default";
		case EApplicationMode::EditTask: return "EditTask";
		case EApplicationMode::Modal: return "Modal";
	}

	return "";
}

class StateManager : public Manager
{
public:
	void Update() override;

	EApplicationMode GetMode() { return _mode; }
	void SetMode(EApplicationMode value) { _mode = value; }

	TaskPtr GetCurrentSelectedTask() const { return _currentSelectedTask; }
	void SetCurrentSelectedTask(TaskPtr value) { _currentSelectedTask = value; }

private:
	EApplicationMode _mode = EApplicationMode::Default;
	TaskPtr _currentSelectedTask;

};

struct StateManagerProxy : public Proxy<StateManager>
{
	StateManagerProxy();
};