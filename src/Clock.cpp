#include "Clock.h"
#include <chrono>

void Clock::clockStart() {
	startTime = std::chrono::high_resolution_clock::now();
}

void Clock::clockEnd() {
	endTime = std::chrono::high_resolution_clock::now();
	std::chrono::duration< double, std::milli > totalTime = endTime - startTime;
	read = totalTime.count();
}

