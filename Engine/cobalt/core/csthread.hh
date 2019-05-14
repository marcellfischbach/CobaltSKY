#pragma once

#include <cobalt/csexport.hh>
#include <csrefl/class.hh>

#include <cobalt/core/csthread.refl.hh>

struct csThreadPriv;
CS_CLASS();
class CSE_API csThread : public CS_SUPER(cs::Object)
{
  CS_CLASS_GEN;

public:
  csThread();
  virtual ~csThread();

  void Start();
  void Stop();
  void WaitForFinished() const;
  bool IsFinished() const;

  virtual void Run() = 0;

protected:
  bool m_running;

  csThreadPriv *m_priv;
};
