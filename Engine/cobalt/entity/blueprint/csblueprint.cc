#include <cobalt/entity/blueprint/csblueprint.hh>
#include <cobalt/entity/blueprint/csbpentity.hh>
#include <cobalt/entity/blueprint/csbpentitystate.hh>
#include <cobalt/entity/csentity.hh>
#include <cobalt/entity/csentitystate.hh>

cs::Blueprint::Blueprint()
  : cs::iObject ()
  , m_entity(0)
  , m_entityState(0)
{
  CS_CLASS_GEN_CONSTR;
}

cs::Blueprint::~Blueprint()
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

cs::EntityWrapper *cs::Blueprint::CreateEntity() const
{
  if (!m_entity)
  {
    return nullptr;
  }

  return new cs::EntityWrapper (m_entity->CreateEntity());
}


cs::EntityStateWrapper *cs::Blueprint::CreateEntityState() const
{
  if (!m_entityState)
  {
    return nullptr;
  }

  return new cs::EntityStateWrapper(m_entityState->CreateEntityState());
}


void cs::Blueprint::SetEntity(cs::BPEntity *entity)
{
  m_entity = entity;
}

void cs::Blueprint::SetEntityState(cs::BPEntityState *entityState)
{
  m_entityState = entityState;
}

