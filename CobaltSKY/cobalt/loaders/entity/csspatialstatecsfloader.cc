#include <cobalt/loaders/entity/csspatialstatecsfloader.hh>
#include <cobalt/loaders/entity/csentitystateloaderdata.hh>
#include <cobalt/entity/csspatialstate.hh>


// ***********************************************************************************************
// ***********************************************************************************************
//
//                   csSpatialStateLoader
//
// ***********************************************************************************************
// ***********************************************************************************************




csSpatialStateCSFLoader::csSpatialStateCSFLoader()
  : csEntityStateCSFLoader()
{

}


csSpatialStateCSFLoader::~csSpatialStateCSFLoader()
{

}


const csClass *csSpatialStateCSFLoader::EvalClass(const csfEntry *entry, const csResourceLocator &locator, iObject *userData) const
{
  return csSpatialState::GetStaticClass();
}


iObject *csSpatialStateCSFLoader::Load(const csfEntry *entry, const csResourceLocator &locator, iObject *userData) const
{
  if (!userData || !entry)
  {
    return userData;
  }
  csEntityStateLoaderData *data = csQueryClass<csEntityStateLoaderData>(userData);
  csSpatialState *spatialState = csQueryClass<csSpatialState>(data->state);
  if (spatialState)
  {


    const csfEntry *transformationEntry = entry->GetEntry("transformation");
    if (transformationEntry)
    {
      csTransformation trans = spatialState->GetTransformation();

      const csfEntry *translationEntry = transformationEntry->GetEntry("translation");
      if (translationEntry)
      {
        csVector3f translation = LoadVector3f(translationEntry);
        trans.SetTranslation(translation);
      }

      const csfEntry *axisAngleEntry = transformationEntry->GetEntry("axisAngle");
      if (axisAngleEntry)
      {
        csVector4f axisAngle = LoadVector4f(axisAngleEntry);
        trans.SetRotation(axisAngle.AsVector3f(), axisAngle.w);
      }
    }
    const csfEntry *clippingRangeEntry = entry->GetEntry("clippingRange");
    if (clippingRangeEntry)
    {
      float min = -FLT_MAX;
      float max = FLT_MAX;
      if (clippingRangeEntry->HasAttribute("min"))
      {
        min = LoadFloat(clippingRangeEntry);
      }
      if (clippingRangeEntry->HasAttribute("max"))
      {
        max = LoadFloat(clippingRangeEntry);
      }
      spatialState->SetClippingRange(min, max);
    }
  }

  return csEntityStateCSFLoader::Load(entry, locator, userData);
}

const csClass *csSpatialStateCSFLoader::GetLoadingClass() const
{
  return csSpatialState::GetStaticClass();
}


