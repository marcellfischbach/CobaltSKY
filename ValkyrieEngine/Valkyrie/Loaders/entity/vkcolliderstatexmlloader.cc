
#include <Valkyrie/Loaders/entity/vkcolliderstatexmlloader.hh>
#include <Valkyrie/Loaders/entity/vkentitystateloaderdata.hh>
#include <Valkyrie/entity/vkcolliderstate.hh>
#include <Valkyrie/physics/vkphysicsshapecontainer.hh>





vkColliderStateXMLLoader::vkColliderStateXMLLoader()
  : vkSpatialStateXMLLoader()
{

}


vkColliderStateXMLLoader::~vkColliderStateXMLLoader()
{

}


const vkClass *vkColliderStateXMLLoader::EvalClass(TiXmlElement *element, const vkResourceLocator &locator, IObject *userData) const
{
  return vkColliderState::GetStaticClass();
}

IObject *vkColliderStateXMLLoader::Load(TiXmlElement *element, const vkResourceLocator &locator, IObject *userData) const
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

