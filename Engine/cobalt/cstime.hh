#pragma once

#include <cobalt/csexport.hh>
#include <cobalt/cstypes.hh>
#include <cobalt/csdefs.hh>


class CSE_API csTime
{
public:
  static csTime &Get();
  void Tick();


  csUInt64 GetRunTimeMilli();
  csUInt64 GetCurrentTimeMilli();

  csUInt64 GetRunTimeMicro();
  csUInt64 GetCurrentTimeMicro();
private:
  csTime();

  csUInt64 m_startTimeMicro;
  csUInt64 m_currentTimeMicro;
  csUInt64 m_runTimeMicro;

  csUInt64 m_startTimeMilli;
  csUInt64 m_currentTimeMilli;
  csUInt64 m_runTimeMilli;

};


CS_FORCEINLINE csUInt64 csTime::GetRunTimeMilli()
{
  return m_currentTimeMilli;
}

CS_FORCEINLINE csUInt64 csTime::GetCurrentTimeMilli()
{
  return m_runTimeMilli;
}

CS_FORCEINLINE csUInt64 csTime::GetRunTimeMicro()
{
  return m_currentTimeMicro;
}

CS_FORCEINLINE csUInt64 csTime::GetCurrentTimeMicro()
{
  return m_runTimeMicro;
}
