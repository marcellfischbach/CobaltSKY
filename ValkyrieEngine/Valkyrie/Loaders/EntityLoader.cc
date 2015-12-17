
#include <Valkyrie/Loaders/EntityLoader.hh>
#include <Valkyrie/Entity/Entity.hh>
#include <Valkyrie/Entity/EntityState.hh>
#include <Valkyrie/Entity/LightState.hh>
#include <Valkyrie/Entity/MeshState.hh>
#include <Valkyrie/Core/ClassRegistry.hh>

vkEntityMasterLoader::vkEntityMasterLoader()
  : vkBaseXMLLoader()
{

}


vkEntityMasterLoader::~vkEntityMasterLoader()
{

}


bool vkEntityMasterLoader::CanLoad(TiXmlElement *element, const vkResourceLocator &locator, IObject *userData) const
{
  return std::string(element->Value()) == std::string("entity");
}


IObject *vkEntityMasterLoader::Load(TiXmlElement *element, const vkResourceLocator &locator, IObject *userData) const
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

  const vkEntityLoader *loader = vkEntityLoaderRegistry::Get()->GetEntityLoader(entityClass);
  if (!loader)
  {
    return 0;
  }

  vkEntity *entity = entityClass->CreateInstance<vkEntity>();

  return loader->Load(element, locator, entity);
}



vkEntityLoader::vkEntityLoader()
  : vkBaseXMLLoader()
{

}


vkEntityLoader::~vkEntityLoader()
{

}


bool vkEntityLoader::CanLoad(TiXmlElement *element, const vkResourceLocator &locator, IObject *userData) const
{
  return userData != 0;
}


IObject *vkEntityLoader::Load(TiXmlElement *element, const vkResourceLocator &locator, IObject *userData) const
{
  vkEntity *entity = vkQueryClass<vkEntity>(userData);
  if (!entity)
  {
    userData->Release();
    return 0;
  }

  if (element->Attribute("name"))
  {
    entity->SetName(vkString(element->Attribute("name")));
  }

  std::map<vkString, vkSpatialState*> spatialStates;
  TiXmlElement *statesElement = element->FirstChildElement("entityStates");
  if (statesElement)
  {
    for (TiXmlElement *stateElement = statesElement->FirstChildElement("entityState");
    stateElement;
      stateElement = stateElement->NextSiblingElement("entityState"))
    {

      vkEntityState *entityState = vkResourceManager::Get()->Load<vkEntityState>(stateElement, locator, 0);
      if (entityState)
      {
        entity->AddState(entityState);
        vkSpatialState *spatialState = entityState->ToSpatialState();
        if (stateElement->Attribute("id") && spatialState)
        {
          spatialStates[vkString(stateElement->Attribute("id"))] = spatialState;
        }
      }
    }
  }

  TiXmlElement *rootElement = element->FirstChildElement("rootState");
  if (rootElement && rootElement->Attribute("id"))
  {
    vkSpatialState *rootState = spatialStates[vkString(rootElement->Attribute("id"))];
    if (rootState)
    {
      entity->SetRootState(rootState);
    }
  }

  return entity;
}

const vkClass *vkEntityLoader::GetLoadingClass() const
{
  return vkEntityClass::Get();
}









vkEntityStateMasterLoader::vkEntityStateMasterLoader()
  : vkBaseXMLLoader()
{

}


vkEntityStateMasterLoader::~vkEntityStateMasterLoader()
{

}


bool vkEntityStateMasterLoader::CanLoad(TiXmlElement *element, const vkResourceLocator &locator, IObject *userData) const
{
  return std::string(element->Value()) == std::string("entityState");
}


IObject *vkEntityStateMasterLoader::Load(TiXmlElement *element, const vkResourceLocator &locator, IObject *userData) const
{
  if (!element->Attribute("class"))
  {
    return 0;
  }
  vkString className(element->Attribute("class"));
  const vkClass *entityStateClass = vkClassRegistry::Get()->GetClass(className);
  if (!entityStateClass)
  {
    return 0;
  }

  const vkEntityStateLoader *loader = vkEntityLoaderRegistry::Get()->GetEntityStateLoader(entityStateClass);
  if (!loader)
  {
    return 0;
  }

  vkEntityState *entityState = entityStateClass->CreateInstance<vkEntityState>();

  return loader->Load(element, locator, entityState);
}





vkEntityStateLoader::vkEntityStateLoader()
  : vkBaseXMLLoader()
{

}


vkEntityStateLoader::~vkEntityStateLoader()
{

}


bool vkEntityStateLoader::CanLoad(TiXmlElement *element, const vkResourceLocator &locator, IObject *userData) const
{
  return false;
}


IObject *vkEntityStateLoader::Load(TiXmlElement *element, const vkResourceLocator &locator, IObject *userData) const
{

  return userData;
}

const vkClass *vkEntityStateLoader::GetLoadingClass() const
{
  return vkEntityStateClass::Get();
}






vkEntityLoaderRegistry::vkEntityLoaderRegistry()
{
}

vkEntityLoaderRegistry *vkEntityLoaderRegistry::Get()
{
  static vkEntityLoaderRegistry static_instance;
  return &static_instance;
}


void vkEntityLoaderRegistry::RegisterLoader(const vkEntityLoader *loader)
{
  if (!loader)
  {
    return;
  }

  m_entityLoaders[loader->GetLoadingClass()] = loader;
}


void vkEntityLoaderRegistry::RegisterLoader(const vkEntityStateLoader *loader)
{
  if (!loader)
  {
    return;
  }

  m_entityStateLoaders[loader->GetLoadingClass()] = loader;
}


const vkEntityLoader *vkEntityLoaderRegistry::GetEntityLoader(const vkClass *clazz) const
{
  std::map<const vkClass *, const vkEntityLoader*>::const_iterator it = m_entityLoaders.find(clazz);

  if (it != m_entityLoaders.end())
  {
    return it->second;
  }

  for (size_t i = 0, in = clazz->GetNumberOfSuperClasses(); i < in; ++i)
  {
    const vkClass *clazz1 = clazz->GetSuperClass(i);
    const vkEntityLoader *loader = GetEntityLoader(clazz1);
    if (loader)
    {
      return loader;
    }
  }
  return 0;
}



const vkEntityStateLoader *vkEntityLoaderRegistry::GetEntityStateLoader(const vkClass *clazz) const
{
  std::map<const vkClass *, const vkEntityStateLoader*>::const_iterator it = m_entityStateLoaders.find(clazz);

  if (it != m_entityStateLoaders.end())
  {
    return it->second;
  }

  for (size_t i = 0, in = clazz->GetNumberOfSuperClasses(); i < in; ++i)
  {
    const vkClass *clazz1 = clazz->GetSuperClass(i);
    const vkEntityStateLoader *loader = GetEntityStateLoader(clazz1);
    if (loader)
    {
      return loader;
    }
  }
  return 0;
}
