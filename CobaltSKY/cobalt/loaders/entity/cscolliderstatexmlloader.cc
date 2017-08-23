
#include <cobalt/loaders/entity/cscolliderstatexmlloader.hh>
#include <cobalt/loaders/entity/csentitystateloaderdata.hh>
#include <cobalt/entity/cscolliderstate.hh>
#include <cobalt/physics/csphysicsshapecontainer.hh>





csColliderStateXMLLoader::csColliderStateXMLLoader()
  : csSpatialStateXMLLoader()
{

}


csColliderStateXMLLoader::~csColliderStateXMLLoader()
{

}


const csClass *csColliderStateXMLLoader::EvalClass(TiXmlElement *element, const csResourceLocator &locator, iObject *userData) const
{
  return csColliderState::GetStaticClass();
}

iObject *csColliderStateXMLLoader::Load(TiXmlElement *element, const csResourceLocator &locator, iObject *userData) const
{
  if (!userData || !element)
  {
    return userData;
  }

  csEntityStateLoaderData *data = csQueryClass<csEntityStateLoaderData>(userData);
  csColliderState *collider = csQueryClass<csColliderState>(data->state);
  TiXmlElement *shapeElement = element->FirstChildElement("shape");
  if (shapeElement)
  {
    const char *txt = shapeElement->GetText();
    csResourceLoadingMode loadingMode = GetResourceLoadingMode(shapeElement, eRLM_Shared, eRLM_Instance);
    csPhysicsShapeContainer *shapes = csResourceManager::Get()->Aquire<csPhysicsShapeContainer>(csResourceLocator(std::string(txt)), 0, loadingMode);
    collider->AttachShape(shapes);
  }

  return csColliderStateXMLLoader::Load(element, locator, userData);
}

const csClass *csColliderStateXMLLoader::GetLoadingClass() const
{
  return csColliderState::GetStaticClass();
}

