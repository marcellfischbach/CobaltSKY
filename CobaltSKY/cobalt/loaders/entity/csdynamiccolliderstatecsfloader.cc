#include <cobalt/loaders/entity/csdynamiccolliderstatecsfloader.hh>
#include <cobalt/loaders/entity/csentitystateloaderdata.hh>
#include <cobalt/entity/csdynamiccolliderstate.hh>


csDynamicColliderStateCSFLoader::csDynamicColliderStateCSFLoader()
  : csBaseColliderStateCSFLoader()
{

}


csDynamicColliderStateCSFLoader::~csDynamicColliderStateCSFLoader()
{

}


const csClass *csDynamicColliderStateCSFLoader::EvalClass(const csfEntry *entry, const csResourceLocator &locator, iObject *userData) const
{
  return csDynamicColliderState::GetStaticClass();
}

iObject *csDynamicColliderStateCSFLoader::Load(const csfEntry *entry, const csResourceLocator &locator, iObject *userData) const
{
  if (!userData || !entry)
  {
    return userData;
  }

  csEntityStateLoaderData *data = csQueryClass<csEntityStateLoaderData>(userData);
  csDynamicColliderState *dynCollider = csQueryClass<csDynamicColliderState>(data->state);
  if (dynCollider)
  {
    const csfEntry *massElement = entry->GetEntry("mass");
    if (massElement)
    {
      float mass = LoadFloat(massElement);
      dynCollider->SetMass(mass);
    }

    const csfEntry *inertiaElement = entry->GetEntry("inertia");
    if (inertiaElement)
    {
      csVector3f inertia = LoadVector3f(inertiaElement);
      dynCollider->SetInertia(inertia);
    }
    else
    {
      dynCollider->SetAutoInertia(true);
    }

    const csfEntry *kinematicElement = entry->GetEntry("kinematic");
    if (kinematicElement)
    {
      bool kinematic = LoadBool(kinematicElement);
      dynCollider->SetKinematic(kinematic);
    }
  }

  return csBaseColliderStateCSFLoader::Load(entry, locator, userData);
}

const csClass *csDynamicColliderStateCSFLoader::GetLoadingClass() const
{
  return csDynamicColliderState::GetStaticClass();
}

