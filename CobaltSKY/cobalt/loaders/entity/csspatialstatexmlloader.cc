#include <cobalt/loaders/entity/csspatialstatexmlloader.hh>
#include <cobalt/loaders/entity/csentitystateloaderdata.hh>
#include <cobalt/entity/csspatialstate.hh>


// ***********************************************************************************************
// ***********************************************************************************************
//
//                   csSpatialStateLoader
//
// ***********************************************************************************************
// ***********************************************************************************************




csSpatialStateXMLLoader::csSpatialStateXMLLoader()
  : csEntityStateXMLLoader()
{

}


csSpatialStateXMLLoader::~csSpatialStateXMLLoader()
{

}


const csClass *csSpatialStateXMLLoader::EvalClass(TiXmlElement *element, const csResourceLocator &locator, iObject *userData) const
{
  return csSpatialState::GetStaticClass();
}


iObject *csSpatialStateXMLLoader::Load(TiXmlElement *element, const csResourceLocator &locator, iObject *userData) const
{
  if (!userData || !element)
  {
    return userData;
  }
  csEntityStateLoaderData *data = csQueryClass<csEntityStateLoaderData>(userData);
  csSpatialState *spatialState = csQueryClass<csSpatialState>(data->state);
  if (spatialState)
  {


    TiXmlElement *transformationElement = element->FirstChildElement("transformation");
    if (transformationElement)
    {
      csTransformation trans = spatialState->GetTransformation();

      TiXmlElement *translationElement = transformationElement->FirstChildElement("translation");
      if (translationElement)
      {
        const char *txt = translationElement->GetText();
        if (txt)
        {
          csVector3f translation = LoadVector3f(txt);
          trans.SetTranslation(translation);
        }
      }

      TiXmlElement *axisAngleElement = transformationElement->FirstChildElement("axisAngle");
      if (axisAngleElement)
      {
        const char *txt = axisAngleElement->GetText();
        if (txt)
        {
          csVector4f axisAngle = LoadVector4f(txt);
          trans.SetRotation(axisAngle.AsVector3f(), axisAngle.w);
        }
      }
    }
    TiXmlElement *clippingRangeElement = element->FirstChildElement("clippingRange");
    if (clippingRangeElement)
    {
      float min = -FLT_MAX;
      float max = FLT_MAX;
      if (clippingRangeElement->Attribute("min"))
      {
        min = LoadFloat(clippingRangeElement->Attribute("min"));
      }
      if (clippingRangeElement->Attribute("max"))
      {
        max = LoadFloat(clippingRangeElement->Attribute("max"));
      }
      spatialState->SetClippingRange(min, max);
    }
  }

  return csEntityStateXMLLoader::Load(element, locator, userData);
}

const csClass *csSpatialStateXMLLoader::GetLoadingClass() const
{
  return csSpatialState::GetStaticClass();
}


