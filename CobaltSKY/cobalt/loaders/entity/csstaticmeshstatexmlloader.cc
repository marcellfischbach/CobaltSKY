#include <cobalt/loaders/entity/csstaticmeshstatexmlloader.hh>
#include <cobalt/loaders/entity/csentitystateloaderdata.hh>
#include <cobalt/graphics/csmaterial.hh>
#include <cobalt/graphics/csmesh.hh>
#include <cobalt/entity/csstaticmeshstate.hh>

// ***********************************************************************************************
// ***********************************************************************************************
//
//                   csStaticMeshStateLoader
//
// ***********************************************************************************************
// ***********************************************************************************************




csStaticMeshStateXMLLoader::csStaticMeshStateXMLLoader()
  : csRenderStateXMLLoader()
{

}


csStaticMeshStateXMLLoader::~csStaticMeshStateXMLLoader()
{

}

const csClass *csStaticMeshStateXMLLoader::EvalClass(TiXmlElement *element, const csResourceLocator &locator, iObject *userData) const
{
  return csStaticMeshState::GetStaticClass();
}


iObject *csStaticMeshStateXMLLoader::Load(TiXmlElement *element, const csResourceLocator &locator, iObject *userData) const
{
  if (!userData || !element)
  {
    return userData;
  }

  csEntityStateLoaderData *data = csQueryClass<csEntityStateLoaderData>(userData);
  csStaticMeshState *staticMeshState = csQueryClass<csStaticMeshState>(data->state);
  if (staticMeshState)
  {
    TiXmlElement *meshElement = element->FirstChildElement("mesh");
    if (meshElement)
    {
      csMesh *mesh = 0;
      if (meshElement->GetText())
      {
        const char *txt = meshElement->GetText();
        csResourceLoadingMode loadingMode = GetResourceLoadingMode(meshElement, eRLM_Shared, eRLM_Instance);
        mesh = csResourceManager::Get()->Aquire<csMesh>(csResourceLocator(std::string(txt)), 0, loadingMode);
      }
      else
      {
        mesh = csResourceManager::Get()->Load<csMesh>(meshElement, locator, userData);
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
        csResourceLoadingMode loadingMode = GetResourceLoadingMode(materialElement, eRLM_Shared, eRLM_Instance);
        csMaterial *material = csResourceManager::Get()->Aquire<csMaterial>(csResourceLocator(std::string(txt)), 0, loadingMode);
        int slot = 0;
        if (materialElement->Attribute("slot"))
        {
          slot = atoi(materialElement->Attribute("slot"));
        }
        if (materialElement->Attribute("slotName"))
        {
          std::string slotName(materialElement->Attribute("slotName"));
          csUInt32 meshSlot = staticMeshState->GetMesh()->GetMaterialIndex(slotName);
          if (meshSlot != csInvalidMaterialIndex)
          {
            slot = meshSlot;
          }
        }
        staticMeshState->SetMaterial(slot, material);
      }
    }
    else
    {
      TiXmlElement *materialElement = element->FirstChildElement("material");
      if (materialElement)
      {
        const char *txt = materialElement->GetText();
        csResourceLoadingMode loadingMode = GetResourceLoadingMode(materialElement, eRLM_Shared, eRLM_Instance);
        csMaterial *material = csResourceManager::Get()->Aquire<csMaterial>(csResourceLocator(std::string(txt)), 0, loadingMode);
        staticMeshState->SetMaterial(0, material);
      }
    }

    TiXmlElement *colliderElement = element->FirstChildElement("collider");
    if (colliderElement)
    {
      TiXmlElement *shapesElement = colliderElement->FirstChildElement("shapes");
      if (shapesElement)
      {
        csPhysicsShapeContainer *shapes = 0;
        if (shapesElement->GetText())
        {
          const char *txt = shapesElement->GetText();
          csResourceLoadingMode loadingMode = GetResourceLoadingMode(shapesElement, eRLM_Shared, eRLM_Instance);
          shapes = csResourceManager::Get()->Aquire<csPhysicsShapeContainer>(csResourceLocator(std::string(txt)), 0, loadingMode);
        }
        else
        {
          shapes = csResourceManager::Get()->Load<csPhysicsShapeContainer>(shapesElement, locator, userData);
        }
        for (csSize i=0, in=shapes->GetNumberOfShapes(); i<in; ++i)
        {
          staticMeshState->AddColliderShape(shapes->GetShape(i));
        }
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

  return csRenderStateXMLLoader::Load(element, locator, userData);
}

const csClass *csStaticMeshStateXMLLoader::GetLoadingClass() const
{
  return csStaticMeshState::GetStaticClass();
}


