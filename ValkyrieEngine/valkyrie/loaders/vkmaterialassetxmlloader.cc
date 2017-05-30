
#include <valkyrie/loaders/vkmaterialassetxmlloader.hh>
#include <valkyrie/graphics/vkmaterialdef.hh>
#include <valkyrie/graphics/itexture.hh>
#include <valkyrie/graphics/ishader.hh>
#include <valkyrie/vkengine.hh>



vkMaterialAssetXMLLoader::vkMaterialAssetXMLLoader()
  : vkBaseXMLLoader()
{
}

vkMaterialAssetXMLLoader::~vkMaterialAssetXMLLoader()
{

}


bool vkMaterialAssetXMLLoader::CanLoad(TiXmlElement *element, const vkResourceLocator &locator, iObject *userData) const
{
  vkString tagName(element->Value());

  return tagName == vkString("material");
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

const vkClass *vkMaterialAssetXMLLoader::EvalClass(TiXmlElement *element, const vkResourceLocator &locator, iObject *userData) const
{
  return vkMaterialDef::GetStaticClass();
}


iObject *vkMaterialAssetXMLLoader::Load(TiXmlElement *element, const vkResourceLocator &locator, iObject *userData) const
{
  if (vkString(element->Value()) != vkString("material"))
  {
    return 0;
  }
  
  vkMaterialDef *material = new vkMaterialDef();

  TiXmlElement *passesElement = element->FirstChildElement("passes");
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
    else if (passName == vkString("ForwardUnlit"))
    {
      renderPass = eRP_ForwardUnlit;
    }
    else if (passName == vkString("Forward1Light"))
    {
      renderPass = eRP_Forward1Light;
    }
    else if (passName == vkString("Forward2Light"))
    {
      renderPass = eRP_Forward2Light;
    }
    else if (passName == vkString("Forward3Light"))
    {
      renderPass = eRP_Forward3Light;
    }
    else if (passName == vkString("Forward4Light"))
    {
      renderPass = eRP_Forward4Light;
    }
    else
    {
      material->Release();
      printf("Exit 2\n");
      return 0;
    }

    vkResourceLocator shaderLocator(passElement->GetText());
    iShader *shader = vkResourceManager::Get()->GetOrLoad<iShader>(shaderLocator);
    if (!shader)
    {
      material->Release();
      printf("Exit 3\n");
      return 0;
    }

    material->SetShader(renderPass, shader);
  }



  TiXmlElement *parametersElement = element->FirstChildElement("parameters");
  if (parametersElement)
  {
    for (TiXmlElement *parameterElement = parametersElement->FirstChildElement("parameter");
         parameterElement;
         parameterElement = parameterElement->NextSiblingElement("parameter"))
    {
      vkString type, name;
      if (!parameterElement->Attribute("type") || !parameterElement->Attribute("name"))
      {
        continue;
      }
      type = vkString(parameterElement->Attribute("type"));
      name = vkString(parameterElement->Attribute("name"));
      vkString defaultValue = vkString(parameterElement->GetText());

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

      vkSize idx = material->RegisterParam(name, paramType);
      switch (paramType)
      {
      case eSPT_Float:
        {
          float f = LoadFloat(parameterElement->GetText());
          material->SetDefault(idx, f);
        }
        break;
      case eSPT_Vector2:
        {
          vkVector2f v = LoadVector2f(parameterElement->GetText());
          material->SetDefault(idx, v);
        }
        break;
      case eSPT_Vector3:
        {
          vkVector3f v = LoadVector3f(parameterElement->GetText());
          material->SetDefault(idx, v);
        }
        break;
      case eSPT_Vector4:
        {
          vkVector4f v = LoadVector4f(parameterElement->GetText());
          material->SetDefault(idx, v);
        }
        break;
      case eSPT_Color4:
        {
          vkColor4f v = LoadColor4f(parameterElement->GetText());
          material->SetDefault(idx, v);
        }
        break;
      case eSPT_Texture:
        {
          vkResourceLoadingMode rlm = GetResourceLoadingMode(parameterElement);
          iTexture *texture = 0;
          switch (rlm)
          {
          case eRLM_Shared:
            texture = vkResourceManager::Get()->GetOrLoad<iTexture>(vkResourceLocator(parameterElement->GetText()));
            material->SetDefault(idx, texture);
            break;
          case eRLM_Instance:
            texture = vkResourceManager::Get()->Load<iTexture>(vkResourceLocator(parameterElement->GetText()));
            material->SetDefault(idx, texture);
            if (texture) texture->Release();
            break;
          default:
            printf("not implemented yet.");

          }
        }
      }
    }

  }



  return material;
}






