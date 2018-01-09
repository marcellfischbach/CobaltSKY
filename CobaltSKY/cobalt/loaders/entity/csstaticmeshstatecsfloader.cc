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
      if (HasLocator(meshEntry))
      {
        csResourceLoadingMode loadingMode = GetResourceLoadingMode(meshEntry, eRLM_Shared, eRLM_Instance);
        mesh = csResourceManager::Get()->Aquire<csMesh>(GetLocator(meshEntry), 0, loadingMode);
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
        if (HasLocator(materialEntry))
        {
          csResourceLoadingMode loadingMode = GetResourceLoadingMode(materialEntry, eRLM_Shared, eRLM_Instance);
          csMaterial *material = csResourceManager::Get()->Aquire<csMaterial>(GetLocator(materialEntry), 0, loadingMode);
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
          staticMeshState->SetMaterial(slot, material);
        }
      }
    }
    else
    {
      const csfEntry *materialEntry = entry->GetEntry("material");
      if (materialEntry && HasLocator(materialEntry))
      {
        csResourceLoadingMode loadingMode = GetResourceLoadingMode(materialEntry, eRLM_Shared, eRLM_Instance);
        csMaterial *material = csResourceManager::Get()->Aquire<csMaterial>(GetLocator(materialEntry), 0, loadingMode);
        staticMeshState->SetMaterial(0, material);
      }
    }

    const csfEntry *colliderEntry = entry->GetEntry("collider");
    if (colliderEntry)
    {
      const csfEntry *shapesEntry = colliderEntry->GetEntry("shapes");
      if (shapesEntry)
      {
        csPhysicsShapeContainer *shapes = 0;
        if (HasLocator(shapesEntry))
        {
          csResourceLoadingMode loadingMode = GetResourceLoadingMode(shapesEntry, eRLM_Shared, eRLM_Instance);
          shapes = csResourceManager::Get()->Aquire<csPhysicsShapeContainer>(GetLocator(shapesEntry), 0, loadingMode);
        }
        else
        {
          shapes = csResourceManager::Get()->Load<csPhysicsShapeContainer>(shapesEntry, locator, userData);
        }
        for (csSize i=0, in=shapes->GetNumberOfShapes(); i<in; ++i)
        {
          staticMeshState->AddColliderShape(shapes->GetShape(i));
        }
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


