#include <cobalt/loaders/entity/csdynamiccolliderstatexmlloader.hh>
#include <cobalt/loaders/entity/csentitystateloaderdata.hh>
#include <cobalt/entity/csdynamiccolliderstate.hh>


csDynamicColliderStateXMLLoader::csDynamicColliderStateXMLLoader()
  : csBaseColliderStateXMLLoader()
{

}


csDynamicColliderStateXMLLoader::~csDynamicColliderStateXMLLoader()
{

}


const csClass *csDynamicColliderStateXMLLoader::EvalClass(TiXmlElement *element, const csResourceLocator &locator, iObject *userData) const
{
  return csDynamicColliderState::GetStaticClass();
}

iObject *csDynamicColliderStateXMLLoader::Load(TiXmlElement *element, const csResourceLocator &locator, iObject *userData) const
{
  if (!userData || !element)
  {
    return userData;
  }

  csEntityStateLoaderData *data = csQueryClass<csEntityStateLoaderData>(userData);
  csDynamicColliderState *dynCollider = csQueryClass<csDynamicColliderState>(data->state);
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
      csVector3f inertia = LoadVector3f(inertiaElement->GetText());
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

  return csBaseColliderStateXMLLoader::Load(element, locator, userData);
}

const csClass *csDynamicColliderStateXMLLoader::GetLoadingClass() const
{
  return csDynamicColliderState::GetStaticClass();
}

