
#include <cobalt/loaders/entity/csentitymasterxmlloader.hh>
#include <cobalt/loaders/entity/csentityxmlloader.hh>
#include <cobalt/loaders/entity/csentityloaderregistry.hh>
#include <cobalt/entity/csentity.hh>
#include <cobalt/core/csclassregistry.hh>

// ***********************************************************************************************
// ***********************************************************************************************
//
//                   csEntityMasterLoader
//
// ***********************************************************************************************
// ***********************************************************************************************

csEntityMasterXMLLoader::csEntityMasterXMLLoader()
  : csBaseXMLLoader()
{

}


csEntityMasterXMLLoader::~csEntityMasterXMLLoader()
{

}


bool csEntityMasterXMLLoader::CanLoad(TiXmlElement *element, const csResourceLocator &locator, iObject *userData) const
{
  return std::string(element->Value()) == std::string("entity");
}


const csClass *csEntityMasterXMLLoader::EvalClass(TiXmlElement *element, const csResourceLocator &locator, iObject *userData) const
{
  if (!element->Attribute("class"))
  {
    return 0;
  }
  csString className(element->Attribute("class"));
  return csClassRegistry::Get()->GetClass(className);
}


iObject *csEntityMasterXMLLoader::Load(TiXmlElement *element, const csResourceLocator &locator, iObject *userData) const
{
  if (!element->Attribute("class"))
  {
    return 0;
  }
  csString className(element->Attribute("class"));
  const csClass *entityClass = csClassRegistry::Get()->GetClass(className);
  if (!entityClass)
  {
    return 0;
  }

  const csEntityXMLLoader *loader = csEntityLoaderRegistry::Get()->GetEntityLoader(entityClass);
  if (!loader)
  {
    return 0;
  }

  csEntity *entity = entityClass->CreateInstance<csEntity>();

  iObject *obj = loader->Load(element, locator, entity);
  //entity->Create();
  return obj;

}

