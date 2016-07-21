
#include <Valkyrie/Loaders/EntityLoader.hh>
#include <Valkyrie/Entity/Entity.hh>
#include <Valkyrie/Entity/EntityState.hh>
#include <Valkyrie/Entity/JointState.hh>
#include <Valkyrie/Entity/LightState.hh>
#include <Valkyrie/Entity/MeshState.hh>
#include <Valkyrie/Entity/ColliderState.hh>
#include <Valkyrie/Physics/PhysicsShapeContainer.hh>
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


const vkClass *vkEntityMasterLoader::EvalClass(TiXmlElement *element, const vkResourceLocator &locator, IObject *userData) const
{
  if (!element->Attribute("class"))
  {
    return 0;
  }
  vkString className(element->Attribute("class"));
  return vkClassRegistry::Get()->GetClass(className);
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

  IObject *obj = loader->Load(element, locator, entity);
  //entity->Create();
  return obj;

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

const vkClass *vkEntityStateMasterLoader::EvalClass(TiXmlElement *element, const vkResourceLocator &locator, IObject *userData) const
{
  if (!element->Attribute("class"))
  {
    return 0;
  }
  vkString className(element->Attribute("class"));
  return vkClassRegistry::Get()->GetClass(className);
}


IObject *vkEntityStateMasterLoader::Load(TiXmlElement *element, const vkResourceLocator &locator, IObject *userData) const
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

  const vkEntityStateLoader *loader = vkEntityLoaderRegistry::Get()->GetEntityStateLoader(entityStateClass);
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

const vkClass *vkEntityLoader::EvalClass(TiXmlElement *element, const vkResourceLocator &locator, IObject *userData) const
{
  return vkEntity::GetStaticClass();
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

      vkEntityState *entityState = vkResourceManager::Get()->Load<vkEntityState>(stateElement, locator, entity);
      if (entityState)
      {
        bool addedToEntity = false;
        vkSpatialState *spatialState = entityState->ToSpatialState();
        if (spatialState)
        {
          if (stateElement->Attribute("id"))
          {
            spatialStates[vkString(stateElement->Attribute("id"))] = spatialState;
          }

          if (stateElement->Attribute("parentID"))
          {
            vkSpatialState *parentState = spatialStates[vkString(stateElement->Attribute("parentID"))];
            if (parentState)
            {
              entity->AddState(spatialState, parentState);
              addedToEntity = true;
            }
          }
        }

        if (!addedToEntity)
        {
          entity->AddState(entityState);
        }
      }
    }
  }

  TiXmlElement *rootElement = element->FirstChildElement("rootState");
  if (rootElement)
  {
    vkSpatialState *rootState = spatialStates[vkString(rootElement->GetText())];
    if (rootState)
    {
      entity->SetRootState(rootState);
    }
  }

  return entity;
}

const vkClass *vkEntityLoader::GetLoadingClass() const
{
  return vkEntity::GetStaticClass();
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


const vkClass *vkEntityStateLoader::EvalClass(TiXmlElement *element, const vkResourceLocator &locator, IObject *userData) const
{
  return vkEntityState::GetStaticClass();
}

IObject *vkEntityStateLoader::Load(TiXmlElement *element, const vkResourceLocator &locator, IObject *userData) const
{
  if (!userData || !element)
  {
    return userData;
  }

  vkEntityStateLoaderData *data = vkQueryClass<vkEntityStateLoaderData>(userData);
  vkEntityState *entityState = vkQueryClass<vkEntityState>(data->state);
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

    if (element->Attribute("id"))
    {
      vkID id = static_cast<vkID>(atoi(element->Attribute("id")));
      entityState->SetID(id);
    }
  }
  return data->state;
}

