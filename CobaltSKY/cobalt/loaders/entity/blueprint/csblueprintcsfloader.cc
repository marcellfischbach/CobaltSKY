
#include <cobalt/loaders/entity/blueprint/csblueprintcsfloader.hh>
#include <cobalt/entity/blueprint/csblueprint.hh>


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
  return 0;
}
