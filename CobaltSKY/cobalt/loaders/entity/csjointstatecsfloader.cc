#include <cobalt/loaders/entity/csjointstatecsfloader.hh>
#include <cobalt/loaders/entity/csentitystateloaderdata.hh>
#include <cobalt/entity/csjointstate.hh>
#include <cobalt/entity/csentity.hh>
#include <cobalt/entity/csdynamiccolliderstate.hh>


csJointStateCSFLoader::csJointStateCSFLoader()
  : csSpatialStateCSFLoader()
{

}

csJointStateCSFLoader::~csJointStateCSFLoader()
{

}


const csClass *csJointStateCSFLoader::EvalClass(const csfEntry *entry, const csResourceLocator &locator, iObject *userData) const
{
  return csJointState::GetStaticClass();
}

iObject *csJointStateCSFLoader::Load(const csfEntry *entry, const csResourceLocator &locator, iObject *userData) const
{
  csEntityStateLoaderData *data = csQueryClass<csEntityStateLoaderData>(userData);
  csJointState *jointState = csQueryClass<csJointState>(data->state);
  if (jointState)
  {
    const csfEntry *colliderAEntry = entry->GetEntry("colliderA");
    if (colliderAEntry && colliderAEntry->HasAttribute("id"))
    {
      csID id = static_cast<csID>(colliderAEntry->GetAttributeLong("id"));
      csEntityState *state = data->entity->GetState(id);
      csDynamicColliderState *dynState = csQueryClass<csDynamicColliderState>(state);
      if (dynState)
      {
        jointState->SetColliderA(dynState);
      }
    }
    const csfEntry *colliderBEntry = entry->GetEntry("colliderB");
    if (colliderBEntry && colliderBEntry->HasAttribute("id"))
    {
      csID id = static_cast<csID>(colliderBEntry->GetAttributeLong("id"));
      csEntityState *state = data->entity->GetState(id);
      csDynamicColliderState *dynState = csQueryClass<csDynamicColliderState>(state);
      if (dynState)
      {
        jointState->SetColliderB(dynState);
      }
    }
  }
  return csSpatialStateCSFLoader::Load(entry, locator, userData);
}

const csClass *csJointStateCSFLoader::GetLoadingClass() const
{
  return csJointState::GetStaticClass();
}