const vkClass *vkEntityStateLoader::GetLoadingClass() const
{
  return vkEntityState::GetStaticClass();
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


const vkClass *vkSpatialStateLoader::EvalClass(TiXmlElement *element, const vkResourceLocator &locator, IObject *userData) const
{
  return vkSpatialState::GetStaticClass();
}


IObject *vkSpatialStateLoader::Load(TiXmlElement *element, const vkResourceLocator &locator, IObject *userData) const
{
  if (!userData || !element)
  {
    return userData;
  }
  vkEntityStateLoaderData *data = vkQueryClass<vkEntityStateLoaderData>(userData);
  vkSpatialState *spatialState = vkQueryClass<vkSpatialState>(data->state);
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
    TiXmlElement *clippingRangeElement = element->FirstChildElement("clippingRange");
    if (clippingRangeElement)
    {
      float min = -FLT_MAX;
      float max = FLT_MAX;
      if (clippingRangeElement->Attribute("min"))
      {
        min = LoadFloat(clippingRangeElement->Attribute("min"));
      }
      if (clippingRangeElement->Attribute("max"))
      {
        max = LoadFloat(clippingRangeElement->Attribute("max"));
      }
      spatialState->SetClippingRange(min, max);
    }
  }

  return vkEntityStateLoader::Load(element, locator, userData);
}

