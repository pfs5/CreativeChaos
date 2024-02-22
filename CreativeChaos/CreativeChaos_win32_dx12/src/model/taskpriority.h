#pragma once

class TaskPriority
{
public:
	static constexpr uint8_t MAX_PRIO = 10; 
	static constexpr uint8_t MIN_PRIO = 0; 

public:
	TaskPriority() :
		_value{ MAX_PRIO }
	{

	}

	uint8_t GetValue() const { return _value; }
	void SetValue(uint8_t value) { }

private:
	uint8_t _value = 0;

private:
	void ClampValue() { _value = std::clamp(_value, MIN_PRIO, MAX_PRIO); }
};
