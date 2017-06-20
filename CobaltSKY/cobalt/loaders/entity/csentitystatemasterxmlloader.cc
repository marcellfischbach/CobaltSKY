
#include <cobalt/loaders/entity/csentitystatemasterxmlloader.hh>
#include <cobalt/loaders/entity/csentitystateloaderdata.hh>
#include <cobalt/loaders/entity/csentitystatexmlloader.hh>
#include <cobalt/loaders/entity/csentityloaderregistry.hh>
#include <cobalt/entity/csentity.hh>
#include <cobalt/entity/csentitystate.hh>
#include <cobalt/core/csclassregistry.hh>

// ***********************************************************************************************
// ***********************************************************************************************
//
//                   csEntityStateMasterLoader
//
// ***********************************************************************************************
// ***********************************************************************************************


csEntityStateMasterXMLLoader::csEntityStateMasterXMLLoader()
  : csBaseXMLLoader()
{

}


csEntityStateMasterXMLLoader::~csEntityStateMasterXMLLoader()
{

}


bool csEntityStateMasterXMLLoader::CanLoad(TiXmlElement *element, const csResourceLocator &locator, iObject *userData) const
{
  return std::string(element->Value()) == std::string("entityState");
}

const csClass *csEntityStateMasterXMLLoader::EvalClass(TiXmlElement *element, const csResourceLocator &locator, iObject *userData) const
{
  if (!element->Attribute("class"))
  {
    return 0;
  }
  csString className(element->Attribute("class"));
  return csClassRegistry::Get()->GetClass(className);
}


iObject *csEntityStateMasterXMLLoader::Load(TiXmlElement *element, const csResourceLocator &locator, iObject *userData) const
{
  if (!element->Attribute("class"))
  {
    return 0;
  }

  csEntity *entity = csQueryClass<csEntity>(userData);


  csString className(element->Attribute("class"));
  const csClass *entityStateClass = csClassRegistry::Get()->GetClass(className);
  if (!entityStateClass)
  {
    return 0;
  }

  const csEntityStateXMLLoader *loader = csEntityLoaderRegistry::Get()->GetEntityStateLoader(entityStateClass);
  if (!loader)
  {
    return 0;
  }

  csEntityState *entityState = entityStateClass->CreateInstance<csEntityState>();
  csEntityStateLoaderData data;
  data.entity = entity;
  data.state = entityState;

  iObject *obj = loader->Load(element, locator, &data);
  //entityState->Create();
  return obj;

}

