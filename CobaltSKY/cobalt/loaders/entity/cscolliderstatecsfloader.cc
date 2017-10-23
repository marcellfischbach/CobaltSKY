
#include <cobalt/loaders/entity/cscolliderstatecsfloader.hh>
#include <cobalt/loaders/entity/csentitystateloaderdata.hh>
#include <cobalt/entity/cscolliderstate.hh>
#include <cobalt/physics/csphysicsshapecontainer.hh>





csColliderStateCSFLoader::csColliderStateCSFLoader()
  : csSpatialStateCSFLoader()
{

}


csColliderStateCSFLoader::~csColliderStateCSFLoader()
{

}


const csClass *csColliderStateCSFLoader::EvalClass(const csfEntry *entry, const csResourceLocator &locator, iObject *userData) const
{
  return csColliderState::GetStaticClass();
}

iObject *csColliderStateCSFLoader::Load(const csfEntry *entry, const csResourceLocator &locator, iObject *userData) const
{
  if (!userData || !entry)
  {
    return userData;
  }

  csEntityStateLoaderData *data = csQueryClass<csEntityStateLoaderData>(userData);
  csColliderState *collider = csQueryClass<csColliderState>(data->state);
  const csfEntry *shapeEntry = entry->GetEntry("shape");
  if (shapeEntry)
  {
    csResourceLoadingMode loadingMode = GetResourceLoadingMode(shapeEntry, eRLM_Shared, eRLM_Instance);
    csPhysicsShapeContainer *shapes = csResourceManager::Get()->Aquire<csPhysicsShapeContainer>(csResourceLocator(shapeEntry->GetAttribute()), 0, loadingMode);
    collider->AttachShape(shapes);
  }

  return csSpatialStateCSFLoader::Load(entry, locator, userData);
}

const csClass *csColliderStateCSFLoader::GetLoadingClass() const
{
  return csColliderState::GetStaticClass();
}

