
#include <Valkyrie/loaders/entity/vkrenderstatexmlloader.hh>
#include <Valkyrie/loaders/entity/vkentitystateloaderdata.hh>
#include <Valkyrie/entity/vkrenderstate.hh>

// ***********************************************************************************************
// ***********************************************************************************************
//
//                   vkRenderStateLoader
//
// ***********************************************************************************************
// ***********************************************************************************************




vkRenderStateXMLLoader::vkRenderStateXMLLoader()
  : vkSpatialStateXMLLoader()
{

}


vkRenderStateXMLLoader::~vkRenderStateXMLLoader()
{

}

const vkClass *vkRenderStateXMLLoader::EvalClass(TiXmlElement *element, const vkResourceLocator &locator, IObject *userData) const
{
  return vkRenderState::GetStaticClass();
}


IObject *vkRenderStateXMLLoader::Load(TiXmlElement *element, const vkResourceLocator &locator, IObject *userData) const
{
  vkEntityStateLoaderData *data = vkQueryClass<vkEntityStateLoaderData>(userData);
  vkRenderState *renderState = vkQueryClass<vkRenderState>(data->state);
  if (!renderState)
  {
    return userData;
  }
  TiXmlElement *fadeOutElement = element->FirstChildElement("fadeOut");
  if (fadeOutElement && fadeOutElement->Attribute("near") && fadeOutElement->Attribute("far"))
  {
    float near = (float)atof(fadeOutElement->Attribute("near"));
    float far = (float)atof(fadeOutElement->Attribute("far"));
    renderState->SetFadeOut(near, far);
  }
  return vkSpatialStateXMLLoader::Load(element, locator, userData);
}

const vkClass *vkRenderStateXMLLoader::GetLoadingClass() const
{
  return vkRenderState::GetStaticClass();
}

