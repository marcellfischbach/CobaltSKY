#pragma once

#include <Valkyrie/Export.hh>
#include <Valkyrie/Core/Object.hh>
#include <Valkyrie/Core/Thread.refl.hh>

#ifdef VK_WIN32
#include <Windows.h>
#endif


VK_INTERFACE();
class VKE_API vkThread : public vkObject
{
  VK_CLASS_GEN;

public:
  vkThread();
  virtual ~vkThread();

  void Start();
  void Stop();
  void WaitForFinished() const;
  bool IsFinished() const;

  virtual void Run() = 0;

protected:
  bool m_running;

#ifdef VK_WIN32
  DWORD m_threadID;
  HANDLE m_threadHandle;
#endif
};