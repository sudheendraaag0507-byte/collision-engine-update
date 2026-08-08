#pragma once
#include <chrono>
class Clock{

public :

	std::chrono::high_resolution_clock::time_point endTime ;
	std::chrono::high_resolution_clock::time_point startTime ;

	double read  ;

	void clockStart() ;

	void clockEnd() ;

	
};

