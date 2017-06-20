
#include <cobalt/loaders/entity/csbasecolliderstatexmlloader.hh>
#include <cobalt/loaders/entity/csentitystateloaderdata.hh>
#include <cobalt/entity/csbasecolliderstate.hh>



csBaseColliderStateXMLLoader::csBaseColliderStateXMLLoader()
  : csColliderStateXMLLoader()
{

}


csBaseColliderStateXMLLoader::~csBaseColliderStateXMLLoader()
{

}

const csClass *csBaseColliderStateXMLLoader::EvalClass(TiXmlElement *element, const csResourceLocator &locator, iObject *userData) const
{
  return csBaseColliderState::GetStaticClass();
}


iObject *csBaseColliderStateXMLLoader::Load(TiXmlElement *element, const csResourceLocator &locator, iObject *userData) const
{
  if (!userData || !element)
  {
    return userData;
  }

  csEntityStateLoaderData *data = csQueryClass<csEntityStateLoaderData>(userData);
  csBaseColliderState *baseCollider = csQueryClass<csBaseColliderState>(data->state);
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


  return csColliderStateXMLLoader::Load(element, locator, userData);
}

const csClass *csBaseColliderStateXMLLoader::GetLoadingClass() const
{
  return csBaseColliderState::GetStaticClass();
}


