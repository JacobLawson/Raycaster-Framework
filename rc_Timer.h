#include <stdafx.h>
#pragma once

#ifndef __TIMER_H__
#define __TIMER_H__

class Timer
{
public:
	//Utilites for timing; Get() updates timers and returns time between last get call
	static void ResetTimer();
	static float TickTimer();
	static float GetDeltaTime();
	static float GetTotalTime();
};
#endif
