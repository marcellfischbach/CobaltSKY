#pragma once

#include <cobalt/csexport.hh>
#include <cobalt/cstypes.hh>
#include <cobalt/csdefs.hh>

namespace cs
{

class CSE_API Time
{
public:
  static cs::Time& Get();
  void Tick();


  csUInt64 GetRunTimeMilli();
  csUInt64 GetCurrentTimeMilli();

  csUInt64 GetRunTimeMicro();
  csUInt64 GetCurrentTimeMicro();
private:
  Time();

  csUInt64 m_startTimeMicro;
  csUInt64 m_currentTimeMicro;
  csUInt64 m_runTimeMicro;

  csUInt64 m_startTimeMilli;
  csUInt64 m_currentTimeMilli;
  csUInt64 m_runTimeMilli;

};

}

CS_FORCEINLINE csUInt64 cs::Time::GetRunTimeMilli()
{
  return m_currentTimeMilli;
}

CS_FORCEINLINE csUInt64 cs::Time::GetCurrentTimeMilli()
{
  return m_runTimeMilli;
}

CS_FORCEINLINE csUInt64 cs::Time::GetRunTimeMicro()
{
  return m_currentTimeMicro;
}

CS_FORCEINLINE csUInt64 cs::Time::GetCurrentTimeMicro()
{
  return m_runTimeMicro;
}
