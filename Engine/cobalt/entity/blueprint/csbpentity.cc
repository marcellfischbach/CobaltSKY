#include <cobalt/entity/blueprint/csbpentity.hh>
#include <cobalt/entity/blueprint/csbpentitystate.hh>
#include <cobalt/entity/csentity.hh>
#include <cobalt/entity/csspatialstate.hh>
#include <cobalt/core/property/cspropertysetter.hh>

cs::BPEntity::BPEntity()
  : m_entityClass(0)
{
}

cs::BPEntity::~BPEntity()
{
  for (auto prop : m_properties)
  {
    CS_RELEASE(prop);
  }
  m_properties.clear();

  for (auto state : m_entityStates)
  {
    delete state;
  }
  m_entityStates.clear();
}

void cs::BPEntity::SetEntityClass(const cs::Class *entityClass)
{
  m_entityClass = entityClass;
}

const cs::Class *cs::BPEntity::GetEntityClass() const
{
  return m_entityClass;
}

void cs::BPEntity::AddProperty(cs::PropertySetter *property)
{
  CS_ADDREF(property);
  m_properties.push_back(property);
}

void cs::BPEntity::AddEntityState(cs::BPEntityState *entityState)
{
  m_entityStates.push_back(entityState);
}

cs::BPEntityState* cs::BPEntity::GetEntityStateByID(unsigned id) const
{
  for (auto state : m_entityStates)
  {
    if (state->GetId() == id)
    {
      return state;
    }
  }
  return 0;
}

cs::Entity *cs::BPEntity::CreateEntity() const
{
  if (!m_entityClass)
  {
    return 0;
  }
  cs::Entity *entity = m_entityClass->CreateInstance<cs::Entity>();
  if (!entity)
  {
    return 0;
  }

  for (cs::PropertySetter *prop : m_properties)
  {
    prop->Apply(entity);
  }


  std::map<unsigned, cs::EntityState*> entityStates;
  for (cs::BPEntityState *stat : m_entityStates)
  {
    cs::EntityState *state = stat->CreateEntityState();
    if (state && stat->GetId())
    {
      entityStates[stat->GetId()] = state;
    }

    if (stat->GetParentId())
    {
      cs::EntityState *parentState = entityStates[stat->GetParentId()];
      cs::SpatialState *spatialState = cs::QueryClass<cs::SpatialState>(state);
      cs::SpatialState *spatialParentState = cs::QueryClass<cs::SpatialState>(parentState);
      if (spatialState && spatialParentState)
      {
        entity->AddState(spatialState, spatialParentState);
      }
      else
      {
        printf("Error attaching state [%p] to parentState [%p]\n", spatialState, spatialParentState);
      }
    }
    else
    {
      if (stat->IsRoot())
      {
        if (entity->GetRootState())
        {
          printf("Entity root state is already set.\n");
        }
        else
        {
          cs::SpatialState *spatialState = cs::QueryClass<cs::SpatialState>(state);
          if (spatialState)
          {
            entity->SetRootState(spatialState);
          }
          else
          {
            printf("Try to set nonspatial root state.");
          }
        }

      }
      entity->AddState(state);
    }
  }

  return entity;
}

