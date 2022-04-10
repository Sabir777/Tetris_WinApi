#include "Options.h"

Timer::Timer() {
	time_prev = std::chrono::steady_clock::now();
}

void Timer::set_time_point() {
	time_prev = std::chrono::steady_clock::now();
}

int Timer::duration() {
	auto time_real = std::chrono::steady_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(time_real - time_prev);
	return duration.count();
}