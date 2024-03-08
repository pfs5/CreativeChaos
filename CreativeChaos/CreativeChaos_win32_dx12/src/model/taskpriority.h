#pragma once

class TaskPriority
{
public:
	static constexpr uint8_t MAX_PRIO = 5; 
	static constexpr uint8_t MIN_PRIO = 0; 

public:
	TaskPriority() :
		_value{ MAX_PRIO }
	{

	}

	TaskPriority(uint8_t value) :
		_value{ std::clamp(value, MIN_PRIO, MAX_PRIO) }
	{

	}

	template <typename IntegerType, typename std::enable_if_t<std::is_integral<IntegerType>::value, bool> = true>
	TaskPriority(IntegerType value) :
		_value{ (uint8_t)std::clamp(value, (IntegerType)MIN_PRIO, (IntegerType)MAX_PRIO) }
	{

	}

	bool operator==(const TaskPriority& other) const { return _value == other._value; }
	bool operator!=(const TaskPriority& other) const { return _value != other._value; }

	TaskPriority& operator += (uint8_t offset) { _value += offset; ClampValue(); return *this; }
	TaskPriority& operator -= (uint8_t offset) { _value = offset <= _value ? _value - offset : 0; ClampValue(); return *this; }

	uint8_t GetValue() const { return _value; }
	void SetValue(uint8_t value) { _value = value; ClampValue(); }

private:
	uint8_t _value = 0;

private:
	void ClampValue() { _value = std::clamp(_value, MIN_PRIO, MAX_PRIO); }
};
