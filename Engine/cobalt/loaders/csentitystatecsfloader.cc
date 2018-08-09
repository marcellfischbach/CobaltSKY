
#include <cobalt/loaders/csentitystatecsfloader.hh>
#include <cobalt/csengine.hh>
#include <cobalt/entity/blueprint/csblueprint.hh>
#include <cobalt/entity/csentitystate.hh>
#include <cobalt/core/csclassregistry.hh>
#include <cobalt/core/property/cspropertysetter.hh>

csEntityStateCSFLoader::csEntityStateCSFLoader()
  : csBaseCSFLoader()
{

}

csEntityStateCSFLoader::~csEntityStateCSFLoader()
{

}

bool csEntityStateCSFLoader::CanLoad(const csfEntry *entry, const csResourceLocator &locator, iObject *userData) const
{
  return entry->GetTagName() == std::string("entityState");
}

const csClass *csEntityStateCSFLoader::EvalClass(const csfEntry *entry, const csResourceLocator &locator, iObject *userData) const
{
  return csEntityState::GetStaticClass();
}

iObject *csEntityStateCSFLoader::Load(const csfEntry *entry, const csResourceLocator &locator, iObject *userData) const
{
  csEntityState *entityState = userData ? csQueryClass<csEntityState>(userData) : 0;
  if (!entityState)
  {
    if (entry->HasAttribute("locator"))
    {
      iObject *obj = csEng->Get(csResourceLocator(entry->GetAttribute("locator")));
      if (!obj)
      {
        return 0;
      }
      entityState = csQueryClass<csEntityState>(obj);
      if (!entityState)
      {
        csBlueprint *blueprint = csQueryClass<csBlueprint>(obj);
        if (blueprint)
        {
          entityState = blueprint->CreateEntityState();
        }
      }
    }
    else
    {

      if (!entry->HasAttribute("class"))
      {
        return 0;
      }
      const csClass *entityStateClass = csClassRegistry::Get()->GetClass(entry->GetAttribute("class"));
      if (!entityStateClass)
      {
        return 0;
      }
      entityState = entityStateClass->CreateInstance<csEntityState>();
    }
    if (!entityState)
    {
      return 0;
    }
  }

  for (const csfEntry *propEntry = entry->GetEntry("property");
    propEntry; propEntry = propEntry->GetSiblingEntry("property"))
  {
    LoadProperty(entityState, propEntry, locator, userData);
  }

  return entityState;
}


void csEntityStateCSFLoader::LoadProperty(csEntityState *entityState, const csfEntry *entry, const csResourceLocator &locator, iObject *userData) const
{
  if (!entry->HasAttribute("name"))
  {
    return;
  }
  csPropertySetter *prop = csResourceManager::Get()->Load<csPropertySetter>(entry, locator, userData);
  if (!prop)
  {
    return;
  }
  prop->SetName(entry->GetAttribute("name"));

  if (entry->HasAttribute("collection"))
  {
    std::string collection = entry->GetAttribute("collection");
    prop->SetCollection(collection == std::string("true"));
    if (entry->HasAttribute("idx"))
    {
      prop->SetCollectionIdx(entry->GetAttributeInt("idx"));
    }
  }

  prop->Apply(entityState);

  prop->Release();
}
