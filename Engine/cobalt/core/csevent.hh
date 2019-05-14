#pragma once

#include <cobalt/csexport.hh>
#include <csrefl/class.hh>
#include <cobalt/core/csevent.refl.hh>

CS_CLASS()
class CSE_API csEvent : public CS_SUPER(cs::Object)
{
  CS_CLASS_GEN;
public:
  virtual ~csEvent();

  unsigned long GetId() const;

  void Accept();
  bool IsAccepted() const;

protected:
  csEvent();

private:
  unsigned long m_id;

  bool m_accepted;
};
