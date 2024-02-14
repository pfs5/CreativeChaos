#pragma once

void DebugLog(const char* text);

template<typename... Args>
void DebugLogFormat(const std::format_string<Args...> format, Args&&... args)
{
	DebugLog(std::vformat(format.get(), std::make_format_args(args...)).c_str());
}