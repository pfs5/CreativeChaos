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

	void PushDebug(const char* text) { _debugEntries.emplace_back(text); }
	void ClearDebug() { _debugEntries.clear(); }

	void PushLog(const char* text) { _logEntries.emplace_back(text); }
	void ClearLog() { _logEntries.clear(); }

	const std::vector<std::string>& GetDebugEntries() const { return _debugEntries; }
	const std::vector<LogEntry>& GetLogEntries() const { return _logEntries; }

private:
	std::vector<std::string> _debugEntries;
	std::vector<LogEntry> _logEntries;
};

struct DebugManagerProxy : public Proxy<DebugManager>
{
	DebugManagerProxy();
};