#ifndef Timer_H
#define Timer_H

#include <windows.h>

namespace byhj
{

namespace d3d
{


class Timer
{
public:
	Timer();

	float GetTotalTime() const;    //In seconds
	float GetDeltaTime() const; //In seconds

	void Reset();      //Call before message loop;
	void Start();      //Call when unpaused;
	void Stop();       //Call when paused;
	void Count();      //Call every frame;

private:
	double m_SecondsPerCount;
	double m_DeltaTime;

	__int64 m_BaseTime;
	__int64 m_PausedTime;
	__int64 m_StopTime;
	__int64 m_PrevTime;
	__int64 m_CurrTime;

	bool m_Stopped;

};

}

}

#endif