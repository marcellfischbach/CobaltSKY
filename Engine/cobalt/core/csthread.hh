#pragma once

#include <cobalt/csexport.hh>
#include <csrefl/class.hh>

#include <cobalt/core/csthread.refl.hh>

namespace cs
{


struct ThreadPriv;
CS_CLASS();
class CSE_API Thread : public CS_SUPER(cs::Object)
{
  CS_CLASS_GEN;

public:
  Thread();
  virtual ~Thread();

  void Start();
  void Stop();
  void WaitForFinished() const;
  bool IsFinished() const;

  virtual void Run() = 0;

protected:
  bool m_running;

  cs::ThreadPriv* m_priv;
};

}

