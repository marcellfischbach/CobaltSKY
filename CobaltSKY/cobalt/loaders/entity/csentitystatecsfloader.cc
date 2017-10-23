#include <cobalt/loaders/entity/csentitystatecsfloader.hh>
#include <cobalt/loaders/entity/csentitystateloaderdata.hh>
#include <cobalt/entity/csentity.hh>
#include <cobalt/entity/csentitystate.hh>



// ***********************************************************************************************
// ***********************************************************************************************
//
//                   csEntityStateLoader
//
// ***********************************************************************************************
// ***********************************************************************************************




csEntityStateCSFLoader::csEntityStateCSFLoader()
  : csBaseCSFLoader()
{

}


csEntityStateCSFLoader::~csEntityStateCSFLoader()
{

}


bool csEntityStateCSFLoader::CanLoad(const csfEntry *entry, const csResourceLocator &locator, iObject *userData) const
{
  return false;
}


const csClass *csEntityStateCSFLoader::EvalClass(const csfEntry *entry, const csResourceLocator &locator, iObject *userData) const
{
  return csEntityState::GetStaticClass();
}

iObject *csEntityStateCSFLoader::Load(const csfEntry *entry, const csResourceLocator &locator, iObject *userData) const
{
  if (!userData || !entry)
  {
    return userData;
  }

  csEntityStateLoaderData *data = csQueryClass<csEntityStateLoaderData>(userData);
  csEntityState *entityState = csQueryClass<csEntityState>(data->state);
  if (entityState)
  {
    if (entry->HasAttribute("name"))
    {
      entityState->SetName(entry->GetAttribute("name"));
    }
    else
    {
      entityState->SetName(entityState->GetClass()->GetName());
    }

    if (entry->HasAttribute("id"))
    {
      csID id = static_cast<csID>(entry->GetAttributeInt("id"));
      entityState->SetID(id);
    }
  }
  return data->state;
}

const csClass *csEntityStateCSFLoader::GetLoadingClass() const
{
  return csEntityState::GetStaticClass();
}
