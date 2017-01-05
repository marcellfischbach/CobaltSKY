
#include <Valkyrie/Loaders/entity/vkstaticcolliderstatexmlloader.hh>
#include <Valkyrie/Loaders/entity/vkentitystateloaderdata.hh>
#include <Valkyrie/entity/vkstaticcolliderstate.hh>


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

