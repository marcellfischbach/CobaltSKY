
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
#include <Valkyrie/Core/vkclassregistry.hh>


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



// ***********************************************************************************************
// ***********************************************************************************************
//
//                   vkEntityLoader
//
// ***********************************************************************************************
// ***********************************************************************************************


vkEntityXMLLoader::vkEntityXMLLoader()
  : vkBaseXMLLoader()
{

}


vkEntityXMLLoader::~vkEntityXMLLoader()
{

}


bool vkEntityXMLLoader::CanLoad(TiXmlElement *element, const vkResourceLocator &locator, IObject *userData) const
{
  return userData != 0;
}

const vkClass *vkEntityXMLLoader::EvalClass(TiXmlElement *element, const vkResourceLocator &locator, IObject *userData) const
{
  return vkEntity::GetStaticClass();
}

IObject *vkEntityXMLLoader::Load(TiXmlElement *element, const vkResourceLocator &locator, IObject *userData) const
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

const vkClass *vkEntityXMLLoader::GetLoadingClass() const
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




vkEntityStateXMLLoader::vkEntityStateXMLLoader()
  : vkBaseXMLLoader()
{

}


vkEntityStateXMLLoader::~vkEntityStateXMLLoader()
{

}


bool vkEntityStateXMLLoader::CanLoad(TiXmlElement *element, const vkResourceLocator &locator, IObject *userData) const
{
  return false;
}


const vkClass *vkEntityStateXMLLoader::EvalClass(TiXmlElement *element, const vkResourceLocator &locator, IObject *userData) const
{
  return vkEntityState::GetStaticClass();
}

IObject *vkEntityStateXMLLoader::Load(TiXmlElement *element, const vkResourceLocator &locator, IObject *userData) const
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

const vkClass *vkEntityStateXMLLoader::GetLoadingClass() const
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




vkSpatialStateXMLLoader::vkSpatialStateXMLLoader()
  : vkEntityStateXMLLoader()
{

}


vkSpatialStateXMLLoader::~vkSpatialStateXMLLoader()
{

}


const vkClass *vkSpatialStateXMLLoader::EvalClass(TiXmlElement *element, const vkResourceLocator &locator, IObject *userData) const
{
  return vkSpatialState::GetStaticClass();
}


IObject *vkSpatialStateXMLLoader::Load(TiXmlElement *element, const vkResourceLocator &locator, IObject *userData) const
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

  return vkEntityStateXMLLoader::Load(element, locator, userData);
}

const vkClass *vkSpatialStateXMLLoader::GetLoadingClass() const
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




vkRenderStateXMLLoader::vkRenderStateXMLLoader()
  : vkSpatialStateXMLLoader()
{

}


vkRenderStateXMLLoader::~vkRenderStateXMLLoader()
{

}

const vkClass *vkRenderStateXMLLoader::EvalClass(TiXmlElement *element, const vkResourceLocator &locator, IObject *userData) const
{
  return vkRenderState::GetStaticClass();
}


IObject *vkRenderStateXMLLoader::Load(TiXmlElement *element, const vkResourceLocator &locator, IObject *userData) const
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
    float near = (float)atof(fadeOutElement->Attribute("near"));
    float far = (float)atof(fadeOutElement->Attribute("far"));
    renderState->SetFadeOut(near, far);
  }
  return vkSpatialStateXMLLoader::Load(element, locator, userData);
}

const vkClass *vkRenderStateXMLLoader::GetLoadingClass() const
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




vkStaticMeshStateXMLLoader::vkStaticMeshStateXMLLoader()
  : vkRenderStateXMLLoader()
{

}


vkStaticMeshStateXMLLoader::~vkStaticMeshStateXMLLoader()
{

}

const vkClass *vkStaticMeshStateXMLLoader::EvalClass(TiXmlElement *element, const vkResourceLocator &locator, IObject *userData) const
{
  return vkStaticMeshState::GetStaticClass();
}


IObject *vkStaticMeshStateXMLLoader::Load(TiXmlElement *element, const vkResourceLocator &locator, IObject *userData) const
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
    if (meshElement)
    {
      vkMesh *mesh = 0;
      if (meshElement->GetText())
      {
        const char *txt = meshElement->GetText();
        vkResourceLoadingMode loadingMode = GetResourceLoadingMode(meshElement, eRLM_Shared, eRLM_Instance);
        mesh = vkResourceManager::Get()->Aquire<vkMesh>(vkResourceLocator(vkString(txt)), 0, loadingMode);
      }
      else
      {
        mesh = vkResourceManager::Get()->Load<vkMesh>(meshElement, locator, userData);
      }
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
      TiXmlElement *shapesElement = colliderElement->FirstChildElement("shapes");
      if (shapesElement)
      {
        vkPhysicsShapeContainer *shapes = 0;
        if (shapesElement->GetText())
        {
          const char *txt = shapesElement->GetText();
          vkResourceLoadingMode loadingMode = GetResourceLoadingMode(shapesElement, eRLM_Shared, eRLM_Instance);
          shapes = vkResourceManager::Get()->Aquire<vkPhysicsShapeContainer>(vkResourceLocator(vkString(txt)), 0, loadingMode);
        }
        else
        {
          shapes = vkResourceManager::Get()->Load<vkPhysicsShapeContainer>(shapesElement, locator, userData);
        }
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

  return vkRenderStateXMLLoader::Load(element, locator, userData);
}

const vkClass *vkStaticMeshStateXMLLoader::GetLoadingClass() const
{
  return vkStaticMeshState::GetStaticClass();
}








vkColliderStateXMLLoader::vkColliderStateXMLLoader()
  : vkSpatialStateXMLLoader()
{

}


vkColliderStateXMLLoader::~vkColliderStateXMLLoader()
{

}


const vkClass *vkColliderStateXMLLoader::EvalClass(TiXmlElement *element, const vkResourceLocator &locator, IObject *userData) const
{
  return vkColliderState::GetStaticClass();
}

IObject *vkColliderStateXMLLoader::Load(TiXmlElement *element, const vkResourceLocator &locator, IObject *userData) const
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

  return vkColliderStateXMLLoader::Load(element, locator, userData);
}

