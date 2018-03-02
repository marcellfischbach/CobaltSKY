
#include <cobalt/loaders/entity/blueprint/csblueprintcsfloader.hh>
#include <cobalt/entity/blueprint/csblueprint.hh>
#include <cobalt/entity/blueprint/csbpentity.hh>
#include <cobalt/entity/blueprint/csbpentitystate.hh>
#include <cobalt/core/csclassregistry.hh>
#include <cobalt/core/property/csgenericpropertysetter.hh>
#include <cobalt/core/property/csresourcepropertysetter.hh>


csBlueprintCSFLoader::csBlueprintCSFLoader()
  : csBaseCSFLoader()
{

}

csBlueprintCSFLoader::~csBlueprintCSFLoader()
{

}

bool csBlueprintCSFLoader::CanLoad(const csfEntry *entry, const csResourceLocator &locator, iObject *userData) const
{
  return std::string("blueprint") == entry->GetTagName();
}

const csClass *csBlueprintCSFLoader::EvalClass(const csfEntry *entry, const csResourceLocator &locator, iObject *userData) const
{
  return csBlueprint::GetStaticClass();
}

iObject *csBlueprintCSFLoader::Load(const csfEntry *entry, const csResourceLocator &locator, iObject *userData) const
{
  csBlueprint *bp = new csBlueprint();

  const csfEntry *entityEntry = entry->GetEntry("entity");
  if (entityEntry)
  {
    LoadEntity(bp, entityEntry, locator, userData);
  }

  return bp;
}


void csBlueprintCSFLoader::LoadEntity(csBlueprint *blueprint, const csfEntry *entry, const csResourceLocator &locator, iObject *userData) const
{
  if (!entry->HasAttribute("class"))
  {
    return;
  }
  const csClass *entityClass = csClassRegistry::Get()->GetClass(entry->GetAttribute("class"));
  if (!entityClass)
  {
    return;
  }
  csBPEntity *bpEntity = new csBPEntity();
  bpEntity->SetEntityClass(entityClass);
  blueprint->SetEntity(bpEntity);

  std::map<unsigned, csBPEntityState*> states;
  for (const csfEntry *entityStateEntry = entry->GetEntry("entityState");
    entityStateEntry; entityStateEntry = entityStateEntry->GetSiblingEntry("entityState"))
  {
    LoadEntityState(bpEntity, entityStateEntry, locator, userData);
  }
}


void csBlueprintCSFLoader::LoadEntityState(csBPEntity *entity, const csfEntry *entry, const csResourceLocator &locator, iObject *userData) const
{
  if (!entry->HasAttribute("class") || !entry->HasAttribute("id"))
  {
    return;
  }
  const csClass *entityStateClass = csClassRegistry::Get()->GetClass(entry->GetAttribute("class"));
  if (!entityStateClass)
  {
    return;
  }
  csBPEntityState *bpEntityState = new csBPEntityState();
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


void csBlueprintCSFLoader::LoadProperty(csBPEntityState *bpEntityState, const csfEntry *entry, const csResourceLocator &locator, iObject *userData) const
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

csPropertySetter *csBlueprintCSFLoader::CreateProperty(const csfEntry *entry) const
{
  csPropertySetter *result = 0;
  std::string tag = entry->GetTagName();
  if (std::string("resource") == tag && entry->HasAttribute("locator"))
  {
    csResourcePropertySetter *prop = new csResourcePropertySetter();
    prop->SetResourceLocator(csResourceLocator(entry->GetAttribute("locator")));
    result = prop;
  }
  else if (std::string("bool") == tag && entry->HasAttribute("value"))
  {
    csGenericPropertySetter<bool> *prop = new csGenericPropertySetter<bool>;
    prop->Set(entry->GetAttribute("value") == "true");
    result = prop;
  }
  else if (std::string("float") == tag && entry->HasAttribute("value"))
  {
    csGenericPropertySetter<float> *prop = new csGenericPropertySetter<float>;
    prop->Set(entry->GetAttributeFloat("value"));
    result = prop;
  }
  else if (std::string("vector2f") == tag && entry->HasAttribute("x") && entry->HasAttribute("y"))
  {
    csGenericPropertySetter<csVector2f> *prop = new csGenericPropertySetter<csVector2f>;
    prop->Set(csVector2f (
      entry->GetAttributeFloat("x"),
      entry->GetAttributeFloat("y")));
    result = prop;
  }
  else if (std::string("vector3f") == tag && entry->HasAttribute("x") && entry->HasAttribute("y") && entry->HasAttribute("z"))
  {
    csGenericPropertySetter<csVector3f> *prop = new csGenericPropertySetter<csVector3f>;
    prop->Set(csVector3f(
      entry->GetAttributeFloat("x"),
      entry->GetAttributeFloat("y"),
      entry->GetAttributeFloat("z")));
    result = prop;
  }
  else if (std::string("vector4f") == tag && entry->HasAttribute("x") && entry->HasAttribute("y") && entry->HasAttribute("z") && entry->HasAttribute("w"))
  {
    csGenericPropertySetter<csVector4f> *prop = new csGenericPropertySetter<csVector4f>;
    prop->Set(csVector4f(
      entry->GetAttributeFloat("x"),
      entry->GetAttributeFloat("y"),
      entry->GetAttributeFloat("z"),
      entry->GetAttributeFloat("w")));
    result = prop;
  }
  return result;
}
