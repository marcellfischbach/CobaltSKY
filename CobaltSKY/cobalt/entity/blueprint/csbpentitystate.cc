
#include <cobalt/entity/blueprint/csbpentitystate.hh>
#include <cobalt/entity/csentitystate.hh>


csBPEntityState::csBPEntityState()
  : m_entityStateClass(0)
{
}


void csBPEntityState::SetEntityStateClass(const csClass *entityStateClass)
{
  m_entityStateClass = entityStateClass;
}

csEntityState *csBPEntityState::CreateEntityState() const
{
  if (!m_entityStateClass)
  {
    return 0;
  }

  return m_entityStateClass->CreateInstance<csEntityState>();
}

