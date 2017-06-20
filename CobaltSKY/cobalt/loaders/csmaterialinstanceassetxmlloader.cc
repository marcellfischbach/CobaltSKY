
#include <cobalt/loaders/csmaterialinstanceassetxmlloader.hh>
#include <cobalt/core/csclassregistry.hh>
#include <cobalt/graphics/csmaterialdef.hh>
#include <cobalt/graphics/csmaterial.hh>
#include <cobalt/csengine.hh>
#include <cobalt/graphics/igraphics.hh>
#include <cobalt/graphics/itexture.hh>
#include <map>



csMaterialInstanceAssetXMLLoader::csMaterialInstanceAssetXMLLoader()
  : csBaseXMLLoader()
{

}


csMaterialInstanceAssetXMLLoader::~csMaterialInstanceAssetXMLLoader()
{

}

bool csMaterialInstanceAssetXMLLoader::CanLoad(TiXmlElement *element, const csResourceLocator &locator, iObject *userData) const
{
  return csString(element->Value()) == csString("materialInstance");
}

const csClass *csMaterialInstanceAssetXMLLoader::EvalClass(TiXmlElement *element, const csResourceLocator &locator, iObject *userData) const
{
  return csMaterial::GetStaticClass();
}

iObject *csMaterialInstanceAssetXMLLoader::Load(TiXmlElement *element, const csResourceLocator &locator, iObject *userData) const
{
  csMaterial *materialInstance = new csMaterial();

  TiXmlElement *materialElement = element->FirstChildElement("material");
  if (!materialElement)
  {
    return materialInstance;
  }

  csMaterialDef *material = csResourceManager::Get()->GetOrLoad<csMaterialDef>(csResourceLocator(csString(materialElement->GetText())));
  if (!material)
  {
    return materialInstance;
  }

  materialInstance->SetMaterial(material);
  material->Release();

  TiXmlElement *parametersElement = element->FirstChildElement("parameters");
  if (parametersElement)
  {
    for (TiXmlElement *parameterElement = parametersElement->FirstChildElement("parameter"); parameterElement; parameterElement = parameterElement->NextSiblingElement("parameter"))
    {
      if (!parameterElement->Attribute("name"))
      {
        continue;
      }
      csString parameterName(parameterElement->Attribute("name"));
      csInt16 index = materialInstance->GetIndex(parameterName);
      if (index == -1)
      {
        continue;
      }

      csShaderParameterType type = material->GetParamType(index);
      TiXmlElement *valueElement = parameterElement->FirstChildElement();
      if (!valueElement)
      {
        continue;
      }

      csString tagName(valueElement->Value());
      if (tagName == csString("float"))
      {
        materialInstance->Set(index, LoadFloat(valueElement->GetText()));
      }
      else if (tagName == csString("float2"))
      {
        materialInstance->Set(index, LoadVector2f(valueElement->GetText()));
      }
      else if (tagName == csString("float3"))
      {
        materialInstance->Set(index, LoadVector3f(valueElement->GetText()));
      }
      else if (tagName == csString("float4"))
      {
        materialInstance->Set(index, LoadVector4f(valueElement->GetText()));
      }
      else if (tagName == csString("color4"))
      {
        materialInstance->Set(index, LoadColor4f(valueElement->GetText()));
      }
      else if (tagName == csString("locator"))
      {
        iTexture *texture = csResourceManager::Get()->GetOrLoad<iTexture>(csResourceLocator(csString(valueElement->GetText())));
        materialInstance->Set(index, texture);
      }
    }
  }


  return materialInstance;
}


