
#include <valkyrie/loaders/entity/vkcolliderstatexmlloader.hh>
#include <valkyrie/loaders/entity/vkentitystateloaderdata.hh>
#include <valkyrie/entity/vkcolliderstate.hh>
#include <valkyrie/physics/vkphysicsshapecontainer.hh>





vkColliderStateXMLLoader::vkColliderStateXMLLoader()
  : vkSpatialStateXMLLoader()
{

}


vkColliderStateXMLLoader::~vkColliderStateXMLLoader()
{

}


const vkClass *vkColliderStateXMLLoader::EvalClass(TiXmlElement *element, const vkResourceLocator &locator, iObject *userData) const
{
  return vkColliderState::GetStaticClass();
}

iObject *vkColliderStateXMLLoader::Load(TiXmlElement *element, const vkResourceLocator &locator, iObject *userData) const
{
  if (!userData || !element)
  {
    return userData;
  }

  vkEntityStateLoaderData *data = vkQueryClass<vkEntityStateLoaderData>(userData);
  vkColliderState *collider = vkQueryClass<vkColliderState>(data->state);
  TiXmlElement *shapeElement = element->FirstChildElement("shape");
  if (shapeElement)
  {
    const char *txt = shapeElement->GetText();
    vkResourceLoadingMode loadingMode = GetResourceLoadingMode(shapeElement, eRLM_Shared, eRLM_Instance);
    vkPhysicsShapeContainer *shapes = vkResourceManager::Get()->Aquire<vkPhysicsShapeContainer>(vkResourceLocator(vkString(txt)), 0, loadingMode);
    collider->AttachShape(shapes);
  }

  return vkColliderStateXMLLoader::Load(element, locator, userData);
}

const vkClass *vkColliderStateXMLLoader::GetLoadingClass() const
{
  return vkColliderState::GetStaticClass();
}

