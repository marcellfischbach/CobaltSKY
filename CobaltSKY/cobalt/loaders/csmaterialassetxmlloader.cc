
#include <cobalt/loaders/csmaterialassetxmlloader.hh>
#include <cobalt/core/csclassregistry.hh>
#include <cobalt/graphics/csmaterialdef.hh>
#include <cobalt/graphics/csmaterial.hh>
#include <cobalt/csengine.hh>
#include <cobalt/graphics/igraphics.hh>
#include <cobalt/graphics/itexture.hh>
#include <map>



csMaterialAssetXMLLoader::csMaterialAssetXMLLoader()
  : csBaseXMLLoader()
{

}


csMaterialAssetXMLLoader::~csMaterialAssetXMLLoader()
{

}

bool csMaterialAssetXMLLoader::CanLoad(TiXmlElement *element, const csResourceLocator &locator, iObject *userData) const
{
  return csString(element->Value()) == csString("material");
}

const csClass *csMaterialAssetXMLLoader::EvalClass(TiXmlElement *element, const csResourceLocator &locator, iObject *userData) const
{
  return csMaterial::GetStaticClass();
}

iObject *csMaterialAssetXMLLoader::Load(TiXmlElement *element, const csResourceLocator &locator, iObject *userData) const
{
  csMaterial *material = new csMaterial();

  TiXmlElement *materialDefElement = element->FirstChildElement("materialDef");
  if (!materialDefElement)
  {
    return material;
  }

  if (!materialDefElement->GetText())
  {
    return material;
  }
  csMaterialDef *materialDef = csResourceManager::Get()->GetOrLoad<csMaterialDef>(csResourceLocator(csString(materialDefElement->GetText())));
  if (!materialDef)
  {
    return material;
  }

  material->SetMaterialDef(materialDef);
  materialDef->Release();

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
      csInt16 index = material->GetIndex(parameterName);
      if (index == -1)
      {
        continue;
      }

      csShaderParameterType type = materialDef->GetParamType(index);
      TiXmlElement *valueElement = parameterElement->FirstChildElement();
      if (!valueElement)
      {
        continue;
      }

      csString tagName(valueElement->Value());
      if (tagName == csString("float"))
      {
        material->Set(index, LoadFloat(valueElement->GetText()));
      }
      else if (tagName == csString("float2"))
      {
        material->Set(index, LoadVector2f(valueElement->GetText()));
      }
      else if (tagName == csString("float3"))
      {
        material->Set(index, LoadVector3f(valueElement->GetText()));
      }
      else if (tagName == csString("float4"))
      {
        material->Set(index, LoadVector4f(valueElement->GetText()));
      }
      else if (tagName == csString("color4"))
      {
        material->Set(index, LoadColor4f(valueElement->GetText()));
      }
      else if (tagName == csString("locator"))
      {
        iTexture *texture = csResourceManager::Get()->GetOrLoad<iTexture>(csResourceLocator(csString(valueElement->GetText())));
        material->Set(index, texture);
      }
    }
  }


  return material;
}


