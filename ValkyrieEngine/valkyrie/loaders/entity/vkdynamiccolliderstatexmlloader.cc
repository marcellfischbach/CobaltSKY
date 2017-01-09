#include <valkyrie/loaders/entity/vkdynamiccolliderstatexmlloader.hh>
#include <valkyrie/loaders/entity/vkentitystateloaderdata.hh>
#include <valkyrie/entity/vkdynamiccolliderstate.hh>


vkDynamicColliderStateXMLLoader::vkDynamicColliderStateXMLLoader()
  : vkBaseColliderStateXMLLoader()
{

}


vkDynamicColliderStateXMLLoader::~vkDynamicColliderStateXMLLoader()
{

}


const vkClass *vkDynamicColliderStateXMLLoader::EvalClass(TiXmlElement *element, const vkResourceLocator &locator, iObject *userData) const
{
  return vkDynamicColliderState::GetStaticClass();
}

iObject *vkDynamicColliderStateXMLLoader::Load(TiXmlElement *element, const vkResourceLocator &locator, iObject *userData) const
{
  if (!userData || !element)
  {
    return userData;
  }

  vkEntityStateLoaderData *data = vkQueryClass<vkEntityStateLoaderData>(userData);
  vkDynamicColliderState *dynCollider = vkQueryClass<vkDynamicColliderState>(data->state);
  if (dynCollider)
  {
    TiXmlElement *massElement = element->FirstChildElement("mass");
    if (massElement)
    {
      float mass = LoadFloat(massElement->GetText());
      dynCollider->SetMass(mass);
    }

    TiXmlElement *inertiaElement = element->FirstChildElement("inertia");
    if (inertiaElement)
    {
      vkVector3f inertia = LoadVector3f(inertiaElement->GetText());
      dynCollider->SetInertia(inertia);
    }
    else
    {
      dynCollider->SetAutoInertia(true);
    }

    TiXmlElement *kinematicElement = element->FirstChildElement("kinematic");
    if (kinematicElement)
    {
      bool kinematic = LoadBool(kinematicElement->GetText());
      dynCollider->SetKinematic(kinematic);
    }
  }

  return vkBaseColliderStateXMLLoader::Load(element, locator, userData);
}

const vkClass *vkDynamicColliderStateXMLLoader::GetLoadingClass() const
{
  return vkDynamicColliderState::GetStaticClass();
}

