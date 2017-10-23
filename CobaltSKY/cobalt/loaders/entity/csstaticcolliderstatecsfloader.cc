
#include <cobalt/loaders/entity/csstaticcolliderstatecsfloader.hh>
#include <cobalt/loaders/entity/csentitystateloaderdata.hh>
#include <cobalt/entity/csstaticcolliderstate.hh>


csStaticColliderStateCSFLoader::csStaticColliderStateCSFLoader()
  : csBaseColliderStateCSFLoader()
{

}


csStaticColliderStateCSFLoader::~csStaticColliderStateCSFLoader()
{

}

const csClass *csStaticColliderStateCSFLoader::EvalClass(const csfEntry *entry, const csResourceLocator &locator, iObject *userData) const
{
  return csStaticColliderState::GetStaticClass();
}


iObject *csStaticColliderStateCSFLoader::Load(const csfEntry *entry
                                              , const csResourceLocator &locator, iObject *userData) const
{
  if (!userData || !entry)
  {
    return userData;
  }


  return csBaseColliderStateCSFLoader::Load(entry, locator, userData);
}

const csClass *csStaticColliderStateCSFLoader::GetLoadingClass() const
{
  return csStaticColliderState::GetStaticClass();
}

