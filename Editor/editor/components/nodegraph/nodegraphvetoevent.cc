
#include <editor/components/nodegraph/nodegraphvetoevent.hh>


NodeGraphVetoEvent::NodeGraphVetoEvent()
  : m_veto(false)
{


}

void NodeGraphVetoEvent::Veto()
{
  m_veto = true;
}

bool NodeGraphVetoEvent::IsVeto() const
{
  return m_veto;
}

