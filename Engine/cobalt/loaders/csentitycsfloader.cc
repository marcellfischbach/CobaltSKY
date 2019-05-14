
#include <cobalt/loaders/csentitycsfloader.hh>
#include <cobalt/csengine.hh>
#include <cobalt/entity/blueprint/csblueprint.hh>
#include <cobalt/entity/csentity.hh>
#include <cobalt/entity/csspatialstate.hh>
#include <csrefl/classregistry.hh>

csEntityCSFLoader::csEntityCSFLoader()
  : csBaseCSFLoader()
{

}

csEntityCSFLoader::~csEntityCSFLoader()
{

}

bool csEntityCSFLoader::CanLoad(const csfEntry *entry, const csResourceLocator &locator, cs::iObject *userData) const
{
  return entry->GetTagName() == std::string("entity");
}

const cs::Class *csEntityCSFLoader::EvalClass(const csfEntry *entry, const csResourceLocator &locator, cs::iObject *userData) const
{
  return csEntity::GetStaticClass();
}

csResourceWrapper *csEntityCSFLoader::Load(const csfEntry *entry, const csResourceLocator &locator, cs::iObject *userData) const
{
  CS_UNUSED(userData);
  csEntity *entity = nullptr;
  csEntityWrapper *wrapper= nullptr;
  if (entry->HasAttribute("locator"))
  { 
    csResourceWrapper *obj = csEng->Get(csResourceLocator(entry->GetAttribute("locator")));
    if (!obj)
    {
      return nullptr;
    }
    wrapper = cs::QueryClass<csEntityWrapper>(obj);
    if (!entity)
    {
      csBlueprintWrapper *blueprint = cs::QueryClass<csBlueprintWrapper>(obj);
      if (blueprint && blueprint->IsValid())
      {
        wrapper = blueprint->Get()->CreateEntity();
      }
    }
  }
  else
  {

    if (!entry->HasAttribute("class"))
    {
      return nullptr;
    }
    const cs::Class *entityClass = cs::ClassRegistry::Get()->GetClass(entry->GetAttribute("class"));
    if (!entityClass)
    {
      return nullptr;
    }

    entity = entityClass->CreateInstance<csEntity>();
    wrapper = new csEntityWrapper(entity);
  }
  if (!entity && (!wrapper || wrapper->IsNull()))
  {
    return nullptr;
  }

  if (!entity)
  {
    entity = wrapper->Get();
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
    csEntityState *state = nullptr;
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
        csSpatialState *spatialState = cs::QueryClass<csSpatialState>(state);
        csSpatialState *parentSpatialState = cs::QueryClass<csSpatialState>(parentState);
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
  }

  return wrapper;
}
