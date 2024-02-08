#pragma once

namespace App
{
	static constexpr const char* APP_TITLE = "Creative Chaos";
	static constexpr uint32_t MAJOR_VERSION = 0;
	static constexpr uint32_t MINOR_VERSION = 1;

	inline std::string GetVersionString() { return std::format("v{}.{}", MAJOR_VERSION, MINOR_VERSION); }
	inline std::string GetAppString() { return std::format("{} {}", APP_TITLE, GetVersionString()); }

}