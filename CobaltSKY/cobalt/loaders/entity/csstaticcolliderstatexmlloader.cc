
#include <cobalt/loaders/entity/csstaticcolliderstatexmlloader.hh>
#include <cobalt/loaders/entity/csentitystateloaderdata.hh>
#include <cobalt/entity/csstaticcolliderstate.hh>


csStaticColliderStateXMLLoader::csStaticColliderStateXMLLoader()
  : csBaseColliderStateXMLLoader()
{

}


csStaticColliderStateXMLLoader::~csStaticColliderStateXMLLoader()
{

}

const csClass *csStaticColliderStateXMLLoader::EvalClass(TiXmlElement *element, const csResourceLocator &locator, iObject *userData) const
{
  return csStaticColliderState::GetStaticClass();
}


iObject *csStaticColliderStateXMLLoader::Load(TiXmlElement *element, const csResourceLocator &locator, iObject *userData) const
{
  if (!userData || !element)
  {
    return userData;
  }


  return csBaseColliderStateXMLLoader::Load(element, locator, userData);
}

const csClass *csStaticColliderStateXMLLoader::GetLoadingClass() const
{
  return csStaticColliderState::GetStaticClass();
}

