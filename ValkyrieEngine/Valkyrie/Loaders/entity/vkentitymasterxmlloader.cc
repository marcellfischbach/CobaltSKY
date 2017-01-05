
#include <Valkyrie/Loaders/entity/vkentitymasterxmlloader.hh>
#include <Valkyrie/Loaders/entity/vkentityxmlloader.hh>
#include <Valkyrie/Loaders/entity/vkentityloaderregistry.hh>
#include <Valkyrie/entity/vkentity.hh>
#include <Valkyrie/core/vkclassregistry.hh>

// ***********************************************************************************************
// ***********************************************************************************************
//
//                   vkEntityMasterLoader
//
// ***********************************************************************************************
// ***********************************************************************************************

vkEntityMasterXMLLoader::vkEntityMasterXMLLoader()
  : vkBaseXMLLoader()
{

}


vkEntityMasterXMLLoader::~vkEntityMasterXMLLoader()
{

}


bool vkEntityMasterXMLLoader::CanLoad(TiXmlElement *element, const vkResourceLocator &locator, IObject *userData) const
{
  return std::string(element->Value()) == std::string("entity");
}


const vkClass *vkEntityMasterXMLLoader::EvalClass(TiXmlElement *element, const vkResourceLocator &locator, IObject *userData) const
{
  if (!element->Attribute("class"))
  {
    return 0;
  }
  vkString className(element->Attribute("class"));
  return vkClassRegistry::Get()->GetClass(className);
}


IObject *vkEntityMasterXMLLoader::Load(TiXmlElement *element, const vkResourceLocator &locator, IObject *userData) const
{
  if (!element->Attribute("class"))
  {
    return 0;
  }
  vkString className(element->Attribute("class"));
  const vkClass *entityClass = vkClassRegistry::Get()->GetClass(className);
  if (!entityClass)
  {
    return 0;
  }

  const vkEntityXMLLoader *loader = vkEntityLoaderRegistry::Get()->GetEntityLoader(entityClass);
  if (!loader)
  {
    return 0;
  }

  vkEntity *entity = entityClass->CreateInstance<vkEntity>();

  IObject *obj = loader->Load(element, locator, entity);
  //entity->Create();
  return obj;

}

