

#include <Valkyrie/Loaders/EntityLoader.hh>

vkEntityLoader::vkEntityLoader()
  : vkBaseXMLLoader()
{

}


vkEntityLoader::~vkEntityLoader()
{

}


bool vkEntityLoader::CanLoad(TiXmlElement *element, const vkResourceLocator &locator, IObject *userData) const
{
  return false;
}


IObject *vkEntityLoader::Load(TiXmlElement *element, const vkResourceLocator &locator, IObject *userData) const
{
  return 0;
}

