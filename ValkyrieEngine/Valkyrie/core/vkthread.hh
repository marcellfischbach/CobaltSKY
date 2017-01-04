#pragma once

#include <Valkyrie/vkexport.hh>
#include <Valkyrie/core/vkclass.hh>

#include <Valkyrie/core/vkthread.refl.hh>

struct vkThreadPriv;
VK_INTERFACE();
class VKE_API vkThread : public VK_SUPER(vkObject)
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
