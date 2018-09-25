
#include <cobalt/loaders/csentitycsfloader.hh>
#include <cobalt/csengine.hh>
#include <cobalt/entity/blueprint/csblueprint.hh>
#include <cobalt/entity/csentity.hh>
#include <cobalt/entity/csspatialstate.hh>
#include <cobalt/core/csclassregistry.hh>

csEntityCSFLoader::csEntityCSFLoader()
  : csBaseCSFLoader()
{

}

csEntityCSFLoader::~csEntityCSFLoader()
{

}

bool csEntityCSFLoader::CanLoad(const csfEntry *entry, const csResourceLocator &locator, iObject *userData) const
{
  return entry->GetTagName() == std::string("entity");
}

const csClass *csEntityCSFLoader::EvalClass(const csfEntry *entry, const csResourceLocator &locator, iObject *userData) const
{
  return csEntity::GetStaticClass();
}

iObject *csEntityCSFLoader::Load(const csfEntry *entry, const csResourceLocator &locator, iObject *userData) const
{
  csEntity *entity = 0;
  if (entry->HasAttribute("locator"))
  { 
    iObject *obj = csEng->Get(csResourceLocator(entry->GetAttribute("locator")));
    if (!obj)
    {
      return 0;
    }
    entity = csQueryClass<csEntity>(obj);
    if (!entity)
    {
      csBlueprint *blueprint = csQueryClass<csBlueprint>(obj);
      if (blueprint)
      {
        entity = blueprint->CreateEntity();
      }
    }
  }
  else
  {

    if (!entry->HasAttribute("class"))
    {
      return 0;
    }
    const csClass *entityClass = csClassRegistry::Get()->GetClass(entry->GetAttribute("class"));
    if (!entityClass)
    {
      return 0;
    }

    entity = entityClass->CreateInstance<csEntity>();
  }
  if (!entity)
  {
    return 0;
  }

  std::map<csID, csEntityState*> states;
  for (const csfEntry *entityStateEntry = entry->GetEntry("entityState");
    entityStateEntry;
    entityStateEntry = entityStateEntry->GetSiblingEntry("entityState")) 
  {
    if (!entityStateEntry->HasAttribute("id"))
    {
      continue;
    }
    csID id = csID_Undefined;
    std::string idStr = entityStateEntry->GetAttribute("id");
    csEntityState *state = 0;
    if (idStr == std::string("root"))
    {
      state = entity->GetRootState();
    }
    else
    {
      csID id = entityStateEntry->GetAttributeLong("id");
      csEntityState *state = entity->GetState(id);
      bool alreadyPresent = state;
      state = csResourceManager::Get()->Load<csEntityState>(entityStateEntry, locator, state);
    }

    if (!state)
    {
      continue;
    }
    if (state->GetEntity() != entity)
    {
      state->SetId(id);


      states[id] = state;

      if (entityStateEntry->HasAttribute("parentId"))
      {
        csID parentId = entityStateEntry->GetAttributeLong("parentId");
        if (states.find(parentId) == states.end())
        {
          continue;
        }

        csEntityState *parentState = states[id];
        csSpatialState *spatialState = csQueryClass<csSpatialState>(state);
        csSpatialState *parentSpatialState = csQueryClass<csSpatialState>(parentState);
        if (spatialState && parentSpatialState)
        {
          entity->AddState(spatialState, parentSpatialState);
        }
        else
        {
          printf("Error attaching state [%p] to parentState [%p]\n", spatialState, parentSpatialState);
        }
      }
      else
      {
        if (entityStateEntry->GetAttribute("root", "false") == std::string("true"))
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
  }

  return entity;
}
