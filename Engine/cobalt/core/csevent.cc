
#include <cobalt/core/csevent.hh>

static long s_event_id = 0;

csEvent::csEvent()
  : csObject()
  , m_id(s_event_id++)
  , m_accepted(false)
{
}

csEvent::~csEvent()
{

}



unsigned long csEvent::GetId() const
{
  return m_id;
}


void csEvent::Accept()
{
  m_accepted = true;
}

bool csEvent::IsAccepted() const
{
  return m_accepted;
}
