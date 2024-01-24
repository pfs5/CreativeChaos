#pragma once

#include "core/manager.h"
#include "util/proxy.h"

class DebugManager : public Manager
{
public:
	DebugManager() { _debugEntries.reserve(128); }

	void Update() override { ClearDebug(); }

	void PushDebug(const char* text) { _debugEntries.emplace_back(text); }
	void ClearDebug() { _debugEntries.clear(); }

	const std::vector<std::string>& GetDebugEntries() const { return _debugEntries; }

private:
	std::vector<std::string> _debugEntries;
};

struct DebugManagerProxy : public Proxy<DebugManager>
{
	DebugManagerProxy();
};