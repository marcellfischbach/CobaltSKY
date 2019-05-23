
#include <cobalt/loaders/csblueprintcsfloader.hh>
#include <cobalt/entity/blueprint/csblueprint.hh>
#include <cobalt/entity/blueprint/csbpentity.hh>
#include <cobalt/entity/blueprint/csbpentitystate.hh>
#include <csrefl/classregistry.hh>
#include <cobalt/core/property/csgenericpropertysetter.hh>
#include <cobalt/core/property/csresourcepropertysetter.hh>


cs::BlueprintCSFLoader::BlueprintCSFLoader()
  : cs::BaseCSFLoader()
{

}

cs::BlueprintCSFLoader::~BlueprintCSFLoader()
{

}

bool cs::BlueprintCSFLoader::CanLoad(const csfEntry *entry, const cs::ResourceLocator &locator, cs::iObject *userData) const
{
  CS_UNUSED(locator);
  CS_UNUSED(userData);
  return std::string("blueprint") == entry->GetTagName();
}

const cs::Class *cs::BlueprintCSFLoader::EvalClass(const csfEntry *entry, const cs::ResourceLocator &locator, cs::iObject *userData) const
{
  CS_UNUSED(entry);
  CS_UNUSED(locator);
  CS_UNUSED(userData);
  return cs::Blueprint::GetStaticClass();
}

cs::ResourceWrapper *cs::BlueprintCSFLoader::Load(const csfEntry *entry, const cs::ResourceLocator &locator, cs::iObject *userData) const
{
  cs::Blueprint *bp = new cs::Blueprint();

  const csfEntry *entityEntry = entry->GetEntry("entity");
  if (entityEntry)
  {
    LoadEntity(bp, entityEntry, locator, userData);
  }

  return new cs::BlueprintWrapper(bp);
}


void cs::BlueprintCSFLoader::LoadEntity(cs::Blueprint *blueprint, const csfEntry *entry, const cs::ResourceLocator &locator, cs::iObject *userData) const
{
  if (!entry->HasAttribute("class"))
  {
    return;
  }
  const cs::Class *entityClass = cs::ClassRegistry::Get()->GetClass(entry->GetAttribute("class"));
  if (!entityClass)
  {
    return;
  }
  cs::BPEntity *bpEntity = new cs::BPEntity();
  bpEntity->SetEntityClass(entityClass);
  blueprint->SetEntity(bpEntity);

  std::map<unsigned, cs::BPEntityState*> states;
  for (const csfEntry *entityStateEntry = entry->GetEntry("entityState");
    entityStateEntry; entityStateEntry = entityStateEntry->GetSiblingEntry("entityState"))
  {
    LoadEntityState(bpEntity, entityStateEntry, locator, userData);
  }
}


void cs::BlueprintCSFLoader::LoadEntityState(cs::BPEntity *entity, const csfEntry *entry, const cs::ResourceLocator &locator, cs::iObject *userData) const
{
  if (!entry->HasAttribute("class") || !entry->HasAttribute("id"))
  {
    return;
  }
  const cs::Class *entityStateClass = cs::ClassRegistry::Get()->GetClass(entry->GetAttribute("class"));
  if (!entityStateClass)
  {
    return;
  }
  cs::BPEntityState *bpEntityState = new cs::BPEntityState();
  bpEntityState->SetEntityStateClass(entityStateClass);
  bpEntityState->SetId(entry->GetAttributeInt("id"));
  bpEntityState->SetParentId(entry->GetAttributeInt("parentId"));
  if (entry->HasAttribute("root"))
  {
    std::string root = entry->GetAttribute("root");
    bpEntityState->SetRoot(root == std::string("true"));
  }
  entity->AddEntityState(bpEntityState);

  for (const csfEntry *propEntry = entry->GetEntry("property"); 
    propEntry; propEntry = propEntry->GetSiblingEntry("property"))
  {
    LoadProperty(bpEntityState, propEntry, locator, userData);
  }

}


void cs::BlueprintCSFLoader::LoadProperty(cs::BPEntityState *bpEntityState, const csfEntry *entry, const cs::ResourceLocator &locator, cs::iObject *userData) const
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
  bpEntityState->AddProperty(prop);
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

}

cs::PropertySetter *cs::BlueprintCSFLoader::CreateProperty(const csfEntry *entry) const
{
  cs::PropertySetter *result = 0;
  std::string tag = entry->GetTagName();
  if (std::string("resource") == tag && entry->HasAttribute("locator"))
  {
    cs::ResourcePropertySetter *prop = new cs::ResourcePropertySetter();
    prop->SetResourceLocator(cs::ResourceLocator(entry->GetAttribute("locator")));
    result = prop;
  }
  else if (std::string("bool") == tag && entry->HasAttribute("value"))
  {
    cs::GenericPropertySetter<bool> *prop = new cs::GenericPropertySetter<bool>;
    prop->Set(entry->GetAttribute("value") == "true");
    result = prop;
  }
  else if (std::string("float") == tag && entry->HasAttribute("value"))
  {
    cs::GenericPropertySetter<float> *prop = new cs::GenericPropertySetter<float>;
    prop->Set(entry->GetAttributeFloat("value"));
    result = prop;
  }
  else if (std::string("vector2f") == tag && entry->HasAttribute("x") && entry->HasAttribute("y"))
  {
    cs::GenericPropertySetter<cs::Vector2f> *prop = new cs::GenericPropertySetter<cs::Vector2f>;
    prop->Set(cs::Vector2f (
      entry->GetAttributeFloat("x"),
      entry->GetAttributeFloat("y")));
    result = prop;
  }
  else if (std::string("vector3f") == tag && entry->HasAttribute("x") && entry->HasAttribute("y") && entry->HasAttribute("z"))
  {
    cs::GenericPropertySetter<cs::Vector3f> *prop = new cs::GenericPropertySetter<cs::Vector3f>;
    prop->Set(cs::Vector3f(
      entry->GetAttributeFloat("x"),
      entry->GetAttributeFloat("y"),
      entry->GetAttributeFloat("z")));
    result = prop;
  }
  else if (std::string("vector4f") == tag && entry->HasAttribute("x") && entry->HasAttribute("y") && entry->HasAttribute("z") && entry->HasAttribute("w"))
  {
    cs::GenericPropertySetter<cs::Vector4f> *prop = new cs::GenericPropertySetter<cs::Vector4f>;
    prop->Set(cs::Vector4f(
      entry->GetAttributeFloat("x"),
      entry->GetAttributeFloat("y"),
      entry->GetAttributeFloat("z"),
      entry->GetAttributeFloat("w")));
    result = prop;
  }
  return result;
}
