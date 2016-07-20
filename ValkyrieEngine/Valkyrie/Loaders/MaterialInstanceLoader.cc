
#include <Valkyrie/Loaders/MaterialInstanceLoader.hh>
#include <Valkyrie/Core/ClassRegistry.hh>
#include <Valkyrie/Graphics/Material.hh>
#include <Valkyrie/Engine.hh>
#include <Valkyrie/Graphics/IGraphics.hh>
#include <Valkyrie/Graphics/ITexture.hh>
#include <map>



vkMaterialInstanceAssetXMLLoader::vkMaterialInstanceAssetXMLLoader()
  : vkBaseXMLLoader()
{

}


vkMaterialInstanceAssetXMLLoader::~vkMaterialInstanceAssetXMLLoader()
{

}

bool vkMaterialInstanceAssetXMLLoader::CanLoad(TiXmlElement *element, const vkResourceLocator &locator, IObject *userData) const
{
  return vkString(element->Value()) == vkString("materialinstance");
}


IObject *vkMaterialInstanceAssetXMLLoader::Load(TiXmlElement *element, const vkResourceLocator &locator, IObject *userData) const
{
  vkMaterialInstance *materialInstance = new vkMaterialInstance();

  TiXmlElement *materialElement = element->FirstChildElement("material");
  if (!materialElement)
  {
    materialInstance->Release();
    return 0;
  }

  vkMaterial *material = vkResourceManager::Get()->GetOrLoad<vkMaterial>(vkResourceLocator(vkString(materialElement->GetText())));
  if (!material)
  {
    materialInstance->Release();
    return 0;
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
        materialInstance->Release();
        return 0;
      }
      vkString parameterName(parameterElement->Attribute("name"));
      vkInt16 index = materialInstance->GetIndex(parameterName);
      if (index == -1)
      {
        materialInstance->Release();
        return 0;
      }

      vkShaderParameterType type = material->GetParamType(index);
      TiXmlElement *valueElement = parameterElement->FirstChildElement();
      if (!valueElement)
      {
        materialInstance->Release();
        return 0;
      }

      vkString tagName(valueElement->Value());
      if (tagName == vkString("float"))
      {
        materialInstance->Set(index, LoadFloat(valueElement->Value()));
      }
      else if (tagName == vkString("float2"))
      {
        materialInstance->Set(index, LoadVector2f(valueElement->Value()));
      }
      else if (tagName == vkString("float3"))
      {
        materialInstance->Set(index, LoadVector3f(valueElement->Value()));
      }
      else if (tagName == vkString("float4"))
      {
        materialInstance->Set(index, LoadVector4f(valueElement->Value()));
      }
      else if (tagName == vkString("color4"))
      {
        materialInstance->Set(index, LoadColor4f(valueElement->Value()));
      }
      else if (tagName == vkString("locator"))
      {
        ITexture *texture = vkResourceManager::Get()->GetOrLoad<ITexture>(vkResourceLocator(vkString(valueElement->GetText())));
        materialInstance->Set(index, texture);
      }
    }
  }


  return materialInstance;
}


