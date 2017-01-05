
#include <Valkyrie/Loaders/MaterialInstanceLoader.hh>
#include <Valkyrie/core/vkclassregistry.hh>
#include <Valkyrie/Graphics/vkmaterial.hh>
#include <Valkyrie/Graphics/vkmaterialinstance.hh>
#include <Valkyrie/vkengine.hh>
#include <Valkyrie/Graphics/igraphics.hh>
#include <Valkyrie/Graphics/itexture.hh>
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
  return vkString(element->Value()) == vkString("materialInstance");
}

const vkClass *vkMaterialInstanceAssetXMLLoader::EvalClass(TiXmlElement *element, const vkResourceLocator &locator, IObject *userData) const
{
  return vkMaterialInstance::GetStaticClass();
}

IObject *vkMaterialInstanceAssetXMLLoader::Load(TiXmlElement *element, const vkResourceLocator &locator, IObject *userData) const
{
  vkMaterialInstance *materialInstance = new vkMaterialInstance();

  TiXmlElement *materialElement = element->FirstChildElement("material");
  if (!materialElement)
  {
    return materialInstance;
  }

  vkMaterial *material = vkResourceManager::Get()->GetOrLoad<vkMaterial>(vkResourceLocator(vkString(materialElement->GetText())));
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
      vkString parameterName(parameterElement->Attribute("name"));
      vkInt16 index = materialInstance->GetIndex(parameterName);
      if (index == -1)
      {
        continue;
      }

      vkShaderParameterType type = material->GetParamType(index);
      TiXmlElement *valueElement = parameterElement->FirstChildElement();
      if (!valueElement)
      {
        continue;
      }

      vkString tagName(valueElement->Value());
      if (tagName == vkString("float"))
      {
        materialInstance->Set(index, LoadFloat(valueElement->GetText()));
      }
      else if (tagName == vkString("float2"))
      {
        materialInstance->Set(index, LoadVector2f(valueElement->GetText()));
      }
      else if (tagName == vkString("float3"))
      {
        materialInstance->Set(index, LoadVector3f(valueElement->GetText()));
      }
      else if (tagName == vkString("float4"))
      {
        materialInstance->Set(index, LoadVector4f(valueElement->GetText()));
      }
      else if (tagName == vkString("color4"))
      {
        materialInstance->Set(index, LoadColor4f(valueElement->GetText()));
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


