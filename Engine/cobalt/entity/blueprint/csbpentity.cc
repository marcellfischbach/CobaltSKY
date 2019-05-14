#include <cobalt/entity/blueprint/csbpentity.hh>
#include <cobalt/entity/blueprint/csbpentitystate.hh>
#include <cobalt/entity/csentity.hh>
#include <cobalt/entity/csspatialstate.hh>
#include <cobalt/core/property/cspropertysetter.hh>

csBPEntity::csBPEntity()
  : m_entityClass(0)
{
}

csBPEntity::~csBPEntity()
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

void csBPEntity::SetEntityClass(const cs::Class *entityClass)
{
  m_entityClass = entityClass;
}

const cs::Class *csBPEntity::GetEntityClass() const
{
  return m_entityClass;
}

void csBPEntity::AddProperty(csPropertySetter *property)
{
  CS_ADDREF(property);
  m_properties.push_back(property);
}

void csBPEntity::AddEntityState(csBPEntityState *entityState)
{
  m_entityStates.push_back(entityState);
}

csBPEntityState* csBPEntity::GetEntityStateByID(unsigned id) const
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

csEntity *csBPEntity::CreateEntity() const
{
  if (!m_entityClass)
  {
    return 0;
  }
  csEntity *entity = m_entityClass->CreateInstance<csEntity>();
  if (!entity)
  {
    return 0;
  }

  for (csPropertySetter *prop : m_properties)
  {
    prop->Apply(entity);
  }


  std::map<unsigned, csEntityState*> entityStates;
  for (csBPEntityState *stat : m_entityStates)
  {
    csEntityState *state = stat->CreateEntityState();
    if (state && stat->GetId())
    {
      entityStates[stat->GetId()] = state;
    }

    if (stat->GetParentId())
    {
      csEntityState *parentState = entityStates[stat->GetParentId()];
      csSpatialState *spatialState = cs::QueryClass<csSpatialState>(state);
      csSpatialState *spatialParentState = cs::QueryClass<csSpatialState>(parentState);
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
          csSpatialState *spatialState = cs::QueryClass<csSpatialState>(state);
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

