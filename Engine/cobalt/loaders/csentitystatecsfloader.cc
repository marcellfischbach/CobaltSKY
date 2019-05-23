
#include <cobalt/loaders/csentitystatecsfloader.hh>
#include <cobalt/csengine.hh>
#include <cobalt/entity/blueprint/csblueprint.hh>
#include <cobalt/entity/csentitystate.hh>
#include <csrefl/classregistry.hh>
#include <cobalt/core/property/cspropertysetter.hh>

cs::EntityStateCSFLoader::EntityStateCSFLoader()
  : cs::BaseCSFLoader()
{

}

cs::EntityStateCSFLoader::~EntityStateCSFLoader()
{

}

bool cs::EntityStateCSFLoader::CanLoad(const csfEntry *entry, const cs::ResourceLocator &locator, cs::iObject *userData) const
{
  CS_UNUSED(locator);
  CS_UNUSED(userData);
  return entry->GetTagName() == std::string("entityState");
}

const cs::Class *cs::EntityStateCSFLoader::EvalClass(const csfEntry *entry, const cs::ResourceLocator &locator, cs::iObject *userData) const
{
  CS_UNUSED(entry);
  CS_UNUSED(locator);
  CS_UNUSED(userData);
  return cs::EntityStateWrapper::GetStaticClass();
}

cs::ResourceWrapper *cs::EntityStateCSFLoader::Load(const csfEntry *entry, const cs::ResourceLocator &locator, cs::iObject *userData) const
{
  CS_UNUSED(userData);
  cs::EntityStateWrapper *wrapper = userData ? cs::QueryClass<cs::EntityStateWrapper>(userData) : nullptr;
  cs::EntityState *state = nullptr;
  if (!wrapper)
  {
    if (entry->HasAttribute("locator"))
    {
      cs::ResourceWrapper *obj = csEng->Get(cs::ResourceLocator(entry->GetAttribute("locator")));
      if (!obj)
      {
        return nullptr;
      }
      wrapper = cs::QueryClass<cs::EntityStateWrapper>(obj);
      if (!wrapper)
      {
        cs::BlueprintWrapper *blueprint = cs::QueryClass<cs::BlueprintWrapper>(obj);
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
      const cs::Class *entityStateClass = cs::ClassRegistry::Get()->GetClass(entry->GetAttribute("class"));
      if (!entityStateClass)
      {
        return nullptr;
      }
      state = entityStateClass->CreateInstance<cs::EntityState>();
      wrapper = new cs::EntityStateWrapper(state);
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


void cs::EntityStateCSFLoader::LoadProperty(cs::EntityState *entityState, const csfEntry *entry, const cs::ResourceLocator &locator, cs::iObject *userData) const
{
  if (!entry->HasAttribute("name"))
  {
    return;
  }
  cs::PropertySetter *prop = cs::ResourceManager::Get()->Load<cs::PropertySetter>(entry, locator, userData);
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
