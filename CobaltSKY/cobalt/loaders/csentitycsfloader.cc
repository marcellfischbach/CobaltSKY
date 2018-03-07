
#include <cobalt/loaders/csentitycsfloader.hh>

csEntityCSFLoader::csEntityCSFLoader()
  : csBaseCSFLoader()
{

}

csEntityCSFLoader::~csEntityCSFLoader()
{

}

bool csEntityCSFLoader::CanLoad(const csfEntry *entry, const csResourceLocator &locator, iObject *userData) const
{
  return false;
}

const csClass *csEntityCSFLoader::EvalClass(const csfEntry *entry, const csResourceLocator &locator, iObject *userData) const
{
  return 0;
}

iObject *csEntityCSFLoader::Load(const csfEntry *entry, const csResourceLocator &locator, iObject *userData) const
{
  return 0;
}
