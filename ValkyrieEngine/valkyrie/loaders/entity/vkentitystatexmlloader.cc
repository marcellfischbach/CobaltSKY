#include <valkyrie/loaders/entity/vkentitystatexmlloader.hh>
#include <valkyrie/loaders/entity/vkentitystateloaderdata.hh>
#include <valkyrie/entity/vkentity.hh>
#include <valkyrie/entity/vkentitystate.hh>



// ***********************************************************************************************
// ***********************************************************************************************
//
//                   vkEntityStateLoader
//
// ***********************************************************************************************
// ***********************************************************************************************




vkEntityStateXMLLoader::vkEntityStateXMLLoader()
  : vkBaseXMLLoader()
{

}


vkEntityStateXMLLoader::~vkEntityStateXMLLoader()
{

}


bool vkEntityStateXMLLoader::CanLoad(TiXmlElement *element, const vkResourceLocator &locator, IObject *userData) const
{
  return false;
}


const vkClass *vkEntityStateXMLLoader::EvalClass(TiXmlElement *element, const vkResourceLocator &locator, IObject *userData) const
{
  return vkEntityState::GetStaticClass();
}

IObject *vkEntityStateXMLLoader::Load(TiXmlElement *element, const vkResourceLocator &locator, IObject *userData) const
{
  if (!userData || !element)
  {
    return userData;
  }

  vkEntityStateLoaderData *data = vkQueryClass<vkEntityStateLoaderData>(userData);
  vkEntityState *entityState = vkQueryClass<vkEntityState>(data->state);
  if (entityState)
  {
    if (element->Attribute("name"))
    {
      entityState->SetName(vkString(element->Attribute("name")));
    }
    else
    {
      entityState->SetName(entityState->GetClass()->GetName());
    }

    if (element->Attribute("id"))
    {
      vkID id = static_cast<vkID>(atoi(element->Attribute("id")));
      entityState->SetID(id);
    }
  }
  return data->state;
}

const vkClass *vkEntityStateXMLLoader::GetLoadingClass() const
{
  return vkEntityState::GetStaticClass();
}