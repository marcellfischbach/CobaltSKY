#include <valkyrie/loaders/entity/vkspatialstatexmlloader.hh>
#include <valkyrie/loaders/entity/vkentitystateloaderdata.hh>
#include <valkyrie/entity/vkspatialstate.hh>


// ***********************************************************************************************
// ***********************************************************************************************
//
//                   vkSpatialStateLoader
//
// ***********************************************************************************************
// ***********************************************************************************************




vkSpatialStateXMLLoader::vkSpatialStateXMLLoader()
  : vkEntityStateXMLLoader()
{

}


vkSpatialStateXMLLoader::~vkSpatialStateXMLLoader()
{

}


const vkClass *vkSpatialStateXMLLoader::EvalClass(TiXmlElement *element, const vkResourceLocator &locator, iObject *userData) const
{
  return vkSpatialState::GetStaticClass();
}


iObject *vkSpatialStateXMLLoader::Load(TiXmlElement *element, const vkResourceLocator &locator, iObject *userData) const
{
  if (!userData || !element)
  {
    return userData;
  }
  vkEntityStateLoaderData *data = vkQueryClass<vkEntityStateLoaderData>(userData);
  vkSpatialState *spatialState = vkQueryClass<vkSpatialState>(data->state);
  if (spatialState)
  {


    TiXmlElement *transformationElement = element->FirstChildElement("transformation");
    if (transformationElement)
    {
      vkTransformation trans = spatialState->GetTransformation();

      TiXmlElement *translationElement = transformationElement->FirstChildElement("translation");
      if (translationElement)
      {
        const char *txt = translationElement->GetText();
        if (txt)
        {
          vkVector3f translation = LoadVector3f(txt);
          trans.SetTranslation(translation);
        }
      }

      TiXmlElement *axisAngleElement = transformationElement->FirstChildElement("axisAngle");
      if (axisAngleElement)
      {
        const char *txt = axisAngleElement->GetText();
        if (txt)
        {
          vkVector4f axisAngle = LoadVector4f(txt);
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

  return vkEntityStateXMLLoader::Load(element, locator, userData);
}

const vkClass *vkSpatialStateXMLLoader::GetLoadingClass() const
{
  return vkSpatialState::GetStaticClass();
}


