#pragma once

enum class ETaskCategory : uint8_t
{
	Main,
	Backlog,
	Trash
};

class Task
{
public:
	Task(const char* name, ETaskCategory category, uint32_t id) :
		_name{ name },
		_category{ category },
		_id{ id }
	{

	}
	
	uint32_t GetID() const { return _id; }

	const char* GetName() const { return _name.c_str(); }
	void SetName(const char* value) { _name = value; }

	ETaskCategory GetCategory() const { return _category; }
	void SetCategory(ETaskCategory value) { _category = value; }

	bool IsActive() const { return _active; }
	void SetActive(bool value) { _active = value; }

private:
	uint32_t _id;
	std::string _name;
	ETaskCategory _category;
	bool _active = false;
};