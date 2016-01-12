
#include <Valkyrie/Loaders/EntityLoader.hh>
#include <Valkyrie/Entity/Entity.hh>
#include <Valkyrie/Entity/EntityState.hh>
#include <Valkyrie/Entity/LightState.hh>
#include <Valkyrie/Entity/MeshState.hh>
#include <Valkyrie/Graphics/Material.hh>
#include <Valkyrie/Graphics/Mesh.hh>
#include <Valkyrie/Core/ClassRegistry.hh>


// ***********************************************************************************************
// ***********************************************************************************************
//
//                   vkEntityMasterLoader
//
// ***********************************************************************************************
// ***********************************************************************************************

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



// ***********************************************************************************************
// ***********************************************************************************************
//
//                   vkEntityStateMasterLoader
//
// ***********************************************************************************************
// ***********************************************************************************************


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



// ***********************************************************************************************
// ***********************************************************************************************
//
//                   vkEntityLoader
//
// ***********************************************************************************************
// ***********************************************************************************************


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







// ***********************************************************************************************
// ***********************************************************************************************
//
//                   vkEntityStateLoader
//
// ***********************************************************************************************
// ***********************************************************************************************




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
  if (!userData || !element)
  {
    return userData;
  }

  vkEntityState *entityState = vkQueryClass<vkEntityState>(userData);
  if (entityState)
  {
    if (element->Attribute("name"))
    {
      entityState->SetName(vkString(element->Attribute("name")));
    }
    else
    {
      entityState->SetName(entityState->GetClass()->GetName());
    }
  }
  return userData;
}

const vkClass *vkEntityStateLoader::GetLoadingClass() const
{
  return vkEntityStateClass::Get();
}




// ***********************************************************************************************
// ***********************************************************************************************
//
//                   vkSpatialStateLoader
//
// ***********************************************************************************************
// ***********************************************************************************************




vkSpatialStateLoader::vkSpatialStateLoader()
  : vkEntityStateLoader()
{

}


vkSpatialStateLoader::~vkSpatialStateLoader()
{

}



IObject *vkSpatialStateLoader::Load(TiXmlElement *element, const vkResourceLocator &locator, IObject *userData) const
{
  if (!userData || !element)
  {
    return userData;
  }

  vkSpatialState *spatialState = vkQueryClass<vkSpatialState>(userData);
  if (spatialState)
  {


    TiXmlElement *transformationElement = element->FirstChildElement("transformation");
    if (transformationElement)
    {
      vkTransformation trans = spatialState->GetTransformation();

      TiXmlElement *translationElement = transformationElement->FirstChildElement("translation");
      if (translationElement)
      {
        const char *txt = translationElement->GetText();
        if (txt)
        {
          vkVector3f translation = LoadVector3f(txt);
          trans.SetTranslation(translation);
        }
      }

      TiXmlElement *axisAngleElement = transformationElement->FirstChildElement("axisAngle");
      if (axisAngleElement)
      {
        const char *txt = axisAngleElement->GetText();
        if (txt)
        {
          vkVector4f axisAngle = LoadVector4f(txt);
          trans.SetRotation(axisAngle.AsVector3f(), axisAngle.w);
        }
      }

    }
  }
  return vkEntityStateLoader::Load(element, locator, userData);
}

const vkClass *vkSpatialStateLoader::GetLoadingClass() const
{
  return vkSpatialStateClass::Get();
}




// ***********************************************************************************************
// ***********************************************************************************************
//
//                   vkRenderStateLoader
//
// ***********************************************************************************************
// ***********************************************************************************************




vkRenderStateLoader::vkRenderStateLoader()
  : vkSpatialStateLoader()
{

}


vkRenderStateLoader::~vkRenderStateLoader()
{

}


IObject *vkRenderStateLoader::Load(TiXmlElement *element, const vkResourceLocator &locator, IObject *userData) const
{
  vkRenderState *renderState = vkQueryClass<vkRenderState>(userData);
  if (!renderState)
  {
    return userData;
  }
  TiXmlElement *fadeOutElement = element->FirstChildElement("fadeOut");
  if (fadeOutElement && fadeOutElement->Attribute("near") && fadeOutElement->Attribute("far"))
  {
    float near = atof(fadeOutElement->Attribute("near"));
    float far= atof(fadeOutElement->Attribute("far"));
    renderState->SetFadeOut(near, far);
  }
  return vkSpatialStateLoader::Load(element, locator, userData);
}

const vkClass *vkRenderStateLoader::GetLoadingClass() const
{
  return vkRenderStateClass::Get();
}




// ***********************************************************************************************
// ***********************************************************************************************
//
//                   vkStaticMeshStateLoader
//
// ***********************************************************************************************
// ***********************************************************************************************




vkStaticMeshStateLoader::vkStaticMeshStateLoader()
  : vkRenderStateLoader()
{

}


vkStaticMeshStateLoader::~vkStaticMeshStateLoader()
{

}


IObject *vkStaticMeshStateLoader::Load(TiXmlElement *element, const vkResourceLocator &locator, IObject *userData) const
{
  if (!userData || !element)
  {
    return userData;
  }

  vkStaticMeshState *staticMeshState = vkQueryClass<vkStaticMeshState>(userData);
  if (staticMeshState)
  {
    TiXmlElement *meshElement = element->FirstChildElement("mesh");
    if (meshElement && meshElement->GetText())
    {
      const char *txt = meshElement->GetText();
      vkResourceLoadingMode loadingMode = GetResourceLoadingMode(meshElement, eRLM_Shared, eRLM_Instance);
      vkMesh *mesh = vkResourceManager::Get()->Aquire<vkMesh>(vkResourceLocator(vkString(txt)), 0, loadingMode);
      staticMeshState->SetMesh(mesh);
    }
    TiXmlElement *materialsElement = element->FirstChildElement("materials");
    if (materialsElement)
    {
      for (TiXmlElement *materialElement = materialsElement->FirstChildElement("material");
      materialElement;
        materialElement = materialElement->NextSiblingElement("material"))
      {
        const char *txt = materialElement->GetText();
        vkResourceLoadingMode loadingMode = GetResourceLoadingMode(materialElement, eRLM_Shared, eRLM_Instance);
        vkMaterialInstance *material = vkResourceManager::Get()->Aquire<vkMaterialInstance>(vkResourceLocator(vkString(txt)), 0, loadingMode);
        int slot = 0;
        if (materialElement->Attribute("slot"))
        {
          slot = atoi(materialElement->Attribute("slot"));
        }
        if (materialElement->Attribute("slotName"))
        {
          vkString slotName(materialElement->Attribute("slotName"));
          vkUInt32 meshSlot = staticMeshState->GetMesh()->GetMaterialIndex(slotName);
          if (meshSlot != vkInvalidMaterialIndex)
          {
            slot = meshSlot;
          }
        }
        staticMeshState->SetMaterial(material, slot);
      }
    }
    else
    {
      TiXmlElement *materialElement = element->FirstChildElement("material");
      if (materialElement)
      {
        const char *txt = materialElement->GetText();
        vkResourceLoadingMode loadingMode = GetResourceLoadingMode(materialElement, eRLM_Shared, eRLM_Instance);
        vkMaterialInstance *material = vkResourceManager::Get()->Aquire<vkMaterialInstance>(vkResourceLocator(vkString(txt)), 0, loadingMode);
        staticMeshState->SetMaterial(material, 0);
      }
    }


  }

  return vkRenderStateLoader::Load(element, locator, userData);
}

const vkClass *vkStaticMeshStateLoader::GetLoadingClass() const
{
  return vkStaticMeshStateClass::Get();
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