const vkClass *vkColliderStateXMLLoader::GetLoadingClass() const
{
  return vkColliderState::GetStaticClass();
}




vkBaseColliderStateXMLLoader::vkBaseColliderStateXMLLoader()
  : vkColliderStateXMLLoader()
{

}


vkBaseColliderStateXMLLoader::~vkBaseColliderStateXMLLoader()
{

}

const vkClass *vkBaseColliderStateXMLLoader::EvalClass(TiXmlElement *element, const vkResourceLocator &locator, IObject *userData) const
{
  return vkBaseColliderState::GetStaticClass();
}


IObject *vkBaseColliderStateXMLLoader::Load(TiXmlElement *element, const vkResourceLocator &locator, IObject *userData) const
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
  

  return vkColliderStateXMLLoader::Load(element, locator, userData);
}

const vkClass *vkBaseColliderStateXMLLoader::GetLoadingClass() const
{
  return vkBaseColliderState::GetStaticClass();
}



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




vkDynamicColliderStateXMLLoader::vkDynamicColliderStateXMLLoader()
  : vkBaseColliderStateXMLLoader()
{

}


vkDynamicColliderStateXMLLoader::~vkDynamicColliderStateXMLLoader()
{

}


const vkClass *vkDynamicColliderStateXMLLoader::EvalClass(TiXmlElement *element, const vkResourceLocator &locator, IObject *userData) const
{
  return vkDynamicColliderState::GetStaticClass();
}

IObject *vkDynamicColliderStateXMLLoader::Load(TiXmlElement *element, const vkResourceLocator &locator, IObject *userData) const
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

  return vkBaseColliderStateXMLLoader::Load(element, locator, userData);
}

const vkClass *vkDynamicColliderStateXMLLoader::GetLoadingClass() const
{
  return vkDynamicColliderState::GetStaticClass();
}




vkJointStateXMLLoader::vkJointStateXMLLoader()
  : vkSpatialStateXMLLoader()
{

}

vkJointStateXMLLoader::~vkJointStateXMLLoader()
{

}


const vkClass *vkJointStateXMLLoader::EvalClass(TiXmlElement *element, const vkResourceLocator &locator, IObject *userData) const
{
  return vkJointState::GetStaticClass();
}

IObject *vkJointStateXMLLoader::Load(TiXmlElement *element, const vkResourceLocator &locator, IObject *userData) const
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
  return vkSpatialStateXMLLoader::Load(element, locator, userData);
}

const vkClass *vkJointStateXMLLoader::GetLoadingClass() const
{
  return vkJointState::GetStaticClass();
}




vkHingeJointStateXMLLoader::vkHingeJointStateXMLLoader()
  : vkJointStateXMLLoader()
{

}

vkHingeJointStateXMLLoader::~vkHingeJointStateXMLLoader()
{

}

const vkClass *vkHingeJointStateXMLLoader::EvalClass(TiXmlElement *element, const vkResourceLocator &locator, IObject *userData) const
{
  return vkHingeJointState::GetStaticClass();
}


IObject *vkHingeJointStateXMLLoader::Load(TiXmlElement *element, const vkResourceLocator &locator, IObject *userData) const
{
  return vkJointStateXMLLoader::Load(element, locator, userData);
}

const vkClass *vkHingeJointStateXMLLoader::GetLoadingClass() const
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


void vkEntityLoaderRegistry::RegisterLoader(const vkEntityXMLLoader *loader)
{
  if (!loader)
  {
    return;
  }

  m_entityLoaders[loader->GetLoadingClass()] = loader;
}


void vkEntityLoaderRegistry::RegisterLoader(const vkEntityStateXMLLoader *loader)
{
  if (!loader)
  {
    return;
  }

  m_entityStateLoaders[loader->GetLoadingClass()] = loader;
}


const vkEntityXMLLoader *vkEntityLoaderRegistry::GetEntityLoader(const vkClass *clazz) const
{
  std::map<const vkClass *, const vkEntityXMLLoader*>::const_iterator it = m_entityLoaders.find(clazz);

  if (it != m_entityLoaders.end())
  {
    return it->second;
  }

  for (size_t i = 0, in = clazz->GetNumberOfSuperClasses(); i < in; ++i)
  {
    const vkClass *clazz1 = clazz->GetSuperClass(i);
    const vkEntityXMLLoader *loader = GetEntityLoader(clazz1);
    if (loader)
    {
      return loader;
    }
  }
  return 0;
}



const vkEntityStateXMLLoader *vkEntityLoaderRegistry::GetEntityStateLoader(const vkClass *clazz) const
{
  std::map<const vkClass *, const vkEntityStateXMLLoader*>::const_iterator it = m_entityStateLoaders.find(clazz);

  if (it != m_entityStateLoaders.end())
  {
    return it->second;
  }

  for (size_t i = 0, in = clazz->GetNumberOfSuperClasses(); i < in; ++i)
  {
    const vkClass *clazz1 = clazz->GetSuperClass(i);
    const vkEntityStateXMLLoader *loader = GetEntityStateLoader(clazz1);
    if (loader)
    {
      return loader;
    }
  }
  return 0;
}
