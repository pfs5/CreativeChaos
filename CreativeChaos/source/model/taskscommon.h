#pragma once

struct Task;

struct TaskPtr
{
	uint32_t Index = static_cast<uint32_t>(-1);

	TaskPtr() = default;
	TaskPtr(uint32_t index) :
		Index{ index }
	{

	}

	bool IsValid() const { return Index != static_cast<uint32_t>(-1); }
	Task& GetTask() const;

	TaskPtr& operator++();
	TaskPtr& operator--();

	bool operator==(TaskPtr other) const { return Index == other.Index; }
	bool operator!=(TaskPtr other) const { return Index != other.Index; }

	static const TaskPtr Invalid;
};