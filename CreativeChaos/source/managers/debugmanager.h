#pragma once

#include "core/manager.h"
#include "util/proxy.h"
#include "time/time.h"

class DebugManager : public Manager
{
public:
	struct LogEntry
	{
		std::string Text;
		Timestamp Time;

		LogEntry(const char* text) :
			Text{ text },
			Time{ Timestamp::Now() }
		{

		}
	};

public:
	DebugManager() { _debugEntries.reserve(128); }

	void Update() override { ClearDebug(); }

	void PushDebug(const char* text) { _debugEntries.emplace_back(text); MarkDebugDirty(); }
	void ClearDebug() { _debugEntries.clear(); MarkDebugDirty(); }

	void PushLog(const char* text) { _logEntries.emplace_back(text); MarkLogDirty(); }
	void ClearLog() { _logEntries.clear(); MarkLogDirty(); }

	const std::vector<std::string>& GetDebugEntries() const { return _debugEntries; }
	const std::vector<LogEntry>& GetLogEntries() const { return _logEntries; }

	bool GetDebugDirty() const { return _debugDirty; }
	bool GetLogDirty() const { return _logDirty; }

	void ClearDebugDirty() { _debugDirty = false; }
	void ClearLogDirty() { _logDirty = false; }

private:
	void MarkDebugDirty() { _debugDirty = true; }
	void MarkLogDirty() { _logDirty = true; }

private:
	std::vector<std::string> _debugEntries;
	std::vector<LogEntry> _logEntries;

	bool _debugDirty = true;
	bool _logDirty = true;
};

struct DebugManagerProxy : public Proxy<DebugManager>
{
	DebugManagerProxy();
};