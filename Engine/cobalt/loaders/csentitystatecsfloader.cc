
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
  CS_UNUSED(locator);
  CS_UNUSED(userData);
  return entry->GetTagName() == std::string("entityState");
}

const csClass *csEntityStateCSFLoader::EvalClass(const csfEntry *entry, const csResourceLocator &locator, iObject *userData) const
{
  CS_UNUSED(entry);
  CS_UNUSED(locator);
  CS_UNUSED(userData);
  return csEntityStateWrapper::GetStaticClass();
}

csResourceWrapper *csEntityStateCSFLoader::Load(const csfEntry *entry, const csResourceLocator &locator, iObject *userData) const
{
  CS_UNUSED(userData);
  csEntityStateWrapper *wrapper = userData ? csQueryClass<csEntityStateWrapper>(userData) : nullptr;
  csEntityState *state = nullptr;
  if (!wrapper)
  {
    if (entry->HasAttribute("locator"))
    {
      csResourceWrapper *obj = csEng->Get(csResourceLocator(entry->GetAttribute("locator")));
      if (!obj)
      {
        return nullptr;
      }
      wrapper = csQueryClass<csEntityStateWrapper>(obj);
      if (!wrapper)
      {
        csBlueprintWrapper *blueprint = csQueryClass<csBlueprintWrapper>(obj);
        if (blueprint && blueprint->IsValid())
        {
          wrapper = blueprint->Get()->CreateEntityState();
        }
      }
    }
    else
    {

      if (!entry->HasAttribute("class"))
      {
        return nullptr;
      }
      const csClass *entityStateClass = csClassRegistry::Get()->GetClass(entry->GetAttribute("class"));
      if (!entityStateClass)
      {
        return nullptr;
      }
      state = entityStateClass->CreateInstance<csEntityState>();
      wrapper = new csEntityStateWrapper(state);
    }
    if (!state && (!wrapper || wrapper->IsNull()))
    {
      return nullptr;
    }

    if (!state)
    {
      state = wrapper->Get();
    }
  }

  for (const csfEntry *propEntry = entry->GetEntry("property");
    propEntry; propEntry = propEntry->GetSiblingEntry("property"))
  {
    LoadProperty(state, propEntry, locator, userData);
  }

  return wrapper;
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
