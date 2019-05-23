#pragma once

#include <cobalt/csexport.hh>
#include <csrefl/class.hh>
#include <cobalt/core/csevent.refl.hh>

namespace cs
{

CS_CLASS()
class CSE_API Event : public CS_SUPER(cs::Object)
{
  CS_CLASS_GEN;
public:
  virtual ~Event();

  unsigned long GetId() const;

  void Accept();
  bool IsAccepted() const;

protected:
  Event();

private:
  unsigned long m_id;

  bool m_accepted;
};

}

