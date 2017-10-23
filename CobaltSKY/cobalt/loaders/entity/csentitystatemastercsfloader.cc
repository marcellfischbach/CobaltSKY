
#include <cobalt/loaders/entity/csentitystatemastercsfloader.hh>
#include <cobalt/loaders/entity/csentitystateloaderdata.hh>
#include <cobalt/loaders/entity/csentitystatecsfloader.hh>
#include <cobalt/loaders/entity/csentityloaderregistry.hh>
#include <cobalt/entity/csentity.hh>
#include <cobalt/entity/csentitystate.hh>
#include <cobalt/core/csclassregistry.hh>

// ***********************************************************************************************
// ***********************************************************************************************
//
//                   csEntityStateMasterLoader
//
// ***********************************************************************************************
// ***********************************************************************************************


csEntityStateMasterCSFLoader::csEntityStateMasterCSFLoader()
  : csBaseCSFLoader()
{

}


csEntityStateMasterCSFLoader::~csEntityStateMasterCSFLoader()
{

}


bool csEntityStateMasterCSFLoader::CanLoad(const csfEntry *entry, const csResourceLocator &locator, iObject *userData) const
{
  return entry->GetTagName() == std::string("entityState");
}

const csClass *csEntityStateMasterCSFLoader::EvalClass(const csfEntry *entry, const csResourceLocator &locator, iObject *userData) const
{
  if (!entry->HasAttribute())
  {
    return 0;
  }
  std::string className = entry->GetAttribute();
  return csClassRegistry::Get()->GetClass(className);
}


iObject *csEntityStateMasterCSFLoader::Load(const csfEntry *entry, const csResourceLocator &locator, iObject *userData) const
{
  if (!entry->HasAttribute())
  {
    return 0;
  }

  csEntity *entity = csQueryClass<csEntity>(userData);


  std::string className =entry->GetAttribute();
  const csClass *entityStateClass = csClassRegistry::Get()->GetClass(className);
  if (!entityStateClass)
  {
    return 0;
  }

  const csEntityStateCSFLoader *loader = csEntityLoaderRegistry::Get()->GetEntityStateCSFLoader(entityStateClass);
  if (!loader)
  {
    return 0;
  }

  csEntityState *entityState = entityStateClass->CreateInstance<csEntityState>();
  csEntityStateLoaderData data;
  data.entity = entity;
  data.state = entityState;

  iObject *obj = loader->Load(entry, locator, &data);
  //entityState->Create();
  return obj;

}

