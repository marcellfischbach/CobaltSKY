#include <cobalt/loaders/entity/csentitystatexmlloader.hh>
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




csEntityStateXMLLoader::csEntityStateXMLLoader()
  : csBaseXMLLoader()
{

}


csEntityStateXMLLoader::~csEntityStateXMLLoader()
{

}


bool csEntityStateXMLLoader::CanLoad(TiXmlElement *element, const csResourceLocator &locator, iObject *userData) const
{
  return false;
}


const csClass *csEntityStateXMLLoader::EvalClass(TiXmlElement *element, const csResourceLocator &locator, iObject *userData) const
{
  return csEntityState::GetStaticClass();
}

iObject *csEntityStateXMLLoader::Load(TiXmlElement *element, const csResourceLocator &locator, iObject *userData) const
{
  if (!userData || !element)
  {
    return userData;
  }

  csEntityStateLoaderData *data = csQueryClass<csEntityStateLoaderData>(userData);
  csEntityState *entityState = csQueryClass<csEntityState>(data->state);
  if (entityState)
  {
    if (element->Attribute("name"))
    {
      entityState->SetName(csString(element->Attribute("name")));
    }
    else
    {
      entityState->SetName(entityState->GetClass()->GetName());
    }

    if (element->Attribute("id"))
    {
      csID id = static_cast<csID>(atoi(element->Attribute("id")));
      entityState->SetID(id);
    }
  }
  return data->state;
}

const csClass *csEntityStateXMLLoader::GetLoadingClass() const
{
  return csEntityState::GetStaticClass();
}