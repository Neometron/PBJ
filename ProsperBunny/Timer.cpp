#include "Timer.h"
using namespace HBS;

void Timer::startCounter()
{
	LARGE_INTEGER li;
	if (!QueryPerformanceFrequency(&li)) {}
		//throw

	pcFreq = double(li.QuadPart);

	QueryPerformanceCounter(&li);
	counterStart = li.QuadPart;
}

double Timer::getCounter()
{
	LARGE_INTEGER li;
	QueryPerformanceCounter(&li);
	return double(li.QuadPart - counterStart) / pcFreq;
}