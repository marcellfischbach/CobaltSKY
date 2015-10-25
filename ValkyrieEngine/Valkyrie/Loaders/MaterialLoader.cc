
#include <Valkyrie/Loaders/MaterialLoader.hh>
#include <Valkyrie/Graphics/Material.hh>



vkMaterialLoader::vkMaterialLoader()
  : vkBaseXMLLoader()
{
}

vkMaterialLoader::~vkMaterialLoader()
{

}


bool vkMaterialLoader::CanLoad(TiXmlElement *element, const vkResourceLocator &locator, IObject *userData) const
{
  vkString tagName(element->Value());

  return tagName == vkString("material") || tagName == vkString("materials");
}




IObject *vkMaterialLoader::Load(TiXmlElement *element, const vkResourceLocator &locator, IObject *userData) const
{
  TiXmlElement *materialElement = FindElement(element, "material", locator.GetResourceName());
  if (!materialElement)
  {
    printf("Exit 0");
    return 0;
  }

  vkMaterial *material = new vkMaterial();

  TiXmlElement *passesElement = materialElement->FirstChildElement("passes");
  if (!passesElement)
  {
    material->Release();
    printf("Exit 1");
    return 0;
  }

  for (TiXmlElement *passElement = passesElement->FirstChildElement("pass");
  passElement;
    passElement = passElement->NextSiblingElement("pass"))
  {
    vkString passName;
    if (passElement->Attribute("name"))
    {
      passName = vkString(passElement->Attribute("name"));
    }

    vkRenderPass renderPass;
    if (passName == vkString("GBuffer"))
    {
      renderPass = eRP_GBuffer;
    }
    else
    {
      material->Release();
      printf("Exit 2");
      return 0;
    }

    vkResourceLocator shaderLocator(passElement->GetText());
    IShader *shader = vkResourceManager::Get()->GetOrLoad<IShader>(shaderLocator);
    if (!shader)
    {
      material->Release();
      printf("Exit 3");
      return 0;
    }

    material->SetShader(renderPass, shader);
  }



  TiXmlElement *parametersElement = materialElement->FirstChildElement("parameters");
  if (parametersElement)
  {
    for (TiXmlElement *parameterElement = parametersElement->FirstChildElement("parameter");
    parameterElement;
      parameterElement = parameterElement->NextSiblingElement("parameter"))
    {
      vkString type;
      if (parameterElement->Attribute("type"))
      {
        type = vkString(parameterElement->Attribute("type"));
      }

      vkShaderParameterType paramType;
      if (type == vkString("float"))
      {
        paramType = eSPT_Float;
      }
      else if (type == vkString("vec2"))
      {
        paramType = eSPT_Vector2;
      }
      else if (type == vkString("vec3"))
      {
        paramType = eSPT_Vector3;
      }
      else if (type == vkString("vec4"))
      {
        paramType = eSPT_Vector4;
      }
      else if (type == vkString("int"))
      {
        paramType = eSPT_Int;
      }
      else if (type == vkString("ivec2"))
      {
        paramType = eSPT_IVector2;
      }
      else if (type == vkString("ivec3"))
      {
        paramType = eSPT_IVector3;
      }
      else if (type == vkString("ivec4"))
      {
        paramType = eSPT_IVector4;
      }
      else if (type == vkString("mat3"))
      {
        paramType = eSPT_Matrix3;
      }
      else if (type == vkString("mat4"))
      {
        paramType = eSPT_Matrix4;
      }
      else if (type == vkString("texture"))
      {
        paramType = eSPT_Texture;
      }
      else
      {
        continue;
      }

      vkString name(parameterElement->GetText());
      material->RegisterParam(vkShaderAttributeID(name), paramType);

    }

  }



  return material;
}
