#include <Valkyrie/loaders/entity/vkstaticmeshstatexmlloader.hh>
#include <Valkyrie/loaders/entity/vkentitystateloaderdata.hh>
#include <Valkyrie/graphics/vkmaterialinstance.hh>
#include <Valkyrie/graphics/vkmesh.hh>
#include <Valkyrie/entity/vkstaticmeshstate.hh>

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


