#pragma once

#include <Valkyrie/Export.hh>
#include <Valkyrie/Core/Object.hh>

#include <Valkyrie/Core/Thread.refl.hh>

struct vkThreadPriv;
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

  vkThreadPriv *m_priv;
};