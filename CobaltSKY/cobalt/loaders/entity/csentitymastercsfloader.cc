
#include <cobalt/loaders/entity/csentitymastercsfloader.hh>
#include <cobalt/loaders/entity/csentityxmlloader.hh>
#include <cobalt/loaders/entity/csentitycsfloader.hh>
#include <cobalt/loaders/entity/csentityloaderregistry.hh>
#include <cobalt/entity/csentity.hh>
#include <cobalt/core/csclassregistry.hh>

// ***********************************************************************************************
// ***********************************************************************************************
//
//                   csEntityMasterLoader
//
// ***********************************************************************************************
// ***********************************************************************************************

csEntityMasterCSFLoader::csEntityMasterCSFLoader()
  : csBaseCSFLoader()
{

}


csEntityMasterCSFLoader::~csEntityMasterCSFLoader()
{

}


bool csEntityMasterCSFLoader::CanLoad(const csfEntry *entry, const csResourceLocator &locator, iObject *userData) const
{
  return entry->GetTagName()  == std::string("entity");
}


const csClass *csEntityMasterCSFLoader::EvalClass(const csfEntry *entry, const csResourceLocator &locator, iObject *userData) const
{
  if (!entry->HasAttribute())
  {
    return 0;
  }
  std::string className = entry->GetAttribute();
  return csClassRegistry::Get()->GetClass(className);
}


iObject *csEntityMasterCSFLoader::Load(const csfEntry *entry, const csResourceLocator &locator, iObject *userData) const
{
  if (!entry->HasAttribute())
  {
    return 0;
  }
  std::string className = entry->GetAttribute();
  const csClass *entityClass = csClassRegistry::Get()->GetClass(className);
  if (!entityClass)
  {
    return 0;
  }

  const csEntityCSFLoader *loader = csEntityLoaderRegistry::Get()->GetEntityCSFLoader(entityClass);
  if (!loader)
  {
    return 0;
  }

  csEntity *entity = entityClass->CreateInstance<csEntity>();

  iObject *obj = loader->Load(entry, locator, entity);
  //entity->Create();
  return obj;

}

