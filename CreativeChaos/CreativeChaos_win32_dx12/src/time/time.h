#pragma once
#include "util/tostring.h"

class TimeSpan
{
	friend class Timestamp;

public:
	TimeSpan() = default;

	std::string ToString() const
	{
		uint64_t totalSeconds = _duration.count();

		// ptodo - fix these conversions
		uint64_t seconds = totalSeconds % 60;
		uint64_t minutes = (totalSeconds / 60) % 60;
		uint64_t hours = (totalSeconds / 60 / 60) % 24;
		uint64_t days = (totalSeconds / 60 / 60 / 24);

		return std::format("{:02}days {:02}hours {:02}mins {:02}secs", days, hours, minutes, seconds);
	}

	// Return only the days/hours/part part.
	TimeSpan GetDays() const { return TimeSpan{ std::chrono::seconds{_duration.count() / 60 / 60 / 24 * 60 * 60 * 24} }; }
	TimeSpan GetHours() const { return TimeSpan{ std::chrono::seconds{_duration.count() / 60 / 60 * 60 * 60} }; }
	TimeSpan GetMinutes() const { return TimeSpan{ std::chrono::seconds{_duration.count() / 60 * 60}}; }
	TimeSpan GetSeconds() const { return TimeSpan{ std::chrono::seconds{_duration.count() % 60}}; }

	uint64_t GetTotalSeconds() const { return _duration.count(); }

	TimeSpan& operator+= (const TimeSpan& other)
	{
		_duration += other._duration;
		return *this;
	}
	TimeSpan& operator-= (const TimeSpan& other)
	{
		_duration -= other._duration;
		return *this;
	}
	
	friend TimeSpan operator+(const TimeSpan& lhs, const TimeSpan& rhs) { return TimeSpan{lhs._duration + rhs._duration}; }
	friend TimeSpan operator-(const TimeSpan& lhs, const TimeSpan& rhs) { return TimeSpan{lhs._duration - rhs._duration}; }

private:
	std::chrono::seconds _duration {0};	// seconds are enough for our purposes

private:
	TimeSpan(const std::chrono::seconds& seconds) :
		_duration{ seconds }
	{

	}
};

class Timestamp
{
	using Clock = std::chrono::system_clock;

public:
	enum class EDifferenceType : uint8_t
	{
		All,
		OnlyWorkingHours
	};

public:
	Timestamp() = default;

	static Timestamp Now()
	{
		return Timestamp{ Clock::now() };
	}

	static Timestamp FromUnix(uint64_t unixSeconds)
	{
		return Timestamp{ std::chrono::seconds{ unixSeconds } };
	}

	uint64_t ToUnix() const { return (*this - Timestamp{std::chrono::seconds{0}}).GetTotalSeconds(); }

	// Difference
	static TimeSpan Difference(const Timestamp& end, const Timestamp& start, EDifferenceType calculationType = EDifferenceType::All);

	inline friend TimeSpan operator-(const Timestamp& end, const Timestamp& start)
	{
		return Timestamp::Difference(end, start);
	}

	Timestamp& operator+= (const TimeSpan span) { _timestamp += span._duration; return *this; }
	Timestamp& operator-= (const TimeSpan span) { _timestamp -= span._duration; return *this; }
	
	inline friend Timestamp operator+ (const Timestamp& time, const TimeSpan& duration)
	{
		Timestamp ret {time};
		ret += duration;
		return ret;
	}
	inline friend Timestamp operator- (const Timestamp& time, const TimeSpan& duration)
	{
		Timestamp ret{ time };
		ret -= duration;
		return ret;
	}
	inline friend bool operator< (const Timestamp& lhs, const Timestamp& rhs) { return lhs._timestamp < rhs._timestamp; }
	inline friend bool operator<= (const Timestamp& lhs, const Timestamp& rhs) { return lhs._timestamp <= rhs._timestamp; }
	inline friend bool operator> (const Timestamp& lhs, const Timestamp& rhs) { return lhs._timestamp > rhs._timestamp; }
	inline friend bool operator>= (const Timestamp& lhs, const Timestamp& rhs) { return lhs._timestamp >= rhs._timestamp; }

	std::string ToString() const
	{
		const auto zonedTime { std::chrono::zoned_time { std::chrono::current_zone(), _timestamp}};
		return std::format("{:%Y-%m-%d %EX}", zonedTime);
	}

private:
	constexpr Timestamp(std::chrono::seconds seconds) :
		_timestamp{ seconds }
	{

	}

	constexpr Timestamp(std::chrono::time_point<Clock> timestamp) :
		_timestamp{ timestamp }
	{

	}

private:
	std::chrono::time_point<Clock> _timestamp;
};

struct TimePeriod
{
	Timestamp Start;
	Timestamp End;

public:
	TimePeriod() = default;
	TimePeriod(const Timestamp& start, const Timestamp& end) :
		Start{ start },
		End{ end }
	{

	}

	TimeSpan GetDuration() const { return End - Start; }

	std::string ToString() const { return std::format("Start: {}, End: {}", Start.ToString(), End.ToString()); }

	static bool Overlap(const TimePeriod& lhs, const TimePeriod& rhs, TimePeriod& outOverlap)
	{
		if (lhs.Start > rhs.End || lhs.End < rhs.Start)
		{
			// No overlap
			return false;
		}

		outOverlap.Start = std::max<Timestamp>(lhs.Start, rhs.Start);
		outOverlap.End = std::min<Timestamp>(lhs.End, rhs.End);

		return true;
	}
};