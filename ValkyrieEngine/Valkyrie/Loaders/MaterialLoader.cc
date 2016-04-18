
#include <Valkyrie/Loaders/MaterialLoader.hh>
#include <Valkyrie/Graphics/Material.hh>
#include <Valkyrie/Graphics/ITexture.hh>
#include <Valkyrie/Engine.hh>



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



namespace
{
vkShaderParameterType get_shader_parameter_type(const vkString &name)
{
  if (name == vkString("float")) return eSPT_Float;
  else if (name == vkString("vec2")) return eSPT_Vector2;
  else if (name == vkString("vec3")) return eSPT_Vector3;
  else if (name == vkString("vec4")) return eSPT_Vector4;
  else if (name == vkString("int")) return eSPT_Int;
  else if (name == vkString("ivec2")) return eSPT_IVector2;
  else if (name == vkString("ivec3")) return eSPT_IVector3;
  else if (name == vkString("ivec4")) return eSPT_IVector4;
  else if (name == vkString("col")) return eSPT_Color4;
  else if (name == vkString("mat3")) return eSPT_Matrix3;
  else if (name == vkString("mat4")) return eSPT_Matrix4;
  else if (name == vkString("texture")) return eSPT_Texture;
  return eSPT_Float;
}
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
    printf("Exit 1\n");
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
    else if (passName == vkString("ShadowPSSM"))
    {
      renderPass = eRP_ShadowPSSM;
    }
    else if (passName == vkString("ShadowCube"))
    {
      renderPass = eRP_ShadowCube;
    }
    else
    {
      material->Release();
      printf("Exit 2\n");
      return 0;
    }

    vkResourceLocator shaderLocator(passElement->GetText());
    IShader *shader = vkResourceManager::Get()->GetOrLoad<IShader>(shaderLocator);
    if (!shader)
    {
      material->Release();
      printf("Exit 3\n");
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
      else if (type == vkString("col"))
      {
        paramType = eSPT_Color4;
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

vkMaterialInstanceLoader::vkMaterialInstanceLoader()
  : vkBaseXMLLoader()
{
}

vkMaterialInstanceLoader::~vkMaterialInstanceLoader()
{

}


bool vkMaterialInstanceLoader::CanLoad(TiXmlElement *element, const vkResourceLocator &locator, IObject *userData) const
{
  vkString tagName(element->Value());

  return tagName == vkString("materialinstance") || tagName == vkString("materialinstances");
}


IObject *vkMaterialInstanceLoader::Load(TiXmlElement *element, const vkResourceLocator &locator, IObject *userData) const
{
  TiXmlElement *materialInstanceElement = FindElement(element, "materialinstance", locator.GetResourceName());
  if (!materialInstanceElement)
  {
    return 0;
  }

  TiXmlElement *materialElement = materialInstanceElement->FirstChildElement("material");
  if (!materialElement)
  {
    printf("No material defined\n");
    return 0;
  }

  vkMaterial *material = vkResourceManager::Get()->GetOrLoad<vkMaterial>(vkResourceLocator(materialElement->GetText()));
  if (!material)
  {
    printf("No valid material found\n");
    return 0;
  }

  vkMaterialInstance *instance = new vkMaterialInstance();
  instance->SetMaterial(material);


  TiXmlElement *parametersElement = materialInstanceElement->FirstChildElement("parameters");
  if (parametersElement)
  {
    for (TiXmlElement *parameterElement = parametersElement->FirstChildElement("parameter");
    parameterElement;
      parameterElement = parameterElement->NextSiblingElement("parameter"))
    {
      if (!parameterElement->Attribute("name"))
      {
        printf("no name\n");
        continue;
      }

      vkString name(parameterElement->Attribute("name"));

      vkInt16 idx = instance->GetIndex(vkShaderAttributeID(name));
      if (idx == -1)
      {
        printf("name not defined %s\n", name.c_str());
        continue;
      }

      vkShaderParameterType type = material->GetParamType(idx);
      switch (type)
      {
      case eSPT_Float:
        {
          float f = LoadFloat(parameterElement->GetText());
          instance->Set(idx, f);
        }
        break;
      case eSPT_Vector2:
        {
          vkVector2f v = LoadVector2f(parameterElement->GetText());
          instance->Set(idx, v);
        }
        break;
      case eSPT_Vector3:
        {
          vkVector3f v = LoadVector3f(parameterElement->GetText());
          instance->Set(idx, v);
        }
        break;
      case eSPT_Vector4:
        {
          vkVector4f v = LoadVector4f(parameterElement->GetText());
          instance->Set(idx, v);
        }
        break;
      case eSPT_Color4:
        {
          vkColor4f v = LoadColor4f(parameterElement->GetText());
          printf("Load color: %.2f %.2f %.2f %.2f\n", v.r, v.g, v.b, v.a);
          instance->Set(idx, v);
        }
        break;
      case eSPT_Texture:
        {
          vkResourceLoadingMode rlm = GetResourceLoadingMode(parameterElement);
          ITexture *texture = 0;
          switch (rlm)
          {
          case eRLM_Shared:
            texture = vkResourceManager::Get()->GetOrLoad<ITexture>(vkResourceLocator(parameterElement->GetText()));
            instance->Set(idx, texture);
            break;
          case eRLM_Instance:
            texture = vkResourceManager::Get()->Load<ITexture>(vkResourceLocator(parameterElement->GetText()));
            instance->Set(idx, texture);
            if (texture) texture->Release();
            break;
          default:
            printf("not implemented yet.");

          }
        }
      }
    }
  }

  return instance;
}
