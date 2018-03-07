
#include <cobalt/loaders/csentitystatecsfloader.hh>

csEntityStateCSFLoader::csEntityStateCSFLoader()
  : csBaseCSFLoader()
{

}

csEntityStateCSFLoader::~csEntityStateCSFLoader()
{

}

bool csEntityStateCSFLoader::CanLoad(const csfEntry *entry, const csResourceLocator &locator, iObject *userData) const
{
  return false;
}

const csClass *csEntityStateCSFLoader::EvalClass(const csfEntry *entry, const csResourceLocator &locator, iObject *userData) const
{
  return 0;
}

iObject *csEntityStateCSFLoader::Load(const csfEntry *entry, const csResourceLocator &locator, iObject *userData) const
{
  return 0;
}
