#include <Valkyrie/loaders/entity/vkjointstatexmlloader.hh>
#include <Valkyrie/loaders/entity/vkentitystateloaderdata.hh>
#include <Valkyrie/entity/vkjointstate.hh>
#include <Valkyrie/entity/vkentity.hh>
#include <Valkyrie/entity/vkdynamiccolliderstate.hh>


vkJointStateXMLLoader::vkJointStateXMLLoader()
  : vkSpatialStateXMLLoader()
{

}

vkJointStateXMLLoader::~vkJointStateXMLLoader()
{

}


const vkClass *vkJointStateXMLLoader::EvalClass(TiXmlElement *element, const vkResourceLocator &locator, IObject *userData) const
{
  return vkJointState::GetStaticClass();
}

IObject *vkJointStateXMLLoader::Load(TiXmlElement *element, const vkResourceLocator &locator, IObject *userData) const
{
  vkEntityStateLoaderData *data = vkQueryClass<vkEntityStateLoaderData>(userData);
  vkJointState *jointState = vkQueryClass<vkJointState>(data->state);
  if (jointState)
  {
    TiXmlElement *colliderAElement = element->FirstChildElement("colliderA");
    if (colliderAElement && colliderAElement->Attribute("id"))
    {
      vkID id = static_cast<vkID>(atoi(colliderAElement->Attribute("id")));
      vkEntityState *state = data->entity->GetState(id);
      vkDynamicColliderState *dynState = vkQueryClass<vkDynamicColliderState>(state);
      if (dynState)
      {
        jointState->SetColliderA(dynState);
      }
    }
    TiXmlElement *colliderBElement = element->FirstChildElement("colliderB");
    if (colliderBElement && colliderBElement->Attribute("id"))
    {
      vkID id = static_cast<vkID>(atoi(colliderBElement->Attribute("id")));
      vkEntityState *state = data->entity->GetState(id);
      vkDynamicColliderState *dynState = vkQueryClass<vkDynamicColliderState>(state);
      if (dynState)
      {
        jointState->SetColliderB(dynState);
      }
    }
  }
  return vkSpatialStateXMLLoader::Load(element, locator, userData);
}

const vkClass *vkJointStateXMLLoader::GetLoadingClass() const
{
  return vkJointState::GetStaticClass();
}