const vkClass *vkSpatialStateLoader::GetLoadingClass() const
{
  return vkSpatialState::GetStaticClass();
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

const vkClass *vkRenderStateLoader::EvalClass(TiXmlElement *element, const vkResourceLocator &locator, IObject *userData) const
{
  return vkRenderState::GetStaticClass();
}


IObject *vkRenderStateLoader::Load(TiXmlElement *element, const vkResourceLocator &locator, IObject *userData) const
{
  vkEntityStateLoaderData *data = vkQueryClass<vkEntityStateLoaderData>(userData);
  vkRenderState *renderState = vkQueryClass<vkRenderState>(data->state);
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
  return vkRenderState::GetStaticClass();
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

const vkClass *vkStaticMeshStateLoader::EvalClass(TiXmlElement *element, const vkResourceLocator &locator, IObject *userData) const
{
  return vkStaticMeshState::GetStaticClass();
}


IObject *vkStaticMeshStateLoader::Load(TiXmlElement *element, const vkResourceLocator &locator, IObject *userData) const
{
  if (!userData || !element)
  {
    return userData;
  }

  vkEntityStateLoaderData *data = vkQueryClass<vkEntityStateLoaderData>(userData);
  vkStaticMeshState *staticMeshState = vkQueryClass<vkStaticMeshState>(data->state);
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
    TiXmlElement *colliderElement = element->FirstChildElement("collider");
    if (colliderElement)
    {
      TiXmlElement *shapeElement = colliderElement->FirstChildElement("shape");
      if (shapeElement)
      {
        const char *txt = shapeElement->GetText();
        vkResourceLoadingMode loadingMode = GetResourceLoadingMode(shapeElement, eRLM_Shared, eRLM_Instance);
        vkPhysicsShapeContainer *shapes = vkResourceManager::Get()->Aquire<vkPhysicsShapeContainer>(vkResourceLocator(vkString(txt)), 0, loadingMode);
        if (shapes)
        {
          staticMeshState->SetColliderShape(shapes);

          TiXmlElement *frictionElement = colliderElement->FirstChildElement("friction");
          if (frictionElement)
          {
            staticMeshState->SetFriction(LoadFloat(frictionElement->GetText()));
          }
          TiXmlElement *restitutionElement = colliderElement->FirstChildElement("restitution");
          if (restitutionElement)
          {
            staticMeshState->SetRestitution(LoadFloat(restitutionElement->GetText()));
          }
        }
      }

    }


  }

  return vkRenderStateLoader::Load(element, locator, userData);
}

const vkClass *vkStaticMeshStateLoader::GetLoadingClass() const
{
  return vkStaticMeshState::GetStaticClass();
}








vkColliderStateLoader::vkColliderStateLoader()
  : vkSpatialStateLoader()
{

}


vkColliderStateLoader::~vkColliderStateLoader()
{

}


const vkClass *vkColliderStateLoader::EvalClass(TiXmlElement *element, const vkResourceLocator &locator, IObject *userData) const
{
  return vkColliderState::GetStaticClass();
}

IObject *vkColliderStateLoader::Load(TiXmlElement *element, const vkResourceLocator &locator, IObject *userData) const
{
  if (!userData || !element)
  {
    return userData;
  }

  vkEntityStateLoaderData *data = vkQueryClass<vkEntityStateLoaderData>(userData);
  vkColliderState *collider = vkQueryClass<vkColliderState>(data->state);
  TiXmlElement *shapeElement = element->FirstChildElement("shape");
  if (shapeElement)
  {
    const char *txt = shapeElement->GetText();
    vkResourceLoadingMode loadingMode = GetResourceLoadingMode(shapeElement, eRLM_Shared, eRLM_Instance);
    vkPhysicsShapeContainer *shapes = vkResourceManager::Get()->Aquire<vkPhysicsShapeContainer>(vkResourceLocator(vkString(txt)), 0, loadingMode);
    collider->AttachShape(shapes);
  }

  return vkSpatialStateLoader::Load(element, locator, userData);
}

const vkClass *vkColliderStateLoader::GetLoadingClass() const
{
  return vkColliderState::GetStaticClass();
}




vkBaseColliderStateLoader::vkBaseColliderStateLoader()
  : vkColliderStateLoader()
{

}


vkBaseColliderStateLoader::~vkBaseColliderStateLoader()
{

}

const vkClass *vkBaseColliderStateLoader::EvalClass(TiXmlElement *element, const vkResourceLocator &locator, IObject *userData) const
{
  return vkBaseColliderState::GetStaticClass();
}


IObject *vkBaseColliderStateLoader::Load(TiXmlElement *element, const vkResourceLocator &locator, IObject *userData) const
{
  if (!userData || !element)
  {
    return userData;
  }

  vkEntityStateLoaderData *data = vkQueryClass<vkEntityStateLoaderData>(userData);
  vkBaseColliderState *baseCollider = vkQueryClass<vkBaseColliderState>(data->state);
  if (baseCollider)
  {
    TiXmlElement *frictionElement = element->FirstChildElement("friction");
    if (frictionElement)
    { 
      baseCollider->SetFriction(LoadFloat(frictionElement->GetText()));
    }
    TiXmlElement *restitutionElement = element->FirstChildElement("restitution");
    if (restitutionElement)
    {
      baseCollider->SetRestitution(LoadFloat(restitutionElement->GetText()));
    }
  }
  

  return vkColliderStateLoader::Load(element, locator, userData);
}

const vkClass *vkBaseColliderStateLoader::GetLoadingClass() const
{
  return vkBaseColliderState::GetStaticClass();
}



vkStaticColliderStateLoader::vkStaticColliderStateLoader()
  : vkBaseColliderStateLoader()
{

}


vkStaticColliderStateLoader::~vkStaticColliderStateLoader()
{

}

const vkClass *vkStaticColliderStateLoader::EvalClass(TiXmlElement *element, const vkResourceLocator &locator, IObject *userData) const
{
  return vkStaticColliderState::GetStaticClass();
}


IObject *vkStaticColliderStateLoader::Load(TiXmlElement *element, const vkResourceLocator &locator, IObject *userData) const
{
  if (!userData || !element)
  {
    return userData;
  }

  
  return vkBaseColliderStateLoader::Load(element, locator, userData);
}

const vkClass *vkStaticColliderStateLoader::GetLoadingClass() const
{
  return vkStaticColliderState::GetStaticClass();
}




vkDynamicColliderStateLoader::vkDynamicColliderStateLoader()
  : vkBaseColliderStateLoader()
{

}


vkDynamicColliderStateLoader::~vkDynamicColliderStateLoader()
{

}


const vkClass *vkDynamicColliderStateLoader::EvalClass(TiXmlElement *element, const vkResourceLocator &locator, IObject *userData) const
{
  return vkDynamicColliderState::GetStaticClass();
}

IObject *vkDynamicColliderStateLoader::Load(TiXmlElement *element, const vkResourceLocator &locator, IObject *userData) const
{
  if (!userData || !element)
  {
    return userData;
  }

  vkEntityStateLoaderData *data = vkQueryClass<vkEntityStateLoaderData>(userData);
  vkDynamicColliderState *dynCollider = vkQueryClass<vkDynamicColliderState>(data->state);
  if (dynCollider)
  {
    TiXmlElement *massElement = element->FirstChildElement("mass");
    if (massElement)
    {
      float mass = LoadFloat(massElement->GetText());
      dynCollider->SetMass(mass);
    }

    TiXmlElement *inertiaElement = element->FirstChildElement("inertia");
    if (inertiaElement)
    {
      vkVector3f inertia = LoadVector3f(inertiaElement->GetText());
      dynCollider->SetInertia(inertia);
    }
    else
    {
      dynCollider->SetAutoInertia(true);
    }

    TiXmlElement *kinematicElement = element->FirstChildElement("kinematic");
    if (kinematicElement)
    {
      bool kinematic = LoadBool(kinematicElement->GetText());
      dynCollider->SetKinematic(kinematic);
    }
  }

  return vkBaseColliderStateLoader::Load(element, locator, userData);
}

const vkClass *vkDynamicColliderStateLoader::GetLoadingClass() const
{
  return vkDynamicColliderState::GetStaticClass();
}




vkJointStateLoader::vkJointStateLoader()
  : vkSpatialStateLoader()
{

}

vkJointStateLoader::~vkJointStateLoader()
{

}


const vkClass *vkJointStateLoader::EvalClass(TiXmlElement *element, const vkResourceLocator &locator, IObject *userData) const
{
  return vkJointState::GetStaticClass();
}

IObject *vkJointStateLoader::Load(TiXmlElement *element, const vkResourceLocator &locator, IObject *userData) const
{
  vkEntityStateLoaderData *data = vkQueryClass<vkEntityStateLoaderData>(userData);
  vkJointState *jointState = vkQueryClass<vkJointState>(data->state);
  if (jointState)
  {
    TiXmlElement *colliderAElement = element->FirstChildElement("colliderA");
    if (colliderAElement && colliderAElement->Attribute("id"))
    {
      vkID id = static_cast<vkID>(atoi(colliderAElement->Attribute("id")));
      vkEntityState *state = data->entity->GetState(id);
      vkDynamicColliderState *dynState = vkQueryClass<vkDynamicColliderState>(state);
      if (dynState)
      {
        jointState->SetColliderA(dynState);
      }
    }
    TiXmlElement *colliderBElement = element->FirstChildElement("colliderB");
    if (colliderBElement && colliderBElement->Attribute("id"))
    {
      vkID id = static_cast<vkID>(atoi(colliderBElement->Attribute("id")));
      vkEntityState *state = data->entity->GetState(id);
      vkDynamicColliderState *dynState = vkQueryClass<vkDynamicColliderState>(state);
      if (dynState)
      {
        jointState->SetColliderB(dynState);
      }
    }
  }
  return vkSpatialStateLoader::Load(element, locator, userData);
}

const vkClass *vkJointStateLoader::GetLoadingClass() const
{
  return vkJointState::GetStaticClass();
}




vkHingeJointStateLoader::vkHingeJointStateLoader()
  : vkJointStateLoader()
{

}

vkHingeJointStateLoader::~vkHingeJointStateLoader()
{

}

const vkClass *vkHingeJointStateLoader::EvalClass(TiXmlElement *element, const vkResourceLocator &locator, IObject *userData) const
{
  return vkHingeJointState::GetStaticClass();
}


IObject *vkHingeJointStateLoader::Load(TiXmlElement *element, const vkResourceLocator &locator, IObject *userData) const
{
  return vkJointStateLoader::Load(element, locator, userData);
}

const vkClass *vkHingeJointStateLoader::GetLoadingClass() const
{
  return vkHingeJointState::GetStaticClass();
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
