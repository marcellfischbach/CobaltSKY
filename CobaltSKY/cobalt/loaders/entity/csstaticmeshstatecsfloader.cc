#include <cobalt/loaders/entity/csstaticmeshstatecsfloader.hh>
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




csStaticMeshStateCSFLoader::csStaticMeshStateCSFLoader()
  : csRenderStateCSFLoader()
{

}


csStaticMeshStateCSFLoader::~csStaticMeshStateCSFLoader()
{

}

const csClass *csStaticMeshStateCSFLoader::EvalClass(const csfEntry *entry, const csResourceLocator &locator, iObject *userData) const
{
  return csStaticMeshState::GetStaticClass();
}


iObject *csStaticMeshStateCSFLoader::Load(const csfEntry *entry, const csResourceLocator &locator, iObject *userData) const
{
  if (!userData || !entry)
  {
    return userData;
  }

  csEntityStateLoaderData *data = csQueryClass<csEntityStateLoaderData>(userData);
  csStaticMeshState *staticMeshState = csQueryClass<csStaticMeshState>(data->state);
  if (staticMeshState)
  {
    const csfEntry *meshEntry = entry->GetEntry("mesh");
    if (meshEntry)
    {
      csMesh *mesh = 0;
      if (meshEntry->HasAttribute())
      {
        csResourceLoadingMode loadingMode = GetResourceLoadingMode(meshEntry, eRLM_Shared, eRLM_Instance);
        mesh = csResourceManager::Get()->Aquire<csMesh>(csResourceLocator(meshEntry->GetAttribute()), 0, loadingMode);
      }
      else
      {
        mesh = csResourceManager::Get()->Load<csMesh>(meshEntry, locator, userData);
      }
      staticMeshState->SetMesh(mesh);
    }

    const csfEntry *materialsEntry = entry->GetEntry("materials");
    if (materialsEntry)
    {
      for (const csfEntry *materialEntry = materialsEntry->GetEntry("material");
        materialEntry;
        materialEntry = materialEntry->GetSiblingEntry("material"))
      {
        csResourceLoadingMode loadingMode = GetResourceLoadingMode(materialEntry, eRLM_Shared, eRLM_Instance);
        csMaterial *material = csResourceManager::Get()->Aquire<csMaterial>(csResourceLocator(materialEntry->GetAttribute()), 0, loadingMode);
        int slot = 0;
        if (materialEntry->HasAttribute("slot"))
        {
          slot = materialEntry->GetAttributeInt("slot");
        }
        if (materialEntry->HasAttribute("slotName"))
        {
          std::string slotName = materialEntry->GetAttribute("slotName");
          csUInt32 meshSlot = staticMeshState->GetMesh()->GetMaterialIndex(slotName);
          if (meshSlot != csInvalidMaterialIndex)
          {
            slot = meshSlot;
          }
        }
        staticMeshState->SetMaterial(material, slot);
      }
    }
    else
    {
      const csfEntry *materialEntry = entry->GetEntry("material");
      if (materialEntry)
      {
        csResourceLoadingMode loadingMode = GetResourceLoadingMode(materialEntry, eRLM_Shared, eRLM_Instance);
        csMaterial *material = csResourceManager::Get()->Aquire<csMaterial>(csResourceLocator(materialEntry->GetAttribute()), 0, loadingMode);
        staticMeshState->SetMaterial(material, 0);
      }
    }

    const csfEntry *colliderEntry = entry->GetEntry("collider");
    if (colliderEntry)
    {
      const csfEntry *shapesEntry = colliderEntry->GetEntry("shapes");
      if (shapesEntry)
      {
        csPhysicsShapeContainer *shapes = 0;
        if (shapesEntry->HasAttribute())
        {
          csResourceLoadingMode loadingMode = GetResourceLoadingMode(shapesEntry, eRLM_Shared, eRLM_Instance);
          shapes = csResourceManager::Get()->Aquire<csPhysicsShapeContainer>(csResourceLocator(shapesEntry->GetAttribute()), 0, loadingMode);
        }
        else
        {
          shapes = csResourceManager::Get()->Load<csPhysicsShapeContainer>(shapesEntry, locator, userData);
        }
        staticMeshState->SetColliderShape(shapes);
        const csfEntry *frictionEntry = colliderEntry->GetEntry("friction");
        if (frictionEntry)
        {
          staticMeshState->SetFriction(LoadFloat(frictionEntry));
        }
        const csfEntry *restitutionEntry = colliderEntry->GetEntry("restitution");
        if (restitutionEntry)
        {
          staticMeshState->SetRestitution(LoadFloat(restitutionEntry));
        }
      }

    }


  }

  return csRenderStateCSFLoader::Load(entry, locator, userData);
}

const csClass *csStaticMeshStateCSFLoader::GetLoadingClass() const
{
  return csStaticMeshState::GetStaticClass();
}


