#pragma once

class PerformanceTimer
{
	using Clock = std::chrono::high_resolution_clock;

public:
	PerformanceTimer()
	{
		Start();
	}

	void Start() { _start = Clock::now(); }
	float GetElapsedMs() const
	{
		std::chrono::microseconds durationMicro = duration_cast<std::chrono::microseconds>(Clock::now() - _start);
		return durationMicro.count() / 1000.f;
	}

private:
	std::chrono::time_point<Clock> _start;
};