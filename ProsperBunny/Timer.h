#pragma once
#ifdef WIN32
#include <Windows.h>
#endif

namespace HBS
{
	class Timer
	{
	public:
		Timer()  {};
		~Timer() {};
	public:
		void   startCounter();
		double getCounter();
	private:
		double   pcFreq = 0.0;
		__int64  counterStart = 0;
		double   counterDifference = 0;
	};
};