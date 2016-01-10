#pragma once

#include <Valkyrie/Export.hh>
#include <Valkyrie/Types.hh>
#include <Valkyrie/Defs.hh>


class VKE_API vkTime
{
public:
  static vkTime &Get();
  void Tick();


  vkUInt64 GetRunTimeMilli();
  vkUInt64 GetCurrentTimeMilli();

  vkUInt64 GetRunTimeMicro();
  vkUInt64 GetCurrentTimeMicro();
private:
  vkTime();

  vkUInt64 m_startTimeMicro;
  vkUInt64 m_currentTimeMicro;
  vkUInt64 m_runTimeMicro;

  vkUInt64 m_startTimeMilli;
  vkUInt64 m_currentTimeMilli;
  vkUInt64 m_runTimeMilli;

};


VK_FORCEINLINE vkUInt64 vkTime::GetRunTimeMilli()
{
  return m_currentTimeMilli;
}

VK_FORCEINLINE vkUInt64 vkTime::GetCurrentTimeMilli()
{
  return m_runTimeMilli;
}

VK_FORCEINLINE vkUInt64 vkTime::GetRunTimeMicro()
{
  return m_currentTimeMicro;
}

VK_FORCEINLINE vkUInt64 vkTime::GetCurrentTimeMicro()
{
  return m_runTimeMicro;
}


