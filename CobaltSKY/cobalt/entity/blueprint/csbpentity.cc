#include <cobalt/entity/blueprint/csbpentity.hh>
#include <cobalt/entity/blueprint/csbpentitystate.hh>
#include <cobalt/entity/blueprint/csbpproperty.hh>
#include <cobalt/entity/csentity.hh>
#include <cobalt/entity/csspatialstate.hh>

csBPEntity::csBPEntity()
  : m_entityClass(0)
{
}

csBPEntity::~csBPEntity()
{
  for (auto prop : m_properties)
  {
    delete prop;
  }
  m_properties.clear();

  for (auto state : m_entityStates)
  {
    delete state;
  }
  m_entityStates.clear();
}

void csBPEntity::SetEntityClass(const csClass *entityClass)
{
  m_entityClass = entityClass;
}

const csClass *csBPEntity::GetEntityClass() const
{
  return m_entityClass;
}

void csBPEntity::AddProperty(csBPBaseProperty *property)
{
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

  for (csBPBaseProperty *prop : m_properties)
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
      csSpatialState *spatialState = csQueryClass<csSpatialState>(state);
      csSpatialState *spatialParentState = csQueryClass<csSpatialState>(parentState);
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
          csSpatialState *spatialState = csQueryClass<csSpatialState>(state);
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

