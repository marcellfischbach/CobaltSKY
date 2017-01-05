
#include <Valkyrie/Loaders/entity/vkentitystatemasterxmlloader.hh>
#include <Valkyrie/Loaders/entity/vkentitystateloaderdata.hh>
#include <Valkyrie/Loaders/entity/vkentitystatexmlloader.hh>
#include <Valkyrie/Loaders/entity/vkentityloaderregistry.hh>
#include <Valkyrie/entity/vkentity.hh>
#include <Valkyrie/core/vkclassregistry.hh>

// ***********************************************************************************************
// ***********************************************************************************************
//
//                   vkEntityStateMasterLoader
//
// ***********************************************************************************************
// ***********************************************************************************************


vkEntityStateMasterXMLLoader::vkEntityStateMasterXMLLoader()
  : vkBaseXMLLoader()
{

}


vkEntityStateMasterXMLLoader::~vkEntityStateMasterXMLLoader()
{

}


bool vkEntityStateMasterXMLLoader::CanLoad(TiXmlElement *element, const vkResourceLocator &locator, IObject *userData) const
{
  return std::string(element->Value()) == std::string("entityState");
}

const vkClass *vkEntityStateMasterXMLLoader::EvalClass(TiXmlElement *element, const vkResourceLocator &locator, IObject *userData) const
{
  if (!element->Attribute("class"))
  {
    return 0;
  }
  vkString className(element->Attribute("class"));
  return vkClassRegistry::Get()->GetClass(className);
}


IObject *vkEntityStateMasterXMLLoader::Load(TiXmlElement *element, const vkResourceLocator &locator, IObject *userData) const
{
  if (!element->Attribute("class"))
  {
    return 0;
  }

  vkEntity *entity = vkQueryClass<vkEntity>(userData);


  vkString className(element->Attribute("class"));
  const vkClass *entityStateClass = vkClassRegistry::Get()->GetClass(className);
  if (!entityStateClass)
  {
    return 0;
  }

  const vkEntityStateXMLLoader *loader = vkEntityLoaderRegistry::Get()->GetEntityStateLoader(entityStateClass);
  if (!loader)
  {
    return 0;
  }

  vkEntityState *entityState = entityStateClass->CreateInstance<vkEntityState>();
  vkEntityStateLoaderData data;
  data.entity = entity;
  data.state = entityState;

  IObject *obj = loader->Load(element, locator, &data);
  //entityState->Create();
  return obj;

}

