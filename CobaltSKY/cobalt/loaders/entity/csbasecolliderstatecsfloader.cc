
#include <cobalt/loaders/entity/csbasecolliderstatecsfloader.hh>
#include <cobalt/loaders/entity/csentitystateloaderdata.hh>
#include <cobalt/entity/csbasecolliderstate.hh>



csBaseColliderStateCSFLoader::csBaseColliderStateCSFLoader()
  : csColliderStateCSFLoader()
{

}


csBaseColliderStateCSFLoader::~csBaseColliderStateCSFLoader()
{

}

const csClass *csBaseColliderStateCSFLoader::EvalClass(const csfEntry *entry, const csResourceLocator &locator, iObject *userData) const
{
  return csBaseColliderState::GetStaticClass();
}


iObject *csBaseColliderStateCSFLoader::Load(const csfEntry *entry, const csResourceLocator &locator, iObject *userData) const
{
  if (!userData || !entry)
  {
    return userData;
  }

  csEntityStateLoaderData *data = csQueryClass<csEntityStateLoaderData>(userData);
  csBaseColliderState *baseCollider = csQueryClass<csBaseColliderState>(data->state);
  if (baseCollider)
  {
    const csfEntry *frictionEntry = entry->GetEntry("friction");
    if (frictionEntry)
    {
      baseCollider->SetFriction(LoadFloat(frictionEntry));
    }
    const csfEntry *restitutionEntry = entry->GetEntry("restitution");
    if (restitutionEntry)
    {
      baseCollider->SetRestitution(LoadFloat(restitutionEntry));
    }
  }


  return csColliderStateCSFLoader::Load(entry, locator, userData);
}

const csClass *csBaseColliderStateCSFLoader::GetLoadingClass() const
{
  return csBaseColliderState::GetStaticClass();
}


