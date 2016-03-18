#ifndef Timer_H
#define Timer_H

#include <windows.h>

namespace byhj
{

class Timer
{
public:
	Timer();

	float getTotalTime() const;    //In seconds
	float getDeltaTime() const; //In seconds

	void reset();      //Call before message loop;
	void start();      //Call when unpaused;
	void stop();       //Call when paused;
	void count();      //Call every frame;

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

#endif