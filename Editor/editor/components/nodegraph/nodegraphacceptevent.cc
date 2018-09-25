
#include <editor/nodegraph/nodegraphacceptevent.hh>


NodeGraphAcceptEvent::NodeGraphAcceptEvent()
  : m_accepted(false)
{


}

void NodeGraphAcceptEvent::Accept()
{
  m_accepted = true;
}

bool NodeGraphAcceptEvent::IsAccepted() const
{
  return m_accepted;
}

