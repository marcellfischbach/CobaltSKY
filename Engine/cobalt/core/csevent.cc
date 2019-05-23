
#include <cobalt/core/csevent.hh>

static long s_event_id = 0;

cs::Event::Event()
  : cs::Object()
  , m_id(s_event_id++)
  , m_accepted(false)
{
}

cs::Event::~Event()
{

}



unsigned long cs::Event::GetId() const
{
  return m_id;
}


void cs::Event::Accept()
{
  m_accepted = true;
}

bool cs::Event::IsAccepted() const
{
  return m_accepted;
}
