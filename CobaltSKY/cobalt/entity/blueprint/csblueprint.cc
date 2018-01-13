#include <cobalt/entity/blueprint/csblueprint.hh>
#include <cobalt/entity/blueprint/csbpentity.hh>
#include <cobalt/entity/blueprint/csbpentitystate.hh>

csBlueprint::csBlueprint()
  : iObject ()
  , m_entity(0)
  , m_entityState(0)
{
  CS_CLASS_GEN_CONSTR;
}

csBlueprint::~csBlueprint()
{
  if (m_entity)
  {
    delete m_entity;
  }
  if (m_entityState)
  {
    delete m_entityState;
  }
}

csEntity *csBlueprint::CreateEntity() const
{
  if (!m_entity)
  {
    return 0;
  }

  return m_entity->CreateEntity();
}


csEntityState *csBlueprint::CreateEntityState() const
{
  if (!m_entityState)
  {
    return 0;
  }

  return m_entityState->CreateEntityState();
}


void csBlueprint::SetEntity(csBPEntity *entity)
{
  m_entity = entity;
}

void csBlueprint::SetEntityState(csBPEntityState *entityState)
{
  m_entityState = entityState;
}

