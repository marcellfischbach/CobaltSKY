
#include <valkyrie/loaders/entity/vkstaticcolliderstatexmlloader.hh>
#include <valkyrie/loaders/entity/vkentitystateloaderdata.hh>
#include <valkyrie/entity/vkstaticcolliderstate.hh>


vkStaticColliderStateXMLLoader::vkStaticColliderStateXMLLoader()
  : vkBaseColliderStateXMLLoader()
{

}


vkStaticColliderStateXMLLoader::~vkStaticColliderStateXMLLoader()
{

}

const vkClass *vkStaticColliderStateXMLLoader::EvalClass(TiXmlElement *element, const vkResourceLocator &locator, IObject *userData) const
{
  return vkStaticColliderState::GetStaticClass();
}


IObject *vkStaticColliderStateXMLLoader::Load(TiXmlElement *element, const vkResourceLocator &locator, IObject *userData) const
{
  if (!userData || !element)
  {
    return userData;
  }


  return vkBaseColliderStateXMLLoader::Load(element, locator, userData);
}

const vkClass *vkStaticColliderStateXMLLoader::GetLoadingClass() const
{
  return vkStaticColliderState::GetStaticClass();
}

