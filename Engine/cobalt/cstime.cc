
#include <cobalt/cstime.hh>

#ifdef CS_WIN32
#include <Windows.h>
LARGE_INTEGER FrequencyCounter;
#endif

#define MICRO 1000000
#define MILLI 1000


csTime::csTime()
{
#ifdef CS_WIN32
  LARGE_INTEGER StartTime;
  QueryPerformanceFrequency(&FrequencyCounter);
  QueryPerformanceCounter(&StartTime); 
  m_startTimeMicro = StartTime.QuadPart * MICRO / FrequencyCounter.QuadPart;
  m_startTimeMilli = StartTime.QuadPart * MILLI / FrequencyCounter.QuadPart;
#endif
  Tick();
}

csTime &csTime::Get()
{
  static csTime time;
  return time;
}


void csTime::Tick()
{
#ifdef CS_WIN32
  LARGE_INTEGER CurrentTime;
  QueryPerformanceCounter(&CurrentTime);
  m_currentTimeMicro = CurrentTime.QuadPart * MICRO / FrequencyCounter.QuadPart;
  m_currentTimeMilli = CurrentTime.QuadPart * MILLI / FrequencyCounter.QuadPart;
#endif

  m_runTimeMicro = m_currentTimeMicro - m_startTimeMicro;
  m_runTimeMilli = m_currentTimeMilli - m_startTimeMilli;
}
