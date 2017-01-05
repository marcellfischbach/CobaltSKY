
#include <Valkyrie/Loaders/entity/vkbasecolliderstatexmlloader.hh>
#include <Valkyrie/Loaders/entity/vkentitystateloaderdata.hh>
#include <Valkyrie/entity/vkbasecolliderstate.hh>



vkBaseColliderStateXMLLoader::vkBaseColliderStateXMLLoader()
  : vkColliderStateXMLLoader()
{

}


vkBaseColliderStateXMLLoader::~vkBaseColliderStateXMLLoader()
{

}

const vkClass *vkBaseColliderStateXMLLoader::EvalClass(TiXmlElement *element, const vkResourceLocator &locator, IObject *userData) const
{
  return vkBaseColliderState::GetStaticClass();
}


IObject *vkBaseColliderStateXMLLoader::Load(TiXmlElement *element, const vkResourceLocator &locator, IObject *userData) const
{
  if (!userData || !element)
  {
    return userData;
  }

  vkEntityStateLoaderData *data = vkQueryClass<vkEntityStateLoaderData>(userData);
  vkBaseColliderState *baseCollider = vkQueryClass<vkBaseColliderState>(data->state);
  if (baseCollider)
  {
    TiXmlElement *frictionElement = element->FirstChildElement("friction");
    if (frictionElement)
    {
      baseCollider->SetFriction(LoadFloat(frictionElement->GetText()));
    }
    TiXmlElement *restitutionElement = element->FirstChildElement("restitution");
    if (restitutionElement)
    {
      baseCollider->SetRestitution(LoadFloat(restitutionElement->GetText()));
    }
  }


  return vkColliderStateXMLLoader::Load(element, locator, userData);
}

const vkClass *vkBaseColliderStateXMLLoader::GetLoadingClass() const
{
  return vkBaseColliderState::GetStaticClass();
}


