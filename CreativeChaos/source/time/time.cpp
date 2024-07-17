#include "pch.h"
#include "time/time.h"



TimeSpan Timestamp::Difference(const Timestamp& end, const Timestamp& start, EDifferenceType calculationType /*= EDifferenceType::All*/)
{
	if (calculationType == EDifferenceType::All)
	{
		return TimeSpan{ std::chrono::duration_cast<std::chrono::seconds>(end._timestamp - start._timestamp) };
	}
	else if (calculationType == EDifferenceType::OnlyWorkingHours)
	{
		// ptodo FromUnix doesn't work with time zones
		static const Timestamp DAY_START = Timestamp::FromUnix(28800);		// 09:00:00
		static const Timestamp DAY_END = Timestamp::FromUnix(61200);		// 18:00:00
		static const Timestamp BREAK_START = Timestamp::FromUnix(41400);	// 12:30:00
		static const Timestamp BREAK_END = Timestamp::FromUnix(46800);		// 14:00:00

		static const TimePeriod MORNING{ DAY_START, BREAK_START };
		static const TimePeriod AFTERNOON{ BREAK_END, DAY_END };

		const TimeSpan totalTime{ std::chrono::duration_cast<std::chrono::seconds>(end._timestamp - start._timestamp) };
		const TimeSpan totalDays = totalTime.GetDays();
		const Timestamp startNoDays = start - (start - Timestamp{}).GetDays();
		const Timestamp endNoDays = end - (end - Timestamp{}).GetDays();

		TimeSpan durationLastDay;

		if (endNoDays >= startNoDays)
		{
			const TimePeriod totalPeriod{ startNoDays, endNoDays };

			TimePeriod overlap;
			if (TimePeriod::Overlap(totalPeriod, MORNING, overlap))
			{
				durationLastDay += overlap.GetDuration();
			}
			if (TimePeriod::Overlap(totalPeriod, AFTERNOON, overlap))
			{
				durationLastDay += overlap.GetDuration();
			}
		}
		else
		{
			// The start and end happen on different days so we split it into [00:00 - end] and [start - 23:59:59]
			const TimePeriod totalPeriodFirstPart { Timestamp::FromUnix(0) , endNoDays};
			const TimePeriod totalPeriodSecondPart { startNoDays , Timestamp::FromUnix(82799)};	// 23:59:59

			TimePeriod overlap;
			if (TimePeriod::Overlap(totalPeriodFirstPart, MORNING, overlap))
			{
				durationLastDay += overlap.GetDuration();
			}
			if (TimePeriod::Overlap(totalPeriodSecondPart, MORNING, overlap))
			{
				durationLastDay += overlap.GetDuration();
			}
			if (TimePeriod::Overlap(totalPeriodFirstPart, AFTERNOON, overlap))
			{
				durationLastDay += overlap.GetDuration();
			}
			if (TimePeriod::Overlap(totalPeriodSecondPart, AFTERNOON, overlap))
			{
				durationLastDay += overlap.GetDuration();
			}
		}

		return totalDays + durationLastDay;
	}

	ensureMsg(false, "Unhandled case");

	return TimeSpan{};
}
