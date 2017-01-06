
#include <valkyrie/vktime.hh>

#ifdef VK_WIN32
#include <Windows.h>
LARGE_INTEGER FrequencyCounter;
#endif

#define MICRO 1000000
#define MILLI 1000


vkTime::vkTime()
{
#ifdef VK_WIN32
  LARGE_INTEGER StartTime;
  QueryPerformanceFrequency(&FrequencyCounter);
  QueryPerformanceCounter(&StartTime); 
  m_startTimeMicro = StartTime.QuadPart * MICRO / FrequencyCounter.QuadPart;
  m_startTimeMilli = StartTime.QuadPart * MILLI / FrequencyCounter.QuadPart;
#endif
  Tick();
}

vkTime &vkTime::Get()
{
  static vkTime time;
  return time;
}


void vkTime::Tick()
{
#ifdef VK_WIN32
  LARGE_INTEGER CurrentTime;
  QueryPerformanceCounter(&CurrentTime);
  m_currentTimeMicro = CurrentTime.QuadPart * MICRO / FrequencyCounter.QuadPart;
  m_currentTimeMilli = CurrentTime.QuadPart * MILLI / FrequencyCounter.QuadPart;
#endif

  m_runTimeMicro = m_currentTimeMicro - m_startTimeMicro;
  m_runTimeMilli = m_currentTimeMilli - m_startTimeMilli;
}