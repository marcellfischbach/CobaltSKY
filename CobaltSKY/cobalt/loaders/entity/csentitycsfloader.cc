#include <cobalt/loaders/entity/csentitycsfloader.hh>
#include <cobalt/entity/csentity.hh>
#include <cobalt/entity/csentitystate.hh>


// ***********************************************************************************************
// ***********************************************************************************************
//
//                   csEntityLoader
//
// ***********************************************************************************************
// ***********************************************************************************************


csEntityCSFLoader::csEntityCSFLoader()
  : csBaseCSFLoader()
{

}


csEntityCSFLoader::~csEntityCSFLoader()
{

}


bool csEntityCSFLoader::CanLoad(const csfEntry *entry, const csResourceLocator &locator, iObject *userData) const
{
  return userData != 0;
}

const csClass *csEntityCSFLoader::EvalClass(const csfEntry *entry, const csResourceLocator &locator, iObject *userData) const
{
  return csEntity::GetStaticClass();
}

iObject *csEntityCSFLoader::Load(const csfEntry *entry, const csResourceLocator &locator, iObject *userData) const
{
  csEntity *entity = csQueryClass<csEntity>(userData);
  if (!entity)
  {
    userData->Release();
    return 0;
  }

  if (entry->HasAttribute("name"))
  {
    entity->SetName(entry->GetAttribute("name"));
  }

  std::map<std::string, csSpatialState*> spatialStates;
  const csfEntry *statesEntry = entry->GetEntry("entityStates");
  if (statesEntry)
  {
    for (const csfEntry *stateEntry = statesEntry->GetEntry("entityState");
         stateEntry;
         stateEntry = stateEntry->GetSiblingEntry("entityState"))
    {

      csEntityState *entityState = csResourceManager::Get()->Load<csEntityState>(stateEntry, locator, entity);
      if (entityState)
      {
        bool addedToEntity = false;
        csSpatialState *spatialState = entityState->ToSpatialState();
        if (spatialState)
        {
          if (stateEntry->HasAttribute("id"))
          {
            spatialStates[stateEntry->GetAttribute("id")] = spatialState;
          }

          if (stateEntry->HasAttribute("parentID"))
          {
            csSpatialState *parentState = spatialStates[stateEntry->GetAttribute("parentID")];
            if (parentState)
            {
              entity->AddState(spatialState, parentState);
              addedToEntity = true;
            }
          }
        }

        if (!addedToEntity)
        {
          entity->AddState(entityState);
        }
      }
    }
  }

  const csfEntry *rootEntry = entry->GetEntry("rootState");
  if (rootEntry && rootEntry->HasAttribute("id"))
  {
    csSpatialState *rootState = spatialStates[rootEntry->GetAttribute("id")];
    if (rootState)
    {
      entity->SetRootState(rootState);
    }
  }

  return entity;
}

const csClass *csEntityCSFLoader::GetLoadingClass() const
{
  return csEntity::GetStaticClass();
}



