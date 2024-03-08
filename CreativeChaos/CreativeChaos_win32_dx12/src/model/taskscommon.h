#pragma once

struct Task;

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