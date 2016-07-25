

#include <MaterialInstance/MaterialInstanceMetaLoader.hh>
#include <MaterialInstance/MaterialInstanceMeta.hh>

MaterialInstanceMetaLoader::MaterialInstanceMetaLoader()
  : vkBaseXMLLoader()
{

}

MaterialInstanceMetaLoader::~MaterialInstanceMetaLoader()
{

}

bool MaterialInstanceMetaLoader::CanLoad(TiXmlElement *element, const vkResourceLocator &locator, IObject *userData) const
{
  return vkString(element->Value()) == vkString("materialInstanceMeta");
}

const vkClass *MaterialInstanceMetaLoader::EvalClass(TiXmlElement *element, const vkResourceLocator &locator, IObject *userData) const
{
  return MaterialInstanceMeta::GetStaticClass();
}


IObject *MaterialInstanceMetaLoader::Load(TiXmlElement *element, const vkResourceLocator &locator, IObject *userData) const
{
  MaterialInstanceMeta *meta = new MaterialInstanceMeta();

  TiXmlElement *materialElement = element->FirstChildElement("material");
  if (materialElement)
  {
    meta->SetShaderResourceLocator(vkResourceLocator(vkString(materialElement->GetText())));
  }

  TiXmlElement *parametersElement = element->FirstChildElement("parameters");
  if (parametersElement)
  {
    for (TiXmlElement *parameterElement = parametersElement->FirstChildElement("parameter"); parameterElement; parameterElement = parameterElement->NextSiblingElement("parameter"))
    {
      TiXmlElement *locatorElement = parameterElement->FirstChildElement("locator");
      if (locatorElement && parameterElement->Attribute("name"))
      {
        meta->SetResourceLocator(
          vkString(parameterElement->Attribute("name")),
          vkResourceLocator(vkString(locatorElement->GetText()))
        );
      }
    }
  }


    return meta;
}
