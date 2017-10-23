
#include <cobalt/loaders/entity/csrenderstatecsfloader.hh>
#include <cobalt/loaders/entity/csentitystateloaderdata.hh>
#include <cobalt/entity/csrenderstate.hh>

// ***********************************************************************************************
// ***********************************************************************************************
//
//                   csRenderStateLoader
//
// ***********************************************************************************************
// ***********************************************************************************************




csRenderStateCSFLoader::csRenderStateCSFLoader()
  : csSpatialStateCSFLoader()
{

}


csRenderStateCSFLoader::~csRenderStateCSFLoader()
{

}

const csClass *csRenderStateCSFLoader::EvalClass(const csfEntry *entry, const csResourceLocator &locator, iObject *userData) const
{
  return csRenderState::GetStaticClass();
}


iObject *csRenderStateCSFLoader::Load(const csfEntry *entry, const csResourceLocator &locator, iObject *userData) const
{
  csEntityStateLoaderData *data = csQueryClass<csEntityStateLoaderData>(userData);
  csRenderState *renderState = csQueryClass<csRenderState>(data->state);
  if (!renderState)
  {
    return userData;
  }
  const csfEntry *fadeOutElement = entry->GetEntry("fadeOut");
  if (fadeOutElement && fadeOutElement->HasAttribute("near") && fadeOutElement->HasAttribute("far"))
  {
    float near = LoadFloat(fadeOutElement, "near");
    float far = LoadFloat(fadeOutElement, "far");
    renderState->SetFadeOut(near, far);
  }
  return csSpatialStateCSFLoader::Load(entry, locator, userData);
}

const csClass *csRenderStateCSFLoader::GetLoadingClass() const
{
  return csRenderState::GetStaticClass();
}

