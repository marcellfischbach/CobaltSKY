#pragma once

#include <cobalt/csexport.hh>
#include <cobalt/core/csclass.hh>

#include <cobalt/core/csthread.refl.hh>

struct csThreadPriv;
CS_INTERFACE();
class CSE_API csThread : public CS_SUPER(csObject)
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
