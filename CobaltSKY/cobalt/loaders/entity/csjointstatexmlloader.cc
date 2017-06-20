#include <cobalt/loaders/entity/csjointstatexmlloader.hh>
#include <cobalt/loaders/entity/csentitystateloaderdata.hh>
#include <cobalt/entity/csjointstate.hh>
#include <cobalt/entity/csentity.hh>
#include <cobalt/entity/csdynamiccolliderstate.hh>


csJointStateXMLLoader::csJointStateXMLLoader()
  : csSpatialStateXMLLoader()
{

}

csJointStateXMLLoader::~csJointStateXMLLoader()
{

}


const csClass *csJointStateXMLLoader::EvalClass(TiXmlElement *element, const csResourceLocator &locator, iObject *userData) const
{
  return csJointState::GetStaticClass();
}

iObject *csJointStateXMLLoader::Load(TiXmlElement *element, const csResourceLocator &locator, iObject *userData) const
{
  csEntityStateLoaderData *data = csQueryClass<csEntityStateLoaderData>(userData);
  csJointState *jointState = csQueryClass<csJointState>(data->state);
  if (jointState)
  {
    TiXmlElement *colliderAElement = element->FirstChildElement("colliderA");
    if (colliderAElement && colliderAElement->Attribute("id"))
    {
      csID id = static_cast<csID>(atoi(colliderAElement->Attribute("id")));
      csEntityState *state = data->entity->GetState(id);
      csDynamicColliderState *dynState = csQueryClass<csDynamicColliderState>(state);
      if (dynState)
      {
        jointState->SetColliderA(dynState);
      }
    }
    TiXmlElement *colliderBElement = element->FirstChildElement("colliderB");
    if (colliderBElement && colliderBElement->Attribute("id"))
    {
      csID id = static_cast<csID>(atoi(colliderBElement->Attribute("id")));
      csEntityState *state = data->entity->GetState(id);
      csDynamicColliderState *dynState = csQueryClass<csDynamicColliderState>(state);
      if (dynState)
      {
        jointState->SetColliderB(dynState);
      }
    }
  }
  return csSpatialStateXMLLoader::Load(element, locator, userData);
}

const csClass *csJointStateXMLLoader::GetLoadingClass() const
{
  return csJointState::GetStaticClass();
}


