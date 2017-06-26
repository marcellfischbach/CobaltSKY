#pragma once

#include <cobalt/csexport.hh>
#include <cobalt/core/csclass.hh>
#include <cobalt/core/csevent.refl.hh>

CS_INTERFACE()
class CSE_API csEvent : public CS_SUPER(csObject)
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
