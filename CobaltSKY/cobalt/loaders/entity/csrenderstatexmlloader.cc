
#include <cobalt/loaders/entity/csrenderstatexmlloader.hh>
#include <cobalt/loaders/entity/csentitystateloaderdata.hh>
#include <cobalt/entity/csrenderstate.hh>

// ***********************************************************************************************
// ***********************************************************************************************
//
//                   csRenderStateLoader
//
// ***********************************************************************************************
// ***********************************************************************************************




csRenderStateXMLLoader::csRenderStateXMLLoader()
  : csSpatialStateXMLLoader()
{

}


csRenderStateXMLLoader::~csRenderStateXMLLoader()
{

}

const csClass *csRenderStateXMLLoader::EvalClass(TiXmlElement *element, const csResourceLocator &locator, iObject *userData) const
{
  return csRenderState::GetStaticClass();
}


iObject *csRenderStateXMLLoader::Load(TiXmlElement *element, const csResourceLocator &locator, iObject *userData) const
{
  csEntityStateLoaderData *data = csQueryClass<csEntityStateLoaderData>(userData);
  csRenderState *renderState = csQueryClass<csRenderState>(data->state);
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
  return csSpatialStateXMLLoader::Load(element, locator, userData);
}

const csClass *csRenderStateXMLLoader::GetLoadingClass() const
{
  return csRenderState::GetStaticClass();
}

