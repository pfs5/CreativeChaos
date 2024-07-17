#pragma once

namespace newtime
{
	//static constexpr uint64_t YEAR_TO_SEC;
	//static constexpr uint64_t MONTH_TO_SEC;
	//static constexpr uint64_t DAY_TO_SEC = ;
	//static constexpr uint64_t HOUR_TO_SEC = 24 * 60;
	//static constexpr uint64_t MINUTE_TO_SEC = 60;

	class TimePoint
	{
	public:
		uint64_t TruncSeconds() const { return _unixSeconds % 60; }
		uint64_t TruncMinutes() const { return _unixSeconds / 60; }

	private:
		uint64_t _unixSeconds;
	};
}