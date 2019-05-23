
#include <cobalt/loaders/csentitycsfloader.hh>
#include <cobalt/csengine.hh>
#include <cobalt/entity/blueprint/csblueprint.hh>
#include <cobalt/entity/csentity.hh>
#include <cobalt/entity/csspatialstate.hh>
#include <csrefl/classregistry.hh>

cs::EntityCSFLoader::EntityCSFLoader()
  : cs::BaseCSFLoader()
{

}

cs::EntityCSFLoader::~EntityCSFLoader()
{

}

bool cs::EntityCSFLoader::CanLoad(const cs::file::Entry *entry, const cs::ResourceLocator &locator, cs::iObject *userData) const
{
  return entry->GetTagName() == std::string("entity");
}

const cs::Class *cs::EntityCSFLoader::EvalClass(const cs::file::Entry *entry, const cs::ResourceLocator &locator, cs::iObject *userData) const
{
  return cs::Entity::GetStaticClass();
}

cs::ResourceWrapper *cs::EntityCSFLoader::Load(const cs::file::Entry *entry, const cs::ResourceLocator &locator, cs::iObject *userData) const
{
  CS_UNUSED(userData);
  cs::Entity *entity = nullptr;
  cs::EntityWrapper *wrapper= nullptr;
  if (entry->HasAttribute("locator"))
  { 
    cs::ResourceWrapper *obj = csEng->Get(cs::ResourceLocator(entry->GetAttribute("locator")));
    if (!obj)
    {
      return nullptr;
    }
    wrapper = cs::QueryClass<cs::EntityWrapper>(obj);
    if (!entity)
    {
      cs::BlueprintWrapper *blueprint = cs::QueryClass<cs::BlueprintWrapper>(obj);
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

    entity = entityClass->CreateInstance<cs::Entity>();
    wrapper = new cs::EntityWrapper(entity);
  }
  if (!entity && (!wrapper || wrapper->IsNull()))
  {
    return nullptr;
  }

  if (!entity)
  {
    entity = wrapper->Get();
  }



  std::map<csID, cs::EntityState*> states;
  for (const cs::file::Entry *entityStateEntry = entry->GetEntry("entityState");
    entityStateEntry;
    entityStateEntry = entityStateEntry->GetSiblingEntry("entityState")) 
  {
    if (!entityStateEntry->HasAttribute("id"))
    {
      continue;
    }
    csID id = csID_Undefined;
    std::string idStr = entityStateEntry->GetAttribute("id");
    cs::EntityState *state = nullptr;
    if (idStr == std::string("root"))
    {
      state = entity->GetRootState();
    }
    else
    {
      csID id = entityStateEntry->GetAttributeLong("id");
      cs::EntityState *state = entity->GetState(id);
      bool alreadyPresent = state;
      state = cs::ResourceManager::Get()->Load<cs::EntityState>(entityStateEntry, locator, state);
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

        cs::EntityState *parentState = states[id];
        cs::SpatialState *spatialState = cs::QueryClass<cs::SpatialState>(state);
        cs::SpatialState *parentSpatialState = cs::QueryClass<cs::SpatialState>(parentState);
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
  }

  return wrapper;
}
